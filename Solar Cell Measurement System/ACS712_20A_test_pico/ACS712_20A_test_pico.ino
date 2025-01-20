//This code is for raspbery pi Pico

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  
  int adc = analogRead(A0);
  float voltage = adc * 3.3/ 1023;
  float current = (voltage - 2.5) / 0.185; //for 20A 

  Serial.print("Current : ");
  Serial.println(current);
  delay(300);
}