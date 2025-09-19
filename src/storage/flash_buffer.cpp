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
#include <memory.h>

#include <nvs_flash.h>
#include <esp_flash.h>
#include <mbedtls/base64.h>

namespace storage {

FlashBuffer::FlashBuffer(uuid_t uuid) : Storage(uuid) {
    if (flash_was_init_ == false) {
        if (nvs_flash_init() != ESP_OK) return;

        size_t allocatedSize;
        unsigned char base64_uuid[BASE64_UUID_STR_SIZE] = { 0 };
        mbedtls_base64_encode(base64_uuid, BASE64_UUID_STR_SIZE - 1, &allocatedSize, uuid.data(), uuid.size());

        this->base64_uuid_ = reinterpret_cast<const char*>(base64_uuid);

        esp_err_t err = nvs_open(
            base64_uuid_.data(),
            nvs_open_mode::NVS_READWRITE,
            &this->nvs_handle_);

        if (err != ESP_OK) return;

        nvs_set_blob(this->nvs_handle_, "uuid", uuid.data(), uuid.size());

        strncpy(
            reinterpret_cast<char *>(this->entry_id_buffer_.data()),
            reinterpret_cast<const char *>(base64_uuid), this->entry_id_buffer_.size());
    }
}

void FlashBuffer::pushMeasurement(const MeasurementEntry &measurement) {
    ++head_ %= this->buffer_size_;
    updateEntryIdBuffer(&this->entry_id_buffer_, head_);

    if (entry_count_ < this->buffer_size_) entry_count_++;

    nvs_set_blob(
        this->nvs_handle_,
        reinterpret_cast<const char *>(this->entry_id_buffer_.data()),
        static_cast<const void *>(&measurement),
        sizeof(MeasurementEntry));

    latest_measurement_ = measurement;
}

bool FlashBuffer::tryPop() {
    if (entry_count_ == 0) return false;

    --entry_count_;
    --head_ %= this->buffer_size_;
    updateEntryIdBuffer(&this->entry_id_buffer_, head_);

    MeasurementEntry empty{ 0, 0, 0 };

    return nvs_set_blob(
        this->nvs_handle_,
        reinterpret_cast<const char *>(this->entry_id_buffer_.data()),
        static_cast<const void *>(&empty),
        sizeof(MeasurementEntry)) == ESP_OK;
}

const MeasurementEntry *FlashBuffer::getLatestMeasurement() {
    if (entry_count_ == 0) return nullptr;
    return &this->latest_measurement_;
}

MeasurementEntry *FlashBuffer::loadMeasurement(size_t index) {
    entry_id_buffer_t temp_id_buffer = { 0 };
    size_t length = temp_id_buffer.size();

    memcpy(
        reinterpret_cast<char *>(this->entry_id_buffer_.data()),
        reinterpret_cast<const char *>(temp_id_buffer.data()),
        this->entry_id_buffer_.size());

    updateEntryIdBuffer(&temp_id_buffer, index);

    int res = strncmp(
        reinterpret_cast<char *>(this->entry_id_buffer_.data()),
        reinterpret_cast<const char *>(temp_id_buffer.data()),
        this->entry_id_buffer_.size());

    if (res == 0) return nullptr;

    nvs_get_blob(
        this->nvs_handle_,
        reinterpret_cast<const char *>(temp_id_buffer.data()),
        static_cast<void *>(&this->buffered_measurement_),
        &length);
    return &this->buffered_measurement_;
}


void FlashBuffer::updateEntryIdBuffer(entry_id_buffer_t *buffer, size_t index) {
    (*buffer)[23] = (uint8_t)(index & 0xFF);  // Lower byte
    (*buffer)[24] = (uint8_t)((index >> 8) & 0xFF);  // Upper byte
}


}  // namespace storage
