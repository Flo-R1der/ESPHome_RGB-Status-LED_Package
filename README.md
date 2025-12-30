# ESPHome RGB Status LED Package

A reusable ESPHome package that provides a prioritized RGB status LED system. It visualizes 
key device states such as booting, WiFi connectivity, API availability. 
The package only requires a single RGB light entity with the ID **`system_status_led`**.

## 🚀 Features

- Priority-based LED status handling.
   - booting → boot done → WiFi connected → Home Assistant connected
- Drop-in ESPHome package - detects system states on it's own.
- Works with any RGB LED platform (Neopixel, RMT LED Strip, Cwww, etc.).
- Optional `working` states provided (status evaluation, actuator working, beacon activity, ...).


## 🛠️ Set Up

Add the package to your device configuration:

```yaml
packages:
  rgb_status_led: 
    url: https://github.com/Flo-R1der/ESPHome_RGB-Status-LED_Package
    file: status_led_package.yaml
```

Make sure your project defines the required RGB LED light with the **ID `system_status_led`**. The following `light` configuration has proven to work on a [LOLIN C3 Mini](https://www.espboards.dev/esp32/lolin-c3-mini/) (`board: lolin_c3_mini`):

```yaml
light:
  - platform: esp32_rmt_led_strip
    id: system_status_led             # DO NOT CHANGE
    pin: GPIO7                        # check your board
    rgb_order: GRB                    # check your board
    num_leds: 1
    chipset: ws2812                   # check your board
    name: "onboard LED"
    disabled_by_default: true
    default_transition_length: 200ms
    icon: mdi:led-outline
    restore_mode: ALWAYS_OFF
    effects:
      - pulse:
          name: "Slow Pulse"                               # REQUIRED
          update_interval: 1s                              # can be adjusted
          max_brightness: ${system_status_led_brightness}  # provided by the package
      - pulse:
          name: "Fast Pulse"                               # REQUIRED
          update_interval: 400ms                           # can be adjusted
          max_brightness: ${system_status_led_brightness}  # provided by the package
```

> [!NOTE]  
> The package does **not** depend on any specific LED chipset or ESP32 variant.  
> If you prefer another LED type (NeopixelBus, FastLED, CWWW RGB LED), simply keep the ID identical.  


### Optional: brightness value
The predefined brightness is set to `50%`. This can be overridden, by adding a suitable value to your project:

````yaml
substitutions:
  system_status_led_brightness: "20%"
````


## 💡 LED Colors and States

### 🚦 System States - static light

The **LED system status** follows a strict top-down priority. The highest matching state always wins.
If a condition is no longer fulfilled, the LED falls back to the previous applicable state.

| Priority | Color + Effect            | System State             | Notes                                    |
| -------- | ------------------------- | ------------------------ | ---------------------------------------- |
|   1      | 🔴 **Red**<br>static     | Booting / initialization | Shown before WiFi stack is ready         |
|   2      | 🟡 **Yellow**<br>static  | Boot completed           | Waiting for WiFi / network issue?        |
|   3      | ⚪ **White**<br>static   | WiFi connected           | network OK, Home Assistant not connected |
|   4      | 🟢 **Green**<br>static   | Home Assistant connected | FULLY STARTED - Normal operating mode    |



### Optional: ⚙️ Working States - pulsing light

Additionally there are LED states provided, which can be triggered from your main device configuration.
They are meant to signals states separate from the main priority logic and override the system status display until anything else updates the LED state. Currently the following states are supported:

 `led_working_status_<color>`:
<b style="color:dodgerblue">blue</b> |
<b style="color:darkmagenta">purple</b> |
<b style="color:red">red</b> |
<b style="color:darkorange">orange</b> |
<b style="color:sand">yellow</b> |
<b style="color:lime">green</b> |
<b style="color:aqua">cyan</b> |
<b style="color:lightgray">white</b>

```yaml
binary_sensor:
  - platform: ble_presence
    mac_address: ${ble_beacon_mac}
    name: "BLE-Beacon Present"
    id: ble_beacon_presence
    timeout: 60s
    on_press:
      then:
        - script.execute: led_working_status_blue
          effect: "Slow Pulse"   # required
    on_release:
      then:
        - script.execute: led_system_status   # reset to the system status
```

> `led_working_status_1` and `led_working_status_2` from a previous version are still supported.


## 🚩 Troubleshooting

### LED Does Not Light Up

- Confirm that the LED hardware definition exists in your main config and uses the same `id:` expected by the package.
- Double-check pin numbers, chipset type, and RGB order.
- Ensure your power supply can drive the LED (WS2812 requires stable 5V).

### LED Only Shows One Color or Wrong Colors

- Incorrect `rgb_order`.
  Try switching between `RGB`, `GRB`, or `BRG` depending on the LED hardware.

### Home Assistant API Status Seems Wrong

- If API repeatedly connects/disconnects, check your WiFi RSSI.
- Validate that the ESPHome API is properly configured in Home Assistant.
- If using VLANS or firewalls, ensure port `6053` is allowed.

<br>

---

## 📌 Open Topics

- [ ] Create additional working-status templates with more effects.
- [ ] A selectable "stealth mode" (LED off unless error) would be nice.

## ❤️ Like My Work?
[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/I3I4160K4Y)
