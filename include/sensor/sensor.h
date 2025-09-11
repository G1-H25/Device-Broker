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
#include "sensor/measurement-entry.h"

namespace sensor {

typedef struct measurement_entry_t {
    uint32_t timestamp;
    int32_t temp_entries;
    int32_t humidity_entries;

    MeasurementEntry *next;
} MeasurementEntry;


typedef struct sensor_t {
    char uuid[37];
    MeasurementEntry *entries;
} Sensor;

int pushMeasurementEntry(Sensor *sensor, MeasurementEntry entry);
int pushEntries(Sensor *sensor, MeasurementEntry entries[], int num_entries);

int removeMeasurementEntry(Sensor *sensor, int index);
int popMeasurementEntry(Sensor *sensor);

int clearMeasurementEntries(Sensor *sensor);
int removeOldestMeasurementEntry(Sensor *sensor);

}  // namespace sensor

#endif  // INCLUDE_SENSOR_SENSOR_H_
