//!
//! @file sensor.cpp
//! @author Love Lindeborg
//! @brief
//! @version 0.1
//! @date 2025-09-11
//!
//! @copyright Copyright (c) 2025
//!

#include "sensor/sensor.h"
#include <algorithm>
#include <memory>

namespace sensor {

SensorBuffer &SensorBuffer::getInstance() {
    static SensorBuffer instance;
    return instance;
}

SensorBuffer::SensorBuffer() { }

SensorBuffer::~SensorBuffer() { }

void SensorBuffer::pushMeasurement(const MeasurementEntry &measurement) {
    this->entries_[this->head] = measurement;

    if (this->entry_count_ + 1 < this->buffer_size_) this->entry_count_++;

    this->head = ++this->head % this->buffer_size_;
}

bool SensorBuffer::tryPop() {
    if (entry_count_ == 0) return false;
}

bool SensorBuffer::hasData() {
    return this->entry_count_ > 0;
}

uint32_t SensorBuffer::available() {
    return this->entry_count_;
}

const MeasurementEntry *SensorBuffer::getLatestMeasurement() {
    if (this->entry_count_ == 0) return nullptr;

    return &this->entries_[head ? head - 1 : head];
}

}  // namespace sensor
