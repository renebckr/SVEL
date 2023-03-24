/**
 * @file array_proxy.hpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Array proxy utility.
 * @date 2023-03-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __SVEL_UTIL_ARRAY_PROXY_HPP__
#define __SVEL_UTIL_ARRAY_PROXY_HPP__

// STL
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <vector>

namespace SVEL_NAMESPACE {

/**
 * @brief Allows generalized containers to be passed to interface methods.
 */
struct ArrayProxy {
  /**
   * @brief Raw data pointer.
   */
  void *data;

  /**
   * @brief Size of the data in bytes.
   */
  size_t dataSize;

  /**
   * @brief Size of an element in bytes.
   */
  size_t elementSize;

  /**
   * @brief How many elements are in the data.
   */
  size_t elementCount;

  /**
   * @brief Construct an Array Proxy with raw data.
   *
   * @param d   Data pointer.
   * @param s   Data size in bytes.
   * @param eS  Element size in bytes.
   * @param eC  Count of elements in the data.
   */
  ArrayProxy(void *d, size_t s, size_t eS, size_t eC)
      : data(d), dataSize(s), elementSize(eS), elementCount(eC) {}

  /**
   * @brief Construct an Array Proxy with the given initializer list.
   *
   * @tparam T    Type of the element.
   * @param list  Initializer list to use for the internal values.
   */
  template <typename T>
  ArrayProxy(std::initializer_list<T> &list)
      : data((void *)list.begin()), dataSize(list.size() * sizeof(T)),
        elementSize(sizeof(T)), elementCount(list.size()) {}

  /**
   * @brief Construct an Array Proxy with the given vector.
   *
   * @tparam T      Type of the element.
   * @param vector  Vector to use for the internal values.
   */
  template <typename T>
  ArrayProxy(const std::vector<T> &vector)
      : data((void *)vector.data()), dataSize(vector.size() * sizeof(T)),
        elementSize(sizeof(T)), elementCount(vector.size()) {}

  /**
   * @brief Construct an Array Proxy with the given data and element count.
   *
   * @tparam T        Type of the element.
   * @param typedData Data pointer.
   * @param eC        How many elements are in the array.
   */
  template <typename T>
  ArrayProxy(T *typedData, size_t eC)
      : data((void *)typedData), dataSize(eC * sizeof(T)),
        elementSize(sizeof(T)), elementCount(eC) {}
};

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_UTIL_ARRAY_PROXY_HPP__ */