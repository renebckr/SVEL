#ifndef __RENDERER_MESH_MESH_H__
#define __RENDERER_MESH_MESH_H__

#include "core/descriptor/group.h"
#include "renderer/mesh/buffer.h"
#include "svel/util/array_proxy.hpp"
#include <svel/detail/mesh.h>

namespace SVEL_NAMESPACE {

class Mesh {
private:
  inline static uint32_t _bufferOffsets = 0;
  renderer::SharedBuffer _vbo;
  renderer::SharedBuffer _ibo;
  vk::IndexType _iboType;

public:
  Mesh(core::SharedDevice device, const vk::CommandPool &commandPool,
       const SVEL_NAMESPACE::ArrayProxy &nodes,
       const SVEL_NAMESPACE::ArrayProxy &indices, vk::IndexType iboType);

  void Draw(const vk::CommandBuffer &recordBuffer);
};

} // namespace SVEL_NAMESPACE

#endif /* __RENDERER_MESH_MESH_H__ */