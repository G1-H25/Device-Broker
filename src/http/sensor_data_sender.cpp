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
std::vector<storage::uuid_t> sendAllBuffers(storage::BufferManager<T> &buffers) {
    JsonDocument document;
    JsonArray temp;
    std::vector<storage::uuid_t> failed;

    for (storage::uuid_t i : buffers.getBufferUUIDs()) {
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

std::string_view uuidToString(const storage::uuid_t &sensor_id) {
    constexpr uint8_t bufferSize = 37;
    char buffer[bufferSize] = { 0 };

    snprintf(buffer, bufferSize,
        "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        sensor_id[0], sensor_id[1], sensor_id[2], sensor_id[3], sensor_id[4],
        sensor_id[5], sensor_id[6], sensor_id[7], sensor_id[8], sensor_id[9],
        sensor_id[10], sensor_id[11], sensor_id[12], sensor_id[13],
        sensor_id[14], sensor_id[15]);

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
