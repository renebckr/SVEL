#ifndef __SVEL_DETAIL_PIPELINE_H__
#define __SVEL_DETAIL_PIPELINE_H__

#include <svel/config.h>
#include <svel/detail/shader.h>

#include <memory>
#include <vector>

namespace SVEL_NAMESPACE {

/**
 * @brief Attribute Type for vertex descriptions.
 */
enum class AttributeType {
  SIGNED_FLOAT,
};

/**
 * @brief Description of one vertex.
 */
typedef std::vector<std::pair<AttributeType, unsigned int>> VertexDescription;

class Pipeline {
public:
  virtual ~Pipeline() {}
};
SVEL_CLASS(Pipeline)

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_DETAIL_PIPELINE_H__ */