/**
 * @file frame.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the Frame.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

// Local
#include "frame.h"

// STL
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace renderer;

Frame::Frame(core::SharedDevice device, core::SharedSwapchain swapchain)
    : _device(device), _swapchain(swapchain) {
  auto vulkanDevice = device->AsVulkanObj();

  // Create Barrier
  _barrier = std::make_unique<core::Barrier>(device);

  // Create Command Pool
  vk::CommandPoolCreateInfo commandPoolInfo(vk::CommandPoolCreateFlagBits(),
                                            device->GetGraphicsQueueFamily());
  _commandPool = vulkanDevice.createCommandPool(commandPoolInfo);

  // Create Semaphores and Fence
  _imageAvailable = vulkanDevice.createSemaphore(vk::SemaphoreCreateInfo());
  _renderingDone = vulkanDevice.createSemaphore(vk::SemaphoreCreateInfo());
  _inFlightFence = vulkanDevice.createFence(
      vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled));

  // Prepare Structs
  _mainCmdBufferInfo = vk::CommandBufferAllocateInfo(
      _commandPool, vk::CommandBufferLevel::ePrimary, 1);

  _mainCmdBufferBeginInfo = vk::CommandBufferBeginInfo(
      vk::CommandBufferUsageFlagBits::eOneTimeSubmit, nullptr);

  _clearValue.push_back(vk::ClearValue(
      vk::ClearColorValue().setFloat32({0.0f, 0.0f, 0.0f, 0.0f})));
  _clearValue.push_back(vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0.0f)));

  _waitStageMask = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
  _submitInfo = vk::SubmitInfo(1, &_imageAvailable, _waitStageMask.data(), 1,
                               nullptr, 1, &_renderingDone);

  _submitQueue = vulkanDevice.getQueue(device->GetGraphicsQueueFamily(), 0);
  _presentQueue = vulkanDevice.getQueue(device->GetPresentQueueFamily(), 0);

  _presentInfo =
      vk::PresentInfoKHR(1, &_renderingDone, 1, nullptr, nullptr, nullptr);

  // Allocate the command buffer
  _currentBuffer = vulkanDevice.allocateCommandBuffers(_mainCmdBufferInfo)[0];
}

Frame::~Frame() {
  auto vulkanDevice = _device->AsVulkanObj();
  vulkanDevice.destroyCommandPool(_commandPool);
  vulkanDevice.destroySemaphore(_imageAvailable);
  vulkanDevice.destroySemaphore(_renderingDone);
  vulkanDevice.destroyFence(_inFlightFence);
}

void Frame::Instantiate() {
  auto vulkanDevice = _device->AsVulkanObj();

  // Wait for Fence
  auto result = vulkanDevice.waitForFences(
      _inFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
  _barrier->WaitCompletion(); // Reset Ressources as well
  if (result != vk::Result::eSuccess)
    throw std::runtime_error(std::string("Could not wait for Frame fence: ") +
                             vk::to_string(result));
  vulkanDevice.resetFences(_inFlightFence);

  // Reset Pool
  vulkanDevice.resetCommandPool(_commandPool);

  // Acquire Image
  _imageIndex = _swapchain->AcquireNextImage(_imageAvailable);

  // Start Command Buffer
  _currentBuffer.begin(_mainCmdBufferBeginInfo);
}

void Frame::BindPipeline(SharedVulkanPipeline pipeline) {
  vk::RenderPassBeginInfo renderPassBegin(
      pipeline->GetRenderPass(),
      pipeline->GetFramebuffers().at(_imageIndex.value),
      {{0, 0},
       {(uint32_t)pipeline->GetViewport().width,
        (uint32_t)pipeline->GetViewport().height}},
      (uint32_t)_clearValue.size(), _clearValue.data());

  // Record Command Buffer
  _currentBuffer.beginRenderPass(renderPassBegin, vk::SubpassContents::eInline);
  _currentBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics,
                              pipeline->AsVulkanObj());

  // Prototypical as of now ---
  const auto &viewport = pipeline->GetViewport();
  _currentBuffer.setViewport(0, viewport);

  vk::Rect2D scissor({0, 0},
                     {(uint32_t)viewport.width, (uint32_t)viewport.height});
  _currentBuffer.setScissor(0, {scissor});
  // ---

  _boundPipeline = pipeline;
}

void Frame::UnbindPipeline() {
  _currentBuffer.endRenderPass();
  _boundPipeline = nullptr;
}

bool Frame::Submit() {
  // End Command Buffer
  _currentBuffer.end();

  // Submit Command Buffer
  _submitInfo.setCommandBuffers(_currentBuffer);
  _submitQueue.submit(_submitInfo, _inFlightFence);

  // Present Image
  _presentInfo.setSwapchains(_swapchain->AsVulkanObj());
  _presentInfo.setPImageIndices(&_imageIndex.value);
  auto result = _presentQueue.presentKHR(_presentInfo);
  if (result == vk::Result::eSuboptimalKHR)
    return false;
  if (result != vk::Result::eSuccess)
    throw std::runtime_error(vk::to_string(result));
  return true;
}

const vk::CommandPool &Frame::GetCommandPool() { return _commandPool; }

void Frame::GetFrameSize(unsigned int &width, unsigned int &height) {
  // We require a bound pipeline
  if (_boundPipeline == nullptr)
    return;

  width = (uint32_t)_boundPipeline->GetViewport().width;
  height = (uint32_t)_boundPipeline->GetViewport().height;
}

vk::CommandBuffer *Frame::GetCommandBuffer() { return &_currentBuffer; }

vk::PipelineLayout Frame::GetPipelineLayout() {
  return _boundPipeline->GetLayout();
}