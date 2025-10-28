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
#include <cstdarg>
#include <utility>

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

}  // namespace http

#endif  // ESP_PLATFORM

namespace http {

JsonDocument bufferToJson(storage::Storage *buffer, int max_elements) {
    JsonDocument document;
    JsonDocument obj;

    document["sensor_id"] = uuid::MyUuid<uuid::UuidVersion::NAME_BASED_MD5>(
        buffer->getSensorId()).to_string().c_str();

    int loops = max_elements == -1 ? max_elements = buffer->available() : max_elements;
    for (int i = 0; i < loops; i++) {
        storage::MeasurementEntry entry;

        if (!buffer->getMeasurement(entry, i))
            continue;

        obj[HTTP_API_JSON_HUM_KEY] = entry.humidity;
        obj[HTTP_API_JSON_TEMP_KEY] = entry.temperature;
        obj[HTTP_API_JSON_TIME_KEY] = entry.timestamp;

        document["measurements"].add(std::move(obj));
        obj = JsonObject();
    }

    return document;
}

const char *prepareRequest(
        const char *batch_id,
        uint32_t generated_at,
        storage::Storage *buffer,
        int max_elements) {
    JsonDocument payload;

    payload["batch_id"] = batch_id;
    payload["generated_at"] = generated_at;

    payload["sensors"].add(std::move(bufferToJson(buffer, max_elements)));

    std::string str;
    convertFromJson(payload, str);
    return str.c_str();
}

}  // namespace http
