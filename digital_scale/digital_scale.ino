#include "HX711.h"

// HX711.DOUT	- pin #A1
// HX711.PD_SCK	- pin #A0

HX711 scale(A1, A0);		// parameter "gain" is ommited; the default value 128 is used by the library
int cnt = 0;
void setup() {
  Serial.begin(38400);
  Serial.println("HX711 Demo");

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());			// print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));  	// print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);	// print the average of 5 readings from the ADC minus tare weight (not set) divided 
						// by the SCALE parameter (not set yet)  

  scale.set_scale(-18.75);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();				        // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided 
						// by the SCALE parameter set with set_scale

  Serial.println("Readings:");
}

void loop() {
  Serial.print("one reading:\t");
  Serial.print(cnt, 1);
  Serial.print("\t");
  float raw = scale.get_value(10);
  float weight = raw / -18.75;
  weight = weight < 0 ? 0 : weight;
  Serial.print(weight, 1);
  Serial.print("\t");
  Serial.println(raw, 1);
  if (raw >= 50000 || abs(raw) < 50000 && abs(raw) > 300) {
    cnt++;
  }
  else {
    cnt = 0;
  }
  if (cnt == 2) {
    cnt = 0;
    scale.tare();
    Serial.println("Reset!");
  }
  if (raw <= - 50000) {
    Serial.println("Powering up!");
    int weight_time = 10;
    while (weight_time > 0) {
      weight_time--;
      raw = scale.get_value(10);
      weight = raw / -18.75;
      weight = weight < 0 ? 0 : weight;
      Serial.print(weight, 1);
      Serial.print("\t");
      Serial.println(raw, 1);
    }
    Serial.println("Powering down!");
  }
//  Serial.print("\t| average:\t");
//  Serial.println(scale.get_units(10), 1);

//  scale.power_down();			        // put the ADC in sleep mode
//  delay(250);
//  scale.power_up();
}
