#ifndef ENGINE_TEXTURE_INTERFACE_HPP
#define ENGINE_TEXTURE_INTERFACE_HPP

#include <vulkan/vulkan.hpp>

namespace core::descriptor {
class ImageDescriptor {
protected:
  vk::DescriptorImageInfo _imageInfo;

public:
  virtual ~ImageDescriptor() {}
  virtual vk::DescriptorImageInfo &GetImageInfo() { return _imageInfo; }
};
} // namespace core::descriptor

#endif /* ENGINE_TEXTURE_INTERFACE_HPP */