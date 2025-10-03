/**
 * @file main.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-30
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <unity.h>

#include "http/http_client.h"
#include "wifi/wifi_client.h"
#include "secrets/credentials.h"
#include "secrets/routes.h"

using http::HttpClient;
using http::HttpResponse;

void test_http_client_get() {
    HttpClient client{HTTP_API_HOST, HTTP_API_PORT};

    HttpResponse response = client.get("/");
    TEST_ASSERT_EQUAL(200, response.status);  // using current value to test if get works
}

void test_http_client_post() {
    HttpClient client{HTTP_API_HOST, HTTP_API_PORT};

    HttpResponse response = client.get("/");
    TEST_ASSERT_EQUAL(200, response.status);  // using current value to test if get works
}

extern "C" void app_main() {
    UNITY_BEGIN();

    wifi::WiFiClient client{WIFI_SSID, WIFI_PASSWORD};
    client.connect();

    RUN_TEST(test_http_client_get);
    RUN_TEST(test_http_client_post);

    UNITY_END();
}
