/**
 * @file prioritized_order.hpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declares priority order algorithm.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __UTIL_PRIORITIZED_ORDER_HPP_
#define __UTIL_PRIORITIZED_ORDER_HPP_

// STL
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace util {

/**
 * @brief Orders the list by the priorities given.
 *
 * @tparam T                Hashable type.
 * @param list              List to order.
 * @param prioritizedList   Priorities. First element has highest priority.
 * @return std::vector<T *> Ordered list.
 */
template <typename T>
std::vector<T *> PriorityOrder(std::vector<T> &list,
                               const std::vector<T> &prioritizedList) {
  // Move priorities
  std::unordered_map<T, uint32_t> priorities;
  priorities.reserve(prioritizedList.size());
  for (uint32_t i = 0; i < prioritizedList.size(); i++)
    priorities[prioritizedList.at(i)] = i;

  // Zip list
  std::vector<std::pair<T *, uint32_t>> zippedList;
  zippedList.reserve(list.size());
  for (unsigned int i = 0; i < list.size(); i++) {
    auto obj = list.at(i);
    if (priorities.find(obj) != priorities.end())
      zippedList.push_back({&list[i], priorities[obj]});
  }

  // Sort zipped list
  std::sort(
      zippedList.begin(), zippedList.end(),
      [](const std::pair<T *, uint32_t> &a, const std::pair<T *, uint32_t> &b) {
        return std::get<1>(a) < std::get<1>(b);
      });

  // Unzip zipped list
  std::vector<T *> result;
  result.reserve(list.size());
  for (auto pair : zippedList)
    result.push_back(std::get<0>(pair));
  return result;
}

/**
 * @brief Orders the list by the priorities given.
 *
 * @tparam Obj                Hashable type.
 * @tparam Index              The index to reference in the result.
 * @param list                List to be ordered.
 * @param prioritizedList     Priorities. First element has highest priority.
 * @return std::vector<Index> Ordered list of the associated indices.
 */
template <typename Obj, typename Index>
std::vector<Index> PriorityOrder(const std::vector<std::pair<Index, Obj>> &list,
                                 const std::vector<Obj> &prioritizedList) {
  // Move priorities
  std::unordered_map<Obj, uint32_t> priorities;
  priorities.reserve(prioritizedList.size());
  for (uint32_t i = 0; i < prioritizedList.size(); i++)
    priorities[prioritizedList.at(i)] = i;

  // Zip list
  std::vector<std::pair<Index, uint32_t>> zippedList;
  zippedList.reserve(list.size());
  for (auto pair : list)
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
  result.reserve(list.size());
  for (auto pair : zippedList)
    result.push_back(std::get<0>(pair));
  return result;
}

} // namespace util

#endif /* __UTIL_PRIORITIZED_ORDER_HPP_ */