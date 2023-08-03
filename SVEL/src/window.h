/**
 * @file window.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declares the Window Impl.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __WINDOW_H__
#define __WINDOW_H__

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
 * @brief Declaration of the Window Impl for the pImpl Idiom.
 */
class IWindow::Impl {
private:
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

#endif /* __WINDOW_H__ */