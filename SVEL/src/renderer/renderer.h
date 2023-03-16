#ifndef __IMPL_RENDERER_H__
#define __IMPL_RENDERER_H__

#include <core/surface.h>
#include <svel/detail/renderer.h>
#include <util/downcast_impl.hpp>

#include <core/device.h>
#include <core/swapchain.h>

class VulkanRenderer : public SVEL_NAMESPACE::Renderer {
private:
  core::SharedDevice _device;
  core::SharedSwapchain _swapchain;

public:
  VulkanRenderer(core::SharedInstance instance, core::SharedSurface surface);

  core::SharedDevice GetDevice();
  core::SharedSwapchain GetSwapchain();

  SVEL_NAMESPACE::SharedShader
  LoadShader(const std::string &filepath,
             SVEL_NAMESPACE::Shader::Type type) const;
};
SVEL_CLASS(VulkanRenderer)
SVEL_DOWNCAST_IMPL(VulkanRenderer, SVEL_NAMESPACE::Renderer)

#endif /* __IMPL_RENDERER_H__ */