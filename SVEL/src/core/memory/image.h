/**
 * @file image.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declaration of Image.
 * @date 2023-03-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __CORE_MEMORY_IMAGE_H__
#define __CORE_MEMORY_IMAGE_H__

// Local
#include "device_memory.h"

// Internal
#include <core/device.h>
#include <svel/config.h>
#include <util/vulkan_object.hpp>

// Vulkan
#include <vulkan/vulkan.hpp>

namespace core {

/**
 * @brief Wraps a vulkan image and allows access to the image view.
 */
class Image : public util::VulkanAdapter<vk::Image> {
private:
  /**
   * @brief Device to use.
   */
  core::SharedDevice _device;

  /**
   * @brief Memory to use.
   */
  core::UniqueDeviceMemory _memory;

  /**
   * @brief The image view of the image.
   */
  vk::ImageView _imageView;

  /**
   * @brief Format of the image.
   */
  vk::Format _format;

public:
  /**
   * @brief Construct an image.
   *
   * @param device      Device to use.
   * @param imageSize   Extent of the image.
   * @param format      Format of the image.
   * @param imageUsage  Usage of the image.
   * @param aspectFlags Aspect Flags.
   */
  Image(core::SharedDevice device, vk::Extent2D imageSize, vk::Format format,
        vk::ImageUsageFlags imageUsage,
        vk::ImageAspectFlags aspectFlags = vk::ImageAspectFlagBits::eColor);

  /**
   * @brief Destroy the Image.
   */
  ~Image();

  /**
   * @brief Getter for the image view.
   *
   * @return vk::ImageView The image view of the image.
   */
  vk::ImageView GetImageView() const;
};
SVEL_CLASS(Image)

} // namespace core

#endif /* __CORE_MEMORY_IMAGE_H__ */