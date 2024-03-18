#include "storage/lru_buffer_strategy.h"
#include "common/constants.h"

namespace huadb {

void LRUBufferStrategy::Access(size_t frame_no) {
  // 缓存页面访问
  // LAB 1 BEGIN
  
  // 如果页面已经在缓存中，则移除其当前位置
  if (page_map_.find(frame_no) != page_map_.end()) {
    access_order_.erase(page_map_[frame_no]);
  } else if (access_order_.size() >= BUFFER_SIZE) {
    // 如果达到缓存容量上限且页面不在缓存中，则需要先淘汰一个页面
    Evict();
  }

  // 将页面添加到访问顺序的前面
  access_order_.push_front(frame_no);
  // 更新或添加页面编号对应的位置
  page_map_[frame_no] = access_order_.begin();
  return;
};

size_t LRUBufferStrategy::Evict() {
  // 缓存页面淘汰，返回淘汰的页面在 buffer pool 中的下标
  // LAB 1 BEGIN

  auto evict_frame_no = access_order_.back();
  page_map_.erase(evict_frame_no);
  access_order_.pop_back();
  return evict_frame_no;
}

}  // namespace huadb
