# ESP32-WebCommunication

A lightweight library for communication between ESP32 boards on the same network. This library provides a simple web dashboard interface for monitoring and controlling ESP32 devices.

## Features

- Create interactive web dashboards for ESP32 devices
- Support for multiple component types (buttons, toggles, sliders, text inputs, etc.)
- Automatic device discovery on local networks
- Real-time data updates via WebSockets
- Low memory footprint and efficient design
- Simple API suitable for beginners

## Installation

### Using PlatformIO

1. Add the following to your `platformio.ini`:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino

lib_deps =
    AcierDev/ESP32-WebCommunication
    PubSubClient
    ArduinoJson
    me-no-dev/ESPAsyncWebServer
    me-no-dev/AsyncTCP
```

### Manual Installation

1. Download the latest release from the [releases page](https://github.com/AcierDev/ESP32-WebCommunication/releases)
2. In the Arduino IDE, go to Sketch > Include Library > Add .ZIP Library and select the downloaded file
3. Install the required dependencies:
   - PubSubClient
   - ArduinoJson
   - ESP Async WebServer
   - AsyncTCP

## Quick Start

```cpp
#include <Arduino.h>
#include <WebDashboard.h>

const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

WebDashboard dashboard;

// Button callback
void handleButton(const char* id) {
  dashboard.log("Button pressed!");
}

// Toggle callback
void handleToggle(const char* id, bool state) {
  digitalWrite(LED_BUILTIN, state ? HIGH : LOW);
  dashboard.logf(LOG_INFO, "Toggle %s set to %s", id, state ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize the dashboard
  dashboard.begin("ESP32 Dashboard", "My Device");

  // Add components
  dashboard.addButton("btn1", "Press Me", handleButton);
  dashboard.addToggle("toggle1", "LED Control", false, handleToggle);
  dashboard.addSlider("slider1", "Brightness", 0, 255, 128);

  // Log some messages
  dashboard.log("Dashboard initialized");
}

void loop() {
  dashboard.update();
  delay(10);
}
```

## Component Types

### Buttons

Simple push buttons that trigger a callback when pressed.

### Toggles

On/off switches that maintain state and trigger callbacks when toggled.

### Sliders

Range input controls with configurable min/max values and step size.

### Text Inputs

Text fields for entering string data.

### Select Dropdowns

Dropdown menus with configurable options.

### Pin Monitors

Real-time monitoring of digital or analog pins.

### Machine States

Display and control state machines with predefined states.

## API Reference

See the [full documentation](https://acierdev.github.io/ESP32-WebCommunication/) for complete API details.

## License

This library is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the project
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request
