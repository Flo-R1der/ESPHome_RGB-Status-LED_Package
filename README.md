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

Make sure your project defines the required GTB LED light with the **ID `system_status_led`**. The following `light` configuration has proven to work on a [LOLIN C3 Mini](https://www.espboards.dev/esp32/lolin-c3-mini/) (`board: lolin_c3_mini`):

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
          name: "Slow Pulse"          # REQUIRED
          update_interval: 2s         # can be adjusted
      - pulse:
          name: "Fast Pulse"          # REQUIRED
          update_interval: 500ms      # can be adjusted
```

> [!NOTE]  
> The package does **not** depend on any specific LED chipset or ESP32 variant.  
> If you prefer another LED type (NeopixelBus, FastLED, CWWW RGB LED), simply keep the ID identical.  


## 💡 LED Colors and States

### System States

The **LED system status** follows a strict top-down priority. The highest matching state always wins.
If a condition is no longer fulfilled, the LED falls back to the previous applicable state.

| Priority | Color + Effect            | System State             | Notes                                    |
| -------- | ------------------------- | ------------------------ | ---------------------------------------- |
|   1      | **Red**<br>fast pulsing   | Booting / initialization | Shown before WiFi stack is ready         |
|   2      | **Yellow**<br>static      | Boot completed           | Waiting for WiFi / network issue?        |
|   3      | **White**<br>static       | WiFi connected           | network OK, Home Assistant not connected |
|   4      | **Green**<br>static       | Home Assistant connected | FULLY STARTED - Normal operating mode    |


### Custom States

Additionally there are **LED working states** provided, which can be triggered from your main device configuration.
They are meant to signals states separate from the main priority logic.
They temporarily override the LED as long as the script is active, similar to the beacon state.

| Script ID              | Color + Effect             | Notes (Examples)                                                       |
| ---------------------- | -------------------------- | ---------------------------------------------------------------------- |
| `led_working_status_1` | **Blue**<br>slow pulsing   | Example: device performing a background task; BT-beacon detected; etc. |
| `led_working_status_2` | **Purple**<br>slow pulsing | Example: special output is turned on; long-running I²C read; etc.      |
| `led_working_status_*` | _add more if you like_     |                                                                        |

To set and reset the working light you can call the following scripts:
- `- script.execute: led_working_status_1` (_or any other number_)
- `- script.execute: led_system_status` (_to reset the LED to the system-status_)

Example:
```yaml
binary_sensor:
  - platform: ble_presence
    mac_address: ${ble_beacon_mac}
    name: "BLE-Beacon Present"
    id: ble_beacon_presence
    timeout: 60s
    on_press:
      then:
        - script.execute: led_working_status_1
    on_release:
      then:
        - script.execute: led_system_status
```


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

- [ ] Add a substitution value for general brightness (now all states are on 50%)
- [ ] Create additional working-status templates with more effects.
- [ ] A selectable "stealth mode" (LED off unless error) would be nice.

## ❤️ Like My Work?
[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/I3I4160K4Y)
