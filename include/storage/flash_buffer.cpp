/**
 * @file flash_buffer.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-15
 *
 * @copyright Copyright (c) 2025
*/

#include "storage/flash_buffer.h"
#include <memory.h>

#include <nvs_flash.h>
#include <esp_flash.h>
#include <cstdio>

namespace storage {

FlashBuffer::FlashBuffer(uuid_t uuid, uint32_t sensor_id) : Storage(uuid), sensor_id_(sensor_id) {
    if (flash_was_init_ == false) {
        if (nvs_flash_init() != ESP_OK) return;

        snprintf(this->storage_name_.data(), this->storage_name_.size(), "%lx", this->sensor_id_);

        esp_err_t err = nvs_open(
            this->storage_name_.begin(),
            nvs_open_mode::NVS_READWRITE,
            &this->nvs_handle_);

        if (err != ESP_OK) return;

        nvs_set_blob(this->nvs_handle_, "uuid", uuid.data(), uuid.size());

        strncpy(
            reinterpret_cast<char *>(this->entry_id_buffer_.data()),
            reinterpret_cast<const char *>(this->storage_name_.data()), this->entry_id_buffer_.size());
    }
}

void FlashBuffer::pushMeasurement(const MeasurementEntry &measurement) {
    ++head_ %= this->buffer_size_;
    if (entry_count_ < this->buffer_size_) entry_count_++;

    std::array<char, k_storage_name_size_ + k_storage_index_str_size_ - 1> temp_str;

    memcpy(
        temp_str.begin(),
        this->storage_name_.begin(),
        this->storage_name_.size());

    snprintf(temp_str.begin() + storage_name_.size() - 1, k_storage_index_str_size_, "_%lx", head_);

    nvs_set_blob(
        this->nvs_handle_,
        reinterpret_cast<const char *>(temp_str.begin()),
        static_cast<const void *>(&measurement),
        sizeof(MeasurementEntry));

    latest_measurement_ = measurement;
}

bool FlashBuffer::tryPop() {
    if (entry_count_ == 0) return false;

    --entry_count_;
    --head_ %= this->buffer_size_;

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
    std::array<char, k_storage_name_size_ + k_storage_index_str_size_ - 1> temp_str;
    size_t length = temp_str.size();

    memcpy(
        temp_str.begin(),
        this->storage_name_.begin(),
        this->storage_name_.size());

    snprintf(temp_str.begin() + storage_name_.size() - 1, k_storage_index_str_size_, "_%x", index + 1);

    esp_err_t res = nvs_get_blob(
        this->nvs_handle_,
        reinterpret_cast<const char *>(temp_str.begin()),
        static_cast<void *>(&this->buffered_measurement_),
        &length);

    return res == ESP_OK ? &this->buffered_measurement_ : nullptr;
}

}  // namespace storage
