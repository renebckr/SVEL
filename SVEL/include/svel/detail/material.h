#ifndef __SVEL_DETAIL_MATERIAL_H__
#define __SVEL_DETAIL_MATERIAL_H__

#include "svel/detail/pipeline.h"
#include "svel/detail/texture.h"
#include <memory>
#include <svel/config.h>
#include <type_traits>

namespace SVEL_NAMESPACE {

class IMaterial {
  SVEL_PIMPL

protected:
  bool _addAttribute(unsigned int set, unsigned int binding, void *data,
                     unsigned int dataSize);

protected:
  IMaterial(SharedPipeline pipeline);

  template <typename T, typename = std::enable_if_t<std::is_pointer<T>::value>>
  inline bool AddAttribute(unsigned int set, unsigned int binding, T data) {
    return _addAttribute(set, binding, data,
                         sizeof(typename std::remove_pointer<T>::type));
  }

  bool SetTexture(unsigned int set, unsigned int binding,
                  SharedTexture texture);
};
SVEL_CLASS(IMaterial)

class ISceneMaterial : public IMaterial {
protected:
  ISceneMaterial(SharedPipeline pipeline);
};
SVEL_CLASS(ISceneMaterial)

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_DETAIL_MATERIAL_H__ */