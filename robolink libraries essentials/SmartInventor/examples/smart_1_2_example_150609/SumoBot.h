#include <SmartInventor.h>

void SumoBot()
{
  SmartInventor.DCMotorUse();
  
  //□ □ □ □ □ □ □ □
  //11,12,13,14,15,16,17,18
  //BOTTOM SENSOR USE
  pinMode(11, INPUT);
  pinMode(18, INPUT);

  byte speed = 50;
  unsigned int  BackTime = 0;
  unsigned int  TurnTime = 0;

  SmartInventor.DCMove(forward, speed);

  while (1)
  {
    BackTime++;
    TurnTime++;
    if (BackTime >= 200) BackTime = 100;
    if (TurnTime >= 500) TurnTime = 100;

    if (digitalRead(11) == LOW)
    {
      SmartInventor.DCMove(backward, speed);
      SmartInventor.Buzz(1160, 12);
      delay(100 + BackTime);
      SmartInventor.DCMove(right, speed);
      delay(100 + TurnTime);
      SmartInventor.DCMove(forward, speed);
    }

    else if (digitalRead(18) == LOW)
    {
      SmartInventor.DCMove(backward, speed);
      SmartInventor.Buzz(1160, 12);
      delay(100 + BackTime);
      SmartInventor.DCMove(left, speed);
      delay(100 + TurnTime);
      SmartInventor.DCMove(forward, speed);
    }

  }
}


