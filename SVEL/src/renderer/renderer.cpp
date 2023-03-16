#include "renderer.h"
#include "renderer/pipeline/pipeline.h"
#include "svel/detail/pipeline.h"
#include <renderer/shader.h>

using namespace SVEL_NAMESPACE;

VulkanRenderer::VulkanRenderer(core::SharedInstance instance,
                               core::SharedSurface surface)
    : _surface(surface) {
  _device = std::make_shared<core::Device>(instance, _surface);
  _swapchain = std::make_shared<core::Swapchain>(_device, _surface);
}

SharedShader VulkanRenderer::LoadShader(const std::string &filepath,
                                        Shader::Type type) const {
  return std::make_shared<VulkanShader>(_device, filepath, type);
}

SharedPipeline
VulkanRenderer::BuildPipeline(SharedShader vert, SharedShader frag,
                              const VertexDescription &description) {

  return std::make_shared<renderer::VulkanPipeline>(
      _device, _surface, _swapchain, GetImpl(vert)->GetShader(),
      GetImpl(frag)->GetShader(), description);
}