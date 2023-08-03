/**
 * @file instance.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declares the Instance Wrapper
 * @date 2022-10-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __CORE_INSTANCE_H__
#define __CORE_INSTANCE_H__

// Internal
#include <svel/config.h>
#include <util/vulkan_object.hpp>

// Vulkan
#include <vulkan/vulkan.hpp>

// STL
#include <memory>

namespace core {

/**
 * @brief Wrapper for a vulkan instance.
 */
class Instance : public util::VulkanAdapter<vk::Instance> {
private:
  /**
   * @brief Extensions to use.
   */
  std::vector<const char *> _extensions;

  /**
   * @brief Layers to use.
   */
  std::vector<const char *> _layers;

  /**
   * @brief Application name.
   */
  std::string _appName;

  /**
   * @brief Engine name.
   */
  std::string _engineName;

  /**
   * @brief Application version.
   */
  uint32_t _appVersion;

  /**
   * @brief Engine version.
   */
  uint32_t _engineVersion;

  /**
   * @brief Create validation layers for debug purposes.
   */
  void _setupInstanceValidationLayers();

public:
  /**
   * @brief Construct a new Instance
   *
   * @param appName       Application name
   * @param appVersion    Application version
   * @param engineName    Engine name
   * @param engineVersion Engine version
   */
  Instance(const std::string &appName, uint32_t appVersion,
           const std::string &engineName, uint32_t engineVersion);

  /**
   * @brief Instance should not be copied.
   */
  Instance(const Instance &) = delete;

  /**
   * @brief Instance should not be copied.
   *
   * @return Instance& ~unused~
   */
  Instance &operator=(const Instance &) = delete;

  /**
   * @brief Destroy the Instance.
   */
  ~Instance();
};
SVEL_CLASS(Instance)

} // namespace core

#endif /* __CORE_INSTANCE_H__ */