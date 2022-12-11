/**
 * @file window.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Window GLFW wrapper declaration
 * @date 2022-10-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ENGINE_CORE_WINDOW_H
#define ENGINE_CORE_WINDOW_H

#include <svel/util/structs.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

namespace core {

/**
 * @brief Wrapper for the GLFWwindow.
 */
class Window {
private:
  /**
   * @brief The GLFWwindow.
   */
  GLFWwindow *_win;

  /**
   * @brief Title of the window.
   */
  std::string _title;

  /**
   * @brief Extent of the window.
   */
  sv::Extent _size;

public:
  Window(const std::string &title, const sv::Extent &size);

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  /**
   * @brief Destroy the GLFW window.
   */
  ~Window();

  GLFWwindow *Get() { return _win; }
};
SVEL_CLASS(Window)

} // namespace core

#endif /* ENGINE_CORE_WINDOW_H */