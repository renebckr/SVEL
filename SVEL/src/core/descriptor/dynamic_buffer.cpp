/**
 * @file dynamic_buffer.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the dynamic buffer.
 * @date 2023-03-23
 *
 * @copyright Copyright (c) 2023
 *
 */

// Local
#include "dynamic_buffer.h"

using namespace core::descriptor;

DynamicBuffer::DynamicBuffer(std::shared_ptr<core::Device> device,
                             size_t elementSize, vk::DescriptorType type)
    : _device(device), _elementSize(elementSize), _descriptorType(type) {
  // Translate the type to buffer usage using base class
  _usage = _getUsage(type);

  auto minAlignment = _device->GetPhysicalDevice()
                          .getProperties()
                          .limits.minUniformBufferOffsetAlignment;

  // Since alignment has to be a power of 2
  _alignedElementSize = (minAlignment + _elementSize - 1) & ~(minAlignment - 1);
  _bufferSize = _alignedElementSize * SVEL_DESCRIPTOR_DYNAMIC_BUFFER_SIZE;

  // Allocate the first buffer
  _allocateBuffer();
}

DynamicBuffer::~DynamicBuffer() {
  for (const auto &buffer : _buffers)
    _device->AsVulkanObj().unmapMemory(buffer.second->GetMemory());
}

void DynamicBuffer::_allocateBuffer() {
  // Create buffer
  auto buffer = std::make_unique<core::Buffer>(
      _device, _bufferSize, _usage,
      vk::MemoryPropertyFlagBits::eHostVisible |
          vk::MemoryPropertyFlagBits::eHostCoherent);

  // Get memory pointer from the buffer
  void *memoryPointer = _device->AsVulkanObj().mapMemory(
      buffer->GetMemory(), 0, _bufferSize, vk::MemoryMapFlagBits());

  // Update buffers and the info
  _buffers.push_back({memoryPointer, std::move(buffer)});
  _updateBufferInfo((uint32_t)_buffers.size() - 1);
}

void DynamicBuffer::_updateBufferInfo(uint32_t bufferIndex) {
  _bufferInfo = vk::DescriptorBufferInfo(
      _buffers.at(bufferIndex).second->AsVulkanObj(), 0, _alignedElementSize);
}

vk::DescriptorType DynamicBuffer::GetType() const { return _descriptorType; }

DynamicBuffer::WriteResult DynamicBuffer::Write(void *data) {
  // Update buffer offset
  bool requiredAllocation = false;
  uint32_t newBufferIndex =
      (uint32_t)(_head / SVEL_DESCRIPTOR_DYNAMIC_BUFFER_SIZE);
  _bufferOffset = (uint32_t)((_head - (newBufferIndex *
                                       SVEL_DESCRIPTOR_DYNAMIC_BUFFER_SIZE)) *
                             _alignedElementSize);

  // New head position
  _head++;

  // Check if we need a new Buffer
  if (_head >= _buffers.size() * SVEL_DESCRIPTOR_DYNAMIC_BUFFER_SIZE) {
    _allocateBuffer();
    requiredAllocation = true;
  } else if (newBufferIndex != _currentBufferIndex)
    _updateBufferInfo(newBufferIndex);
  _currentBufferIndex = newBufferIndex;

  // Copy Data
  std::memcpy(
      (void *)((char *)_buffers.at(_currentBufferIndex).first + _bufferOffset),
      data, _elementSize);

  if (requiredAllocation)
    return WriteResult::eNeededReallocation;
  return WriteResult::eSuccess;
}

uint32_t DynamicBuffer::GetBufferIndex() { return _currentBufferIndex; }

uint32_t DynamicBuffer::GetBufferOffset() { return _bufferOffset; }

void DynamicBuffer::Reset() { _head = _currentBufferIndex = _bufferOffset = 0; }