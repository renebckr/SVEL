#ifndef ENGINE_TEXTURE_ANIMATION_H
#define ENGINE_TEXTURE_ANIMATION_H

#include "core/descriptor/image.hpp"
#include "core/device.h"
#include "svel/detail/texture.h"
#include "texture.h"
#include <memory>
#include <vector>

namespace texture {
class VulkanAnimation final : public SVEL_NAMESPACE::Animation,
                              public core::descriptor::ImageDescriptor {
private:
  std::vector<std::shared_ptr<SVEL_NAMESPACE::Texture>> _textures;
  bool _isLooping, _isPlaying;
  unsigned long long _speed;
  unsigned long long _microSecondsPassed;
  unsigned long long _length;

public:
  // AnimationSpeed = image changes per second
  VulkanAnimation(core::SharedDevice device, vk::CommandPool &commandPool,
                  core::SharedBarrier barrier,
                  const std::vector<SVEL_NAMESPACE::SharedImage> &images,
                  float animationSpeed, bool looping);
  VulkanAnimation(const VulkanAnimation &) = delete;

  void Play() override;
  void Reset() override;
  void Stop() override;
  bool IsPlaying() const override;

  // Frametime in seconds
  void Update(float frametime) override;
};
} // namespace texture

#endif /* ENGINE_TEXTURE_ANIMATION_H */