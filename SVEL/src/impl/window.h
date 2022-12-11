#ifndef __IMPL_WINDOW_H__
#define __IMPL_WINDOW_H__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <svel/detail/app.h>
#include <svel/detail/window.h>
#include <svel/util/structs.hpp>

namespace SVEL_NAMESPACE {

class IWindow::Impl {
private:
  sv::SharedIApplication _parentApp;

  /**
   * @brief The GLFWwindow ptr.
   */
  GLFWwindow *_win;

  /**
   * @brief The Vulkan Surface for this window.
   *
   */
  vk::SurfaceKHR _surface;

  /**
   * @brief Title of the window.
   */
  std::string _title;

  /**
   * @brief Extent of the window.
   */
  sv::Extent _winSize;

public:
  Impl(sv::SharedIApplication parent, const std::string &title,
       const Extent &size);
  ~Impl();

  vk::SurfaceKHR GetSurface();
};

} // namespace SVEL_NAMESPACE

#endif /* __IMPL_WINDOW_H__ */