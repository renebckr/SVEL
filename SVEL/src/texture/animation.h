/**
 * @file animation.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declaration of the VulkanAnimation.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __TEXTURE_ANIMATION_H__
#define __TEXTURE_ANIMATION_H__

// Local
#include "texture.h"

// Internal
#include <core/descriptor/image.hpp>
#include <core/device.h>
#include <svel/detail/texture.h>

// STL
#include <memory>
#include <vector>

namespace texture {

/**
 * @brief Concrete Implementation of the Animation interface for Vulkan.
 */
class VulkanAnimation final : public SVEL_NAMESPACE::Animation,
                              public core::descriptor::ImageDescriptor {
private:
  /**
   * @brief Array of all textures in the animation.
   */
  std::vector<std::shared_ptr<SVEL_NAMESPACE::Texture>> _textures;

  /**
   * @brief Is the animation looping?
   */
  bool _isLooping;

  /**
   * @brief Is the animation playing right now?
   */
  bool _isPlaying;

  /**
   * @brief The playback speed of the animation. (Frames per second)
   */
  unsigned long long _speed;

  /**
   * @brief How many micro seconds have already passed so far.
   */
  unsigned long long _microSecondsPassed;

  /**
   * @brief Complete length of the animation in seconds.
   */
  unsigned long long _length;

public:
  /**
   * @brief Construct a Vulkan Animation.
   *
   * @param device          Device to use.
   * @param commandPool     Command pool to use for transfering data.
   * @param barrier         Barrier to use.
   * @param images          Images that the animation should contain.
   * @param animationSpeed  Playback speed in frames per second.
   * @param looping         Is the animation looping?
   */
  VulkanAnimation(core::SharedDevice device, vk::CommandPool &commandPool,
                  core::SharedBarrier barrier,
                  const std::vector<SVEL_NAMESPACE::SharedImage> &images,
                  float animationSpeed, bool looping);

  /**
   * @brief Implementation of the Play Interface.
   */
  void Play() override;

  /**
   * @brief Implementation of the Reset Interface.
   */
  void Reset() override;

  /**
   * @brief Implementation of the Stop Interface.
   */
  void Stop() override;

  /**
   * @brief Implementation of the IsPlaying Interface.
   */
  bool IsPlaying() const override;

  /**
   * @brief Implementation of the Update Interface.
   */
  void Update(float frametime) override;

  /**
   * @brief Implementation of the GetTexture Interface.
   *
   * @return SVEL_NAMESPACE::SharedTexture The current texture.
   */
  SVEL_NAMESPACE::SharedTexture GetTexture() const override;
};
} // namespace texture

#endif /* __TEXTURE_ANIMATION_H__ */