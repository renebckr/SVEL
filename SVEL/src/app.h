/**
 * @file app.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declares the Application Impl.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __APP_H__
#define __APP_H__

// Internal
#include "core/instance.h"
#include "svel/detail/texture.h"
#include <svel/detail/app.h>
#include <util/vulkan_object.hpp>

// Vulkan
#include <vulkan/vulkan.hpp>

namespace SVEL_NAMESPACE {

/**
 * @brief Declaration of the Application Impl for the pImpl Idiom.
 */
class IApplication::Impl {
private:
  /**
   * @brief Vulkan instance that the application uses.
   */
  core::SharedInstance _instance;

public:
  /**
   * @brief Construct a new Application Implementation
   *
   * @param appName       Application name
   * @param appVersion    Application version
   * @param engineName    Engine name
   * @param engineVersion Engine version
   */
  Impl(const std::string &appName, uint32_t appVersion,
       const std::string &engineName, uint32_t engineVersion);

  /**
   * @brief Cannot be copied.
   */
  Impl(const IApplication::Impl &) = delete;

  /**
   * @brief Cannot be copied.
   *
   * @return Impl& ~unused~
   */
  Impl &operator=(const IApplication::Impl &) = delete;

  /**
   * @brief Getter for the Vulkan Instance.
   *
   * @return core::SharedInstance The vulkan instance.
   */
  core::SharedInstance GetInstance();
};

} // namespace SVEL_NAMESPACE

#endif /* __APP_H__ */