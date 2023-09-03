/**
 * @file app.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implements the Application Impl and Interface.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

// Local
#include "app.h"

// Internal
#include <svel/detail/image.h>

// STL
#include <memory>

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