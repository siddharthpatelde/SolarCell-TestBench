// Pin definitions
const int buttonPin = 7;          // Pin connected to the pushbutton
const int relayPin = 4;           // Pin connected to the relay module
const int currentSensorPin = A0;  // Current sensor input pin
const int voltageSensorPin = A1;  // Voltage sensor input pin

// Variables for button and relay state
bool relayState = false;           // Tracks the state of the relay (ON/OFF)
bool lastButtonState = LOW;        // Previous state of the button
bool currentButtonState = LOW;     // Current state of the button

// Constants for debounce and sensor calculations
const unsigned long debounceDelay = 50; // Debounce delay in milliseconds
const float voltageFactor = 3.3;        // Factor for Raspberry Pi Pico
const float currentOffset = 2.5;       // Current sensor offset voltage (V)
const float currentSensitivity = 0.100; // Current sensor sensitivity (V/A)
const float v_cc = 4.98;               // Measured input voltage to the Pico (V)
const float voltageSensorFactor = 3.3; // Reduction factor of the voltage sensor

// Timing variables
unsigned long lastDebounceTime = 0;    // Debounce timer
unsigned long lastMeasurementTime = 0; // Timer for measurements
const unsigned long measurementInterval = 500; // Measurement interval in ms

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Set the button pin as input with internal pull-up
  pinMode(relayPin, OUTPUT);        // Set the relay pin as output
  digitalWrite(relayPin, LOW);      // Ensure the relay is OFF initially
  Serial.begin(9600);               // Initialize serial communication
}

void loop() {
  // Read the state of the button
  int reading = digitalRead(buttonPin);

  // Check if the button state has changed (pressed or released)
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // Update the debounce timer
  }

  // If enough time has passed since the last toggle
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If the button state has changed and is now pressed
    if (reading == LOW && currentButtonState == HIGH) {
      relayState = !relayState; // Toggle the relay state
      digitalWrite(relayPin, relayState ? HIGH : LOW); // Update the relay
      Serial.println(relayState ? "Relay ON" : "Relay OFF"); // Debug message
    }
    currentButtonState = reading; // Update the current button state
  }

  // Save the button state for the next loop
  lastButtonState = reading;

  // Non-blocking measurement logic
  if (millis() - lastMeasurementTime >= measurementInterval) {
    lastMeasurementTime = millis(); // Update the measurement timer

    if (relayState) {
      // Relay is ON: Measure current
      int adcCurrent = analogRead(currentSensorPin);
      float voltageCurrent = adcCurrent * voltageFactor / 1023.0; // Voltage from ADC
      float current = (voltageCurrent - currentOffset) / currentSensitivity; // Current in Amps
      Serial.print("Current_A: ");
      Serial.println(current);
    } else {
      // Relay is OFF: Measure voltage
      int adcVoltage = analogRead(voltageSensorPin);
      float v_out = (adcVoltage / 1024.0) * v_cc; // Voltage output from the sensor
      float voltageInput = v_out * voltageSensorFactor; // Input voltage in Volts
      Serial.print("Voltage_V: ");
      Serial.println(voltageInput);
    }
  }
}
