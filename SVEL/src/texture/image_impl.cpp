#include <cstdint>
#include <stdexcept>
#include <svel/config.h>
#include <svel/detail/image.h>
#include <sys/types.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace SVEL_NAMESPACE;

Image::Image(const Extent &size, int channels, int dataChannels,
             unsigned char *data, size_t dataSize)
    : _size(size), _channels(channels), _dataChannels(dataChannels),
      _data(data), _dataSize(dataSize) {}

Image::Image(const std::string &_path) {
  int width, height;
  _data = stbi_load(_path.c_str(), &width, &height, &_channels, STBI_rgb_alpha);

  if (_data == nullptr)
    throw std::runtime_error(
        "Image could not be loaded. Perhaps invalid path?");

  if (width < 0 || height < 0 || _channels < 0)
    throw std::logic_error("An Image cannot have negative extent.");

  _size.width = (uint32_t)width;
  _size.height = (uint32_t)height;
  _dataChannels = 4;
  _dataSize = _size.width * _size.height * (unsigned int)_dataChannels;
  _loadedFromFile = true;
}

Image::~Image() {
  if (_loadedFromFile)
    stbi_image_free(_data);
  else
    delete[] _data;
}

unsigned char *Image::GetData() const { return _data; }

size_t Image::GetSize() const { return _dataSize; }

unsigned int Image::GetDataChannelCount() const {
  return (unsigned int)_dataChannels;
}

const Extent &Image::GetExtent() const { return _size; }