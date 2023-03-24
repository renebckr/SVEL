/**
 * @file structs.hpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Contains utility structs for the library.
 * @date 2023-03-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __SVEL_UTIL_STRUCTS_HPP__
#define __SVEL_UTIL_STRUCTS_HPP__

// SVEL
#include <svel/config.h>

// STL
#include <cstdint>

namespace SVEL_NAMESPACE {

/**
 * @brief Compound to specify width and height.
 */
struct Extent {
  /**
   * @brief Width of the extent.
   */
  uint32_t width = 0;

  /**
   * @brief Height of the extent.
   */
  uint32_t height = 0;

  /**
   * @brief Allow default construction.
   */
  Extent() = default;

  /**
   * @brief Construct an extent.
   *
   * @param w Width
   * @param h Height
   */
  Extent(uint32_t w, uint32_t h) : width(w), height(h) {}
};

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_UTIL_STRUCTS_HPP__ */