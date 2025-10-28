/**
 * @file main.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-16
 *
 * @copyright Copyright (c) 2025
*/

#ifndef UNIT_TEST

#include <esp_log.h>
#include <driver/gpio.h>
#include <sntp.h>
#include <esp_sntp.h>
#include <esp_netif_sntp.h>
#include <mbedtls/base64.h>
#include <string>
#include "http/http_client.h"
#include "http/http_driver.h"
#include "http/http_esp_client_driver.h"
#include "http/sensor_data_sender.h"
#include "wifi/wifi_client.h"
#include "storage/buffer_manager.h"
#include "storage/flash_buffer.h"
#include "gpio/esp_gpio_driver.h"
#include "gpio/button.h"
#include "secrets/routes.h"
#include "secrets/credentials.h"

#define IO_MUX_BASE_ADDR 0x60009000

using jenlib::gpio::Pin;
using jenlib::gpio::PinIndex;
using jenlib::gpio::PinMode;
using jenlib::gpio::DigitalValue;

using jenlib::events::Event;
using jenlib::events::EventCallback;
using jenlib::events::EventId;
using jenlib::events::EventType;

using storage::BufferManager;
using storage::FlashBuffer;

#define CONFIG_EXAMPLE_ENABLE_RESPONSE_BUFFER_DUMP 1

#define TAG "NTP"

// void createMockSensor(FlashBuffer buffer, storage::uuid_t uuid) {
//     int period_ms = 1000;
//     TimeOut_t timeout;
//     TickType_t period_tick = pdMS_TO_TICKS(period_ms);
//     vTaskSetTimeOutState(&timeout);

//     for (;;) {
//         if (xTaskCheckForTimeOut(&timeout, &period_tick) != pdFALSE) {
//             vTaskSetTimeOutState(&timeout);
//             period_tick = pdMS_TO_TICKS(period_ms);

//             uint32_t t;
//             buffer.pushMeasurement({
//                 static_cast<uint32_t>(time(nullptr)),
//                 static_cast<uint16_t>((esp_random() % 20) + 5),
//                 static_cast<uint16_t>(esp_random() % 100)});
//         }
//     }
// }

extern "C" void app_main() {
    wifi::WiFiClient client{WIFI_SSID, WIFI_PASSWORD};
    client.connect();

    while (client.getStatus() != wifi::CONNECTED) {}
    vTaskDelay(pdMS_TO_TICKS(2000));

    gpio::EspGpioDriver driver;
    http::HttpClient::setDriver(new http::EspHttpDriver{});

    storage::BufferManager<storage::FlashBuffer> buffers;

    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
    esp_netif_sntp_init(&config);

    if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(10000)) != ESP_OK) {
        ESP_LOGI("SNTP", "Failed to update system time within 10s timeout");
    }

    time_t now;
    struct tm timeinfo;
    setenv("TZ", "CET", 1);
    tzset();

    time(&now);
    localtime_r(&now, &timeinfo);

    buffers.createBuffer({0});

    storage::Storage *buffer = buffers.getBuffer({0});
    for (int i = 0; i < 10; i++) {
        buffer->pushMeasurement({
            now,
            static_cast<uint16_t>((esp_random() % 20) + 5),
            static_cast<uint16_t>((esp_random() % 100))
        });
    }

    // Skapa buffer värden loop
    while (1) {
        // ESP_LOGI(http::HttpResponse::status);
        time(&now);
        // localtime_r(&now, &tm);

        if (!buffer->hasData()) continue;


        constexpr int elements_to_send = 3;

        JsonDocument payload;
        payload["gatewayUUID"] = "3fa85f64-5717-4562-b3fc-2c963f66afa6";
        payload["readings"]["batch_id"] = "batch-1";
        payload["readings"]["generated_at"] = now;
        payload["readings"]["sensors"].add(http::bufferToJson(buffer, elements_to_send));

        std::string data;
        convertFromJson(payload, data);
        ESP_LOGI("__JSON__", "%s", data.c_str());

        http::HttpResponse resp = http::HttpClient::getDriver()->performPostRequest(
            HTTP_API_HOST,
            HTTP_API_PORT,
            HTTP_API_SUBMIT_BATCH,
            {
                .data = data.c_str(),
                .headers = {
                    {
                        "Content-Type", "application/json"
                    }
                }
            }, false);

        if (resp.status == HttpStatus_Ok) {
            for (int i = 0; i < elements_to_send; i++) {
                buffer->tryPop();
            }
        }

        ESP_LOGI("__STATUS__", "%i", resp.status);
        ESP_LOGI("__DATA__", "%s", resp.data);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

#endif  // UNIT_TEST
