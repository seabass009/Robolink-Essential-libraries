#include <SmartInventor.h>

const int analogInPin0 = A0;  // Analog input pin that the potentiometer is attached to
const int analogInPin2 = A2;  // Analog input pin that the potentiometer is attached to

int leftSensorValue;
int rightSensorValue;

int angles1 = 0;
int angles2 = 0;

word  selectLevelA0 = 0;
word  selectLevelA2 = 0;

double kp = 0;
double ki = 0;
double kd = 0;

double kis = 0;
double pid = 0;

void Segway()
{
  delay(500);
  SmartInventor.Buzz(NOTE_C5, 12);
  SmartInventor.DCMotorUse();

  for (int thisPin = 11; thisPin < 18; thisPin++)  {
    pinMode(thisPin, OUTPUT);
  }

  for (int thisPin = 11; thisPin < 18; thisPin++)  {
    digitalWrite(thisPin, HIGH);
  }

  for (int thisPin = 18 - 1; thisPin >= 11; thisPin--) {
    delay(200);
    digitalWrite(thisPin, LOW);
  }

  selectLevelA0 = SmartInventor.SensorCalibration(A0);
  selectLevelA2 = SmartInventor.SensorCalibration(A2);

  if (F_CPU == 16000000UL)
  {
    kp = 1.6;
    ki = 0.5;
    kd = 1.5;
  }
  else if (F_CPU == 7372800UL)
  {
    kp = 1;
    ki = 0.325;
    kd = 0.5;
  }

  while (1)
  {
    leftSensorValue = analogRead(A0) - selectLevelA0;
    rightSensorValue  = analogRead(A2) - selectLevelA2;

    angles1 = leftSensorValue - rightSensorValue;
    angles1 = angles1 / 5;

    kis = kis + ki * 0.01 * angles1; // kis 변수에 현재 기울기에 ki와 0.01를 곱한 값을 누적
    pid = kp * angles1 + kis + kd * (angles1 - angles2); // 각각 연산한 값을 더하여 pid변수에 대입
    angles2 = angles1; // 현재 기울기를 기록

    if (0 > pid)
    {
      pid = pid * -1;
      SmartInventor.DCMove(backward, pid);
    }
    else if (0 < pid)
    {
      SmartInventor.DCMove(forward, pid);
    }
  }
}

