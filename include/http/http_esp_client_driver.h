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

namespace http {

class EspHttpDriver : public HttpDriver {
 public:
    EspHttpDriver();
    ~EspHttpDriver();

    HttpResponse performGetRequest(
        const std::string_view &host,
        uint16_t port,
        const std::string_view &endpoint) override;

    HttpResponse performPostRequest(
        const std::string_view &host,
        uint16_t port,
        const std::string_view &endpoint,
        const HttpRequest &req) override;

 protected:
    esp_http_client_config_t *config_;

 private:
    static esp_err_t event_handler(esp_http_client_event_t *event);

    esp_http_client_handle_t handle_;
};

}  // namespace http

#endif  // INCLUDE_HTTP_HTTP_ESP_CLIENT_DRIVER_H_
