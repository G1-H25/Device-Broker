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

#include <unity.h>
#include "wifi/wifi_client.h"
#include "http/http_client.h"
#include "http/sensor_data_sender.h"
#include "http/http_esp_client_driver.h"
#include "storage/buffer_manager.h"
#include "storage/memory_buffer.h"
#include "secrets/credentials.h"
#include "secrets/routes.h"

void test_send_to_test_api() {
    constexpr uint8_t kId;
    storage::BufferManager<storage::MemoryBuffer> buffers;

    buffers.createBuffer(kId);
    buffers.getBuffer(kId)->pushMeasurement({10, 10, 10});
    buffers.getBuffer(kId)->pushMeasurement({20, 20, 20});

    http::HttpResponse resp = http::HttpClient::
        getDriver()->performPostRequest(
            HTTP_TEST_API_HOST,
            HTTP_TEST_API_PORT,
            HTTP_API_SUBMIT_BATCH,
            {
                .data = http::bufferToJson(buffers.getBuffer(kId)),
                .headers = {
                    { "Content-Type", "application/json" }
                }
            });

    TEST_ASSERT_EQUAL(200, resp.status);
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
