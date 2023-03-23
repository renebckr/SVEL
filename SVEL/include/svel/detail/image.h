#ifndef __SVEL_DETAIL_IMAGE_H__
#define __SVEL_DETAIL_IMAGE_H__

#include "svel/util/structs.hpp"
#include <svel/config.h>
#include <svel/detail/shader.h>

#include <memory>
#include <vector>

namespace SVEL_NAMESPACE {

class Image {
private:
  Extent _size;
  int _channels, _dataChannels;
  unsigned char *_data = nullptr;
  size_t _dataSize;
  bool _loadedFromFile = false;

public:
  Image(const Extent &size, int channels, int dataChannels, unsigned char *data,
        size_t dataSize);
  Image(const std::string &path);
  Image(const Image &) = default;
  ~Image();

  unsigned char *GetData() const;
  size_t GetSize() const;
  unsigned int GetDataChannelCount() const;
  const Extent &GetExtent() const;
};
SVEL_CLASS(Image)

class SpriteSheet {
private:
  bool _spritesLoaded = false;
  SharedImage _baseImage;
  std::vector<SharedImage> _spriteCache;
  Extent _dim;

public:
  // Assumes no padding
  SpriteSheet(SharedImage baseImage, const Extent &spriteCount);

  std::vector<SharedImage>
  GetSprites(std::vector<std::pair<uint32_t, uint32_t>> spritePositions);
};
SVEL_CLASS(SpriteSheet)

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_DETAIL_IMAGE_H__ */