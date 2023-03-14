//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages) : capacity_(num_pages) {}

LRUReplacer::~LRUReplacer() = default;

auto LRUReplacer::Victim(frame_id_t *frame_id) -> bool {
  lru_mutex_.lock();
  if (cache_.empty()) {
    lru_mutex_.unlock();
    return false;
  }
  auto r = cache_.back();
  map_.erase(r);
  cache_.pop_back();
  *frame_id = r;
  lru_mutex_.unlock();

  return true;
}

void LRUReplacer::Pin(frame_id_t frame_id) {
  lru_mutex_.lock();
  if (map_.find(frame_id) != map_.end()) {
    cache_.erase(map_[frame_id]);
    map_.erase(frame_id);
  }

  lru_mutex_.unlock();
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
  lru_mutex_.lock();
  if (map_.find(frame_id) != map_.end()) {
    lru_mutex_.unlock();
    return;
  }

  while (map_.size() >= capacity_) {
    map_.erase(cache_.back());
    cache_.pop_back();
  }

  cache_.push_front(frame_id);
  map_[frame_id] = cache_.begin();
  lru_mutex_.unlock();
}

auto LRUReplacer::Size() -> size_t { return cache_.size(); }

}  // namespace bustub
