#ifndef __RENDERER_MATERIAL_MATERIAL_H__
#define __RENDERER_MATERIAL_MATERIAL_H__

#include "core/descriptor/group.h"
#include "renderer/pipeline/pipeline.h"
#include <svel/config.h>
#include <svel/detail/material.h>
#include <unordered_map>
#include <unordered_set>

namespace SVEL_NAMESPACE {

class IMaterial::Impl {
private:
  struct Attribute {
    void *data;
    size_t dataSize;
    std::shared_ptr<core::descriptor::WriteHandler> writeHandler;
  };

  renderer::SharedVulkanPipeline _pipeline;
  std::unordered_set<vk::DescriptorType> _enabledTypes;

  std::unordered_map<uint32_t, std::pair<vk::DescriptorType, uint32_t>>
      _slotTypes;
  std::unordered_map<uint32_t, Attribute> _attributes;
  std::unordered_map<uint32_t, SharedTexture> _textures;

public:
  Impl(renderer::SharedVulkanPipeline pipeline);

  inline bool AddAttribute(uint32_t setId, uint32_t binding, void *data,
                           size_t dataSize);

  void SetAvailableAttributeType(
      const std::unordered_set<vk::DescriptorType> &enabledTypes);

  void WriteAttributes();

  inline bool SetTexture(unsigned int set, unsigned int binding,
                         SharedTexture texture);
};

} // namespace SVEL_NAMESPACE

#endif /* __RENDERER_MATERIAL_MATERIAL_H__ */