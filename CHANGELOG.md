# 2026-03-10
- replaced multiple effects with one lambda `system_status_led_fx` (#8)
- use `system_status_led_effect_handler` script to map old an new effects
- implemented "Brightness System Status LED"-Slider for dynamic brightness adjustment (#7)
- LED-ID can now be adjusted with `system_status_led_id` substitution (#4)
- updated Documentation

<br>

# 2026-01-17
- **System Status** is now 'static' / no effect for min. dependency and max. stability
- **Working States** are not pulsing: "Fast Pulse", "Slow Pulse", "Breathe"
- Pulsing Effect is now adjustable (see readme for details)
- Working States now have color-names instead of numbers (legacy names still supported)
- **Introducing Feedback States** with blinking effect / blink counter adjustable
- System Status LED is now `entity_category: "diagnostic"`
- other small adjustments

<br>

# 2025-12-01
- Fix missing boot-state (red) by [montaguethomas](https://github.com/montaguethomas) in #1
- Add substitution for brightness value as requested by [MattWestB](https://github.com/MattWestb), see #2

<br>

# 2025-11-16
- initial release