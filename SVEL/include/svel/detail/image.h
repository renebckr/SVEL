/**
 * @file image.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declares image classes.
 * @date 2023-03-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __SVEL_DETAIL_IMAGE_H__
#define __SVEL_DETAIL_IMAGE_H__

// SVEL
#include <svel/config.h>
#include <svel/detail/shader.h>
#include <svel/util/structs.hpp>

// STL
#include <memory>
#include <vector>

namespace SVEL_NAMESPACE {

/**
 * @brief Enables the user to create/load image data.
 */
class Image {
private:
  /**
   * @brief Size of the image.
   */
  Extent _size;

  /**
   * @brief How many channels the image has.
   */
  int _channels;

  /**
   * @brief How many data channels the image has.
   */
  int _dataChannels;

  /**
   * @brief Pointer to the image data.
   */
  unsigned char *_data = nullptr;

  /**
   * @brief Size of the image data in bytes.
   */
  size_t _dataSize;

  /**
   * @brief Flag that specifies if the data was loaded from a file.
   */
  bool _loadedFromFile = false;

public:
  /**
   * @brief Construct an image manually.
   *
   * @param size          Width and height of the image.
   * @param channels      Channels of the image.
   * @param dataChannels  Data channels of the image.
   * @param data          Raw pointer to the image data. Currently the Image
   *                      class takes ownership.
   * @param dataSize      Size of the image in bytes.
   */
  Image(const Extent &size, int channels, int dataChannels, unsigned char *data,
        size_t dataSize);

  /**
   * @brief Load an image from a file. Currently only supports .png files.
   *
   * @param path Path to the image file.
   */
  Image(const std::string &path);

  /**
   * @brief Destroys the image.
   */
  ~Image();

  /**
   * @brief Getter for the data.
   *
   * @return unsigned char* Data pointer.
   */
  unsigned char *GetData() const;

  /**
   * @brief Getter for the image size in bytes.
   *
   * @return size_t Image data size.
   */
  size_t GetSize() const;

  /**
   * @brief Getter for the Data Channel Count
   *
   * @return unsigned int How many data channels exist.
   */
  unsigned int GetDataChannelCount() const;

  /**
   * @brief Getter for Extent of the image.
   *
   * @return const Extent& Width,Height of the image.
   */
  const Extent &GetExtent() const;
};
SVEL_CLASS(Image)

/**
 * @brief Allows a simple way of splitting an image into tiles.
 */
class SpriteSheet {
private:
  /**
   * @brief Flag that signals if the sprites were loaded.
   */
  bool _spritesLoaded = false;

  /**
   * @brief The image to be split into parts.
   */
  SharedImage _baseImage;

  /**
   * @brief All of the sprites.
   */
  std::vector<SharedImage> _spriteCache;

  /**
   * @brief How many sprites in which direction exist.
   */
  Extent _dim;

public:
  /**
   * @brief Construct a Sprite Sheet with the given image. This assumes that the
   * sprites are not padded.
   *
   * @param baseImage   The image to be used as a sprite sheet.
   * @param spriteCount How many sprites in which direction.
   */
  SpriteSheet(SharedImage baseImage, const Extent &spriteCount);

  /**
   * @brief Getter for sprites at the specified positions.
   *
   * @param spritePositions           All sprites to get.
   * @return std::vector<SharedImage> The sprites that could be fetched.
   */
  std::vector<SharedImage>
  GetSprites(std::vector<std::pair<uint32_t, uint32_t>> spritePositions);
};
SVEL_CLASS(SpriteSheet)

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_DETAIL_IMAGE_H__ */