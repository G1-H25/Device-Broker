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
#include "uuid/uuid.h"

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

JsonDocument bufferToJson(storage::Storage *buffer) {
    JsonDocument document;
    JsonDocument obj;

    document["sensor_id"] = uuid::MyUuid<uuid::UuidVersion::NAME_BASED_MD5>(
        buffer->getSensorId()).to_string().c_str();

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
