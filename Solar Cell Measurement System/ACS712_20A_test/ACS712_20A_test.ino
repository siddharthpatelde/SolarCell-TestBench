/* 
 CREATED BY AATIK"s LAB
 FOR TESTING PURPOSE

SENSOR CONNECTIONS

GND_PIN -> GND OF ARDUINO
VCC     -> 5v PIN OF ARDUINO
OUT     -> A0 PIN OF ARDUNO
 
 This code is for the 5A sensor, if 20A or 30A sensor.
 You need to MODIFY/REPLACE (.0264 * analogRead(A0) -13.51) this formula to 
 
(.19 * analogRead(A0) -25) for 20A sensor
(.044 * analogRead(A0) -3.78) for 30A sensor
*/

void setup() {
  Serial.begin(9600);
}

void loop() {
 
  float average = 0;
  for(int i = 0; i < 1000; i++) 
  {
    average = average + (.19 * analogRead(A0) -13.51) / 1000;
    delay(1);
  }
  Serial.println(average);  
}




















