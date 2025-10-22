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

#define HTTP_API_HOST               "192.168.8.169"
#define HTTP_API_PORT               5000

#define HTTP_API_HEALTH_ENDPOINT    "/Health"
#define HTTP_API_POST_MEASUREMENT   "/Post"
#define HTTP_API_SYNC_SENSORS       "/GetSensors"

#define HTTP_API_JSON_TEMP_KEY      "temp"
#define HTTP_API_JSON_HUM_KEY       "hum"
#define HTTP_API_JSON_TIME_KEY      "time"

#endif
```

When building on github actions these must be defined when compiling the program otherwise build will fail.

## credentials.h

```cpp
// Replace with actual username and password. These are used to authenticate the broker towards the backend server. In exchange we will recieve a JWT.
#define USERNAME "admin"
#define PASSWORD "123456"
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
