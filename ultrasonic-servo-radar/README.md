# Ultrasonic Sensor + Servo Sweep

A servo sweeps an HC-SR04 ultrasonic sensor back and forth like a mini radar, measuring distance at each angle and flagging nearby obstacles.

![circuit](./circuit.gif)

## How it works
1. The servo sweeps between 0° and 180°, reversing direction at each end
2. At each angle, the HC-SR04 sends a pulse and measures the time for it to bounce back (`pulseIn`)
3. Distance is calculated from the pulse duration using the speed of sound
4. If an object is within 15cm, an "obstacle detected" flag is logged

## Wiring
| Component | ESP32 Pin |
|---|---|
| HC-SR04 VCC | 5V (or 3.3V) |
| HC-SR04 Trig | GPIO 5 |
| HC-SR04 Echo | GPIO 18 |
| HC-SR04 GND | GND |
| Servo Signal | GPIO 13 |
| Servo Power | 5V (or 3.3V) |
| Servo GND | GND |

## Libraries
```
ESP32Servo
```

## Code
See [`sketch.ino`](./sketch.ino)

## Concepts learned
- Ultrasonic distance measurement (`pulseIn`, speed-of-sound conversion)
- Servo angle control via PWM
- Simple sweep/reverse-direction control logic — a pattern used across robotics and animation
