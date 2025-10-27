/**
 * @file http_esp_client_driver.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-07
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifdef ESP_PLATFORM

#include <esp_log.h>
#include <esp_netif.h>
#include <sys/param.h>
#include <esp_crt_bundle.h>
#include <esp_tls.h>

#include "http/http_esp_client_driver.h"

namespace http {

EspHttpDriver::EspHttpDriver() {
    this->config_ = new esp_http_client_config_t{};
}

EspHttpDriver::~EspHttpDriver() {
    delete this->config_;
    delete this->response_buffer;
}

/**
 * @brief Performs a post request to the specified host and port.
 *
 * @param host The host to get from. For example `archlinux.org`
 * @param port Which port to get from. For HTTPS it should be port `443` unless it has been changed by host machine
 * @param endpoint Which endpoint in the host to get from. For example `/` or `/api/measurement`
 * @return HttpResponse Contains data and status code.
 */
HttpResponse EspHttpDriver::performGetRequest(
                const std::string_view &host,
                uint16_t port,
                const std::string_view &endpoint,
                bool use_https ) {
    memset(this->response_buffer, 0, MAX_HTTP_OUTPUT_BUFFER + 1);

    *config_ = {
        .host = host.begin(),
        .port = port,
        .path = endpoint.begin(),
        .disable_auto_redirect = true,
        .event_handler = event_handler,
        .transport_type = use_https ? HTTP_TRANSPORT_OVER_SSL : HTTP_TRANSPORT_OVER_TCP,
        .user_data = response_buffer,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };

    esp_netif_t *netif = esp_netif_get_default_netif();
    esp_netif_ip_info_t info;
    esp_netif_get_ip_info(netif, &info);

    ESP_LOGI("HTTP_IP", "%lli", info.ip.addr);

    esp_http_client_handle_t client;
    client = esp_http_client_init(config_);

    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(HTTP_LOG_TAG, "HTTP GET Status = %d, content_length = %" PRId64,
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(HTTP_LOG_TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }
    ESP_LOG_BUFFER_HEX(HTTP_LOG_TAG, response_buffer, strlen(response_buffer));

    int status = esp_http_client_get_status_code(client);

    esp_http_client_cleanup(client);

    return HttpResponse {
        .status = status,
        .data = response_buffer,
    };
}

/**
 * @brief Performs a post request to the specified host and port.
 *
 * @param host The host to post to. For example `archlinux.org`
 * @param port Which port to post to. For HTTPS it should be port `443` unless it has been changed by host machine
 * @param endpoint Which endpoint in the host to post to. For example `/` or `/api/measurement`
 * @param req Data to send
 * @return HttpResponse Contains data and status code.
 */
HttpResponse EspHttpDriver::performPostRequest(
                const std::string_view &host,
                uint16_t port,
                const std::string_view &endpoint,
                const HttpRequest &req,
                bool use_https ) {
    memset(this->response_buffer, 0, MAX_HTTP_OUTPUT_BUFFER + 1);

    *config_ = {
        .host = host.begin(),
        .port = port,
        .username = "admin",
        .password = "123",
        .path = endpoint.begin(),
        .disable_auto_redirect = true,
        .event_handler = event_handler,
        .transport_type = use_https ? HTTP_TRANSPORT_OVER_SSL : HTTP_TRANSPORT_OVER_TCP,
        .user_data = response_buffer,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };

    esp_netif_t *netif = esp_netif_get_default_netif();
    esp_netif_ip_info_t info;
    esp_netif_get_ip_info(netif, &info);

    ESP_LOGI("HTTP_IP", "%lli", info.ip.addr);

    esp_http_client_handle_t client;
    client = esp_http_client_init(config_);

    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_post_field(client, req.data.c_str(), req.data.size());

    for (auto i : req.headers) {
        esp_http_client_set_header(client, i.name.c_str(), i.value.c_str());
    }

    esp_http_client_add_auth(client);
    esp_http_client_set_authtype(client, HTTP_AUTH_TYPE_BASIC);

    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(HTTP_LOG_TAG, "HTTP GET Status = %d, content_length = %" PRId64,
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(HTTP_LOG_TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }
    ESP_LOG_BUFFER_HEX(HTTP_LOG_TAG, response_buffer, strlen(response_buffer));

    int status = esp_http_client_get_status_code(client);

    esp_http_client_cleanup(client);

    return HttpResponse {
        .status = status,
        .data = response_buffer,
    };
}

esp_err_t EspHttpDriver::event_handler(esp_http_client_event_t *event) {
    constexpr const char *TAG = "HTTP_Event";

    static char *output_buffer;  // Buffer to store response of http request from event handler
    static int output_len;       // Stores number of bytes read

    switch (event->event_id) {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", event->header_key, event->header_value);
        break;
    case HTTP_EVENT_ON_DATA: {
        ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", event->data_len);
        // Clean the buffer in case of a new request
        if (output_len == 0 && event->user_data) {
            // we are just starting to copy the output data into the use
            memset(event->user_data, 0, MAX_HTTP_OUTPUT_BUFFER);
        }
        /*
            *  Check for chunked encoding is added as the URL for chunked encoding used in this example returns binary data.
            *  However, event handler can also be used in case chunked encoding is used.
            */
        if (!esp_http_client_is_chunked_response(event->client)) {
            // If user_data buffer is configured, copy the response into the buffer
            int copy_len = 0;
            if (event->user_data) {
                // The last byte in evt->user_data is kept for the NULL character in case of out-of-bound access.
                copy_len = MIN(event->data_len, (MAX_HTTP_OUTPUT_BUFFER - output_len));
                if (copy_len) {
                    memcpy(event->user_data + output_len, event->data, copy_len);
                }
            } else {
                int content_len = esp_http_client_get_content_length(event->client);
                if (output_buffer == NULL) {
                    // We initialize output_buffer with 0 because it is used by strlen() and
                    // similar functions therefore should be null terminated.
                    output_buffer = reinterpret_cast<char *>(calloc(content_len + 1, sizeof(char)));
                    output_len = 0;
                    if (output_buffer == NULL) {
                        ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                        return ESP_FAIL;
                    }
                }
                copy_len = MIN(event->data_len, (content_len - output_len));
                if (copy_len) {
                    memcpy(output_buffer + output_len, event->data, copy_len);
                }
            }
            output_len += copy_len;
        }
        break;
    }
    case HTTP_EVENT_ON_FINISH:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            if (output_buffer != NULL) {
#if CONFIG_EXAMPLE_ENABLE_RESPONSE_BUFFER_DUMP
                ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
#endif
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
    case HTTP_EVENT_DISCONNECTED: {
        ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
        int mbedtls_err = 0;
        esp_err_t err = esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)event->data, &mbedtls_err, NULL);
        if (err != 0) {
            ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
            ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
        }
        if (output_buffer != NULL) {
            free(output_buffer);
            output_buffer = NULL;
        }
        output_len = 0;
        break;
    }
    default:
        break;
    }
    return ESP_OK;
}


}  // namespace http

#endif  // ESP_PLATFORM
