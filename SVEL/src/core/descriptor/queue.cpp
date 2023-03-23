/**
 * @file queue.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the SetQueue.
 * @date 2023-03-23
 *
 * @copyright Copyright (c) 2023
 *
 */

// Local
#include "queue.h"

using namespace core::descriptor;

SetQueue::SetQueue(std::vector<SharedSet> &_queueData) {
  for (auto set : _queueData)
    sets.push(set);
}

SharedSet SetQueue::Next() {
  auto set = sets.front();
  sets.pop();
  set->Reset();
  sets.push(set);
  return set;
}
