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

#ifdef ESP_PLATFORM

#include <soc/io_mux_reg.h>
#include <esp32s3/rom/gpio.h>
#include <driver/gpio.h>
#include <esp_adc/adc_oneshot.h>
#include <hal/adc_ll.h>
#include <hal/adc_hal.h>
#include <hal/dac_types.h>
#include <esp_adc_cal.h>
#include <driver/adc.h>

#include "gpio/esp_gpio_driver.h"

/**
 * @brief Set pin mode of given pin.
 *
 * @param pin pin to configure.
 * @param mode mode to use.
 *
 * @example gpio/gpio_output.cpp
 */
void gpio::EspGpioDriver::set_pin_mode(PinIndex pin, PinMode mode) noexcept {
    switch (mode) {
    case PinMode::INPUT:
        configure_pin(
            pin,
            gpio_mode_t::GPIO_MODE_INPUT,
            gpio_int_type_t::GPIO_INTR_DISABLE,
            false,
            false);
        break;
    case PinMode::INPUT_PULLUP:
        configure_pin(
            pin,
            gpio_mode_t::GPIO_MODE_INPUT,
            gpio_int_type_t::GPIO_INTR_DISABLE,
            false,
            true);
        break;
    case PinMode::INPUT_PULLDOWN:
        configure_pin(
            pin,
            gpio_mode_t::GPIO_MODE_INPUT,
            gpio_int_type_t::GPIO_INTR_DISABLE,
            true,
            false);
        break;
    case PinMode::OUTPUT:
        configure_pin(
            pin,
            gpio_mode_t::GPIO_MODE_OUTPUT,
            gpio_int_type_t::GPIO_INTR_DISABLE,
            false,
            false);
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
 *
 * @example gpio/gpio_output.cpp
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
 *
 * @warning Not implemented fully in the current version
 *
 * @example gpio/gpio_output.cpp
 * General output example
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
}

/**
 * @brief Reads an analog value from specified pin.
 *
 * @param pin The pin to read from. Can only read from pins 1 to 20.
 * @return std::uint16_t
 *
 * @example gpio/gpio_input.cpp
 */
std::uint16_t gpio::EspGpioDriver::analog_read(PinIndex pin) noexcept {
    adc_unit_t adc_unit;
    adc_channel_t adc_channel;
    if (pin > 0 && pin <= 10)
        adc_unit = adc_unit_t::ADC_UNIT_1;
    else if (pin > 10 && pin <= 20)
        adc_unit = adc_unit_t::ADC_UNIT_2;
    else
        return 0;

    adc_channel = static_cast<adc_channel_t>((pin % 10) + 1);

    if (adc_unit == adc_unit_t::ADC_UNIT_1) {
        return adc1_get_raw(static_cast<adc1_channel_t>(adc_channel));
    } else if (adc_unit == adc_unit_t::ADC_UNIT_2) {
        int read;
        adc2_get_raw(static_cast<adc2_channel_t>(adc_channel), this->read_resolution_, &read);
        return read;
    }

    return 0;
}

/**
 * @brief Set analog read resolution.
 *
 * @param bits The desired read resolution
 *
 * @example gpio/gpio_input.cpp
 */
void gpio::EspGpioDriver::set_analog_read_resolution(std::uint8_t bits) noexcept {
    // Configure ADC1 to desired precision. ADC2 width is configured upon reading.
    esp_err_t err = adc1_config_width(static_cast<adc_bits_width_t>(bits));
    if (err != ESP_OK) return;

    this->read_resolution_ = static_cast<adc_bits_width_t>(bits);
}

/**
 * @brief Set analog write resolution.
 *
 * @warning On the esp32-s3 the write resolution is fixed and cannot be changed.
 *
 * @param bits
 *
 * @example gpio/gpio_input.cpp
 */
void gpio::EspGpioDriver::set_analog_write_resolution(std::uint8_t bits) noexcept {
    return;
}

std::uint8_t gpio::EspGpioDriver::get_analog_read_resolution() const noexcept {
    return this->read_resolution_;
}

/**
 * @brief Get DAC write resolution.
 *
 * @return std::uint8_t
 *
 * @example gpio/gpio_output.cpp
 */
std::uint8_t gpio::EspGpioDriver::get_analog_write_resolution() const noexcept {
    return 8;
}

/**
 * @brief Configure a pin with more precision than `set_pin_mode`. Useful if you want to register interrupts to a pin.
 *
 * @param pin Pin to configure
 * @param mode Which mode the pin should be configured for
 * @param intr_type Which type of interrupt it should trigger
 * @param enable_pulldown Whether or not pulldown should be enabled
 * @param enable_pullup Whether or not pulldown should be enabled
 */
void gpio::EspGpioDriver::configure_pin(
        PinIndex pin,
        gpio_mode_t mode,
        gpio_int_type_t intr_type,
        bool enable_pulldown,
        bool enable_pullup) {
    gpio_config_t config{
        .pin_bit_mask = BIT(pin),
        .mode = mode,
        .pull_up_en = static_cast<gpio_pullup_t>(enable_pullup),
        .pull_down_en = static_cast<gpio_pulldown_t>(enable_pulldown),
        .intr_type = intr_type,
    };

    gpio_config(&config);
}

#endif  // ESP_PLATFORM
