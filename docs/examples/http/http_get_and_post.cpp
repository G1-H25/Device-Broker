/**
 * @file http_get_and_post.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-13
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <esp_log.h>

#include "http/http_esp_client_driver.h"
#include "http/http_client.h"

// HTTPS

int httpsGetRequestExample() {
    http::HttpResponse response = http::HttpClient::getDriver()->
        performGetRequest(
            "example.com",
            443,
            "/");

    // Print the status code and data
    ESP_LOGI(HTTP_LOG_TAG, "Status: %d, Data Length: %"PRId64,
            response.status, response.data.size());

    return 0;
}

int httpsPostRequestExample() {
    http::HttpResponse response = http::HttpClient::getDriver()->
        performPostRequest(
            "example.com",
            443,
            "/",
            {
                .data = "{\"message\": \"This is an example json string!\"}",
                .is_json = true
            });

    // Print the status code and data
    ESP_LOGI(HTTP_LOG_TAG, "Status: %d, Data Length: %"PRId64,
            response.status, response.data.size());

    return 0;
}

// HTTP

int httpsGetRequestExample() {
    http::HttpResponse response = http::HttpClient::getDriver()->
        performGetRequest(
            "example.com",
            443,
            "/",
            false);

    // Print the status code and data
    ESP_LOGI(HTTP_LOG_TAG, "Status: %d, Data Length: %"PRId64,
            response.status, response.data.size());

    return 0;
}

int httpsPostRequestExample() {
    http::HttpResponse response = http::HttpClient::getDriver()->
        performPostRequest(
            "example.com",
            443,
            "/",
            {
                .data = "{\"message\": \"This is an example json string!\"}",
                .is_json = true
            },
            false);

    // Print the status code and data
    ESP_LOGI(HTTP_LOG_TAG, "Status: %d, Data Length: %"PRId64,
            response.status, response.data.size());

    return 0;
}
