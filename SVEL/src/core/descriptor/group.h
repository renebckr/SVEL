#ifndef ENGINE_DESCRIPTOR_GROUP_H
#define ENGINE_DESCRIPTOR_GROUP_H

#include "core/descriptor/allocator.h"
#include "core/descriptor/buffer_interface.h"
#include "core/descriptor/queue.h"
#include "core/descriptor/set.h"
#include "core/descriptor/textureInterface.hpp"
#include "core/shader.h"

#include <vulkan/vulkan.hpp>

namespace core::descriptor {

class WriteHandler {
private:
  SharedSet _set;
  uint32_t _binding;
  SharedBufferInterface _buffer;

public:
  WriteHandler(SharedSet set, uint32_t binding);
  void WriteData(void *_data);
};

class SetGroup {
private:
  using BindingInfo = std::pair<vk::ShaderStageFlags, core::Shader::Binding>;
  using SetDetails = std::vector<BindingInfo>;

  struct QueueDetails {
    UniqueSetQueue queue;
    SharedSet currentSet = nullptr;
  };

  std::vector<vk::DescriptorSetLayout> _layouts;
  SharedAllocator _staticAllocator;
  std::vector<QueueDetails> _queueDetails;

  void _createQueue(std::shared_ptr<core::Device> device, uint32_t copyCount,
                    std::vector<vk::DescriptorSetLayoutBinding> &layoutBindings,
                    std::vector<BindingDetails> &bindingDetails);
  void _grabSets();

public:
  SetGroup(std::shared_ptr<core::Device> device,
           std::vector<core::SharedShader> &shaders,
           unsigned int maxFramesInFlight);
  SetGroup(const SetGroup &) = delete;

  void NotifyNewFrame();
  void Bind(vk::CommandBuffer &commandBuffer, const vk::PipelineLayout &layout);
  const std::vector<vk::DescriptorSetLayout> &GetLayouts();

  WriteHandler GetBuffer(uint32_t setId, uint32_t binding);
  unsigned int BindTexture(ImageDescriptor *texture, uint32_t setId,
                           uint32_t binding);
  void RebindTexture(unsigned int textureId, uint32_t setId, uint32_t binding);
};

} // namespace core::descriptor

#endif /* ENGINE_DESCRIPTOR_GROUP_H */