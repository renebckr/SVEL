/**
 * @file buffer.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the buffer interface.
 * @date 2023-03-20
 *
 * @copyright Copyright (c) 2023
 *
 */

// Local
#include "buffer.h"

using namespace core::descriptor;

vk::BufferUsageFlags IBuffer::_getUsage(vk::DescriptorType _descriptorType) {
  // Check type and return appropriate usage flags
  switch (_descriptorType) {
  case vk::DescriptorType::eUniformBuffer:
    return vk::BufferUsageFlagBits::eUniformBuffer;
  case vk::DescriptorType::eUniformBufferDynamic:
    return vk::BufferUsageFlagBits::eUniformBuffer;
  default:
    break;
  }

  // Throw for unsupported types
  throw std::runtime_error("Unsupported Descriptor Type");
}

IBuffer::~IBuffer() {}

vk::DescriptorType IBuffer::GetType() const {
  // Concrete implementation should return their type here
  return vk::DescriptorType();
}

const vk::DescriptorBufferInfo &IBuffer::GetBufferInfo() const {
  return _bufferInfo;
}

IBuffer::WriteResult IBuffer::Write(void *) {
  // The interface does not support writing
  return WriteResult::eFailure;
};

uint32_t IBuffer::GetBufferIndex() {
  // Return default value
  return 0;
}

uint32_t IBuffer::GetBufferOffset() {
  // Return default value
  return 0;
}

void IBuffer::Reset() {
  // Implementation specific
}
