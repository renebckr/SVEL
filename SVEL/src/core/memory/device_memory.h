#ifndef __CORE_MEMORY_DEVICE_MEMORY_H__
#define __CORE_MEMORY_DEVICE_MEMORY_H__

#include <core/device.h>
#include <util/vulkan_object.hpp>
#include <vulkan/vulkan.hpp>

namespace core {

/**
 * @brief Basic Wrapper around vulkan memory.
 */
class DeviceMemory : public util::VulkanAdapter<vk::DeviceMemory> {
private:
  /**
   * @brief Shared Pointer to the device.
   */
  SharedDevice _device;

  /**
   * @brief Finds an appropriate memory type for the given flags and type bits.
   *
   * @param memoryFlags Memory Flags to search for.
   * @param memoryTypeBits Memory Type Bits to search for.
   * @return uint32_t Found memory type
   */
  uint32_t _findMemoryType(vk::MemoryPropertyFlags memoryFlags,
                           uint32_t memoryTypeBits);

public:
  /**
   * @brief Construct Memory
   *
   * @param device Device to use.
   * @param requirements Requirements put on the memory.
   * @param properties Properties of the memory.
   */
  DeviceMemory(SharedDevice device, vk::MemoryRequirements &requirements,
               vk::MemoryPropertyFlags properties);

  DeviceMemory(const DeviceMemory &) = delete;

  /**
   * @brief Destroy the Memory.
   */
  ~DeviceMemory();
};
SVEL_CLASS(DeviceMemory)

} // namespace core

#endif /* __CORE_MEMORY_DEVICE_MEMORY_H__ */