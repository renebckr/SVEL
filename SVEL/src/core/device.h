/**
 * @file device.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Device declaration
 * @date 2022-10-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __CORE_DEVICE_H__
#define __CORE_DEVICE_H__

// Local
#include "surface.h"

// Internal
#include <util/vulkan_object.hpp>

namespace core {

/**
 * @brief Device Wrapper that automatically finds the suitable physical device
 * to use.
 */
class Device : public util::VulkanAdapter<vk::Device> {
private:
  /**
   * @brief Instance to use.
   */
  core::SharedInstance _instance;

  /**
   * @brief Surface to use.
   */
  core::SharedSurface _surface;

  /**
   * @brief Extensions that should be enabled on the physical device.
   */
  std::vector<const char *> _extensions;

  /**
   * @brief Features that should be enabled on the physical device.
   */
  vk::PhysicalDeviceFeatures _features = {};

  /**
   * @brief Priorities for all selected queues.
   */
  std::vector<float> _queuePriorities;

  /**
   * @brief The Queue family responsible for graphics.
   */
  unsigned int _queueFamilyGraphics;

  /**
   * @brief The Queue family responsible for presentation.
   */
  unsigned int _queueFamilyPresent;

  /**
   * @brief How many queues are available.
   */
  unsigned int _queueCount;

  /**
   * @brief Which Physical Device was selected for use.
   */
  vk::PhysicalDevice _selectedPhysicalDevice;

  /**
   * @brief Prioritize all the Physical Devices.
   *
   * @param devices Devices to prioritize
   * @return std::vector<unsigned int> Prioritized index list
   */
  std::vector<unsigned int>
  prioritizePhysicalDevices(const std::vector<vk::PhysicalDevice> &devices);

  /**
   * @brief Find Queue Families that satisfy the constraints.
   *
   * @param device Physical Device to use.
   * @param constraintQueueCount Queue count constraint.
   * @return std::pair<unsigned int, unsigned int> Graphics Queue, Present Queue
   */
  std::pair<unsigned int, unsigned int>
  findQueueFamilies(vk::PhysicalDevice device, uint32_t &constraintQueueCount);

public:
  /**
   * @brief Construct a Device with the provided instance and surface.
   *
   * @param instance  Instance to use.
   * @param surface   Surface to use.
   */
  Device(core::SharedInstance instance, core::SharedSurface surface);

  /**
   * @brief The device should not be copied.
   */
  Device(const Device &) = delete;

  /**
   * @brief The device should not be copied.
   *
   * @return Device& ~unused~
   */
  Device &operator=(const Device &) = delete;

  /**
   * @brief Destroy the Device.
   */
  ~Device();

  /**
   * @brief Getter for Physical Device.
   *
   * @return vk::PhysicalDevice The Physical Device in use.
   */
  vk::PhysicalDevice GetPhysicalDevice() { return _selectedPhysicalDevice; }

  /**
   * @brief Getter for Graphics Queue Family.
   *
   * @return uint32_t The GraphicsQueueFamily to use.
   */
  uint32_t GetGraphicsQueueFamily() { return _queueFamilyGraphics; }

  /**
   * @brief Getter for Present Queue Family.
   *
   * @return uint32_t The PresentQueueFamily to use.
   */
  uint32_t GetPresentQueueFamily() { return _queueFamilyPresent; }
};
SVEL_CLASS(Device)

} // namespace core

#endif /* __CORE_DEVICE_H__ */