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
    bool createBuffer(sensor_id_t sensor_id);
    bool removeBuffer(sensor_id_t sensor_id);
    T *getBuffer(sensor_id_t sensor_id);

    std::vector<sensor_id_t> getBufferIds() const;

    void clear();
    void clearBuffer(const sensor_id_t sensor_id);

 private:
    std::map<sensor_id_t, T> buffers_;
};

template <typename T>
bool BufferManager<T>::createBuffer(sensor_id_t sensor_id) {
    if (this->buffers_.contains(sensor_id)) return false;

    this->buffers_.insert({sensor_id, T{sensor_id}});
    return true;
}

template <typename T>
bool BufferManager<T>::removeBuffer(sensor_id_t sensor_id) {
    bool has_value = this->buffers_.contains(sensor_id);

    if (has_value)
        this->buffers_.erase(sensor_id);

    return has_value;
}

template <typename T>
T *BufferManager<T>::getBuffer(sensor_id_t sensor_id) {
    if (this->buffers_.contains(sensor_id))
        return &this->buffers_.at(sensor_id);
    return nullptr;
}

template <typename T>
void BufferManager<T>::clear() {
    this->buffers_.clear();
}

template <typename T>
std::vector<sensor_id_t> BufferManager<T>::getBufferIds() const {
    std::vector<sensor_id_t> vec;
    vec.reserve(this->buffers_.size());

    for (auto it = this->buffers_.begin(); it != this->buffers_.end(); it++) {
        vec.push_back(it->first);
    }

    return vec;
}

template <typename T>
void BufferManager<T>::clearBuffer(const sensor_id_t sensor_id) {
    if (this->buffers_.contains(sensor_id))
        this->buffers_.at(sensor_id).clearAll();
}


}  // namespace storage


#endif  // INCLUDE_STORAGE_BUFFER_MANAGER_H_
