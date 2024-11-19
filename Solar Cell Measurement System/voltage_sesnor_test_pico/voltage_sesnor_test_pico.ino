// This code is for Raspberry pi pico

// Voltage measurement using a voltage sensor connected to pin A3

//factor will be 3.3 for raspbery pi pico

// Pin Definitions
const int voltageSensorPin = A1;  // Voltage sensor input pin (green wire)

// Constants
const float factor = 3.3;        // Reduction factor of the Voltage Sensor shield
const float v_cc = 4.98;           // Arduino input voltage (measurable by voltmeter)

// Variables
float v_in = 0.0;                  // Measured input voltage
float v_out = 0.0;                 // Voltage output from the sensor
int sesnor_value = 0;         // Raw ADC value (0 - 1023)

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Read the raw ADC value from the voltage sensor
  sesnor_value = analogRead(voltageSensorPin);

  // Convert the raw value to the actual voltage on the analog pin
  v_out = (sesnor_value / 1024.0 ) * v_cc;

  // Calculate the input voltage using the sensor factor
  v_in = v_out * factor;

  // Print the measured voltage
  Serial.print("Input Voltage: ");
  Serial.print(v_in);
  Serial.println(" V");

  // Wait for a second before the next measurement
  delay(1000);
}
