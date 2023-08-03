/**
 * @file queue.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declaration of the SetQueue.
 * @date 2023-03-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __CORE_DESCRIPTOR_QUEUE_H__
#define __CORE_DESCRIPTOR_QUEUE_H__

// Local
#include "set.h"

// Internal
#include <svel/config.h>

// STL
#include <queue>
#include <vector>

namespace core::descriptor {

/**
 * @brief Circular queue for descriptor sets. Best described as a round desk
 * around which people sit. The queue never runs out of elements as there is
 * always another one.
 */
class SetQueue {
private:
  /**
   * @brief Sets to use for the queue. This implicitly handles the order.
   */
  std::queue<SharedSet> sets;

public:
  /**
   * @brief Creates the SetQueue.
   *
   * @param _queueData Sets to use.
   */
  SetQueue(std::vector<SharedSet> &_queueData);

  /**
   * @brief These should not be copied around to make sure no sets are used when
   * they should'nt.
   */
  SetQueue(const SetQueue &) = delete;

  /**
   * @brief Get the next set out of the queue. Will put that set at the end of
   * the queue afterwards.
   *
   * @return SharedSet The next set.
   */
  SharedSet Next();
};
SVEL_CLASS(SetQueue);

} // namespace core::descriptor

#endif /* __CORE_DESCRIPTOR_QUEUE_H__ */