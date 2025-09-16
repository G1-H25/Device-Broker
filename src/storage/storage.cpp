//!
//! @file storage.cpp
//! @author Love Lindeborg
//! @brief
//! @version 0.1
//! @date 2025-09-16
//!
//! @copyright Copyright (c) 2025
//!

#include "storage/storage.h"

namespace storage {

const uint8_t Storage::getBufferSize() {
    return this->buffer_size_;
}

const uuid_t Storage::getUUID() {
    return this->uuid_;
}

}  // namespace storage
