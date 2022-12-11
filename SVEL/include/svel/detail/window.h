#ifndef __SVEL_DETAIL_WINDOW_H__
#define __SVEL_DETAIL_WINDOW_H__

#include <svel/config.h>
#include <svel/detail/app.h>
#include <svel/detail/renderer.h>
#include <svel/util/structs.hpp>

#include <memory>

namespace SVEL_NAMESPACE {

class IWindow {
  SVEL_PIMPL

public:
  IWindow(SharedIApplication parent, const std::string &title,
          const Extent &windowSize);

  SharedRenderer GetRenderer();
};

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_DETAIL_WINDOW_H__ */