#pragma once
#include "esphome.h"

namespace esphome {
namespace system_status_led_fx {

inline void pulse(light::LightState *state,
                    int speed,
                    float max_brightness) {

    static int start_time = millis();
    static float last_brightness = -1.0f;
    float multiplier = 1.0f;
    float phase = 1.0f;

    if (speed > 0) {   // can not calculate with speed=0 >>> speed=0 is a steady light on max_brightness
        phase = ((millis() - start_time) % speed) / (float)speed;
        multiplier = (sinf(phase * 2.0f * 3.14159f) + 1.0f) / 2.0f;
    }
    float calc_brightness = multiplier * max_brightness;

    ESP_LOGV("system_status_led", "speed=%i, max_brightness=%f, start_time=%i", speed, max_brightness, start_time);
    ESP_LOGV("system_status_led", "phase=%f, multiplier=%f, calc_brightness=%f", phase, multiplier, calc_brightness);

    if (fabsf(calc_brightness - last_brightness) < 0.01f) {
        ESP_LOGV("system_status_led", "last_brightness=%f >>> STOP brightness change", last_brightness);
        return;   // continue, only if there is a brightness change > 1%
    }
    ESP_LOGV("system_status_led", "last_brightness=%f >>> UPDATING brightness value", last_brightness);

    auto call = state->turn_on();
    call.set_brightness(calc_brightness);
    call.set_transition_length(100);   // transition length schould meet the update_interval in the main.yaml
    call.perform();

    last_brightness = calc_brightness;
}

}   // system_status_led_fx
}   // esphome
