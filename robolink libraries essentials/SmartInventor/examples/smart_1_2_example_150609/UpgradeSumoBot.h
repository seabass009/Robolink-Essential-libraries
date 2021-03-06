#include <SmartInventor.h>
#include <Servo.h>

#define PUNCH1_ANGLE_A 0
#define PUNCH1_ANGLE_B 119

#define PUNCH2_ANGLE_A 179
#define PUNCH2_ANGLE_B 120

void UpgradeSumoBot()
{
  SmartInventor.DCMotorUse();

  //□ □ □ □ □ □ □ □
  //11,12,13,14,15,16,17,18
  //BOTTOM SENSOR USE
  
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(14,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(17,OUTPUT);
  
  pinMode(11, INPUT);
  pinMode(18, INPUT);

  Servo myservo1;
  Servo myservo2;

  myservo1.attach(27);
  myservo2.attach(28);

  myservo1.write(PUNCH1_ANGLE_A);
  myservo2.write(PUNCH2_ANGLE_B);

  byte speed = 50;
  byte punchFlag = 0;
  unsigned int  BackTime = 0;
  unsigned int  TurnTime = 0;

  SmartInventor.DCMove(forward, speed);

  while (1)
  {
    BackTime++;
    TurnTime++;
    if (BackTime >= 200) BackTime = 100;
    if (TurnTime >= 500) TurnTime = 100;

    if (SmartInventor.TimeCheckMillis(800))
    {
      if (punchFlag == 1)
      {
        myservo1.write(PUNCH1_ANGLE_A);
        myservo2.write(PUNCH2_ANGLE_B);
        punchFlag = 0;
      }
      else
      {
        myservo1.write(PUNCH1_ANGLE_B);
        myservo2.write(PUNCH2_ANGLE_A);
        punchFlag = 1;
      }
    }
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
    if (SmartInventor.TimeCheckMillis(800))
    {
      if (punchFlag == 1)
      {
        myservo1.write(PUNCH1_ANGLE_A);
        myservo2.write(PUNCH2_ANGLE_B);
        punchFlag = 0;
      }
      else
      {
        myservo1.write(PUNCH1_ANGLE_B);
        myservo2.write(PUNCH2_ANGLE_A);
        punchFlag = 1;
      }
    }
  }
}


