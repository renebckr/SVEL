/**
 * @file mesh.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declaration of the mesh.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __RENDERER_MESH_MESH_H__
#define __RENDERER_MESH_MESH_H__

// Local
#include "buffer.h"

// Internal
#include <core/descriptor/group.h>
#include <svel/detail/mesh.h>
#include <svel/util/array_proxy.hpp>

namespace SVEL_NAMESPACE {

/**
 * @brief Combines Vertex Buffer Object and Index Buffer Object to form a mesh
 * that can be drawn by the renderer.
 */
class Mesh {
private:
  /**
   * @brief Variable that has to be in memory for the draw. Static to preserve
   * memory.
   */
  inline static uint32_t _bufferOffsets = 0;

  /**
   * @brief The Vertex Buffer Object.
   */
  renderer::SharedBuffer _vbo;

  /**
   * @brief The Index Buffer Object.
   */
  renderer::SharedBuffer _ibo;

  /**
   * @brief The type of the index buffer object.
   */
  vk::IndexType _iboType;

public:
  /**
   * @brief Construct a Mesh with the given data.
   *
   * @param device      Device to use.
   * @param commandPool Command pool to use.
   * @param nodes       The nodes that define the mesh point data.
   * @param indices     The indices that define the geometry of the mesh.
   * @param iboType     The data type of the indices.
   */
  Mesh(core::SharedDevice device, const vk::CommandPool &commandPool,
       const SVEL_NAMESPACE::ArrayProxy &nodes,
       const SVEL_NAMESPACE::ArrayProxy &indices, vk::IndexType iboType);

  /**
   * @brief Draw the mesh using the provided record buffer.
   *
   * @param recordBuffer The record buffer to use for recording the draw.
   */
  void Draw(const vk::CommandBuffer &recordBuffer);
};

} // namespace SVEL_NAMESPACE

#endif /* __RENDERER_MESH_MESH_H__ */