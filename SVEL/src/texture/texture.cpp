/**
 * @file texture.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the Texture.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

// Local
#include "texture.h"

// Internal
#include <core/barrier.h>
#include <core/device.h>

// Vulkan
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

// STL
#include <cstring>
#include <memory>
#include <stdexcept>

using namespace SVEL_NAMESPACE;

core::SharedBuffer Texture::_createStagingBuffer(SharedImage img) {
  // Create Staging Buffer
  auto imgSize = img->GetSize();
  auto stagingBuffer = std::make_shared<core::Buffer>(
      _device, imgSize, vk::BufferUsageFlagBits::eTransferSrc,
      vk::MemoryPropertyFlagBits::eHostVisible |
          vk::MemoryPropertyFlagBits::eHostCoherent);

  // Copy Data into stagingBuffer
  auto vulkanDevice = _device->AsVulkanObj();
  auto dst = vulkanDevice.mapMemory(stagingBuffer->GetMemory(), 0, imgSize,
                                    vk::MemoryMapFlags());
  std::memcpy(dst, img->GetData(), imgSize);
  vulkanDevice.unmapMemory(stagingBuffer->GetMemory());
  return stagingBuffer;
}

void Texture::_createImage(SharedImage _img) {
  auto vulkanDevice = _device->AsVulkanObj();

  // Create Vulkan Image
  auto imageCreateInfo = vk::ImageCreateInfo(
      vk::ImageCreateFlags(), vk::ImageType::e2D, vk::Format::eR8G8B8A8Srgb,
      {_img->GetExtent().width, _img->GetExtent().height, 1}, 1, 1,
      vk::SampleCountFlagBits::e1, vk::ImageTiling::eOptimal,
      vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
      vk::SharingMode::eExclusive, {}, vk::ImageLayout::ePreinitialized);
  _image = vulkanDevice.createImage(imageCreateInfo);

  // Allocate Image Memory
  auto memRequirements = vulkanDevice.getImageMemoryRequirements(_image);
  _imageMemory = std::make_unique<core::DeviceMemory>(
      _device, memRequirements, vk::MemoryPropertyFlagBits::eDeviceLocal);
  vulkanDevice.bindImageMemory(_image, _imageMemory->AsVulkanObj(), 0);

  // Create Image View
  auto imageViewInfo = vk::ImageViewCreateInfo(
      vk::ImageViewCreateFlagBits(), _image, vk::ImageViewType::e2D,
      vk::Format::eR8G8B8A8Srgb, vk::ComponentMapping(),
      vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
  _imageView = vulkanDevice.createImageView(imageViewInfo);
}

void Texture::_createSampler() {
  auto samplerInfo = vk::SamplerCreateInfo(
      vk::SamplerCreateFlags(), vk::Filter::eNearest, vk::Filter::eLinear,
      vk::SamplerMipmapMode::eLinear, vk::SamplerAddressMode::eRepeat,
      vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode::eRepeat, 0.0f,
      VK_FALSE, 0.0f, VK_FALSE, vk::CompareOp::eAlways, 0.0f, 0.0f,
      vk::BorderColor::eFloatOpaqueBlack, VK_FALSE);
  _sampler = _device->AsVulkanObj().createSampler(samplerInfo);
}

void Texture::_updateImageInfo() {
  _imageInfo = vk::DescriptorImageInfo(_sampler, _imageView,
                                       vk::ImageLayout::eShaderReadOnlyOptimal);
}

Texture::Texture(core::SharedDevice device, SharedImage img) : _device(device) {
  // Check if image valid
  if (img == nullptr || img->GetData() == nullptr)
    throw std::runtime_error("Image has no data");

  // Create Buffers/Structs
  _stagingBuffer = _createStagingBuffer(img);
  _dim = img->GetExtent();
  _createImage(img);
  _createSampler();
  _updateImageInfo();
}

void Texture::Dispatch(vk::CommandPool &_commandPool,
                       core::SharedBarrier _barrier) {
  auto vulkanDevice = _device->AsVulkanObj();
  // Create Fence
  auto fence = std::make_shared<core::Fence>(_device);

  // Create CommandBuffer
  auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo(
      _commandPool, vk::CommandBufferLevel::ePrimary, 1);
  _commandBuffer =
      vulkanDevice.allocateCommandBuffers(commandBufferAllocateInfo).front();

  auto commandBufferBeginInfo = vk::CommandBufferBeginInfo(
      vk::CommandBufferUsageFlagBits::eOneTimeSubmit, nullptr);
  _commandBuffer.begin(commandBufferBeginInfo);

  // Create Pre copy barrier
  auto layoutBarrier = vk::ImageMemoryBarrier(
      vk::AccessFlagBits::eHostWrite, vk::AccessFlagBits::eTransferWrite,
      vk::ImageLayout::ePreinitialized, vk::ImageLayout::eTransferDstOptimal,
      {}, {}, _image,
      vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
  _commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eHost,
                                 vk::PipelineStageFlagBits::eTransfer,
                                 vk::DependencyFlags(), {}, {}, layoutBarrier);

  // Copy Data into Image
  auto bufferImageCopy = vk::BufferImageCopy(
      0, 0, 0,
      vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1),
      {0, 0, 0}, {_dim.width, _dim.height, 1});
  _commandBuffer.copyBufferToImage(_stagingBuffer->AsVulkanObj(), _image,
                                   vk::ImageLayout::eTransferDstOptimal,
                                   bufferImageCopy);

  // Post Copy Barrier
  layoutBarrier.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite);
  layoutBarrier.setDstAccessMask(vk::AccessFlagBits::eShaderRead);
  layoutBarrier.setOldLayout(vk::ImageLayout::eTransferDstOptimal);
  layoutBarrier.setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal);
  _commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
                                 vk::PipelineStageFlagBits::eAllCommands,
                                 vk::DependencyFlags(), {}, {}, layoutBarrier);
  _commandBuffer.end();

  // Submit CommandBuffer
  auto queue = vulkanDevice.getQueue(_device->GetGraphicsQueueFamily(), 0);
  auto submitInfo =
      vk::SubmitInfo(0, nullptr, nullptr, 1, &_commandBuffer, 0, nullptr);
  queue.submit(submitInfo, fence->AsVulkanObj());

  // Add Resource to Barrier
  _barrier->AddResource(
      fence,
      std::bind(&Texture::_onUpload, this->shared_from_this(), _commandPool));
}

Texture::~Texture() {
  auto vulkanDevice = _device->AsVulkanObj();
  vulkanDevice.destroyImage(_image);
  vulkanDevice.destroyImageView(_imageView);
  vulkanDevice.destroySampler(_sampler);
}