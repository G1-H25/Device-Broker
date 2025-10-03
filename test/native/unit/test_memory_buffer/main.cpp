/**
 * @file main.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-16
 *
 * @copyright Copyright (c) 2025
*/

#include <unity.h>

#include <stdio.h>
#include "storage/memory_buffer.h"

constexpr uint32_t current_time = 123456;
constexpr uint32_t temperature = 10;
constexpr uint32_t humidity = 10;

void test_sensor_buffer_push() {
    storage::MemoryBuffer buffer = storage::MemoryBuffer({ 1 });
    storage::MeasurementEntry entry;

    buffer.pushMeasurement({ 0, 0, 0 });

    TEST_ASSERT_EQUAL(1, buffer.available());
    TEST_ASSERT_TRUE(buffer.hasData());

    TEST_ASSERT_TRUE(buffer.getLatestMeasurement(entry));

    buffer.pushMeasurement({ 0, 0, 0 });

    TEST_ASSERT_EQUAL(2, buffer.available());
    TEST_ASSERT_TRUE(buffer.getLatestMeasurement(entry));

    for (int i = 0; i < buffer.getBufferSize() + 10; i++) {
        buffer.pushMeasurement({ 0, 0, 0 });
    }

    TEST_ASSERT_EQUAL(buffer.getBufferSize(), buffer.available());
}

void test_sensor_buffer_pop() {
    storage::MemoryBuffer buffer = storage::MemoryBuffer({ 1 });
    storage::MeasurementEntry entry;

    TEST_ASSERT_EQUAL(0, buffer.available());
    TEST_ASSERT_FALSE(buffer.hasData());
    TEST_ASSERT_FALSE(buffer.tryPop(entry));

    buffer.pushMeasurement({ 0, 0, 0 });

    TEST_ASSERT_TRUE(buffer.tryPop(entry));
    TEST_ASSERT_FALSE(buffer.hasData());
    TEST_ASSERT_EQUAL(0, buffer.available());

    TEST_ASSERT_FALSE(buffer.getLatestMeasurement(entry));
}

void test_sensor_buffer_data_integrity() {
    storage::MemoryBuffer buffer = storage::MemoryBuffer({ 1 });
    storage::MeasurementEntry entry;

    buffer.pushMeasurement({current_time, temperature, humidity});

    TEST_ASSERT_EQUAL(*(__uint128_t *) storage::uuid_t{ 1 }.data(), *(__uint128_t *) buffer.getUUID().data());

    TEST_ASSERT_EQUAL(current_time, entry.timestamp);
    TEST_ASSERT_EQUAL(temperature, entry.temperature);
    TEST_ASSERT_EQUAL(humidity, entry.humidity);
}

int main(int argc, char const *argv[]) {
    UNITY_BEGIN();

    RUN_TEST(test_sensor_buffer_push);
    RUN_TEST(test_sensor_buffer_pop);
    RUN_TEST(test_sensor_buffer_data_integrity);

    return UNITY_END();
}
