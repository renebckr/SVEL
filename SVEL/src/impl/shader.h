#ifndef __IMPL_SHADER_H__
#define __IMPL_SHADER_H__

#include "core/descriptor/group.h"
#include "core/shader.h"
#include <svel/detail/shader.h>

#include <unordered_set>

namespace SVEL_NAMESPACE {

class IShader::Impl {
private:
  core::SharedShader _shader;
  std::unordered_set<unsigned int> _setIds;

public:
  Impl(core::SharedDevice device, const std::string &filepath, Type shaderType);

  void AddSetLayout(unsigned int id, const SetLayout &setLayout);
};

} // namespace SVEL_NAMESPACE

#endif /* __IMPL_SHADER_H__ */