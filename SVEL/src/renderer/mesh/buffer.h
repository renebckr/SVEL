/**
 * @file buffer.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declares a Buffer that can be used for rendering
 * @date 2022-10-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __RENDERER_MESH_BUFFER_H__
#define __RENDERER_MESH_BUFFER_H__

#include "core/barrier.h"
#include "core/device.h"
#include "core/memory/buffer.h"
#include "svel/util/array_proxy.hpp"
#include "util/vulkan_object.hpp"
#include <atomic>
#include <memory>
#include <vulkan/vulkan.hpp>

namespace renderer {
class Buffer : public util::VulkanAdapter<vk::Buffer>,
               public std::enable_shared_from_this<Buffer> {
private:
  std::atomic<bool> _bufferReady;
  core::SharedBuffer _buffer;
  unsigned int _elementCount;

  void _onCompletion(core::SharedBuffer buffer);

public:
  void Transfer(core::SharedDevice device, core::Barrier *barrier,
                const vk::CommandPool &commandPool,
                const SVEL_NAMESPACE::ArrayProxy &data,
                vk::BufferUsageFlagBits usage);

  const unsigned int &GetElementCount() const;

  bool IsBufferReady();
};
SVEL_CLASS(Buffer)

} // namespace renderer

#endif /* __RENDERER_MESH_BUFFER_H__ */