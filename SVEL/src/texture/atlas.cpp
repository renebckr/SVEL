/**
 * @file atlas.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the the vulkan texture atlas.
 * @date 2023-08-29
 *
 * @copyright Copyright (c) 2023
 *
 */

// Local
#include "atlas.h"

// STL
#include <stdexcept>

using namespace texture;

VulkanTextureAtlas::VulkanTextureAtlas(core::SharedDevice device,
                                       vk::CommandPool &commandPool,
                                       core::SharedBarrier barrier,
                                       SVEL_NAMESPACE::SharedImage img,
                                       unsigned int tileCountX,
                                       unsigned int tileCountY)
    : _imageWidth(img->GetExtent().width),
      _imageHeight(img->GetExtent().height),
      _tileWidth(_imageWidth / tileCountX),
      _tileHeight(_imageHeight / tileCountY), _tileCountX(tileCountX),
      _tileCountY(tileCountY) {

  _texture = std::make_shared<SVEL_NAMESPACE::Texture>(device, img);
  _texture->Dispatch(commandPool, barrier);
  _imageInfo = _texture->GetImageInfo();
}

void VulkanTextureAtlas::GetTexCoords(unsigned int tileId, float &offsetX,
                                      float &offsetY, float &extentX,
                                      float &extentY) {
  // TODO: Perhaps compute less?
  unsigned int yPos = tileId / _tileCountX;
  unsigned int xPos = tileId % _tileCountX;

  if (yPos >= _tileCountY)
    throw std::logic_error("Invalid TileID");

  unsigned int x = _tileWidth * xPos;
  unsigned int y = _tileHeight * yPos;

  offsetX = ((float)x / (float)_imageWidth);
  offsetY = ((float)y / (float)_imageHeight);
  extentX = ((float)(_tileWidth) / (float)(_imageWidth));
  extentY = ((float)(_tileHeight) / (float)(_imageHeight));
}

SVEL_NAMESPACE::SharedTexture VulkanTextureAtlas::GetTexture() const {
  return _texture;
}