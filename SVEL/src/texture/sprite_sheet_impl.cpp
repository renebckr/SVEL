#include <svel/config.h>
#include <svel/detail/image.h>

#include <cstring>
#include <iostream>
#include <memory>

using namespace SVEL_NAMESPACE;

SpriteSheet::SpriteSheet(SharedImage baseImage, const Extent &spriteCount)
    : _baseImage(baseImage), _dim(spriteCount) {

  // Check if operation possible
  Extent size = _baseImage->GetExtent();
  if ((size.width % _dim.width != 0) || (size.height % _dim.height != 0))
    return;

  // Create Data Buffers
  unsigned int totalSpriteCount = _dim.height * _dim.width;
  unsigned int channels = baseImage->GetDataChannelCount();
  unsigned char *imgData = baseImage->GetData();
  unsigned char **data = new unsigned char *[totalSpriteCount];

  // Allocate Buffers
  unsigned int spriteSizeX = size.width / _dim.width;
  unsigned int spriteSizeY = size.height / _dim.height;
  unsigned int spriteByteSize = spriteSizeX * spriteSizeY * channels;
  for (unsigned int i = 0; i < totalSpriteCount; i++)
    data[i] = new unsigned char[spriteByteSize];

  // TODO: Optimize inner loop
  for (unsigned int j = 0; j < size.height; j++) {
    for (unsigned int i = 0; i < size.width; i++) {
      // Calculate new Buffer index
      unsigned int index = (j / spriteSizeY) * _dim.width + (i / spriteSizeX);
      unsigned int x = i % spriteSizeX;
      unsigned int y = j % spriteSizeY;

      std::memcpy(&(data[index][channels * (y * spriteSizeX + x)]),
                  &(imgData[channels * (j * size.width + i)]), channels);
    }
  }

  for (unsigned int i = 0; i < totalSpriteCount; i++)
    _spriteCache.push_back(std::make_shared<Image>(
        Extent{spriteSizeX, spriteSizeY}, baseImage->GetDataChannelCount(),
        baseImage->GetDataChannelCount(), data[i], spriteByteSize));
  delete[] data;
  _spritesLoaded = true;
}

std::vector<SharedImage> SpriteSheet::GetSprites(
    std::vector<std::pair<uint32_t, uint32_t>> _spritePositions) {
  if (!_spritesLoaded)
    return {};

  std::vector<SharedImage> sprites;
  for (auto &pos : _spritePositions) {
    if (pos.first >= _dim.width || pos.second >= _dim.height)
      continue;
    sprites.push_back(_spriteCache.at(pos.second * _dim.width + pos.first));
  }
  return sprites;
}
