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

#include "window.h"
#include <svel/detail/app.h>

#include <util/vulkan_object.hpp>

#include <memory>
#include <vulkan/vulkan.hpp>

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
  sv::SharedIApplication _parentApp;

public:
  /**
   * @brief Construct a Surface.
   *
   * @param window Window that the surface should use
   * @param instance Instance that the surface should use
   */
  Surface(sv::SharedIApplication parent, core::SharedWindow window);

  Surface(const Surface &) = delete;
  Surface &operator=(const Surface &) = delete;

  /**
   * @brief Destroy the Surface
   */
  ~Surface();
};
SVEL_CLASS(Surface)

} // namespace core

#endif /* __CORE_SURFACE_H_ */