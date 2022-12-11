#ifndef __SVEL_UTIL_STRUCTS_HPP__
#define __SVEL_UTIL_STRUCTS_HPP__

#include <cstdint>
#include <svel/config.h>

namespace SVEL_NAMESPACE {

struct Extent {
  uint32_t width, height;
  Extent() = default;
  Extent(uint32_t w, uint32_t h) : width(w), height(h) {}
};

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_UTIL_STRUCTS_HPP__ */