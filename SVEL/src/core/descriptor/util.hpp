/**
 * @file util.hpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Defines utility methods.
 * @date 2023-03-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __CORE_DESCRIPTOR_UTIL_HPP__
#define __CORE_DESCRIPTOR_UTIL_HPP__

#include <cstdint>
#include <utility>

namespace core::descriptor {

/**
 * @brief Combines set id and binding id into a single key.
 *
 * @param setId               Set identifier
 * @param bindingId           Binding identifier
 * @return constexpr uint64_t Key
 */
inline constexpr uint64_t CombineSetBinding(uint32_t setId,
                                            uint32_t bindingId) {
  return ((uint64_t)setId << 32) | (uint64_t)bindingId;
}

/**
 * @brief Extract set id and binding id from a key.
 *
 * @param key                                       Key
 * @return constexpr std::pair<uint32_t, uint32_t>  SetId, BindingId pair
 */
inline constexpr std::pair<uint32_t, uint32_t> ExtractSetBinding(uint64_t key) {
  return {(uint32_t)(key >> 32), (uint32_t)(key & 0xFFFFFFFF)};
}

} // namespace core::descriptor

#endif /* __CORE_DESCRIPTOR_UTIL_HPP__ */