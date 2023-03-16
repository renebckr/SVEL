#include "window.h"
#include "app.h"
#include <core/instance.h>
#include <core/surface.h>
#include <renderer/renderer.h>
#include <svel/detail/renderer.h>

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
}

SharedVulkanRenderer IWindow::Impl::GetRenderer() const { return _renderer; }

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