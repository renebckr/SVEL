/**
 * @file pipeline.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Vulkan Pipeline Wrapper implementation
 * @date 2022-10-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "pipeline.h"

#include <iostream>
#include <stdexcept>

using namespace renderer;
using namespace SVEL_NAMESPACE;

vk::Format VulkanPipeline::_getAttributeFormat(AttributeType _type,
                                               unsigned int _count) {
  // Translate attribute type to vk format
  switch (_type) {
  case AttributeType::SIGNED_FLOAT:
    switch (_count) {
    case 1:
      return vk::Format::eR32Sfloat;
    case 2:
      return vk::Format::eR32G32Sfloat;
    case 3:
      return vk::Format::eR32G32B32Sfloat;
    case 4:
      return vk::Format::eR32G32B32A32Sfloat;
    }
    break;
  }
  return vk::Format::eUndefined;
}

size_t VulkanPipeline::_getAttributeSize(AttributeType _type,
                                         unsigned int _count) {
  // Calculate size of the attribute type
  switch (_type) {
  case AttributeType::SIGNED_FLOAT:
    return sizeof(float) * _count;
  }
  return 0;
}

void VulkanPipeline::_buildVertexInputStateInfo(
    const VertexDescription &_vertexDescription) {
  // Check for the vertex description
  if (_vertexDescription.size() == 0) {
    _vertexInputStateInfo = vk::PipelineVertexInputStateCreateInfo(
        vk::PipelineVertexInputStateCreateFlagBits(), 0, nullptr, 0, nullptr);
    return;
  }

  // Create vertex attribute description
  _vertexInputSize = 0;
  unsigned int index = 0;
  for (auto description : _vertexDescription) {
    vk::Format format =
        _getAttributeFormat(description.first, description.second);
    if (format == vk::Format::eUndefined)
      throw std::runtime_error("Invalid Vertex Description.");

    _vertexInputAttributeDescriptions.push_back(
        vk::VertexInputAttributeDescription(index, 0, format,
                                            _vertexInputSize));
    index++;
    _vertexInputSize +=
        _getAttributeSize(description.first, description.second);
  }

  // Create info and description
  _vertexBindingDescription = vk::VertexInputBindingDescription(
      0, _vertexInputSize, vk::VertexInputRate::eVertex);

  _vertexInputStateInfo = vk::PipelineVertexInputStateCreateInfo(
      vk::PipelineVertexInputStateCreateFlagBits(), 1,
      &_vertexBindingDescription,
      (uint32_t)_vertexInputAttributeDescriptions.size(),
      _vertexInputAttributeDescriptions.data());
}

VulkanPipeline::VulkanPipeline(core::SharedDevice device,
                               core::SharedSurface surface,
                               core::SharedSwapchain swapchain,
                               core::SharedShader vert, core::SharedShader frag,
                               const VertexDescription &vertexDescription)
    : _device(device), _surface(surface), _swapchain(swapchain), _vert(vert),
      _frag(frag) {
  // Setup vertex input state
  _buildVertexInputStateInfo(vertexDescription);

  // Build descriptor Group
  std::vector<core::SharedShader> shaders = {frag, vert};
  _setGroup = std::make_shared<core::descriptor::SetGroup>(
      device, shaders, swapchain->GetSwapchainImageCount());

  // Fetch devices
  auto physicalDevice = _device->GetPhysicalDevice();
  auto vulkanDevice = _device->AsVulkanObj();

  // Get Viewport and setup scissor
  auto surfaceCapabilities =
      physicalDevice.getSurfaceCapabilitiesKHR(_surface->AsVulkanObj());
  _viewport =
      vk::Viewport(0.0f, 0.0f, (float)surfaceCapabilities.currentExtent.width,
                   (float)surfaceCapabilities.currentExtent.height, 0.0f, 1.0f);

  vk::Rect2D scissor({0, 0}, {surfaceCapabilities.currentExtent.width,
                              surfaceCapabilities.currentExtent.height});

  // Vertex Shader info
  vk::PipelineShaderStageCreateInfo pipelineCreateInfoVert(
      vk::PipelineShaderStageCreateFlagBits(), vk::ShaderStageFlagBits::eVertex,
      _vert->AsVulkanObj(), "main", nullptr);

  // Fragment Shader info
  vk::PipelineShaderStageCreateInfo pipelineCreateInfoFrag(
      vk::PipelineShaderStageCreateFlagBits(),
      vk::ShaderStageFlagBits::eFragment, _frag->AsVulkanObj(), "main",
      nullptr);

  std::vector<vk::PipelineShaderStageCreateInfo> pipelineShaderStages = {
      pipelineCreateInfoVert, pipelineCreateInfoFrag};

  // Create all Infos
  vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateInfo(
      vk::PipelineInputAssemblyStateCreateFlagBits(),
      vk::PrimitiveTopology::eTriangleList, VK_FALSE);

  vk::PipelineViewportStateCreateInfo pipelineViewportStateInfo(
      vk::PipelineViewportStateCreateFlagBits(), 1, &_viewport, 1, &scissor);

  vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateInfo(
      vk::PipelineRasterizationStateCreateFlagBits(), VK_FALSE, VK_FALSE,
      vk::PolygonMode::eFill, vk::CullModeFlagBits::eNone, vk::FrontFace(),
      VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f);

  vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateInfo(
      vk::PipelineMultisampleStateCreateFlagBits(), vk::SampleCountFlagBits::e1,
      VK_FALSE, 1.0f, nullptr, VK_FALSE, VK_FALSE);

  vk::PipelineColorBlendAttachmentState colorBlendAttachment(
      VK_TRUE, vk::BlendFactor::eSrcAlpha, vk::BlendFactor::eOneMinusSrcAlpha,
      vk::BlendOp::eAdd, vk::BlendFactor::eOne, vk::BlendFactor::eZero,
      vk::BlendOp::eAdd,
      vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
          vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

  vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateInfo(
      vk::PipelineColorBlendStateCreateFlagBits(), VK_FALSE, vk::LogicOp::eNoOp,
      1, &colorBlendAttachment, {0.0f, 0.0f, 0.0f, 0.0f});

  // We want viewport and scissor as dynamic states to resize easily
  vk::DynamicState dynamicStates[] = {vk::DynamicState::eViewport,
                                      vk::DynamicState::eScissor};
  vk::PipelineDynamicStateCreateInfo pipelineDynamicStateInfo(
      vk::PipelineDynamicStateCreateFlagBits(), 2, &dynamicStates[0]);

  vk::PipelineLayoutCreateInfo pipelineLayoutInfo(
      vk::PipelineLayoutCreateFlagBits(), _setGroup->GetLayouts(), {});

  _pipelineLayout = vulkanDevice.createPipelineLayout(pipelineLayoutInfo);

  // Render Pass START
  vk::AttachmentDescription attachmentDescription(
      vk::AttachmentDescriptionFlagBits(),
      _swapchain->GetSelectedFormat().format, vk::SampleCountFlagBits::e1,
      vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
      vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare,
      vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR);

  vk::AttachmentReference attachmentReference(
      0, vk::ImageLayout::eColorAttachmentOptimal);

  vk::SubpassDescription subpassDescription(
      vk::SubpassDescriptionFlagBits(), vk::PipelineBindPoint::eGraphics, 0,
      nullptr, // TODO: Later
      1, &attachmentReference, nullptr, nullptr, 0, nullptr);

  vk::SubpassDependency subpassDependency(
      VK_SUBPASS_EXTERNAL, 0, vk::PipelineStageFlagBits::eColorAttachmentOutput,
      vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::AccessFlagBits(),
      vk::AccessFlagBits::eColorAttachmentRead |
          vk::AccessFlagBits::eColorAttachmentWrite,
      vk::DependencyFlagBits());

  vk::RenderPassCreateInfo renderPassInfo(
      vk::RenderPassCreateFlagBits(), 1, &attachmentDescription, 1,
      &subpassDescription, 1, &subpassDependency);

  _renderPass = vulkanDevice.createRenderPass(renderPassInfo);
  // Render Pass END

  vk::GraphicsPipelineCreateInfo graphicsPipelineInfo(
      vk::PipelineCreateFlagBits(), 2, &pipelineShaderStages[0],
      &_vertexInputStateInfo, &pipelineInputAssemblyStateInfo, nullptr,
      &pipelineViewportStateInfo, &pipelineRasterizationStateInfo,
      &pipelineMultisampleStateInfo, nullptr, &pipelineColorBlendStateInfo,
      nullptr, //&pipelineDynamicStateInfo,
      _pipelineLayout, _renderPass, 0, VK_NULL_HANDLE);

  std::vector<vk::GraphicsPipelineCreateInfo> graphicPipelineInfos = {
      graphicsPipelineInfo};

  _vulkanObj =
      vulkanDevice.createGraphicsPipelines(VK_NULL_HANDLE, graphicPipelineInfos)
          .value.front();

  // Create Framebuffers
  vk::FramebufferCreateInfo framebufferCreateInfo(
      vk::FramebufferCreateFlagBits(), _renderPass, 1, nullptr,
      surfaceCapabilities.currentExtent.width,
      surfaceCapabilities.currentExtent.height, 1);

  for (auto imageView : _swapchain->GetImageViews()) {
    framebufferCreateInfo.pAttachments = &imageView;
    _framebuffers.push_back(
        vulkanDevice.createFramebuffer(framebufferCreateInfo));
  }
}

VulkanPipeline::~VulkanPipeline() {
  auto vulkanDevice = _device->AsVulkanObj();

  for (auto framebuffer : _framebuffers)
    vulkanDevice.destroyFramebuffer(framebuffer);
  vulkanDevice.destroyPipeline(_vulkanObj);
  vulkanDevice.destroyRenderPass(_renderPass);
  vulkanDevice.destroyPipelineLayout(_pipelineLayout);
}

void VulkanPipeline::NotifyNewFrame() { _setGroup->NotifyNewFrame(); }