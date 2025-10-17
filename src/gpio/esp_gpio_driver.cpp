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
#include <driver/adc.h>
#include <hal/adc_ll.h>
#include <hal/adc_hal.h>
#include <esp_adc_cal.h>

#include "gpio/esp_gpio_driver.h"

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
    if (value == DigitalValue::HIGH) gpio_output_set(1 << pin, 0, 0, 0);
    if (value == DigitalValue::LOW) gpio_output_set(0, 1 << pin, 0, 0);
}

jenlib::gpio::DigitalValue gpio::EspGpioDriver::digital_read(PinIndex pin) noexcept {
    return static_cast<DigitalValue>(
        static_cast<bool>(gpio_input_get() & (1 << pin)));
}

void gpio::EspGpioDriver::analog_write(PinIndex pin, std::uint16_t value) noexcept {
}

std::uint16_t gpio::EspGpioDriver::analog_read(PinIndex pin) noexcept {
}

void gpio::EspGpioDriver::set_analog_read_resolution(std::uint8_t bits) noexcept {
    adc_set_data_width(ADC_UNIT_1, static_cast<adc_bits_width_t>(bits));
    adc_set_data_width(ADC_UNIT_2, static_cast<adc_bits_width_t>(bits));
}

void gpio::EspGpioDriver::set_analog_write_resolution(std::uint8_t bits) noexcept {
    esp_adc_cal_characteristics_t adc1_chars, adc2_chars;

    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, static_cast<adc_bits_width_t>(bits), 0, &adc1_chars);
    esp_adc_cal_characterize(ADC_UNIT_2, ADC_ATTEN_DB_11, static_cast<adc_bits_width_t>(bits), 0, &adc2_chars);
}

std::uint8_t gpio::EspGpioDriver::get_analog_read_resolution() const noexcept {
}

std::uint8_t gpio::EspGpioDriver::get_analog_write_resolution() const noexcept {
}
