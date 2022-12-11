/**
 * @file device.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Device declaration
 * @date 2022-10-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ENGINE_CORE_DEVICE_H
#define ENGINE_CORE_DEVICE_H

#include "core/surface.h"
#include <svel/detail/app.h>

#include <util/vulkan_object.hpp>

#include <vulkan/vulkan.hpp>

namespace core {

/**
 * @brief Device Wrapper that automatically finds the suitable physical device
 * to use.
 */
class Device : public util::VulkanAdapter<vk::Device> {
private:
  sv::SharedIApplication _parentApp;
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
  Device(sv::SharedIApplication parent, core::SharedSurface surface);

  Device(const Device &) = delete;
  Device &operator=(const Device &) = delete;

  /**
   * @brief Destroy the Device
   */
  ~Device();

  /**
   * @brief Getter for Physical Device
   *
   * @return vk::PhysicalDevice The Physical Device in use.
   */
  vk::PhysicalDevice GetPhysicalDevice() { return _selectedPhysicalDevice; }

  /**
   * @brief Getter for Graphics Queue Family
   *
   * @return uint32_t The GraphicsQueueFamily to use.
   */
  uint32_t GetGraphicsQueueFamily() { return _queueFamilyGraphics; }

  /**
   * @brief Getter for Present Queue Family
   *
   * @return uint32_t The PresentQueueFamily to use.
   */
  uint32_t GetPresentQueueFamily() { return _queueFamilyPresent; }
};
SVEL_CLASS(Device)

} // namespace core

#endif /* ENGINE_CORE_DEVICE_H */