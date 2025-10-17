/**
 * @file main.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-09-16
 *
 * @copyright Copyright (c) 2025
*/

#ifndef UNIT_TEST

#include <esp32s3/rom/gpio.h>
#include <esp_rom_gpio.h>
#include <soc/io_mux_reg.h>
#include <esp_log.h>
#include <gpio/esp_gpio_driver.h>

#define IO_MUX_BASE_ADDR 0x60009000

extern "C" {
    void app_main(void);
}

void app_main() {
    gpio::EspGpioDriver driver();

    *reinterpret_cast<uint32_t *>(IO_MUX_GPIO1_REG) |= FUN_IE | FUN_PU;

    while (true) {
        uint32_t in = *reinterpret_cast<uint32_t *> GPIO_IN_REG;
        ESP_LOGI("Power", "%x", (in & (1u << 1)) ? 1 : 0);
    }
}

#endif
