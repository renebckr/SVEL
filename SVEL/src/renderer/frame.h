/**
 * @file frame.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declaration of the Frame.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __RENDERER_FRAME_H__
#define __RENDERER_FRAME_H__

// Internal
#include <core/barrier.h>
#include <core/device.h>
#include <core/swapchain.h>
#include <renderer/pipeline/pipeline.h>

// Vulkan
#include <vulkan/vulkan.hpp>

// STL
#include <unordered_set>

namespace renderer {

/**
 * @brief Manages everything related to a single frame during the draw phase.
 * TODO: Do this in a different way to reduce complexity and make the system
 * more flexible.
 */
class Frame {
private:
  /**
   * @brief Device to use.
   */
  core::SharedDevice _device;

  /**
   * @brief Swapchain to use.
   */
  core::SharedSwapchain _swapchain;

  /**
   * @brief Which pipeline is currently bound.
   */
  SharedVulkanPipeline _boundPipeline = nullptr;

  /**
   * @brief General purpose barrier.
   */
  core::UniqueBarrier _barrier;

  /**
   * @brief General purpose command pool.
   */
  vk::CommandPool _commandPool;

  /**
   * @brief Semaphore to wait for an available image.
   */
  vk::Semaphore _imageAvailable;

  /**
   * @brief Semaphore to wait for rendering.
   */
  vk::Semaphore _renderingDone;

  /**
   * @brief Fence to wait on to ensure that a new frame can start.
   */
  vk::Fence _inFlightFence;

  // --- Pre filled structs/values ---

  /**
   * @brief Wait stages of the pipeline.
   */
  std::vector<vk::PipelineStageFlags> _waitStageMask;

  /**
   * @brief Allocation info for the main command buffer.
   */
  vk::CommandBufferAllocateInfo _mainCmdBufferInfo;

  /**
   * @brief Begin info for the main command buffer.
   */
  vk::CommandBufferBeginInfo _mainCmdBufferBeginInfo;

  /**
   * @brief Clear values of the pipeline frambuffer images.
   */
  std::vector<vk::ClearValue> _clearValue;

  /**
   * @brief Info to use for the submission.
   */
  vk::SubmitInfo _submitInfo;

  /**
   * @brief Queue to use for submission.
   */
  vk::Queue _submitQueue;

  /**
   * @brief Queue to use for presentation.
   */
  vk::Queue _presentQueue;

  /**
   * @brief Info used for presentation.
   */
  vk::PresentInfoKHR _presentInfo;

  // Updated every Frame

  /**
   * @brief The current buffer that is used for recording draws, binding
   * pipelines, using descriptor sets, ...
   */
  vk::CommandBuffer _currentBuffer;

  /**
   * @brief The image index of the swapchain image.
   */
  vk::ResultValue<uint32_t> _imageIndex =
      vk::ResultValue<uint32_t>(vk::Result::eSuccess, 0);

public:
  /**
   * @brief Construct a Frame.
   *
   * @param device    Device to use.
   * @param swapchain Swapchain to use.
   */
  Frame(core::SharedDevice device, core::SharedSwapchain swapchain);

  /**
   * @brief Destroy the Frame.
   */
  ~Frame();

  /**
   * @brief Instantiate the frame. Marks this frame as active.
   */
  void Instantiate();

  /**
   * @brief Bind a graphics pipeline.
   *
   * @param pipeline The graphics pipeline to bind.
   */
  void BindPipeline(SharedVulkanPipeline pipeline);

  /**
   * @brief Undbinds a graphics pipeline.
   */
  void UnbindPipeline();

  /**
   * @brief Getter for the Pipeline Layout.
   *
   * @return vk::PipelineLayout Layout of the bound pipeline.
   */
  vk::PipelineLayout GetPipelineLayout();

  /**
   * @brief Submit the frame to the gpu.
   */
  void Submit();

  /**
   * @brief Getter for the frame width/height.
   *
   * @param width   Width of the frame.
   * @param height  Height of the frame.
   */
  void GetFrameSize(unsigned int &width, unsigned int &height);

  /**
   * @brief Getter for the command pool of the frame.
   *
   * @return const vk::CommandPool& Command pool to use in the context of this
   *                                frame.
   */
  const vk::CommandPool &GetCommandPool();

  /**
   * @brief Getter for the current command buffer.
   *
   * @return vk::CommandBuffer* The currently active command buffer.
   */
  vk::CommandBuffer *GetCommandBuffer();
};
SVEL_CLASS(Frame)

} // namespace renderer

#endif /* __RENDERER_FRAME_H__ */