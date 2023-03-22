#ifndef __CORE_IMAGE_H__
#define __CORE_IMAGE_H__

#include "core/device.h"
#include "core/memory/device_memory.h"
#include "svel/config.h"
#include "util/vulkan_object.hpp"
#include <vulkan/vulkan.hpp>

namespace core {

class Image : public util::VulkanAdapter<vk::Image> {
private:
  core::SharedDevice _device;

  core::SharedDeviceMemory _memory;
  vk::ImageView _imageView;
  vk::Format _format;

public:
  Image(core::SharedDevice device, vk::Extent2D imageSize, vk::Format format,
        vk::ImageUsageFlags imageUsage,
        vk::ImageAspectFlags aspectFlags = vk::ImageAspectFlagBits::eColor);
  ~Image();

  vk::ImageView GetImageView() const;
};
SVEL_CLASS(Image)

} // namespace core

#endif /* __CORE_IMAGE_H__ */