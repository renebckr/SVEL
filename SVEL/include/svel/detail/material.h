/**
 * @file material.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declares interface for user defined materials.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __SVEL_DETAIL_MATERIAL_H__
#define __SVEL_DETAIL_MATERIAL_H__

// SVEL
#include <svel/config.h>
#include <svel/detail/pipeline.h>
#include <svel/detail/texture.h>

// STL
#include <memory>
#include <type_traits>

namespace SVEL_NAMESPACE {

/**
 * @brief Interface a Material to derive your material from.
 */
class IMaterial {
  SVEL_PIMPL

protected:
  /**
   * @brief Adds an attribute to the material.
   *
   * @param set       The id of the set.
   * @param binding   The id of the binding.
   * @param data      Pointer to the data. Must persist until the end of
   *                  lifetime of the material.
   * @param dataSize  The size of the data in bytes.
   * @return true     The attribute could be added.
   * @return false    The attribute could not be added.
   */
  bool _addAttribute(unsigned int set, unsigned int binding, void *data,
                     unsigned int dataSize);

protected:
  /**
   * @brief Construct a Material with the corresponding pipeline.
   *
   * @param pipeline The pipeline which the material is for.
   */
  IMaterial(SharedPipeline pipeline);

  /**
   * @brief Convenience method to pass an attribute to the material.
   *
   * @tparam T        Type of the attribute.
   * @tparam typename T must be a pointer.
   * @param set       The id of the set.
   * @param binding   The id of the binding.
   * @param data      Pointer to the data.
   * @return true     The attribute could be added.
   * @return false    The attribute could not be added.
   */
  template <typename T, typename = std::enable_if_t<std::is_pointer<T>::value>>
  inline bool AddAttribute(unsigned int set, unsigned int binding, T data) {
    return _addAttribute(set, binding, data,
                         sizeof(typename std::remove_pointer<T>::type));
  }

  /**
   * @brief Binds a texture to the provided binding.
   *
   * @param set     The id of the set.
   * @param binding The id of the binding.
   * @param texture The texture to bind.
   * @return true   The texture could be bound.
   * @return false  The texture could not be bound.
   */
  bool SetTexture(unsigned int set, unsigned int binding,
                  SharedTexture texture);
};
SVEL_CLASS(IMaterial)

/**
 * @brief Interface for a SceneMaterial to derive your static material from.
 * This material will only be used once a frame to write unique scene attributes
 * to the shader (like projection matrix/view matrix).
 */
class ISceneMaterial : public IMaterial {
protected:
  /**
   * @brief Construct a SceneMaterial with the corresponding pipeline.
   *
   * @param pipeline The pipeline which the material is for.
   */
  ISceneMaterial(SharedPipeline pipeline);
};
SVEL_CLASS(ISceneMaterial)

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_DETAIL_MATERIAL_H__ */