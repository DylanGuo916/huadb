#pragma once

#include "storage/buffer_strategy.h"
#include <list>
#include <unordered_map>


namespace huadb {

class LRUBufferStrategy : public BufferStrategy {
 public:
  void Access(size_t frame_no) override;
  size_t Evict() override;

  private:
  std::list<size_t> access_order_;
  std::unordered_map<size_t, std::list<size_t>::iterator> page_map_;
};

}  // namespace huadb
