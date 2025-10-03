/**
 * @file http_interface.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-03
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifdef ESP_PLATFORM

#include "http/http_interface.h"

namespace http {

HttpClient::HttpClient(const std::string_view &url, uint16_t port) : port_(port), url_(url) {
}

}  // namespace http


#endif  // ESP_PLATFORM
