# HTTP requests

## HttpClient

The HTTP client class is a class that manages the HTTP driver which will be used to perform requests.

All member methods are static. This class cannot be
instantiated.

## Before requests

1. Connect to WiFi using [WiFiClient](wifi_connectivity.md)

2. Set the driver

```cpp
#include "http/http_esp_client_driver.h"

extern "C" void app_main() {
    EspHttpDriver driver();

    HttpClient::setDriver(&driver);
}
```

## Performing a get request

```cpp
#include "http/http_client.h"
#include "http/http_response.h"

void getRequestExample() {
    // This method is sync and returns data once it is finished.
    HttpResponse response = HttpClient::getDriver()->performGetRequest("archlinux.org", 443, "/");
}
```

## Performing a post request

```cpp
#include "http/http_client.h"
#include "http/http_response.h"

void postRequestExample() {
    // Send raw text data using `Content-Type: application/text`
    HttpRequest data{
        .data = "This is a text message"
    };

    // Will return an error 403 since POST is not supported on `archlinux.org`.
    HttpResponse response = HttpClient::getDriver()->performPostRequest("archlinux.org", 443, "/", data);

    // Send JSON data `Content-Type: application/json`
    data = {
        .data = "{\"message\": \"Hello, World!\"}",
        .is_json = true
    };

    response = HttpClient::getDriver()->performPostRequest("archlinux.org", 443, "/", data);
}
```
