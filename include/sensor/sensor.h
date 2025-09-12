//!
//! @file sensor.h
//! @author Love Lindeborg
//! @brief
//! @version 0.1
//! @date 2025-09-09
//!
//! @copyright Copyright (c) 2025
//!

#ifndef INCLUDE_SENSOR_SENSOR_H_
#define INCLUDE_SENSOR_SENSOR_H_

#include <array>
#include <cstdint>

#define ENTRY_BUFFER_SIZE_PER_SENSOR 32

namespace sensor {

typedef std::array<uint8_t, 16> uuid_t;

typedef struct measurement_entry_t {
    uint32_t timestamp;
    uint32_t temperature;
    uint32_t humidity;
} MeasurementEntry;

typedef struct flash_measurement_entry_t {
    uuid_t uuid;
    MeasurementEntry data;
} FlashMeasurementEntry;

class SensorBuffer {
 public:
    static SensorBuffer &getInstance();
    void pushMeasurement(const MeasurementEntry &measurement);
    bool tryPop();
    bool hasData();
    uint32_t available();

    const MeasurementEntry *getLatestMeasurement();

 private:
    SensorBuffer();

    uuid_t uuid_;
    uint8_t entry_count_ = 0;
    uint8_t head = 0;

    const uint8_t buffer_size_ = ENTRY_BUFFER_SIZE_PER_SENSOR;
    MeasurementEntry entries_[ENTRY_BUFFER_SIZE_PER_SENSOR];
};

}  // namespace sensor

#endif  // INCLUDE_SENSOR_SENSOR_H_
