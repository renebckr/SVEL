#include "renderer.h"
#include "renderer/shader.h"

using namespace SVEL_NAMESPACE;

VulkanRenderer::VulkanRenderer(sv::SharedIApplication parent,
                               core::SharedSurface surface) {
  _device = std::make_shared<core::Device>(parent, surface);
  _swapchain = std::make_shared<core::Swapchain>(_device, surface);
}

SharedShader VulkanRenderer::LoadShader(const std::string &filepath,
                                        Shader::Type type) const {
  return std::make_shared<VulkanShader>(_device, filepath, type);
}