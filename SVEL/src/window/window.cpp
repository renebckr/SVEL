/**
 * @file window.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the Window Impl and Interface.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

// Local
#include "window.h"
#include "app.h"

// GLFW
#include <GLFW/glfw3.h>

// Internal
#include <core/instance.h>
#include <core/surface.h>
#include <renderer/frame.h>
#include <renderer/renderer.h>
#include <svel/detail/renderer.h>
#include <texture/texture.h>
#include <util/macros.hpp>
#include <window/input/keyMapping.h>

// STL
#include <iostream>
#include <stdexcept>

using namespace SVEL_NAMESPACE;

/**
 * @brief Macro for the implementation of static window callback methods.
 *
 */
#define IMPL_WINDOW_CALLBACK(name, ...)                                        \
  void IWindow::Impl::_##name(GLFWwindow *window, SIG_GET(__VA_ARGS__)) {      \
    if (const auto &it = _windowTable.find(window); it != _windowTable.end())  \
      it->second->_handle_##name(SIG_VALS(__VA_ARGS__));                       \
    else                                                                       \
      std::cout << "No internal handler for window event " #name               \
                   " could be found."                                          \
                << std::endl;                                                  \
  }                                                                            \
  void IWindow::Impl::_handle_##name(SIG_GET(__VA_ARGS__))

/**
 * @brief Define the window callback for window resizing. (Not in use)
 */
IMPL_WINDOW_CALLBACK(framebufferResizeCallback, int, x, int, y) {
  x = x + y; // get rid of warning
}

// --- IMPL ---

IWindow::Impl::Impl(core::SharedInstance instance, const std::string &title,
                    const Extent &size)
    : _instance(instance) {
  _window = std::make_shared<core::Window>(title, size);
  _surface = std::make_shared<core::Surface>(_instance, _window);

  // Create Renderer
  _renderer = std::make_shared<VulkanRenderer>(_instance, _surface);

  // Set default texture
  auto imgData = new unsigned char[4];
  imgData[0] = imgData[2] = 255;
  imgData[1] = 0;
  imgData[3] = 255;
  auto defaultImage = std::make_shared<Image>(Extent{1, 1}, 4, 4, imgData, 4);
  _defaultTexture = _renderer->CreateTexture(defaultImage);
  core::descriptor::Set::SetDefaultTexture(_defaultTexture);

  // Add this window to the table
  _windowTable[_window->Get()] = this;

  // Register callbacks
  glfwSetWindowSizeCallback(_window->Get(), &Impl::_framebufferResizeCallback);
}

IWindow::Impl::~Impl() {
  if (const auto &it = _windowTable.find(_window->Get());
      it != _windowTable.end())
    _windowTable.erase(it);
}

SharedVulkanRenderer IWindow::Impl::GetRenderer() const { return _renderer; }

core::SharedWindow IWindow::Impl::GetWindow() const { return _window; }

Extent IWindow::Impl::GetWindowSize() const {
  int width, height;
  glfwGetWindowSize(_window->Get(), &width, &height);
  return {(unsigned int)width, (unsigned int)height};
}

// --- INTERFACE ---

IWindow::IWindow(SharedIApplication parent, const std::string &title,
                 const Extent &windowSize) {
  __pImpl = std::make_shared<IWindow::Impl>(parent->__getImpl()->GetInstance(),
                                            title, windowSize);
}

SharedRenderer IWindow::GetRenderer() const { return __pImpl->GetRenderer(); }

Extent IWindow::GetWindowSize() const { return __pImpl->GetWindowSize(); }

void IWindow::StartRenderLoop() {
  const unsigned int maxInFlightFrameCount = 2;
  unsigned int currentFrame = 0;
  auto renderer = __pImpl->GetRenderer();
  auto window = __pImpl->GetWindow();

  // Create Frames
  std::vector<std::shared_ptr<renderer::Frame>> frames;
  for (unsigned int i = 0; i < maxInFlightFrameCount; i++)
    frames.push_back(std::make_shared<renderer::Frame>(
        renderer->GetDevice(), renderer->GetSwapchain()));

  // Start Render Loop
  while (!glfwWindowShouldClose(window->Get())) {
    glfwPollEvents();

    try {
      // Draw
      auto frame = frames.at(currentFrame);
      frame->Instantiate();
      renderer->SelectFrame(frame);
      Draw();
      if (!frame->Submit())
        renderer->RecreateSwapchain();
      // ---
    } catch (const vk::OutOfDateKHRError &e) {
      renderer->RecreateSwapchain();
    }
    currentFrame = (currentFrame + 1) % maxInFlightFrameCount;
  }

  // Finish up before frame destruction
  renderer->GetDevice()->AsVulkanObj().waitIdle();
}

void IWindow::SetKeyMapping(KeyMapping &mapping) {
  mapping.__getImpl()->Apply(__pImpl->GetWindow()->Get());
}