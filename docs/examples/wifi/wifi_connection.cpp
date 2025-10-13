/**
 * @file wifi_connection.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-13
 *
 * @copyright Copyright (c) 2025
 *
 */


#include "wifi/wifi_client.h"

wifi::WiFiStatus exampleConnectToWiFi() {
    wifi::WiFiClient client{"SSID", "PASSWORD"};
    client.connect();

    // Delay to wait for connection to stabilize
    vTaskDelay(pdMS_TO_TICKS(2000));

    return client.getStatus();
}
