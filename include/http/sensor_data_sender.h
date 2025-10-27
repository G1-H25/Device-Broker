/**
 * @file sensor_data_sender.h
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef INCLUDE_HTTP_SENSOR_DATA_SENDER_H_
#define INCLUDE_HTTP_SENSOR_DATA_SENDER_H_

#include <ArduinoJson.h>

#include <vector>
#include <string>

#include "storage/storage.h"
#include "storage/buffer_manager.h"

namespace http {

/**
 * @brief Get the Sensor UUIDs from backend server
 *
 * @returns A json document with containing
 * all uuids that are registered to the broker.
 */
JsonDocument getSensorUUIDs();

/**
 * @brief Converts a stored buffer into a JsonArray.
 *
 * @param buffers A reference to a buffermanager containing all available buffers
 * @param uuid_t The sensor to convert buffered values into json.
 * @returns A JsonArray containing all buffered data.
 *
 * The keys below are defined in secrets/routes.h
 *
 * @code { .json }
[
    {
        HTTP_API_JSON_TEMP_KEY:  uint16_t,
        HTTP_API_JSON_HUM_KEY:   uint16_t,
        HTTP_API_JSON_TIME_KEY:  uint32_t
    }
]
 * @endcode
 *
 */
JsonDocument bufferToJson(storage::Storage *buffer);

/**
 * @brief Iterates and sends all buffered sensor data.
 *
 * @param batch_id Id of the batch
 * @param generated_at The timestamp when the request was generated.
 * @returns A vector containing all failed sensor uuids that could not be sent.
 */
const char *prepareRequest(const char *batch_id, uint32_t generated_at, storage::Storage *buffer);

}  // namespace http


#endif  // INCLUDE_HTTP_SENSOR_DATA_SENDER_H_
