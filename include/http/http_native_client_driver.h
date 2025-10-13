/**
 * @file http_native_client_driver.h
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef INCLUDE_HTTP_HTTP_NATIVE_CLIENT_DRIVER_H_
#define INCLUDE_HTTP_HTTP_NATIVE_CLIENT_DRIVER_H_

#include "http/http_driver.h"

namespace http {

class NativeHttpDriver : public HttpDriver {
 public:
    HttpResponse performGetRequest(
        const std::string_view &host,
        uint16_t port,
        const std::string_view &endpoint) override;

    HttpResponse performPostRequest(
        const std::string_view &host,
        uint16_t port,
        const std::string_view &endpoint,
        const HttpRequest &req) override;
};

}  // namespace http

#endif  // INCLUDE_HTTP_HTTP_NATIVE_CLIENT_DRIVER_H_
