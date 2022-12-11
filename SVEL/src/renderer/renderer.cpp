#include "renderer.h"

using namespace SVEL_NAMESPACE;

VulkanRenderer::VulkanRenderer(sv::SharedIApplication parent,
                               core::SharedSurface surface) {
  _device = std::make_shared<core::Device>(parent, surface);
  _swapchain = std::make_shared<core::Swapchain>(_device, surface);
}