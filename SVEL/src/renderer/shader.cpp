/**
 * @file shader.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the VulkanShader.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

// Local
#include "shader.h"

// Internal
#include <core/shader.h>

// Vulkan
#include <vulkan/vulkan_enums.hpp>

// STL
#include <memory>
#include <stdexcept>

using namespace SVEL_NAMESPACE;

SetLayout &SetLayout::Add(unsigned int id, const Binding &binding) {
  _bindings.emplace(id, binding);
  return *this;
}

const std::unordered_map<unsigned int, SetLayout::Binding> &
SetLayout::GetBindings() const {
  return _bindings;
}

VulkanShader::VulkanShader(core::SharedDevice device,
                           const std::string &filepath, Type shaderType) {
  _shader = std::make_shared<core::Shader>(
      device, filepath, static_cast<core::Shader::Type>(shaderType));
}

Shader &VulkanShader::AddSetLayout(unsigned int id,
                                   const SetLayout &setLayout) {
  // Validate if set already exists
  if (_setIds.find(id) != _setIds.end())
    throw std::invalid_argument("Can only add one set layout per id.");
  _setIds.insert(id);

  // Convert Binding to Shader Binding information
  auto bindings = setLayout.GetBindings();
  for (const auto &[bindingId, binding] : bindings) {
    core::Shader::Binding shaderBinding;
    shaderBinding.bindingId = bindingId;
    shaderBinding.elementSize = binding.dataElementSize;
    shaderBinding.setId = id;

    switch (binding.type) {
    case BindingType::eUniformBuffer:
      shaderBinding.type = vk::DescriptorType::eUniformBuffer;
      break;
    case BindingType::eUniformBufferDynamic:
      shaderBinding.type = vk::DescriptorType::eUniformBufferDynamic;
      break;
    case BindingType::eCombinedImageSampler:
      shaderBinding.type = vk::DescriptorType::eCombinedImageSampler;
      break;
    }

    // Add to the shader
    _shader->AddBinding(shaderBinding);
  }
  return *this;
}

core::SharedShader VulkanShader::GetShader() const { return _shader; }