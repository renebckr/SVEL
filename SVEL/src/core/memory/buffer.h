/**
 * @file buffer.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Buffer declaration.
 * @date 2023-03-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __CORE_MEMORY_BUFFER_H__
#define __CORE_MEMORY_BUFFER_H__

// Local
#include "device_memory.h"

// Internal
#include <core/device.h>
#include <util/vulkan_object.hpp>

// Vulkan
#include <vulkan/vulkan.hpp>

namespace core {

/**
 * @brief Buffer class that can be generically used to allocate any type of gpu
 * buffer.
 */
class Buffer : public util::VulkanAdapter<vk::Buffer> {
protected:
  /**
   * @brief Pointer to the used Device.
   */
  SharedDevice _device;

  /**
   * @brief Pointer to the memory.
   */
  UniqueDeviceMemory _memory;

  /**
   * @brief Size of the buffer.
   */
  size_t _size;

public:
  /**
   * @brief Construct a Buffer
   *
   * @param device    Device to use.
   * @param size      Size of the Buffer.
   * @param usage     Usage of the Buffer.
   * @param memFlags  What memory to use.
   */
  Buffer(SharedDevice device, size_t size, vk::BufferUsageFlags usage,
         vk::MemoryPropertyFlags memFlags);

  Buffer(const Buffer &) = delete;

  /**
   * @brief Destroy the Buffer
   */
  ~Buffer();

  /**
   * @brief Getter for Memory.
   *
   * @return const vk::DeviceMemory& The memory reference.
   */
  const vk::DeviceMemory &GetMemory();
};
SVEL_CLASS(Buffer)

} // namespace core

#endif /* __CORE_MEMORY_BUFFER_H__ */