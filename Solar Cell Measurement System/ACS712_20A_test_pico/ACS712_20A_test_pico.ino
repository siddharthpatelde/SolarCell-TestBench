//filterd adc value from current sensor

#include <Filters.h>
FilterOnePole lowpassFilter(LOWPASS, 0.5); // Smoothing filter
float adc_filtered = 0;


void setup() {
  Serial.begin(9600);
}

void loop() {
  float adc_non_filtered;
  adc_non_filtered = analogRead(A0);

  // Print both non-filtered and filtered values
  adc_filtered = lowpassFilter.input(adc_non_filtered);

  Serial.print("non_Filterd ");
  Serial.print(adc_non_filtered);
  Serial.print(" Filterd  ");
  Serial.println(adc_filtered);

}
