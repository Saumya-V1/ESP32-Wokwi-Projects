# ESP32 Embedded & IoT Projects

A collection of ESP32 projects exploring embedded systems, IoT networking, and robotics-adjacent control logic. Built and simulated in [Wokwi](https://wokwi.com), progressing from fundamentals to a multi-sensor system with a finite-state-machine architecture and MQTT telemetry.

I'm a student exploring embedded/IoT/hardware as a career direction, and built these to learn the fundamentals hands-on before working with physical hardware. Each project README explains the wiring, code, and concepts learned.

## Projects 

| Project | Focus | Key Concepts |
|---|---|---|
| [Potentiometer-Controlled LED Brightness] | Analog input → PWM output | ADC, `map()`, PWM duty cycle |
| [DHT22 + OLED Display] | Sensor + display integration | I2C protocol, sensor libraries, error handling |
| [Wi-Fi LED Web Server] | Networking | HTTP server, routing, Wi-Fi connection |
| [Ultrasonic Sensor + Servo] | Robotics-flavored sensing | Distance measurement, servo control, sweep logic |
| [Smart Environment Monitor] | Multi-peripheral integration | Combines sensing + display + networking + actuation |
| [Multi-Zone Environmental Controller] | Full system design | Finite state machine, hysteresis control, MQTT pub/sub |

## Tools & Stack
- **Platform:** ESP32 (Arduino framework, C++)
- **Simulation:** Wokwi
- **Protocols used:** I2C, HTTP, MQTT
- **Libraries:** Adafruit GFX/SSD1306, DHT sensor library, ESP32Servo, PubSubClient

## Note on simulation vs. hardware
All projects were designed and tested in Wokwi's simulator. I'm working toward validating these on physical ESP32 hardware next — simulations let me iterate quickly on logic and wiring before committing to physical builds.

## What I'm exploring next
- Custom PCB design (KiCad)
- Deploying one of these projects on real hardware
- Deeper control-systems and firmware architecture concepts
