/**
 * @file transfer_buffer.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Helper structure to transfer a buffer to the GPU
 * @date 2022-10-03
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __CORE_MEMORY_TRANSFER_BUFFER_HPP__
#define __CORE_MEMORY_TRANSFER_BUFFER_HPP__

// Local
#include "buffer.h"

// Internal
#include <core/barrier.h>
#include <svel/util/array_proxy.hpp>

// Vulkan
#include <vulkan/vulkan.hpp>

// STL
#include <atomic>
#include <functional>

namespace core {

/**
 * @brief Will transfer a buffer to the GPU. The resulting buffer will be handed
 * over via callback.
 */
class TransferBuffer : public std::enable_shared_from_this<TransferBuffer> {
public:
  /**
   * @brief Description for a callback when buffer is transferred.
   */
  using TransferCompletionHandler = std::function<void(SharedBuffer)>;

private:
  /**
   * @brief Device to use.
   */
  SharedDevice _device;

  /**
   * @brief Command Pool in use.
   */
  vk::CommandPool _commandPool;

  /**
   * @brief Command Buffer in use.
   */
  vk::CommandBuffer _commandBuffer;

  /**
   * @brief Buffer from which to send the data.
   */
  UniqueBuffer _stagingBuffer;

  /**
   * @brief Buffer which should receive the data.
   */
  SharedBuffer _transferredBuffer;

  /**
   * @brief Size of the buffer.
   */
  size_t _bufferSize;

  /**
   * @brief Callback to call on completion.
   */
  TransferCompletionHandler _completionCallback;

  /**
   * @brief Handles the transfer completion.
   */
  void onTransferCompleted();

public:
  /**
   * @brief Construct a Transfer Buffer.
   *
   * @param device              Device to use
   * @param commandPool         Command Pool from which to allocate a command
   *                            buffer
   * @param data                Data to send to the GPU
   * @param dataLength          Length of the Data
   * @param usage               Buffer Usage Flags to pass
   * @param completionCallback  Callback to use when transfer is completed
   */
  TransferBuffer(SharedDevice device, vk::CommandPool commandPool,
                 const SVEL_NAMESPACE::ArrayProxy &data,
                 vk::BufferUsageFlags usage,
                 TransferCompletionHandler completionCallback);

  /**
   * @brief Transfer the buffer.
   *
   * @param _barrier  Barrier on which to add the fence. This barrier has to be
   *                  waited on to complete transfer!
   */
  void TransferData(Barrier *barrier);
};

} // namespace core

#endif /* __CORE_MEMORY_TRANSFER_BUFFER_HPP__ */