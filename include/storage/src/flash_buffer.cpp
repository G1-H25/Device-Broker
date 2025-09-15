//!
//! @file flash_buffer.cpp
//! @author Love Lindeborg
//! @brief
//! @version 0.1
//! @date 2025-09-15
//!
//! @copyright Copyright (c) 2025
//!

#include "storage/flash_buffer.h"

namespace Storage {

FlashBuffer::FlashBuffer(uuid_t uuid, uint16_t sensor_index) : Storage(uuid) {
    EEPROM.begin(sizeof(uuid_t) + sizeof(MeasurementEntry) * BUFFER_SIZE_PER_SENSOR);
}

void FlashBuffer::pushMeasurement(const MeasurementEntry *measurement) {
    // EEPROM.put(0, this->uuid_.data());
    // EEPROM.put(this->head * sizeof(MeasurementEntry) + sizeof(uuid_t), *measurement);

    EEPROM.write(this->head * sizeof(MeasurementEntry) + sizeof(uuid_t), measurement->humidity);
    EEPROM.write(this->head * sizeof(MeasurementEntry) + sizeof(uuid_t), measurement->humidity);
    EEPROM.write(this->head * sizeof(MeasurementEntry) + sizeof(uuid_t), measurement->humidity);

    if (this->entry_count_ + 1 < this->buffer_size_) this->entry_count_++;

    this->head = ++this->head % this->buffer_size_;
}

bool FlashBuffer::tryPop() {
    return false;
}

bool FlashBuffer::hasData() {
    return false;
}

uint32_t FlashBuffer::available() {
    return 0;
}

const MeasurementEntry *FlashBuffer::getLatestMeasurement() {
    return nullptr;
}

}  // namespace Storage
