/**
 * @file scene.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the SceneMaterial interface.
 * @date 2023-03-24
 *
 * @copyright Copyright (c) 2023
 *
 */

// Local
#include "material.h"

// Internal
#include <svel/detail/material.h>

// Vulkan
#include <vulkan/vulkan_enums.hpp>

using namespace SVEL_NAMESPACE;

ISceneMaterial::ISceneMaterial(SharedPipeline pipeline) : IMaterial(pipeline) {
  // Currently a workaround
  __getImpl()->SetAvailableAttributeType({vk::DescriptorType::eUniformBuffer});
}
