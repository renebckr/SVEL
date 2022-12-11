#ifndef __SVEL_DETAIL_APP_H__
#define __SVEL_DETAIL_APP_H__

#include <svel/config.h>

#include <memory>

namespace SVEL_NAMESPACE {

class IApplication {
  SVEL_PIMPL

public:
  IApplication(const std::string &_appName, uint32_t _appVersion = 0);
  virtual ~IApplication() {}

  virtual void Run() = 0;
};
SVEL_CLASS(IApplication)

} // namespace SVEL_NAMESPACE

#define SVEL_MAKE_APP(ConcreteApp_T)                                           \
  SVEL_NAMESPACE::SharedIApplication svel_make_app(int argc, char *argv[]) {   \
    return std::make_shared<ConcreteApp_T>(argc, argv);                        \
  }

SVEL_NAMESPACE::SharedIApplication svel_make_app(int argc, char *argv[]);

#endif /* __SVEL_DETAIL_APP_H__ */