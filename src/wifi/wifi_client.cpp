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
#ifdef ESP_PLATFORM

#include <esp_log.h>
#include <esp_err.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_netif.h>
#include <nvs_flash.h>
#include <lwip/inet.h>
#include <esp_system.h>

#include <string_view>
#include <cstring>

#include "secrets/credentials.h"

#include "wifi/wifi_client.h"
#include "storage/flash_buffer.h"

#define LOG_TAG_WIFI "wifi"

namespace wifi {


// adjust tag as desired
static const char *TAG = "WiFiClient";

WiFiClient::WiFiClient(const std::string_view &ssid, const std::string_view &pass)
    : retry_count_(0), connected_(false), failed_(false) {
    storage::FlashBuffer::tryInitNVS();

    this->init_conf_ = reinterpret_cast<wifi_init_config_t *>
        (malloc(sizeof(*this->init_conf_)));
    this->conf_ = reinterpret_cast<wifi_config_t *>
        (malloc(sizeof(*this->conf_)));

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *netif = esp_netif_create_default_wifi_sta();

    // Init wifi config
    *init_conf_ = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(init_conf_));

    // register event handlers
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &WiFiClient::eventHandlerStatic,
                                                        this,
                                                        nullptr));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &WiFiClient::eventHandlerStatic,
                                                        this,
                                                        nullptr));

    // validate lengths
    if (ssid.size() > sizeof(conf_->sta.ssid) - 1) {
        ESP_LOGE(TAG, "SSID too long");
        failed_ = true;
        return;
    }

    if (pass.size() > sizeof(conf_->sta.password) - 1) {
        ESP_LOGE(TAG, "Password too long");
        failed_ = true;
        return;
    }

    memset(conf_, 0, sizeof(*conf_));
    strncpy(reinterpret_cast<char*>(conf_->sta.ssid), ssid.data(), ssid.size());
    strncpy(reinterpret_cast<char*>(conf_->sta.password), pass.data(), pass.size());

    conf_->sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    conf_->sta.pmf_cfg.required = false;
    conf_->sta.sae_pwe_h2e = wifi_sae_pwe_method_t::WPA3_SAE_PWE_UNSPECIFIED;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, conf_));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_start());
}

WiFiClient::~WiFiClient() {
    free(this->conf_);
    free(this->init_conf_);

    esp_wifi_disconnect();
    esp_wifi_stop();
    esp_wifi_deinit();
}

esp_err_t WiFiClient::connect() {
    if (failed_) return ESP_FAIL;

    esp_err_t err = esp_wifi_connect();
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "esp_wifi_connect returned %s", esp_err_to_name(err));
        connected_ = false;
        retry_count_ = 0;
    }

    return err;
}

esp_err_t WiFiClient::disconnect() {
    return esp_wifi_disconnect();
}

void WiFiClient::eventHandlerStatic(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data) {
    WiFiClient *self = reinterpret_cast<WiFiClient*>(arg);
    if (self) self->eventHandler(event_base, event_id, event_data);
}

void WiFiClient::eventHandler(esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_CONNECTED: {
                this->status_ = CONNECTED;
                break;
            }
            case WIFI_EVENT_STA_DISCONNECTED: {
                this->status_ = NOT_CONNECTED;
                ESP_LOGI(TAG, "WIFI_EVENT_STA_DISCONNECTED");
                if (retry_count_ < 5) {
                    retry_count_++;
                    esp_wifi_connect();
                    ESP_LOGI(TAG, "retrying connect (%d)", retry_count_);
                } else {
                    failed_ = true;
                    ESP_LOGE(TAG, "failed to connect after retries");
                }
                break;
            }
            case WIFI_EVENT_STA_START: {
                this->status_ = CONNECTING;
                ESP_LOGI(TAG, "WIFI_EVENT_STA_START -> connecting");
                esp_wifi_connect();
                break;
            }
            default:
                break;
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* got_ip = static_cast<ip_event_got_ip_t*>(event_data);
        ESP_LOGI(TAG, "GOT IP: " IPSTR, IP2STR(&got_ip->ip_info.ip));
        connected_ = true;
        retry_count_ = 0;
    }
}

WiFiStatus WiFiClient::getStatus() {
    return this->status_;
}


}  // namespace wifi

#endif  // ESP_PLATFORM
