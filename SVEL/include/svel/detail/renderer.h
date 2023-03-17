#ifndef __SVEL_DETAIL_RENDERER_H__
#define __SVEL_DETAIL_RENDERER_H__

#include "svel/detail/image.h"
#include "svel/detail/texture.h"
#include <svel/config.h>
#include <svel/detail/pipeline.h>
#include <svel/detail/shader.h>

#include <memory>

namespace SVEL_NAMESPACE {

class Renderer {
public:
  virtual ~Renderer() {}

  virtual SharedShader LoadShader(const std::string &filepath,
                                  Shader::Type type) const = 0;

  virtual SharedPipeline
  BuildPipeline(SharedShader vert, SharedShader frag,
                const VertexDescription &description) = 0;

  virtual void BindPipeline(SharedPipeline pipeline) = 0;
  virtual void UnbindPipeline() = 0;

  virtual SharedTexture CreateTexture(SharedImage image) = 0;

  virtual SharedAnimation
  CreateAnimation(const std::vector<SharedImage> &images, float animationSpeed,
                  bool looping) = 0;
};
SVEL_CLASS(Renderer)

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_DETAIL_RENDERER_H__ */