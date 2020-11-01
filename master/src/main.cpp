#include <MQ2.h>
#include <Wire.h> 
//I2C pins declaration

int Analog_Input = 15;
int lpg, co, smoke;

MQ2 mq2(Analog_Input);

void setup(){
  Serial.begin(9600);
  mq2.begin();
}
void loop(){
  //float* values= mq2.read(true); //set it false if you don't want to print the values in the Serial
  //lpg = values[0];
  lpg = mq2.readLPG();
  //co = values[1];
  co = mq2.readCO();
  //smoke = values[2];
  smoke = mq2.readSmoke();
  Serial.println("LPG:");
  Serial.println(lpg);
  Serial.println(" CO:");
  Serial.println(co);
  Serial.println("SMOKE:");
  Serial.println(smoke);
  Serial.println(" PPM");
  delay(1000);
}