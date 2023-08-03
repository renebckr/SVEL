/**
 * @file static_buffer.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declaration of the StaticBuffer.
 * @date 2023-03-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __CORE_DESCRIPTOR_STATIC_BUFFER_H__
#define __CORE_DESCRIPTOR_STATIC_BUFFER_H__

// Local
#include "buffer.h"

// Internal
#include <core/device.h>
#include <core/memory/buffer.h>

namespace core::descriptor {

/**
 * @brief Implementation of the Buffer Interface. This buffer only allows for
 * one write. It should thus be regarded as static in the context of a frame.
 */
class StaticBuffer final : public IBuffer {
private:
  /**
   * @brief Device to use.
   */
  core::SharedDevice _device;

  /**
   * @brief The buffer that will be used.
   */
  core::UniqueBuffer _buffer;

  /**
   * @brief The size of the buffer in bytes.
   */
  size_t _size;

  /**
   * @brief Type of the buffer.
   */
  vk::DescriptorType _descriptorType;

  /**
   * @brief Direct pointer to the mapped memory of the buffer.
   */
  void *_memoryPointer;

public:
  /**
   * @brief Construct a Static Buffer.
   *
   * @param device          Device to use.
   * @param descriptorType  Type of the buffer.
   * @param size            Size of the buffer in bytes.
   */
  StaticBuffer(core::SharedDevice device, vk::DescriptorType descriptorType,
               size_t size);

  /**
   * @brief Destroy the Static Buffer
   */
  ~StaticBuffer();

  /**
   * @brief Getter for the type of the buffer.
   *
   * @return vk::DescriptorType Type of the buffer.
   */
  vk::DescriptorType GetType() const override;

  /**
   * @brief Writes the data to the buffer. Will overwrite anything that was
   * written before.
   *
   * @param _data        Data to write.
   * @return WriteResult Always Success.
   */
  WriteResult Write(void *_data) override;

  /**
   * @brief Does nothing for a static buffer as every write resets the buffer.
   */
  void Reset() override;
};

} // namespace core::descriptor

#endif /* __CORE_DESCRIPTOR_STATIC_BUFFER_H__ */