/**
 * @file http_client.h
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef INCLUDE_HTTP_HTTP_CLIENT_H_
#define INCLUDE_HTTP_HTTP_CLIENT_H_

#define HTTP_RESPONSE_BUFFER_SIZE 1024

#include <esp_http_client.h>

#include <array>
#include <cstddef>
#include <string_view>
#include <string>
#include <optional>

#include "http/http_driver.h"

namespace http {

/**
 * @brief The HTTP client class is a class that manages the
 * http driver which will be used to perform requests.
 *
 * All member methods are static. This class cannot be
 * instantiated.
 */
class HttpClient {
 public:
    static HttpDriver *getDriver();
    static void setDriver(HttpDriver *driver) noexcept;

    HttpClient() = delete;
 private:
    static HttpDriver *driver;
};

}  // namespace http

#endif  // INCLUDE_HTTP_HTTP_CLIENT_H_
