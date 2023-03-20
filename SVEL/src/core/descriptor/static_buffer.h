#ifndef __CORE_DESCRIPTOR_STATIC_BUFFER_H__
#define __CORE_DESCRIPTOR_STATIC_BUFFER_H__

#include "buffer_interface.h"
#include "core/device.h"
#include "core/memory/buffer.h"

namespace core::descriptor {

class StaticBuffer final : public BufferInterface {
private:
  core::SharedDevice _device;
  core::UniqueBuffer _buffer;
  size_t _size;
  vk::DescriptorType _descriptorType;
  void *_memoryPointer;

public:
  StaticBuffer(core::SharedDevice device, vk::DescriptorType descriptorType,
               size_t size);
  ~StaticBuffer();

  vk::DescriptorType GetType() const override;
  WriteInfo Write(void *_data) override;
  void Reset() override;
};

} // namespace core::descriptor

#endif /* __CORE_DESCRIPTOR_STATIC_BUFFER_H__ */