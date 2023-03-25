/**
 * @file shader.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declares classes for shader creation.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __SVEL_DETAIL_SHADER_H__
#define __SVEL_DETAIL_SHADER_H__

#include <svel/config.h>

#include <memory>
#include <unordered_map>

namespace SVEL_NAMESPACE {

/**
 * @brief Type of a shader binding.
 */
enum class BindingType {
  eUniformBuffer,        // Uniform buffer of static size
  eUniformBufferDynamic, // Uniform buffer of dynamic size
  eCombinedImageSampler  // Texture i.e. sampler2D
};

/**
 * @brief Allows description of a binding set layout.
 */
class SetLayout {
public:
  /**
   * @brief Describes a single binding.
   */
  struct Binding {
    /**
     * @brief Type of the binding.
     */
    BindingType type;

    /**
     * @brief If applicable: How large the datatype for that binding is in
     * bytes.
     */
    uint32_t dataElementSize;

    /**
     * @brief Construct a Binding from the given type and element size.
     *
     * @param bindingType Type of the binding.
     * @param elementSize When possible, the size of the element passed over the
     *                    binding. (used for uniform buffers)
     */
    Binding(const BindingType &bindingType, uint32_t elementSize = 0)
        : type(bindingType), dataElementSize(elementSize) {}
  };

private:
  /**
   * @brief Contains every binding.
   */
  std::unordered_map<unsigned int, Binding> _bindings;

public:
  /**
   * @brief Adds a single binding to the set.
   *
   * @param id          The id of the binding.
   * @param binding     The binding info.
   * @return SetLayout& Builder-like return value to stack Add()
   */
  SetLayout &Add(unsigned int id, const Binding &binding);

  /**
   * @brief Getter for all bindings that the set contains.
   *
   * @return const std::unordered_map<unsigned int, Binding>& Every binding
   *                                                          inside of this
   *                                                          set.
   */
  const std::unordered_map<unsigned int, Binding> &GetBindings() const;
};

/**
 * @brief Describes the interface of a shader.
 */
class Shader {
public:
  /**
   * @brief Possible types of shaders.
   */
  enum class Type { eFragment, eVertex };

  /**
   * @brief Destroy the Shader.
   */
  virtual ~Shader(){};

  /**
   * @brief Add a new set to the layout of the shader.
   *
   * @param id        The id of the set.
   * @param setLayout The set layout to add.
   * @return Shader&  Builder-like return value to stack set additions.
   */
  virtual Shader &AddSetLayout(unsigned int id, const SetLayout &setLayout) = 0;
};
SVEL_CLASS(Shader);

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_DETAIL_SHADER_H__ */