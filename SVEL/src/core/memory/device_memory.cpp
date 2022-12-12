/**
 * @file device_memory.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the DeviceMemory Wrapper
 * @date 2022-10-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <core/memory/device_memory.h>

uint32_t
core::DeviceMemory::_findMemoryType(vk::MemoryPropertyFlags memoryFlags,
                                    uint32_t memoryTypeBits) {
  // Fetch memory properties
  auto memoryProps = _device->GetPhysicalDevice().getMemoryProperties();

  // Iterate through memory properties to find valid memory type
  for (unsigned int i = 0; i < memoryProps.memoryTypeCount; i++)
    if ((memoryTypeBits & (1 << i)) &&
        (memoryProps.memoryTypes[i].propertyFlags & memoryFlags))
      return i;
  throw std::runtime_error("Could not find proper memory type.");
}

core::DeviceMemory::DeviceMemory(core::SharedDevice device,
                                 vk::MemoryRequirements &requirements,
                                 vk::MemoryPropertyFlags properties)
    : _device(device) {
  // Allocate vulkan memory
  auto vulkanDevice = device->AsVulkanObj();
  vk::MemoryAllocateInfo allocateInfo(
      requirements.size,
      _findMemoryType(properties, requirements.memoryTypeBits));
  _vulkanObj = vulkanDevice.allocateMemory(allocateInfo);
}

core::DeviceMemory::~DeviceMemory() {
  _device->AsVulkanObj().freeMemory(_vulkanObj);
}