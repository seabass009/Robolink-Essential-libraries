#include <SmartDroneControl.h>

void setup() {
  Serial.begin(9600);
  SmartDroneControl.begin();
  delay(1000);
  ANALOG_OFFSET = 10; // josystick analog offset
}

void loop()
{

  byte dipBt1 = digitalRead(8);
  byte dipBt2 = digitalRead(9);
  byte dipBt3 = digitalRead(10);

  byte bt1 = digitalRead(11);
  byte bt4 = digitalRead(14);
  byte bt5 = digitalRead(15);
  byte bt8 = digitalRead(18);

  int analogValue3 = analogRead(A3);  //throttle
  int analogValue4 = analogRead(A4);  //yaw
  int analogValue5 = analogRead(A5);  //pitch
  int analogValue6 = analogRead(A6);  //roll

  THROTTLE  = SmartDroneControl.AnalogScaleChange(analogValue3);
  YAW = SmartDroneControl.AnalogScaleChange(analogValue4);
  PITCH = SmartDroneControl.AnalogScaleChange(analogValue5);
  ROLL = SmartDroneControl.AnalogScaleChange(analogValue6);

  //dip switch
  if (dipBt1 == HIGH) Serial.print("1");
  else Serial.print("0");
  if (dipBt2 == HIGH) Serial.print("1");
  else Serial.print("0");
  if (dipBt3 == HIGH) Serial.print("1");
  Serial.print("\t");

  //bottom sensor
  if (bt1 == HIGH) Serial.print("1");
  else Serial.print("0");

  Serial.print("0");
  Serial.print("0");

  if (bt4 == HIGH) Serial.print("1");
  else Serial.print("0");

  if (bt5 == HIGH) Serial.print("1");
  else Serial.print("0");

  Serial.print("0");
  Serial.print("0");

  if (bt8 == HIGH) Serial.print("1");
  else Serial.print("0");

  Serial.print("\t");
  Serial.print(THROTTLE);
  Serial.print("\t");
  Serial.print(YAW);
  Serial.print("\t");
  Serial.print(PITCH);
  Serial.print("\t");
  Serial.print(ROLL);
  Serial.print("\t");

  if (dipBt1 == HIGH && dipBt2 == LOW && dipBt3 == HIGH)  //PAIRING
  {
    Serial.println("Pairing");
  }

  else if (bt1 && !bt4 && !bt8) //Stop
  {
    Serial.println("Stop");
  }

  else if (bt4 && !bt1)  //RESETYAW (direction set) & flyingReady
  {
    Serial.println("Reset Yaw & Flying Ready");
  }

  else if (bt8 && bt1 && !bt4)  //Trim Reset
  {
    Serial.println("Trim Reset");
  }

  else if (bt8)  //TrimMode Set
  {
    Serial.println("Trim Mode");
  }
  else
  {
    Serial.println();
  }


}
