#include "DHT.h"
#include <math.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin definitions for relay and sensors
#define RELAY_PIN_HEATER 4   // Relay pin for heater
#define BUTTON_PIN 7         // Button pin for relay toggling
#define RELAY_PIN_MEASURE 8  // Relay pin for measurement (current/voltage)
#define CURRENT_SENSOR_PIN A0 // Current sensor pin
#define VOLTAGE_SENSOR_PIN A1 // Voltage sensor pin

// Button pins for temperature adjustment
#define BUTTON_INCREASE_PIN 9  // Button to increase temperature
#define BUTTON_DECREASE_PIN 10 // Button to decrease temperature

// DHT Sensor settings
#define DHTPIN1 2            // DHT sensor 1 pin
#define DHTPIN2 3            // DHT sensor 2 pin
#define DHTTYPE DHT22        // Sensor type
DHT dht1(DHTPIN1, DHTTYPE);  // DHT sensor 1
DHT dht2(DHTPIN2, DHTTYPE);  // DHT sensor 2

// LCD settings
LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the LCD address to 0x27 for a 20x4 display

// Constants
float desiredTemp = 25.0;           // Target temperature (modifiable)
const unsigned long rmsInterval = 500; // RMS temperature calculation interval
const float voltageFactor = 3.3;       // Voltage factor for Raspberry Pi Pico
const float currentOffset = 2.5;       // Current sensor offset voltage (V)
const float currentSensitivity = 0.185; // Current sensor sensitivity (V/A)
const float v_cc = 4.98;               // Measured input voltage to the Pico (V)
const float voltageSensorFactor = 3.3; // Reduction factor of the voltage sensor

// Timing variables
unsigned long lastDebounceTime = 0;
unsigned long lastMeasurementTime = 0;
unsigned long lastRmsTime = 0;
unsigned long lastToggleMillis = 0;
const unsigned long debounceDelay = 50;
const unsigned long measurementInterval = 500;
const int onTime = 6000;
const int offTime = 4000;

// Variables for states and values
bool relayStateMeasure = false;  // State of relay for measurement
bool relayStateHeat = false;     // State of relay for heating
bool lastButtonState = LOW;      // Previous button state
bool currentButtonState = LOW;   // Current button state
float error = 0.0;               // Temperature error

// Button states
bool lastIncreaseButtonState = HIGH;
bool lastDecreaseButtonState = HIGH;
unsigned long lastIncreaseButtonTime = 0;
unsigned long lastDecreaseButtonTime = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_INCREASE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DECREASE_PIN, INPUT_PULLUP);
  pinMode(RELAY_PIN_MEASURE, OUTPUT);
  pinMode(RELAY_PIN_HEATER, OUTPUT);

  digitalWrite(RELAY_PIN_MEASURE, LOW);
  digitalWrite(RELAY_PIN_HEATER, LOW);

  Serial.begin(9600);
  dht1.begin();
  dht2.begin();

  // Initialize the LCD
  lcd.init();
  lcd.backlight();

  // Display initial desired temperature on LCD
  lcd.setCursor(0, 3);
  lcd.print("SetTemp: ");
  lcd.print(desiredTemp);
  lcd.print("    "); // Clear any leftover characters
}

void loop() {
  unsigned long currentMillis = millis();

  // Handle button press for relay toggling
  handleRelayToggling(currentMillis);

  // Handle button press for temperature adjustment
  handleTemperatureAdjustment(currentMillis);

  // Measure current or voltage depending on relay state
  handleMeasurement(currentMillis);

  // Calculate RMS temperature and control heating
  handleTemperatureControl(currentMillis);
}

void handleRelayToggling(unsigned long currentMillis) {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = currentMillis;
  }

  if ((currentMillis - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && currentButtonState == HIGH) {
      relayStateMeasure = !relayStateMeasure;
      digitalWrite(RELAY_PIN_MEASURE, relayStateMeasure ? HIGH : LOW);
    }
    currentButtonState = reading;
  }

  lastButtonState = reading;
}

void handleTemperatureAdjustment(unsigned long currentMillis) {
  int increaseButtonState = digitalRead(BUTTON_INCREASE_PIN);
  int decreaseButtonState = digitalRead(BUTTON_DECREASE_PIN);

  // Handle increase button
  if (increaseButtonState == LOW && lastIncreaseButtonState == HIGH && (currentMillis - lastIncreaseButtonTime) > debounceDelay) {
    desiredTemp += 5.0;
    lastIncreaseButtonTime = currentMillis;
    updateDesiredTemperatureDisplay();
  }
  lastIncreaseButtonState = increaseButtonState;

  // Handle decrease button
  if (decreaseButtonState == LOW && lastDecreaseButtonState == HIGH && (currentMillis - lastDecreaseButtonTime) > debounceDelay) {
    desiredTemp -= 5.0;
    lastDecreaseButtonTime = currentMillis;
    updateDesiredTemperatureDisplay();
  }
  lastDecreaseButtonState = decreaseButtonState;
}

void updateDesiredTemperatureDisplay() {
  lcd.setCursor(0, 3);
  lcd.print("SetTemp: ");
  lcd.print(desiredTemp);
  lcd.print("    "); // Clear any leftover characters
}

void handleMeasurement(unsigned long currentMillis) {
  if (currentMillis - lastMeasurementTime >= measurementInterval) {
    lastMeasurementTime = currentMillis;

    if (relayStateMeasure) {
      int adcCurrent = analogRead(CURRENT_SENSOR_PIN);
      float voltageCurrent = adcCurrent * voltageFactor / 1023.0;
      float current = (voltageCurrent - currentOffset) / currentSensitivity;
      if (current < 0.16) {
        current = 0;
      }
      Serial.print("Current (A) ");
      Serial.print(current);
      Serial.print(" ");

      // Display current on LCD
      lcd.setCursor(0, 1);
      lcd.print("Current (A): ");
      lcd.print(current);
      lcd.print("    "); // Clear any leftover characters
    } else {
      int adcVoltage = analogRead(VOLTAGE_SENSOR_PIN);
      float v_out = (adcVoltage / 1024.0) * v_cc;
      float voltageInput = v_out * voltageSensorFactor;
      Serial.print("Voltage (V) ");
      Serial.print(voltageInput);
      Serial.print(" ");

      // Display voltage on LCD
      lcd.setCursor(0, 1);
      lcd.print("Voltage (V): ");
      lcd.print(voltageInput);
      lcd.print("    "); // Clear any leftover characters
    }
  }
}

void handleTemperatureControl(unsigned long currentMillis) {
  if (currentMillis - lastRmsTime >= rmsInterval) {
    lastRmsTime = currentMillis;

    float t1 = dht1.readTemperature();
    float t2 = dht2.readTemperature();

    if (isnan(t1) || isnan(t2)) {
      Serial.println(F("Failed to read from one or both DHT sensors!"));
      digitalWrite(RELAY_PIN_HEATER, LOW);
      return;
    }

    float rmsTemp = sqrt((t1 * t1 + t2 * t2) / 2.0);

    Serial.print("RMS Temperature (°C) ");
    Serial.println(rmsTemp);

    // Display RMS temperature on LCD
    lcd.setCursor(0, 0);
    lcd.print("Temp (C): ");
    lcd.print(rmsTemp);
    lcd.print("    "); // Clear any leftover characters

    error = desiredTemp - rmsTemp;
  }

  if (error > 0) {
    heatAlgorithm(onTime, offTime, currentMillis);
  } else {
    digitalWrite(RELAY_PIN_HEATER, LOW);
    relayStateHeat = false;
  }
}

void heatAlgorithm(int onTime, int offTime, unsigned long currentMillis) {
  if (relayStateHeat && (currentMillis - lastToggleMillis >= onTime)) {
    relayStateHeat = false;
    digitalWrite(RELAY_PIN_HEATER, LOW);
    lastToggleMillis = currentMillis;
  } else if (!relayStateHeat && (currentMillis - lastToggleMillis >= offTime)) {
    relayStateHeat = true;
    digitalWrite(RELAY_PIN_HEATER, HIGH);
    lastToggleMillis = currentMillis;
  }
}