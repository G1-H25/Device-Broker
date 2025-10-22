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

#include <esp32s3/rom/gpio.h>
#include <esp_rom_gpio.h>
#include <soc/io_mux_reg.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <string_view>
#include <string>
#include <vector>
#include <ArduinoJson.h>
#include "http/http_client.h"
#include "http/http_driver.h"
#include "http/http_esp_client_driver.h"
#include "storage/buffer_manager.h"
#include "storage/flash_buffer.h"
#include "gpio/esp_gpio_driver.h"
#include "gpio/button.h"
#include "secrets/routes.h"

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

/**
 * @brief Get the Sensor UUIDs from backend server
 *
 * @returns A json document with containing
 * all uuids that are registered to the broker.
 */
JsonDocument getSensorUUIDs() {
    http::http_response_t resp = http::HttpClient::getDriver()->
        performGetRequest(
            HTTP_API_HOST,
            HTTP_API_PORT,
            HTTP_API_SYNC_SENSORS,
            true);

    JsonDocument document;
    if (resp.status != 200) return document;

    convertToJson(resp.data, document);
    return document;
}

/**
 * @brief Converts a stored buffer into a JsonArray.
 *
 * @param buffers A reference to a buffermanager containing all available buffers
 * @param sensor_id_t The sensor to convert buffered values into json.
 * @returns A JsonArray containing all buffered data.
 */
JsonArray bufferToJson(const storage::BufferManager<storage::FlashBuffer> &buffers, storage::sensor_id_t sensor_id) {
    JsonArray document;
    JsonObject obj;

    FlashBuffer *buffer = buffers.getBuffer(sensor_id);

    for (int i = 0; i < buffer->available(); i++) {
        storage::MeasurementEntry entry;

        if (!buffer->loadMeasurement(i, entry))
            continue;

        obj[HTTP_API_JSON_TIME_KEY] = entry.timestamp;
        obj[HTTP_API_JSON_TEMP_KEY] = entry.temperature;
        obj[HTTP_API_JSON_HUM_KEY] = entry.humidity;

        document.add(obj);
    }
}

/**
 * @brief Iterates and sends all buffered sensor data.
 *
 * @param buffers Buffer manager where all buffers are stored.
 * @returns A vector containing all failed sensor uuids that could not be sent.
 */
template<typename T>
std::vector<storage::sensor_id_t> sendAllBuffers(storage::BufferManager<T> &buffers) {
    static_assert(std::is_base_of<storage::Storage, T>(), "T does not derive from storage::Storage class");

    JsonDocument document;
    JsonArray temp;
    std::vector<storage::sensor_id_t> failed;

    for (storage::sensor_id_t i : buffers.getBufferUUIDs()) {
        document[i] = bufferToJson(buffers, i);

        std::string payload;
        convertFromJson(document, payload);

        http::http_response_t resp = http::HttpClient::getDriver()->
            performPostRequest(
                HTTP_API_HOST,
                HTTP_API_PORT,
                HTTP_API_POST_MEASUREMENT,
                {.data = payload, .is_json = true});

        document.clear();
        if (resp.status == 201) {
            buffers.clearBuffer(i);
        } else {
            failed.push_back(i);
        }
    }

    return failed;
}

void createMockSensor(FlashBuffer buffer, storage::sensor_id_t uuid) {
    int period_ms = 1000;
    TimeOut_t timeout;
    TickType_t period_tick = pdMS_TO_TICKS(period_ms);
    vTaskSetTimeOutState(&timeout);

    for (;;) {
        if (xTaskCheckForTimeOut(&timeout, &period_tick) != pdFALSE) {
            vTaskSetTimeOutState(&timeout);
            period_tick = pdMS_TO_TICKS(period_ms);

            uint32_t t;
            buffer.pushMeasurement({
                static_cast<uint32_t>(time(nullptr)),
                static_cast<uint16_t>((esp_random() % 20) + 5),
                static_cast<uint16_t>(esp_random() % 100)});
        }
    }
}

void app_main() {
    gpio::EspGpioDriver driver;
    http::HttpClient::setDriver(new http::EspHttpDriver{});

    gpio::Button button {1, [](const Event &event) {
        std::vector<storage::sensor_id_t> uuids;
    }};

    delete http::HttpClient::getDriver();
}

#endif
