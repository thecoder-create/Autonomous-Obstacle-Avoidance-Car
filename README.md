# Autonomous-Obstacle-Avoidance-Car

## Overview
This project demonstrates an **Autonomous Obstacle Avoidance Car** built using the **TM4C123GH6PM (Tiva C) microcontroller**. The car uses **hardware PWM**, **IR distance sensors**, and **interrupt-driven controls** to autonomously detect obstacles and adjust its movement accordingly.

Developed as part of **CECS 347 – Embedded Systems Course**, this example project highlights embedded programming concepts including ADC usage, GPIO control, sensor interfacing, and basic motion logic for robotics applications.

---

## Features

- **Autonomous Navigation**: Avoids obstacles based on IR sensor readings.
- **PWM-Based Motor Control**: Hardware PWM adjusts motor speed for direction and turning.
- **Real-Time Sensing**: Uses median-filtered ADC readings for reliable distance measurements.
- **LED Indicators**: Onboard LEDs show current motion state (forward, reverse, turning).
- **Interrupt-Based Sampling**: System uses SysTick and GPIO interrupts for periodic sensor sampling and button input.

---

## Hardware Components

- **TM4C123GH6PM (Tiva C) Microcontroller**
- **IR Distance Sensors** (x3)
  - Mounted facing left, right, and forward
- **DC Motors** (x2) with H-Bridge Driver
- **LEDs** (Onboard PF1–PF3)
- **Pushbutton** (Onboard PF0)
- **Wheels and Chassis**

---

## Sensor Thresholds

| Distance (cm) | Voltage (V) | ADC Value (12-bit) |
|---------------|-------------|---------------------|
| 15            | 1.8         | 2233                |
| 20            | 1.39        | 1724                |
| 30            | 0.9         | 1116                |
| 40            | 0.74        | 918                 |
| 80            | 0.4         | 496                 |

---

## Movement Logic

- **Forward Movement**: Default state when no obstacle is near.
- **Stop or Reverse**: If an object is closer than 20 cm in front.
- **Pivot Left/Right**: When obstacle is only on one side.
- **LED Colors**:
  - Green: Pivot Right
  - Blue: Pivot Left or Stop (far object)
  - Red: Reverse
  - Off: Forward

---

## File Structure

- `main.c`: Main program with system initialization, control loop, and steering logic.
- `PWM.c/h`: PWM initialization and duty cycle control.
- `GPIO.c/h`: GPIO setup for motors, LEDs, and buttons.
- `Sensors.c/h`: ADC and IR sensor reading logic.
- `LEDSW.c/h`: LED and switch configuration.
- `PLL.c/h`: Clock setup for 80 MHz system bus.

---

## Build & Flash Instructions

1. Open the project in **Keil uVision** or another ARM IDE supporting TM4C123GH6PM.
2. Make sure all `.c` and `.h` files are included in the project.
3. Compile and build the project.
4. Flash the binary to the Tiva board using a debugger or USB connection.

---

## How to Use

1. Power the system and press the onboard **button (SW1)** to start.
2. The car begins moving forward and autonomously adjusts motion based on sensor inputs.
3. Use LEDs to monitor its current action.

---


