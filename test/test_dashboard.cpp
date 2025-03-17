#include <Arduino.h>
#include <WebDashboard.h>
#include <unity.h>

WebDashboard dashboard;
bool buttonPressed = false;
bool toggleState = false;
int sliderValue = 0;

// Test callback functions
void testButtonCallback(const char* id) {
  buttonPressed = true;
  TEST_ASSERT_EQUAL_STRING("test_button", id);
}

void testToggleCallback(const char* id, bool state) {
  toggleState = state;
  TEST_ASSERT_EQUAL_STRING("test_toggle", id);
}

void testSliderCallback(const char* id, int value) {
  sliderValue = value;
  TEST_ASSERT_EQUAL_STRING("test_slider", id);
}

void test_dashboard_initialization() {
  bool result = dashboard.begin("Test Dashboard", "Test Device");
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_TRUE(dashboard.isOnline());
}

void test_component_creation() {
  // Test adding various components
  bool buttonResult =
      dashboard.addButton("test_button", "Test Button", testButtonCallback);
  TEST_ASSERT_TRUE(buttonResult);

  bool toggleResult = dashboard.addToggle("test_toggle", "Test Toggle", false,
                                          testToggleCallback);
  TEST_ASSERT_TRUE(toggleResult);

  bool sliderResult = dashboard.addSlider("test_slider", "Test Slider", 0, 100,
                                          50, 1, testSliderCallback);
  TEST_ASSERT_TRUE(sliderResult);
}

void test_update_values() {
  // Test updating component values
  bool textResult = dashboard.updateValue("test_button", "Updated");
  TEST_ASSERT_TRUE(textResult);

  bool intResult = dashboard.updateValue("test_slider", 75);
  TEST_ASSERT_TRUE(intResult);

  bool floatResult = dashboard.updateValue("test_slider", 75.5, 1);
  TEST_ASSERT_TRUE(floatResult);

  bool boolResult = dashboard.updateValue("test_toggle", true);
  TEST_ASSERT_TRUE(boolResult);
}

void test_logging() {
  bool infoLogResult = dashboard.log("Info log message", LOG_INFO);
  TEST_ASSERT_TRUE(infoLogResult);

  bool warningLogResult = dashboard.log("Warning log message", LOG_WARNING);
  TEST_ASSERT_TRUE(warningLogResult);

  bool errorLogResult = dashboard.log("Error log message", LOG_ERROR);
  TEST_ASSERT_TRUE(errorLogResult);

  bool formatLogResult = dashboard.logf(LOG_INFO, "Formatted log %d", 123);
  TEST_ASSERT_TRUE(formatLogResult);
}

void test_machine_state() {
  dashboard.setMachineState("RUNNING");
  // This test would need a way to verify the state was set correctly
  // which would require more detailed internal access
}

void setup() {
  // Wait 2 seconds before starting tests
  delay(2000);

  // Begin unit tests
  UNITY_BEGIN();

  // Run tests
  RUN_TEST(test_dashboard_initialization);
  RUN_TEST(test_component_creation);
  RUN_TEST(test_update_values);
  RUN_TEST(test_logging);
  RUN_TEST(test_machine_state);

  // End unit tests
  UNITY_END();
}

void loop() {
  // Nothing to do here
}