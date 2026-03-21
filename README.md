# STM32C8T6 Smart Car with Line Tracking, Obstacle Avoidance, and Bluetooth Control

## Project Overview
This project implements a smart STM32C8T6-based car that integrates line tracking, infrared obstacle avoidance, and Bluetooth remote control.
The car can autonomously follow a path, detect and avoid obstacles, and respond to remote commands via Bluetooth, providing both automatic and manual operation modes.

## File Description
`user/main.c`  
Main program entry, responsible for system initialization and control of line tracking, obstacle avoidance, and Bluetooth modules.

`hardware/`  
Contains hardware abstraction modules:
- `CAR` – Motor and movement control
- `LED` – Status indicators
- `MOTO` – Motor driver interface
- `HCSR` – Ultrasonic sensor module
- `OLED` – Screen display
- `PWM` – PWM signal generation
- `SERIAL` – UART communication
- `SERVO` – Servo control
- `TRACK` – Line tracking sensor management


## Features
- Line tracking using infrared sensors with automatic correction and turning.
- Infrared obstacle avoidance with distance detection; automatically chooses turning or backing to avoid obstacles.
- Bluetooth control via serial commands for remote forward, backward, turning, or stop.
- LED indicators and OLED display for status visualization.
- Multi-module integration allowing Bluetooth commands to override autonomous line tracking.

## Usage Instructions
1. Power on the car.
2. Line tracking module enables autonomous path following.
3. Use Bluetooth commands to manually control movement or override line tracking.
4. The car automatically detects obstacles in front and reroutes using ultrasonic sensors.
5. Check LEDs and OLED for real-time status updates.
6. If you want to take a closer look at the project files, you can open the **STM32C8T6_Smart_Car_Module_Function_Explanation(Chinese Version)** document. But that it is in Chinese.
7. Bluetooth Control
