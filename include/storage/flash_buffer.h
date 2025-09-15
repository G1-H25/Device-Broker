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

#include "storage/storage.h"
#include <EEPROM.h>

namespace Storage {

class FlashBuffer : public Storage {
 public:
    explicit FlashBuffer(uuid_t uuid);

    void pushMeasurement(const MeasurementEntry *measurement) override;
    bool tryPop() override;

    bool hasData() override;
    uint32_t available() override;

    const MeasurementEntry *getLatestMeasurement() override;
};

}  // namespace Storage

#endif  // INCLUDE_STORAGE_FLASH_BUFFER_H_
