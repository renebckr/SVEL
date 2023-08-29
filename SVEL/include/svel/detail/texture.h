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

  /**
   * @brief Retrieve the current texture of the animation.
   *
   * @return SharedTexture The current texture.
   */
  virtual SharedTexture GetTexture() const = 0;
};
SVEL_CLASS(Animation)

/**
 * @brief Declaration of the TextureAtlas interface.
 */
class TextureAtlas {
public:
  /**
   * @brief Destroy the Texture Atlas.
   *
   */
  virtual ~TextureAtlas(){};

  /**
   * @brief Getter for Tex Coords
   *
   * @param tileId  Id of the tile for which to get the coordinates. (y * width
   * + x)
   * @param offsetX Tile offset x dimension.
   * @param offsetY Tile offset y dimension.
   * @param extentX Width of the tile.
   * @param extentY Height of the tile.
   */
  virtual void GetTexCoords(unsigned int tileId, float &offsetX, float &offsetY,
                            float &extentX, float &extentY) = 0;

  /**
   * @brief Getter for the complete texture.
   *
   * @return SharedTexture The atlas texture.
   */
  virtual SharedTexture GetTexture() const = 0;
};
SVEL_CLASS(TextureAtlas)

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_DETAL_TEXTURE_H__ */