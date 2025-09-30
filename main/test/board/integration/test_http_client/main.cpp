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
#include "secrets/routes.h"

using http::HttpClient;

void test_http_client_get() {
    HttpClient client{HTTP_API_HOST, HTTP_API_PORT};

    std::string_view response = client.get("/");
    TEST_ASSERT_EQUAL(0, response.compare("get"));  // using current value to test if get works
}

void test_http_client_post() {
    HttpClient client{HTTP_API_HOST, HTTP_API_PORT};

    std::string_view response = client.get("/");
    TEST_ASSERT_EQUAL(0, response.compare("post"));  // using current value to test if get works
}

int main(int argc, char const *argv[]) {
    UNITY_BEGIN();

    RUN_TEST(test_http_client_get);
    RUN_TEST(test_http_client_post);

    return UNITY_END();
}
