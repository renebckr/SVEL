#ifndef __IO_OBJ_MODEL_HPP__
#define __IO_OBJ_MODEL_HPP__

#include <array>
#include <glm/glm.hpp>
#include <string>
#include <unordered_set>
#include <vector>

#include <io/util/tuple_hash.hpp>

namespace io::obj {

enum class FaceDescriptionType {
  eCoords,
  eCoordsTexCoords,
  eCoordsNormals,
  eCoordsTexCoordsNormals
};

struct Model {
  using Indices = std::tuple<uint32_t, uint32_t, uint32_t>;
  using Primitive = std::vector<Indices>;

  std::string name;
  std::vector<glm::vec3> coordinates;
  std::vector<glm::vec2> textureCoords;
  std::vector<glm::vec3> normals;

  FaceDescriptionType faceType;
  std::vector<Primitive> faces;
  std::unordered_set<Indices> vertices;
};

} // namespace io::obj

#endif /* __IO_OBJ_MODEL_HPP__ */