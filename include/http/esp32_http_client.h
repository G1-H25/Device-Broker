/**
 * @file esp32_http_client.h
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-03
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef INCLUDE_HTTP_ESP32_HTTP_CLIENT_H_
#define INCLUDE_HTTP_ESP32_HTTP_CLIENT_H_

#ifdef ESP_PLATFORM

#include "http/http_interface.h"

namespace http {

class EspHttpClient : public HttpClient {
 public:
    EspHttpClient(const std::string_view &url, uint16_t port);

    void get(std::string_view endpoint) override;
    void post(std::string_view endpoint, std::string_view data, bool is_json) override;
    void put(std::string_view endpoint, std::string_view data, bool is_json) override;
};

}  // namespace http

#endif  // ESP_PLATFORM

#endif  // INCLUDE_HTTP_ESP32_HTTP_CLIENT_H_
