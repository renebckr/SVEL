/**
 * @file pipeline.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declaration of the pipeline.
 * @date 2023-03-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __SVEL_DETAIL_PIPELINE_H__
#define __SVEL_DETAIL_PIPELINE_H__

// SVEL
#include <svel/config.h>
#include <svel/detail/shader.h>

// STL
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

/**
 * @brief Interface of a graphics pipeline.
 */
class Pipeline {
public:
  /**
   * @brief Destroy the Pipeline.
   *
   */
  virtual ~Pipeline() {}

  /**
   * @brief Users have to notify the pipeline that a new frame is being
   * processed (Call this at the start of the draw).
   */
  virtual void NotifyNewFrame() = 0;
};
SVEL_CLASS(Pipeline)

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_DETAIL_PIPELINE_H__ */