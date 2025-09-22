/**
 * @file flash_buffer.h
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-15
 *
 * @copyright Copyright (c) 2025
*/

#ifndef INCLUDE_STORAGE_FLASH_BUFFER_H_
#define INCLUDE_STORAGE_FLASH_BUFFER_H_

#include <nvs_flash.h>
#include <esp_flash.h>
#include <memory>
#include <mutex>

#include "storage/storage.h"

#define BASE64_UUID_STR_SIZE 25

namespace storage {

// Byte 23 and 24 are reserved for entry id. Little-endian.
typedef std::array<uint8_t, BASE64_UUID_STR_SIZE + 2> entry_id_buffer_t;

class FlashBuffer : public Storage {
 public:
    explicit FlashBuffer(uuid_t uuid, uint32_t sensor_id);

    void pushMeasurement(const MeasurementEntry &measurement) override;
    bool tryPop() override;

    const MeasurementEntry *getLatestMeasurement() override;
    MeasurementEntry *loadMeasurement(size_t index);

 private:
    void updateEntryIdBuffer(entry_id_buffer_t *buffer, size_t index);

    MeasurementEntry latest_measurement_;
    MeasurementEntry buffered_measurement_;

    entry_id_buffer_t entry_id_buffer_ = { 0 };
    uint32_t sensor_id_;
    static constexpr uint16_t k_storage_name_size_ = 5;
    static constexpr uint16_t k_storage_index_str_size_ = 5;
    std::array<char, k_storage_name_size_> storage_name_;

    const size_t k_flash_size = sizeof(uuid_t) + sizeof(MeasurementEntry) * BUFFER_SIZE_PER_SENSOR;
    static bool flash_was_init_;

    esp_flash_t flash_chip_;
    esp_partition_t partition_;

    static nvs_handle_t nvs_handle_;
    static std::mutex flash_mtx_;
};

}  // namespace storage

#endif  // INCLUDE_STORAGE_FLASH_BUFFER_H_
