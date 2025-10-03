/**
 * @file main.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-03
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <unity.h>
#include "secrets/routes.h"
#include "secrets/credentials.h"
#include "http/esp32_http_client.h"

using http::EspHttpClient;

void test_http_get() {
    EspHttpClient client("test", 80);
}

extern "C" void app_main() {
    UNITY_BEGIN();

    UNITY_END();
}
