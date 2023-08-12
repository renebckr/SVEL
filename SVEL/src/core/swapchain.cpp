/**
 * @file swapchain.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Swapchain Vulkan Wrapper implementation
 * @date 2022-10-02
 *
 * @copyright Copyright (c) 2022
 *
 */

// Local
#include "swapchain.h"
#include "device.h"

// Internal
#include <util/prioritized_order.hpp>

// STL
#include <cstdint>
#include <limits>

vk::SurfaceFormatKHR core::Swapchain::_findSurfaceFormat() {
  // Preferences
  std::vector<vk::Format> priorities = {vk::Format::eB8G8R8A8Unorm};

  // Fetch available formats
  auto formats = _device->GetPhysicalDevice().getSurfaceFormatsKHR(
      _surface->AsVulkanObj());
  if (formats.size() == 0)
    throw std::runtime_error("No Surface format exists.");

  // Priority order this list
  std::vector<std::pair<int, vk::Format>> formatList;
  for (uint32_t i = 0; i < formats.size(); i++)
    formatList.push_back({i, formats.at(i).format});

  auto prioritizedList = util::PriorityOrder(formatList, priorities);
  if (prioritizedList.size() == 0)
    throw std::runtime_error("No suitable Surface Format found.");
  return formats.at((unsigned int)prioritizedList.front());
}

vk::PresentModeKHR core::Swapchain::_findPresentMode() {
  // Preferences
  std::vector<vk::PresentModeKHR> priorities = {
      vk::PresentModeKHR::eFifo,    // VSYNC
      vk::PresentModeKHR::eMailbox, // Draw newest
      vk::PresentModeKHR::eFifoRelaxed, vk::PresentModeKHR::eImmediate};

  // Fetch possible present modes
  auto modes = _device->GetPhysicalDevice().getSurfacePresentModesKHR(
      _surface->AsVulkanObj());
  if (modes.size() == 0)
    throw std::runtime_error("No Surface Present Mode exists.");

  // Priority order the modes
  auto prioritizedList = util::PriorityOrder(modes, priorities);
  if (prioritizedList.size() == 0)
    throw std::runtime_error("No suitable Surface Mode found.");

  return *prioritizedList.front();
}

sv::Extent core::Swapchain::_createSwapchain() {
  // Get Surface Information
  _surfaceFormat = _findSurfaceFormat();
  auto surfacePresentMode = _findPresentMode();
  auto surfaceCapabilities =
      _device->GetPhysicalDevice().getSurfaceCapabilitiesKHR(
          _surface->AsVulkanObj());

  // Try doing triple buffering when possible
  uint32_t surfaceImageCount = 3;

  // Some implementations do not set min/max properly
  if (surfaceCapabilities.maxImageCount <= surfaceCapabilities.minImageCount)
    surfaceImageCount = surfaceCapabilities.minImageCount;
  else
    surfaceImageCount = std::min(
        surfaceCapabilities.maxImageCount,
        std::max(surfaceImageCount, surfaceCapabilities.minImageCount));

  uint32_t surfaceImageArrayLayers = 1;
  surfaceImageArrayLayers = std::min(surfaceCapabilities.maxImageArrayLayers,
                                     surfaceImageArrayLayers);

  // QueueFamily Information
  auto graphicQueueFamily = _device->GetGraphicsQueueFamily();
  auto presentQueueFamily = _device->GetPresentQueueFamily();
  bool multipleQueueFamilies = graphicQueueFamily != presentQueueFamily;
  uint32_t queueFamilyCount = multipleQueueFamilies ? 2 : 0;
  uint32_t *usedQueueFamilies = nullptr;
  if (multipleQueueFamilies) {
    usedQueueFamilies = new uint32_t[queueFamilyCount];
    usedQueueFamilies[0] = graphicQueueFamily;
    usedQueueFamilies[1] = presentQueueFamily;
  }

  // Setup Swapchain
  vk::SwapchainCreateInfoKHR swapchainInfo(
      vk::SwapchainCreateFlagsKHR(), _surface->AsVulkanObj(), surfaceImageCount,
      _surfaceFormat.format, _surfaceFormat.colorSpace,
      surfaceCapabilities.minImageExtent, surfaceImageArrayLayers,
      vk::ImageUsageFlagBits::eColorAttachment,
      multipleQueueFamilies ? vk::SharingMode::eConcurrent
                            : vk::SharingMode::eExclusive,
      queueFamilyCount, usedQueueFamilies,
      vk::SurfaceTransformFlagBitsKHR::eIdentity,
      vk::CompositeAlphaFlagBitsKHR::eOpaque, surfacePresentMode, VK_TRUE,
      VK_NULL_HANDLE);
  _vulkanObj = _device->AsVulkanObj().createSwapchainKHR(swapchainInfo);

  // Create Swapchain image views
  auto images = _device->AsVulkanObj().getSwapchainImagesKHR(_vulkanObj);
  _imageViews.reserve(images.size());
  vk::ImageViewCreateInfo imageViewInfo(
      vk::ImageViewCreateFlagBits(), {}, vk::ImageViewType::e2D,
      _surfaceFormat.format, vk::ComponentSwizzle::eIdentity,
      {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});
  for (auto image : images) {
    imageViewInfo.image = image;
    _imageViews.push_back(
        _device->AsVulkanObj().createImageView(imageViewInfo));
  }

  return {swapchainInfo.imageExtent.width, swapchainInfo.imageExtent.height};
}

void core::Swapchain::_destroySwapchain() {
  for (auto imageView : _imageViews)
    _device->AsVulkanObj().destroyImageView(imageView);
  _imageViews.clear();

  _device->AsVulkanObj().destroySwapchainKHR(_vulkanObj);
}

core::Swapchain::Swapchain(core::SharedDevice device,
                           core::SharedSurface surface)
    : _notifier(std::make_shared<Notifier>()), _device(device),
      _surface(surface) {
  _createSwapchain();
}

core::Swapchain::~Swapchain() { _destroySwapchain(); }

vk::ResultValue<uint32_t>
core::Swapchain::AcquireNextImage(vk::Semaphore semaphore,
                                  vk::Fence fence) const {
  return _device->AsVulkanObj().acquireNextImageKHR(
      _vulkanObj, std::numeric_limits<uint64_t>::max(), semaphore, fence);
}

void core::Swapchain::Recreate() {
  _destroySwapchain();
  _notifier->Notify(Event::eRecreate, _createSwapchain());
}

core::Swapchain::Notifier &core::Swapchain::GetNotifier() const {
  return *_notifier;
}