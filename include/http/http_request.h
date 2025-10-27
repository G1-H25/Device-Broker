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

#ifndef INCLUDE_HTTP_HTTP_REQUEST_H_
#define INCLUDE_HTTP_HTTP_REQUEST_H_

#include <string>
#include <vector>

typedef struct http_header_t {
    std::string name;
    std::string value;
} HttpHeader;

typedef struct http_request_t {
    std::string data = "";
    std::vector<HttpHeader> headers = {};
    std::string token = "";
} HttpRequest;

#endif  // INCLUDE_HTTP_HTTP_REQUEST_H_
