/**
 * @file memory_buffer.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-15
 *
 * @copyright Copyright (c) 2025
*/

#include "storage/memory_buffer.h"

namespace storage {

/**
 * @brief Construct a new Memory Buffer object
 *
 * Stores values in a ring-buffer
 *
 * @param uuid UUID to use for the sensor
 */
MemoryBuffer::MemoryBuffer(uuid_t uuid) : Storage(uuid) {
}

/**
 * @brief Push a measurement to the buffer
 *
 * @param measurement The measurement to be stored
 */
void MemoryBuffer::pushMeasurement(const MeasurementEntry &measurement) {
    this->entries_[this->head_] = measurement;

    if (this->entry_count_ + 1 < this->buffer_size_) this->entry_count_++;

    ++this->head_ %= this->buffer_size_;
}

/**
 * @brief Pop a value from memory.
 *
 * @returns True if success, false otherwise
 */
bool MemoryBuffer::tryPop() {
    if (this->entry_count_ == 0) return false;

    this->entry_count_--;
    ++this->head_ %= this->buffer_size_;
    return true;
}

/**
 * @brief Get the latest measurement from memory

 * @returns `MeasurementEntry *` or a `nullptr` failed
 */
const MeasurementEntry *MemoryBuffer::getLatestMeasurement() {
    if (this->entry_count_ == 0) return nullptr;

    return &this->entries_[head_ ? head_ - 1 : head_];
}

void MemoryBuffer::clearAll() {
    this->entries_.fill({0, 0, 0});
    this->entry_count_ = 0;
    this->head_ = 0;
}

}  // namespace storage
