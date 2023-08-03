/**
 * @file shader.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declaration of Shader.
 * @date 2023-03-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __CORE_SHADER_H__
#define __CORE_SHADER_H__

// Local
#include "device.h"

// Internal
#include <svel/detail/shader.h>
#include <util/vulkan_object.hpp>

// Vulkan
#include <vulkan/vulkan.hpp>

namespace core {

class Shader : public util::VulkanAdapter<vk::ShaderModule> {
public:
  /**
   * @brief Type of Shader.
   */
  enum class Type {
    eVertex = static_cast<int>(SVEL_NAMESPACE::Shader::Type::eVertex),
    eFragment = static_cast<int>(SVEL_NAMESPACE::Shader::Type::eFragment)
  };

  /**
   * @brief Compound that describes a binding for a shader.
   */
  struct Binding {
    /**
     * @brief Set id of the binding.
     */
    uint32_t setId;

    /**
     * @brief Binding id inside of the set.
     */
    uint32_t bindingId;

    /**
     * @brief Type of the binding.
     */
    vk::DescriptorType type;

    /**
     * @brief Size of a single element in the binding if applicable.
     */
    size_t elementSize;
  };

private:
  /**
   * @brief Device to use.
   */
  core::SharedDevice _device;

  /**
   * @brief Flags for this shader stage.
   */
  vk::ShaderStageFlags _stage;

  /**
   * @brief List of all bindings the shader has.
   */
  std::vector<Binding> _bindings;

  /**
   * @brief Load a shader from a file.
   *
   * @param _path Path to a valid shader file compiled by SPIRV
   * @return std::vector<char> Data in the shaderfile
   */
  std::vector<char> _loadShaderFile(const std::string &path);

  /**
   * @brief Sets stage flags for the given shader module type.
   *
   * @param _type Type of the shader module
   */
  void _setStageFlags(Type _type);

public:
  /**
   * @brief Construct a new Shader Module.
   *
   * @param device  Device to use
   * @param path    Path to the shader file
   * @param type    Type of the shader module
   */
  Shader(core::SharedDevice device, const std::string &path, Type type);

  /**
   * @brief Destroy the Shader Module.
   */
  ~Shader();

  /**
   * @brief Getter for the stage flags of the shader.
   *
   * @return vk::ShaderStageFlags Shader Stage Flags
   */
  vk::ShaderStageFlags GetStage() const;

  /**
   * @brief Adds a binding to the shader.
   *
   * @param setLayout Layout of the binding.
   */
  void AddBinding(const Binding &bindingInfo);

  /**
   * @brief Getter for the binding layout.
   *
   * @return const std::vector<Binding>& The binding layout.
   */
  const std::vector<Binding> &GetBindingLayout() const;
};
SVEL_CLASS(Shader);

} // namespace core

#endif /* __CORE_SHADER_H__ */