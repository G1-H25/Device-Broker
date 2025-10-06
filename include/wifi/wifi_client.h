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

enum WiFiStatus {
    CONNECTED,
    CONNECTING,
    FAILED,
    NOT_CONNECTED
};

class WiFiClient {
 public:
    WiFiClient(const std::string_view &ssid, const std::string_view &pass);
    ~WiFiClient();

    esp_err_t connect();
    esp_err_t disconnect();

    static void eventHandlerStatic(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data);
    void eventHandler(esp_event_base_t event_base, int32_t event_id, void* event_data);

    WiFiStatus getStatus();
 private:
    WiFiStatus status_;

    wifi_init_config_t *init_conf_;
    wifi_config_t *conf_;

    int retry_count_ = 0;
    bool connected_ = false;
    bool failed_ = false;
};

}  // namespace wifi



#endif  // ESP_PLATFORM
#endif  // INCLUDE_WIFI_WIFI_CLIENT_H_
