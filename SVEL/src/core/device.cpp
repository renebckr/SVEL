/**
 * @file device.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the vulkan device wrapper
 * @date 2022-10-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "device.h"

#include <impl/app.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <util/prioritized_order.hpp>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>

std::vector<unsigned int> core::Device::prioritizePhysicalDevices(
    const std::vector<vk::PhysicalDevice> &devices) {
  // Preference
  std::vector<vk::PhysicalDeviceType> priorities = {
      vk::PhysicalDeviceType::eDiscreteGpu,
      vk::PhysicalDeviceType::eIntegratedGpu,
      vk::PhysicalDeviceType::eVirtualGpu, vk::PhysicalDeviceType::eCpu};

  if (devices.size() == 0)
    return {};

  std::vector<std::pair<uint32_t, vk::PhysicalDeviceType>> typeList;
  for (uint32_t i = 0; i < devices.size(); i++) {
    // Check for Extensions
    bool allExtensionsSupported = true;
    auto supportedExtensions =
        devices.at(i).enumerateDeviceExtensionProperties();
    for (auto str : _extensions) {
      // TODO: Optimize
      bool extensionSupported = false;
      for (auto extension : supportedExtensions) {
        if (std::strcmp(extension.extensionName, str) == 0) {
          extensionSupported = true;
          break;
        }
      }
      if (!extensionSupported) {
        allExtensionsSupported = false;
        break;
      }
    }

    if (allExtensionsSupported)
      typeList.push_back({i, devices.at(i).getProperties().deviceType});
  }

  return util::PriorityOrder(typeList, priorities);
}

std::pair<unsigned int, unsigned int>
core::Device::findQueueFamilies(vk::PhysicalDevice device,
                                uint32_t &constraintQueueCount) {
  std::vector<unsigned int> graphicQueueFamilies;
  // Find Graphic Queue Families
  auto props = device.getQueueFamilyProperties();
  for (unsigned int i = 0; i < props.size(); i++) {
    auto prop = props.at(i);
    if ((prop.queueFlags | vk::QueueFlagBits::eGraphics) == prop.queueFlags)
      graphicQueueFamilies.push_back(i);
  }

  // Prioritize highest Queue Count
  std::sort(graphicQueueFamilies.begin(), graphicQueueFamilies.end(),
            [props](const unsigned int &a, const unsigned int &b) {
              return props.at(a).queueCount > props.at(b).queueCount;
            });

  // Check if they are able to present
  for (auto queueFamily : graphicQueueFamilies) {
    if (device.getSurfaceSupportKHR(queueFamily, _surface->AsVulkanObj())) {
      constraintQueueCount = props.at(queueFamily).queueCount;
      return {queueFamily, queueFamily};
    }
  }

  // No Graphics Queue Family able to present -> Look for other Queues that may
  // present
  for (unsigned int i = 0; i < props.size(); i++) {
    auto prop = props.at(i);
    if ((prop.queueFlags | vk::QueueFlagBits::eGraphics) != prop.queueFlags &&
        device.getSurfaceSupportKHR(i, _surface->AsVulkanObj())) {
      constraintQueueCount = props.at(graphicQueueFamilies.front()).queueCount;
      return {graphicQueueFamilies.front(), i};
    }
  }

  constraintQueueCount = 0;
  return {};
}

core::Device::Device(sv::SharedIApplication parent, core::SharedSurface surface)
    : _parentApp(parent), _surface(surface) {
  // Append Extensions
  _extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

  // Get prioritized physical devices
  auto physicalDevices =
      _parentApp->__getImpl()->AsVulkanObj().enumeratePhysicalDevices();
  auto deviceIndices = prioritizePhysicalDevices(physicalDevices);
  if (deviceIndices.size() == 0)
    throw std::runtime_error("Could not get prioritized Physical Devices.");

  // Get first valid QueueFamilies from physical devices
  bool physicalDeviceFound = false;
  for (auto deviceIndex : deviceIndices) {
    // Find Queue Family that has at least constraintQueueCount many queues
    unsigned int constraintQueueCount = 1;
    auto [graphicsQueue, presentQueue] = findQueueFamilies(
        physicalDevices.at(deviceIndex), constraintQueueCount);
    if (constraintQueueCount >= 1) {
      _selectedPhysicalDevice = physicalDevices.at(deviceIndex);
      _queueFamilyGraphics = graphicsQueue;
      _queueFamilyPresent = presentQueue;
      _queueCount = constraintQueueCount;
      physicalDeviceFound = true;
      break;
    }
  }
  if (!physicalDeviceFound)
    throw std::runtime_error("No Physical Device fits Queue constraints.");

  // Setup Logical Device
  _queuePriorities = std::vector<float>(_queueCount, 1.0f);
  vk::DeviceQueueCreateInfo deviceQueueInfo(vk::DeviceQueueCreateFlagBits(),
                                            _queueFamilyGraphics, _queueCount,
                                            &_queuePriorities[0]);
  std::vector<vk::DeviceQueueCreateInfo> deviceQueueInfos = {deviceQueueInfo};
  vk::DeviceCreateInfo deviceInfo(vk::DeviceCreateFlagBits(), deviceQueueInfos,
                                  {}, _extensions, &_features);
  _vulkanObj = _selectedPhysicalDevice.createDevice(deviceInfo);
}

core::Device::~Device() { _vulkanObj.destroy(); }