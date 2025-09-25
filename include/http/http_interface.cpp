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

namespace http {

void HttpClient::get(std::string_view endpoint) {
    std::stringstream stream;
    stream << url_ << endpoint;

    esp_http_client_config_t conf = {
        .url = stream.str().c_str(),
        .method = HTTP_METHOD_GET
    };

    this->client_handle_ = esp_http_client_init(&conf);

    esp_http_client_perform(client_handle_);

    esp_http_client_cleanup(this->client_handle_);
}

void HttpClient::post(std::string_view endpoint, std::string_view data, bool is_json) {
    std::stringstream stream;
    stream << url_ << endpoint;

    esp_http_client_config_t conf = {
        .url = stream.str().c_str(),
        .method = HTTP_METHOD_POST
    };

    this->client_handle_ = esp_http_client_init(&conf);

    esp_http_client_set_header(this->client_handle_, "Content-Type", is_json ? "application/json" : "application/text");
    esp_http_client_set_post_field(this->client_handle_, data.begin(), data.size());

    esp_http_client_perform(client_handle_);

    esp_http_client_cleanup(this->client_handle_);
}


void HttpClient::put(std::string_view endpoint, std::string_view data, bool is_json) {
    std::stringstream stream;
    stream << url_ << endpoint;

    esp_http_client_config_t conf = {
        .url = stream.str().c_str(),
        .method = HTTP_METHOD_PUT
    };

    this->client_handle_ = esp_http_client_init(&conf);

    esp_http_client_set_header(this->client_handle_, "Content-Type", is_json ? "application/json" : "application/text");
    esp_http_client_set_post_field(this->client_handle_, data.begin(), data.size());

    esp_http_client_perform(client_handle_);

    esp_http_client_cleanup(this->client_handle_);
}

}  // namespace http
