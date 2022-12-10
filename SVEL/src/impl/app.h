#ifndef __IMPL_APP_H__
#define __IMPL_APP_H__

#include <svel/detail/app.h>

#include <util/vulkan_object.hpp>

#include <vulkan/vulkan.hpp>

namespace SVEL_NAMESPACE {

class IApplication::Impl : public util::VulkanAdapter<vk::Instance> {
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
  Impl(const std::string &_appName, uint32_t _appVersion,
       const std::string &_engineName, uint32_t _engineVersion);

  Impl(const IApplication::Impl &) = delete;
  Impl &operator=(const IApplication::Impl &) = delete;

  /**
   * @brief Destroy the Impl
   */
  ~Impl();
};

} // namespace SVEL_NAMESPACE

#endif /* __IMPL_APP_H__ */