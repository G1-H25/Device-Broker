/**
 * @file storage.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-16
 *
 * @copyright Copyright (c) 2025
*/

#include <cstddef>
#include "storage/storage.h"

namespace storage {

uint8_t Storage::getBufferSize() const {
    return this->buffer_size_;
}

const uuid_t Storage::getUUID() {
    return this->uuid_;
}

bool Storage::hasData() {
    return this->entry_count_ > 0;
}

size_t Storage::available() {
    return this->entry_count_;
}

}  // namespace storage


