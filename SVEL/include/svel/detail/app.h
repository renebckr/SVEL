/**
 * @file app.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Application interface declaration.
 * @date 2023-03-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __SVEL_DETAIL_APP_H__
#define __SVEL_DETAIL_APP_H__

// SVEL
#include <svel/config.h>

// STL
#include <memory>

namespace SVEL_NAMESPACE {

/**
 * @brief Interface that the application should be derived from. As a user of
 * the library this replaces the main method.
 * HINT: To construct a window using this Application, you should use
 * shared_from_this()
 */
class IApplication {
  SVEL_PIMPL

public:
  /**
   * @brief Construct an IApplication with the application name and version.
   *
   * @param _appName    The name of the application.
   * @param _appVersion The version of the application encoded in an unsigned
   *                    int.
   */
  IApplication(const std::string &_appName, uint32_t _appVersion = 0);

  /**
   * @brief Destroys the IApplication.
   */
  virtual ~IApplication() {}

  /**
   * @brief Run method that should be implemented by the user. Will be called
   * once after startup.
   */
  virtual void Run() = 0;
};
SVEL_CLASS(IApplication)

} // namespace SVEL_NAMESPACE

// Macro to use for declaring your own derived application as the application
// that the engine should use.
#define SVEL_MAKE_APP(ConcreteApp_T)                                           \
  SVEL_NAMESPACE::SharedIApplication svel_make_app(int argc, char *argv[]) {   \
    return std::make_shared<ConcreteApp_T>(argc, argv);                        \
  }

/**
 * @brief Should be implemented by the user. Use SVEL_MAKE_APP() for this
 * purpose.
 *
 * @param argc                                ArgC passed to main at startup.
 * @param argv                                ArgV passed to main at startup.
 * @return SVEL_NAMESPACE::SharedIApplication The user defined application.
 */
SVEL_NAMESPACE::SharedIApplication svel_make_app(int argc, char *argv[]);

#endif /* __SVEL_DETAIL_APP_H__ */