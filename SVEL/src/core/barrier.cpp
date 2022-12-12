/**
 * @file barrier.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the Barrier
 * @date 2022-09-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <core/barrier.h>
#include <core/device.h>

#include <limits>
#include <stdexcept>
#include <vulkan/vulkan.hpp>

core::Fence::Fence(SharedDevice device) : _device(device) {
  _vulkanObj = _device->AsVulkanObj().createFence({});
}

core::Fence::~Fence() { _device->AsVulkanObj().destroyFence(_vulkanObj); }

void core::Barrier::_notifyAll() {
  // Iterate through all saved ressources
  for (auto &resource : _resources)
    // Notify all notifiers
    for (auto &notifier : resource.second)
      notifier();

  // Free resources
  _resources.clear();
  _fences.clear();
}

core::Barrier::Barrier(SharedDevice device) : _device(device) {}

core::Barrier::~Barrier() { WaitCompletion(); }

void core::Barrier::AddResource(SharedFence fence,
                                std::vector<Notifier> resources) {
  // Check if already in map
  if (_resources.find(fence) == _resources.end()) {
    _resources[fence] = resources;
    _fences.push_back(fence->AsVulkanObj());
  } else {
    auto &vec = _resources[fence];
    vec.insert(vec.end(), resources.begin(), resources.end());
  }
}

void core::Barrier::AddResource(SharedFence fence, Notifier resource) {
  // Check if already in map
  if (_resources.find(fence) == _resources.end()) {
    _resources[fence] = {};
    _fences.push_back(fence->AsVulkanObj());
  }
  _resources[fence].push_back(resource);
}

bool core::Barrier::IsCompleted() {
  // Check if no fences exist
  if (_fences.size() == 0)
    return true;

  // Check with instant timeout
  auto result = _device->AsVulkanObj().waitForFences(_fences, VK_TRUE, 0);
  switch (result) {
  case vk::Result::eSuccess:
    _notifyAll();
    return true;
  case vk::Result::eTimeout:
    return false;
  default:
    throw std::runtime_error(vk::to_string(result));
  }
}

void core::Barrier::WaitCompletion() {
  // Check if fences exist
  if (_fences.size() == 0)
    return;

  // Wait until all fences are completed
  auto result = _device->AsVulkanObj().waitForFences(
      _fences, VK_TRUE, std::numeric_limits<uint64_t>::max());
  if (result != vk::Result::eSuccess)
    throw std::runtime_error(vk::to_string(result));
  _notifyAll();
}