#include "group.h"
#include "allocator.h"
#include "queue.h"
#include "set.h"

#include <algorithm>
#include <stdexcept>
#include <vulkan/vulkan_handles.hpp>

using namespace core::descriptor;

WriteHandler::WriteHandler(SharedSet set, uint32_t binding)
    : _set(set), _binding(binding) {
  _buffer = _set->GetBuffer(_binding);
}

void WriteHandler::WriteData(void *_data) {
  unsigned int oldBuffer = _buffer->GetBufferIndex();
  _buffer->Write(_data);
  if (oldBuffer != _buffer->GetBufferIndex())
    _set->NotifyBufferChange(_binding);
  return;
}

void SetGroup::_createQueue(
    std::shared_ptr<core::Device> device, uint32_t copyCount,
    std::vector<vk::DescriptorSetLayoutBinding> &layoutBindings,
    std::vector<BindingDetails> &bindingDetails) {
  auto layout = _staticAllocator->CreateLayout(layoutBindings);
  // Need to create Sets
  std::vector<SharedSet> newSets;
  for (unsigned int i = 0; i < copyCount; i++)
    newSets.push_back(std::make_shared<Set>(device, _staticAllocator, layout,
                                            _defaultTexture, bindingDetails));

  // Create Queue
  QueueDetails details;
  details.queue = std::make_unique<SetQueue>(newSets);
  _queueDetails.push_back(std::move(details));
  _layouts.push_back(layout);
}

void SetGroup::_grabSets() {
  for (auto &detail : _queueDetails)
    detail.currentSet = detail.queue->Next();
}

SetGroup::SetGroup(std::shared_ptr<core::Device> device,
                   std::vector<core::SharedShader> &shaders,
                   unsigned int maxFramesInFlight,
                   engine::TextureInterface *defaultTexture)
    : _defaultTexture(defaultTexture) {
  _staticAllocator = std::make_shared<Allocator>(device);

  SetDetails details;

  // Populate details
  for (const auto &shader : shaders) {
    auto layouts = shader->GetLayout();
    auto flags = shader->GetStage();
    for (const auto &binding : layouts)
      details.push_back({flags, binding});
  }

  // Sort incoming details
  std::sort(details.begin(), details.end(),
            [](const BindingInfo &a, const BindingInfo &b) {
              const auto &_a = a.second, _b = b.second;
              return _a.setId == _b.setId ? _a.bindingId < _b.bindingId
                                          : _a.setId < _b.setId;
            });

  // Assert start with setID 0
  if (details.front().second.setId != 0)
    throw std::logic_error("A set with ID=0 is required.");

  unsigned int currentSet = 0;
  std::vector<vk::DescriptorSetLayoutBinding> layoutBindings;
  std::vector<BindingDetails> bindingDetails;
  for (const auto &[shaderFlags, detail] : details) {
    // Check if we entered a new set
    if (currentSet != detail.setId) {
      _createQueue(device, maxFramesInFlight, layoutBindings, bindingDetails);
      bindingDetails.clear();
      layoutBindings.clear();
      currentSet++;
    }

    // Check if we encounter a hole
    if (currentSet != detail.setId)
      throw std::logic_error(
          "SetIDs need to be increasing. There shall be no holes.");

    // Add Binding informations
    layoutBindings.push_back(vk::DescriptorSetLayoutBinding(
        detail.bindingId, detail.type, 1, shaderFlags, nullptr));

    bindingDetails.push_back(
        BindingDetails{detail.bindingId, detail.type, detail.elementSize});
  }
  _createQueue(device, maxFramesInFlight, layoutBindings, bindingDetails);
  _grabSets();
}

void SetGroup::NotifyNewFrame() { _grabSets(); }

void SetGroup::Bind(vk::CommandBuffer &commandBuffer,
                    const vk::PipelineLayout &layout) {
  std::vector<vk::DescriptorSet> sets;
  std::vector<uint32_t> offsets;

  sets.reserve(_queueDetails.size());
  for (const auto &detail : _queueDetails)
    sets.emplace_back(detail.currentSet->Get(offsets));

  commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, layout, 0,
                                   sets, offsets);
}

const std::vector<vk::DescriptorSetLayout> &SetGroup::GetLayouts() {
  return _layouts;
}

WriteHandler SetGroup::GetBuffer(uint32_t setId, uint32_t binding) {
  return WriteHandler(_queueDetails.at(setId).currentSet, binding);
}

unsigned int SetGroup::BindTexture(engine::TextureInterface *texture,
                                   uint32_t setId, uint32_t binding) {
  return _queueDetails.at(setId).currentSet->BindTexture(texture, binding);
}

void SetGroup::RebindTexture(unsigned int textureId, uint32_t setId,
                             uint32_t binding) {
  _queueDetails.at(setId).currentSet->BindTexture(textureId, binding);
}
