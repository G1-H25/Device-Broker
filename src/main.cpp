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
#include <driver/gpio.h>
#include "gpio/esp_gpio_driver.h"

#define IO_MUX_BASE_ADDR 0x60009000

using jenlib::gpio::Pin;
using jenlib::gpio::PinIndex;
using jenlib::gpio::PinMode;
using jenlib::gpio::DigitalValue;

extern "C" {
    void app_main(void);
}

static volatile bool shouldSync;
void syncSensors(void *data) {
    shouldSync = true;
}

void app_main() {
    gpio::EspGpioDriver driver;
    Pin button_pin{&driver, 1};

    button_pin.pin_mode(PinMode::INPUT_PULLUP);

    gpio_set_intr_type(
        static_cast<gpio_num_t>(button_pin.index()),
        gpio_int_type_t::GPIO_INTR_POSEDGE);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(
        static_cast<gpio_num_t>(button_pin.index()),
        syncSensors,
        reinterpret_cast<void *>(button_pin.index()));
}

#endif
