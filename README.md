# RTOS-UART-based-sensor_data_logging

## Project Overview:

This project implements real-time task scheduling using FreeRTOS on an STM32F446RE microcontroller. 
Sensor data from the DHT11 Temperature & Humidity Sensor is collected and transmitted via Bolt IoT using MQTT to a Mosquitto MQTT Broker, 
enabling cloud-based logging and visualization. Debugging is performed via UART for real-time system monitoring.

## Features:

1) FreeRTOS-Based Task Scheduling
2) Real-time MQTT Cloud Logging
3) DHT11 Sensor Integration (Temperature & Humidity Monitoring)
4) UART Debugging & System Health Monitoring
5) Cloud Dashboard with Data Visualization
6) Watchdog Timer for System Fault Recovery

Hardware Requirements:

1) Microcontroller: STM32F446RE (Nucleo-64 Board) 
2) IoT Module: Bolt IoT Wi-Fi Module 
3) Sensor: DHT11 Temperature & Humidity Sensor 
4) Communication: UART  
5) Protocol: MQTT over Wi-Fi 
6) Debugging Tools: ST-Link / J-Link Debugger 

Software Requirements:

1) STM32CubeIDE 
2) Freertos Middleware
3) Mosquitto MQTT Broker
4) Bolt IoT Cloud

