/**
 * @file atlas.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declares a concrete texture atlas class.
 * @date 2023-08-29
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __TEXTURE_ATLAS_H__
#define __TEXTURE_ATLAS_H__

// Local
#include "texture.h"

// Internal
#include <svel/detail/texture.h>

// GLM
#include <glm/glm.hpp>

namespace texture {

/**
 * @brief Concrete texture atlas. Acts as a provider for texture coordinates.
 */
class VulkanTextureAtlas final : public SVEL_NAMESPACE::TextureAtlas,
                                 public core::descriptor::ImageDescriptor {
private:
  /**
   * @brief The texture of the atlas.
   */
  SVEL_NAMESPACE::SharedTexture _texture;

  /**
   * @brief Width of the image.
   */
  unsigned int _imageWidth;

  /**
   * @brief Height of the image.
   */
  unsigned int _imageHeight;

  /**
   * @brief Width of a tile.
   */
  unsigned int _tileWidth;

  /**
   * @brief Height of a tile.
   */
  unsigned int _tileHeight;

  /**
   * @brief How many tiles in the x dimension.
   */
  unsigned int _tileCountX;

  /**
   * @brief How many tiles in the y dimension.
   */
  unsigned int _tileCountY;

public:
  /**
   * @brief Construct a Vulkan Texture Atlas.
   *
   * @param device      Device to use.
   * @param commandPool Command pool to use for texture dispatching.
   * @param barrier     Barrier for the texture dispatch.
   * @param img         Image to use.
   * @param tileCountX  How many tiles in x dimension.
   * @param tileCountY  How many tiles in y dimension.
   */
  VulkanTextureAtlas(core::SharedDevice device, vk::CommandPool &commandPool,
                     core::SharedBarrier barrier,
                     SVEL_NAMESPACE::SharedImage img, unsigned int tileCountX,
                     unsigned int tileCountY);

  /**
   * @brief Implementation of the GetTexCoords Interface.
   *
   * @param tileId  Id of the tile for which to get the coordinates. (y * width
   * + x)
   * @param offsetX Tile offset x dimension.
   * @param offsetY Tile offset y dimension.
   * @param extentX Width of the tile.
   * @param extentY Height of the tile.
   */
  void GetTexCoords(unsigned int tileId, float &offsetX, float &offsetY,
                    float &extentX, float &extentY) override;

  /**
   * @brief Implementation of the GetTexture Interface.
   *
   * @return SVEL_NAMESPACE::SharedTexture The atlas texture.
   */
  SVEL_NAMESPACE::SharedTexture GetTexture() const override;
};

} // namespace texture

#endif /* __TEXTURE_ATLAS_H__ */