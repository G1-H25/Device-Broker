//!
//! @file memory_buffer.h
//! @author Love Lindeborg
//! @brief
//! @version 0.1
//! @date 2025-09-15
//!
//! @copyright Copyright (c) 2025
//!

#ifndef INCLUDE_STORAGE_MEMORY_BUFFER_H_
#define INCLUDE_STORAGE_MEMORY_BUFFER_H_

#include "storage/storage.h"

namespace storage {

class MemoryBuffer : public Storage {
 public:
    explicit MemoryBuffer(uuid_t uuid);

    void pushMeasurement(const MeasurementEntry &measurement) override;
    bool tryPop() override;

    bool hasData() override;
    uint32_t available() override;

    const MeasurementEntry *getLatestMeasurement() override;

 private:
    std::array<MeasurementEntry, BUFFER_SIZE_PER_SENSOR> entries_;
};

}  // namespace storage

#endif  // INCLUDE_STORAGE_MEMORY_BUFFER_H_
