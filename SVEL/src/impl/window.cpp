#include "window.h"
#include "app.h"
#include "renderer/frame.h"
#include <core/instance.h>
#include <core/surface.h>
#include <renderer/renderer.h>
#include <svel/detail/renderer.h>
#include <texture/texture.h>

#include <stdexcept>

using namespace SVEL_NAMESPACE;

// --- IMPL ---

IWindow::Impl::Impl(core::SharedInstance instance, const std::string &title,
                    const Extent &size)
    : _instance(instance) {
  _window = std::make_shared<core::VulkanWindow>(title, size);
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
}

SharedVulkanRenderer IWindow::Impl::GetRenderer() const { return _renderer; }

core::SharedVulkanWindow IWindow::Impl::GetWindow() const { return _window; }

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
    // Draw
    auto frame = frames.at(currentFrame);
    frame->Instantiate();
    renderer->SelectFrame(frame);
    Draw();
    frame->Submit();
    // ---
    currentFrame = (currentFrame + 1) % maxInFlightFrameCount;
  }

  // Finish up before frame destruction
  renderer->GetDevice()->AsVulkanObj().waitIdle();
}