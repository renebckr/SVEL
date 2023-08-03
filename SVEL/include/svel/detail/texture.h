/**
 * @file texture.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declares texture interfaces.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __SVEL_DETAL_TEXTURE_H__
#define __SVEL_DETAL_TEXTURE_H__

// SVEL
#include <svel/config.h>

// STL
#include <memory>

namespace SVEL_NAMESPACE {

/**
 * @brief Forward declaration of the texture.
 */
class Texture;
SVEL_CLASS(Texture)

/**
 * @brief Declaration of the Animation interface.
 */
class Animation {
public:
  /**
   * @brief Destroy the Animation.
   */
  virtual ~Animation(){};

  /**
   * @brief Start playing the animation.
   */
  virtual void Play() = 0;

  /**
   * @brief Reset the animation.
   */
  virtual void Reset() = 0;

  /**
   * @brief Stop playing the animation.
   */
  virtual void Stop() = 0;

  /**
   * @brief Gets the status of the animation.
   *
   * @return true   Animation is playing.
   * @return false  Animation is not playing.
   */
  virtual bool IsPlaying() const = 0;

  /**
   * @brief Update the animation.
   *
   * @param _frametime Time in seconds since the last update.
   */
  virtual void Update(float _frametime) = 0;
};
SVEL_CLASS(Animation)

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_DETAL_TEXTURE_H__ */