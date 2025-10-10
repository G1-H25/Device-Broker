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
#include <esp_timer.h>
#include <esp_log.h>

#include "http/http_client.h"
#include "http/http_response.h"
#include "http/http_esp_client_driver.h"
#include "wifi/wifi_client.h"
#include "secrets/credentials.h"
#include "secrets/routes.h"

using http::HttpClient;
using http::HttpResponse;

void test_http_client_get() {
    HttpResponse response = HttpClient::getDriver()->performGetRequest(HTTP_API_HOST, 443, "");
    TEST_ASSERT_NOT_EQUAL(200, response.status);  // using current value to test if get works

    ESP_LOGI("HTTP_GET_DATA", "%s", response.data.begin());
}

void test_http_client_post() {
    HttpResponse response = HttpClient::getDriver()->performPostRequest(HTTP_API_HOST, 443, "", {});
    TEST_ASSERT_EQUAL(400, response.status);  // using current value to test if get works

    ESP_LOGI("HTTP_POST_DATA", "%s", response.data.begin());
}

extern "C" void app_main() {
    UNITY_BEGIN();

    wifi::WiFiClient client{WIFI_SSID, WIFI_PASSWORD};

    while (client.getStatus() != wifi::CONNECTED) {}
    vTaskDelay(pdMS_TO_TICKS(10000));

    http::EspHttpDriver driver;  // Does not need to be allocated on heap since it is used in main.
    HttpClient::setDriver(&driver);

    RUN_TEST(test_http_client_get);
    vTaskDelay(pdMS_TO_TICKS(10000));
    RUN_TEST(test_http_client_post);

    UNITY_END();
}
