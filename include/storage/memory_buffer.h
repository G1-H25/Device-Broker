/**
 * @file memory_buffer.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-15
 *
 * @copyright Copyright (c) 2025
*/

#ifndef INCLUDE_STORAGE_MEMORY_BUFFER_H_
#define INCLUDE_STORAGE_MEMORY_BUFFER_H_

#include <cstddef>
#include "storage/storage.h"

namespace storage {

class MemoryBuffer : public Storage {
 public:
    explicit MemoryBuffer(uuid_t uuid);

    bool pushMeasurement(const MeasurementEntry &measurement) override;
    bool tryPop(MeasurementEntry &out) override;

    bool getLatestMeasurement(MeasurementEntry &out) override;

    void clearAll() override;
 private:
    std::array<MeasurementEntry, BUFFER_SIZE_PER_SENSOR> entries_;
};

}  // namespace storage

#endif  // INCLUDE_STORAGE_MEMORY_BUFFER_H_
