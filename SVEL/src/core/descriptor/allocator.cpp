#include "allocator.h"

#include <stdexcept>

using namespace core::descriptor;

void Allocator::_allocatePool() {
  std::vector<vk::DescriptorPoolSize> sizes;
  for (const auto &size : _poolSizes)
    sizes.push_back(vk::DescriptorPoolSize(
        size.first, (uint32_t)(size.second * SVEL_DESCRIPTOR_POOL_SIZE)));

  vk::DescriptorPoolCreateInfo createInfo(vk::DescriptorPoolCreateFlagBits(),
                                          SVEL_DESCRIPTOR_POOL_SIZE, sizes);

  _availablePools.push(_device->AsVulkanObj().createDescriptorPool(createInfo));
}

Allocator::Allocator(core::SharedDevice device) : _device(device) {
  _allocatePool();
}

Allocator::~Allocator() {
  ResetPools();

  for (const auto &layout : _layouts)
    _device->AsVulkanObj().destroyDescriptorSetLayout(layout);

  while (!_availablePools.empty()) {
    _device->AsVulkanObj().destroyDescriptorPool(_availablePools.top());
    _availablePools.pop();
  }
}

vk::DescriptorSetLayout Allocator::CreateLayout(
    const std::vector<vk::DescriptorSetLayoutBinding> &bindings) {
  vk::DescriptorSetLayoutCreateInfo createInfo(
      vk::DescriptorSetLayoutCreateFlagBits(), bindings);
  auto layout = _device->AsVulkanObj().createDescriptorSetLayout(createInfo);
  _layouts.push_back(layout);

  return layout;
}

vk::DescriptorSet Allocator::AllocateSet(vk::DescriptorSetLayout &layout) {
  if (_availablePools.empty())
    _allocatePool();

  auto pool = _availablePools.top();

  vk::DescriptorSetAllocateInfo allocateInfo(pool, layout);

  vk::DescriptorSet set;
  try {
    set = _device->AsVulkanObj().allocateDescriptorSets(allocateInfo).front();
  } catch (vk::SystemError &err) {
    // Try allocating new pool - If this fails -> escalates exception
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
  _device->AsVulkanObj().resetDescriptorPool(_availablePools.top());
  for (const auto &pool : _usedPools) {
    _device->AsVulkanObj().resetDescriptorPool(pool);
    _availablePools.push(pool);
  }
  _usedPools.clear();
}
