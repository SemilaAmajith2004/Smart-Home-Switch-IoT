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
%%{init: {'theme': 'base', 'themeVariables': { 'primaryColor': '#E3F2FD', 'edgeLabelBackground':'#FFFFFF', 'tertiaryColor': '#fff'}}}%%

graph TD
    subgraph UI [User Interface Layer]
        A[User Voice Command]:::ui
        D[Physical Wall Switch\nToggle/Push Button]:::ui
    end

    subgraph Services [Cloud & Connectivity Layer]
        B{Google Assistant\nNLP & Action}:::cloud
        C{Sinric Pro\nCloud Platform}:::cloud
        Hub((Data Hub)):::logic 
    end

    subgraph MCU [Control Layer - NodeMCU ESP8266]
        E(ESP8266 Core\nFirmware Loop):::mcu
        GPIO_In[GPIO Input\nInterface D2]:::logic
        GPIO_Out[GPIO Output\nInterface D1]:::logic
        WiFi[Wi-Fi Stack\nWiFiManager]:::intern
        WebSocket[WebSocket Client\nSinricPro Lib]:::intern
    end

    subgraph Output [Hardware Layer]
        F[5V Relay Module\nActive Low/High]:::hard
        G[230V AC\nLight Bulb]:::hard
    end

    A -->|Starts Action| B
    B -->|API Request| C
    C -->|Secure Protocol| WebSocket

    WebSocket -->|Data Link| WiFi
    WiFi <-->|Control & Status| Hub
    Hub <-->|Sync Commands| E

    D -->|Signal Interrupt| GPIO_In
    GPIO_In -->|Trigger| E
    
    E -->|Write Logic| GPIO_Out
    GPIO_Out -->|Control Signal| F
    F -->|High Voltage Switch| G

    classDef ui fill:#D4E157,stroke:#333,stroke-width:2px,color:#000;
    classDef cloud fill:#90CAF9,stroke:#1E88E1,stroke-width:2px,color:#000;
    classDef mcu fill:#CE93D8,stroke:#8E24AA,stroke-width:2px,color:#000;
    classDef logic fill:#E0E0E0,stroke:#333,stroke-width:1px,stroke-dasharray: 5 5,color:#000;
    classDef intern fill:#FFF59D,stroke:#FBC02D,stroke-width:1px,color:#000;
    classDef hard fill:#EF9A9A,stroke:#E53935,stroke-width:2px,color:#000;