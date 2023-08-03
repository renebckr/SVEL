/**
 * @file buffer.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the Buffer.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

// Local
#include "buffer.h"

// Internal
#include <core/memory/transfer_buffer.h>

using namespace renderer;
using namespace SVEL_NAMESPACE;

void Buffer::_onCompletion(core::SharedBuffer buffer) {
  // Update internal state
  _buffer = buffer;
  _vulkanObj = _buffer->AsVulkanObj();
  _bufferReady = true;
}

void Buffer::Transfer(core::SharedDevice device, core::Barrier *barrier,
                      const vk::CommandPool &commandPool,
                      const ArrayProxy &data, vk::BufferUsageFlagBits usage) {
  // Use transfer buffer
  auto transferBuffer = std::make_shared<core::TransferBuffer>(
      device, commandPool, data, usage,
      std::bind(&Buffer::_onCompletion, this->shared_from_this(),
                std::placeholders::_1));

  // Transfer the data
  transferBuffer->TransferData(barrier);
  _elementCount = (unsigned int)data.elementCount;
}

const unsigned int &Buffer::GetElementCount() const { return _elementCount; }

bool Buffer::IsBufferReady() { return _bufferReady; }