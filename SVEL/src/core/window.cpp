/**
 * @file window.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Window GLFW wrapper implementation
 * @date 2022-10-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "window.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

core::VulkanWindow::VulkanWindow(const std::string &title,
                                 const sv::Extent &size)
    : _title(title) {
  // Fetch primary monitor and video mode
  auto monitor = glfwGetPrimaryMonitor();
  auto mode = glfwGetVideoMode(monitor);

  // Set size
  _size.height = size.height == 0 ? (unsigned int)mode->height : size.height;
  _size.width = size.width == 0 ? (unsigned int)mode->width : size.width;

  // Set Hints
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

  // Create Window
  _win = glfwCreateWindow((int)_size.width, (int)_size.height, _title.c_str(),
                          monitor, nullptr);
  if (_win == nullptr)
    throw std::runtime_error("Could not create GLFW window.");
}

core::VulkanWindow::~VulkanWindow() { glfwDestroyWindow(_win); }