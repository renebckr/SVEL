#ifndef __IO_OBJ_MODEL_HPP__
#define __IO_OBJ_MODEL_HPP__

#include <array>
#include <glm/glm.hpp>
#include <string>
#include <unordered_set>
#include <vector>

namespace io::obj {

enum class FaceDescriptionType {
  eCoords,
  eCoordsTexCoords,
  eCoordsNormals,
  eCoordsTexCoordsNormals
};

struct Model {
  struct Indice {
    uint32_t coordId;
    uint32_t texId;
    uint32_t normalId;

    Indice(uint32_t a, uint32_t b, uint32_t c)
        : coordId(a), texId(b), normalId(c) {}

    bool operator==(const Indice &i) const {
      return coordId == i.coordId && texId == i.texId && normalId == i.normalId;
    }

    struct HashFunction {
      size_t operator()(const Indice &i) const {
        size_t seed = 0;
        seed ^= std::hash<uint32_t>()(i.coordId) + 0x9e3779b9 + (seed << 6) +
                (seed >> 2);
        seed ^= std::hash<uint32_t>()(i.texId) + 0x9e3779b9 + (seed << 6) +
                (seed >> 2);
        seed ^= std::hash<uint32_t>()(i.normalId) + 0x9e3779b9 + (seed << 6) +
                (seed >> 2);
        return seed;
      }
    };
  };

  using Primitive = std::vector<Indice>;

  std::string name;
  std::vector<glm::vec3> coordinates;
  std::vector<glm::vec2> textureCoords;
  std::vector<glm::vec3> normals;

  FaceDescriptionType faceType;
  std::vector<Primitive> faces;
  std::unordered_set<Indice, Indice::HashFunction> vertices;
};

} // namespace io::obj

#endif /* __IO_OBJ_MODEL_HPP__ */