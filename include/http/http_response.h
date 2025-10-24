/**
 * @file http_response.h
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef INCLUDE_HTTP_HTTP_RESPONSE_H_
#define INCLUDE_HTTP_HTTP_RESPONSE_H_

#include <string>

namespace http {

typedef struct http_response_t {
    int status;
    std::string data;
} HttpResponse;

}  // namespace http

#endif  // INCLUDE_HTTP_HTTP_RESPONSE_H_
