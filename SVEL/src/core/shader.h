#ifndef __CORE_SHADER_H__
#define __CORE_SHADER_H__

#include "core/device.h"
#include "util/vulkan_object.hpp"

#include <svel/detail/shader.h>
#include <vulkan/vulkan.hpp>

namespace core {

class Shader : public util::VulkanAdapter<vk::ShaderModule> {
public:
  /**
   * @brief Type of Shader
   */
  enum class Type {
    VERTEX = static_cast<int>(SVEL_NAMESPACE::Shader::Type::eVertex),
    FRAGMENT = static_cast<int>(SVEL_NAMESPACE::Shader::Type::eFragment)
  };

  struct Binding {
    uint32_t setId;
    uint32_t bindingId;
    vk::DescriptorType type;
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
   * @param device Device to use
   * @param path Path to the shader file
   * @param type Type of the shader module
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

  void AddBinding(const Binding &setLayout);

  const std::vector<Binding> &GetLayout() const;
};
SVEL_CLASS(Shader);

} // namespace core

#endif /* __CORE_SHADER_H__ */