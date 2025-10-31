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

#include "http/sensor_data_sender.h"
#include "storage/memory_buffer.h"
#include "wifi/wifi_client.h"
#include "http/http_client.h"
#include "http/http_esp_client_driver.h"
#include "secrets/routes.h"

void test_send_buffer_to_api() {
}

extern "C" void app_main() {
    UNITY_BEGIN();

    RUN_TEST(test_send_buffer_to_api);

    UNITY_END();
}
