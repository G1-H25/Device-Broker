/**
 * @file esp_gpio_driver.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <soc/io_mux_reg.h>
#include <esp32s3/rom/gpio.h>
#include <driver/gpio.h>
#include <driver/dac.h>
#include <driver/adc.h>
#include <hal/adc_ll.h>
#include <hal/adc_hal.h>
#include <hal/dac_types.h>
#include <esp_adc_cal.h>

#include "gpio/esp_gpio_driver.h"

/**
 * @brief Set pin mode of given pin.
 *
 * @param pin Pin to configure.
 * @param mode Mode to use.
 */
void gpio::EspGpioDriver::set_pin_mode(PinIndex pin, PinMode mode) noexcept {
    gpio_config_t conf {
        .pin_bit_mask = pin,
    };

    gpio_num_t native_pin = static_cast<gpio_num_t>(pin);
    switch (mode) {
    case PinMode::INPUT:
        conf.mode = gpio_mode_t::GPIO_MODE_INPUT;
        conf.pull_down_en = gpio_pulldown_t::GPIO_PULLDOWN_DISABLE;
        conf.pull_up_en = gpio_pullup_t::GPIO_PULLUP_DISABLE;

        gpio_config(&conf);
        break;
    case PinMode::INPUT_PULLUP:
        conf.mode = gpio_mode_t::GPIO_MODE_INPUT;
        conf.pull_down_en = gpio_pulldown_t::GPIO_PULLDOWN_DISABLE;
        conf.pull_up_en = gpio_pullup_t::GPIO_PULLUP_ENABLE;

        gpio_config(&conf);
        break;
    case PinMode::INPUT_PULLDOWN:
        conf.mode = gpio_mode_t::GPIO_MODE_INPUT;
        conf.pull_down_en = gpio_pulldown_t::GPIO_PULLDOWN_ENABLE;
        conf.pull_up_en = gpio_pullup_t::GPIO_PULLUP_DISABLE;

        gpio_config(&conf);
        break;
    case PinMode::OUTPUT:
        conf.mode = gpio_mode_t::GPIO_MODE_OUTPUT;
        conf.pull_down_en = gpio_pulldown_t::GPIO_PULLDOWN_DISABLE;
        conf.pull_up_en = gpio_pullup_t::GPIO_PULLUP_DISABLE;

        gpio_config(&conf);
        break;
    default:
        break;
    }
}

void gpio::EspGpioDriver::digital_write(PinIndex pin, DigitalValue value) noexcept {
    if (value == DigitalValue::HIGH) gpio_output_set(BIT(pin), 0, 0, 0);
    if (value == DigitalValue::LOW) gpio_output_set(0, BIT(pin), 0, 0);
}

/**
 * @brief Read input value from desired pin. Returns HIGH or LOW.
 *
 * @param pin The desired pin to read from. Supports pin 0 to 31.
 * @returns jenlib::gpio::DigitalValue
 */
jenlib::gpio::DigitalValue gpio::EspGpioDriver::digital_read(PinIndex pin) noexcept {
    return static_cast<DigitalValue>(
        static_cast<bool>(gpio_input_get() & BIT(pin)));
}

/**
 * @brief Sets a pin analog pin strength on pin 25 or 26.
 * These are the only pins with **DAC** support.
 *
 * @param pin The pin to write a value to. Only supports pin 25 or 26.
 * @param value The strength of the output.
 */
void gpio::EspGpioDriver::analog_write(PinIndex pin, std::uint16_t value) noexcept {
    dac_channel_t dac_channel;
    switch (pin) {
    case 25:
        dac_channel = DAC_CHAN_0;
        break;
    case 26:
        dac_channel = DAC_CHAN_1;
        break;
    default:
        return;
    }

    if (value == 0) {
        dac_output_disable(dac_channel);
    }

    if (value == 0) {
        dac_output_enable(dac_channel);
        dac_output_voltage(dac_channel, value);
    }
}

/**
 * @brief Reads an analog value from specified pin.
 *
 * @param pin The pin to read from. Can only read from pins 1 to 20.
 * @return std::uint16_t
 */
std::uint16_t gpio::EspGpioDriver::analog_read(PinIndex pin) noexcept {
    adc_unit_t adc_unit;
    adc_channel_t adc_channel;
    if (pin > 0 && pin <= 10)
        adc_unit == 0;
    else if (pin > 10 && pin <= 20)
        adc_unit == 1;
    else
        return;

    adc_channel = static_cast<adc_channel_t>((pin % 10) + 1);

    if (adc_unit == adc_unit_t::ADC_UNIT_1) {
        return adc1_get_raw(static_cast<adc1_channel_t>(adc_channel));
    } else if (adc_unit == adc_unit_t::ADC_UNIT_2) {
        int read;
        adc2_get_raw(static_cast<adc2_channel_t>(adc_channel), this->read_resolution, &read);
    }
}

/**
 * @brief Set analog read resolution.
 *
 * @example
 *
 * @param bits The desired read resolution
 */
void gpio::EspGpioDriver::set_analog_read_resolution(std::uint8_t bits) noexcept {
    // Configure ADC1 to desired precision. ADC2 width is configured upon reading.
    esp_err_t err = adc1_config_width(static_cast<adc_bits_width_t>(bits));
    if (err != ESP_OK) return;

    this->read_resolution = static_cast<adc_bits_width_t>(bits);
}

/**
 * @brief Set analog write resolution.
 *
 * @warning On the esp32-s3 the write resolution is fixed and cannot be changed.
 *
 * @param bits
 */
void gpio::EspGpioDriver::set_analog_write_resolution(std::uint8_t bits) noexcept {
    return;
}

std::uint8_t gpio::EspGpioDriver::get_analog_read_resolution() const noexcept {
    return this->read_resolution;
}

/**
 * @brief Get DAC write resolution.
 *
 * @return std::uint8_t
 */
std::uint8_t gpio::EspGpioDriver::get_analog_write_resolution() const noexcept {
    return 8;
}
