/**
 * @file instance.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Vulkan Instance wrapper implementation
 * @date 2022-10-02
 *
 * @copyright Copyright (c) 2022
 *
 */

// Local
#include "instance.h"

// GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Vulkan
#include <vulkan/vulkan_core.h>

// STL
#include <algorithm>
#include <stdexcept>

using namespace core;

void core::Instance::_setupInstanceValidationLayers() {
  // Get layer properties
  auto props = vk::enumerateInstanceLayerProperties();
  for (unsigned int i = 0; i < props.size(); i++) {
    // Check for layers that we want
    auto prop = props.at(i);
    std::string name = prop.layerName.data();
    if (name.find("VK_LAYER_KHRONOS_validation") != std::string::npos)
      _layers.push_back("VK_LAYER_KHRONOS_validation");
  }
}

core::Instance::Instance(const std::string &appName, uint32_t appVersion,
                         const std::string &engineName, uint32_t engineVersion)
    : _appName(appName), _engineName(engineName), _appVersion(appVersion),
      _engineVersion(engineVersion) {
  // Fetch extensions required for glfw
  uint32_t glfwExtensionCount = 0;
  auto extensionArray = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  if (extensionArray == nullptr)
    throw std::runtime_error("Extensions for window creation not available.");

  // Copy extensions
  _extensions.reserve(glfwExtensionCount);
  _extensions.insert(_extensions.end(), &extensionArray[0],
                     &extensionArray[glfwExtensionCount]);

  // When debugging we setup validation layers
#ifndef NDEBUG
  _setupInstanceValidationLayers();
#endif

  // Setup instance
  vk::ApplicationInfo appInfo(_appName.c_str(), _appVersion,
                              _engineName.c_str(), _engineVersion);

  vk::InstanceCreateInfo instanceInfo(vk::InstanceCreateFlagBits(), &appInfo,
                                      _layers, _extensions);
  _vulkanObj = vk::createInstance(instanceInfo);
}

core::Instance::~Instance() { _vulkanObj.destroy(); }