/**
 * @file image.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the Image.
 * @date 2023-03-23
 *
 * @copyright Copyright (c) 2023
 *
 */

// Local
#include "image.h"

using namespace core;

Image::Image(core::SharedDevice device, vk::Extent2D imageSize,
             vk::Format format, vk::ImageUsageFlags imageUsage,
             vk::ImageAspectFlags aspectFlags)
    : _device(device), _format(format) {
  auto vulkanDevice = _device->AsVulkanObj();

  // Create Vulkan Image
  auto imageCreateInfo = vk::ImageCreateInfo(
      vk::ImageCreateFlags(), vk::ImageType::e2D, _format,
      {imageSize.width, imageSize.height, 1}, 1, 1, vk::SampleCountFlagBits::e1,
      vk::ImageTiling::eOptimal, imageUsage, vk::SharingMode::eExclusive, {},
      vk::ImageLayout::ePreinitialized);
  _vulkanObj = vulkanDevice.createImage(imageCreateInfo);

  // Allocate Image Memory
  auto memRequirements = vulkanDevice.getImageMemoryRequirements(_vulkanObj);
  _memory = std::make_unique<core::DeviceMemory>(
      _device, memRequirements, vk::MemoryPropertyFlagBits::eDeviceLocal);
  vulkanDevice.bindImageMemory(_vulkanObj, _memory->AsVulkanObj(), 0);

  // Create Image View
  auto imageViewInfo = vk::ImageViewCreateInfo(
      vk::ImageViewCreateFlagBits(), _vulkanObj, vk::ImageViewType::e2D,
      _format, vk::ComponentMapping(),
      vk::ImageSubresourceRange(aspectFlags, 0, 1, 0, 1));
  _imageView = vulkanDevice.createImageView(imageViewInfo);
}

Image::~Image() {
  auto vulkanDevice = _device->AsVulkanObj();
  vulkanDevice.destroyImageView(_imageView);
  vulkanDevice.destroyImage(_vulkanObj);
}

vk::ImageView Image::GetImageView() const { return _imageView; }
