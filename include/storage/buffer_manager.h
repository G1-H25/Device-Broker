/**
 * @file buffer_manager.h
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-29
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef INCLUDE_STORAGE_BUFFER_MANAGER_H_
#define INCLUDE_STORAGE_BUFFER_MANAGER_H_

#include <map>
#include <optional>
#include <type_traits>

#include "storage/storage.h"

namespace storage {

template <typename T>
class BufferManager {
    static_assert(std::is_base_of<Storage, T>(), "T must be derived of class Storage");

 public:
    bool createBuffer(const uuid_t &uuid);
    bool removeBuffer(const uuid_t &uuid);
    T *getBuffer(const uuid_t &uuid);

    void clear();

 private:
    std::map<uuid_t, T> buffers_;
};

template <typename T>
bool BufferManager<T>::createBuffer(const uuid_t &uuid) {
    // T t{uuid};
    if (this->buffers_.contains(uuid)) return false;

    this->buffers_.insert({uuid, T{uuid}});
    return true;
}

template <typename T>
bool BufferManager<T>::removeBuffer(const uuid_t &uuid) {
    bool has_value = this->buffers_.contains(uuid);

    if (has_value)
        this->buffers_.erase(uuid);

    return has_value;
}

template <typename T>
T *BufferManager<T>::getBuffer(const uuid_t &uuid) {
    if (this->buffers_.contains(uuid))
        return &this->buffers_.at(uuid);
    return nullptr;
}

template <typename T>
void BufferManager<T>::clear() {
    this->buffers_.clear();
}

}  // namespace storage


#endif  // INCLUDE_STORAGE_BUFFER_MANAGER_H_
