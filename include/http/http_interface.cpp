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

#include <esp_http_client.h>
#include <sstream>
#include "http/http_interface.h"

void http::HttpClient::getRequest(std::string_view endpoint) {
    std::stringstream stream;
    stream << url_ << endpoint;

    esp_http_client_config_t conf = {
        .url = stream.str().c_str(),
        .use_global_ca_store = true
    };

    esp_http_client_init(&conf);
    esp_http_client_set_method(this->client_handle_, esp_http_client_method_t::HTTP_METHOD_GET);
    esp_http_client_set_authtype(this->client_handle_, esp_http_client_auth_type_t::HTTP_AUTH_TYPE_NONE);
    esp_http_client_set_url(this->client_handle_, stream.str().c_str());

    esp_http_client_open(this->client_handle_, 0);
    esp_http_client_close(this->client_handle_);

    esp_http_client_cleanup(this->client_handle_);
}

void http::HttpClient::postRequest(std::string_view endpoint, std::string_view data, bool is_json) {
}

void http::HttpClient::putRequest(std::string_view endpoint, std::string_view data, bool is_json) {
}
