/**
 * @file model.hpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief [TODO] Generalize the model inside of the engine.
 * @date 2023-03-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __IO_OBJ_MODEL_HPP__
#define __IO_OBJ_MODEL_HPP__

// GLM
#include <glm/glm.hpp>

// STL
#include <array>
#include <string>
#include <unordered_set>
#include <vector>

namespace io::obj {

/**
 * @brief Describes available data for a vertex.
 */
enum class FaceDescriptionType {
  eCoords,
  eCoordsTexCoords,
  eCoordsNormals,
  eCoordsTexCoordsNormals
};

/**
 * @brief Contains all data that could be parsed be the obj parser.
 */
struct Model {
  /**
   * @brief Contains all data of an indice for faces.
   */
  struct Indice {
    /**
     * @brief Which coordinate this indice uses.
     */
    uint32_t coordId;

    /**
     * @brief Which texture coordinate this indice uses.
     */
    uint32_t texId;

    /**
     * @brief Which normal this indice uses.
     */
    uint32_t normalId;

    /**
     * @brief Construct an Indice with the
     *
     * @param coord   Coordinate Id of this indice.
     * @param tex     Texture Coordinate Id of this indice.
     * @param normal  Normal Id of this indice.
     */
    Indice(uint32_t coord, uint32_t tex, uint32_t normal)
        : coordId(coord), texId(tex), normalId(normal) {}

    /**
     * @brief Compare operator for the indice.
     *
     * @param i       Indice to compare to.
     * @return true   Indices are equal.
     * @return false  Indices are not equal.
     */
    bool operator==(const Indice &i) const {
      return coordId == i.coordId && texId == i.texId && normalId == i.normalId;
    }

    /**
     * @brief Enables hashing of the Indices
     */
    struct HashFunction {
      /**
       * @brief Hash operator. Returns the hash for the given indice.
       *
       * @param i       Indice to hash.
       * @return size_t The hash.
       */
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

  /**
   * @brief Primitive is the equivalent of a face.
   */
  using Primitive = std::vector<Indice>;

  /**
   * @brief Name of the mesh.
   */
  std::string name;

  /**
   * @brief All coordinates of the model.
   */
  std::vector<glm::vec3> coordinates;

  /**
   * @brief All the texture coordinates of the model.
   */
  std::vector<glm::vec2> textureCoords;

  /**
   * @brief All the normals of the model.
   */
  std::vector<glm::vec3> normals;

  /**
   * @brief Describes which data is available for every face.
   */
  FaceDescriptionType faceType;

  /**
   * @brief All of the faces the model contains.
   */
  std::vector<Primitive> faces;

  /**
   * @brief All unique vertices in the model.
   */
  std::unordered_set<Indice, Indice::HashFunction> vertices;
};

} // namespace io::obj

#endif /* __IO_OBJ_MODEL_HPP__ */