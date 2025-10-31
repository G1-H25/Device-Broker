# Secrets

This directory contains the following files:

- routes.h
- secrets.h

These files contains the following information:

## routes.h

```cpp

// Defines that must exist and have values in similar format.
// This does not represent actual endpoints but are just examples.
//
// This is most likely incomplete and will need to be updated continuously
#ifndef SECRETS_H_
#define SECRETS_H_

#define HTTP_API_HOST               "prod.someserver.topdomain"
#define HTTP_API_PORT               443

#define HTTP_TEST_API_HOST          "dev.someserver.topdomain"
#define HTTP_TEST_API_PORT          443

#define HTTP_API_HEALTH_ENDPOINT    "/health"
#define HTTP_API_SUBMIT_BATCH       "/api/gateway/batch"
#define HTTP_API_SYNC_SENSORS       "/api/delivery/packages"
#define HTTP_API_REGISTER_GATEWAY   "/api/gateway/register"

#define HTTP_API_JSON_TEMP_KEY      "temp"
#define HTTP_API_JSON_HUM_KEY       "hum"
#define HTTP_API_JSON_TIME_KEY      "time"

#endif
```

When building on github actions these must be defined when compiling the program otherwise build will fail.

## credentials.h

```cpp
#ifndef CREDENTIALS_H_
#define CREDENTIALS_H_

#define WIFI_PASSWORD "admin"
#define WIFI_SSID "123456"

#define TEST_WIFI_PASSWORD "admin"
#define TEST_WIFI_SSID "123456"

#define BACKEND_PASSWORD "example"
#define BACKEND_USERNAME "big_strong_password123"

#endif  // CREDENTIALS_H_

```

# Networking

Program flow:

Driver scans package -->

App registers package with broker -->

Broker waits for message to start from driver -->

When delivery started broker broadcasts BLE message to start measuring.


# Endpoints

REST endpoints on the broker for the app:

- POST /startsensors
- POST /register
