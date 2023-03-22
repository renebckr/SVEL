#include "static_buffer.h"

using namespace core::descriptor;

StaticBuffer::StaticBuffer(core::SharedDevice device,
                           vk::DescriptorType descriptorType, size_t size)
    : _device(device), _size(size), _descriptorType(descriptorType) {

  _buffer = std::make_unique<core::Buffer>(
      _device, size, _getUsage(_descriptorType),
      vk::MemoryPropertyFlagBits::eHostVisible |
          vk::MemoryPropertyFlagBits::eHostCoherent);
  _memoryPointer = device->AsVulkanObj().mapMemory(
      _buffer->GetMemory(), 0, size, vk::MemoryMapFlagBits());
  _bufferInfo =
      vk::DescriptorBufferInfo(_buffer->AsVulkanObj(), vk::DeviceSize(0), size);
}

StaticBuffer::~StaticBuffer() {
  _device->AsVulkanObj().unmapMemory(_buffer->GetMemory());
}

vk::DescriptorType StaticBuffer::GetType() const { return _descriptorType; }

StaticBuffer::WriteResult StaticBuffer::Write(void *_data) {
  std::memcpy(_memoryPointer, _data, _size);
  return WriteResult::eSuccess;
}

void StaticBuffer::Reset() {}
