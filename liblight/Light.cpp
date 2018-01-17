/*
 * Copyright (C) 2017 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "LightService"

#include <log/log.h>
#include <cutils/properties.h>

#include "Light.h"

#include <fstream>

namespace android {
namespace hardware {
namespace light {
namespace V2_0 {
namespace implementation {

#define LEDS             "/sys/class/leds/"

#define LCD_LED          LEDS "lcd-backlight/"
#define BUTTON_LED       LEDS "button-backlight/"
#define CHARGING_LED     LEDS "charging/"

#define BRIGHTNESS       "brightness"
#define BLINK_BRIGHTNESS "blink_brightness"
#define TRIGER           "trigger"
#define LED_ON           "delay_on"
#define LED_OFF          "delay_off"


/*
 * Write value to path and close file.
 */
static void set(std::string path, std::string value) {
    std::ofstream file(path);
    file << value;
}

static void set(std::string path, int value) {
    set(path, std::to_string(value));
}


static uint32_t rgbToBrightness(const LightState& state) {
    uint32_t color = state.color & 0x00ffffff;
    return ((77 * ((color >> 16) & 0xff)) + (150 * ((color >> 8) & 0xff)) +
            (29 * (color & 0xff))) >> 8;
}

static void handleBacklight(const LightState& state) {
    uint32_t brightness = rgbToBrightness(state);
    set(BUTTON_LED BRIGHTNESS, brightness);
}

static void handleNotification(const LightState& state) {
    uint32_t Brightness;
    int32_t onMS, offMS;

    if (state.flashMode == Flash::TIMED) {
        onMS = state.flashOnMs;
        offMS = state.flashOffMs;
    } else {
        onMS = 0;
        offMS = 0;
    }

    if (onMS > 0) { 
        Brightness = (state.color & 0xFF000000) >> 24;
        if (Brightness == 0x00) Brightness = 0xFF;

        if (Brightness == 255) Brightness = 15; // max_brightness 15 steps of 5
        else if (Brightness > 1 && Brightness < 255) Brightness = 10;
        else Brightness = 5;

        if ((offMS - 0) > 0) {
           set(CHARGING_LED BRIGHTNESS, 0);
           set(CHARGING_LED BLINK_BRIGHTNESS, Brightness);
           set(CHARGING_LED TRIGER, "timer");
           set(CHARGING_LED LED_ON, onMS);
           set(CHARGING_LED LED_OFF, offMS);
       } else {
           set(CHARGING_LED TRIGER, "none");
           set(CHARGING_LED BRIGHTNESS, Brightness);
       }

       ALOGE("liblights notification led brightness=%d, timer 0n=%d, off=%d\n", Brightness, onMS, offMS);
    } else {
        char value[PROPERTY_VALUE_MAX];
        property_get("led.batton", value, NULL);
        if (strstr(value, "1")) {
           set(CHARGING_LED BLINK_BRIGHTNESS, 0);
           set(CHARGING_LED BRIGHTNESS, 0);
           set(CHARGING_LED TRIGER, "battery-full");
        } else set(CHARGING_LED TRIGER, "none");// none alredy set LED_BRIGHTNESS to 0
    }
}


static std::map<Type, std::function<void(const LightState&)>> lights = {
    {Type::BACKLIGHT, handleBacklight},
    {Type::BATTERY, handleNotification},
    {Type::NOTIFICATIONS, handleNotification},
    {Type::ATTENTION, handleNotification},
};

Light::Light() {}

Return<Status> Light::setLight(Type type, const LightState& state) {
    auto it = lights.find(type);

    if (it == lights.end()) {
        return Status::LIGHT_NOT_SUPPORTED;
    }

    /*
     * Lock global mutex until light state is updated.
     */
    std::lock_guard<std::mutex> lock(globalLock);

    it->second(state);

    return Status::SUCCESS;
}

Return<void> Light::getSupportedTypes(getSupportedTypes_cb _hidl_cb) {
    std::vector<Type> types;

    for (auto const& light : lights) types.push_back(light.first);

    _hidl_cb(types);

    return Void();
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace light
}  // namespace hardware
}  // namespace android

