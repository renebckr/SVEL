#ifndef ENGINE_TEXTURE_INTERFACE_HPP
#define ENGINE_TEXTURE_INTERFACE_HPP

// TO BE MOVED

#include <vulkan/vulkan.hpp>

namespace engine {
class TextureInterface {
protected:
  vk::DescriptorImageInfo imageInfo;

public:
  virtual ~TextureInterface() {}

  virtual vk::DescriptorImageInfo &GetImageInfo() { return imageInfo; }
};
} // namespace engine

#endif /* ENGINE_TEXTURE_INTERFACE_HPP */