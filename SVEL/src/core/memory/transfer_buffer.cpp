/**
 * @file transfer_buffer.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the TransferBuffer.
 * @date 2023-03-23
 *
 * @copyright Copyright (c) 2023
 *
 */

// Local
#include "transfer_buffer.h"
#include "buffer.h"

// Internal
#include <core/barrier.h>

// STL
#include <memory>

void core::TransferBuffer::onTransferCompleted() {
  // Free resources and run callback
  _device->AsVulkanObj().freeCommandBuffers(_commandPool, _commandBuffer);
  _completionCallback(_transferredBuffer);
}

core::TransferBuffer::TransferBuffer(
    SharedDevice device, vk::CommandPool commandPool,
    const SVEL_NAMESPACE::ArrayProxy &data, vk::BufferUsageFlags _usage,
    TransferCompletionHandler completionCallback)
    : _device(device), _commandPool(commandPool), _bufferSize(data.dataSize),
      _completionCallback(completionCallback) {
  auto vulkanDevice = device->AsVulkanObj();

  // Create staging buffer
  _stagingBuffer = std::make_unique<Buffer>(
      device, _bufferSize, vk::BufferUsageFlagBits::eTransferSrc,
      vk::MemoryPropertyFlagBits::eHostVisible |
          vk::MemoryPropertyFlagBits::eHostCoherent);

  // Create Buffer
  _transferredBuffer = std::make_shared<Buffer>(
      device, _bufferSize, vk::BufferUsageFlagBits::eTransferDst | _usage,
      vk::MemoryPropertyFlagBits::eDeviceLocal);

  // Write to staging Buffer
  auto rawPointer = vulkanDevice.mapMemory(
      _stagingBuffer->GetMemory(), 0, _bufferSize, vk::MemoryMapFlagBits());
  std::memcpy(rawPointer, data.data, _bufferSize);
  vulkanDevice.unmapMemory(_stagingBuffer->GetMemory());
}

void core::TransferBuffer::TransferData(Barrier *barrier) {
  auto fence = std::make_shared<core::Fence>(_device);

  // Transfer Data
  vk::CommandBufferAllocateInfo commandBufferInfo(
      _commandPool, vk::CommandBufferLevel::ePrimary, 1);
  _commandBuffer =
      _device->AsVulkanObj().allocateCommandBuffers(commandBufferInfo)[0];

  // Record to buffer
  vk::CommandBufferBeginInfo beginInfo(
      vk::CommandBufferUsageFlagBits::eOneTimeSubmit, nullptr);

  _commandBuffer.begin(beginInfo);
  _commandBuffer.copyBuffer(_stagingBuffer->AsVulkanObj(),
                            _transferredBuffer->AsVulkanObj(),
                            vk::BufferCopy(0, 0, _bufferSize));
  _commandBuffer.end();

  // Submit to graphics queue
  vk::SubmitInfo submitInfo(0, nullptr, nullptr, 1, &_commandBuffer, 0,
                            nullptr);

  auto queue =
      _device->AsVulkanObj().getQueue(_device->GetGraphicsQueueFamily(), 0);
  auto result = queue.submit(1, &submitInfo, fence->AsVulkanObj());
  if (result != vk::Result::eSuccess)
    throw std::runtime_error(vk::to_string(result));

  // Create Notifier
  barrier->AddResource(fence, std::bind(&TransferBuffer::onTransferCompleted,
                                        this->shared_from_this()));
}