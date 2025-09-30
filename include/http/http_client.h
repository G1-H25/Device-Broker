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

#ifndef INCLUDE_HTTP_HTTP_CLIENT_H_
#define INCLUDE_HTTP_HTTP_CLIENT_H_

#include <esp_http_client.h>

#include <cstddef>
#include <string_view>
#include <optional>

#include "secrets/routes.h"

namespace http {

class HttpClient {
 public:
    HttpClient(std::string_view url, uint16_t port);

    void get(std::string_view endpoint);
    void post(std::string_view endpoint, std::string_view data, bool is_json);
    void put(std::string_view endpoint, std::string_view data, bool is_json);

 private:
    std::string_view url_;
    esp_http_client_handle_t client_handle_;
    esp_http_client_event_handle_t event_handle_;
};

}  // namespace http

#endif  // INCLUDE_HTTP_HTTP_CLIENT_H_
