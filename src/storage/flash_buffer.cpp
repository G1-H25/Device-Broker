/**
 * @file flash_buffer.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-15
 *
 * @copyright Copyright (c) 2025
*/

#ifdef ESP_PLATFORM

#include "storage/flash_buffer.h"
#include <memory.h>

#include <nvs_flash.h>
#include <esp_flash.h>
#include <cstdio>

namespace storage {

static nvs_handle_t registered_sensors_handle;
bool FlashBuffer::flash_was_init_ = false;
std::mutex FlashBuffer::flash_mtx_{};

/**
 * @brief Construct a new Flash Buffer object
 *
 * @param uuid - The unique identifier for the sensor.
 * @param sensor_id - The sensor id is used to store values in the nvs.
 */
FlashBuffer::FlashBuffer(uuid_t uuid, uint32_t sensor_id) : Storage(uuid), sensor_id_(sensor_id) {
    std::unique_lock<std::mutex> lock(FlashBuffer::flash_mtx_);
    if (flash_was_init_ == false) {
        if (nvs_flash_init() != ESP_OK) return;

        nvs_open("registered_sensors", nvs_open_mode_t::NVS_READWRITE, &registered_sensors_handle);
    }

    snprintf(this->storage_name_.data(), this->storage_name_.size(), "%lx", this->sensor_id_);

    esp_err_t err = nvs_open(
        this->storage_name_.begin(),
        nvs_open_mode::NVS_READWRITE,
        &this->nvs_handle_);

    if (err != ESP_OK || !this->nvs_handle_) return;
    flash_was_init_ = true;


    nvs_iterator_t iterator;
    esp_err_t res = nvs_entry_find_in_handle(registered_sensors_handle, nvs_type_t::NVS_TYPE_BLOB, &iterator);

    // uuid_t temp_uuid;
    // size_t uuid_size = temp_uuid.size();

    // nvs_entry_info_t info;
    // while (res == ESP_OK) {
    //     nvs_entry_info(iterator, &info);

    //     nvs_get_blob(
    //         this->nvs_handle_,
    //         "uuid",
    //         temp_uuid.begin(),
    //         &uuid_size);

    //     int res = timingsafe_memcmp(uuid.begin(), temp_uuid.data(), uuid_size);

    //     if (res == 0) break;
    //     res = nvs_entry_next(&iterator);
    // }

    nvs_release_iterator(iterator);

    nvs_set_blob(registered_sensors_handle, getKeyFromIndex(this->sensor_id_).data(), uuid.data(), uuid.size());
    nvs_commit(registered_sensors_handle);
}

/**
 * @brief Push and store a new measurement entry into flash.
 *
 * @param measurement - The measurement entry to be stored in the buffer.
 */
bool FlashBuffer::pushMeasurement(const MeasurementEntry &measurement) {
    if (!this->flash_was_init_) return false;

    head_++;
    head_ %= this->buffer_size_;
    if (entry_count_ < this->buffer_size_) entry_count_++;

    nvs_key_t temp_str;

    temp_str = getKeyFromIndex(head_);

    nvs_set_blob(
        this->nvs_handle_,
        reinterpret_cast<const char *>(temp_str.begin()),
        static_cast<const void *>(&measurement),
        sizeof(MeasurementEntry));

    std::unique_lock<std::mutex> lock(FlashBuffer::flash_mtx_);
    nvs_commit(this->nvs_handle_);
    latest_measurement_ = measurement;

    return true;
}

/**
 * @brief Pop a value from flash.
 *
 * @returns True if success, false otherwise
 */
bool FlashBuffer::tryPop(MeasurementEntry &out) {
    if (!this->flash_was_init_ || entry_count_ == 0) return false;

    --entry_count_;
    --head_ %= this->buffer_size_;

    esp_err_t err = nvs_erase_key(
        this->nvs_handle_,
        reinterpret_cast<const char *>(getKeyFromIndex(head_ + 1).data()));

    std::unique_lock<std::mutex> lock(FlashBuffer::flash_mtx_);
    nvs_commit(this->nvs_handle_);
    return err == ESP_OK;
}

bool FlashBuffer::getLatestMeasurement(MeasurementEntry &out) {
    if (!this->flash_was_init_ || entry_count_ == 0) return false;
    return (out = this->latest_measurement_, true);
}

/**
 * @brief Load a measurement from the flash memory
 *
 * @param index The index of the measurement which will be retrieved
 * @returns `MeasurementEntry *` or a `nullptr` if failed
 */
bool FlashBuffer::loadMeasurement(size_t index, MeasurementEntry &out) {
    if (!this->flash_was_init_ || index >= this->available()) return false;

    nvs_key_t temp_str;
    size_t length = temp_str.size();

    temp_str = getKeyFromIndex(index + 1);

    esp_err_t res = nvs_get_blob(
        this->nvs_handle_,
        reinterpret_cast<const char *>(temp_str.begin()),
        reinterpret_cast<void *>(&this->buffered_measurement_),
        &length);

    out = this->buffered_measurement_;
    return res == ESP_OK;
}

constexpr nvs_key_t FlashBuffer::getKeyFromIndex(size_t index) {
    nvs_key_t temp_str{ 0 };
    snprintf(temp_str.begin(), temp_str.size(), "%x", index);
    return temp_str;
}

void FlashBuffer::clearAll() {
    this->entry_count_ = 0;
    this->head_ = 0;

    nvs_erase_all(this->nvs_handle_);

    std::unique_lock<std::mutex> lock(FlashBuffer::flash_mtx_);
    nvs_commit(this->nvs_handle_);
}

}  // namespace storage

#endif  // ESP_PLATFORM
