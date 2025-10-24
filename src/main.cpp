/**
 * @file main.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-16
 *
 * @copyright Copyright (c) 2025
*/

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

    // gpio::Button button {1, [](const Event &event) {
    //     std::vector<storage::uuid_t> uuids;
    // }};

        // init NVS + TCP/IP stack (assume Wi‑Fi already connected)
    // esp_err_t r = nvs_flash_init();
    // if (r == ESP_ERR_NVS_NO_FREE_PAGES || r == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    //     nvs_flash_erase();
    //     nvs_flash_init();
    // }
    // esp_netif_init();
    // esp_event_loop_create_default();

    // // optional: set timezone (UTC here)
    // setenv("TZ", "UTC2", 1);
    // tzset();

    // // init SNTP
    // sntp_setoperatingmode(SNTP_OPMODE_POLL);
    // sntp_setservername(0, "pool.ntp.org"); // or "time.google.com"
    // sntp_init();

    // // wait for sync (simple loop)
    time_t now = 0;
    // struct tm tm = {0};
    // int retries = 0;
    // while (tm.tm_year < (2020 - 1900) && retries++ < 10) {
    //     vTaskDelay(pdMS_TO_TICKS(1000));
    //     time(&now);
    //     localtime_r(&now, &tm);
    // }

    // if (tm.tm_year >= (2020 - 1900)) {
    //     char buf[64];
    //     strftime(buf, sizeof(buf), "%c", &tm);
    //     ESP_LOGI(TAG, "Time synced: %s", buf);
    // } else {
    //     ESP_LOGW(TAG, "SNTP sync failed");
    // }

    buffers.createBuffer({0}, 0);

    // Skapa buffer värden loop
    while (1) {
        // ESP_LOGI(http::HttpResponse::status);
        time(&now);
        // localtime_r(&now, &tm);

        storage::Storage *buffer = buffers.getBuffer({0});
        for (int i = 0; i < 10; i++) {
            buffer->pushMeasurement({
                now,
                static_cast<uint16_t>((esp_random() % 20) + 5),
                static_cast<uint16_t>((esp_random() % 100))
            });
        }

        JsonDocument payload;
        payload["batch_id"] = "asdf";
        payload["generated_at"] = now;
        payload["sensors"].add(http::bufferToJson(buffer));


        std::string data;
        convertFromJson(payload, data);
        ESP_LOGI("__JSON__", "%s", data.c_str());

        // http::HttpResponse resp = http::HttpClient::getDriver()->performGetRequest(
        //     HTTP_TEST_API_HOST,
        //     HTTP_TEST_API_PORT,
        // "/");

        http::HttpResponse resp = http::HttpClient::getDriver()->performPostRequest(
            HTTP_TEST_API_HOST,
            HTTP_TEST_API_PORT,
            HTTP_API_SUBMIT_BATCH,
            {
                .data = data,
                .headers = {
                    {
                        "Content-Type", "application/json"
                    }
                }
            });

        ESP_LOGI("__STATUS__", "%i", resp.status);
        ESP_LOGI("__DATA__", "%s", resp.data.begin());

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
