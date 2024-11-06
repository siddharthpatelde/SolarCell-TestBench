# Solar Cell Test Bench Project

## Overview
This project aims to develop a test bench for analyzing the voltage and current behavior of a solar cell under varying surrounding temperatures. The project is a collaborative effort between **Konstantin Sang** and **Siddhart Patel**. Konstantin is responsible for the mechanical design and literature, while Siddhart handles the electronics and programming aspects.

## Project Goal

1. **Monitoring surrounding temperature** with two DHT22 sensors.
2. **Controlling a heating element** through a relay and MOSFET setup.
3. **Implementing a P algorithm** for maintaining a desired temperature range, using real-time temperature feedback.

*More mini goals will be added in the future as the project progresses.*

## Current Work Steps

### Step 1: Temperature Monitoring
- Utilize two DHT22 temperature sensors to capture the ambient temperature.
- Calculate the **RMS** and **average** temperature values from both sensors to ensure a precise and stable reading.

**Status:** DONE

**Video Demonstration:**  
[![Step 1 Video](images/thumbnail_1.png)](https://drive.google.com/file/d/1SOkulpQlxXZBvUerqa7_w0ityK85tI0_/view?usp=sharing)

**Library Used:**  
- [DHT Sensor Library by Adafruit](https://github.com/adafruit/DHT-sensor-library)

### Step 2: Temperature Control
- Integrate a heating element, controlled by a relay and MOSFET, to adjust the surrounding temperature.
- Program the microcontroller to turn the heating element on or off as needed to reach target temperatures.

**Status:** In Progress

### Step 3: Implementing a Control Algorithm
- Develop a **Proportional (P) control algorithm** that uses the RMS temperature value as feedback.
- Adjust the heating element in response to deviations from the desired temperature, helping stabilize the environment for the solar cell.

**Status:** In Progress

*More steps will be added in the future*

### Prerequisites
- Arduino or compatible microcontroller
- DHT22 temperature sensors (2x)
- Relay and MOSFET for heating element control
- Heating element

### Installation
1. Clone this repository to your local machine.
   ```bash
   git clone https://github.com/your-username/solar-cell-test-bench.git

## Author

**Siddharth A. Patel**  
GitHub: [siddharthpatelde](https://github.com/siddharthpatelde?tab=overview&from=2024-10-01&to=2024-10-17)

## Credits
- **Circuit Design and Electronics Programming**: All coding, electronics, and circuit design are done by Siddharth A. Patel.
- **Project Introduction and Mechanical/Literature Work**: Konstantin Sang introduced the project and is responsible for the mechanical design and literature.