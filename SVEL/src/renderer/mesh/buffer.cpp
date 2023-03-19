#include "buffer.h"
#include "core/memory/transfer_buffer.h"

using namespace renderer;
using namespace SVEL_NAMESPACE;

void Buffer::_onCompletion(core::SharedBuffer buffer) {
  _buffer = buffer;
  _vulkanObj = _buffer->AsVulkanObj();
  _bufferReady = true;
}

void Buffer::Transfer(core::SharedDevice device, core::Barrier *barrier,
                      const vk::CommandPool &commandPool,
                      const ArrayProxy &data, vk::BufferUsageFlagBits usage) {
  auto transferBuffer = std::make_shared<core::TransferBuffer>(
      device, commandPool, data, usage,
      std::bind(&Buffer::_onCompletion, this->shared_from_this(),
                std::placeholders::_1));
  transferBuffer->TransferData(barrier);
  _elementCount = (unsigned int)data.elementCount;
}

const unsigned int &Buffer::GetElementCount() const { return _elementCount; }

bool Buffer::IsBufferReady() { return _bufferReady; }