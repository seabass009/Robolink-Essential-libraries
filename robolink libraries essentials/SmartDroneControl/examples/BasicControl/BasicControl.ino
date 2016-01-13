#include <SmartDroneControl.h>

int PreviousEventPitch = 0;
int PreviousEventYaw = 0;
int PreviousEventRoll = 0;

void setup()
{
  Serial.begin(9600);
  SmartDroneControl.begin();
  delay(1000);
}

/***********************************************************/

void loop()
{
  byte dipBt1 = digitalRead(8);
  byte dipBt2 = digitalRead(9);
  byte dipBt3 = digitalRead(10);

  byte bt1 = digitalRead(11);
  byte bt4 = digitalRead(14);
  byte bt8 = digitalRead(18);

  int analogValue3 = analogRead(A3);  //throttle
  int analogValue4 = analogRead(A4);  //yaw
  int analogValue5 = analogRead(A5);  //pitch
  int analogValue6 = analogRead(A6);  //roll

  //*************************** PAIRING *****************************//
  // |_-_| Dip Switch2 UP
  if (dipBt1 == HIGH && dipBt2 == LOW && dipBt3 == HIGH)  //PAIRING
  {
    STATE = READY;
    SmartDroneControl.Pairing();
    delay(1500);

    digitalWrite(13, HIGH);
    digitalWrite(16, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    digitalWrite(16, LOW);
    delay(100);
    digitalWrite(12, HIGH);
    digitalWrite(17, HIGH);
    delay(100);
    digitalWrite(12, LOW);
    digitalWrite(17, LOW);
    delay(100);
  }

  //**************************** STOP ******************************//
  //■ □ □ □ □ □ □ □
  else if (bt1 && !bt4 && !bt8) //Stop
  {
    SmartDroneControl.Blink(80, 3);
    SmartDroneControl.LED(OFF);
    SmartDroneControl.Stop();
    SmartDroneControl.ButtonPreesHoldWait(11);  //button up
    STATE = READY;
  }
  
  //*************************** RESETYAW ***************************//
  //□ □ □ ■ ■ □ □ □
  else if (bt4 && !bt1)  //RESETYAW (direction set) & flyingReady
  {
    SmartDroneControl.Blink(80, 4);
    SmartDroneControl.LED(ON);
    SmartDroneControl.ResetYaw();
    SmartDroneControl.ButtonPreesHoldWait(14);  //button up
    STATE = FLY;
  }
  
  //*************************** Trim Reset ***********************//
  //■ □ □ □ □ □ □ ■
  else if (bt8 && bt1 && !bt4)  //Trim Reset
  {
    SmartDroneControl.Blink(50, 3);
    SmartDroneControl.LED(ON);
    digitalWrite(17, HIGH);
    SmartDroneControl.TrimReset();
    SmartDroneControl.ButtonPreesHoldWait(11, 18); //button up
  }
  
  //*************************** TrimMode Set ***********************//
  //□ □ □ □ □ □ □ ■
  if (bt8)  //TrimMode Set
  {
    if (STATE == FLY) STATE = TRIM;
  }
  else if (!bt8)  //TrimMode Set
  {
    if (STATE == TRIM) STATE = FLY;
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

  //*************************** TRIM CONTROL ***********************//
  else if (STATE == TRIM)
  {
    YAW = SmartDroneControl.AnalogScaleChange(analogValue4);
    PITCH = SmartDroneControl.AnalogScaleChange(analogValue5);
    ROLL = SmartDroneControl.AnalogScaleChange(analogValue6);

    if (PITCH == 0) PreviousEventPitch = 0;
    if (ROLL == 0) PreviousEventRoll = 0;
    if (YAW == 0) PreviousEventYaw = 0;

    if (PITCH > 90)
    {
      if (TRIM_PITCH_INCREASE != PreviousEventPitch)
      {
        PreviousEventPitch = TRIM_PITCH_INCREASE;
        THROTTLE = 0;            PITCH = 0;            ROLL = 0;            YAW = 0;
        SmartDroneControl.TrimPitch(UP);
      }
    }
    else if (PITCH < -90)
    {
      if (TRIM_PITCH_DECREASE != PreviousEventPitch)
      {
        PreviousEventPitch = TRIM_PITCH_DECREASE;
        THROTTLE = 0;            PITCH = 0;            ROLL = 0;            YAW = 0;
        SmartDroneControl.TrimPitch(DOWN);
      }
    }
    else if (ROLL > 90)
    {
      if (TRIM_ROLL_DECREASE != PreviousEventRoll)
      {
        PreviousEventRoll = TRIM_ROLL_DECREASE;
        THROTTLE = 0;            PITCH = 0;            ROLL = 0;            YAW = 0;
        SmartDroneControl.TrimRoll(DOWN);
      }
    }
    else if (ROLL < -90)
    {
      if (TRIM_ROLL_INCREASE != PreviousEventRoll)
      {
        PreviousEventRoll = TRIM_ROLL_INCREASE;
        THROTTLE = 0;            PITCH = 0;            ROLL = 0;            YAW = 0;
        SmartDroneControl.TrimRoll(UP);
      }
    }
    else if (YAW > 90)
    {
      if (TRIM_YAW_INCREASE != PreviousEventYaw)
      {
        PreviousEventYaw = TRIM_YAW_INCREASE;
        THROTTLE = 0;            PITCH = 0;            ROLL = 0;            YAW = 0;
        SmartDroneControl.TrimYaw(UP);
      }
    }
    else if (YAW < -90)
    {
      if (TRIM_YAW_DECREASE != PreviousEventYaw)
      {
        PreviousEventYaw = TRIM_YAW_DECREASE;
        THROTTLE = 0;            PITCH = 0;            ROLL = 0;            YAW = 0;
        SmartDroneControl.TrimYaw(DOWN);
      }
    }
  }
}

//************************************************************************//



