/**
 * @file buffer.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declaration of the buffer interface.
 * @date 2023-03-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __CORE_DESCRIPTOR_BUFFER_INTERFACE_H__
#define __CORE_DESCRIPTOR_BUFFER_INTERFACE_H__

// Internal
#include <svel/config.h>

// Vulkan
#include <vulkan/vulkan.hpp>

// STL
#include <memory>

namespace core::descriptor {

/**
 * @brief Interface for descriptor buffers.
 */
class IBuffer {
public:
  /**
   * @brief Possible results for the write operation. Needed Reallocation
   * indicates that the user is required to respect that the old buffer info is
   * now outdated.
   */
  enum class WriteResult { eSuccess, eFailure, eNeededReallocation };

protected:
  /**
   * @brief The info for the current buffer.
   */
  vk::DescriptorBufferInfo _bufferInfo;

  /**
   * @brief Properly converts the descriptor type to usage flags for buffer
   * allocation.
   *
   * @param _descriptorType The descriptor type to get the usage flags for.
   * @return vk::BufferUsageFlags Buffer usage flags.
   */
  vk::BufferUsageFlags _getUsage(vk::DescriptorType _descriptorType);

public:
  /**
   * @brief Destroy the IBuffer.
   */
  virtual ~IBuffer();

  /**
   * @brief Getter for the descriptor type of the buffer.
   *
   * @return vk::DescriptorType The descriptor type of the buffer.
   */
  virtual vk::DescriptorType GetType() const;

  /**
   * @brief Writes the data to the buffer.
   *
   * @param _data Data to be written. Should point to a valid location. Data
   * size is implicitly given by the binding element size.
   * @return WriteResult Result of the write operation.
   */
  virtual WriteResult Write(void *_data);

  /**
   * @brief Getter for the buffer index.
   *
   * @return uint32_t The buffer index.
   */
  virtual uint32_t GetBufferIndex();

  /**
   * @brief Getter for the buffer offset.
   *
   * @return uint32_t The buffer offset.
   */
  virtual uint32_t GetBufferOffset();

  /**
   * @brief Resets the buffer. This should be done to reuse old allocated
   * buffers.
   */
  virtual void Reset();

  /**
   * @brief Getter for the current buffer info.
   *
   * @return const vk::DescriptorBufferInfo& The buffer info.
   */
  const vk::DescriptorBufferInfo &GetBufferInfo() const;
};
SVEL_CLASS(IBuffer)

} // namespace core::descriptor

#endif /* __CORE_DESCRIPTOR_BUFFER_INTERFACE_H__ */