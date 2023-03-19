#ifndef __IMPL_RENDERER_H__
#define __IMPL_RENDERER_H__

#include "renderer/frame.h"
#include "svel/util/array_proxy.hpp"
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
  renderer::SharedFrame _currentFrame;
  vk::CommandBuffer *_currentRecordBuffer;
  vk::CommandPool _persistentCommandPool;

  SVEL_NAMESPACE::SharedISceneMaterial _sceneMaterial;
  renderer::SharedVulkanPipeline _boundPipeline;

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

  void BindPipeline(SVEL_NAMESPACE::SharedPipeline pipeline) override;
  void UnbindPipeline() override;

  SVEL_NAMESPACE::SharedTexture
  CreateTexture(SVEL_NAMESPACE::SharedImage image) override;

  SVEL_NAMESPACE::SharedAnimation
  CreateAnimation(const std::vector<SVEL_NAMESPACE::SharedImage> &images,
                  float animationSpeed, bool looping) override;

  SVEL_NAMESPACE::SharedMesh
  CreateMesh(const SVEL_NAMESPACE::ArrayProxy &nodes,
             const std::vector<uint16_t> &indices) override;

  SVEL_NAMESPACE::SharedMesh
  CreateMesh(const SVEL_NAMESPACE::ArrayProxy &nodes,
             const std::vector<uint32_t> &indices) override;

  void SetSceneMaterial(SVEL_NAMESPACE::SharedISceneMaterial material) override;

  void Draw(SVEL_NAMESPACE::SharedMesh mesh) override;

  void Draw(SVEL_NAMESPACE::SharedMesh mesh,
            SVEL_NAMESPACE::SharedIMaterial material) override;

  void SelectFrame(renderer::SharedFrame frame);
};
SVEL_CLASS(VulkanRenderer)
SVEL_DOWNCAST_IMPL(VulkanRenderer, SVEL_NAMESPACE::Renderer)

#endif /* __IMPL_RENDERER_H__ */