#ifndef __IMPL_APP_H__
#define __IMPL_APP_H__

#include "core/instance.h"
#include <svel/detail/app.h>

#include <util/vulkan_object.hpp>

#include <vulkan/vulkan.hpp>

namespace SVEL_NAMESPACE {

class IApplication::Impl {
private:
  core::SharedInstance _instance;

public:
  /**
   * @brief Construct a new Instance
   *
   * @param _appName Application name
   * @param _appVersion Application version
   * @param _engineName Engine name
   * @param _engineVersion Engine version
   */
  Impl(const std::string &_appName, uint32_t _appVersion,
       const std::string &_engineName, uint32_t _engineVersion);

  Impl(const IApplication::Impl &) = delete;
  Impl &operator=(const IApplication::Impl &) = delete;

  core::SharedInstance GetInstance();
};

} // namespace SVEL_NAMESPACE

#endif /* __IMPL_APP_H__ */