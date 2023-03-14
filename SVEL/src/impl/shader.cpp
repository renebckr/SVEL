#include "shader.h"
#include "core/shader.h"
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan_enums.hpp>

using namespace SVEL_NAMESPACE;

SetLayout &SetLayout::Add(unsigned int id, const Binding &binding) {
  _bindings.emplace(id, binding);
  return *this;
}

const std::unordered_map<unsigned int, SetLayout::Binding> &
SetLayout::GetBindings() const {
  return _bindings;
}

// --- IMPL ---

IShader::Impl::Impl(core::SharedDevice device, const std::string &filepath,
                    Type shaderType) {
  _shader = std::make_shared<core::Shader>(
      device, filepath, static_cast<core::Shader::Type>(shaderType));
}

void IShader::Impl::AddSetLayout(unsigned int id, const SetLayout &setLayout) {
  if (_setIds.find(id) == _setIds.end())
    throw std::invalid_argument("Can only add one set layout per id.");
  _setIds.insert(id);

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

    _shader->AddBinding(shaderBinding);
  }
}

// --- Interface ---

IShader::IShader(SharedImpl impl) : __pImpl(impl) {}

IShader &IShader::AddSetLayout(unsigned int id, const SetLayout &setLayout) {
  __pImpl->AddSetLayout(id, setLayout);
  return *this;
}