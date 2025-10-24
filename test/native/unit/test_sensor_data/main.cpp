/**
 * @file main.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <unity.h>

#include <string>
#include <vector>

#include "http/sensor_data_sender.h"
#include "storage/memory_buffer.h"
#include "wifi/wifi_client.h"
#include "http/http_client.h"
#include "secrets/routes.h"

void test_buffer_to_json() {
    constexpr int sensor_id = 0;
    constexpr int temp_data = 10;
    storage::BufferManager<storage::MemoryBuffer> manager;

    manager.createBuffer(sensor_id);
    manager.getBuffer(sensor_id)->pushMeasurement({temp_data, temp_data, temp_data});
    manager.getBuffer(sensor_id)->pushMeasurement({temp_data, temp_data, temp_data});

    JsonDocument arr = http::bufferToJson(static_cast<storage::Storage *>(manager.getBuffer(sensor_id)));

    std::string str;
    serializeJsonPretty(arr, str);

    TEST_ASSERT_EQUAL(2, arr.size());

    TEST_ASSERT_EQUAL(temp_data, arr["measurements"][0][HTTP_API_JSON_TIME_KEY]);
    TEST_ASSERT_EQUAL(temp_data, arr["measurements"][0][HTTP_API_JSON_HUM_KEY]);
    TEST_ASSERT_EQUAL(temp_data, arr["measurements"][0][HTTP_API_JSON_TEMP_KEY]);

    TEST_ASSERT_EQUAL(temp_data, arr["measurements"][1][HTTP_API_JSON_TIME_KEY]);
    TEST_ASSERT_EQUAL(temp_data, arr["measurements"][1][HTTP_API_JSON_HUM_KEY]);
    TEST_ASSERT_EQUAL(temp_data, arr["measurements"][1][HTTP_API_JSON_TEMP_KEY]);
}

int main(int argc, char const *argv[]) {
    UNITY_BEGIN();

    RUN_TEST(test_buffer_to_json);

    return UNITY_END();
}
