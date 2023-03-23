/**
 * @file dynamic_buffer.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declaration of the Dynamic Buffer.
 * @date 2023-03-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __CORE_DESCRIPTOR_DYNAMIC_BUFFER_H__
#define __CORE_DESCRIPTOR_DYNAMIC_BUFFER_H__

// Local
#include <core/descriptor/buffer.h>

// Internal
#include <core/device.h>
#include <core/memory/buffer.h>

// Vulkan
#include <vulkan/vulkan.hpp>

#ifndef SVEL_DESCRIPTOR_DYNAMIC_BUFFER_SIZE
/**
 * @brief How many elements should be able to be fit insie a dynamic buffer per
 * default?
 */
#define SVEL_DESCRIPTOR_DYNAMIC_BUFFER_SIZE 1000
#endif /* SVEL_DESCRIPTOR_DYNAMIC_BUFFER_SIZE */

namespace core::descriptor {

/**
 * @brief Implements the Buffer interface and allows storage expansions by being
 * similar to a dynamic array.
 */
class DynamicBuffer final : public IBuffer {
private:
  /**
   * @brief Device to use.
   */
  core::SharedDevice _device;

  /**
   * @brief Contains all buffers and their memory adresses for fast access.
   */
  std::vector<std::pair<void *, core::UniqueBuffer>> _buffers;

  /**
   * @brief How many elements have already been written to the buffer.
   */
  size_t _head = 0;

  /**
   * @brief How large a single buffer is.
   */
  size_t _bufferSize;

  /**
   * @brief Size of an element in bytes.
   */
  size_t _elementSize;

  /**
   * @brief How many bytes are required in the buffer to represent an element.
   * (The element has to be aligned inside of the buffer)
   */
  size_t _alignedElementSize;

  /**
   * @brief Type of the Buffer.
   */
  vk::DescriptorType _descriptorType;

  /**
   * @brief Usage flags for the buffers that will be created.
   */
  vk::BufferUsageFlags _usage;

  /**
   * @brief Which Buffer is currently being used?
   */
  uint32_t _currentBufferIndex = 0;

  /**
   * @brief The current offset inside of the current buffer with respect to
   * alignment.
   */
  uint32_t _bufferOffset = 0;

  /**
   * @brief Allocates a new buffer and adds it to the existing buffers.
   */
  void _allocateBuffer();

  /**
   * @brief Updates the buffer info of the base class to be up to date to the
   * current buffer.
   *
   * @param _bufferIndex Buffer index of the buffer to use for the buffer info.
   */
  void _updateBufferInfo(uint32_t _bufferIndex);

public:
  /**
   * @brief Construct a Dynamic Buffer.
   *
   * @param device      Device to use.
   * @param elementSize Size of an element that will be written to the buffer in
   *                    bytes.
   * @param type        Type of the buffer.
   */
  DynamicBuffer(std::shared_ptr<core::Device> device, size_t elementSize,
                vk::DescriptorType type);

  /**
   * @brief Destroys the dynamic buffer.
   */
  ~DynamicBuffer();

  /**
   * @brief Getter for the buffer type.
   *
   * @return vk::DescriptorType Type of buffer.
   */
  vk::DescriptorType GetType() const override;

  /**
   * @brief Tries to write to the end of the current buffer. Will allocate new
   * buffer if the old buffer would overflow.
   *
   * @param data         Data to write.
   * @return WriteResult Result of the Write. Needed Allocation implies, that a
   *                     new buffer had to be allocated.
   */
  WriteResult Write(void *data) override;

  /**
   * @brief Getter for the current buffer index. The index is consistent and
   * will always refer to the same buffer.
   *
   * @return uint32_t Index of the current buffer.
   */
  uint32_t GetBufferIndex() override;

  /**
   * @brief Getter for the current buffer offset inside of the current buffer.
   *
   * @return uint32_t The current buffer offset.
   */
  uint32_t GetBufferOffset() override;

  /**
   * @brief Resets the dynamic buffer. New data will overwrite existing old
   * data. Old data won't be wiped and old buffers will not be free'd.
   */
  void Reset() override;
};
} // namespace core::descriptor

#endif /* __CORE_DESCRIPTOR_DYNAMIC_BUFFER_H__ */