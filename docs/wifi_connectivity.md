# WiFi Connection

## Examples

- [Connecting to WiFi](examples/wifi/wifi_connection.cpp)

## Connect to WiFi

```cpp

#include "wifi/wifi_client.h"

extern "C" void app_main() {
    WiFiClient client{WIFI_SSID, WIFI_PASSWORD};
    client.connect();

    // Delay to wait for connection to stabilize
    vTaskDelay(pdMS_TO_TICKS(2000));
}

```
