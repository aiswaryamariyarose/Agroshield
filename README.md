# Kala Sahaayi
![POSTER](https://github.com/user-attachments/assets/1b249f59-8c35-4bea-baf9-96a6aa4e678b)



# Basic Details

Team Name: Kaapi Addicts

Team Members:

Member 1: Aiswarya Mariya Rose- [MITS]

Member 2: Sania Susan Reji - [MITS]

Hosted Project Link: https://drive.google.com/file/d/17V3BidqnooIWotr87QdwmgptobOrhIdD/view?usp=drivesdk


# VIDEO DEMO LINK
https://drive.google.com/file/d/17V3BidqnooIWotr87QdwmgptobOrhIdD/view?usp=drivesdk


# Project Description

Kala Sahaayi is a smart agriculture safety system that protects crops from wild animals using sensors and alerts. It also monitors water levels to ensure optimal irrigation automatically and suggest the crops to be planted in the farm land

# The Problem Statement

Farmers face losses due to wild animals invading crops, and inefficient irrigation often leads to water wastage and damage of water pump or insufficient water for plants.

# The Solution

Kala sahaayi uses sensors to detect animal intrusion and alerts the farmer via LED and buzzer. It also monitors water levels using ultrasonic sensors and automatically controls the water pump, reducing crop loss and water wastage.

# Technical Details
Technologies / Components Used

Software:

Languages: Python, Arduino C/C++

Frameworks: None (Python scripts with libraries)

Libraries: OpenCV, dlib, Adafruit_SSD1306, Servo, JSON

Tools: Arduino IDE, VS Code, Git

Hardware:

Main Components: Arduino Uno, IR sensor, Ultrasonic sensor, Servo motor, Buzzer, LED, OLED display, Relay module, Touch sensor

Specifications: Arduino Uno (ATmega328P, 16MHz), OLED 0.96” I2C, Servo SG90 9g, Relay 5V

Tools Required: Jumper wires, Breadboard, Power supply, Screwdriver, Soldering kit (optional)

# Features

Animal Detection: Detects wild animals approaching crops using camera.

Alerts: LED and buzzer alert farmer immediately.

Water Monitoring: Measures water levels in tanks and stops pumps when below 20%.

OLED Display: Shows real-time animal intrusion or water level data.

Touch Sensor Mode Switch: Switch between water monitoring and animal detection display.

# Implementation
Software

Installation:

pip install -r requirements.txt

Run:

python agroshield_surveillance.py


Hardware

Components Required:

Arduino Uno

IR Sensors 

Ultrasonic Sensor

Servo Motor

Buzzer

LED

OLED Display

Touch Sensor

Relay Module

Jumper wires

Breadboard

Power Supply


# Circuit Setup:

Connect IR sensors to D2 and D3.

Ultrasonic sensor TRIG to D4, ECHO to D5.

Servo motor to D9.

Buzzer to D6, LED to D7 (with resistor).

Relay module IN to D8.

OLED SDA → A4, SCL → A5.

Touch sensor SIG → D10.

Power everything via 5V and GND from Arduino.



# Project Documentation
<img width="1899" height="894" alt="Screenshot 2026-02-28 083444" src="https://github.com/user-attachments/assets/468e6d26-7559-413a-bc38-906e24d109ba" />

<img width="842" height="773" alt="Screenshot 2026-02-28 083454" src="https://github.com/user-attachments/assets/29028054-a609-410a-b229-c43b2591f294" />
<img width="842" height="766" alt="Screenshot 2026-02-28 083514" src="https://github.com/user-attachments/assets/428777a0-f902-42eb-ad5e-ba2c6e7e1b7b" />


![1](https://github.com/user-attachments/assets/fa41324a-5f6c-4846-af46-1927168f0f03)
![2](https://github.com/user-attachments/assets/41a27d2c-9a2c-4464-a2fb-8db2cc2ac265)

![3](https://github.com/user-attachments/assets/884dee5f-df49-4c5d-87f2-1b3170c0188b)
![4](https://github.com/user-attachments/assets/6b440008-f2bc-4b12-bc77-8ee557a27da0)





Diagrams

System Architecture:

Sensors → Arduino → OLED/Buzzer/LED/Relay → Dashboard

Data flow: Animal detection & water level data sent to local Python scripts → JSON → Dashboard

Application Workflow:

Sensors detect input

Arduino processes signals

Alerts displayed via LED, buzzer, and OLED

Water pump controlled automatically if levels low


Hardware

Schematic & Circuit:

![Circuit Diagram](Add circuit diagram path)
Shows Arduino connections with sensors, buzzer, LED, relay, servo, and OLED

![Schematic](Add schematic path)
Detailed schematic for hardware setup

Build Photos:

![Team](Add team photo path)
<img width="1899" height="894" alt="Screenshot 2026-02-28 083444" src="https://github.com/user-attachments/assets/2de54bc2-83c2-4db0-87d4-04c94373c56b" />
<img width="842" height="773" alt="Screenshot 2026-02-28 083454" src="https://github.com/user-attachments/assets/2380f9ba-cfa9-4191-ac70-63239d19acf7" />
<img width="842" height="766" alt="Screenshot 2026-02-28 083514" src="https://github.com/user-attachments/assets/d23a22c0-62ed-4d39-9326-5dc84d90ab1f" />

![Uploa![1](https://github.com/user-attachments/assets/c134b1c8-6ca1-4da1-b11f-47026d5ab5c2)
![2](https://github.com/user-attachments/assets/76b91df8-1573-4628-ab2c-8e8a76a3f507)
![3](https://github.com/user-attachments/assets/a108f3e7-ccd5-407f-9fb3-b805727445e8)
![4](https://github.com/user-attachments/assets/0a0edb79-6bce-4de8-b545-ff6f6fcd4fa0)
![5](https://github.com/user-attachments/assets/b2574aa9-cf92-4b5f-ae0a-2316b3d802f8)



# Bill of Materials (BOM)
Component	Quantity	Specifications	Price (₹)	Source/Link
Arduino Uno	1	ATmega328P, 16MHz	450	[Link]
IR Sensor	2	Digital, 5V	100	[Link]
Ultrasonic Sensor	1	HC-SR04, 5V	150	[Link]
Servo Motor	1	SG90, 9g	200	[Link]
Buzzer	1	5V Piezo	50	[Link]
LED	5	5mm, 20mA	5 each	[Link]
OLED Display	1	0.96" I2C, 128x64	400	[Link]
Touch Sensor	1	TTP223 Module	100	[Link]
Relay Module	1	5V Single Channel	150	[Link]
Jumper Wires	20	Male-to-male	50	[Link]
Breadboard	1	830 points	100	[Link]
Power Supply	1	5V, 2A Adapter	300	[Link]

Total Estimated Cost: ₹2060

# Assembly Instructions

Prepare Components → Check specs and lay them out.

Build Power Supply → Connect Arduino 5V and GND to breadboard rails.

Add LEDs → Connect resistors, cathodes to GND, anodes to Arduino pins.

Connect IR and Ultrasonic sensors → Pins D2-D5.

Connect Servo → Pin D9.

Connect Buzzer & Relay → Pins D6, D8.

Connect OLED → SDA/A4, SCL/A5.

Connect Touch Sensor → Pin D10.

Power up Arduino → Upload AgroShield.ino.

Test sensors individually → Verify OLED, LED, buzzer, and water pump control.

Command Reference (Python Scripts)

Basic Usage:

python agroshield_surveillance.py

Options:

-h, --help → Show help

-v, --verbose → Verbose output

-o, --output FILE → Save output JSON

Examples:

# Basic usage
python agroshield_surveillance.py

# Save output to JSON
python agroshield_surveillance.py -o intrusion_data.json
Project Demo Video

Demo Video Link

https://drive.google.com/file/d/17V3BidqnooIWotr87QdwmgptobOrhIdD/view?usp=drivesdk

AI Tools Used 

Tool Used: ChatGPT

Purpose: Debugging and documentation formatting

Human Contributions: Architecture design, hardware integration, code implementation, testing, UI/UX design

Team Contributions

Aiswarya: Arduino coding, sensors integration, OLED & buzzer setup

Sania: Python scripts, dashboard design, testing, documentation

License

This project is licensed under the MIT License – see License file for details.

Made with ❤️ at TinkerHub
