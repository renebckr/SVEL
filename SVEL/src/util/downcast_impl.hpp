/**
 * @file downcast_impl.hpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Defines downcast macro.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __UTIL_DOWNCAST_IMPL_HPP__
#define __UTIL_DOWNCAST_IMPL_HPP__

// Allows method declaration to cast from base to the derived class.
#define SVEL_DOWNCAST_IMPL(DERIVED, BASE)                                      \
  inline std::shared_ptr<DERIVED> GetImpl(std::shared_ptr<BASE> ptr) {         \
    return std::static_pointer_cast<DERIVED>(ptr);                             \
  }

#endif /* __UTIL_DOWNCAST_IMPL_HPP__ */