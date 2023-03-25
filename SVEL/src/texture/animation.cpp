/**
 * @file animation.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the VulkanAnimation.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

// Local
#include "animation.h"

// Internal
#include <svel/detail/image.h>

using namespace texture;
using namespace SVEL_NAMESPACE;

VulkanAnimation::VulkanAnimation(core::SharedDevice device,
                                 vk::CommandPool &commandPool,
                                 core::SharedBarrier barrier,
                                 const std::vector<SharedImage> &images,
                                 float animationSpeed, bool looping)
    : _isLooping(looping), _speed((unsigned int)(animationSpeed * 1000000)) {
  // Set internal values
  assert(animationSpeed >= 0.0f);
  _length = images.size() * _speed;
  _isPlaying = false;

  // Create a texture for every frame of the animation
  for (auto image : images) {
    auto texture = std::make_shared<Texture>(device, image);
    texture->Dispatch(commandPool, barrier);
    _textures.push_back(texture);
  }

  // Set the image info to the initial image
  _imageInfo = _textures.at(0)->GetImageInfo();
}

void VulkanAnimation::Play() { _isPlaying = true; }

void VulkanAnimation::Reset() {
  _microSecondsPassed = 0;
  _imageInfo = _textures.at(0)->GetImageInfo();
}

void VulkanAnimation::Stop() { _isPlaying = false; }

bool VulkanAnimation::IsPlaying() const { return _isPlaying; }

void VulkanAnimation::Update(float frametime) {
  assert(frametime >= 0.0f);
  // Don't do anything if we are not playing
  if (!IsPlaying())
    return;

  // Calculate how far we are in the animation and act according to looping
  // flag.
  _microSecondsPassed += (unsigned int)(frametime * 1000000);
  if (_isLooping)
    _microSecondsPassed %= _length;
  else if (_microSecondsPassed > _length) {
    Stop();
    Reset();
  }

  // Set new image info
  _imageInfo = _textures.at(_microSecondsPassed / _speed)->GetImageInfo();
}
