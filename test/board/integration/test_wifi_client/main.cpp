/**
 * @file main.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-30
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <unity.h>

#include "wifi/wifi_client.h"
#include "secrets/routes.h"
#include "secrets/credentials.h"

using wifi::WiFiClient;

void test_wifi_client_connection() {
    WiFiClient client{WIFI_SSID, WIFI_PASSWORD};

    TEST_ASSERT_EQUAL(ESP_OK, client.connect());
    TEST_ASSERT_EQUAL(ESP_OK, client.disconnect());
}

extern "C" void app_main() {
    UNITY_BEGIN();

    RUN_TEST(test_wifi_client_connection);

    UNITY_END();
}
