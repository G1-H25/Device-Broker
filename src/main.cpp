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

#include "storage/flash_buffer.h"

extern "C" {
    void app_main(void);
}

constexpr uint32_t current_time = 123456;
constexpr uint32_t temperature = 10;
constexpr uint32_t humidity = 10;

void test_sensor_buffer_push() {
    storage::uuid_t uuid;
    uuid.fill(0xFF);
    storage::FlashBuffer buffer = storage::FlashBuffer(uuid, 0x7612);

    buffer.pushMeasurement({ 0, 0, 0 });

    TEST_ASSERT_EQUAL(1, buffer.available());
    TEST_ASSERT_TRUE(buffer.hasData());

    TEST_ASSERT_NOT_NULL(buffer.getLatestMeasurement());

    buffer.pushMeasurement({ 0, 0, 0 });

    TEST_ASSERT_EQUAL(2, buffer.available());
    TEST_ASSERT_NOT_NULL(buffer.getLatestMeasurement());

    for (int i = 0; i < buffer.getBufferSize() + 10; i++) {
        buffer.pushMeasurement({ 0, 0, 0 });
    }

    TEST_ASSERT_EQUAL(buffer.getBufferSize(), buffer.available());
}

void test_sensor_buffer_pop() {
    storage::uuid_t uuid;
    uuid.fill(0xFF);
    storage::FlashBuffer buffer = storage::FlashBuffer(uuid, 0x7612);

    TEST_ASSERT_EQUAL(0, buffer.available());
    TEST_ASSERT_FALSE(buffer.hasData());
    TEST_ASSERT_FALSE(buffer.tryPop());

    buffer.pushMeasurement({ 0, 0, 0 });

    TEST_ASSERT_TRUE(buffer.tryPop());
    TEST_ASSERT_FALSE(buffer.hasData());
    TEST_ASSERT_EQUAL(0, buffer.available());

    TEST_ASSERT_NULL(buffer.getLatestMeasurement());
}

void test_sensor_buffer_data_integrity() {
    storage::uuid_t uuid;
    uuid.fill(0xFF);
    storage::FlashBuffer buffer = storage::FlashBuffer(uuid, 0x7612);
    buffer.pushMeasurement({current_time, temperature, humidity});
    buffer.pushMeasurement({current_time + 10, temperature + 10, humidity + 10});

    TEST_ASSERT_EQUAL(*(uint32_t *) uuid.data(),
                    *(uint32_t *) buffer.getUUID().data());

    TEST_ASSERT_EQUAL(*(uint32_t *) uuid.data() + sizeof(uint32_t),
                    *(uint32_t *) buffer.getUUID().data() + sizeof(uint32_t));

    TEST_ASSERT_EQUAL(*(uint32_t *) uuid.data() + sizeof(uint32_t) * 2,
                *(uint32_t *) buffer.getUUID().data() + sizeof(uint32_t) * 2);

    TEST_ASSERT_EQUAL(*(uint32_t *) uuid.data() + sizeof(uint32_t) * 3,
                    *(uint32_t *) buffer.getUUID().data() + sizeof(uint32_t) * 3);

    TEST_ASSERT_NOT_NULL(buffer.loadMeasurement(0));
    TEST_ASSERT_NOT_NULL(buffer.loadMeasurement(1));

    TEST_ASSERT_EQUAL(current_time, buffer.loadMeasurement(0)->timestamp);
    TEST_ASSERT_EQUAL(temperature, buffer.loadMeasurement(0)->temperature);
    TEST_ASSERT_EQUAL(humidity, buffer.loadMeasurement(0)->humidity);

    TEST_ASSERT_EQUAL(current_time + 10, buffer.loadMeasurement(1)->timestamp);
    TEST_ASSERT_EQUAL(temperature + 10, buffer.loadMeasurement(1)->temperature);
    TEST_ASSERT_EQUAL(humidity + 10, buffer.loadMeasurement(1)->humidity);
}

void app_main() {
    UNITY_BEGIN();

    RUN_TEST(test_sensor_buffer_push);
    RUN_TEST(test_sensor_buffer_pop);
    RUN_TEST(test_sensor_buffer_data_integrity);

    UNITY_END();
}
