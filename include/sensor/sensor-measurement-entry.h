//!
//! @file package.h
//! @author Love Lindeborg
//! @brief
//! @version 0.1
//! @date 2025-09-09
//!
//! @copyright Copyright (c) 2025
//!

#ifndef INCLUDE_SENSOR_SENSOR_MEASUREMENT_ENTRY_H_
#define INCLUDE_SENSOR_SENSOR_MEASUREMENT_ENTRY_H_

#include <cstdint>

namespace storage {

struct SensorMeasurementEntry {
    uint32_t timestamp;
    int32_t temp_entries;
    int32_t humidity_entries;
};

}

#endif  // INCLUDE_SENSOR_SENSOR_MEASUREMENT_ENTRY_H_
