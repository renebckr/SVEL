/**
 * @file shader.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declaration of the VulkanShader.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __RENDERER_SHADER_H__
#define __RENDERER_SHADER_H__

// Internal
#include <core/descriptor/group.h>
#include <core/shader.h>
#include <svel/detail/shader.h>
#include <util/downcast_impl.hpp>

// STL
#include <unordered_set>

/**
 * @brief Implements the user interface Shader. Allows building descriptor set
 * layouts. TODO: Should this even be in it's own class?
 */
class VulkanShader : public SVEL_NAMESPACE::Shader {
private:
  /**
   * @brief The shader itself.
   */
  core::SharedShader _shader;

  /**
   * @brief Sets added to the layout.
   */
  std::unordered_set<unsigned int> _setIds;

public:
  /**
   * @brief Construct a Vulkan Shader. Will load the shader from the filepath.
   *
   * @param device      Device to use.
   * @param filepath    Filepath from which to load the shader from.
   * @param shaderType  Type of the shader.
   */
  VulkanShader(core::SharedDevice device, const std::string &filepath,
               Type shaderType);

  /**
   * @brief Implementation of the AddSetLayout Interface.
   *
   * @param id                        The id of the set.
   * @param setLayout                 The layout of the set.
   * @return SVEL_NAMESPACE::Shader&  Builder pattern return value.
   */
  SVEL_NAMESPACE::Shader &
  AddSetLayout(unsigned int id,
               const SVEL_NAMESPACE::SetLayout &setLayout) final override;

  /**
   * @brief Getter for the shader itself.
   *
   * @return core::SharedShader The shader itself.
   */
  core::SharedShader GetShader() const;
};
SVEL_CLASS(VulkanShader)
SVEL_DOWNCAST_IMPL(VulkanShader, SVEL_NAMESPACE::Shader)

#endif /* __RENDERER_SHADER_H__ */