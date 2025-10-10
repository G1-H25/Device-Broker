/**
 * @file http_driver.h
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef INCLUDE_HTTP_HTTP_DRIVER_H_
#define INCLUDE_HTTP_HTTP_DRIVER_H_

#include <esp_http_client.h>

#include "http/http_response.h"
#include "http/http_request.h"

namespace http {

/**
 * @brief The HTTP driver is an abstract class that needs to
 * be implemented to match target platform. For example ESP32.
 */
class HttpDriver {
 public:
    virtual HttpResponse performGetRequest(
        const std::string_view &host,
        uint16_t port,
        const std::string_view &endpoint) = 0;

    virtual HttpResponse performPostRequest(
        const std::string_view &host,
        uint16_t port,
        const std::string_view &endpoint,
        const HttpRequest &request) = 0;
};

}  // namespace http


#endif  // INCLUDE_HTTP_HTTP_DRIVER_H_
