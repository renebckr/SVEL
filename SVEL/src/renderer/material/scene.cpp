#include "material.h"

#include <svel/detail/material.h>
#include <vulkan/vulkan_enums.hpp>

using namespace SVEL_NAMESPACE;

ISceneMaterial::ISceneMaterial(SharedPipeline pipeline) : IMaterial(pipeline) {
  __getImpl()->SetAvailableAttributeType({vk::DescriptorType::eUniformBuffer});
}
