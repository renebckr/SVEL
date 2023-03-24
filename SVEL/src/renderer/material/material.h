/**
 * @file material.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declaration of the Material Impl.
 * @date 2023-03-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __RENDERER_MATERIAL_MATERIAL_H__
#define __RENDERER_MATERIAL_MATERIAL_H__

// Internal
#include <core/descriptor/group.h>
#include <renderer/pipeline/pipeline.h>
#include <svel/config.h>
#include <svel/detail/material.h>

// STL
#include <unordered_map>
#include <unordered_set>

namespace SVEL_NAMESPACE {

/**
 * @brief Implementation of the Material Interface described in the public
 * header.
 */
class IMaterial::Impl {
private:
  /**
   * @brief Helper struct to hold data associated to one attribute of the
   * shader.
   */
  struct Attribute {
    void *data;
    size_t dataSize;
    std::shared_ptr<core::descriptor::WriteHandler> writeHandler;
  };

  /**
   * @brief The pipeline that the material is created for.
   */
  renderer::SharedVulkanPipeline _pipeline;

  /**
   * @brief Which descriptor types are enabled for this material. Default is the
   * dynamic uniform buffer.
   */
  std::unordered_set<vk::DescriptorType> _enabledTypes;

  /**
   * @brief Type of the slot and which element size should be used for writes.
   */
  std::unordered_map<uint64_t, std::pair<vk::DescriptorType, uint32_t>>
      _slotTypes;

  /**
   * @brief Contains all attributes of this material.
   */
  std::unordered_map<uint64_t, Attribute> _attributes;

  /**
   * @brief Contains all textures that this material uses.
   */
  std::unordered_map<uint64_t, SharedTexture> _textures;

public:
  /**
   * @brief Construct the Material implementation.
   *
   * @param pipeline Pipeline that this material is based on.
   */
  Impl(renderer::SharedVulkanPipeline pipeline);

  /**
   * @brief Adds an attribute to the shader at the specified set and binding.
   *
   * @param setId     The set of the attribute.
   * @param binding   The binding of the attribute.
   * @param data      Pointer to the data of the attribute (i.e. glm::mat4 for a
   *                  model matrix) has to persist as long as the material
   *                  persists!
   * @param dataSize  Size of the data for sanity checking.
   * @return true     True if the attribute was correctly added.
   * @return false    Binding does not exist, Data has invalid size or not
   *                  allowed.
   */
  inline bool AddAttribute(uint32_t setId, uint32_t binding, void *data,
                           size_t dataSize);

  /**
   * @brief Setter for the available attribute type of this material.
   *
   * @param enabledTypes Which types should be enabled for this material.
   */
  void SetAvailableAttributeType(
      const std::unordered_set<vk::DescriptorType> &enabledTypes);

  /**
   * @brief Writes all attributes to the shader.
   */
  void WriteAttributes();

  /**
   * @brief Sets the binding to the provided texture. Only valid if the binding
   * is also specified for texture usage.
   *
   * @param set     Set of the texture.
   * @param binding Binding of the texture.
   * @param texture Texture to use.
   * @return true   Was able to set the texture properly.
   * @return false  Could not set the texture.
   */
  inline bool SetTexture(unsigned int set, unsigned int binding,
                         SharedTexture texture);
};

} // namespace SVEL_NAMESPACE

#endif /* __RENDERER_MATERIAL_MATERIAL_H__ */