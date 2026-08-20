# 🔌 Smart Home IoT Switch with Dual-Control & Debounce Logic

An enterprise-grade, hybrid IoT Smart Switch implementation using ESP8266 (NodeMCU), Sinric Pro Cloud, and Google Assistant integration. This system supports both **Cloud Voice/App Commands** and **Physical Wall Switch Control** with hardware-friendly software debouncing algorithm.

---

## 🌟 Key Features

- **Hybrid Dual Control:** Control appliances via Cloud Voice Assistants (Google Home/Alexa) or standard physical toggle switches.
- **Software Switch Debouncing:** Integrated 50ms non-blocking debounce algorithm (`millis()`) to prevent mechanical contact bounce.
- **Active-Low Relay Logic:** Configured default HIGH pin states to prevent unintended relay triggers on MCU restart.
- **Real-Time State Synchronization:** Instant state sync between physical toggles and cloud dashboard using WebSockets.
- **Modular Firmware Architecture:** Separation of sensitive parameters (`config.h`) from application logic for secure Git tracking.

---

## 🏗️ System Architecture

The following diagram illustrates the complete end-to-end data flow from User Interfaces to High Voltage Hardware Components:

```mermaid
graph TD
    %% User Interfaces
    subgraph UI ["User Control Interfaces"]
        A[Push Button]
        B[Mobile App / SinricPro App]
        C[Voice Assistant - Alexa / Google]
    end

    %% Cloud Services
    subgraph Cloud ["Cloud Platform"]
        D[SinricPro Cloud Server]
    end

    %% Hardware / Firmware
    subgraph Device ["ESP32 Controller Unit"]
        E[ESP32 Microcontroller]
        F[Relay Module]
    end

    %% External Load
    G[AC Load / Light Bulb]

    %% Connections
    A -- "Physical Interrupt (GPIO 4)" --> E
    C -- "Voice Commands" --> D
    B -- "Wi-Fi / REST API" --> D
    D <== "Wi-Fi / WebSockets (Real-time Sync)" ==> E
    E -- "Relay Driver Signal (GPIO 23)" --> F
    F -- "High Voltage Switching" --> G