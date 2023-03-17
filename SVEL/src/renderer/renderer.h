#ifndef __IMPL_RENDERER_H__
#define __IMPL_RENDERER_H__

#include <core/surface.h>
#include <renderer/pipeline/pipeline.h>
#include <svel/detail/renderer.h>
#include <util/downcast_impl.hpp>

#include <core/device.h>
#include <core/swapchain.h>

class VulkanRenderer final : public SVEL_NAMESPACE::Renderer {
private:
  core::SharedDevice _device;
  core::SharedSwapchain _swapchain;
  core::SharedSurface _surface;

  std::vector<renderer::VulkanPipeline> _pipelines;
  vk::CommandPool _persistentCommandPool;

public:
  VulkanRenderer(core::SharedInstance instance, core::SharedSurface surface);
  ~VulkanRenderer();

  core::SharedDevice GetDevice();
  core::SharedSwapchain GetSwapchain();

  SVEL_NAMESPACE::SharedShader
  LoadShader(const std::string &filepath,
             SVEL_NAMESPACE::Shader::Type type) const override;

  SVEL_NAMESPACE::SharedPipeline
  BuildPipeline(SVEL_NAMESPACE::SharedShader vert,
                SVEL_NAMESPACE::SharedShader frag,
                const SVEL_NAMESPACE::VertexDescription &description) override;

  SVEL_NAMESPACE::SharedTexture
  CreateTexture(SVEL_NAMESPACE::SharedImage image) override;

  SVEL_NAMESPACE::SharedAnimation
  CreateAnimation(const std::vector<SVEL_NAMESPACE::SharedImage> &images,
                  float animationSpeed, bool looping) override;
};
SVEL_CLASS(VulkanRenderer)
SVEL_DOWNCAST_IMPL(VulkanRenderer, SVEL_NAMESPACE::Renderer)

#endif /* __IMPL_RENDERER_H__ */