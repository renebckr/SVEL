#ifndef __IMPL_RENDERER_H__
#define __IMPL_RENDERER_H__

#include "core/surface.h"
#include <svel/detail/renderer.h>

#include <core/device.h>
#include <core/swapchain.h>

class VulkanRenderer : public SVEL_NAMESPACE::Renderer {
private:
  core::SharedDevice _device;
  core::SharedSwapchain _swapchain;

public:
  VulkanRenderer(sv::SharedIApplication parent, core::SharedSurface surface);

  SVEL_NAMESPACE::SharedShader
  LoadShader(const std::string &filepath,
             SVEL_NAMESPACE::Shader::Type type) const override;

  core::SharedDevice GetDevice();
  core::SharedSwapchain GetSwapchain();
};
SVEL_CLASS(VulkanRenderer)

#endif /* __IMPL_RENDERER_H__ */