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
#include <esp_vfs.h>
#include "storage/flash_buffer.h"

extern "C" {
    void app_main(void);
}

using storage::MeasurementEntry;

constexpr uint32_t current_time = 123456;
constexpr uint32_t temperature = 10;
constexpr uint32_t humidity = 10;

void test_sensor_buffer_push() {
    storage::sensor_id_t uuid;
    uuid = 10;
    storage::FlashBuffer buffer = storage::FlashBuffer(uuid);

    buffer.pushMeasurement({ 0, 0, 0 });

    TEST_ASSERT_EQUAL(1, buffer.available());
    TEST_ASSERT_TRUE(buffer.hasData());

    MeasurementEntry entry;
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
    storage::sensor_id_t uuid;
    uuid = 10;
    storage::FlashBuffer buffer = storage::FlashBuffer(uuid);

    MeasurementEntry entry;
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
    storage::sensor_id_t uuid, uuid2;
    uuid = 15;
    uuid2 = 10;

    storage::FlashBuffer buffer = storage::FlashBuffer(uuid);
    storage::FlashBuffer buffer2 = storage::FlashBuffer(uuid2);

    buffer.pushMeasurement({current_time, temperature, humidity});
    buffer.pushMeasurement({current_time + 10, temperature + 10, humidity + 10});

    buffer2.pushMeasurement({current_time + 25, temperature + 25, humidity + 25});
    buffer2.pushMeasurement({current_time + 15, temperature + 15, humidity + 15});

    TEST_ASSERT_EQUAL(uuid, buffer.getSensorId());

    MeasurementEntry entry;
    TEST_ASSERT_TRUE(buffer.getMeasurement(entry, 0));
    TEST_ASSERT_TRUE(buffer.getMeasurement(entry, 1));

    TEST_ASSERT_FALSE(buffer.getMeasurement(entry, 10));
    TEST_ASSERT_FALSE(buffer.getMeasurement(entry, -1));

    buffer.getMeasurement(entry, 0);
    TEST_ASSERT_EQUAL(current_time, entry.timestamp);
    TEST_ASSERT_EQUAL(temperature, entry.temperature);
    TEST_ASSERT_EQUAL(humidity, entry.humidity);

    buffer.getMeasurement(entry, 1);
    TEST_ASSERT_EQUAL(current_time + 10, entry.timestamp);
    TEST_ASSERT_EQUAL(temperature + 10, entry.temperature);
    TEST_ASSERT_EQUAL(humidity + 10, entry.humidity);

    TEST_ASSERT_EQUAL(uuid2, buffer2.getSensorId());

    TEST_ASSERT_TRUE(buffer2.getMeasurement(entry, 0));
    TEST_ASSERT_TRUE(buffer2.getMeasurement(entry, 1));

    TEST_ASSERT_FALSE(buffer2.getMeasurement(entry, 10));
    TEST_ASSERT_FALSE(buffer2.getMeasurement(entry, -1));

    buffer2.getMeasurement(entry, 0);
    TEST_ASSERT_EQUAL(current_time + 25, entry.timestamp);
    TEST_ASSERT_EQUAL(temperature + 25, entry.temperature);
    TEST_ASSERT_EQUAL(humidity + 25, entry.humidity);

    buffer2.getMeasurement(entry, 1);
    TEST_ASSERT_EQUAL(current_time + 15, entry.timestamp);
    TEST_ASSERT_EQUAL(temperature + 15, entry.temperature);
    TEST_ASSERT_EQUAL(humidity + 15, entry.humidity);
}

void app_main() {
    UNITY_BEGIN();

    RUN_TEST(test_sensor_buffer_push);
    RUN_TEST(test_sensor_buffer_pop);
    RUN_TEST(test_sensor_buffer_data_integrity);

    UNITY_END();
}
