#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"
#include "WiFi.h"
#include "config.h"

// Sinric Pro Callback Function (the part of the working place when on or off switch from cloud)
bool onPowerState(const String &deviceId, bool &state) {
  digitalWrite(RELAY_PIN, state ? LOW : HIGH); // Active Low Relay එකක් සඳහා
  return true; 
}

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  // Wi-Fi Connection Setup
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // SinricPro Hardware Setup
  SinricProSwitch& mySwitch = SinricPro[SWITCH_ID];
  mySwitch.onPowerState(onPowerState);
  SinricPro.begin(APP_KEY, APP_SECRET);
}
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"
#include "config.h"

// --- Global Variables for State & Debounce ---
bool lastRelayState = false;
bool lastSwitchState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // 50ms Debounce Time

// --- SinricPro Callback Function ---
bool onPowerState(const String &deviceId, bool &state) {
  Serial.printf("Device %s turned %s via Cloud\r\n", deviceId.c_str(), state ? "ON" : "OFF");
  digitalWrite(RELAY_PIN, state ? LOW : HIGH); // Active Low Relay Logic
  lastRelayState = state;
  return true; 
}

// --- Manual Switch Handler with Debounce ---
void handleManualSwitch() {
  bool currentSwitchState = digitalRead(SWITCH_PIN);

  if (currentSwitchState != lastSwitchState) {
    lastDebounceTime = millis();
    lastSwitchState = currentSwitchState;
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    static bool confirmedSwitchState = HIGH;
    
    if (currentSwitchState != confirmedSwitchState) {
      confirmedSwitchState = currentSwitchState;
      
      // Toggle Relay State
      lastRelayState = !lastRelayState;
      digitalWrite(RELAY_PIN, lastRelayState ? LOW : HIGH);

      // Send Event to SinricPro Cloud
      SinricProSwitch &mySwitch = SinricPro[SWITCH_ID];
      mySwitch.sendPowerStateEvent(lastRelayState);
      
      Serial.printf("Manual Switch toggled! Relay is now %s\r\n", lastRelayState ? "ON" : "OFF");
    }
  }
}

void setup() {
  Serial.begin(BAUD_RATE);
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Default OFF for Active Low Relay
  
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  // WiFi Connection
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

  // SinricPro Setup
  SinricProSwitch &mySwitch = SinricPro[SWITCH_ID];
  mySwitch.onPowerState(onPowerState);

  SinricPro.begin(APP_KEY, APP_SECRET);
}

void loop() {
  SinricPro.handle();
  handleManualSwitch();
}
void loop() {
  SinricPro.handle(); // to cloud communicate
}