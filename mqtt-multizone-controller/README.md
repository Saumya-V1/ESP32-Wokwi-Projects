# Multi-Zone Environmental Control System (MQTT)

A simplified greenhouse/server-room controller featuring a finite state machine, hysteresis-based control logic, and MQTT telemetry — the most complex project in this collection.

![circuit](./circuit.mp4)

## How it works
1. Reads temperature, humidity (DHT22), and light level (LDR) each cycle
2. Runs a finite state machine with three states: `IDLE`, `COOLING`, `ALERT`
3. Uses hysteresis (different ON/OFF thresholds) to prevent rapid actuator flickering at the boundary temperature
4. Publishes live sensor data as JSON to an MQTT broker (`test.mosquitto.org`) every 3 seconds
5. Subscribes to a command topic, allowing remote override (`FORCE_COOL`, `RESET`) from any MQTT client
6. Controls a servo (vent) and buzzer (alarm) based on current state
7. Uses non-blocking timing (`millis()`) so sensor reads, MQTT handling, and actuator control all run smoothly without freezing the loop

## Wiring
| Component | ESP32 Pin |
|---|---|
| DHT22 DATA | GPIO 4 |
| LDR Module AO | GPIO 34 |
| OLED SDA / SCL | GPIO 21 / GPIO 22 |
| Servo Signal | GPIO 13 |
| Buzzer + | GPIO 15 |

## Libraries
```
Adafruit GFX Library
Adafruit SSD1306
DHT sensor library
Adafruit Unified Sensor
ESP32Servo
PubSubClient
```

## Code
See [`sketch.ino`](./sketch.ino)

## Concepts learned
- Finite state machine design for embedded control logic
- Hysteresis control (a real technique used in thermostats and industrial control systems)
- MQTT publish/subscribe architecture — the industry-standard protocol for IoT device fleets
- Non-blocking timing patterns using `millis()` instead of `delay()`
- Voltage dividers and analog light sensing (LDR)

## Demo
Live sensor data can be viewed by subscribing to `multizone/data` on `test.mosquitto.org` using any MQTT client (e.g. MQTT Explorer).
