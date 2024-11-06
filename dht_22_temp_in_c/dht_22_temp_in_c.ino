// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"
#include <math.h>


#define DHTPIN1 2     // Digital pin connected to the first DHT sensor
#define DHTPIN2 3     // Digital pin connected to the second DHT sensor
#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321

//notes from liberary owner::

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 3 (on the right) of the sensor to GROUND (if your sensor has 3 pins)
// Connect pin 4 (on the right) of the sensor to GROUND and leave the pin 3 EMPTY (if your sensor has 4 pins)
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht1(DHTPIN1, DHTTYPE);  // First DHT sensor
DHT dht2(DHTPIN2, DHTTYPE);  // Second DHT sensor

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test with two sensors!"));

  dht1.begin();
  dht2.begin();
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  delay(100);
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  
  float t1 = dht1.readTemperature(); // Read temperature from the first sensor
  float t2 = dht2.readTemperature(); // Read temperature from the second sensor

// // Check if any reads failed for the first sensor
//   if (isnan(t1)) {
//     Serial.print("Temperature1:Error  ");
//   } else {
//     Serial.print("Temperature1:");
//     Serial.print(t1);
//   }

//   Serial.print("  ");

//   if (isnan(t2)) {
//     Serial.println("Temperature2:Error");
//   } else {
//     Serial.print("Temperature2:");
//     Serial.println(t2); // Use println at the end to move to the next line
//   }

    // Check if readings are valid, else handle error
  if (isnan(t1) || isnan(t2)) {
    Serial.println(F("Failed to read from one or both DHT sensors!"));
    return;
  }

  // Calculate average temperature
  float averageTemp = (t1 + t2) / 2.0;

  // Calculate RMS temperature
  float rmsTemp = sqrt((t1 * t1 + t2 * t2) / 2.0);

  // Display results
  Serial.print("Temperature1:");
  Serial.print(t1);
  Serial.print("  Temperature2:");
  Serial.print(t2);
  Serial.print("  Average_Temperature:");
  Serial.print(averageTemp);
  Serial.print("  RMS_Temperature:");
  Serial.println(rmsTemp);

}
