/**
 * @file surface.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Surface vulkan wrapper declaration
 * @date 2022-10-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __CORE_SURFACE_H_
#define __CORE_SURFACE_H_

// Local
#include "instance.h"
#include "window.h"

// Internal
#include <util/vulkan_object.hpp>

// Vulkan
#include <vulkan/vulkan.hpp>

// STL
#include <memory>

namespace core {

/**
 * @brief Wrapper for the Vulkan Surface.
 */
class Surface : public util::VulkanAdapter<vk::SurfaceKHR> {
private:
  /**
   * @brief Window to use.
   */
  core::SharedWindow _window;

  /**
   * @brief Instance to use.
   */
  core::SharedInstance _instance;

public:
  /**
   * @brief Construct a Surface.
   *
   * @param window    Window that the surface should use
   * @param instance  Instance that the surface should use
   */
  Surface(core::SharedInstance instance, core::SharedWindow window);

  /**
   * @brief Cannot be copied.
   */
  Surface(const Surface &) = delete;

  /**
   * @brief Cannot be copied.
   *
   * @return Surface& ~unused~
   */
  Surface &operator=(const Surface &) = delete;

  /**
   * @brief Destroy the Surface.
   */
  ~Surface();
};
SVEL_CLASS(Surface)

} // namespace core

#endif /* __CORE_SURFACE_H_ */