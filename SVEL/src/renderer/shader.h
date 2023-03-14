#ifndef __RENDERER_SHADER_H__
#define __RENDERER_SHADER_H__

#include "core/descriptor/group.h"
#include "core/shader.h"
#include <svel/detail/shader.h>

#include <unordered_set>

class VulkanShader : public SVEL_NAMESPACE::Shader {
private:
  core::SharedShader _shader;
  std::unordered_set<unsigned int> _setIds;

public:
  VulkanShader(core::SharedDevice device, const std::string &filepath,
               Type shaderType);

  SVEL_NAMESPACE::Shader &
  AddSetLayout(unsigned int id,
               const SVEL_NAMESPACE::SetLayout &setLayout) override;
};

#endif /* __RENDERER_SHADER_H__ */