#include "app.h"
#include "impl/window.h"
#include "svel/detail/image.h"
#include <algorithm>
#include <memory>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vulkan/vulkan_core.h>

using namespace SVEL_NAMESPACE;

// --- IMPL ---

IApplication::Impl::Impl(const std::string &_appName, uint32_t _appVersion,
                         const std::string &_engineName,
                         uint32_t _engineVersion) {
  _instance = std::make_shared<core::Instance>(_appName, _appVersion,
                                               _engineName, _engineVersion);
}

core::SharedInstance IApplication::Impl::GetInstance() { return _instance; }

// --- INTERFACE ---

IApplication::IApplication(const std::string &appName, uint32_t appVersion) {
  __pImpl = std::make_shared<Impl>(appName, appVersion, "SVEL", 1);
}