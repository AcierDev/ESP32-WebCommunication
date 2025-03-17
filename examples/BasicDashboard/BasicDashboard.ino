/**
 * BasicDashboard.ino - Simple dashboard example for ESP32-WebCommunication
 * library
 *
 * This example demonstrates the basic setup of a web dashboard with
 * a few simple controls: button, toggle, and slider.
 *
 * Hardware:
 * - ESP32 board
 * - LED connected to pin 2 (optional)
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

// LED pin
const int ledPin = 2;

// Dashboard instance
WebDashboard dashboard;

// Button callback function
void handleButton(const char* id) {
  Serial.printf("Button %s pressed\n", id);
  dashboard.log("Button pressed!");

  // Flash LED
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
}

// Toggle callback function
void handleToggle(const char* id, bool state) {
  Serial.printf("Toggle %s changed to %s\n", id, state ? "ON" : "OFF");
  dashboard.logf(LOG_INFO, "Toggle %s set to %s", id, state ? "ON" : "OFF");

  // Control LED
  digitalWrite(ledPin, state ? HIGH : LOW);
}

// Slider callback function
void handleSlider(const char* id, int value) {
  Serial.printf("Slider %s changed to %d\n", id, value);
  dashboard.logf(LOG_INFO, "Brightness set to %d", value);

  // Set LED brightness using PWM
  ledcWrite(0, value);
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("ESP32 Web Dashboard Demo");

  // Initialize LED pin
  pinMode(ledPin, OUTPUT);

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
  dashboard.begin("ESP32 Dashboard", "Basic Example");
  Serial.printf("Dashboard available at: http://%s\n",
                dashboard.getIPAddress().c_str());

  // Add components to the dashboard
  dashboard.addButton("btn1", "Press Me", handleButton);
  dashboard.addToggle("toggle1", "LED Control", false, handleToggle);
  dashboard.addSlider("slider1", "Brightness", 0, 255, 128, 1, handleSlider);

  // Log initialization
  dashboard.log("Dashboard initialized");
}

void loop() {
  // Update the dashboard
  dashboard.update();

  // Small delay to prevent overwhelming the CPU
  delay(10);
}