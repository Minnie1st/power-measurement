# Power-measurement
This project provides a low-cost solution for monitoring power usage in a factory setting using the ESP32 microcontroller and the Kancony M16V2 current sensor board. It helps identify which machines consume the most electricity by measuring real-time current on up to 16 channels through a single analog input using time-multiplexing.

Using the True RMS algorithm, the system calculates accurate current values, then transmits the data over the MQTT protocol. A Node-RED dashboard is used to visualize current usage and trigger alerts when abnormal values are detected.

This project is especially useful for small factories in developing regions, where minimizing energy costs is critical and budget-friendly solutions are needed.

## Hardware Used
ESP32 Microcontroller (embedded in KC868)

Kancony M16V2 current sensor board (up to 16 channels)

Analog input via GPIO35

3.3V reference voltage, 12-bit ADC resolution

