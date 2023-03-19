#ifndef __UTIL_ARRAY_PROXY_HPP__
#define __UTIL_ARRAY_PROXY_HPP__

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <vector>

namespace SVEL_NAMESPACE {

struct ArrayProxy {
  void *data;
  size_t dataSize;
  size_t elementSize;
  size_t elementCount;

  ArrayProxy(void *d, size_t s, size_t eS, size_t eC)
      : data(d), dataSize(s), elementSize(eS), elementCount(eC) {}

  template <typename T>
  ArrayProxy(std::initializer_list<T> &list)
      : data((void *)list.begin()), dataSize(list.size() * sizeof(T)),
        elementSize(sizeof(T)), elementCount(list.size()) {}

  template <typename T>
  ArrayProxy(const std::vector<T> &vector)
      : data((void *)vector.data()), dataSize(vector.size() * sizeof(T)),
        elementSize(sizeof(T)), elementCount(vector.size()) {}

  template <typename T>
  ArrayProxy(T *typedData, size_t eC)
      : data((void *)typedData), dataSize(eC * sizeof(T)),
        elementSize(sizeof(T)), elementCount(eC) {}
};

} // namespace SVEL_NAMESPACE

#endif /* __UTIL_ARRAY_PROXY_HPP__ */