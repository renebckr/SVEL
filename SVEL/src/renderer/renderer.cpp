#include "renderer.h"
#include "core/barrier.h"
#include "io/obj/parser.h"
#include "renderer/pipeline/pipeline.h"
#include "svel/detail/mesh.h"
#include "svel/detail/pipeline.h"
#include "texture/animation.h"
#include "texture/texture.h"
#include <iostream>
#include <renderer/material/material.h>
#include <renderer/mesh/mesh.h>
#include <renderer/pipeline/pipeline.h>
#include <renderer/shader.h>

using namespace SVEL_NAMESPACE;

VulkanRenderer::VulkanRenderer(core::SharedInstance instance,
                               core::SharedSurface surface)
    : _surface(surface) {
  _device = std::make_shared<core::Device>(instance, _surface);
  _swapchain = std::make_shared<core::Swapchain>(_device, _surface);

  // Create Persistent Command pool
  vk::CommandPoolCreateInfo persistentCommandPoolInfo(
      vk::CommandPoolCreateFlagBits(), _device->GetGraphicsQueueFamily());
  _persistentCommandPool =
      _device->AsVulkanObj().createCommandPool(persistentCommandPoolInfo);
}

VulkanRenderer::~VulkanRenderer() {
  _device->AsVulkanObj().destroyCommandPool(_persistentCommandPool);
  _device->AsVulkanObj().waitIdle();
}

SharedShader VulkanRenderer::LoadShader(const std::string &filepath,
                                        Shader::Type type) const {
  return std::make_shared<VulkanShader>(_device, filepath, type);
}

core::SharedDevice VulkanRenderer::GetDevice() { return _device; }

core::SharedSwapchain VulkanRenderer::GetSwapchain() { return _swapchain; }

SharedPipeline
VulkanRenderer::BuildPipeline(SharedShader vert, SharedShader frag,
                              const VertexDescription &description) {
  return std::make_shared<renderer::VulkanPipeline>(
      _device, _surface, _swapchain, GetImpl(vert)->GetShader(),
      GetImpl(frag)->GetShader(), description);
}

void VulkanRenderer::BindPipeline(SharedPipeline pipeline) {
  auto vulkanPipeline = renderer::GetImpl(pipeline);
  _boundPipeline = vulkanPipeline;
  _currentFrame->BindPipeline(vulkanPipeline);
  if (_sceneMaterial != nullptr)
    _sceneMaterial->__getImpl()->WriteAttributes();
}

void VulkanRenderer::UnbindPipeline() { _currentFrame->UnbindPipeline(); }

SharedTexture VulkanRenderer::CreateTexture(SharedImage image) {
  _boundPipeline = nullptr;
  auto texture = std::make_shared<Texture>(_device, image);
  texture->Dispatch(_persistentCommandPool,
                    std::make_shared<core::Barrier>(_device));
  return texture;
}

SharedAnimation
VulkanRenderer::CreateAnimation(const std::vector<SharedImage> &images,
                                float animationSpeed, bool looping) {
  auto animation = std::make_shared<texture::VulkanAnimation>(
      _device, _persistentCommandPool, std::make_shared<core::Barrier>(_device),
      images, animationSpeed, looping);
  return animation;
}

SharedMesh VulkanRenderer::CreateMesh(const ArrayProxy &nodes,
                                      const std::vector<uint16_t> &indices) {
  return std::make_shared<Mesh>(_device, _persistentCommandPool, nodes, indices,
                                vk::IndexType::eUint16);
}

SharedMesh VulkanRenderer::CreateMesh(const ArrayProxy &nodes,
                                      const std::vector<uint32_t> &indices) {
  return std::make_shared<Mesh>(_device, _persistentCommandPool, nodes, indices,
                                vk::IndexType::eUint32);
}

struct VertexData {
  glm::vec3 coord;
  glm::vec3 color;
  glm::vec2 tex;
  glm::vec3 normal;

  VertexData(const glm::vec3 &c, const glm::vec3 &co, const glm::vec2 &t,
             const glm::vec3 &n)
      : coord(c), color(co), tex(t), normal(n) {}
};

std::vector<SharedMesh>
VulkanRenderer::LoadObjFile(const std::string &objFile) {
  std::vector<SharedMesh> result{};

  io::obj::Parser parser(objFile);
  auto data = parser.Parse();
  for (const auto &meshData : data) {
    // Skip incomplete faces for now
    if (meshData->faceType !=
        io::obj::FaceDescriptionType::eCoordsTexCoordsNormals)
      continue;

    std::vector<io::obj::Model::Indice> vertices(meshData->vertices.begin(),
                                                 meshData->vertices.end());

    // Build vertices
    std::unordered_map<io::obj::Model::Indice, uint32_t,
                       io::obj::Model::Indice::HashFunction>
        indiceMap{};
    std::vector<VertexData> vertexData{};
    uint32_t index = 0;
    for (const auto &vertex : vertices) {
      const auto &coords = meshData->coordinates.at(
          vertex.coordId - 1); // -1 as Ids start with 1
      const auto &texCoords = meshData->textureCoords.at(vertex.texId - 1);
      const auto &normals = meshData->normals.at(vertex.normalId - 1);
      vertexData.push_back(
          VertexData(coords, glm::vec3{1.0f, 1.0f, 1.0f}, texCoords, normals));
      indiceMap.emplace(vertex, index);
      index++;
    }

    // Build indices list
    std::vector<uint32_t> indiceData{};
    for (const auto &face : meshData->faces)
      for (const auto &indice : face)
        indiceData.push_back(indiceMap[indice]);

    result.push_back(CreateMesh(vertexData, indiceData));
  }
  return result;
}

void VulkanRenderer::SetSceneMaterial(SharedISceneMaterial material) {
  _sceneMaterial = material;
}

void VulkanRenderer::Draw(SharedMesh mesh) {
  mesh->Draw(*_currentRecordBuffer);
}

void VulkanRenderer::Draw(SharedMesh mesh, SharedIMaterial material) {
  material->__getImpl()->WriteAttributes();
  _boundPipeline->GetDescriptorGroup()->Bind(
      *_currentRecordBuffer, _currentFrame->GetPipelineLayout());
  mesh->Draw(*_currentRecordBuffer);
}

void VulkanRenderer::SelectFrame(renderer::SharedFrame frame) {
  _currentFrame = frame;
  _currentRecordBuffer = _currentFrame->GetCommandBuffer();
}
