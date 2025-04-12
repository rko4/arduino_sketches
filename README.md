# Arduino Sketches for Sensor Projects

A collection of Arduino sketches for interfacing sensors such as CO₂, PM2.5, temperature, and more with 7-segment displays. These projects were originally developed as experimental work by [rko4](https://github.com/rko4). The sketches showcase various approaches to reading sensor data and presenting it in a user-friendly format.

## Overview

This repository contains a variety of Arduino sketches including:

- **MH-Z19 Sensor Projects**
  - A basic sketch using the MH-Z19 sensor with a 7-segment display, implemented with an external library.
  - An advanced version that adds reset functionality, temperature measurement, and delta calculation to enhance the reading of CO₂ levels.
  
- **PMS5003 Sensor Project**
  - A sketch for the PMS5003 sensor that reads particulate matter (PM2.5) levels and outputs the readings on a 7-segment display.

These projects provide a foundation for hobbyists and developers interested in environmental sensing and display projects.

## Repository Structure

```
arduino_sketches/
├── libraries/                                # Custom or third-party libraries required by some sketches
├── mh-z19_7_segment_using_library/           # Sketch using MH-Z19 sensor with a 7-segment display (library-dependent)
├── mh_z19_on_7segment_with_reset_and_temperature_and_delta/  
│   # Advanced MH-Z19 sketch with additional reset, temperature, and delta features
└── pms5003_on_7_segment/                      # Sketch for reading PMS5003 sensor data and displaying it
└── LICENSE                                   # GNU GPL-3.0 License

```

## Hardware Requirements

- **Arduino Board:** Compatible with models such as Uno, Mega, etc.
- **Sensors:**
  - *MH-Z19 CO₂ Sensor* (for CO₂ detection)
  - *PMS5003 Particulate Matter Sensor* (for PM2.5 readings)
- **Display:** 7-segment display module (or any compatible display hardware)
- **Additional Components:** Breadboard, jumper wires, resistors, etc.

## Setup and Installation

### 1. Clone the Repository

You can either download the ZIP archive from GitHub or clone the repository using Git.

```bash
git clone https://github.com/rko4/arduino_sketches.git
```

### 2. Install Required Libraries

- Copy the folders from the `libraries/` directory into your Arduino libraries directory (typically located at `Documents/Arduino/libraries`).
- Alternatively, use the Arduino IDE’s Library Manager to install the required libraries.

### 3. Hardware Setup

- **Review the Code:**  
  Check each sketch’s source code for the pin assignments and sensor configurations.
- **Wire Your Hardware:**  
  Connect your sensors (e.g., MH-Z19, PMS5003) and the 7-segment display as specified in the sketches.
- **Adjust as Needed:**  
  Modify the sketches if your wiring or hardware setup differs.

### 4. Load and Upload a Sketch

- Open the desired sketch (e.g., `mh-z19_7_segment_using_library`) in the Arduino IDE.
- Verify and compile the code to ensure that all libraries are correctly referenced.
- Upload the sketch to your Arduino board.

## Usage

- **Customization:**  
  Each sketch includes inline comments that explain the sensor connections and logic. Feel free to adjust the code to suit your needs—such as formatting the display output or integrating new sensors.

- **Troubleshooting:**  
  - Double-check the wiring and connections.
  - Ensure all required libraries are installed.
  - Use the Serial Monitor for debugging output, if available.

## Notes

- **Project Status:**  
  These sketches were created as experimental or learning projects. You may need to adapt them to newer libraries, hardware revisions, or Arduino IDE versions.

- **Contributions:**  
  Suggestions, bug fixes, or feature improvements are welcome via pull requests or issues.

## License

This project is licensed under the [MIT License](LICENSE). You are free to use, modify, and distribute the code under the terms of the license.

