# GSM Based Home Security System using PIR Sensor and Arduino Uno

A low-cost, internet-free home security system built with an **Arduino Uno**, a **PIR motion sensor**, and a **GSM module (SIM900/SIM800A/SIM800L)**. When motion is detected, the system instantly alerts the homeowner via **SMS and/or phone call** — no Wi-Fi or internet connection required.

> Submitted for CSE-211 & CSE-201 (Computer System Organisation and Architecture), IIT (BHU) Varanasi.

---

## Demo Video

<!--
  HOW TO ADD YOUR VIDEO:
  1. Go to this repo on GitHub.com, open any Issue (or create a new one) or edit this README directly in the GitHub web editor.
  2. Drag and drop your video file into the comment/edit box.
  3. GitHub will upload it and auto-insert a link like:
     https://github.com/user-attachments/assets/xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
  4. Copy that generated markdown (it looks like a video tag or plain link) and paste it below, replacing this comment.
  This is the only reliable way to get a playable video embedded directly in a GitHub README.
-->

**[📹 Add your demo video here — see instructions above]**

---

## Overview

Traditional security systems (CCTV, Wi-Fi based alarms) often fail during power cuts or network outages, and can be expensive to install and maintain. This project solves that by using **GSM connectivity** instead of the internet, making it reliable, affordable, and usable even in rural areas with poor internet access.

## Features

- 🔍 Real-time motion detection using a PIR sensor (range ~6 meters, ~110°–120° detection angle)
- 📞 Instant phone call / SMS alert to the homeowner via GSM
- 🔁 Automatic retry (up to 3 attempts) if the first call isn't answered
- 🧹 Software-based debounce/filtering to reduce false alarms from heat, light, or air movement
- 🔌 Works completely offline — no Wi-Fi or internet dependency
- 🧩 Modular design — easily extendable with fire, gas, or smoke sensors
- 💡 Local buzzer/LED alert in addition to remote notification

## Hardware Components

| Component | Purpose |
|---|---|
| Arduino Uno | Main controller / processing unit |
| PIR Motion Sensor | Detects movement via infrared radiation |
| GSM Module (SIM900 / SIM800A / SIM800L) | Sends SMS / makes calls to alert homeowner |
| Buzzer + LED | Local visual/audio alert indicators |
| Buck Converter | Steps down 12V input to ~4V for GSM module |
| Breadboard, jumper wires, resistors | Supporting circuit components |

## Circuit Connections

| Component | Arduino Pin | Connection |
|---|---|---|
| PIR Sensor OUT | D10 | Reads motion signal |
| PIR Sensor VCC | 5V | Power from Arduino |
| PIR Sensor GND | GND | Common ground |
| GSM TX (SIM800L TXD) | D3 | Arduino RX |
| GSM RX (SIM800L RXD) | D2 | Arduino TX |
| GSM GND | GND | Common ground |
| GSM VCC | From Buck Converter (≈4V) | GSM power input |
| Buck Converter Input | 12V Adapter | Power source |

## How It Works

1. The PIR sensor continuously monitors its surroundings for infrared changes caused by motion.
2. When motion is detected, it sends a HIGH signal to the Arduino Uno.
3. The Arduino sends AT commands over serial to the GSM module.
4. The GSM module sends an SMS and/or places a call to the homeowner's registered mobile number.
5. If the call isn't answered, the system automatically retries (up to 3 times).
6. The system then returns to normal monitoring mode.

```
PIR Sensor → Arduino Uno → GSM Module → Mobile Phone (Alert Message / Call)
```

## Software

The firmware is written in **C++ (Arduino/C++ dialect via the Arduino IDE)**. See [`GSM_Home_Security.ino`](./GSM_Home_Security.ino).

Before uploading, update the homeowner's phone number in the sketch:

```cpp
const char PHONE_NUMBER[] = "+91XXXXXXXXXX"; // TODO: set homeowner's number
```

### Requirements
- [Arduino IDE](https://www.arduino.cc/en/software)
- `SoftwareSerial` library (bundled with Arduino IDE)

### Uploading
1. Connect the Arduino Uno via USB.
2. Open `GSM_Home_Security.ino` in the Arduino IDE.
3. Select the correct board (Arduino Uno) and COM port.
4. Update `PHONE_NUMBER`.
5. Upload the sketch.
6. Open the Serial Monitor (9600 baud) to view debug logs (`Motion Detected!`, `Sending SMS...`, etc.).

## Results

- Motion detected accurately within a 6-meter range with minimal false alarms.
- Alert delay of ~2–3 seconds between detection and notification.
- Retry mechanism improved reliability when the first call attempt failed.
- Fully functional without internet connectivity — suitable for both urban and rural deployment.

## Limitations & Future Scope

- Dependent on GSM network coverage.
- May not reliably detect very slow or minor movements (inherent PIR limitation).
- Future improvements: additional sensors (fire, smoke, gas), cloud/IoT-based monitoring, and a companion mobile app for remote control.

## Team

| Name | Roll No. |
|---|---|
| Sunita | 24075087 |
| Ullengula Jhansi | 24075089 |
| Arpita Singh | 24075012 |
| Vaishali Gautam | 24075093 |
| Noone Mounika | 24075061 |

**Submitted to:** Dr. Prasenjit Chanak, Department of Computer Science, IIT (BHU) Varanasi

## License

This project is open-sourced for educational purposes. Feel free to fork and build upon it.
