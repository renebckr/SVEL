/**
 * @file macros.hpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Defines general purpose C macros.
 * @date 2023-08-08
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __UTIL_MACROS_HPP__
#define __UTIL_MACROS_HPP__

/**
 * @brief CONCATENATE Concatenates two strings to form another macro.
 */
#define _CONCATENATE(X, Y) X##Y
#define CONCATENATE(X, Y) _CONCATENATE(X, Y)

#define _SIG_COUNT(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13,     \
                   A14, A15, A16, ...)                                         \
  A16
#define SIG_COUNT(...)                                                         \
  _SIG_COUNT(__VA_ARGS__, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0)

/**
 * @brief SIG_GET Creates a signature from the given parameters
 * SIG_GET(int, x, int, y) -> "int x, int y"
 */
#define SIG_GET1(T1, A) T1 A
#define SIG_GET2(T1, A, T2, B) T1 A, SIG_GET1(T2, B)
#define SIG_GET3(T1, A, T2, B, T3, C) T1 A, SIG_GET2(T2, B, T3, C)
#define SIG_GET4(T1, A, T2, B, T3, C, T4, D) T1 A, SIG_GET3(T2, B, T3, C)
#define SIG_GET5(T1, A, T2, B, T3, C, T4, D, T5, E) T1 A, SIG_GET4(T2, B, T3, C)
#define SIG_GET6(T1, A, T2, B, T3, C, T4, D, T5, E, T6, F)                     \
  T1 A, SIG_GET5(T2, B, T3, C)
#define SIG_GET7(T1, A, T2, B, T3, C, T4, D, T5, E, T6, F, T7, G)              \
  T1 A, SIG_GET6(T2, B, T3, C)
#define SIG_GET8(T1, A, T2, B, T3, C, T4, D, T5, E, T6, F, T7, G, T8, H)       \
  T1 A, SIG_GET7(T2, B, T3, C)
#define SIG_GET(...) CONCATENATE(SIG_GET, SIG_COUNT(__VA_ARGS__))(__VA_ARGS__)

/**
 * @brief SIG_VAL Gets the values from the given parameters
 * SIG_VAL(int, x, int, y) -> "x, y"
 */
#define SIG_VALS1(T1, A) A
#define SIG_VALS2(T1, A, T2, B) A, SIG_VALS1(T2, B)
#define SIG_VALS3(T1, A, T2, B, T3, C) A, SIG_VALS2(T2, B, T3, C)
#define SIG_VALS4(T1, A, T2, B, T3, C, T4, D) A, SIG_VALS3(T2, B, T3, C)
#define SIG_VALS5(T1, A, T2, B, T3, C, T4, D, T5, E) A, SIG_VALS4(T2, B, T3, C)
#define SIG_VALS6(T1, A, T2, B, T3, C, T4, D, T5, E, T6, F)                    \
  A, SIG_VALS5(T2, B, T3, C)
#define SIG_VALS7(T1, A, T2, B, T3, C, T4, D, T5, E, T6, F, T7, G)             \
  A, SIG_VALS6(T2, B, T3, C)
#define SIG_VALS8(T1, A, T2, B, T3, C, T4, D, T5, E, T6, F, T7, G, T8, H)      \
  A, SIG_VALS7(T2, B, T3, C)
#define SIG_VALS(...) CONCATENATE(SIG_VALS, SIG_COUNT(__VA_ARGS__))(__VA_ARGS__)

#endif /* __UTIL_MACROS_HPP__ */