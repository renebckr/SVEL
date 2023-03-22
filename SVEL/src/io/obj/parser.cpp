#include "parser.h"
#include "io/obj/model.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace io::obj;

std::vector<std::string> Parser::_splitBy(char delimiter,
                                          const std::string &str) {
  std::vector<std::string> substrings{};
  size_t start = 0, end = 0;
  while ((end = str.find_first_of(delimiter, start)) != std::string::npos) {
    substrings.push_back(str.substr(start, end - start));
    start = end + 1; // +1 to ignore whitespace
  }
  substrings.push_back(str.substr(start));
  return substrings;
}

void Parser::_handleNode(const std::string &str, std::shared_ptr<Model> model) {
  auto substrings = _splitBy(' ', str);

  // Ignore non 3-sized nodes
  if (substrings.size() > 4)
    return;

  // Get data
  glm::vec3 data;
  std::stringstream ss;
  ss.imbue(std::locale("C"));
  ss << substrings.at(1);
  ss >> data.x;
  ss.clear();
  ss << substrings.at(2);
  ss >> data.y;
  ss.clear();
  if (substrings.size() > 3) {
    ss << substrings.at(3);
    ss >> data.z;
  }

  // Push to correct array
  switch (substrings.at(0).back()) {
  case 'v':
    model->coordinates.emplace_back(data);
    break;
  case 't':
    model->textureCoords.emplace_back(data.x, data.y);
    break;
  case 'n':
    model->normals.emplace_back(data);
    break;
  default:
    break;
  }
}

void Parser::_handleFace(const std::string &str, std::shared_ptr<Model> model) {
  auto substrings = _splitBy(' ', str);
  substrings.erase(substrings.begin());

  if (substrings.size() != 3)
    return; // Ignore non tris for now

  Model::Primitive p;
  for (const auto &indices : substrings) {
    std::array<size_t, 3> data{0, 0, 0};
    std::stringstream ss;
    auto values = _splitBy('/', indices);
    for (unsigned int i = 0; i < 3; i++) {
      if (values.at(i).empty())
        continue;
      ss << values.at(i);
      ss >> data[i];
      ss.clear();
    }

    auto indice = Model::Indices(data[0], data[1], data[2]);
    p.push_back(indice);
    model->vertices.insert(indice);
  }
  model->faces.push_back(p);
}

bool Parser::_handleMeshFinalization(std::shared_ptr<Model> model) {
  // Set primitive type
  if (model->faces.empty())
    return false;

  auto indice = model->faces.front().front();

  // Invalid faces if vertices have invalid ids
  bool hasVertices = std::get<0>(indice) != 0;
  if (!hasVertices)
    return false;

  // Set face description type
  bool hasTexCoords = std::get<1>(indice) != 0;
  bool hasNormals = std::get<2>(indice) != 0;
  if (hasTexCoords && hasNormals)
    model->faceType = FaceDescriptionType::eCoordsTexCoordsNormals;
  else if (!hasTexCoords && hasNormals)
    model->faceType = FaceDescriptionType::eCoordsNormals;
  else if (hasTexCoords && !hasNormals)
    model->faceType = FaceDescriptionType::eCoordsTexCoords;
  else
    model->faceType = FaceDescriptionType::eCoords;
  return true;
}

Parser::Parser(const std::filesystem::path &file) : _file(file) {
  // Check if file is valid
  if (!std::filesystem::exists(_file) ||
      !std::filesystem::is_regular_file(_file))
    throw std::invalid_argument("File invalid for obj loader");
}

std::vector<std::shared_ptr<Model>> Parser::Parse() {
  std::vector<std::shared_ptr<Model>> result;

  // Open file
  std::ifstream in(_file);

  if (!in.is_open())
    throw std::runtime_error("Could not open file.");

  std::shared_ptr<Model> model = nullptr;
  while (!in.eof()) {
    // Get line
    std::string line;
    std::getline(in, line);

    if (line.size() < 2) // make sure there are at least two characters
      continue;

    // Check starting character
    char firstCharacter = line.front();
    if (model == nullptr && firstCharacter != 'o')
      continue; // Don't handle anything until an object

    switch (firstCharacter) {
    case 'o': // Need to create new object
      if (model != nullptr) {
        if (_handleMeshFinalization(model))
          result.push_back(model);
      }
      model = std::make_shared<Model>();
      break;
    case 'v':
      _handleNode(line, model);
      break;
    case 'f':
      _handleFace(line, model);
      break;
    default:
      break; // Ignore anything else
    }
  }

  if (_handleMeshFinalization(model))
    result.push_back(model);
  return result;
}