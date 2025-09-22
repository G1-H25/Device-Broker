/**
 * @file storage.h
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-9
 *
 * @copyright Copyright (c) 2025
*/

#ifndef INCLUDE_STORAGE_STORAGE_H_
#define INCLUDE_STORAGE_STORAGE_H_

#include <cstdint>
#include <array>

#define BUFFER_SIZE_PER_SENSOR 32

namespace storage {

typedef std::array<uint8_t, 16> uuid_t;

typedef struct measurement_entry_t {
    uint32_t timestamp;
    uint16_t temperature;
    uint16_t humidity;
} MeasurementEntry;

class Storage {
 public:
    explicit Storage(uuid_t uuid) : uuid_(uuid) {}

    virtual void pushMeasurement(const MeasurementEntry &measurement) = 0;
    virtual bool tryPop() = 0;
    size_t available();
    bool hasData();

    virtual uint8_t getBufferSize() const;
    virtual const uuid_t getUUID();
    virtual const MeasurementEntry *getLatestMeasurement() = 0;

    virtual void clearAll() = 0;

 protected:
    uint32_t head_ = 0;
    uint32_t entry_count_ = 0;

    uuid_t uuid_;

    const uint8_t buffer_size_ = BUFFER_SIZE_PER_SENSOR;
};

}  // namespace storage

#endif  // INCLUDE_STORAGE_STORAGE_H_
