#include "set.h"
#include "core/descriptor/dynamic_buffer.h"
#include "core/descriptor/static_buffer.h"

using namespace core::descriptor;

uint32_t Set::VectorHasher::operator()(const std::vector<uint32_t> &V) const {
  uint32_t hash = (uint32_t)V.size();
  for (auto &i : V) {
    hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2);
  }
  return hash;
}

SharedIBuffer Set::_createBuffer(const BindingDetails &bindingDetails,
                                 bool &out_dynamicBuffer) {
  // Check if this is a dynamic Buffer
  if (bindingDetails.type == vk::DescriptorType::eStorageBufferDynamic ||
      bindingDetails.type == vk::DescriptorType::eUniformBufferDynamic) {
    auto buffer = std::make_shared<DynamicBuffer>(
        _device, bindingDetails.elementSize, bindingDetails.type);
    out_dynamicBuffer = true;
    return buffer;
  }
  out_dynamicBuffer = false;
  return std::make_shared<StaticBuffer>(_device, bindingDetails.type,
                                        bindingDetails.elementSize);
}

void Set::SetDefaultTexture(std::shared_ptr<ImageDescriptor> defaultTexture) {
  _defaultTexture = defaultTexture;
}

Set::Set(core::SharedDevice device, SharedAllocator staticAllocator,
         vk::DescriptorSetLayout layout, std::vector<BindingDetails> &details)
    : _device(device), _staticAllocator(staticAllocator), _layout(layout) {
  // Create Manager for on-the-fly descriptorSets
  _dynamicAllocator = std::make_unique<Allocator>(_device);

  // Create base DescriptorSet
  _baseDescriptorSet = staticAllocator->AllocateSet(_layout);

  // Create Buffers and WriteSets
  for (const auto &detail : details) {
    // Generate Write Set
    vk::WriteDescriptorSet writeSet(_baseDescriptorSet, detail.binding, 0,
                                    detail.type, {}, {}, {});

    // Check if we deal with a buffer
    if (detail.type != vk::DescriptorType::eCombinedImageSampler) {
      bool dynamicBufferCreated = false;
      auto buffer = _createBuffer(detail, dynamicBufferCreated);
      if (dynamicBufferCreated)
        _dynamicBuffers.push_back(buffer);

      // There should be no duplicates
      if (_buffers.find(detail.binding) != _buffers.end())
        throw std::logic_error("Duplicate Binding in Descriptor Set");

      writeSet.setBufferInfo(buffer->GetBufferInfo());
      _buffers[detail.binding] = buffer;
    } else {
      writeSet.setImageInfo(_defaultTexture->GetImageInfo());
      _buffers[detail.binding] = nullptr;
    }

    _bindingToWriteSetMapping[detail.binding] = (unsigned int)_writeSets.size();
    _writeSets.push_back(writeSet);
  }

  // Update our base Descriptor Set
  _device->AsVulkanObj().updateDescriptorSets(_writeSets, {});

  // Set Buffer Indices
  _bufferIndices.assign(details.size(), 0);
}

SharedIBuffer Set::GetBuffer(uint32_t binding) { return _buffers[binding]; }

vk::DescriptorSet Set::Get(std::vector<uint32_t> &out_offsets) {
  // Append Offsets
  if (_dynamicBuffers.size() > 0)
    for (auto dynBuffer : _dynamicBuffers)
      out_offsets.push_back(dynBuffer->GetBufferOffset());

  // Check if we actually are required to look into the cache
  if (!_isBaseDescriptorSetOutdated)
    return _baseDescriptorSet;

  // Check Cache
  auto it = _descriptorSetCache.find(_bufferIndices);
  if (it == _descriptorSetCache.end()) {
    // We need to allocate a new descriptorSet
    auto set = _dynamicAllocator->AllocateSet(_layout);
    for (auto &writeSet : _writeSets)
      writeSet.setDstSet(set);
    _device->AsVulkanObj().updateDescriptorSets(_writeSets, {});
    _descriptorSetCache[_bufferIndices] = set;
    return set;
  }
  return it->second;
}

void Set::Reset() {
  // Clear Pool and Descriptor Sets
  _dynamicAllocator->ResetPools();
  _descriptorSetCache.clear();

  // Reset Dynamic Buffers
  for (auto &buffer : _dynamicBuffers)
    buffer->Reset();

  // Reset WriteSets
  for (auto &bindingToWrite : _bindingToWriteSetMapping) {
    _bufferIndices[bindingToWrite.second] = 0;
    auto &writeSet = _writeSets[bindingToWrite.second];
    auto buffer = _buffers[bindingToWrite.first];

    if (buffer != nullptr)
      writeSet.setBufferInfo(buffer->GetBufferInfo());
    else
      writeSet.setImageInfo(_defaultTexture->GetImageInfo());
  }

  // Reset Texture Vector
  _boundTextures.clear();

  _isBaseDescriptorSetOutdated = false;
}

void Set::NotifyBufferChange(uint32_t binding) {
  // Get index of this binding
  uint32_t index = _bindingToWriteSetMapping[binding];
  auto buffer = _buffers[binding];

  // Update Indices and writeSet
  _bufferIndices[index] = buffer->GetBufferIndex();
  _writeSets[index].setBufferInfo(buffer->GetBufferInfo());
  _isBaseDescriptorSetOutdated = true;
}

unsigned int Set::BindTexture(ImageDescriptor *texture, uint32_t binding) {
  unsigned int textureID = (unsigned int)_boundTextures.size();
  _boundTextures.push_back(texture);
  BindTexture(textureID, binding);
  return textureID;
}

void Set::BindTexture(unsigned int identifier, uint32_t binding) {
  // Get index of this binding
  uint32_t index = _bindingToWriteSetMapping[binding];

  // Update Indices and writeSet
  _bufferIndices[index] = identifier;
  _writeSets[index].setImageInfo(_boundTextures[identifier]->GetImageInfo());
  _isBaseDescriptorSetOutdated = true;
}
