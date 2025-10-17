/**
 * @file esp_gpio_driver.h
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef INCLUDE_GPIO_ESP_GPIO_DRIVER_H_
#define INCLUDE_GPIO_ESP_GPIO_DRIVER_H_

#include <jenlib/gpio/GpioDriver.h>

namespace gpio {

class EspGpioDriver : public jenlib::gpio::GpioDriver {
    using PinMode = jenlib::gpio::PinMode;
    using PinIndex = jenlib::gpio::PinIndex;
    using DigitalValue = jenlib::gpio::DigitalValue;

 public:
    void set_pin_mode(PinIndex pin, PinMode mode) noexcept override;

    void digital_write(PinIndex pin, DigitalValue value) noexcept override;
    DigitalValue digital_read(PinIndex pin) noexcept override;

    void analog_write(PinIndex pin, std::uint16_t value) noexcept override;

    std::uint16_t analog_read(PinIndex pin) noexcept override;

    void set_analog_read_resolution(std::uint8_t bits) noexcept override;
    void set_analog_write_resolution(std::uint8_t bits) noexcept override;

    std::uint8_t get_analog_read_resolution() const noexcept override;
    std::uint8_t get_analog_write_resolution() const noexcept override;
};

}  // namespace gpio

#endif  // INCLUDE_GPIO_ESP_GPIO_DRIVER_H_
