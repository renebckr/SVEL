#ifndef __IO_OBJ_PARSER_H__
#define __IO_OBJ_PARSER_H__

#include "io/obj/model.hpp"
#include <filesystem>

namespace io::obj {

class Parser {
private:
  std::filesystem::path _file;

  std::vector<std::string> _splitBy(char delimiter, const std::string &str);
  void _handleNode(const std::string &str, std::shared_ptr<Model> model);
  void _handleFace(const std::string &str, std::shared_ptr<Model> model);
  bool _handleMeshFinalization(std::shared_ptr<Model> model);

public:
  Parser(const std::filesystem::path &file);

  std::vector<std::shared_ptr<Model>> Parse();
};

} // namespace io::obj

#endif /* __IO_OBJ_PARSER_H__ */