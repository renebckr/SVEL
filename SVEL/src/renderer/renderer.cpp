#include "renderer.h"
#include "core/barrier.h"
#include "renderer/pipeline/pipeline.h"
#include "svel/detail/pipeline.h"
#include "texture/animation.h"
#include "texture/texture.h"
#include <renderer/shader.h>

using namespace SVEL_NAMESPACE;

VulkanRenderer::VulkanRenderer(core::SharedInstance instance,
                               core::SharedSurface surface)
    : _surface(surface) {
  _device = std::make_shared<core::Device>(instance, _surface);
  _swapchain = std::make_shared<core::Swapchain>(_device, _surface);

  // Create Persistent Command pool
  vk::CommandPoolCreateInfo persistentCommandPoolInfo(
      vk::CommandPoolCreateFlagBits(), _device->GetGraphicsQueueFamily());
  _persistentCommandPool =
      _device->AsVulkanObj().createCommandPool(persistentCommandPoolInfo);
}

VulkanRenderer::~VulkanRenderer() {
  _device->AsVulkanObj().destroyCommandPool(_persistentCommandPool);
  _device->AsVulkanObj().waitIdle();
}

SharedShader VulkanRenderer::LoadShader(const std::string &filepath,
                                        Shader::Type type) const {
  return std::make_shared<VulkanShader>(_device, filepath, type);
}

core::SharedDevice VulkanRenderer::GetDevice() { return _device; }

core::SharedSwapchain VulkanRenderer::GetSwapchain() { return _swapchain; }

SharedPipeline
VulkanRenderer::BuildPipeline(SharedShader vert, SharedShader frag,
                              const VertexDescription &description) {
  return std::make_shared<renderer::VulkanPipeline>(
      _device, _surface, _swapchain, GetImpl(vert)->GetShader(),
      GetImpl(frag)->GetShader(), description);
}

void VulkanRenderer::BindPipeline(SharedPipeline pipeline) {
  _currentFrame->BindPipeline(renderer::GetImpl(pipeline));
}

void VulkanRenderer::UnbindPipeline() { _currentFrame->UnbindPipeline(); }

SharedTexture VulkanRenderer::CreateTexture(SharedImage image) {
  auto texture = std::make_shared<Texture>(_device, image);
  texture->Dispatch(_persistentCommandPool,
                    std::make_shared<core::Barrier>(_device));
  return texture;
}

SharedAnimation
VulkanRenderer::CreateAnimation(const std::vector<SharedImage> &images,
                                float animationSpeed, bool looping) {
  auto animation = std::make_shared<texture::VulkanAnimation>(
      _device, _persistentCommandPool, std::make_shared<core::Barrier>(_device),
      images, animationSpeed, looping);
  return animation;
}

void VulkanRenderer::SelectFrame(renderer::SharedFrame frame) {
  _currentFrame = frame;
}
