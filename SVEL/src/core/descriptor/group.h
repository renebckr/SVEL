/**
 * @file group.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declaration of the SetGroup.
 * @date 2023-03-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __CORE_DESCRIPTOR_GROUP_H__
#define __CORE_DESCRIPTOR_GROUP_H__

// Local
#include "allocator.h"
#include "buffer.h"
#include "image.hpp"
#include "queue.h"
#include "set.h"
#include "write_handler.h"

// Internal
#include <core/shader.h>
#include <svel/config.h>

// Vulkan
#include <vulkan/vulkan.hpp>

// STL
#include <unordered_map>

namespace core::descriptor {

/**
 * @brief Groups multiple sets. Handles frame management for the sets.
 */
class SetGroup {
public:
  /**
   * @brief Binding information for a binding of a shader.
   */
  using BindingInfo = std::pair<vk::ShaderStageFlags, core::Shader::Binding>;

  /**
   * @brief All binding information.
   */
  using Interface = std::vector<BindingInfo>;

private:
  /**
   * @brief Buffers information of the queue and it's state.
   */
  struct QueueDetails {
    /**
     * @brief The queue for the set.
     */
    UniqueSetQueue queue;

    /**
     * @brief The current set of this queue.
     */
    SharedSet currentSet = nullptr;
  };

  /**
   * @brief Layout of every descriptor set.
   */
  std::vector<vk::DescriptorSetLayout> _layouts;

  /**
   * @brief Static allocator that every set can use for base descriptor set
   * allocation.
   */
  SharedAllocator _staticAllocator;

  /**
   * @brief List of all queues and their state.
   */
  std::vector<QueueDetails> _queueDetails;

  /**
   * @brief Complete Interface of the bindings.
   */
  Interface _interface;

  /**
   * @brief All write handlers mapped to setId and bindingId.
   */
  std::unordered_map<uint64_t, std::shared_ptr<WriteHandler>> _writeHandlers;

  /**
   * @brief Create a new queue and fills it with copies of a set that will be
   * created as well.
   *
   * @param device          Device to use.
   * @param copyCount       How many Sets to create as copies.
   * @param layoutBindings  The Layout of the sets.
   * @param bindingDetails  Details of the bindings for this set.
   */
  void _createQueue(std::shared_ptr<core::Device> device, uint32_t copyCount,
                    std::vector<vk::DescriptorSetLayoutBinding> &layoutBindings,
                    std::vector<Set::BindingDetails> &bindingDetails);

  /**
   * @brief Issues all queues to grab the next set and updates states
   * accordingly.
   */
  void _grabSets();

public:
  /**
   * @brief Constructs the set group.
   *
   * @param device            Device to use.
   * @param shaders           Shaders to use for this group. These have to be
   *                          associated with the pipeline that will be used
   *                          together with this group.
   * @param maxFramesInFlight How many frames can be in flight at the same time.
   */
  SetGroup(std::shared_ptr<core::Device> device,
           std::vector<core::SharedShader> &shaders,
           unsigned int maxFramesInFlight);

  /**
   * @brief Set group cannot be copied.
   */
  SetGroup(const SetGroup &) = delete;

  /**
   * @brief Notify the group that a new frame is being created. Will update
   * internal states accordingly.
   */
  void NotifyNewFrame();

  /**
   * @brief Records the descriptor set binding to the commandBuffer.
   *
   * @param commandBuffer The buffer to record to.
   * @param layout        The layout of the pipeline. Has to match shaders.
   */
  void Bind(vk::CommandBuffer &commandBuffer, const vk::PipelineLayout &layout);

  /**
   * @brief Getter for all descriptor set layouts.
   *
   * @return const std::vector<vk::DescriptorSetLayout>&  All descriptor set
   *                                                      layouts.
   */
  const std::vector<vk::DescriptorSetLayout> &GetLayouts();

  /**
   * @brief Getter for Write Handler associated to the setId and bindingId.
   *
   * @param setId                           The set identifier.
   * @param binding                         The binding identifier.
   * @return std::shared_ptr<WriteHandler>  Write handler that writes to the
   *                                        proper location for the set and
   *                                        binding.
   */
  std::shared_ptr<WriteHandler> GetWriteHandler(uint32_t setId,
                                                uint32_t binding);

  /**
   * @brief Binds a texture to the setId and bindingId.
   *
   * @param texture       Texture to bind.
   * @param setId         The set identifier.
   * @param binding       The binding identifier.
   * @return unsigned int Id to use for rebinds during the current frame.
   */
  unsigned int BindTexture(ImageDescriptor *texture, uint32_t setId,
                           uint32_t binding);

  /**
   * @brief Binds a texture again. The textureId has to be provided by a bind
   * call first.
   *
   * @param textureId Id provided by BindTexture
   * @param setId     The set identifier.
   * @param binding   The binding identifier.
   */
  void RebindTexture(unsigned int textureId, uint32_t setId, uint32_t binding);

  /**
   * @brief Getter for the complete set and binding interface of this group.
   *
   * @return const Interface& Set and binding interface of this group.
   */
  const Interface &GetInterface() const;
};
SVEL_CLASS(SetGroup)

} // namespace core::descriptor

#endif /* __CORE_DESCRIPTOR_GROUP_H__ */