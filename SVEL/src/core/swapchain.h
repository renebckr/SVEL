/**
 * @file swapchain.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declaration of the Swapchain.
 * @date 2023-03-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __CORE_SWAPCHAIN_H__
#define __CORE_SWAPCHAIN_H__

// Local
#include "device.h"
#include "surface.h"

// Internal
#include <core/event/notifier.hpp>
#include <util/vulkan_object.hpp>

// Vulkan
#include <vulkan/vulkan.hpp>

namespace core {

/**
 * @brief Wrapper for Vulkan Swapchain.
 */
class Swapchain : public util::VulkanAdapter<vk::SwapchainKHR> {
public:
  /**
   * @brief Possible events to subscribe to.
   */
  enum class Event { eRecreate };

  /**
   * @brief Type of the notifier that can be subscribed to.
   */
  using Notifier = event::Notifier<Event, sv::Extent>;

private:
  /**
   * @brief Notifier that other objects can subscribe to.
   */
  std::shared_ptr<Notifier> _notifier;

  /**
   * @brief Device to use.
   */
  core::SharedDevice _device;

  /**
   * @brief Surface to use.
   */
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

  /**
   * @brief Internal method that creates the swapchain and returns the extent
   * that it was created with.
   *
   * @return sv::Extent Image size for the images of the swapchain.
   */
  sv::Extent _createSwapchain();

  /**
   * @brief Internal method that destroys the swapchain.
   */
  void _destroySwapchain();

public:
  /**
   * @brief Construct a Swapchain.
   *
   * @param surface Surface to use
   * @param device  Device to use
   */
  Swapchain(core::SharedDevice device, core::SharedSurface surface);

  /**
   * @brief Cannot be copied.
   */
  Swapchain(const Swapchain &) = delete;

  /**
   * @brief Cannot be copied.
   *
   * @return Swapchain& ~unused~
   */
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
  unsigned int GetSwapchainImageCount() {
    return (unsigned int)_imageViews.size();
  }

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

  /**
   * @brief Recreate the swapchain with respect to the current surface
   * capabilities.
   */
  void Recreate();

  /**
   * @brief Getter for the notifier.
   *
   * @return Notifier& The notifier that objects may subscribe to.
   */
  Notifier &GetNotifier() const;
};
SVEL_CLASS(Swapchain)

} // namespace core

#endif /* __CORE_SWAPCHAIN_H__ */