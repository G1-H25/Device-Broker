/**
 * @file http_interface.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-24
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifdef ESP_PLATFORM

#include <esp_http_client.h>
#include <esp_tls.h>
#include <esp_log.h>
#include <esp_netif.h>
#include <http_parser.h>

#include <memory>
#include <sstream>

#include "http/http_client.h"

namespace http {

HttpDriver *HttpClient::getDriver() {
    return HttpClient::driver;
}

/**
 * @brief Sets current HttpDriver to defined driver.
 *
 * @param driver Ensure that this variables is allocated on the stack or
 * does not go out of scope in case it is allocated on the stack. Make
 * sure to free this variable when it is no longer going to be used when it
 * is on the heap.
 */
void HttpClient::setDriver(HttpDriver *driver) noexcept {
    HttpClient::driver = driver;
}

HttpDriver *HttpClient::driver = nullptr;

}  // namespace http

#endif  // ESP_PLATFORM
