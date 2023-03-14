#include "dynamic_buffer.h"

using namespace core::descriptor;

DynamicDescriptorBuffer::DynamicDescriptorBuffer(
    std::shared_ptr<core::Device> device, unsigned int alignment,
    size_t elementSize, vk::DescriptorType type)
    : _device(device), _alignment(alignment), _elementSize(elementSize),
      _descriptorType(type) {
  _usage = _getUsage(type);
  _head = _currentBufferIndex = _bufferOffset = 0;

  // Since alignment has to be a power of 2
  _alignedElementSize = (_alignment + _elementSize - 1) & ~(_alignment - 1);
  _bufferSize = _alignedElementSize * SVEL_DESCRIPTOR_DYNAMIC_BUFFER_SIZE;
  _allocateBuffer();
}

DynamicDescriptorBuffer::~DynamicDescriptorBuffer() {
  for (const auto &buffer : _buffers) {
    _device->AsVulkanObj().unmapMemory(buffer.second->GetMemory());
  }
}

void DynamicDescriptorBuffer::_allocateBuffer() {
  auto buffer = std::make_unique<core::Buffer>(
      _device, _bufferSize, _usage,
      vk::MemoryPropertyFlagBits::eHostVisible |
          vk::MemoryPropertyFlagBits::eHostCoherent);
  void *memoryPointer = _device->AsVulkanObj().mapMemory(
      buffer->GetMemory(), 0, _bufferSize, vk::MemoryMapFlagBits());
  _buffers.push_back({memoryPointer, std::move(buffer)});

  // Update info
  _updateBufferInfo((uint32_t)_buffers.size() - 1);
}

void DynamicDescriptorBuffer::_updateBufferInfo(uint32_t bufferIndex) {
  _bufferInfo = vk::DescriptorBufferInfo(
      _buffers.at(bufferIndex).second->AsVulkanObj(), 0, _alignedElementSize);
}

vk::DescriptorType DynamicDescriptorBuffer::GetType() const {
  return _descriptorType;
}

DynamicDescriptorBuffer::WriteInfo DynamicDescriptorBuffer::Write(void *data) {
  bool requiredAllocation = false;
  uint32_t newBufferIndex =
      (uint32_t)(_head / SVEL_DESCRIPTOR_DYNAMIC_BUFFER_SIZE);
  _bufferOffset = (uint32_t)((_head - (newBufferIndex *
                                       SVEL_DESCRIPTOR_DYNAMIC_BUFFER_SIZE)) *
                             _alignedElementSize);

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
    return WriteInfo::eNeededReallocation;
  return WriteInfo::eSuccess;
}

uint32_t DynamicDescriptorBuffer::GetBufferIndex() {
  return _currentBufferIndex;
}

uint32_t DynamicDescriptorBuffer::GetBufferOffset() { return _bufferOffset; }

void DynamicDescriptorBuffer::Reset() {
  _head = _currentBufferIndex = _bufferOffset = 0;
}