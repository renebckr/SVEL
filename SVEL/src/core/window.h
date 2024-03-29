/**
 * @file window.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Window GLFW wrapper declaration
 * @date 2022-10-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __CORE__WINDOW_H__
#define __CORE__WINDOW_H__

// Internal
#include <svel/util/structs.hpp>

// GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// STL
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
  /**
   * @brief Construct a Vulkan Window using GLFW.
   *
   * @param title Title of the window.
   * @param size  Size of the window.
   */
  Window(const std::string &title, const sv::Extent &size);

  /**
   * @brief Cannot be copied.
   */
  Window(const Window &) = delete;

  /**
   * @brief Cannot be copied.
   *
   * @return VulkanWindow& ~unused~
   */
  Window &operator=(const Window &) = delete;

  /**
   * @brief Destroy the GLFW window.
   */
  ~Window();

  /**
   * @brief Getter for the GLFW window.
   *
   * @return GLFWwindow* The GLFW window.
   */
  GLFWwindow *Get() { return _win; }
};
SVEL_CLASS(Window)

} // namespace core

#endif /* __CORE__WINDOW_H__ */