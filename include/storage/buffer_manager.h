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
#include <vector>

#include "storage/storage.h"

namespace storage {

template <class T>
class BufferManager {
    static_assert(std::is_base_of<Storage, T>(), "T must be derived of class Storage");

 public:
    bool createBuffer(const sensor_id_t uuid);
    bool removeBuffer(const sensor_id_t uuid);
    T *getBuffer(const sensor_id_t uuid);

    std::vector<sensor_id_t> getBufferUUIDs() const;

    void clear();
    void clearBuffer(const sensor_id_t uuid);

 private:
    std::map<sensor_id_t, T> buffers_;
};

template <typename T>
bool BufferManager<T>::createBuffer(const sensor_id_t uuid) {
    // T t{uuid};
    if (this->buffers_.contains(uuid)) return false;

    this->buffers_.insert({uuid, T{uuid}});
    return true;
}

template <typename T>
bool BufferManager<T>::removeBuffer(const sensor_id_t uuid) {
    bool has_value = this->buffers_.contains(uuid);

    if (has_value)
        this->buffers_.erase(uuid);

    return has_value;
}

template <typename T>
T *BufferManager<T>::getBuffer(const sensor_id_t uuid) {
    if (this->buffers_.contains(uuid))
        return &this->buffers_.at(uuid);
    return nullptr;
}

template <typename T>
void BufferManager<T>::clear() {
    this->buffers_.clear();
}

template <typename T>
std::vector<sensor_id_t> BufferManager<T>::getBufferUUIDs() const {
    std::vector<sensor_id_t> vec;
    vec.reserve(this->buffers_.size());

    for (auto it = this->buffers_.begin(); it != this->buffers_.end(); it++) {
        vec.push_back(it->first);
    }

    return vec;
}

template <typename T>
void BufferManager<T>::clearBuffer(const sensor_id_t uuid) {
    if (this->buffers_.contains(uuid))
        this->buffers_.at(uuid).clearAll();
}


}  // namespace storage


#endif  // INCLUDE_STORAGE_BUFFER_MANAGER_H_
