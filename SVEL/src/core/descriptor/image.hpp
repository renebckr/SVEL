#ifndef __CORE_DESCRIPTOR_IMAGE_H__
#define __CORE_DESCRIPTOR_IMAGE_H__

// Vulkan
#include <vulkan/vulkan.hpp>

namespace core::descriptor {

/**
 * @brief Describes interface for descriptor sets. Descriptor sets may require
 * an image info to bind any image. Images passed to the descriptor set should
 * implement this interface.
 */
class ImageDescriptor {
protected:
  /**
   * @brief Image info that should be filled by the derived class.
   */
  vk::DescriptorImageInfo _imageInfo;

public:
  /**
   * @brief Destroy the Image Descriptor.
   */
  virtual ~ImageDescriptor() {}

  /**
   * @brief Getter for the image info.
   *
   * @return vk::DescriptorImageInfo& The image info.
   */
  virtual vk::DescriptorImageInfo &GetImageInfo() { return _imageInfo; }
};

} // namespace core::descriptor

#endif /* __CORE_DESCRIPTOR_IMAGE_H__ */