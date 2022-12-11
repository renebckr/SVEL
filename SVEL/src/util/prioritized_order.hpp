#ifndef __UTIL_PRIORITIZED_ORDER_HPP_
#define __UTIL_PRIORITIZED_ORDER_HPP_

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace util {
template <typename T>
std::vector<T *> PriorityOrder(std::vector<T> &_list,
                               const std::vector<T> &_prioritizedList) {
  // Move priorities
  std::unordered_map<T, uint32_t> priorities;
  priorities.reserve(_prioritizedList.size());
  for (uint32_t i = 0; i < _prioritizedList.size(); i++)
    priorities[_prioritizedList.at(i)] = i;

  // Zip list
  std::vector<std::pair<T *, uint32_t>> zippedList;
  zippedList.reserve(_list.size());
  for (unsigned int i = 0; i < _list.size(); i++) {
    auto obj = _list.at(i);
    if (priorities.find(obj) != priorities.end())
      zippedList.push_back({&_list[i], priorities[obj]});
  }

  // Sort zipped list
  std::sort(
      zippedList.begin(), zippedList.end(),
      [](const std::pair<T *, uint32_t> &a, const std::pair<T *, uint32_t> &b) {
        return std::get<1>(a) < std::get<1>(b);
      });

  // Unzip zipped list
  std::vector<T *> result;
  result.reserve(_list.size());
  for (auto pair : zippedList)
    result.push_back(std::get<0>(pair));
  return result;
}

template <typename Obj, typename Index>
std::vector<Index>
PriorityOrder(const std::vector<std::pair<Index, Obj>> &_list,
              const std::vector<Obj> &_prioritizedList) {
  // Move priorities
  std::unordered_map<Obj, uint32_t> priorities;
  priorities.reserve(_prioritizedList.size());
  for (uint32_t i = 0; i < _prioritizedList.size(); i++)
    priorities[_prioritizedList.at(i)] = i;

  // Zip list
  std::vector<std::pair<Index, uint32_t>> zippedList;
  zippedList.reserve(_list.size());
  for (auto pair : _list)
    if (priorities.find(std::get<1>(pair)) != priorities.end())
      zippedList.push_back({std::get<0>(pair), priorities[std::get<1>(pair)]});

  // Sort zipped list
  std::sort(zippedList.begin(), zippedList.end(),
            [](const std::pair<Index, uint32_t> a,
               const std::pair<Index, uint32_t> b) {
              return std::get<1>(a) < std::get<1>(b);
            });

  // Unzip zipped list
  std::vector<Index> result;
  result.reserve(_list.size());
  for (auto pair : zippedList)
    result.push_back(std::get<0>(pair));
  return result;
}
} // namespace util

#endif /* __UTIL_PRIORITIZED_ORDER_HPP_ */