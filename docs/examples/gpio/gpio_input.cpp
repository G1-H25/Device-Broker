/**
 * @file gpio_input.cpp
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

    // Set the pinmode of pin1 to INPUT
    driver.set_pin_mode(pinIndex, PinMode::INPUT);

    // The read resolution can be changed by using the following function
    // By default the read resolution has is 12 bits so this call is redundant.
    driver.set_analog_read_resolution(adc_bits_width_t::ADC_WIDTH_BIT_12);

    // Read analog value from driver.
    uint16_t value_analog = driver.analog_read(pinIndex);


    // Read digital value from specified pin, either HIGH or LOW.
    DigitalValue value_digital = driver.digital_read(pinIndex);


    // ----- Example using wrapper -----
    Pin pin{ &driver, 1 };

    // Initialize pin and set pinmode to desired pinmode.
    // Can also be `INPUT_PULLUP` or `INPUT_PULLDOWN`
    pin.pin_mode(PinMode::INPUT);

    // Read a 12 bit integer from pin. Jenlib supports up to 16 bit
    // values but the esp32 only supports a read resolution of 12 bits.
    uint16_t value_analog = pin.analog_read();

    // Read a digital value (HIGH or LOW, on or off)
    DigitalValue value_digital = pin.digital_read();
}
