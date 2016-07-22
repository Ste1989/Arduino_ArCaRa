
#include "MeOrion.h"
#include <Wire.h>
#include <SoftwareSerial.h>
MeDCMotor motor1(PORT_1);
uint8_t motorSpeed = 200;
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  motor1.run(motorSpeed);
  delay(1000);
  motor1.run(-motorSpeed);
  delay(1000);

  
}
