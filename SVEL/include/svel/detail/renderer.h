#ifndef __SVEL_DETAIL_RENDERER_H__
#define __SVEL_DETAIL_RENDERER_H__

#include <svel/config.h>
#include <svel/detail/shader.h>

#include <memory>

namespace SVEL_NAMESPACE {

class Renderer {
public:
  virtual ~Renderer() {}

  virtual SharedShader LoadShader(const std::string &filepath,
                                  Shader::Type type) const = 0;
};
SVEL_CLASS(Renderer)

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_DETAIL_RENDERER_H__ */