/**
 * @file sensor_data_sender.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <type_traits>
#include <vector>
#include <string>
#include <cstdio>

#include "http/sensor_data_sender.h"
#include "secrets/routes.h"

#ifdef ESP_PLATFORM

#include "http/http_client.h"

namespace http {

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

template<typename T>
std::vector<storage::sensor_id_t> sendAllBuffers(storage::BufferManager<T> &buffers) {
    JsonDocument document;
    JsonArray temp;
    std::vector<storage::sensor_id_t> failed;

    for (storage::sensor_id_t i : buffers.getBufferIds()) {
        document["sensors"].add(bufferToJson(buffers.getBuffer(i)));

        std::string payload;
        convertFromJson(document, payload);

        http::http_response_t resp = http::HttpClient::getDriver()->
            performPostRequest(
                HTTP_API_HOST,
                HTTP_API_PORT,
                HTTP_API_SUBMIT_BATCH,
                {.data = payload});

        document.clear();
        if (resp.status == 201) {
            buffers.clearBuffer(i);
        } else {
            failed.push_back(i);
        }
    }

    return failed;
}

}  // namespace http

#endif  // ESP_PLATFORM

namespace http {

std::string_view uuidFromInt(storage::sensor_id_t sensor_id) {
    storage::uuid_t uuid = { 0 };

    for (int i = 0; i < uuid.size(); i++) {
        uuid[i] = (~0 ^ (sensor_id ^ i));
    }

    uuid[6] |= 0b01001111;

    constexpr uint8_t bufferSize = 37;
    char buffer[bufferSize] = { 0 };

    snprintf(buffer, bufferSize,
        "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        uuid[0], uuid[1], uuid[2], uuid[3], uuid[4],
        uuid[5], uuid[6], uuid[7], uuid[8], uuid[9],
        uuid[10], uuid[11], uuid[12], uuid[13],
        uuid[14], uuid[15]);

    return buffer;
}

JsonDocument bufferToJson(storage::Storage *buffer) {
    JsonDocument document;
    JsonDocument obj;

    document["sensor_id"] = 10;

    for (int i = 0; i < buffer->available(); i++) {
        storage::MeasurementEntry entry;

        if (!buffer->getMeasurement(entry, i))
            continue;

        obj[HTTP_API_JSON_TIME_KEY] = entry.timestamp;
        obj[HTTP_API_JSON_TEMP_KEY] = entry.temperature;
        obj[HTTP_API_JSON_HUM_KEY] = entry.humidity;

        document["measurements"].add(obj);
    }

    return document;
}

}  // namespace http
