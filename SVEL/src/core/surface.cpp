/**
 * @file surface.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Surface vulkan wrapper implementation
 * @date 2022-10-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "surface.h"
#include "window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vulkan/vulkan.hpp>

core::Surface::Surface(core::SharedInstance instance,
                       core::SharedVulkanWindow window)
    : _window(window), _instance(instance) {
  // Create surface by glfw
  VkSurfaceKHR s;
  VkResult result = glfwCreateWindowSurface(_instance->AsVulkanObj(),
                                            _window->Get(), nullptr, &s);

  // Check success
  if (result != VK_SUCCESS)
    throw std::runtime_error(
        std::string("Could not create a window surface: ") +
        vk::to_string(static_cast<vk::Result>(result)));

  // Cast to vk::SurfaceKHR
  _vulkanObj = static_cast<vk::SurfaceKHR>(s);
}

core::Surface::~Surface() {
  // Destroy the surface
  vkDestroySurfaceKHR(_instance->AsVulkanObj(), _vulkanObj, nullptr);
}