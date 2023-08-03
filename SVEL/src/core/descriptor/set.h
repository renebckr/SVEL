/**
 * @file set.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declaration of the Set.
 * @date 2023-03-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __CORE_DESCRIPTOR_SET_H__
#define __CORE_DESCRIPTOR_SET_H__

// Local
#include "allocator.h"
#include "buffer.h"
#include "image.hpp"

// Internal
#include <core/device.h>

// STL
#include <unordered_map>

namespace core::descriptor {

/**
 * @brief Handles everything concerning Vulkan Descriptor Sets.
 */
class Set {
public:
  /**
   * @brief Describes a single binding of the descriptor set.
   */
  struct BindingDetails {
    /**
     * @brief The binding identifier.
     */
    uint32_t binding;

    /**
     * @brief Type of the binding.
     */
    vk::DescriptorType type;

    /**
     * @brief Size of an element for this binding if applicable.
     */
    size_t elementSize;
  };

private:
  /**
   * @brief General hasher for uint32_t vectors.
   */
  struct VectorHasher {
    /**
     * @brief Hash operation.
     *
     * @param V         Vector to hash.
     * @return uint32_t The hash result.
     */
    uint32_t operator()(const std::vector<uint32_t> &V) const;
  };

  /**
   * @brief The default texture for the engine.
   */
  inline static std::shared_ptr<ImageDescriptor> _defaultTexture = nullptr;

  /**
   * @brief Device to use.
   */
  core::SharedDevice _device;

  /**
   * @brief Allocator that is used across multiple sets and should only be used
   * statically.
   */
  SharedAllocator _staticAllocator;

  /**
   * @brief Allocator for this set. Can be used for dynamic set allocation.
   */
  UniqueAllocator _dynamicAllocator;

  /**
   * @brief Layout of the descriptor set.
   */
  vk::DescriptorSetLayout _layout;

  /**
   * @brief Initial descriptor set. Reusable for every frame as the initial
   * set.
   */
  vk::DescriptorSet _baseDescriptorSet;

  /**
   * @brief Cache for all the descriptor sets that were allocated. Key is a
   * vector of identifiers that uniquely represent the buffer status of the set.
   * Every change that requires a write set change has to conclude in a new set.
   * To allow reuse of sets, use the cache.
   */
  std::unordered_map<std::vector<uint32_t>, vk::DescriptorSet, VectorHasher>
      _descriptorSetCache;

  /**
   * @brief All static buffers that the set refers to.
   */
  std::unordered_map<uint32_t, SharedIBuffer> _buffers;

  /**
   * @brief All textures that have been bound so far.
   */
  std::vector<ImageDescriptor *> _boundTextures;

  /**
   * @brief All dynamic buffers that the set refers to.
   */
  std::vector<SharedIBuffer> _dynamicBuffers;

  /**
   * @brief Maps the binding identifier to the write set index. This is required
   * as users may use binding 0, x but not binding 1, ..., x - 1.
   */
  std::unordered_map<uint32_t, uint32_t> _bindingToWriteSetMapping;

  /**
   * @brief All write sets of the descriptor set.
   */
  std::vector<vk::WriteDescriptorSet> _writeSets;

  /**
   * @brief Identifiers of the current set. Can be used as lookup for the set
   * cache.
   */
  std::vector<uint32_t> _setIdentifiers;

  /**
   * @brief Describes if the base set can still be used.
   */
  bool _isBaseDescriptorSetOutdated;

  /**
   * @brief Create a buffer for the provided binding details
   *
   * @param _bindingDetails     Details to use for the buffer.
   * @param _out_dynamicBuffer  Output that notifies the caller wheather the
   *                            created buffer is a dynamic buffer or not.
   * @return SharedIBuffer      The created buffer.
   */
  SharedIBuffer _createBuffer(const BindingDetails &bindingDetails,
                              bool &out_dynamicBuffer);

public:
  /**
   * @brief Setter for the default texture of the engine.
   *
   * @param defaultTexture The default texture to use.
   */
  static void
  SetDefaultTexture(std::shared_ptr<ImageDescriptor> defaultTexture);

  /**
   * @brief Construct a Set with the given layout and binding details.
   *
   * @param device          Device to use.
   * @param staticAllocator Static allocator to use for the base set.
   * @param layout          Descriptor set layout to use.
   * @param details         Description of the bindings used in the set.
   */
  Set(core::SharedDevice device, SharedAllocator staticAllocator,
      vk::DescriptorSetLayout layout, std::vector<BindingDetails> &details);

  /**
   * @brief The set needs to stay unique. And should never be copied.
   */
  Set(const Set &) = delete;

  /**
   * @brief Getter for the buffer at the binding.
   *
   * @param binding         Binding to fetch the buffer for.
   * @return SharedIBuffer  Buffer for the binding. Invalid Buffer if buffer
   * does not exist.
   */
  SharedIBuffer GetBuffer(uint32_t binding);

  /**
   * @brief Getter for the currently used descriptor set. Will also return all
   * offsets of the set. Sets may get allocated here internally. TODO: Sets
   * should probably be allocated somewhere else.
   *
   * @param out_offsets         Offsets of the set.
   * @return vk::DescriptorSet  The current descriptor set.
   */
  vk::DescriptorSet Get(std::vector<uint32_t> &out_offsets);

  /**
   * @brief Should be called by anybody who wrote to a buffer provided by
   * GetBuffer and then noticed a buffer index change.
   *
   * @param binding Binding identifier of the buffer.
   */
  void NotifyBufferChange(uint32_t binding);

  /**
   * @brief Binds the provided texture to the given binding identifier.
   *
   * @param texture       Texture to bind.
   * @param binding       Binding identifier.
   * @return unsigned int Identifier of the texture. Persists until Reset.
   */
  unsigned int BindTexture(ImageDescriptor *texture, uint32_t binding);

  /**
   * @brief Binds the texture associated to the identifier to the given binding
   * identifier.
   *
   * @param identifier  Identifier of a texture returned by a previous bind
   *                    call.
   * @param binding     Binding identifier.
   */
  void BindTexture(unsigned int identifier, uint32_t binding);

  /**
   * @brief Resets the set. Resets buffers, invalidates old sets and any texture
   * identifiers.
   */
  void Reset();
};
SVEL_CLASS(Set);

} // namespace core::descriptor

#endif /* __CORE_DESCRIPTOR_SET_H__ */