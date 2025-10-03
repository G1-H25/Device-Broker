/**
 * @file wifi_client.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-03
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <esp_log.h>
#include <memory.h>

#include "wifi/wifi_client.h"
#include "storage/flash_buffer.h"

#define LOG_TAG_WIFI "wifi"

namespace wifi {

WiFiClient::WiFiClient(const std::string_view &ssid, const std::string_view &pass) {
    storage::FlashBuffer::tryInitNVS();

    if (ssid.size() > 32) return;
    if (pass.size() > 64) return;

    this->init_conf_ = WIFI_INIT_CONFIG_DEFAULT();

    memcpy(this->conf_.sta.ssid, ssid.begin(), ssid.size());
    memcpy(this->conf_.sta.password, pass.begin(), pass.size());

    ESP_ERROR_CHECK(esp_wifi_init(&this->init_conf_));

    this->conf_.sta.sae_pwe_h2e = wifi_sae_pwe_method_t::WPA3_SAE_PWE_UNSPECIFIED;
    this->conf_.sta.threshold.authmode = wifi_auth_mode_t::WIFI_AUTH_WPA2_PSK;
    esp_wifi_set_config(wifi_interface_t::WIFI_IF_STA, &this->conf_);

    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

    ESP_ERROR_CHECK(esp_wifi_start());
}

WiFiClient::~WiFiClient() {
    // esp_wifi_deinit();
}

esp_err_t WiFiClient::connect() {
    return esp_wifi_connect();
}

esp_err_t WiFiClient::disconnect() {
    return esp_wifi_disconnect();
}

}  // namespace wifi

#endif  // ESP_PLATFORM
