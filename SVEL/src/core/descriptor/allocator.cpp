/**
 * @file allocator.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the Allocator.
 * @date 2023-03-20
 *
 * @copyright Copyright (c) 2023
 *
 */

// Local
#include "allocator.h"

// STL
#include <stdexcept>

using namespace core::descriptor;

void Allocator::_allocatePool() {
  // Calculate pool size
  std::vector<vk::DescriptorPoolSize> sizes;
  for (const auto &size : _poolSizes)
    sizes.push_back(vk::DescriptorPoolSize(
        size.first, (uint32_t)(size.second * SVEL_DESCRIPTOR_POOL_SIZE)));

  // Create pool
  vk::DescriptorPoolCreateInfo createInfo(
      vk::DescriptorPoolCreateFlagBits::
          eFreeDescriptorSet, // Use this for very cheap allocation of
                              // descriptor sets
      SVEL_DESCRIPTOR_POOL_SIZE, sizes);

  // Add to available pools
  _availablePools.push(_device->AsVulkanObj().createDescriptorPool(createInfo));
}

Allocator::Allocator(core::SharedDevice device) : _device(device) {
  _allocatePool();
}

Allocator::~Allocator() {
  ResetPools();

  // Destroy all layouts
  for (const auto &layout : _layouts)
    _device->AsVulkanObj().destroyDescriptorSetLayout(layout);

  // Destroy all pools
  while (!_availablePools.empty()) {
    _device->AsVulkanObj().destroyDescriptorPool(_availablePools.top());
    _availablePools.pop();
  }
}

vk::DescriptorSetLayout Allocator::CreateLayout(
    const std::vector<vk::DescriptorSetLayoutBinding> &bindings) {
  // Create layout directly
  vk::DescriptorSetLayoutCreateInfo createInfo(
      vk::DescriptorSetLayoutCreateFlagBits(), bindings);
  auto layout = _device->AsVulkanObj().createDescriptorSetLayout(createInfo);
  _layouts.push_back(layout);

  return layout;
}

vk::DescriptorSet Allocator::AllocateSet(vk::DescriptorSetLayout &layout) {
  // Check if we need a new pool
  if (_availablePools.empty())
    _allocatePool();

  // Get pool to use
  auto pool = _availablePools.top();

  vk::DescriptorSetAllocateInfo allocateInfo(pool, layout);

  // Try to allocate the descriptor set
  vk::DescriptorSet set;
  try {
    set = _device->AsVulkanObj().allocateDescriptorSets(allocateInfo).front();
  } catch (vk::SystemError &err) {
    // Try allocating new pool - If this fails -> escalates exception
    // This error is usually caused by the pool being full
    _usedPools.push_back(pool);
    _availablePools.pop();
    if (_availablePools.empty())
      _allocatePool();
    allocateInfo.setDescriptorPool(_availablePools.top());
    set = _device->AsVulkanObj().allocateDescriptorSets(allocateInfo).front();
  }
  return set;
}

void Allocator::ResetPools() {
  // Reset the pools and make them available again.
  // Keep the amount of pools the same as they can simply be reused.
  _device->AsVulkanObj().resetDescriptorPool(_availablePools.top());
  for (const auto &pool : _usedPools) {
    _device->AsVulkanObj().resetDescriptorPool(pool);
    _availablePools.push(pool);
  }
  _usedPools.clear();
}
