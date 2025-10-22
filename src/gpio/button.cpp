/**
 * @file button.cpp
 * @author Love Lindeborg
 * @brief
 * @version 0.1
 * @date 2025-10-20
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifdef ESP_PLATFORM

#include <jenlib/events/EventDispatcher.h>
#include <esp_intr_alloc.h>
#include <esp_intr_types.h>
#include <soc/interrupts.h>
#include <driver/gpio.h>
#include "gpio/button.h"

namespace gpio {

Button::Button(PinIndex pin, EventCallback callback) : Pin(new EspGpioDriver(), pin) {
    this->button_press_event_id = kButtonPressEventBase + pin;
    EventDispatcher::register_callback(
        static_cast<EventType>(this->button_press_event_id), callback);

    gpio_set_intr_type(static_cast<gpio_num_t>(pin), GPIO_INTR_POSEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(static_cast<gpio_num_t>(pin), onButtonPress_, static_cast<void*>(this));
}

Button::~Button() {
    delete this->driver();
}

void Button::onButtonPress_(void *data) {
    Button *button = reinterpret_cast<Button *>(data);
    EventDispatcher::dispatch_event(Event(static_cast<EventType>(button->button_press_event_id), 0, 0), nullptr);
}

}  // namespace gpio

#endif  // ESP_PLATFORM
