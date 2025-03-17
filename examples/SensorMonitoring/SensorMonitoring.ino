/**
 * SensorMonitoring.ino - Sensor monitoring example for ESP32-WebCommunication
 * library
 *
 * This example demonstrates how to create a dashboard for monitoring sensor
 * data with real-time updates and visual feedback.
 *
 * Hardware:
 * - ESP32 board
 * - Analog sensor connected to pin 34 (potentiometer, LDR, etc.)
 * - Digital sensor connected to pin 13 (button, switch, PIR, etc.)
 * - Optional: DHT22/DHT11 temperature/humidity sensor on pin 15
 *
 * Created by Ben (Everwood), 2024
 * Released under MIT License
 */

#include <Arduino.h>
#include <WebDashboard.h>
#include <WiFi.h>

// Uncomment to use DHT sensor
// #include <DHT.h>

// WiFi credentials
const char* ssid = "YourWiFiSSID";          // Replace with your WiFi SSID
const char* password = "YourWiFiPassword";  // Replace with your WiFi password

// Pin definitions
const int analogSensorPin = 34;   // Analog sensor (potentiometer, LDR, etc.)
const int digitalSensorPin = 13;  // Digital sensor (button, switch, PIR, etc.)
const int dhtPin = 15;            // DHT temperature/humidity sensor

// Sensor thresholds
const int analogLowThreshold = 1000;
const int analogHighThreshold = 3000;

// Dashboard instance
WebDashboard dashboard;

// Uncomment to use DHT sensor
// DHT dht(dhtPin, DHT22);  // Change to DHT11 if using that sensor

// Variables for sensor values
int analogValue = 0;
bool digitalValue = false;
float temperature = 0.0;
float humidity = 0.0;
unsigned long lastSensorUpdate = 0;
unsigned long lastDhtUpdate = 0;

// Callback function for the notification toggle
void handleNotifications(const char* id, bool state) {
  dashboard.logf(LOG_INFO, "Notifications %s", state ? "enabled" : "disabled");
}

// Callback function for threshold slider
void handleThresholdChange(const char* id, int value) {
  if (strcmp(id, "slider_low_threshold") == 0) {
    dashboard.logf(LOG_INFO, "Low threshold set to %d", value);
  } else if (strcmp(id, "slider_high_threshold") == 0) {
    dashboard.logf(LOG_INFO, "High threshold set to %d", value);
  }
}

// Update sensor values
void updateSensors() {
  // Read analog sensor
  analogValue = analogRead(analogSensorPin);

  // Read digital sensor
  digitalValue = digitalRead(digitalSensorPin);

  // Update dashboard with sensor values
  dashboard.updateValue("sensor_analog", analogValue);
  dashboard.updateValue("sensor_digital", digitalValue ? "ON" : "OFF");

  // Check thresholds for analog sensor
  if (analogValue < analogLowThreshold) {
    dashboard.log("Analog sensor value below minimum threshold!", LOG_WARNING);
  } else if (analogValue > analogHighThreshold) {
    dashboard.log("Analog sensor value above maximum threshold!", LOG_WARNING);
  }
}

// Update DHT sensor values
void updateDhtSensor() {
  // Uncomment to use DHT sensor
  /*
  // Read temperature and humidity
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  // Check if any reads failed
  if (isnan(humidity) || isnan(temperature)) {
    dashboard.log("Failed to read from DHT sensor!", LOG_ERROR);
    return;
  }

  // Update dashboard with temperature and humidity values
  dashboard.updateValue("sensor_temp", temperature, 1);
  dashboard.updateValue("sensor_humidity", humidity, 1);

  // Check temperature thresholds
  if (temperature > 30.0) {
    dashboard.log("High temperature detected!", LOG_WARNING);
  }
  */

  // Simulate DHT readings if no actual sensor
  temperature = 22.5 + (random(-20, 20) / 10.0);
  humidity = 45.0 + (random(-50, 50) / 10.0);

  // Update dashboard with temperature and humidity values
  dashboard.updateValue("sensor_temp", temperature, 1);
  dashboard.updateValue("sensor_humidity", humidity, 1);
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("ESP32 Sensor Monitoring Demo");

  // Initialize pins
  pinMode(analogSensorPin, INPUT);
  pinMode(digitalSensorPin, INPUT_PULLUP);
  pinMode(dhtPin, INPUT);

  // Uncomment to use DHT sensor
  // dht.begin();

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
  dashboard.begin("ESP32 Sensor Dashboard", "Sensor Monitoring");
  Serial.printf("Dashboard available at: http://%s\n",
                dashboard.getIPAddress().c_str());

  // Add pin monitors for direct sensor readings
  dashboard.addPinMonitor("pin_analog", "Analog Sensor Pin", analogSensorPin,
                          INPUT, true);
  dashboard.addPinMonitor("pin_digital", "Digital Sensor Pin", digitalSensorPin,
                          INPUT_PULLUP, false);

  // Add components for displaying sensor values
  dashboard.addTextInput("sensor_analog", "Analog Value", "0");
  dashboard.addTextInput("sensor_digital", "Digital State", "OFF");
  dashboard.addTextInput("sensor_temp", "Temperature (°C)", "0.0");
  dashboard.addTextInput("sensor_humidity", "Humidity (%)", "0.0");

  // Add threshold controls
  dashboard.addSlider("slider_low_threshold", "Low Threshold", 0, 4095,
                      analogLowThreshold, 100, handleThresholdChange);
  dashboard.addSlider("slider_high_threshold", "High Threshold", 0, 4095,
                      analogHighThreshold, 100, handleThresholdChange);

  // Add notification toggle
  dashboard.addToggle("toggle_notifications", "Enable Notifications", true,
                      handleNotifications);

  // Log initialization
  dashboard.log("Sensor monitoring dashboard initialized", LOG_INFO);

  // Initial sensor readings
  updateSensors();
  updateDhtSensor();
}

void loop() {
  // Update the dashboard
  dashboard.update();

  // Update sensor values every 500ms
  unsigned long currentTime = millis();
  if (currentTime - lastSensorUpdate > 500) {
    lastSensorUpdate = currentTime;
    updateSensors();
  }

  // Update DHT sensor every 2 seconds (DHT sensors are slow)
  if (currentTime - lastDhtUpdate > 2000) {
    lastDhtUpdate = currentTime;
    updateDhtSensor();
  }

  // Small delay to prevent overwhelming the CPU
  delay(10);
}