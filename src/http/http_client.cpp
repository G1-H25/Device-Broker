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
#include "http/http_client.h"

namespace http {

HttpClient::HttpClient(std::string_view host, uint16_t port)
    : port_(port), host_(host) {

    this->conf.host = host.begin();
}

HttpResponse HttpClient::get(std::string_view endpoint) {
    std::stringstream stream;
    stream << "http://" << host_ << endpoint;

    conf.url = stream.str().c_str();
    conf.method = HTTP_METHOD_GET;

    this->client_handle_ = esp_http_client_init(&conf);

    esp_http_client_perform(client_handle_);

    HttpResponse response {
        .status = esp_http_client_get_status_code(this->client_handle_),
        .data = std::string_view(this->response_buffer.begin()),
    };

    esp_http_client_cleanup(this->client_handle_);
    return response;
}

HttpResponse HttpClient::post(std::string_view endpoint, std::string_view data, bool is_json) {
    std::stringstream stream;
    stream << host_ << endpoint;

    conf.url = stream.str().c_str();
    conf.method = HTTP_METHOD_POST;

    this->client_handle_ = esp_http_client_init(&conf);

    esp_http_client_set_header(this->client_handle_, "Content-Type", is_json ? "application/json" : "application/text");
    esp_http_client_set_post_field(this->client_handle_, data.begin(), data.size());

    esp_http_client_set_url(this->client_handle_, stream.str().c_str());

    esp_http_client_perform(this->client_handle_);

    HttpResponse response {
        .status = esp_http_client_get_status_code(this->client_handle_),
        .data = std::string_view(this->response_buffer.begin()),
    };

    esp_http_client_cleanup(this->client_handle_);

    return response;
}

esp_err_t HttpClient::event_handler(esp_http_client_event_t *event) {
    HttpClient* httpClient = static_cast<HttpClient*>(event->user_data);

    switch (event->event_id) {
        case HTTP_EVENT_ON_DATA:
            if (!esp_http_client_is_chunked_response(event->client)) {
                const uint32_t strLen = event->data_len > httpClient->response_buffer.size() - 1 ?
                    httpClient->response_buffer.size() : event->data_len;
                strncpy(
                    httpClient->response_buffer.begin(),
                    reinterpret_cast<char*>(event->data),
                    strLen);

                httpClient->response_buffer[strLen] = 0;
            }
            break;
        default:
            break;
    }
    return ESP_OK;
}

}  // namespace http
