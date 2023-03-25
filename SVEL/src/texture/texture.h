/**
 * @file texture.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the fwd declared Texture.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __TEXTURE_TEXTURE_H__
#define __TEXTURE_TEXTURE_H__

// Internal
#include <core/barrier.h>
#include <core/descriptor/image.hpp>
#include <core/device.h>
#include <core/memory/buffer.h>
#include <core/memory/device_memory.h>
#include <svel/detail/image.h>

// Vulkan
#include <vulkan/vulkan.hpp>

// STL
#include <atomic>
#include <memory>

namespace SVEL_NAMESPACE {

/**
 * @brief Makes images available to be used in drawing.
 */
class Texture : public core::descriptor::ImageDescriptor,
                public std::enable_shared_from_this<Texture> {
private:
  /**
   * @brief Device to use.
   */
  core::SharedDevice _device;

  /**
   * @brief Sampler of the image.
   */
  vk::Sampler _sampler;

  /**
   * @brief The Vulkan image. TODO: use core::memory::Image
   */
  vk::Image _image;

  /**
   * @brief Memory of the Vulkan image.
   */
  core::UniqueDeviceMemory _imageMemory;

  /**
   * @brief The image view of the texture.
   */
  vk::ImageView _imageView;

  /**
   * @brief Command buffer? TODO: why is this here?
   *
   */
  vk::CommandBuffer _commandBuffer;

  /**
   * @brief Is the image ready?
   */
  std::atomic<bool> _imageReady = false;

  /**
   * @brief Dimension of the image.
   */
  Extent _dim;

  /**
   * @brief The staging buffer for the image.
   */
  core::SharedBuffer _stagingBuffer;

  /**
   * @brief Create a Staging Buffer.
   *
   * @param _img                Image to create the staging buffer for.
   * @return core::SharedBuffer The created buffer.
   */
  core::SharedBuffer _createStagingBuffer(SharedImage _img);

  /**
   * @brief Create the image from the user defined image.
   *
   * @param _img The user image.
   */
  void _createImage(SharedImage _img);

  /**
   * @brief Creates the sampler.
   */
  void _createSampler();

  /**
   * @brief Updates the image info of this texture.
   */
  void _updateImageInfo();

  /**
   * @brief Handles the completed upload to the Graphics Unit.
   *
   * @param _commandPool The used command pool.
   */
  void _onUpload(vk::CommandPool &_commandPool) {
    _device->AsVulkanObj().freeCommandBuffers(_commandPool, _commandBuffer);
    _imageReady = true;
    _stagingBuffer.reset();
  }

public:
  /**
   * @brief Construct a Texture.
   *
   * @param device  Device to use.
   * @param img     User defined image.
   */
  Texture(core::SharedDevice device, SharedImage img);

  /**
   * @brief Destroy the Texture.
   */
  ~Texture();

  /**
   * @brief Dispatches the Texture to the Graphics Unit. Must be called once
   * after initialization and before any usage occurs.
   *
   * @param _commandPool  Command pool to use.
   * @param _barrier      Barrier to use.
   */
  void Dispatch(vk::CommandPool &_commandPool, core::SharedBarrier _barrier);
};

} // namespace SVEL_NAMESPACE

#endif /* __TEXTURE_TEXTURE_H__ */