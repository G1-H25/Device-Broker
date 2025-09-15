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
#include <nvs.h>
#include <nvs_flash.h>
#include <memory.h>

namespace storage {

FlashBuffer::FlashBuffer(uuid_t uuid) : Storage(uuid) {
    if (!flash_was_init_) {
        esp_flash_init(&this->flash_chip_);
    }

    static uint32_t storage_offset_;

    region_ = {
        storage_offset_,  // offset, need a better way to determine offset
        k_flash_size  // Allocated size sensor
    };

    storage_offset_ += k_flash_size;
}

void FlashBuffer::pushMeasurement(const MeasurementEntry *measurement) {
    MeasurementEntry temp_entry{ *measurement };
    esp_flash_write(
        &this->flash_chip_,
        reinterpret_cast<void *>(&temp_entry),
        region_.offset + sizeof(MeasurementEntry) * head + sizeof(uuid_t),
        sizeof(MeasurementEntry));

    if (this->entry_count_ + 1 < this->buffer_size_) this->entry_count_++;

    this->head = ++this->head % this->buffer_size_;
}

bool FlashBuffer::tryPop() {
    if (this->entry_count_ == 0) return false;

    esp_flash_write(
        &this->flash_chip_, (void *) { 0 },
        getCurrentEntryPositionOnFlash(),
        sizeof(MeasurementEntry));

    this->head -= head % this->buffer_size_;
    this->entry_count_--;

    return true;
}

bool FlashBuffer::hasData() {
    return this->entry_count_ > 0;
}

uint32_t FlashBuffer::available() {
    return this->entry_count_;
}

/**
 * @brief Gets the latest entry from the buffer
 *
 * @return const MeasurementEntry*. It is allocated on the heap. Free when done.
 */
const MeasurementEntry *FlashBuffer::getLatestMeasurement() {
    MeasurementEntry *entry = reinterpret_cast<MeasurementEntry *>(malloc(sizeof(MeasurementEntry)));
    esp_flash_read(
        &this->flash_chip_,
        reinterpret_cast<void *>(entry),
        getCurrentEntryPositionOnFlash(),
        sizeof(MeasurementEntry));

    return entry;
}


const MeasurementEntry FlashBuffer::loadFromMemory(size_t measurement_index) {
    MeasurementEntry entry;
    esp_flash_read(
        &this->flash_chip_,
        reinterpret_cast<void *>(&entry),
        region_.offset + sizeof(MeasurementEntry) * measurement_index + sizeof(uuid_t),
        sizeof(MeasurementEntry));

    return entry;
}

const uint32_t FlashBuffer::getCurrentEntryPositionOnFlash() {
    return region_.offset + sizeof(MeasurementEntry) * head + sizeof(uuid_t);
}


}  // namespace storage
