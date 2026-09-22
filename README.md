# Weather Report

<p align="center">
  <img src="assets/logo.png" alt="Weather Report Logo" width="200"/>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/ESP--IDF-v6.1.0-blue?style=for-the-badge&logo=espressif" alt="ESP-IDF">
  <img src="https://img.shields.io/badge/FreeRTOS-Kernel-green?style=for-the-badge&logo=FreeRTOS" alt="FreeRTOS">
  <img src="https://img.shields.io/badge/LVGL-v9-orange?style=for-the-badge" alt="LVGL">
  <img src="https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge" alt="License">
</p>

## Overview

This project displays real-time temperature and humidity data on an ESP32 device with a 320×170 IPS LCD screen. It uses DHT11 or DHT22 sensors for environmental data acquisition, LVGL for graphical interface rendering, and is built using the ESP-IDF framework.

The goal is to provide a lightweight, responsive, and visually appealing embedded weather monitoring system for educational and prototyping purposes.

## Demo

<p align="center">
  <img src="assets/demo.jpeg" alt="Project demonstration" width="400" />
</p>

## Features

- Real-time temperature and humidity monitoring
- Support for DHT11 and DHT22 sensors
- 320×170 IPS LCD display support
- Smooth graphical interface using LVGL
- Built with ESP-IDF (Espressif official framework)
- Lightweight and optimized for embedded systems

## Key Technical Highlights

- **Event-Driven Queue Decoupling:** The DHT sensor task (`vReadTempTask`) and the LVGL render task (`vDrawTask`) communicate exclusively via a non-blocking FreeRTOS `QueueHandle_t`.
- **Data Redundancy Filtering:** Sensor readings are filtered at the source. Data is only dispatched to the queue when an actual change in temperature or humidity occurs, avoiding unnecessary IPC and UI redraws.
- **Optimized UI Loop:** Framerate controlled through `lv_timer_handler()` combined with `vTaskDelay` to enforce a stable ~60 FPS cap while guaranteeing 0% CPU usage during idle periods.
- **Defensive Error Handling:** Built-in robustness without hardware auto-reboots on temporary sensor read glitches.

## Hardware Requirements

- ESP32 development board
- 320×170 IPS LCD display
- DHT11 or DHT22 sensor
- Jumper wires
- Breadboard (optional)

## Software Requirements

- ESP-IDF (Espressif IoT Development Framework)
- LVGL (Light and Versatile Graphics Library)
- FreeRTOS (included in ESP-IDF)

## Installation

```bash
git clone https://github.com/NisaelMoreiraGomes/weather-report.git

cd weather-report

git submodule update --init --recursive

idf.py set-target esp32
idf.py build
idf.py flash
```

## License

This project is licensed under the MIT License.
