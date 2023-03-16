/**
 * @file instance.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declares the Instance Wrapper
 * @date 2022-10-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ENGINE_CORE_INSTANCE_H
#define ENGINE_CORE_INSTANCE_H

#include <memory>
#include <svel/config.h>
#include <util/vulkan_object.hpp>
#include <vulkan/vulkan.hpp>

namespace core {

/**
 * @brief Wrapper for a vulkan instance.
 */
class Instance : public util::VulkanAdapter<vk::Instance> {
private:
  /**
   * @brief Extensions and layers to use.
   */
  std::vector<const char *> extensions, layers;

  /**
   * @brief Application and engine name.
   */
  std::string appName, engineName;

  /**
   * @brief Application and engine version.
   */
  uint32_t appVersion, engineVersion;

  /**
   * @brief Create validation layers for debug purposes.
   */
  void setupInstanceValidationLayers();

public:
  /**
   * @brief Construct a new Instance
   *
   * @param _appName Application name
   * @param _appVersion Application version
   * @param _engineName Engine name
   * @param _engineVersion Engine version
   */
  Instance(const std::string &_appName, uint32_t _appVersion,
           const std::string &_engineName, uint32_t _engineVersion);

  Instance(const Instance &) = delete;
  Instance &operator=(const Instance &) = delete;

  /**
   * @brief Destroy the Instance
   */
  ~Instance();
};
SVEL_CLASS(Instance)

} // namespace core

#endif /* ENGINE_CORE_INSTANCE_H */