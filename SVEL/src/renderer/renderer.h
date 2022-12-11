#ifndef __IMPL_RENDERER_H__
#define __IMPL_RENDERER_H__

#include "core/surface.h"
#include <svel/detail/renderer.h>

#include <core/device.h>
#include <core/swapchain.h>

class VulkanRenderer : public sv::Renderer {
private:
  core::SharedDevice _device;
  core::SharedSwapchain _swapchain;

public:
  VulkanRenderer(sv::SharedIApplication parent, core::SharedSurface surface);

  core::SharedDevice GetDevice();
  core::SharedSwapchain GetSwapchain();
};
SVEL_CLASS(VulkanRenderer)

#endif /* __IMPL_RENDERER_H__ */