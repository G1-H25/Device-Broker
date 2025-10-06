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

#define HTTP_RESPONSE_BUFFER_SIZE 1024

#include <esp_http_client.h>

#include <array>
#include <cstddef>
#include <string_view>
#include <string>
#include <optional>

namespace http {

typedef struct http_response_t {
    int status;
    std::string_view data;
} HttpResponse;

class HttpClient {
 public:
    static HttpResponse get(std::string_view host, std::string_view endpoint);
    static HttpResponse post(std::string_view host, std::string_view endpoint, std::string_view data, bool is_json);

 private:
    static esp_err_t event_handler(esp_http_client_event_t *event);
};


}  // namespace http

#endif  // INCLUDE_HTTP_HTTP_CLIENT_H_
