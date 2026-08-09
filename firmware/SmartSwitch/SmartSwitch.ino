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

void loop() {
  SinricPro.handle(); // to cloud communicate
}