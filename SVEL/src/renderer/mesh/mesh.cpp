/**
 * @file mesh.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the Mesh.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

// Local
#include "mesh.h"

// Internal
#include <core/barrier.h>
#include <renderer/mesh/buffer.h>

// Vulkan
#include <vulkan/vulkan_enums.hpp>

using namespace SVEL_NAMESPACE;

Mesh::Mesh(core::SharedDevice device, const vk::CommandPool &commandPool,
           const ArrayProxy &nodes, const ArrayProxy &indices,
           vk::IndexType iboType)
    : _vbo(std::make_shared<renderer::Buffer>()),
      _ibo(std::make_shared<renderer::Buffer>()), _iboType(iboType) {
  // Transfer all of the data and wait for the transfer to complete.
  // TODO: Allow barrier to be provided in the future.
  auto barrier = std::make_shared<core::Barrier>(device);
  _vbo->Transfer(device, barrier.get(), commandPool, nodes,
                 vk::BufferUsageFlagBits::eVertexBuffer);
  _ibo->Transfer(device, barrier.get(), commandPool, indices,
                 vk::BufferUsageFlagBits::eIndexBuffer);
  barrier->WaitCompletion();
}

void Mesh::Draw(const vk::CommandBuffer &recordBuffer) {
  recordBuffer.bindVertexBuffers(0, _vbo->AsVulkanObj(), _bufferOffsets);
  recordBuffer.bindIndexBuffer(_ibo->AsVulkanObj(), _bufferOffsets, _iboType);
  recordBuffer.drawIndexed(_ibo->GetElementCount(), 1, 0, 0, 0);
}