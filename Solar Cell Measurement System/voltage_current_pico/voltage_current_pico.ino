// Combined code for Raspberry Pi Pico to display current and voltage

// Pin Definitions
const int currentSensorPin = A0;  // Current sensor input pin
const int voltageSensorPin = A1; // Voltage sensor input pin

// Constants
const float voltageFactor = 3.3;    // Factor for Raspberry Pi Pico
const float currentOffset = 2.5;   // Current sensor offset voltage (V)
const float currentSensitivity = 0.100; // Current sensor sensitivity (V/A)
const float v_cc = 4.98;           // Measured input voltage to the Pico (V)
const float voltageSensorFactor = 3.3;  // Reduction factor of the voltage sensor

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Read raw ADC values
  int adcCurrent = analogRead(currentSensorPin);
  int adcVoltage = analogRead(voltageSensorPin);

  // Calculate current
  float voltageCurrent = adcCurrent * voltageFactor / 1023.0; // Voltage from ADC
  float current = (voltageCurrent - currentOffset) / currentSensitivity; // Current in Amps

  // Calculate voltage
  float v_out = (adcVoltage / 1024.0) * v_cc; // Voltage output from the sensor
  float voltageInput = v_out * voltageSensorFactor; // Input voltage in Volts


  // Format for Serial Plotter (labels and units as separate values for plotting)
  Serial.print("Current_A:");
  Serial.print(current);
  Serial.print("\tVoltage_V:");
  Serial.println(voltageInput);

  // Wait for a short delay
  delay(300);
}
