#include "buffer_interface.h"

using namespace core::descriptor;

vk::BufferUsageFlags
BufferInterface::_getUsage(vk::DescriptorType _descriptorType) {
  switch (_descriptorType) {
  case vk::DescriptorType::eUniformBuffer:
    return vk::BufferUsageFlagBits::eUniformBuffer;
  case vk::DescriptorType::eUniformBufferDynamic:
    return vk::BufferUsageFlagBits::eUniformBuffer;
  default:
    break;
  }
  throw std::runtime_error("Unsupported Descriptor Type");
}

BufferInterface::~BufferInterface() {}

vk::DescriptorType BufferInterface::GetType() const {
  return vk::DescriptorType();
}

const vk::DescriptorBufferInfo &BufferInterface::GetBufferInfo() const {
  return _bufferInfo;
}

BufferInterface::WriteInfo BufferInterface::Write(void *) {
  return WriteInfo::eFailure;
};

uint32_t BufferInterface::GetBufferIndex() { return 0; }

uint32_t BufferInterface::GetBufferOffset() { return 0; }

void BufferInterface::Reset() {}
