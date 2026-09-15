/**
 * Project: Smart Eco Hall Automation System
 * Author: Mina Ehab
 * Platform: Arduino UNO / ATmega328P
 * Description: Automated lighting & HVAC control system using PIR sensor 
 *              with active-LOW relay logic and anti-flicker protection.
 */

// Pin Definitions
const int PIR_PIN = 2;        // PIR Motion Sensor Output Pin
const int RELAY_LIGHT = 7;    // Relay Channel 1: Lighting (Active-LOW)
const int RELAY_HVAC = 8;     // Relay Channel 2: HVAC (Active-LOW)

// Timing Constants (in milliseconds)
const unsigned long CALIBRATION_TIME_MS = 15000; // 15 seconds for PIR thermal calibration
const unsigned long STABILITY_DELAY_MS = 5000;   // 5 seconds anti-flicker delay before turning off

void setup() {
  // Initialize Serial Communication for Debugging
  Serial.begin(9600);
  
  // Configure Pin Modes
  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_LIGHT, OUTPUT);
  pinMode(RELAY_HVAC, OUTPUT);

  // Set initial Active-LOW Relay states to OFF (HIGH = OFF)
  digitalWrite(RELAY_LIGHT, HIGH);
  digitalWrite(RELAY_HVAC, HIGH);

  // Sensor Calibration Phase upon Startup
  Serial.println("=========================================");
  Serial.println("[SYSTEM INIT] Calibrating PIR Sensor...");
  delay(CALIBRATION_TIME_MS);
  Serial.println("[SYSTEM READY] Occupancy detection active.");
  Serial.println("=========================================");
}

void loop() {
  // Read motion detection status
  int motionDetected = digitalRead(PIR_PIN);

  if (motionDetected == HIGH) {
    // Motion sensed: Turn ON Lighting & HVAC (Active-LOW logic: LOW = ON)
    Serial.println("[STATE] Motion Detected! Actuating Lighting & HVAC...");
    digitalWrite(RELAY_LIGHT, LOW);
    digitalWrite(RELAY_HVAC, LOW);
  } else {
    // No motion: Wait for stability delay to prevent relay flickering
    Serial.println("[STATE] No Motion. Holding state before shutdown...");
    delay(STABILITY_DELAY_MS);
    
    // Double-check PIR state after delay before turning loads off
    if (digitalRead(PIR_PIN) == LOW) {
      digitalWrite(RELAY_LIGHT, HIGH); // Turn OFF Lighting
      digitalWrite(RELAY_HVAC, HIGH);  // Turn OFF HVAC
      Serial.println("[STATE] Space Unoccupied. Loads deactivated.");
    }
  }

  // Small delay for stability throttling
  delay(200);
}
