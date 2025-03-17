/**
 * AdvancedDashboard.ino - Advanced dashboard example for ESP32-WebCommunication
 * library
 *
 * This example demonstrates a more complex dashboard with multiple component
 * types and more advanced features like machine state monitoring.
 *
 * Hardware:
 * - ESP32 board
 * - LED connected to pin 2 (optional)
 * - Potentiometer connected to pin 34 (optional)
 *
 * Created by Ben (Everwood), 2024
 * Released under MIT License
 */

#include <Arduino.h>
#include <WebDashboard.h>
#include <WiFi.h>

// WiFi credentials
const char* ssid = "YourWiFiSSID";          // Replace with your WiFi SSID
const char* password = "YourWiFiPassword";  // Replace with your WiFi password

// Pin definitions
const int ledPin = 2;
const int potPin = 34;

// Dashboard instance
WebDashboard dashboard;

// State machine states
enum SystemState { STATE_IDLE, STATE_RUNNING, STATE_ERROR, STATE_MAINTENANCE };

SystemState currentState = STATE_IDLE;
unsigned long stateStartTime = 0;
unsigned long lastUpdateTime = 0;

// Select options for dropdown
const char* modeOptions[] = {"Normal", "Power Save", "Performance", "Custom"};
const int modeOptionsCount = 4;
String currentMode = "Normal";

// Button callback function
void handleButton(const char* id) {
  dashboard.logf(LOG_INFO, "Button %s pressed", id);

  if (strcmp(id, "btn_start") == 0) {
    if (currentState == STATE_IDLE) {
      setSystemState(STATE_RUNNING);
    }
  } else if (strcmp(id, "btn_stop") == 0) {
    if (currentState == STATE_RUNNING) {
      setSystemState(STATE_IDLE);
    }
  } else if (strcmp(id, "btn_reset") == 0) {
    if (currentState == STATE_ERROR) {
      setSystemState(STATE_IDLE);
    }
  } else if (strcmp(id, "btn_error") == 0) {
    // Simulate error for testing
    setSystemState(STATE_ERROR);
  }
}

// Toggle callback function
void handleToggle(const char* id, bool state) {
  dashboard.logf(LOG_INFO, "Toggle %s set to %s", id, state ? "ON" : "OFF");

  if (strcmp(id, "toggle_led") == 0) {
    digitalWrite(ledPin, state ? HIGH : LOW);
  } else if (strcmp(id, "toggle_maintenance") == 0) {
    if (state) {
      setSystemState(STATE_MAINTENANCE);
    } else {
      setSystemState(STATE_IDLE);
    }
  }
}

// Slider callback function
void handleSlider(const char* id, int value) {
  dashboard.logf(LOG_INFO, "Slider %s changed to %d", id, value);

  if (strcmp(id, "slider_brightness") == 0) {
    ledcWrite(0, value);
  }
}

// Text input callback function
void handleTextInput(const char* id, const char* value) {
  dashboard.logf(LOG_INFO, "Text input %s changed to: %s", id, value);

  if (strcmp(id, "text_devicename") == 0) {
    dashboard.updateValue("label_devicename", value);
  }
}

// Select callback function
void handleSelect(const char* id, const char* value) {
  dashboard.logf(LOG_INFO, "Select %s changed to: %s", id, value);

  if (strcmp(id, "select_mode") == 0) {
    currentMode = value;

    if (strcmp(value, "Power Save") == 0) {
      dashboard.updateValue("slider_brightness", 50);
      ledcWrite(0, 50);
    } else if (strcmp(value, "Performance") == 0) {
      dashboard.updateValue("slider_brightness", 255);
      ledcWrite(0, 255);
    }
  }
}

// Set system state and update dashboard
void setSystemState(SystemState newState) {
  currentState = newState;
  stateStartTime = millis();

  // Update state label on dashboard
  switch (newState) {
    case STATE_IDLE:
      dashboard.setMachineState("IDLE");
      dashboard.log("System is now IDLE", LOG_INFO);
      break;
    case STATE_RUNNING:
      dashboard.setMachineState("RUNNING");
      dashboard.log("System is now RUNNING", LOG_INFO);
      break;
    case STATE_ERROR:
      dashboard.setMachineState("ERROR");
      dashboard.log("System ERROR detected!", LOG_ERROR);
      break;
    case STATE_MAINTENANCE:
      dashboard.setMachineState("MAINTENANCE");
      dashboard.log("System in MAINTENANCE mode", LOG_WARNING);
      break;
  }
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("ESP32 Advanced Web Dashboard Demo");

  // Initialize pins
  pinMode(ledPin, OUTPUT);
  pinMode(potPin, INPUT);

  // Setup PWM for LED brightness control
  ledcSetup(0, 5000, 8);  // Channel 0, 5kHz, 8-bit resolution
  ledcAttachPin(ledPin, 0);

  // Connect to WiFi
  Serial.printf("Connecting to WiFi: %s\n", ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());

  // Initialize the dashboard
  dashboard.begin("ESP32 Dashboard", "Advanced Example");
  Serial.printf("Dashboard available at: http://%s\n",
                dashboard.getIPAddress().c_str());

  // Enable debug logging
  dashboard.enableDebugLogging(true);

  // Add components to the dashboard

  // Control section
  dashboard.addButton("btn_start", "Start System", handleButton);
  dashboard.addButton("btn_stop", "Stop System", handleButton);
  dashboard.addButton("btn_reset", "Reset Error", handleButton);
  dashboard.addButton("btn_error", "Simulate Error", handleButton);

  // LED Control
  dashboard.addToggle("toggle_led", "LED Power", false, handleToggle);
  dashboard.addSlider("slider_brightness", "LED Brightness", 0, 255, 128, 1,
                      handleSlider);

  // System Status
  dashboard.addToggle("toggle_maintenance", "Maintenance Mode", false,
                      handleToggle);
  dashboard.addSelect("select_mode", "Operation Mode", modeOptions,
                      modeOptionsCount, "Normal", handleSelect);

  // Settings
  dashboard.addTextInput("text_devicename", "Device Name", "ESP32 Device",
                         handleTextInput);

  // Monitoring
  dashboard.addPinMonitor("pin_pot", "Potentiometer", potPin, INPUT, true);

  // Add label for dynamic display
  dashboard.addTextInput("label_devicename", "Current Name", "ESP32 Device");

  // Set initial state
  setSystemState(STATE_IDLE);

  // Log initialization
  dashboard.log("Advanced dashboard initialized");
}

void loop() {
  // Update the dashboard
  dashboard.update();

  // Perform regular updates
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime > 1000) {  // Update every second
    lastUpdateTime = currentTime;

    // Update uptime display
    unsigned long uptime = (currentTime - stateStartTime) / 1000;  // in seconds
    char uptimeStr[32];
    sprintf(uptimeStr, "%02lu:%02lu:%02lu", (uptime / 3600),
            (uptime % 3600) / 60, (uptime % 60));
    dashboard.updateValue("label_devicename", uptimeStr);

    // Simulate some state transitions
    if (currentState == STATE_RUNNING) {
      // Randomly transition to error state (for demo purposes)
      if (random(1000) < 5) {  // 0.5% chance per second
        setSystemState(STATE_ERROR);
      }
    }
  }

  // Small delay to prevent overwhelming the CPU
  delay(10);
}