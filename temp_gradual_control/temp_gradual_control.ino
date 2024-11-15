// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"
#include <math.h>

#define DHTPIN1 2     // Digital pin connected to the first DHT sensor
#define DHTPIN2 3     // Digital pin connected to the second DHT sensor
#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
#define RELAY_PIN 4   // Digital pin connected to the relay

DHT dht1(DHTPIN1, DHTTYPE);  // First DHT sensor
DHT dht2(DHTPIN2, DHTTYPE);  // Second DHT sensor

const float desiredTemp = 35;  // Desired temperature in Celsius
unsigned long previousMillis1 = 0;
const int rms_interval = 100;

float error = 0.0; // Initialize with a default value
bool relayState = false; // Track the relay state

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test with two sensors!"));

  dht1.begin();
  dht2.begin();
  pinMode(RELAY_PIN, OUTPUT); // Set relay pin as output
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Update RMS temperature and error
  if (currentMillis - previousMillis1 >= rms_interval) {
    previousMillis1 = currentMillis;

    float t1 = dht1.readTemperature();
    float t2 = dht2.readTemperature();

    if (isnan(t1) || isnan(t2)) {
      Serial.println(F("Failed to read from one or both DHT sensors!"));
      digitalWrite(RELAY_PIN, LOW); // Ensure heating is off
      return;
    }

    float averageTemp = (t1 + t2) / 2.0;
    float rmsTemp = sqrt((t1 * t1 + t2 * t2) / 2.0);

    Serial.print("RMS_Temperature: ");
    Serial.println(rmsTemp);

    error = desiredTemp - rmsTemp;   
  }

  // Control heating based on error
  if (error > 0) {
    heat_algorithm(6000, 4000, currentMillis);
  } else {
    digitalWrite(RELAY_PIN, LOW);   // Turn off heating element
  }

}

// Adjusts for a gradual temperature increase to accommodate the DHT22 sensor's slow response time.
void heat_algorithm(int on_time, int off_time, unsigned long currentMillis) {
  static unsigned long lastToggleMillis = 0;

  if (relayState && (currentMillis - lastToggleMillis >= on_time)) {
    relayState = false;
    digitalWrite(RELAY_PIN, LOW);
    lastToggleMillis = currentMillis;
  } else if (!relayState && (currentMillis - lastToggleMillis >= off_time)) {
    relayState = true;
    digitalWrite(RELAY_PIN, HIGH);
    lastToggleMillis = currentMillis;
  }
}
