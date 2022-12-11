#include "window.h"
#include "app.h"

#include <stdexcept>

using namespace SVEL_NAMESPACE;

// --- IMPL ---

IWindow::Impl::Impl(sv::SharedIApplication parent, const std::string &title,
                    const Extent &size)
    : _parentApp(parent), _title(title) {
  // Fetch primary monitor and video mode
  auto monitor = glfwGetPrimaryMonitor();
  auto mode = glfwGetVideoMode(monitor);

  // Set size
  _winSize.height = size.height == 0 ? mode->height : size.height;
  _winSize.width = size.width == 0 ? mode->width : size.width;

  // Set Hints
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

  // Create Window
  _win = glfwCreateWindow(_winSize.width, _winSize.height, _title.c_str(),
                          monitor, nullptr);
  if (_win == nullptr)
    throw std::runtime_error("Could not create GLFW window.");

  // Create surface by glfw
  VkSurfaceKHR surface;
  VkResult result = glfwCreateWindowSurface(
      _parentApp->__getImpl()->AsVulkanObj(), _win, nullptr, &surface);

  // Check success
  if (result != VK_SUCCESS)
    throw std::runtime_error(
        std::string("Could not create a window surface: ") +
        vk::to_string(static_cast<vk::Result>(result)));

  // Cast to vk::SurfaceKHR
  _surface = static_cast<vk::SurfaceKHR>(surface);
}

IWindow::Impl::~Impl() {
  vkDestroySurfaceKHR(_parentApp->__getImpl()->AsVulkanObj(), _surface,
                      nullptr);
  glfwDestroyWindow(_win);
}

vk::SurfaceKHR IWindow::Impl::GetSurface() { return _surface; }

// --- INTERFACE ---

IWindow::IWindow(SharedIApplication parent, const std::string &title,
                 const Extent &windowSize) {
  __pImpl = std::make_shared<IWindow::Impl>(parent, title, windowSize);
}