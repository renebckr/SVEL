#ifndef __IMPL_WINDOW_H__
#define __IMPL_WINDOW_H__

#include <core/instance.h>
#include <core/surface.h>
#include <core/window.h>
#include <renderer/renderer.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <svel/detail/app.h>
#include <svel/detail/window.h>
#include <svel/util/structs.hpp>

namespace SVEL_NAMESPACE {

class IWindow::Impl {
private:
  SharedTexture _defaultTexture;
  core::SharedInstance _instance;
  SharedVulkanRenderer _renderer;
  core::SharedVulkanWindow _window;
  core::SharedSurface _surface;

public:
  Impl(core::SharedInstance instance, const std::string &title,
       const Extent &size);
  ~Impl() = default;

  SharedVulkanRenderer GetRenderer() const;
  core::SharedVulkanWindow GetWindow() const;
  Extent GetWindowSize() const;
};

} // namespace SVEL_NAMESPACE

#endif /* __IMPL_WINDOW_H__ */