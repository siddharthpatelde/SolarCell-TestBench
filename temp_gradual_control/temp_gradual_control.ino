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

//to saperate rms calcution from main loop time
unsigned long previousMillis1 = 0.0;
unsigned long previousMillis2 = 0.0;

const int rms_interval = 100;

float error = 0.0; // Move error declaration here

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test with two sensors!"));

  dht1.begin();
  dht2.begin();
  pinMode(RELAY_PIN, OUTPUT); // Set relay pin as output
}

void loop() {

  unsigned long currentMillies = millis();

  if(currentMillies - previousMillis1 >= rms_interval){
    previousMillis1 = currentMillies;

      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      float t1 = dht1.readTemperature(); // Read temperature from the first sensor
      float t2 = dht2.readTemperature(); // Read temperature from the second sensor

      // Check if readings are valid, else handle error
      if (isnan(t1) || isnan(t2)) {
        Serial.println(F("Failed to read from one or both DHT sensors!"));
        return;
      }

      // Calculate average temperature
      float averageTemp = (t1 + t2) / 2.0;

      // Calculate RMS temperature
      float rmsTemp = sqrt((t1 * t1 + t2 * t2) / 2.0);

      Serial.print("RMS_Temperature:");
      Serial.println(rmsTemp);

      // Proportional control algorithm
      error = desiredTemp - rmsTemp;   
  }

  if( (currentMillies - previousMillis2) >= 100 ){
    currentMillies - previousMillis2;
    if (error > 0) {
      heat_algorithm(6000,4000);
    } else {
      digitalWrite(RELAY_PIN, LOW);   // Turn off heating element
    }
  }
}

// Adjusts for a gradual temperature increase to accommodate the DHT22 sensor's slow response time.
void heat_algorithm(int on_time, int off_time){ 
  if((currentMillies - previousMillis2) >= on_time ){
    digitalWrite(RELAY_PIN, HIGH);  // Turn on heating element
  }

  digitalWrite(RELAY_PIN, LOW);  // Turn on heating element
  delay(off_time);
}
