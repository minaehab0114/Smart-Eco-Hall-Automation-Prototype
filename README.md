# Smart-Eco-Hall-Automation-Prototype
Smart Room Automation System — Built an automated lighting and HVAC control system using Arduino UNO, a PIR motion sensor, and C++ with anti-flicker logic.
<h1 align="center">
  🌐 Smart Eco Hall Automation Prototype
</h1>

<p align="center">
  <b>An automated, energy-efficient lighting and HVAC control system powered by Arduino UNO, featuring active-LOW relay control, presence-detection calibration, and anti-flicker logic.</b>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Microcontroller-Arduino%20UNO-00979D?style=for-the-badge&logo=arduino&logoColor=white" />
  <img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
  <img src="https://img.shields.io/badge/Domain-Embedded%20Systems-E7352C?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Status-Completed-007A3D?style=for-the-badge" />
</p>

---

## 🌟 Overview
The **Smart Eco Hall Automation System** is a micro-controller prototype designed to minimize energy consumption in communal or residential spaces by dynamically controlling high-load appliances (Lighting and HVAC systems) based on real-time occupancy. Utilizing an Arduino UNO paired with Passive Infrared (PIR) sensing technology, the firmware ensures reliable relay actuation while protecting electrical contacts from high-frequency switching wear.

---

## ✨ Key Features
- **Real-Time Occupancy Sensing:** Instantaneous detection of human presence via wide-angle PIR motion sensor.
- **Warm-Up Calibration Protocol:** Integrated 15-second initialization phase upon boot to calibrate ambient infrared baseline levels and eliminate false-positive triggers.
- **Active-LOW Safety Logic:** Firmware-configured relay driver logic tailored for optocoupler-isolated relay modules to ensure fail-safe operation.
- **Hysteresis & Anti-Flicker Protection:** Software-implemented stability delay to prevent rapid relay chatter/flickering caused by subtle motion boundaries.
- **Simulated Load Diagnostics:** Integrated LED visual indicators with current-limiting resistors for safe low-voltage testing before AC mains deployment.

---

## 📐 System Architecture

| Stage | Component | Function |
| :--- | :--- | :--- |
| **Input Sensing** | PIR Motion Sensor (HC-SR501) | Detects thermal IR variance from human presence |
| **Processing Core** | Arduino UNO (ATmega328P) | Executes control loops, calibration, and logic timing |
| **Actuation** | 2-Channel Relay Module | Switches high-voltage AC / DC loads (Lighting & HVAC) |
| **Indication** | LEDs + 220Ω Resistors | Low-voltage visual diagnostic feedback |

---

## 🛠️ Hardware Bill of Materials (BOM)
- **Microcontroller:** Arduino UNO R3 (ATmega328P)
- **Occupancy Sensor:** PIR Motion Sensor (HC-SR501)
- **Actuator Module:** 5V 2-Channel Relay Module (Optocoupler Isolated)
- **Testing Indicators:** 2x 5mm LEDs (Green: Lighting, Blue: HVAC)
- **Protection:** 2x 220Ω Resistors (¼W)
- **Prototyping:** Solderless Breadboard & Premium Jumper Wires

---

## 🔌 Circuit & Wiring Pinout

| Arduino UNO Pin | Target Component | Connection Type / Logic |
| :---: | :---: | :--- |
| **D2** | PIR Sensor (OUT) | Digital Input |
| **D7** | Relay IN1 (Lighting) | Digital Output (Active-LOW) |
| **D8** | Relay IN2 (HVAC) | Digital Output (Active-LOW) |
| **5V** | PIR & Relay VCC | Power Supply (+) |
| **GND** | PIR & Relay GND | Common Ground (-) |

<p align="center">
  <img src="Screenshot 2026-09-15 043240.png" alt="Hardware Setup" width="80%" />
</p>
---

## 💻 Firmware Code (C++)

```cpp
/**
 * Project: Smart Eco Hall Automation System
 * Author: Mina Ehab
 * Platform: Arduino UNO / ATmega328P
 */

const int PIR_PIN = 2;        // PIR Sensor Input Pin
const int RELAY_LIGHT = 7;    // Relay Channel 1: Lighting (Active-LOW)
const int RELAY_HVAC = 8;     // Relay Channel 2: HVAC (Active-LOW)

const unsigned long CALIBRATION_TIME_MS = 15000; // 15-second PIR sensor calibration
const unsigned long STABILITY_DELAY_MS = 5000;   // 5-second off delay to prevent flickering

void setup() {
  Serial.begin(9600);
  
  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_LIGHT, OUTPUT);
  pinMode(RELAY_HVAC, OUTPUT);

  // Initial state for Active-LOW Relays (HIGH = OFF)
  digitalWrite(RELAY_LIGHT, HIGH);
  digitalWrite(RELAY_HVAC, HIGH);

  Serial.println("=========================================");
  Serial.println("[SYSTEM INIT] Calibrating PIR Sensor...");
  delay(CALIBRATION_TIME_MS);
  Serial.println("[SYSTEM READY] Occupancy detection active.");
  Serial.println("=========================================");
}

void loop() {
  int motionDetected = digitalRead(PIR_PIN);

  if (motionDetected == HIGH) {
    Serial.println("[STATE] Motion Detected! Actuating Lighting & HVAC...");
    digitalWrite(RELAY_LIGHT, LOW);  // Turn ON Lighting (Active LOW)
    digitalWrite(RELAY_HVAC, LOW);   // Turn ON HVAC (Active LOW)
  } else {
    Serial.println("[STATE] No Motion. Holding state before shutdown...");
    delay(STABILITY_DELAY_MS);       // Anti-flicker delay interval
    
    if (digitalRead(PIR_PIN) == LOW) {
      digitalWrite(RELAY_LIGHT, HIGH); // Turn OFF Lighting
      digitalWrite(RELAY_HVAC, HIGH);  // Turn OFF HVAC
      Serial.println("[STATE] Space Unoccupied. Loads deactivated.");
    }
  }

  delay(200); // Polling stability throttling
}
