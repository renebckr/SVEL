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

// Internal
#include <core/barrier.h>
#include <core/device.h>
#include <core/memory/buffer.h>
#include <svel/util/array_proxy.hpp>
#include <util/vulkan_object.hpp>

// Vulkan
#include <vulkan/vulkan.hpp>

// STL
#include <atomic>
#include <memory>

namespace renderer {

/**
 * @brief Allows transfer of local memory to the gpu.
 */
class Buffer : public util::VulkanAdapter<vk::Buffer>,
               public std::enable_shared_from_this<Buffer> {
private:
  /**
   * @brief Signals whether the buffer is ready or not to be used.
   */
  std::atomic<bool> _bufferReady;

  /**
   * @brief The final buffer.
   */
  core::SharedBuffer _buffer;

  /**
   * @brief How many elements are in the buffer.
   */
  unsigned int _elementCount;

  /**
   * @brief Handles the completed transfer of the data.
   *
   * @param buffer The finalized buffer.
   */
  void _onCompletion(core::SharedBuffer buffer);

public:
  /**
   * @brief Transfers the data to the GPU.
   *
   * @param device      Device to use.
   * @param barrier     The barrier onto which can be waited for transfer
   * completion.
   * @param commandPool The command pool to use for this transfer.
   * @param data        The data to transfer to the gpu.
   * @param usage       The usage of the buffer.
   */
  void Transfer(core::SharedDevice device, core::Barrier *barrier,
                const vk::CommandPool &commandPool,
                const SVEL_NAMESPACE::ArrayProxy &data,
                vk::BufferUsageFlagBits usage);

  /**
   * @brief Getter for the element count.
   *
   * @return const unsigned int& How many elements are in the buffer.
   */
  const unsigned int &GetElementCount() const;

  /**
   * @brief Getter for the buffer status.
   *
   * @return true   Buffer is ready for usage.
   * @return false  Buffer transfer has not completed yet or failed.
   */
  bool IsBufferReady();
};
SVEL_CLASS(Buffer)

} // namespace renderer

#endif /* __RENDERER_MESH_BUFFER_H__ */