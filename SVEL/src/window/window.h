/**
 * @file window.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declares the Window Impl.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __WINDOW_WINDOW_H__
#define __WINDOW_WINDOW_H__

// Internal
#include <core/instance.h>
#include <core/surface.h>
#include <core/window.h>
#include <renderer/renderer.h>
#include <svel/detail/app.h>
#include <svel/detail/window.h>
#include <svel/util/structs.hpp>

// GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Vulkan
#include <vulkan/vulkan.hpp>

namespace SVEL_NAMESPACE {

/**
 * @brief Macro to declare a window callback.
 */
#define WINDOW_CALLBACK(name, ...)                                             \
  static void _##name(GLFWwindow *window, __VA_ARGS__);                        \
  void _handle_##name(__VA_ARGS__);

/**
 * @brief Declaration of the Window Impl for the pImpl Idiom.
 */
class IWindow::Impl {
private:
  /**
   * @brief Table that contains all GLFWwindows and the window class that is
   * responsible for them.
   */
  static inline std::unordered_map<GLFWwindow *, IWindow::Impl *> _windowTable;

  /**
   * @brief Window callback for the resize event.
   */
  WINDOW_CALLBACK(framebufferResizeCallback, int, int)

  /**
   * @brief Default Texture.
   */
  SharedTexture _defaultTexture;

  /**
   * @brief Vulkan instance to use.
   */
  core::SharedInstance _instance;

  /**
   * @brief The renderer of the window.
   */
  SharedVulkanRenderer _renderer;

  /**
   * @brief The window itself.
   */
  core::SharedWindow _window;

  /**
   * @brief The Surface used.
   */
  core::SharedSurface _surface;

public:
  /**
   * @brief Construct the Impl.
   *
   * @param instance  Instance to use.
   * @param title     Title of the window.
   * @param size      Dimensions of the window.
   */
  Impl(core::SharedInstance instance, const std::string &title,
       const Extent &size);

  /**
   * @brief Destroy the Impl.
   */
  ~Impl();

  /**
   * @brief Getter for the renderer.
   *
   * @return SharedVulkanRenderer The vulkan renderer.
   */
  SharedVulkanRenderer GetRenderer() const;

  /**
   * @brief Getter for the window.
   *
   * @return core::SharedWindow The window.
   */
  core::SharedWindow GetWindow() const;

  /**
   * @brief Getter for the window size.
   *
   * @return Extent Size of the window.
   */
  Extent GetWindowSize() const;
};

} // namespace SVEL_NAMESPACE

#endif /* __WINDOW_WINDOW_H__ */