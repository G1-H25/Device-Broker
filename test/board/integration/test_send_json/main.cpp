/**
 * @file main.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-27
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <esp_log.h>
#include <unity.h>
#include <ArduinoJson.h>
#include <string>
#include "wifi/wifi_client.h"
#include "http/http_client.h"
#include "http/sensor_data_sender.h"
#include "http/http_esp_client_driver.h"
#include "storage/buffer_manager.h"
#include "storage/flash_buffer.h"
#include "secrets/credentials.h"
#include "secrets/routes.h"

void test_send_to_test_api() {
    constexpr uint8_t kId = 10;
    storage::BufferManager<storage::FlashBuffer> buffers;

    buffers.createBuffer(kId);
    storage::Storage *buffer = buffers.getBuffer(kId);
    for (int i = 0; i < buffer->getBufferSize() / 4; i++) {
        buffer->pushMeasurement({10, 10, 10});
    }

    std::string str = http::prepareRequest("abc123", 100, buffer);

    ESP_LOGI("JSON", "%s", str);

    http::HttpResponse resp =
        http::HttpClient::getDriver()->performPostRequest(
            HTTP_TEST_API_HOST,
            HTTP_TEST_API_PORT,
            HTTP_API_SUBMIT_BATCH,
            HttpRequest {
                .data = str.c_str(),
                .headers = {
                    { "Content-Type", "application/json" }
                }
            });

    ESP_LOGI("__STATUS__", "(%i) %s", resp.status, resp.data);
    TEST_ASSERT_EQUAL(HttpStatus_Ok, resp.status);
}

extern "C" void app_main() {
    wifi::WiFiClient client{TEST_WIFI_SSID, TEST_WIFI_PASSWORD};
    client.connect();

    http::EspHttpDriver driver;
    http::HttpClient::setDriver(&driver);

    while (client.getStatus() != wifi::CONNECTED) {}
    vTaskDelay(pdMS_TO_TICKS(2000));

    UNITY_BEGIN();

    RUN_TEST(test_send_to_test_api);

    UNITY_END();
}
