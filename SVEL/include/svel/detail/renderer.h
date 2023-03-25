/**
 * @file renderer.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declares the Renderer interface.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __SVEL_DETAIL_RENDERER_H__
#define __SVEL_DETAIL_RENDERER_H__

// SVEL
#include <svel/config.h>
#include <svel/detail/image.h>
#include <svel/detail/material.h>
#include <svel/detail/mesh.h>
#include <svel/detail/pipeline.h>
#include <svel/detail/shader.h>
#include <svel/detail/texture.h>
#include <svel/util/array_proxy.hpp>

// STL
#include <memory>

namespace SVEL_NAMESPACE {

/**
 * @brief Manages all rendering related topics. TODO: Reduce size of this
 * bloated class.
 */
class Renderer {
public:
  /**
   * @brief Destroy the Renderer.
   */
  virtual ~Renderer() {}

  /**
   * @brief Loads a shader from the filesystem. Shader files need to have been
   * compiled first.
   *
   * @param filepath      Path to compiled shader file.
   * @param type          Type of the shader.
   * @return SharedShader The loaded shader.
   */
  virtual SharedShader LoadShader(const std::string &filepath,
                                  Shader::Type type) const = 0;

  /**
   * @brief Build a graphics pipeline from the provided shaders and the input
   * description of vertices.
   *
   * @param vert            The vertex shader.
   * @param frag            The fragment shader.
   * @param description     The input description of vertices.
   * @return SharedPipeline The created pipeline.
   */
  virtual SharedPipeline
  BuildPipeline(SharedShader vert, SharedShader frag,
                const VertexDescription &description) = 0;

  /**
   * @brief Binds the provided pipeline to the frame. Unbind() must be called
   * after usage is done.
   *
   * @param pipeline Pipeline to bind.
   */
  virtual void BindPipeline(SharedPipeline pipeline) = 0;

  /**
   * @brief Unbinds the currently bound pipeline. This method currently has to
   * be called explicitly. TODO: Do stuff like this automatically.
   */
  virtual void UnbindPipeline() = 0;

  /**
   * @brief Create a Texture from the provided image.
   *
   * @param image           Image to use for texture creation.
   * @return SharedTexture  The created texure.
   */
  virtual SharedTexture CreateTexture(SharedImage image) = 0;

  /**
   * @brief Create an Animation from the provided images.
   *
   * @param images            Images to use for the animation.
   * @param animationSpeed    How long the animation takes to be played back.
   *                          (in seconds) TODO: Why speed?
   * @param looping           If set to true, the animation will loop around.
   * @return SharedAnimation  The created animation.
   */
  virtual SharedAnimation
  CreateAnimation(const std::vector<SharedImage> &images, float animationSpeed,
                  bool looping) = 0;

  /**
   * @brief Create a Mesh with small indice count.
   *
   * @param nodes       The vertex data. Must be valid with the pipeline vertex
   *                    layout.
   * @param indices     The indices data.
   * @return SharedMesh The created Mesh.
   */
  virtual SharedMesh CreateMesh(const ArrayProxy &nodes,
                                const std::vector<uint16_t> &indices) = 0;

  /**
   * @brief Create a Mesh with large indice count.
   *
   * @param nodes       The vertex data. Must be valid with the pipeline vertex
   *                    layout.
   * @param indices     The indices data.
   * @return SharedMesh The created Mesh.
   */
  virtual SharedMesh CreateMesh(const ArrayProxy &nodes,
                                const std::vector<uint32_t> &indices) = 0;

  /**
   * @brief EXPERIMENTAL: Load OBJ file and creat meshes from it. Currently
   * limited support.
   *
   * @param objFile                   The OBJ-File to load.
   * @return std::vector<SharedMesh>  All of the meshes contained within the
   *                                  OBJ-File.
   */
  virtual std::vector<SharedMesh> LoadObjFile(const std::string &objFile) = 0;

  /**
   * @brief Setter for Scene Material. This material will be used once per frame
   * at the start of the frame. Should be set before the Render loop is started.
   * TODO: Make this smarter.
   *
   * @param material  The scene material to use.
   */
  virtual void SetSceneMaterial(SharedISceneMaterial material) = 0;

  /**
   * @brief Draw the provided mesh. This assumes that no material is required
   * for drawing.
   *
   * @param mesh Mesh to draw.
   */
  virtual void Draw(SharedMesh mesh) = 0;

  /**
   * @brief Draw the provided mesh with the material. This assumes that the
   * correct pipeline has been bound.
   *
   * @param mesh      Mesh to draw.
   * @param material  The material to use for the mesh.
   */
  virtual void Draw(SharedMesh mesh, SharedIMaterial material) = 0;
};
SVEL_CLASS(Renderer)

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_DETAIL_RENDERER_H__ */