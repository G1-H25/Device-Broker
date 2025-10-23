/**
 * @file button.h
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-20
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef INCLUDE_GPIO_BUTTON_H_
#define INCLUDE_GPIO_BUTTON_H_

#include <jenlib/gpio/GPIO.h>
#include <jenlib/events/EventTypes.h>
#include <jenlib/events/EventDispatcher.h>
#include "gpio/esp_gpio_driver.h"

namespace gpio {

using jenlib::gpio::Pin;
using jenlib::gpio::PinMode;
using jenlib::gpio::PinIndex;
using jenlib::gpio::DigitalValue;

using jenlib::events::EventDispatcher;
using jenlib::events::EventCallback;
using jenlib::events::EventType;
using jenlib::events::Event;

using jenlib::gpio::GpioDriver;

class Button : public jenlib::gpio::Pin {
 public:
    Button(PinIndex pin, EventCallback callback);
    ~Button();

 private:
    static void onButtonPress_(void *data);

    volatile bool has_been_pressed_ = false;
    static const uint8_t kButtonPressEventBase = static_cast<uint8_t>(EventType::kCustom);
    uint8_t button_press_event_id;
    uint8_t pin;
};

}  // namespace gpio

#endif  // INCLUDE_GPIO_BUTTON_H_
