/**
 * @file renderer.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declares the VulkanRenderer.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __RENDERER_RENDERER_H__
#define __RENDERER_RENDERER_H__

// Local
#include "frame.h"

// Internal
#include <core/device.h>
#include <core/surface.h>
#include <core/swapchain.h>
#include <renderer/pipeline/pipeline.h>
#include <svel/detail/renderer.h>
#include <svel/util/array_proxy.hpp>
#include <util/downcast_impl.hpp>

/**
 * @brief Concrete implementation of the Renderer Interface for Vulkan.
 */
class VulkanRenderer final : public SVEL_NAMESPACE::Renderer {
private:
  /**
   * @brief Device to use.
   */
  core::SharedDevice _device;

  /**
   * @brief Swapchain to use.
   */
  core::SharedSwapchain _swapchain;

  /**
   * @brief Surface to use.
   */
  core::SharedSurface _surface;

  /**
   * @brief The currently active frame.
   */
  renderer::SharedFrame _currentFrame;

  /**
   * @brief The current command buffer used for recording.
   */
  vk::CommandBuffer *_currentRecordBuffer;

  /**
   * @brief A persistent command pool for general use.
   */
  vk::CommandPool _persistentCommandPool;

  /**
   * @brief The currently used scene material.
   */
  SVEL_NAMESPACE::SharedISceneMaterial _sceneMaterial;

  /**
   * @brief The currently bound pipeline.
   */
  renderer::SharedVulkanPipeline _boundPipeline;

public:
  /**
   * @brief Construct a Vulkan Renderer.
   *
   * @param instance  Instance on which the renderer operates.
   * @param surface   Surface that the renderer is supposed to use.
   */
  VulkanRenderer(core::SharedInstance instance, core::SharedSurface surface);

  /**
   * @brief Destroy the Vulkan Renderer.
   */
  ~VulkanRenderer();

  /**
   * @brief Getter for the device that the renderer uses.
   *
   * @return core::SharedDevice The device of the renderer.
   */
  core::SharedDevice GetDevice();

  /**
   * @brief Getter for the swapchain that the renderer uses.
   *
   * @return core::SharedSwapchain The swapchain of the renderer.
   */
  core::SharedSwapchain GetSwapchain();

  /**
   * @brief Implementation of the LoadShader Interface.
   *
   * @param filepath                      File to load.
   * @param type                          Type of shader.
   * @return SVEL_NAMESPACE::SharedShader Created shader.
   */
  SVEL_NAMESPACE::SharedShader
  LoadShader(const std::string &filepath,
             SVEL_NAMESPACE::Shader::Type type) const override;

  /**
   * @brief Implementation of the BuildPipeline Interface.
   *
   * @param vert                            Vertex shader to use.
   * @param frag                            Frag shader to use.
   * @param description                     Vertex description.
   * @return SVEL_NAMESPACE::SharedPipeline Created pipeline.
   */
  SVEL_NAMESPACE::SharedPipeline
  BuildPipeline(SVEL_NAMESPACE::SharedShader vert,
                SVEL_NAMESPACE::SharedShader frag,
                const SVEL_NAMESPACE::VertexDescription &description) override;

  /**
   * @brief Implementation of the BindPipeline Interface.
   *
   * @param pipeline Pipeline to bind.
   */
  void BindPipeline(SVEL_NAMESPACE::SharedPipeline pipeline) override;

  /**
   * @brief Implementation of the UnbindPipeline Interface.
   */
  void UnbindPipeline() override;

  /**
   * @brief Implementation of the CreateTexture Interface.
   *
   * @param image                           Image used for texture.
   * @return SVEL_NAMESPACE::SharedTexture  Created texture.
   */
  SVEL_NAMESPACE::SharedTexture
  CreateTexture(SVEL_NAMESPACE::SharedImage image) override;

  /**
   * @brief Implementation of the CreateAnimation Interface.
   *
   * @param images                            Images of the animation.
   * @param animationSpeed                    Period in seconds.
   * @param looping                           Looping animation?
   * @return SVEL_NAMESPACE::SharedAnimation  Created Animation.
   */
  SVEL_NAMESPACE::SharedAnimation
  CreateAnimation(const std::vector<SVEL_NAMESPACE::SharedImage> &images,
                  float animationSpeed, bool looping) override;

  /**
   * @brief Implementation of the CreateMesh Interface.
   *
   * @param nodes                       Point data.
   * @param indices                     Indices of the mesh.
   * @return SVEL_NAMESPACE::SharedMesh Created Mesh.
   */
  SVEL_NAMESPACE::SharedMesh
  CreateMesh(const SVEL_NAMESPACE::ArrayProxy &nodes,
             const std::vector<uint16_t> &indices) override;

  /**
   * @brief Implementation of the CreateMesh Interface.
   *
   * @param nodes                       Point data.
   * @param indices                     Indices of the mesh.
   * @return SVEL_NAMESPACE::SharedMesh Created Mesh.
   */
  SVEL_NAMESPACE::SharedMesh
  CreateMesh(const SVEL_NAMESPACE::ArrayProxy &nodes,
             const std::vector<uint32_t> &indices) override;

  /**
   * @brief Implementation of the LoadObjFile Interface.
   *
   * @param objFile                                   File to load.
   * @return std::vector<SVEL_NAMESPACE::SharedMesh>  Loaded Meshes.
   */
  std::vector<SVEL_NAMESPACE::SharedMesh>
  LoadObjFile(const std::string &objFile) override;

  /**
   * @brief Implementation of the SetSceneMaterial Interface.
   *
   * @param material The scene material.
   */
  void SetSceneMaterial(SVEL_NAMESPACE::SharedISceneMaterial material) override;

  /**
   * @brief Implementation of the Draw Interface.
   *
   * @param mesh Mesh to draw.
   */
  void Draw(SVEL_NAMESPACE::SharedMesh mesh) override;

  /**
   * @brief Implementation of the Draw Interface.
   *
   * @param mesh      Mesh to draw.
   * @param material  Material to use.
   */
  void Draw(SVEL_NAMESPACE::SharedMesh mesh,
            SVEL_NAMESPACE::SharedIMaterial material) override;

  /**
   * @brief Switch out the frame to which the renderer draws to.
   *
   * @param frame The new frame to draw to.
   */
  void SelectFrame(renderer::SharedFrame frame);

  /**
   * @brief Lets the renderer recreate the swapchain.
   */
  void RecreateSwapchain();
};
SVEL_CLASS(VulkanRenderer)
SVEL_DOWNCAST_IMPL(VulkanRenderer, SVEL_NAMESPACE::Renderer)

#endif /* __RENDERER_RENDERER_H__ */