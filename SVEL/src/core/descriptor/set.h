#ifndef __CORE_DESCRIPTOR_SET_H__
#define __CORE_DESCRIPTOR_SET_H__

#include "core/descriptor/allocator.h"
#include "core/descriptor/buffer.h"
#include "core/descriptor/image.hpp"
#include "core/device.h"

#include <unordered_map>

namespace core::descriptor {
struct BindingDetails {
  uint32_t binding;
  vk::DescriptorType type;
  size_t elementSize;
};

class Set {
private:
  inline static std::shared_ptr<ImageDescriptor> _defaultTexture = nullptr;

  struct VectorHasher {
    uint32_t operator()(const std::vector<uint32_t> &V) const;
  };

  core::SharedDevice _device;
  SharedAllocator _staticAllocator;
  UniqueAllocator _dynamicAllocator;
  vk::DescriptorSetLayout _layout;
  vk::DescriptorSet _baseDescriptorSet;
  std::unordered_map<std::vector<uint32_t>, vk::DescriptorSet, VectorHasher>
      _descriptorSetCache;
  std::unordered_map<uint32_t, SharedIBuffer> _buffers;
  std::vector<ImageDescriptor *> _boundTextures;
  std::vector<SharedIBuffer> _dynamicBuffers;
  std::unordered_map<uint32_t, uint32_t> _bindingToWriteSetMapping;
  std::vector<vk::WriteDescriptorSet> _writeSets;
  std::vector<uint32_t> _bufferIndices;
  bool _isBaseDescriptorSetOutdated;

  SharedIBuffer _createBuffer(const BindingDetails &_bindingDetails,
                              bool &_out_dynamicBuffer);

public:
  static void
  SetDefaultTexture(std::shared_ptr<ImageDescriptor> defaultTexture);

  Set(core::SharedDevice device, SharedAllocator staticAllocator,
      vk::DescriptorSetLayout layout, std::vector<BindingDetails> &details);
  Set(const Set &) = delete;

  SharedIBuffer GetBuffer(uint32_t binding);
  vk::DescriptorSet Get(std::vector<uint32_t> &out_offsets);
  void Reset();
  void NotifyBufferChange(uint32_t binding);
  unsigned int BindTexture(ImageDescriptor *texture, uint32_t binding);
  void BindTexture(unsigned int identifier, uint32_t binding);
};
SVEL_CLASS(Set);

} // namespace core::descriptor

#endif /* __CORE_DESCRIPTOR_SET_H__ */