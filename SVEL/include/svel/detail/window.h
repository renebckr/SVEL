/**
 * @file window.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Window interface declaration.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __SVEL_DETAIL_WINDOW_H__
#define __SVEL_DETAIL_WINDOW_H__

// SVEL
#include <svel/config.h>
#include <svel/detail/app.h>
#include <svel/detail/renderer.h>
#include <svel/util/structs.hpp>

// STL
#include <memory>

namespace SVEL_NAMESPACE {

/**
 * @brief Interface that the window should be derived from. Users will have to
 * implement the Draw method.
 */
class IWindow {
  SVEL_PIMPL

public:
  /**
   * @brief Construct a new window.
   *
   * @param parent      The application. Use shared_from_this() to pass your
   *                    application.
   * @param title       Title of the window.
   * @param windowSize  Size of the window (width and height).
   */
  IWindow(SharedIApplication parent, const std::string &title,
          const Extent &windowSize);

  /**
   * @brief Getter for the renderer of the window.
   *
   * @return SharedRenderer The renderer of this window.
   */
  SharedRenderer GetRenderer() const;

  /**
   * @brief Getter for the current size of the window.
   *
   * @return Extent Size of the window.
   */
  Extent GetWindowSize() const;

  /**
   * @brief Should be called from the application to start the render loop of
   * the window.
   */
  void StartRenderLoop();

  /**
   * @brief Draw method to be implemented by the user.
   */
  virtual void Draw() = 0;
};

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_DETAIL_WINDOW_H__ */