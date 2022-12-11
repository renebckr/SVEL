#include "app.h"
#include <algorithm>
#include <memory>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vulkan/vulkan_core.h>

using namespace SVEL_NAMESPACE;

// --- IMPL ---

void IApplication::Impl::setupInstanceValidationLayers() {
  // Get layer properties
  auto props = vk::enumerateInstanceLayerProperties();
  for (unsigned int i = 0; i < props.size(); i++) {
    // Check for layers that we want
    auto prop = props.at(i);
    std::string name = prop.layerName.data();
    if (name.find("VK_LAYER_KHRONOS_validation") != std::string::npos)
      layers.push_back("VK_LAYER_KHRONOS_validation");
  }
}

IApplication::Impl::Impl(const std::string &_appName, uint32_t _appVersion,
                         const std::string &_engineName,
                         uint32_t _engineVersion)
    : appName(_appName), engineName(_engineName), appVersion(_appVersion),
      engineVersion(_engineVersion) {
  // Fetch extensions required for glfw
  uint32_t glfwExtensionCount = 0;
  auto extensionArray = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  if (extensionArray == nullptr)
    throw std::runtime_error("Extensions for window creation not available.");

  // Copy extensions
  extensions.reserve(glfwExtensionCount);
  extensions.insert(extensions.end(), &extensionArray[0],
                    &extensionArray[glfwExtensionCount]);

  // When debugging we setup validation layers
#ifndef NDEBUG
  setupInstanceValidationLayers();
#endif

  // Setup instance
  vk::ApplicationInfo appInfo(appName.c_str(), appVersion, engineName.c_str(),
                              engineVersion);

  vk::InstanceCreateInfo instanceInfo(vk::InstanceCreateFlagBits(), &appInfo,
                                      layers, extensions);
  _vulkanObj = vk::createInstance(instanceInfo);
}

IApplication::Impl::~Impl() { _vulkanObj.destroy(); }

// --- INTERFACE ---

IApplication::IApplication(const std::string &appName, uint32_t appVersion) {
  __pImpl = std::make_shared<Impl>(appName, appVersion, "SVEL", 1);
}