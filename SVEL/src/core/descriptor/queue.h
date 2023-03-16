#ifndef __CORE_DESCRIPTOR_QUEUE_H__
#define __CORE_DESCRIPTOR_QUEUE_H__

#include "set.h"
#include "svel/config.h"

#include <queue>
#include <vector>

namespace core::descriptor {

class SetQueue {
private:
  std::queue<SharedSet> sets;

public:
  SetQueue(std::vector<SharedSet> &_queueData);
  SetQueue(const SetQueue &) = delete;

  SharedSet Next();
};
SVEL_CLASS(SetQueue);

} // namespace core::descriptor

#endif /* __CORE_DESCRIPTOR_QUEUE_H__ */