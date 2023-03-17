#include "animation.h"
#include "svel/detail/image.h"

using namespace texture;
using namespace SVEL_NAMESPACE;

VulkanAnimation::VulkanAnimation(core::SharedDevice device,
                                 vk::CommandPool &commandPool,
                                 core::SharedBarrier barrier,
                                 const std::vector<SharedImage> &images,
                                 float animationSpeed, bool looping)
    : _isLooping(looping), _speed((unsigned int)(animationSpeed * 1000000)) {
  assert(animationSpeed >= 0.0f);
  _length = images.size() * _speed;
  _isPlaying = false;

  for (auto image : images) {
    auto texture = std::make_shared<Texture>(device, image);
    texture->Dispatch(commandPool, barrier);
    _textures.push_back(texture);
  }
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
  if (!IsPlaying())
    return;

  _microSecondsPassed += (unsigned int)(frametime * 1000000);
  if (_isLooping)
    _microSecondsPassed %= _length;
  else if (_microSecondsPassed > _length) {
    Stop();
    Reset();
  }

  _imageInfo = _textures.at(_microSecondsPassed / _speed)->GetImageInfo();
}
