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
