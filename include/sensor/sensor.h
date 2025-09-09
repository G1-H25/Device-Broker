//!
//! @file package.h
//! @author Love Lindeborg
//! @brief
//! @version 0.1
//! @date 2025-09-09
//!
//! @copyright Copyright (c) 2025
//!

#ifndef INCLUDE_SENSOR_SENSOR_H_
#define INCLUDE_SENSOR_SENSOR_H_

#include <cstdint>
#include "sensor/sensor-measurement-entry.h"

namespace storage {

struct Sensor {
    char uuid[37];
    SensorMeasurementEntry *entries;
};

}

#endif  // INCLUDE_SENSOR_SENSOR_H_
