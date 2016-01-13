#include <SmartInventor.h>
#include <Servo.h>
#include <EEPROM.h>

#define SERVO1_ANGLE_ADDRESS 10
#define SERVO2_ANGLE_ADDRESS 11

void ShootingBot()
{
  SmartInventor.TVRemoconUse();
  SmartInventor.DCMotorUse();

  //□ □ □ □ □ □ □ □
  //11,12,13,14,15,16,17,18

  //Led Output
  for (int thisPin = 11; thisPin <= 18; thisPin++)   {
    pinMode(thisPin, OUTPUT);
  }
  digitalWrite(13, HIGH);
  digitalWrite(14, HIGH);
  digitalWrite(15, HIGH);
  digitalWrite(16, HIGH);

  byte speedMeterL = 13;
  byte speedMeterR = 16;

  Servo servo1;
  Servo servo2;

  int sp = 50;
  int keyData = 0;

  int delta1 = 0;
  int delta2 = 0;

  int deg1 = EEPROM.read(SERVO1_ANGLE_ADDRESS);
  int deg2 = EEPROM.read(SERVO2_ANGLE_ADDRESS);

  servo1.attach(27);
  servo2.attach(28);
  servo1.write(deg1);
  servo2.write(deg2);

  while (1)
  {
    keyData = SmartInventor.TVRemoconData();

    if (keyData == KEY_F4)
    {
      delta2++;

      if (delta2 >= 60 ) delta2 = 60;
      servo2.write(deg2 + delta2);
      delay(20);
    }
    else if (keyData == KEY_F3)
    {
      delta2--;

      if (delta2 <= -50 ) delta2 = -50;
      servo2.write(deg2 + delta2);
      delay(20);
    }

    else if (keyData == KEY_C)
    {
      for (int j = deg1; j < deg1 + 35; j++)
      {
        servo1.write(j);
        delay(10);
      }
      delay(500);
      servo1.write(deg1 + 60);
      delay(1000);
      servo1.write(deg1 - 40);
      delay(1000);
      servo1.write(deg1);
    }

    else if (keyData == KEY_U)
    {
      SmartInventor.DCMove(forward, sp);
    }
    else if (keyData == KEY_D)
    {
      SmartInventor.DCMove(backward, sp);
    }
    else if (keyData == KEY_L)
    {
      SmartInventor.DCMove(left, sp);
    }
    else if (keyData == KEY_R)
    {
      SmartInventor.DCMove(right, sp);
    }

    else SmartInventor.DCMove(stop, sp);

    if (keyData == KEY_F1)
    {
      if (SmartInventor.TimeCheckMillis(500))
      {
        if (sp < 90)
        {
          sp = sp + 20;

          speedMeterL = speedMeterL - 1;
          speedMeterR = speedMeterR + 1;

          digitalWrite(speedMeterL, HIGH);
          digitalWrite(speedMeterR, HIGH);

          SmartInventor.Buzz(1160, 12);
        }
      }
    }
    else if (keyData == KEY_F2)
    {
      if (SmartInventor.TimeCheckMillis(500))
      {
        if (sp > 30)
        {
          sp = sp - 20;

          digitalWrite(speedMeterL, LOW);
          digitalWrite(speedMeterR, LOW);
          speedMeterL = speedMeterL + 1;
          speedMeterR = speedMeterR - 1;

          SmartInventor.Buzz(1160, 12);
        }
      }
    }
  }
}



