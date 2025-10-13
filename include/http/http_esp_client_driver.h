/**
 * @file http_esp_client_driver.h
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef INCLUDE_HTTP_HTTP_ESP_CLIENT_DRIVER_H_
#define INCLUDE_HTTP_HTTP_ESP_CLIENT_DRIVER_H_

#include "http/http_driver.h"

#define MAX_HTTP_OUTPUT_BUFFER 256

#define HTTP_LOG_TAG "HTTP_CLIENT"

namespace http {

/**
 * @brief This class is a wrapper for the `esp_http_client` library.
 * Inherits from `HttpDriver` class
 *
 * @implements HttpDriver
 *
 */
class EspHttpDriver : public HttpDriver {
 public:
    EspHttpDriver();
    ~EspHttpDriver();

    HttpResponse performGetRequest(
        const std::string_view &host,
        uint16_t port,
        const std::string_view &endpoint,
        bool use_https = true) override;

    HttpResponse performPostRequest(
        const std::string_view &host,
        uint16_t port,
        const std::string_view &endpoint,
        const HttpRequest &req,
        bool use_https = true) override;

 protected:
    esp_http_client_config_t *config_;

 private:
    static esp_err_t event_handler(esp_http_client_event_t *event);

    esp_http_client_handle_t handle_;

    char *response_buffer = new char[MAX_HTTP_OUTPUT_BUFFER + 1];
};

}  // namespace http

#endif  // INCLUDE_HTTP_HTTP_ESP_CLIENT_DRIVER_H_
