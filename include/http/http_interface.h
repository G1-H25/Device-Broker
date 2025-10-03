/**
 * @file http_interface.h
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef INCLUDE_HTTP_HTTP_INTERFACE_H_
#define INCLUDE_HTTP_HTTP_INTERFACE_H_

#ifdef ESP_PLATFORM

#include <esp_http_client.h>

#include <cstddef>
#include <string_view>
#include <optional>

namespace http {

class HttpClient {
 public:
    HttpClient(const std::string_view &url, uint16_t port);

    virtual void get(std::string_view endpoint) = 0;
    virtual void post(std::string_view endpoint, std::string_view data, bool is_json) = 0;
    virtual void put(std::string_view endpoint, std::string_view data, bool is_json) = 0;

 protected:
    uint16_t port_;
    std::string_view url_;
    esp_http_client_handle_t client_handle_;
    esp_http_client_event_handle_t event_handle_;
};

}  // namespace http

#endif  // ESP_PLATFORM

#endif  // INCLUDE_HTTP_HTTP_INTERFACE_H_
