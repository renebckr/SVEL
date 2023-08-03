/**
 * @file buffer.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Basic Buffer Implementation
 * @date 2022-09-24
 *
 * @copyright Copyright (c) 2022
 */

// Local
#include "buffer.h"

// Internal
#include <core/device.h>

// Vulkan
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_structs.hpp>

// STL
#include <limits>
#include <stdexcept>

core::Buffer::Buffer(core::SharedDevice device, size_t size,
                     vk::BufferUsageFlags usage,
                     vk::MemoryPropertyFlags memFlags)
    : _device(device), _size(size) {
  auto vulkanDevice = device->AsVulkanObj();

  // Create Vulkan Buffer
  vk::BufferCreateInfo createInfo(vk::BufferCreateFlagBits(), size, usage,
                                  vk::SharingMode::eExclusive, 0, nullptr);
  _vulkanObj = vulkanDevice.createBuffer(createInfo);

  // Get Requirements and try allocating memory
  auto memoryRequirements =
      vulkanDevice.getBufferMemoryRequirements(_vulkanObj);
  _memory = std::make_unique<core::DeviceMemory>(device, memoryRequirements,
                                                 memFlags);
  vulkanDevice.bindBufferMemory(_vulkanObj, _memory->AsVulkanObj(), 0);
}

core::Buffer::~Buffer() { _device->AsVulkanObj().destroyBuffer(_vulkanObj); }

const vk::DeviceMemory &core::Buffer::GetMemory() {
  return _memory->AsVulkanObj();
}
