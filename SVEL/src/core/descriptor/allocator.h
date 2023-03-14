#ifndef __CORE_DESCRIPTOR_ALLOCATOR_H__
#define __CORE_DESCRIPTOR_ALLOCATOR_H__

#include "core/device.h"
#include "svel/config.h"

#include <stack>
#include <vulkan/vulkan.hpp>

#ifndef SVEL_DESCRIPTOR_POOL_SIZE
#define SVEL_DESCRIPTOR_POOL_SIZE 1000
#endif /* SVEL_DESCRIPTOR_POOL_SIZE */

namespace core::descriptor {

class Allocator {
private:
  core::SharedDevice _device;
  std::stack<vk::DescriptorPool> _availablePools;
  std::vector<vk::DescriptorPool> _usedPools;
  std::vector<vk::DescriptorSetLayout>
      _layouts; // To be put in a single class (CACHE)

  // Tune these on demand
  static inline const std::vector<std::pair<vk::DescriptorType, float>>
      _poolSizes = {{vk::DescriptorType::eSampler, 0.5f},
                    {vk::DescriptorType::eCombinedImageSampler, 4.0f},
                    {vk::DescriptorType::eSampledImage, 4.0f},
                    {vk::DescriptorType::eStorageImage, 1.0f},
                    {vk::DescriptorType::eUniformTexelBuffer, 1.0f},
                    {vk::DescriptorType::eStorageTexelBuffer, 1.0f},
                    {vk::DescriptorType::eUniformBuffer, 2.0f},
                    {vk::DescriptorType::eStorageBuffer, 2.0f},
                    {vk::DescriptorType::eUniformBufferDynamic, 1.0f},
                    {vk::DescriptorType::eStorageBufferDynamic, 1.0f},
                    {vk::DescriptorType::eInputAttachment, 0.5f}};

  void _allocatePool();

public:
  Allocator(core::SharedDevice device);
  Allocator(const Allocator &) = default;
  ~Allocator();

  // Opportunity for caching here to reduce number of allocated descriptor sets
  vk::DescriptorSetLayout
  CreateLayout(const std::vector<vk::DescriptorSetLayoutBinding> &bindings);
  vk::DescriptorSet AllocateSet(vk::DescriptorSetLayout &layout);
  void ResetPools();
};
SVEL_CLASS(Allocator)

} // namespace core::descriptor

#endif /* __CORE_DESCRIPTOR_ALLOCATOR_H__ */