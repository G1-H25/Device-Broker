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

void test_buffer_manager_create_buffer() {
    storage::BufferManager<storage::MemoryBuffer> buffer;

    TEST_ASSERT_TRUE(buffer.createBuffer({1}));
    TEST_ASSERT_FALSE(buffer.createBuffer({1}));

    storage::MemoryBuffer *entry;
    TEST_ASSERT_NOT_EQUAL(nullptr, buffer.getBuffer({1}));
}

void test_buffer_manager_remove_buffer() {
    storage::BufferManager<storage::MemoryBuffer> buffer;
    storage::MemoryBuffer *entry;

    TEST_ASSERT_FALSE(buffer.removeBuffer({1}));
    TEST_ASSERT_TRUE(buffer.createBuffer({1}));
    TEST_ASSERT_TRUE(buffer.removeBuffer({1}));

    TEST_ASSERT_EQUAL(nullptr, buffer.getBuffer({1}));
}

void test_buffer_manager_get_buffer() {
    storage::BufferManager<storage::MemoryBuffer> buffer;
    storage::MemoryBuffer *entry;

    TEST_ASSERT_TRUE(buffer.createBuffer({1}));
    TEST_ASSERT_NOT_EQUAL(nullptr, buffer.getBuffer({1}));
}


int main(int argc, char const *argv[]) {
    UNITY_BEGIN();

    RUN_TEST(test_buffer_manager_create_buffer);
    RUN_TEST(test_buffer_manager_remove_buffer);
    RUN_TEST(test_buffer_manager_get_buffer);

    return UNITY_END();
}
