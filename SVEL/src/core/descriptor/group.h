#ifndef ENGINE_DESCRIPTOR_GROUP_H
#define ENGINE_DESCRIPTOR_GROUP_H

#include "core/descriptor/allocator.h"
#include "core/descriptor/buffer.h"
#include "core/descriptor/image.hpp"
#include "core/descriptor/queue.h"
#include "core/descriptor/set.h"
#include "core/shader.h"
#include "svel/config.h"

#include <unordered_map>
#include <vulkan/vulkan.hpp>

namespace core::descriptor {

struct QueueDetails {
  UniqueSetQueue queue;
  SharedSet currentSet = nullptr;
};

class WriteHandler {
private:
  SharedSet _set;
  uint32_t _binding;
  SharedIBuffer _buffer;

public:
  WriteHandler(SharedSet set, uint32_t binding);
  void WriteData(void *_data);

  void Update(SharedSet set);
};

class SetGroup {
public:
  using BindingInfo = std::pair<vk::ShaderStageFlags, core::Shader::Binding>;
  using SetDetails = std::vector<BindingInfo>;

private:
  std::vector<vk::DescriptorSetLayout> _layouts;
  SharedAllocator _staticAllocator;
  std::vector<QueueDetails> _queueDetails;
  SetDetails _details;
  std::unordered_map<uint32_t, std::shared_ptr<WriteHandler>> _writeHandlers;

  void _createQueue(std::shared_ptr<core::Device> device, uint32_t copyCount,
                    std::vector<vk::DescriptorSetLayoutBinding> &layoutBindings,
                    std::vector<Set::BindingDetails> &bindingDetails);
  void _grabSets();

public:
  SetGroup(std::shared_ptr<core::Device> device,
           std::vector<core::SharedShader> &shaders,
           unsigned int maxFramesInFlight);
  SetGroup(const SetGroup &) = delete;

  void NotifyNewFrame();
  void Bind(vk::CommandBuffer &commandBuffer, const vk::PipelineLayout &layout);
  const std::vector<vk::DescriptorSetLayout> &GetLayouts();

  std::shared_ptr<WriteHandler> GetWriteHandler(uint32_t setId,
                                                uint32_t binding);
  unsigned int BindTexture(ImageDescriptor *texture, uint32_t setId,
                           uint32_t binding);
  void RebindTexture(unsigned int textureId, uint32_t setId, uint32_t binding);

  const SetDetails &GetInterface() const;
};
SVEL_CLASS(SetGroup)

} // namespace core::descriptor

#endif /* ENGINE_DESCRIPTOR_GROUP_H */