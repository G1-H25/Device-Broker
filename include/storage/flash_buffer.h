//!
//! @file eeprom_buffer.h
//! @author Love Lindeborg
//! @brief
//! @version 0.1
//! @date 2025-09-15
//!
//! @copyright Copyright (c) 2025
//!

#ifndef INCLUDE_STORAGE_FLASH_BUFFER_H_
#define INCLUDE_STORAGE_FLASH_BUFFER_H_

#include <esp_flash.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <memory>
#include "storage/storage.h"

namespace storage {

class FlashBuffer : public Storage {
 public:
    explicit FlashBuffer(uuid_t uuid);

    void pushMeasurement(const MeasurementEntry *measurement) override;
    bool tryPop() override;

    bool hasData() override;
    uint32_t available() override;

    const uuid_t getUUID();
    const MeasurementEntry *getLatestMeasurement() override;

 private:
    const uint32_t getCurrentEntryPositionOnFlash();
    const MeasurementEntry loadFromMemory(size_t measurement_index);

    std::array<MeasurementEntry, BUFFER_SIZE_PER_SENSOR> entries_;

    const size_t k_flash_size = sizeof(uuid_t) + sizeof(MeasurementEntry) * BUFFER_SIZE_PER_SENSOR;

    bool flash_was_init_ = false;

    esp_flash_t flash_chip_;
    esp_flash_region_t region_;

    static uint8_t config_register_;
};

}  // namespace storage

#endif  // INCLUDE_STORAGE_FLASH_BUFFER_H_
