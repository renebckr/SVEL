/**
 * @file vulkan_object.hpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declares the VulkanObject Interface
 * @date 2022-10-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __UTIL_VULKAN_OBJECT_HPP__
#define __UTIL_VULKAN_OBJECT_HPP__

namespace util {

/**
 * @brief Vulkan Adapter usable to wrap vulkan types into a usable class
 * structure.
 *
 * @tparam VulkanType Type of the wrapped vulkan object.
 */
template <typename VulkanType> class VulkanAdapter {
protected:
  /**
   * @brief The wrapped vulkan object.
   */
  VulkanType _vulkanObj;

public:
  /**
   * @brief Getter for the vulkan object.
   *
   * @return const VulkanType& The Vulkan object.
   */
  const VulkanType &AsVulkanObj() { return _vulkanObj; }
};

} // namespace util

#endif /* __UTIL_VULKAN_OBJECT_HPP__ */