/**
 * @file config.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Contains all configuration options of the engine and basic defines.
 * @date 2023-03-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __SVEL_CONFIG_H__
#define __SVEL_CONFIG_H__

// Namespace that SVEL should use
#ifndef SVEL_NAMESPACE
#define SVEL_NAMESPACE sv
#endif

// Convenience syntax for smart pointers
#define SVEL_CLASS(T)                                                          \
  using Unique##T = std::unique_ptr<T>;                                        \
  using Shared##T = std::shared_ptr<T>;

// Simple pImpl idiom declaration for classes
#define SVEL_PIMPL                                                             \
private:                                                                       \
  class Impl;                                                                  \
  SVEL_CLASS(Impl)                                                             \
  SharedImpl __pImpl;                                                          \
                                                                               \
public:                                                                        \
  SharedImpl __getImpl() { return __pImpl; }

#define SVEL_FLAGS(eType)                                                      \
  inline eType operator|(const eType &a, const eType &b) {                     \
    return static_cast<eType>(static_cast<int>(a) | static_cast<int>(b));      \
  }

#endif /* __SVEL_CONFIG_H__ */