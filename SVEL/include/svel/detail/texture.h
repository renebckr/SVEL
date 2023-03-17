#ifndef __SVEL_DETAL_TEXTURE_H__
#define __SVEL_DETAL_TEXTURE_H__

#include <memory>
#include <svel/config.h>

namespace SVEL_NAMESPACE {

class Texture;
SVEL_CLASS(Texture)

class Animation {
public:
  virtual ~Animation(){};
  virtual void Play() = 0;
  virtual void Reset() = 0;
  virtual void Stop() = 0;
  virtual bool IsPlaying() const = 0;

  // Frametime in seconds
  virtual void Update(float _frametime) = 0;
};
SVEL_CLASS(Animation)

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_DETAL_TEXTURE_H__ */