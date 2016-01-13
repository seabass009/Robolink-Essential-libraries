#include <SmartDroneControl.h>

int PreviousEventPitch = 0;
int PreviousEventYaw = 0;
int PreviousEventRoll = 0;

long PreviousMillis = 0;
int soundPin = 7;

void setup()
{
  Serial.begin(9600);
  SmartDroneControl.begin();
  delay(500);
}

/***********************************************************/

void loop()
{
  byte dipBt1 = digitalRead(8);
  byte dipBt2 = digitalRead(9);
  byte dipBt3 = digitalRead(10);

  int bt1 = digitalRead(11);
  int bt4 = digitalRead(14);
  int bt8 = digitalRead(18);

  int analogValue3 = analogRead(A3);  //throttle
  int analogValue4 = analogRead(A4);  //yaw
  int analogValue5 = analogRead(A5);  //pitch
  int analogValue6 = analogRead(A6);  //roll

  SmartDroneControl.Receive();

  if (SUCESS_RECEIVE == true)
  {
    //energy display (12,13,16,17)
    //□ □ ■ ■ □ □ ■ ■ □ □

    //binary display
    //□ □ ■ ■ □ □ ■ ■ □ □ 15
    // .
    // .
    // .
    //□ □ □ ■ □ □ ■ □ □ □ 6
    //□ □ □ ■ □ □ □ ■ □ □ 5
    //□ □ □ ■ □ □ □ □ □ □ 4
    //□ □ □ □ □ □ ■ ■ □ □ 3
    //□ □ □ □ □ □ ■ □ □ □ 2
    //□ □ □ □ □ □ □ ■ □ □ 1
    //□ □ □ □ □ □ □ □ □ □ 0

    digitalWrite(12, (Energy / 8));             // 8
    digitalWrite(13, (Energy % 8) / 4);         // 4
    digitalWrite(16, ((Energy % 8) % 4) / 2);   // 2
    digitalWrite(17, ((Energy % 8) % 4) % 2);   // 1
  }

  //*************************** PAIRING *****************************//
  // |_-_| Dip Switch2 UP
  if (dipBt1 == HIGH && dipBt2 == LOW && dipBt3 == HIGH)  //PAIRING
  {
    STATE = READY;
    SmartDroneControl.Pairing();
    SuccessSound();
    delay(2500);
    digitalWrite(13, HIGH);
    digitalWrite(16, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    digitalWrite(16, LOW);
    delay(200);
    digitalWrite(12, HIGH);
    digitalWrite(17, HIGH);
    delay(200);
    digitalWrite(12, LOW);
    digitalWrite(17, LOW);
    delay(200);
  }

  //*************************** TeamSelect ***********************//
  // |_-_| Dip Switch3 UP
  else if (dipBt1 == HIGH && dipBt2 == HIGH && dipBt3 == LOW)  // TeamSelect
  {
    //■ □ □ □ □ □ □ □
    if (bt1 && !bt4 && !bt8) //TEAM_RED
    {
      SmartDroneControl.Blink(80, 3);
      SmartDroneControl.TeamSelect(TEAM_BLUE);
      SuccessSound();
      SmartDroneControl.ButtonPreesHoldWait(11);  //button up
    }

    //□ □ □ □ □ □ □ ■
    else if (!bt1 && !bt4 && bt8)   // TEAM_BLUE
    {
      SmartDroneControl.Blink(80, 3);
      SmartDroneControl.TeamSelect(TEAM_RED);
      SuccessSound();
      SmartDroneControl.ButtonPreesHoldWait(18);  //button up
    }
  }

  //*************************** Missile Fire ***********************//
  //□ □ □ □ □ □ □ ■
  else if (!bt1 && !bt4 && bt8)
  {
      SmartDroneControl.MissileFire();
      MissileSound();
      SmartDroneControl.ButtonPreesHoldWait(18);  //button up
  }

  //**************************** STOP ******************************//
  //■ □ □ □ □ □ □ □
  else if (bt1 && !bt4 && !bt8) //Stop
  {
    SmartDroneControl.Blink(80, 3);

    SmartDroneControl.Stop();
    SmartDroneControl.ButtonPreesHoldWait(11);  //button up
    STATE = READY;
  }

  //*************************** RESETYAW ***************************//
  //□ □ □ ■ ■ □ □ □
  else if (!bt1 && bt4 && !bt8)  //RESETYAW (direction set) && flyingReady
  {
    SmartDroneControl.Blink(80, 4);
    SmartDroneControl.ResetYaw();
    SmartDroneControl.ButtonPreesHoldWait(14);  //button up
    STATE = FLY;
  }

  //*************************** FLY CONTROL ***********************//
  if (STATE == FLY)
  {
    THROTTLE  = SmartDroneControl.AnalogScaleChange(analogValue3);
    YAW = SmartDroneControl.AnalogScaleChange(analogValue4);
    PITCH = SmartDroneControl.AnalogScaleChange(analogValue5);
    ROLL = SmartDroneControl.AnalogScaleChange(analogValue6);
    SmartDroneControl.IntervalSend(SEND_INTERVAL);
  }
}

//******************************Time******************************************//

void WaitMotion(word interval)
{
  do {
    SmartDroneControl.Receive();
  }
  while (!(TimeCheck(interval)));
}

boolean TimeCheck(word interval) //milliseconds
{
  boolean time = false;
  unsigned long currentMillis = millis();
  if (currentMillis - PreviousMillis > interval)
  {
    PreviousMillis = currentMillis;
    time = true;
  }
  return time;
}


//**************************** Sound *************************************//
void SuccessSound()
{
  tone(soundPin, 988, 62);
  PreviousMillis = millis();
  WaitMotion(80);

  tone(soundPin, 1319, 500);
  PreviousMillis = millis();
  WaitMotion(650);

  noTone(soundPin);
}

void MissileSound()
{
  for (int i = 1000; i < 2000; i = i + 1) {
    tone(soundPin, i, 5);
  }
  PreviousMillis = millis();
  WaitMotion(10);

  noTone(soundPin);
}



