/**
 * @file main.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-17
 *
 * @copyright Copyright (c) 2025
*/

#include <unity.h>
#include <esp_vfs.h>
#include "gpio/esp_gpio_driver.h"

void test_pin_input() {
    gpio::EspGpioDriver driver;
}

void test_pin_output() {
}

extern "C" void app_main() {
    UNITY_BEGIN();
    UNITY_END();
}
