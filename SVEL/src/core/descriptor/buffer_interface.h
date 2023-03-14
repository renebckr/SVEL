#ifndef __CORE_DESCRIPTOR_BUFFER_INTERFACE_H__
#define __CORE_DESCRIPTOR_BUFFER_INTERFACE_H__

#include <vulkan/vulkan.hpp>

namespace core::descriptor {

class BufferInterface {
public:
  enum class WriteInfo { eSuccess, eFailure, eNeededReallocation };

protected:
  vk::DescriptorBufferInfo _bufferInfo;

  vk::BufferUsageFlags _getUsage(vk::DescriptorType _descriptorType);

public:
  virtual ~BufferInterface();

  virtual vk::DescriptorType GetType() const;
  virtual WriteInfo Write(void *_data);
  virtual uint32_t GetBufferIndex();
  virtual uint32_t GetBufferOffset();
  virtual void Reset();

  const vk::DescriptorBufferInfo &GetBufferInfo() const;
};

} // namespace core::descriptor

#endif /* __CORE_DESCRIPTOR_BUFFER_INTERFACE_H__ */