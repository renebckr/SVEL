/**
 * @file pipeline.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Vulkan Pipeline Wrapper declaration
 * @date 2022-10-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __RENDERER_PIPELINE_PIPELINE_H__
#define __RENDERER_PIPELINE_PIPELINE_H__

// Internal
#include <core/descriptor/group.h>
#include <core/device.h>
#include <core/event/notifier.hpp>
#include <core/memory/image.h>
#include <core/shader.h>
#include <core/surface.h>
#include <core/swapchain.h>
#include <svel/detail/pipeline.h>
#include <util/downcast_impl.hpp>
#include <util/vulkan_object.hpp>

// Vulkan
#include <vulkan/vulkan.hpp>

// STL
#include <vector>

namespace renderer {

/**
 * @brief Wrapper for Vulkan Pipeline.
 */
class VulkanPipeline : public SVEL_NAMESPACE::Pipeline,
                       public util::VulkanAdapter<vk::Pipeline> {
private:
  /**
   * @brief Device to use.
   */
  core::SharedDevice _device;

  /**
   * @brief Surface to use.
   */
  core::SharedSurface _surface;

  /**
   * @brief Swapchain to use.
   */
  core::SharedSwapchain _swapchain;

  /**
   * @brief Subscription handle for the swapchain recreation notification.
   */
  std::unique_ptr<core::event::SubscriptionHandle>
      _swapchainRecreationSubscription;

  /**
   * @brief Shaders to use.
   */
  core::SharedShader _vert, _frag;

  /**
   * @brief Binding Description for vertices.
   */
  vk::VertexInputBindingDescription _vertexBindingDescription;

  /**
   * @brief Input attribute description.
   */
  std::vector<vk::VertexInputAttributeDescription>
      _vertexInputAttributeDescriptions;

  /**
   * @brief Input State info.
   */
  vk::PipelineVertexInputStateCreateInfo _vertexInputStateInfo;

  /**
   * @brief Viewport that the Pipeline should use.
   */
  vk::Viewport _viewport;

  /**
   * @brief Layout of the pipeline.
   */
  vk::PipelineLayout _pipelineLayout;

  /**
   * @brief The Render Pass that this pipeline uses. Should be expanded in the
   * future to include multiple render passes.
   */
  vk::RenderPass _renderPass;

  /**
   * @brief Framebuffers for this Pipeline.
   */
  std::vector<vk::Framebuffer> _framebuffers;

  /**
   * @brief The set group of this pipeline used for descriptor sets.
   */
  std::shared_ptr<core::descriptor::SetGroup> _setGroup;

  /**
   * @brief The depth buffer image for depth buffering.
   */
  core::SharedImage _depthBuffer;

  /**
   * @brief Size of a vertex.
   */
  unsigned int _vertexInputSize = 0;

  /**
   * @brief Format of the depth image.
   */
  vk::Format _depthFormat;

  /**
   * @brief Returns the vk::Format corresponding to the Attribute Type.
   *
   * @param type        The attribute type.
   * @param count       How many attributes.
   * @return vk::Format The corresponding format.
   */
  vk::Format _getAttributeFormat(SVEL_NAMESPACE::AttributeType type,
                                 unsigned int count);

  /**
   * @brief Returns the size of the Attribute.
   *
   * @param type    Type of the attribute.
   * @param count   How many attributes.
   * @return size_t Size of the composite attribute.
   */
  size_t _getAttributeSize(SVEL_NAMESPACE::AttributeType type,
                           unsigned int count);

  /**
   * @brief Build the Vertex Input State Info from a given vertex description.
   *
   * @param vertexDescription Provided vertex description.
   */
  void _buildVertexInputStateInfo(
      const SVEL_NAMESPACE::VertexDescription &vertexDescription);

  /**
   * @brief Finds the first supported format of the provided format list.
   *
   * @param formats       Formats to query support for.
   * @param tiling        Tiling to use.
   * @param featureFlags  Feature flags to support.
   * @return vk::Format   The supported format if found. Will throw if not
   * found.
   */
  vk::Format _findSupportedFormat(const std::vector<vk::Format> &formats,
                                  vk::ImageTiling tiling,
                                  vk::FormatFeatureFlags featureFlags);

  /**
   * @brief Create all framebuffers for the pipeline.
   *
   * @param extent Extent that the framebuffers should have.
   */
  void _createFramebuffers(const sv::Extent &extent);

  /**
   * @brief Destroys all framebuffers of the pipeline.
   */
  void _destroyFramebuffers();

  /**
   * @brief Handler that is called when the underlying swapchain is recreated.
   *
   * @param eventType Must be the recreation event.
   * @param extent    The new extent that the framebuffer must have (at least).
   */
  void _handleSwapchainRecreation(core::Swapchain::Event eventType,
                                  const sv::Extent &extent);

public:
  /**
   * @brief Construct a Vulkan Pipeline.
   *
   * @param device            Device to use.
   * @param surface           Surface to use.
   * @param swapchain         Swapchain to use.
   * @param vert              Vertex Shader to use.
   * @param frag              Fragment Shader to use.
   * @param vertexDescription Description of Vertex handled by vertex shader.
   */
  VulkanPipeline(core::SharedDevice device, core::SharedSurface surface,
                 core::SharedSwapchain swapchain, core::SharedShader vert,
                 core::SharedShader frag,
                 const SVEL_NAMESPACE::VertexDescription &vertexDescription);

  /**
   * @brief Pipeline cannot be copied.
   */
  VulkanPipeline(const Pipeline &) = delete;

  /**
   * @brief Pipeline cannot be copied.
   *
   * @return VulkanPipeline& ~unused~
   */
  VulkanPipeline &operator=(const Pipeline &) = delete;

  /**
   * @brief Destroy the Pipeline
   */
  ~VulkanPipeline();

  /**
   * @brief Getter for Render Pass.
   *
   * @return const vk::RenderPass& The render pass
   */
  const vk::RenderPass &GetRenderPass() const { return _renderPass; }

  /**
   * @brief Getter for Framebuffers.
   *
   * @return const std::vector<vk::Framebuffer>& Framebuffers
   */
  const std::vector<vk::Framebuffer> &GetFramebuffers() const {
    return _framebuffers;
  }

  /**
   * @brief Getter for Vertex Size.
   *
   * @return const uint32_t Size of Vertex
   */
  const uint32_t &GetVertexSize() const { return _vertexInputSize; }

  /**
   * @brief Getter for Viewport.
   *
   * @return const vk::Viewport& Viewport
   */
  const vk::Viewport &GetViewport() const { return _viewport; }

  /**
   * @brief Getter for Layout of the Pipeline.
   *
   * @return const vk::PipelineLayout& Layout of the Pipeline
   */
  const vk::PipelineLayout &GetLayout() const { return _pipelineLayout; }

  /**
   * @brief Getter for the descriptor set group of this pipeline.
   *
   * @return core::descriptor::SharedSetGroup The set group of this pipeline.
   */
  core::descriptor::SharedSetGroup GetDescriptorGroup() { return _setGroup; }

  /**
   * @brief Notifies the pipeline that a new frame has started. TODO: Do this
   * smarter.
   */
  void NotifyNewFrame() final override;
};
SVEL_CLASS(VulkanPipeline)
SVEL_DOWNCAST_IMPL(VulkanPipeline, SVEL_NAMESPACE::Pipeline)

} // namespace renderer

#endif /* __RENDERER_PIPELINE_PIPELINE_H__ */