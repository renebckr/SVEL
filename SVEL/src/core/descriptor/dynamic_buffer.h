#ifndef __CORE_DESCRIPTOR_DYNAMIC_BUFFER_H__
#define __CORE_DESCRIPTOR_DYNAMIC_BUFFER_H__

#include "core/descriptor/buffer.h"
#include "core/device.h"
#include "core/memory/buffer.h"

#include <vulkan/vulkan.hpp>

#ifndef SVEL_DESCRIPTOR_DYNAMIC_BUFFER_SIZE
#define SVEL_DESCRIPTOR_DYNAMIC_BUFFER_SIZE 1000
#endif /* SVEL_DESCRIPTOR_DYNAMIC_BUFFER_SIZE */

namespace core::descriptor {

// Do NOT use this in a multi threaded environment. Allocate Buffers for every
// thread!
class DynamicBuffer final : public IBuffer {
private:
  core::SharedDevice _device;
  std::vector<std::pair<void *, core::UniqueBuffer>> _buffers;
  size_t _head, _bufferSize;
  unsigned int _alignment;
  size_t _elementSize, _alignedElementSize;
  vk::DescriptorType _descriptorType;
  vk::BufferUsageFlags _usage;
  uint32_t _currentBufferIndex, _bufferOffset;

  void _allocateBuffer();
  void _updateBufferInfo(uint32_t _bufferIndex);

public:
  DynamicBuffer(std::shared_ptr<core::Device> device, unsigned int alignment,
                size_t elementSize, vk::DescriptorType type);
  ~DynamicBuffer();

  vk::DescriptorType GetType() const override;
  WriteResult Write(void *data) override;
  uint32_t GetBufferIndex() override;
  uint32_t GetBufferOffset() override;
  void Reset() override;
};
} // namespace core::descriptor

#endif /* __CORE_DESCRIPTOR_DYNAMIC_BUFFER_H__ */