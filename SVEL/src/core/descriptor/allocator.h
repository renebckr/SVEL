/**
 * @file allocator.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declaration of descriptor allocator.
 * @date 2023-03-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __CORE_DESCRIPTOR_ALLOCATOR_H__
#define __CORE_DESCRIPTOR_ALLOCATOR_H__

// Internal
#include <core/device.h>
#include <svel/config.h>

// Vulkan
#include <vulkan/vulkan.hpp>

// STL
#include <stack>

/**
 * @brief Which factor to use for pool sizes.
 */
#ifndef SVEL_DESCRIPTOR_POOL_SIZE
#define SVEL_DESCRIPTOR_POOL_SIZE 1000
#endif /* SVEL_DESCRIPTOR_POOL_SIZE */

namespace core::descriptor {

/**
 * @brief Allocator used to allocate descriptor set layouts and descriptor sets.
 */
class Allocator {
private:
  /**
   * @brief Vulkan Device to use.
   */
  core::SharedDevice _device;

  /**
   * @brief All the available pools.
   */
  std::stack<vk::DescriptorPool> _availablePools;

  /**
   * @brief Pools that are used up.
   */
  std::vector<vk::DescriptorPool> _usedPools;

  /**
   * @brief All created layouts. THIS SHOULD BE HANDLED IN ANOTHER CLASS.
   */
  std::vector<vk::DescriptorSetLayout> _layouts;

  /**
   * @brief Descriptor pool size factors. These are generally well tuned.
   */
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

  /**
   * @brief Allocate a new descriptor pool.
   */
  void _allocatePool();

public:
  /**
   * @brief Construct a new allocator.
   *
   * @param device Device to use for the allocation.
   */
  Allocator(core::SharedDevice device);

  /**
   * @brief Destroy the allocator.
   */
  ~Allocator();

  /**
   * @brief Create a new descriptor set layout with the specified bindings.
   * POSSIBILITY OF CACHING HERE.
   *
   * @param bindings Descriptor set bindings.
   * @return vk::DescriptorSetLayout The created layout.
   */
  vk::DescriptorSetLayout
  CreateLayout(const std::vector<vk::DescriptorSetLayoutBinding> &bindings);

  /**
   * @brief Allocates a new descriptor set for the given layout.
   *
   * @param layout Layout to use for the allocation of the set.
   * @return vk::DescriptorSet The allocated descriptor set.
   */
  vk::DescriptorSet AllocateSet(vk::DescriptorSetLayout &layout);

  /**
   * @brief Resets all descriptor pools.
   */
  void ResetPools();
};
SVEL_CLASS(Allocator)

} // namespace core::descriptor

#endif /* __CORE_DESCRIPTOR_ALLOCATOR_H__ */