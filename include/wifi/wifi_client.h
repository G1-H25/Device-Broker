/**
 * @file wifi_client.h
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-03
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef INCLUDE_WIFI_WIFI_CLIENT_H_
#define INCLUDE_WIFI_WIFI_CLIENT_H_
#ifdef ESP_PLATFORM

#include <esp_wifi.h>
#include <string_view>

namespace wifi {

class WiFiClient {
 public:
    WiFiClient(const std::string_view &ssid, const std::string_view &pass);
    ~WiFiClient();

    esp_err_t connect();
    esp_err_t disconnect();

 private:
    wifi_init_config_t init_conf_;
    wifi_config_t conf_;
};

}  // namespace wifi



#endif  // ESP_PLATFORM
#endif  // INCLUDE_WIFI_WIFI_CLIENT_H_
