/**
 * @file main.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-22
 *
 * @copyright Copyright (c) 2025
*/

#include <unity.h>

#include "storage/memory_buffer.h"

extern "C" {
    void app_main(void);
}

constexpr uint32_t current_time = 123456;
constexpr uint32_t temperature = 10;
constexpr uint32_t humidity = 10;

void setUp() {}
void tearDown() {}

void test_smoke_sensor_buffer_push_pop() {
    storage::MemoryBuffer buffer = storage::MemoryBuffer({ 1 });

    buffer.pushMeasurement({0, 0, 0});

    TEST_ASSERT_EQUAL(1, buffer.available());

    TEST_ASSERT_TRUE(buffer.tryPop());
    TEST_ASSERT_EQUAL(1, buffer.available());
}

int main(int argc, char const *argv[]) {
    UNITY_BEGIN();

    RUN_TEST(test_smoke_sensor_buffer_push_pop);

    return UNITY_END();
}
