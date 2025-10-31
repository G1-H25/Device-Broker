/**
 * @file gpio_output.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-20
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "gpio/esp_gpio_driver.h"

using jenlib::gpio::DigitalValue;
using jenlib::gpio::PinMode;
using jenlib::gpio::PinIndex;
using jenlib::gpio::Pin;

extern "C" void app_main() {
    // ----- Example using only driver -----
    constexpr PinIndex pinIndex = 1;
    gpio::EspGpioDriver driver;

    // Set the pinmode of pin1 to OUTPUT
    driver.set_pin_mode(pinIndex, PinMode::OUTPUT);

    // Analog write resolution is fixed to 8 bits. Cannot be changed.
    driver.set_analog_write_resolution(0);

    // Write analog value from driver. Jenlib requires a 16 bit
    // integer but the esp32 supports up to 8 bit values.
    driver.analog_write(pinIndex, 255);

    // Read digital value from specified pin, either HIGH or LOW.
    driver.digital_write(pinIndex, DigitalValue::HIGH);

    // ----- Example using wrapper -----
    Pin pin{ &driver, 1 };

    // Initialize pin and set pinmode to desired pinmode.
    pin.pin_mode(PinMode::OUTPUT);

    // Write a value between 0 and 255. Jenlib supports up to 16 bit
    // values but the esp32 only supports a write resolution of 8 bits.
    pin.analog_write(120);

    // Write a digital value (HIGH or LOW, on or off)
    pin.digital_write(DigitalValue::HIGH);
}
