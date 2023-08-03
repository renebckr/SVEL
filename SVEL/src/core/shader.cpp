/**
 * @file shader.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Vulkan Shader Module Wrapper implementation
 * @date 2022-10-02
 *
 * @copyright Copyright (c) 2022
 *
 */

// Local
#include "shader.h"

// STL
#include <fstream>
#include <stdexcept>

using namespace core;

std::vector<char> Shader::_loadShaderFile(const std::string &path) {
  std::ifstream in(path, std::ios::binary | std::ios::ate);

  // Check stream
  if (!in) {
    std::string errMsg = "Could not open file: ";
    errMsg += path;
    throw std::runtime_error(errMsg.c_str());
  }

  // Get size
  auto size = in.tellg();
  if (in.fail())
    throw std::runtime_error("Failed to find file size.");

  // Read all the data
  std::vector<char> data((unsigned long)size);
  in.seekg(0);
  in.read(data.data(), size);
  in.close();
  return data;
}

void Shader::_setStageFlags(Type type) {
  switch (type) {
  case Shader::Type::eVertex:
    _stage = vk::ShaderStageFlagBits::eVertex;
    break;
  case Shader::Type::eFragment:
    _stage = vk::ShaderStageFlagBits::eFragment;
    break;
  default:
    _stage = vk::ShaderStageFlagBits::eAll;
  }
}

Shader::Shader(core::SharedDevice device, const std::string &path, Type type)
    : _device(device) {
  // Create Module
  auto data = _loadShaderFile(path);
  _setStageFlags(type);

  if (data.size() % 4 != 0) // Shader files have compiled size of 4 * n bytes
    throw std::runtime_error("Shader file is invalid/corrupted.");

  vk::ShaderModuleCreateInfo shaderModuleInfo(
      vk::ShaderModuleCreateFlagBits(), data.size(), (uint32_t *)data.data());

  _vulkanObj = device->AsVulkanObj().createShaderModule(shaderModuleInfo);
}

Shader::~Shader() { _device->AsVulkanObj().destroyShaderModule(_vulkanObj); }

vk::ShaderStageFlags Shader::GetStage() const { return _stage; }

void Shader::AddBinding(const Binding &bindingInfo) {
  _bindings.emplace_back(bindingInfo);
}

const std::vector<Shader::Binding> &Shader::GetBindingLayout() const {
  return _bindings;
}