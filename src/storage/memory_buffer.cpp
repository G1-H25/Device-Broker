//!
//! @file memory_buffer.cpp
//! @author Love Lindeborg
//! @brief
//! @version 0.1
//! @date 2025-09-15
//!
//! @copyright Copyright (c) 2025
//!


#include "storage/memory_buffer.h"

namespace storage {

MemoryBuffer::MemoryBuffer(uuid_t uuid) : Storage(uuid) {
}

void MemoryBuffer::pushMeasurement(const MeasurementEntry &measurement) {
    this->entries_[this->head_] = measurement;

    if (this->entry_count_ + 1 < this->buffer_size_) this->entry_count_++;

    ++this->head_ %= this->buffer_size_;
}

bool MemoryBuffer::tryPop() {
    if (this->entry_count_ == 0) return false;

    this->entry_count_--;
    ++this->head_ &= this->buffer_size_;
    return true;
}

bool MemoryBuffer::hasData() {
    return this->entry_count_ > 0;
}

uint32_t MemoryBuffer::available() {
    return this->entry_count_;
}

const MeasurementEntry *MemoryBuffer::getLatestMeasurement() {
    if (this->entry_count_ == 0) return nullptr;

    return &this->entries_[head_ ? head_ - 1 : head_];
}

}  // namespace storage
