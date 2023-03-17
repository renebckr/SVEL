#ifndef __TEXTURE_TEXTURE_H__
#define __TEXTURE_TEXTURE_H__

#include "core/barrier.h"
#include "core/descriptor/textureInterface.hpp"
#include "core/device.h"
#include "core/memory/buffer.h"
#include "core/memory/device_memory.h"
#include "svel/detail/image.h"
#include <atomic>
#include <memory>
#include <vulkan/vulkan.hpp>

namespace SVEL_NAMESPACE {
class Texture : public core::descriptor::ImageDescriptor,
                public std::enable_shared_from_this<Texture> {
private:
  core::SharedDevice _device;
  vk::Sampler _sampler;
  vk::Image _image;
  core::UniqueDeviceMemory _imageMemory;
  vk::ImageView _imageView;
  vk::CommandBuffer _commandBuffer;
  std::atomic<bool> _imageReady = false;
  Extent _dim;
  core::SharedBuffer _stagingBuffer;

  core::SharedBuffer createStagingBuffer(SharedImage _img);
  void createImage(SharedImage _img);
  void createSampler();
  void updateImageInfo();

  void onUpload(vk::CommandPool &_commandPool) {
    _device->AsVulkanObj().freeCommandBuffers(_commandPool, _commandBuffer);
    _imageReady = true;
    _stagingBuffer.reset();
  }

public:
  Texture(core::SharedDevice device, SharedImage img);
  ~Texture();

  void Dispatch(vk::CommandPool &_commandPool, core::SharedBarrier _barrier);
};
} // namespace SVEL_NAMESPACE

#endif /* __TEXTURE_TEXTURE_H__ */