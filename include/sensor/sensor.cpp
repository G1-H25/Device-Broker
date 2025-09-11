//!
//! @file sensor.cpp
//! @author Love Lindeborg
//! @brief
//! @version 0.1
//! @date 2025-09-11
//!
//! @copyright Copyright (c) 2025
//!

#include <sensor/sensor.h>
#include <memory.h>
#include <heap_memory_layout.h>
#include <memory>
#include <algorithm>

int sensor::pushMeasurementEntry(Sensor *sensor, MeasurementEntry *entry) {
    if (!std::is_heap(entry, entry + sizeof(*entry))) return -1;

    entry->next = sensor->entries;
    sensor->entries = entry;

    return 0;
}

int sensor::pushEntries(Sensor *sensor, MeasurementEntry *entries[], int num_entries) {
    for (int i = 0; i < num_entries; i++) {
        if (pushMeasurementEntry(sensor, entries[i]) < 0) {
            return -1;
        }
    }

    return 0;
}


int sensor::removeMeasurementEntry(Sensor *sensor, int index) {
    MeasurementEntry *nextEntry, *entry = sensor->entries;
    for (int i = 0; i < index - 1; i++) {
        entry = entry->next;
        if (entry == nullptr) return -1;
    }

    nextEntry = entry->next;
    if (nextEntry == nullptr) return -1;

    delete entry;
    entry = nextEntry;

    return 0;
}

int sensor::popMeasurementEntry(Sensor *sensor) {
    MeasurementEntry *entry = sensor->entries->next;

    if (entry == nullptr) {
        return -1;
    }

    delete sensor->entries;
    sensor->entries = entry;

    return 0;
}


int sensor::clearMeasurementEntries(Sensor *sensor) {
    while (popMeasurementEntry(sensor) == 0) { }
}

int sensor::removeOldestMeasurementEntry(Sensor *sensor) {
    MeasurementEntry *entry = sensor->entries;
    while (entry->next != nullptr) {
        entry = entry->next;
    }

    delete entry;
}
