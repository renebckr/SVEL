#ifndef __CORE_SWAPCHAIN_H__
#define __CORE_SWAPCHAIN_H__

#include "core/surface.h"
#include "device.h"

#include <util/vulkan_object.hpp>

#include <vulkan/vulkan.hpp>

namespace core {

/**
 * @brief Wrapper for Vulkan Swapchain.
 */
class Swapchain : public util::VulkanAdapter<vk::SwapchainKHR> {
private:
  /**
   * @brief Device to use.
   */
  core::SharedDevice _device;
  core::SharedSurface _surface;

  /**
   * @brief The surface format.
   */
  vk::SurfaceFormatKHR _surfaceFormat;

  /**
   * @brief Image views created for the swapchain.
   */
  std::vector<vk::ImageView> _imageViews;

  /**
   * @brief Finds the surface format required for the physical device and
   * surface.
   *
   * @return vk::SurfaceFormatKHR The surface format
   */
  vk::SurfaceFormatKHR _findSurfaceFormat();

  /**
   * @brief Finds the present mode for the physical device and surface.
   *
   * @return vk::PresentModeKHR The present mode
   */
  vk::PresentModeKHR _findPresentMode();

public:
  /**
   * @brief Construct a Swapchain.
   *
   * @param surface Surface to use
   * @param device Device to use
   */
  Swapchain(core::SharedDevice device, core::SharedSurface surface);

  Swapchain(const Swapchain &) = delete;
  Swapchain &operator=(const Swapchain &) = delete;

  /**
   * @brief Destroy the Swapchain.
   */
  ~Swapchain();

  /**
   * @brief Getter for the Selected Format of the Surface.
   *
   * @return const vk::SurfaceFormatKHR& Surface format
   */
  const vk::SurfaceFormatKHR &GetSelectedFormat() { return _surfaceFormat; }

  /**
   * @brief Getter for the image views created for every image of the swapchain.
   *
   * @return const std::vector<vk::ImageView>& Vector of image views
   */
  const std::vector<vk::ImageView> &GetImageViews() { return _imageViews; }

  /**
   * @brief Getter for the Swapchain Image Count.
   *
   * @return unsigned int How many images there are in the swapchain
   */
  unsigned int GetSwapchainImageCount() { return _imageViews.size(); }

  /**
   * @brief Acquires the next swapchain image.
   *
   * @param semaphore Semaphore to notify
   * @param fence Fence to notify
   * @return vk::ResultValue<uint32_t>
   */
  vk::ResultValue<uint32_t>
  AcquireNextImage(vk::Semaphore semaphore = VK_NULL_HANDLE,
                   vk::Fence fence = VK_NULL_HANDLE) const;
};
SVEL_CLASS(Swapchain)

} // namespace core

#endif /* __CORE_SWAPCHAIN_H__ */