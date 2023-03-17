#ifndef __RENDERER_FRAME_H__
#define __RENDERER_FRAME_H__

#include "core/barrier.h"
#include "core/device.h"
#include "core/swapchain.h"
#include "renderer/pipeline/pipeline.h"
#include <unordered_set>
#include <vulkan/vulkan.hpp>

namespace renderer {
class Frame {
private:
  core::SharedDevice _device;
  core::SharedSwapchain _swapchain;
  SharedVulkanPipeline _boundPipeline = nullptr;
  core::UniqueBarrier _barrier;
  vk::CommandPool _commandPool;
  vk::Semaphore _imageAvailable, _renderingDone;
  vk::Fence _inFlightFence;

  // Pre filled structs/values
  std::vector<vk::PipelineStageFlags> _waitStageMask;
  vk::CommandBufferAllocateInfo _mainCmdBufferInfo;
  vk::CommandBufferBeginInfo _mainCmdBufferBeginInfo;
  vk::ClearValue _clearValue;
  vk::SubmitInfo _submitInfo;
  vk::Queue _submitQueue;
  vk::Queue _presentQueue;
  vk::PresentInfoKHR _presentInfo;

  // Updated every Frame
  vk::CommandBuffer _currentBuffer;
  vk::ResultValue<uint32_t> _imageIndex =
      vk::ResultValue<uint32_t>(vk::Result::eSuccess, 0);

public:
  Frame(core::SharedDevice device, core::SharedSwapchain swapchain);
  ~Frame();

  void Instantiate();
  void BindPipeline(SharedVulkanPipeline pipeline);
  void UnbindPipeline();
  vk::PipelineLayout GetPipelineLayout();
  void Submit();
  void GetFrameSize(unsigned int &width, unsigned int &height);

  const vk::CommandPool &GetCommandPool();
  vk::CommandBuffer &GetCommandBuffer();
};
SVEL_CLASS(Frame)

} // namespace renderer

#endif /* __RENDERER_FRAME_H__ */