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

FlashBuffer::FlashBuffer(uuid_t uuid) : Storage(uuid) {
}

void FlashBuffer::pushMeasurement(const MeasurementEntry *measurement) {
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
