#include <SmartInventor.h>

void BugBot()
{
  SmartInventor.TVRemoconUse();
  SmartInventor.RFRemoconUse(57600);
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

  byte speed = 50;
  int keyData = 0;

  delay(500);

  while (1)
  {
    if (RFdata != 0)   keyData =  SmartInventor.RFRemoconData();
    else              keyData =  SmartInventor.TVRemoconData();

    if (keyData != 0) Serial.println(keyData);

    if (keyData == KEY_U)
    {
      SmartInventor.DCMove(forward, speed);
    }
    else if (keyData == KEY_D)
    {
      SmartInventor.DCMove(backward, speed);
    }
    else if (keyData == KEY_L)
    {
      SmartInventor.DCMove(left, speed);
    }
    else if (keyData == KEY_R)
    {
      SmartInventor.DCMove(right, speed);
    }
    else
    {
      SmartInventor.DCMove(LOOSE, 0); //not use speed value
    }

    if (keyData == KEY_F1)
    {
      if (SmartInventor.TimeCheckMillis(500))
      {
        if (speed < 90)
        {
          speed = speed + 20;

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
        if (speed > 30)
        {
          speed = speed - 20;

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

