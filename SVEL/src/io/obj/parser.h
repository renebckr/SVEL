/**
 * @file parser.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief OBJ-Parser declaration.
 * @date 2023-03-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __IO_OBJ_PARSER_H__
#define __IO_OBJ_PARSER_H__

// Local
#include "model.hpp"

// STL
#include <filesystem>

namespace io::obj {

/**
 * @brief Allows processing of an obj file. Will convert the models contained in
 * the obj file to an io::obj::Model. Currently only supports:
 *  - Triangulated meshes
 *  - TexCoords and normals
 */
class Parser {
private:
  /**
   * @brief Path to the obj file.
   */
  std::filesystem::path _file;

  /**
   * @brief Splits a string into substrings at the delimiter. Will not include
   * the delimiter in the substrings.
   *
   * @param delimiter                 Delimiter at which to split.
   * @param str                       String to split.
   * @return std::vector<std::string> Resulted substrings.
   */
  std::vector<std::string> _splitBy(char delimiter, const std::string &str);

  /**
   * @brief Handles any obj file line that starts with 'v' for vertex.
   *
   * @param str   Line that starts with 'v' and has size at least 2.
   * @param model Model which to update.
   */
  void _handleNode(const std::string &str, std::shared_ptr<Model> model);

  /**
   * @brief Handles any obj file line that begins with 'f' for face.
   *
   * @param str   Line that starts with 'f'.
   * @param model Model which to update.
   */
  void _handleFace(const std::string &str, std::shared_ptr<Model> model);

  /**
   * @brief Handles a model that should be finalized.
   *
   * @param model   Model to be finalized.
   * @return true   Model could be finalized properly.
   * @return false  Model invalid.
   */
  bool _handleModelFinalization(std::shared_ptr<Model> model);

public:
  /**
   * @brief Construct a Parser for a an obj-file.
   *
   * @param file OBJ-File that the parser should process.
   */
  Parser(const std::filesystem::path &file);

  /**
   * @brief Parses the file and returns the models that the file contains.
   *
   * @return std::vector<std::shared_ptr<Model>> Models defined in the OBJ-File.
   */
  std::vector<std::shared_ptr<Model>> Parse();
};

} // namespace io::obj

#endif /* __IO_OBJ_PARSER_H__ */