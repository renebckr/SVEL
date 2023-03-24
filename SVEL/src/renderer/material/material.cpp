/**
 * @file material.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the Material and it's Impl.
 * @date 2023-03-24
 *
 * @copyright Copyright (c) 2023
 *
 */

// Local
#include "material.h"

// Internal
#include <core/descriptor/util.hpp>
#include <texture/texture.h>

// STL
#include <iostream>
#include <stdexcept>

using namespace SVEL_NAMESPACE;

// --- IMPL ---

IMaterial::Impl::Impl(renderer::SharedVulkanPipeline pipeline)
    : _pipeline(pipeline) {

  // Setup known interface information
  const auto &interface = pipeline->GetDescriptorGroup()->GetInterface();
  for (const auto &[_, binding] : interface) {
    const uint64_t key =
        core::descriptor::CombineSetBinding(binding.setId, binding.bindingId);
    _slotTypes[key] = {binding.type, binding.elementSize};
  }
}

bool IMaterial::Impl::AddAttribute(uint32_t setId, uint32_t binding, void *data,
                                   size_t dataSize) {
  // Check if it has already been added
  const uint64_t key = core::descriptor::CombineSetBinding(setId, binding);
  if (_attributes.find(key) != _attributes.end())
    throw std::invalid_argument("Cannot add attribute twice.");

  // Validate interface
  const auto &slotIt = _slotTypes.find(key);
  if (slotIt == _slotTypes.end())
    throw std::invalid_argument("Set and or binding does not exist.");

  const auto &[type, elementSize] = slotIt->second;
  if (elementSize != (uint32_t)dataSize)
    throw std::invalid_argument("Attribute datasize missmatch.");

  if (_enabledTypes.find(type) == _enabledTypes.end())
    throw std::invalid_argument(
        "Attribute type not enabled for this material.");

  _attributes.emplace(
      key, Attribute{data, dataSize,
                     _pipeline->GetDescriptorGroup()->GetWriteHandler(
                         setId, binding)});
  return true;
}

void IMaterial::Impl::SetAvailableAttributeType(
    const std::unordered_set<vk::DescriptorType> &enabledTypes) {
  _enabledTypes = enabledTypes;
}

void IMaterial::Impl::WriteAttributes() {
  for (const auto &[_, attribute] : _attributes) {
    attribute.writeHandler->WriteData(attribute.data);
  }

  for (const auto &[key, texture] : _textures) {
    const auto &[set, binding] = core::descriptor::ExtractSetBinding(key);
    _pipeline->GetDescriptorGroup()->BindTexture(texture.get(), set, binding);
  }
}

bool IMaterial::Impl::SetTexture(unsigned int set, unsigned int binding,
                                 SharedTexture texture) {
  // Check if already exists
  const uint64_t key = core::descriptor::CombineSetBinding(set, binding);
  if (_textures.find(key) != _textures.end()) {
    _textures[key] = texture;
    return true;
  }

  // Validate interface
  const auto &slotIt = _slotTypes.find(key);
  if (slotIt == _slotTypes.end())
    throw std::invalid_argument("Set and or binding does not exist.");

  const auto &[type, _] = slotIt->second;
  if (type != vk::DescriptorType::eCombinedImageSampler)
    throw std::invalid_argument(
        "Given set, binding does not refer to a valid texture slot.");

  _textures[key] = texture;
  return true;
}

// --- INTERFACE ---

IMaterial::IMaterial(SharedPipeline pipeline) {
  __pImpl = std::make_shared<Impl>(renderer::GetImpl(pipeline));
  __pImpl->SetAvailableAttributeType(
      {vk::DescriptorType::eUniformBufferDynamic});
}

bool IMaterial::_addAttribute(unsigned int set, unsigned int binding,
                              void *data, unsigned int dataSize) {
  __pImpl->AddAttribute(set, binding, data, dataSize);
  return true;
}

bool IMaterial::SetTexture(unsigned int set, unsigned int binding,
                           SharedTexture texture) {
  return __pImpl->SetTexture(set, binding, texture);
}