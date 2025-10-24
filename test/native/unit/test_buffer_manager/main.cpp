/**
 * @file main.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-29
 *
 * @copyright Copyright (c) 2025
*/

#include <unity.h>

#include "storage/buffer_manager.h"
#include "storage/memory_buffer.h"

constexpr storage::sensor_id_t sensor_id = 101;

void test_buffer_manager_create_buffer() {
    storage::BufferManager<storage::MemoryBuffer> buffer;

    TEST_ASSERT_TRUE(buffer.createBuffer(sensor_id));
    TEST_ASSERT_FALSE(buffer.createBuffer(sensor_id));

    storage::MemoryBuffer *entry;
    TEST_ASSERT_NOT_EQUAL(nullptr, buffer.getBuffer(sensor_id));
}

void test_buffer_manager_remove_buffer() {
    storage::BufferManager<storage::MemoryBuffer> buffer;
    storage::MemoryBuffer *entry;

    TEST_ASSERT_FALSE(buffer.removeBuffer(sensor_id));
    TEST_ASSERT_TRUE(buffer.createBuffer(sensor_id));
    TEST_ASSERT_TRUE(buffer.removeBuffer(sensor_id));

    TEST_ASSERT_EQUAL(nullptr, buffer.getBuffer(sensor_id));
}

void test_buffer_manager_get_buffer() {
    storage::BufferManager<storage::MemoryBuffer> buffer;
    storage::MemoryBuffer *entry;

    TEST_ASSERT_TRUE(buffer.createBuffer(sensor_id));
    TEST_ASSERT_NOT_EQUAL(nullptr, buffer.getBuffer(sensor_id));
}


int main(int argc, char const *argv[]) {
    UNITY_BEGIN();

    RUN_TEST(test_buffer_manager_create_buffer);
    RUN_TEST(test_buffer_manager_remove_buffer);
    RUN_TEST(test_buffer_manager_get_buffer);

    return UNITY_END();
}
