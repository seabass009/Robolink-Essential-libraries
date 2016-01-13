/*
  SmartDroneControl.cpp - SmartDroneControl library
  Copyright (C) 2014 RoboLink.  All rights reserved.
  LastUpdate : 2015-08-07
*/

#include "SmartDroneControl.h"
#include "Arduino.h"

void SmartDroneControlClass::begin(void)
{
	SendInterval = 100; //millis seconds		
	
	state = READY;	
	analogOffset = 10; // analog sensor offset
	
	SuccessReceive = false;
	
	pinMode(8, INPUT_PULLUP);
	pinMode(9, INPUT_PULLUP);
	pinMode(10, INPUT_PULLUP);
	
	pinMode(11, INPUT);
	pinMode(12, OUTPUT);
	pinMode(13, OUTPUT);
	pinMode(14, INPUT);
	pinMode(15, INPUT);
	pinMode(16, OUTPUT);
	pinMode(17, OUTPUT);
	pinMode(18, INPUT);
}

void SmartDroneControlClass::Send(void)
{
  byte checkSum = 0x00;
  type = CONTROL;

  //START CODE
  packet[0] = START1;
  packet[1] = START2;

  //header
  packet[2] = type;
  packet[3] = LENGTH;

  //data
  packet[4] = roll;
  packet[5] = pitch;
  packet[6] = yaw;
  packet[7] = throttle;
  packet[8] = event;

  for (int i = 2; i < 9; i++)
  {
    checkSum = checkSum + packet[i];
  }
  packet[9] = checkSum;

  Serial.write(packet, PACKET_LENGTH);

  roll = 0x00;
  pitch = 0x00;
  yaw = 0x00;
  throttle = 0x00;
  event = 0x00;
}

void SmartDroneControlClass::IntervalSend(int interval)
{
    if (TimeCheck(interval))  //delay
    {
      Send();
      PreviousMillis = millis();
    }
}

void SmartDroneControlClass::ButtonPreesHoldWait(int button)
{
  do {
    delay(10);
  }    while (digitalRead(button));
}

void SmartDroneControlClass::ButtonPreesHoldWait(int button1, int button2)
{
  do {
    delay(10);
  }    while (digitalRead(button1) && digitalRead(button2));
}


boolean SmartDroneControlClass::TimeCheck(word interval) //milliseconds
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


boolean SmartDroneControlClass::TimeOutConnetionCheck(word interval) //milliseconds
{
  boolean time = false;
  unsigned long currentMillis = millis();
  if (currentMillis - timeOutConnectionPreviousMillis > interval)
  {
    timeOutConnectionPreviousMillis = currentMillis;
    time = true;
  }
  return time;
}


void SmartDroneControlClass::LED(int command)
{
  if (command == ON)
  {
    digitalWrite(12, HIGH);	
 //   digitalWrite(13, HIGH);    
 //   digitalWrite(16, HIGH);
    digitalWrite(17, HIGH);
  }
  else if (command == OFF)
  {
      digitalWrite(12, LOW);
   //   digitalWrite(13, LOW);      
   //   digitalWrite(16, LOW);
      digitalWrite(17, LOW);
  }
}

void SmartDroneControlClass::Blink(int time, int count)
{
    for (int i = 0; i < count; i++)
    {
   //   digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      digitalWrite(16, HIGH);
  //    digitalWrite(17, HIGH);    
        
      delay(time);      

   //   digitalWrite(12, LOW);
      digitalWrite(13, LOW);      
      digitalWrite(16, LOW);
   //   digitalWrite(17, LOW);
      
      delay(time);
    }
}

void SmartDroneControlClass::PrintSensor()
{

	int analogValue3 = analogRead(A3);  //throttle
	int analogValue4 = analogRead(A4);  //yaw
	int analogValue5 = analogRead(A5);  //pitch
	int analogValue6 = analogRead(A6);  //roll
	
	THROTTLE  = AnalogScaleChange(analogValue3);
	YAW = AnalogScaleChange(analogValue4);
	PITCH = AnalogScaleChange(analogValue5);
	ROLL = AnalogScaleChange(analogValue6);
	Serial.print(throttle);
	Serial.print("\t");
	Serial.print(yaw);
	Serial.print("\t");
	Serial.print(pitch);
	Serial.print("\t");
	Serial.println(roll);
}

/*********************************************************/
/*
void SmartDroneControlClass::ReadSensor(void)
{
  bt1 = digitalRead(11);
  bt2 = digitalRead(12);
  //  bt3 = digitalRead(13);
  bt4 = digitalRead(14);
  //  bt6 = digitalRead(16);
  bt7 = digitalRead(17);
  bt8 = digitalRead(18);
      
  analogValue0 = analogRead(A0);
  analogValue1 = analogRead(A1);
  analogValue2 = analogRead(A2);
  analogValue3 = analogRead(A3);  //throttle
  analogValue4 = analogRead(A4);  //yaw
  analogValue5 = analogRead(A5);  //pitch
  analogValue6 = analogRead(A6);  //roll
  analogValue7 = analogRead(A7);
}
*/

int SmartDroneControlClass::AnalogScaleChange(int analogValue)
{	
    int ScaleChange = map(analogValue, 0, 1023, -100, 100);
    if ((ScaleChange > -1 * analogOffset) && (ScaleChange < analogOffset)) ScaleChange = 0;        
    return ScaleChange;
}

/*********************************************************/
void SmartDroneControlClass::TrimPitch(int value)
{
	if(value == UP)
	{
		event = TRIM_PITCH_INCREASE;		
	}	
	else if(value == DOWN)
	{
		event = TRIM_PITCH_DECREASE;	
	}
	IntervalSend(SendInterval);
}
void SmartDroneControlClass::TrimYaw(int value)
{
	if(value == UP)
	{
		event = TRIM_YAW_INCREASE;		
	}	
	else if(value == DOWN)
	{
		event = TRIM_YAW_DECREASE;	
	}
	IntervalSend(SendInterval);
}

void SmartDroneControlClass::TrimRoll(int value)
{
	if(value == UP)
	{
		event = TRIM_ROLL_INCREASE;		
	}	
	else if(value == DOWN)
	{
		event = TRIM_ROLL_DECREASE;	
	}	
	IntervalSend(SendInterval);
}
/*********************************************************/

void SmartDroneControlClass::Lesson(int i)
{
	 //*************************** PAIRING *****************************//
  // |_-_| Dip Switch2 UP starts the pairing process between the board and the drone, the drone wont necessarly do anything important if it works
  if (digitalRead(8)== HIGH && digitalRead(9) == LOW && digitalRead(10) == HIGH && i != 2)  //PAIRING
  {
    //when pairing have the state as ready and call pairig function
    STATE = READY;
    Pairing();
    //Just a display to show that you are in parring
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
  //■ □ □ □ □ □ □ □ changes state so that it will not fly any longer, if the drone is airborne it will turn the motors automatically and drop
  //for landing need to call stop to cut power to the motors
  else if (digitalRead(11) && !digitalRead(14) && !digitalRead(18) && i != 3) //Stop
  {
    Blink(80, 3); //Just to signify that its turning off
    LED(OFF);    
    Stop();       //call function so as as to send stop package
    ButtonPreesHoldWait(11);  //button up
    STATE = READY;
  }
  
  //*************************** RESETYAW ***************************//
  //□ □ □ ■ ■ □ □ □ sets it so that the drone is ready to fly if tjis is not activated the controls will not work
  else if (digitalRead(14) && !digitalRead(11) && i != 4)  //RESETYAW (direction set) & flyingReady
  {
    Blink(80, 4); //Just to signify that its turning on
    LED(ON);
    ResetYaw();   
    ButtonPreesHoldWait(14);  //button up
    STATE = FLY;
  }
  
  //*************************** Trim Reset ***********************//
  //■ □ □ □ □ □ □ ■ The drone keeps its trim changes however if this is activated any trim changes you made will b deleted
  else if (digitalRead(18) && digitalRead(11) && !digitalRead(14) && i != 5)  //Trim Reset
  {
    Blink(50, 3);
    LED(ON);
    digitalWrite(17, HIGH);
    TrimReset();
    ButtonPreesHoldWait(11, 18); //button up
  }
  
    //*************************** TrimMode Set ***********************//
  //□ □ □ □ □ □ □ ■
  
  if(i != 6)
  {
	if (digitalRead(18))  //TrimMode Set
	{
		if (STATE == FLY) STATE = TRIM;
	}
	else if (!digitalRead(18))  //TrimMode Set
	{
		if (STATE == TRIM) STATE = FLY;
	}
  }
  
    if (STATE == FLY && i != 1)
	{
		Fly();
	}
    else if (STATE == TRIM && i != 7)
	{
		Trim();
	}
}

void SmartDroneControlClass::Circle(void)
{
	
	Lesson(1);

  if (STATE == FLY)
  {
    if (lock == 0)
    {
      lock = 1;
      timer = millis();
    }

    if (millis() - timer < 500)
    {
      THROTTLE = 60;
    }
    else if (millis() - timer < 1000)
    {
      THROTTLE = 0;
      PITCH = 100;
    }
    else if (millis() - timer < 1500)
    {
      THROTTLE = 0;
      ROLL = 100;
    }
    else if (millis() - timer < 2000)
    {
      THROTTLE = 0;
      PITCH = -100;
    }
    else if (millis() - timer < 2500)
    {

      THROTTLE = 0;
      ROLL = -100;
    }
    else if (millis() - timer < 3000)
    {
      THROTTLE = 0;
     }
    else if (millis() - timer < 5000)
    {
      THROTTLE = -100;
    }

    else
    {
      Stop();
    }
    IntervalSend(SEND_INTERVAL);
  }
  else
  {
    lock = 0;
  }
}
void SmartDroneControlClass::Fly(void)
{
    THROTTLE  = AnalogScaleChange(analogRead(A3));
    YAW = AnalogScaleChange(analogRead(A4));
    PITCH = AnalogScaleChange(analogRead(A5));
    ROLL = AnalogScaleChange(analogRead(A6));
    IntervalSend(SEND_INTERVAL);	
}
void SmartDroneControlClass::Trim(void)
{
	YAW = AnalogScaleChange(analogRead(A4));
    PITCH = AnalogScaleChange(analogRead(A5));
    ROLL = AnalogScaleChange(analogRead(A6));

    if (PITCH == 0) PreviousEventPitch = 0;
    if (ROLL == 0) PreviousEventRoll = 0;
    if (YAW == 0) PreviousEventYaw = 0;

    if (PITCH > 90)
    {
      if (TRIM_PITCH_INCREASE != PreviousEventPitch)
      {
        PreviousEventPitch = TRIM_PITCH_INCREASE;
        THROTTLE = 0;            PITCH = 0;            ROLL = 0;            YAW = 0;
        TrimPitch(UP);
      }
    }
    else if (PITCH < -90)
    {
      if (TRIM_PITCH_DECREASE != PreviousEventPitch)
      {
        PreviousEventPitch = TRIM_PITCH_DECREASE;
        THROTTLE = 0;            PITCH = 0;            ROLL = 0;            YAW = 0;
        TrimPitch(DOWN);
      }
    }
    else if (ROLL > 90)
    {
      if (TRIM_ROLL_DECREASE != PreviousEventRoll)
      {
        PreviousEventRoll = TRIM_ROLL_DECREASE;
        THROTTLE = 0;            PITCH = 0;            ROLL = 0;            YAW = 0;
        TrimRoll(DOWN);
      }
    }
    else if (ROLL < -90)
    {
      if (TRIM_ROLL_INCREASE != PreviousEventRoll)
      {
        PreviousEventRoll = TRIM_ROLL_INCREASE;
        THROTTLE = 0;            PITCH = 0;            ROLL = 0;            YAW = 0;
        TrimRoll(UP);
      }
    }
    else if (YAW > 90)
    {
      if (TRIM_YAW_INCREASE != PreviousEventYaw)
      {
        PreviousEventYaw = TRIM_YAW_INCREASE;
        THROTTLE = 0;            PITCH = 0;            ROLL = 0;            YAW = 0;
        TrimYaw(UP);
      }
    }
    else if (YAW < -90)
    {
      if (TRIM_YAW_DECREASE != PreviousEventYaw)
      {
        PreviousEventYaw = TRIM_YAW_DECREASE;
        THROTTLE = 0;            PITCH = 0;            ROLL = 0;            YAW = 0;
        TrimYaw(DOWN);
      }
    }
}
void SmartDroneControlClass::Pairing(void)
{
    event = PAIRING;
    
    IntervalSend(SendInterval);
}

void SmartDroneControlClass::Stop(void)
{
    event = STOP;
    
    IntervalSend(SendInterval);
}

void SmartDroneControlClass::ResetYaw(void)
{
    event = RESET_YAW;
    
    IntervalSend(SendInterval);
}

void SmartDroneControlClass::TrimReset(void)
{
    event = TRIM_RESET;
    
    IntervalSend(SendInterval);
}

void SmartDroneControlClass::MissileFire(void)
{	
    event = MISSILE;
    
    IntervalSend(SendInterval);
}

void SmartDroneControlClass::TeamSelect(byte team)
{	
    event = team;
    
    IntervalSend(SendInterval);
}



void SmartDroneControlClass::Receive(void)
{	
 SuccessReceive = false;
 team = -1;
 flightStatus = -1;
 energy = -1;
 battery = -1;
 missileQuantity  = -1;
        	
  if (Serial.available() > 0)
  {
    int input = Serial.read();
    cmdBuff[cmdIndex++] = (char)input;
    if (cmdIndex >= MAX_CMD_LENGTH)
    {
      checkHeader = 0;
      cmdIndex = 0;
    }
    else
    {
      if (cmdIndex == 2)
      {
        if ((cmdBuff[0] == 0x0A) && (cmdBuff[1] == 0x55))
        {
          checkHeader = 1;
        }
        else
        {
          checkHeader = 0;
          cmdIndex = 0;
        }
      }
      
      if (checkHeader == 1)
      {
        if (cmdIndex == 3)
        {
          if (cmdBuff[2] == 0x21)
          {
            int type = cmdBuff[2];
            checkHeader = 2;
          }
          else
          {
            checkHeader = 0;
            cmdIndex = 0;
          }
        }
      }

      if (checkHeader == 2)
      {
        if (cmdIndex == 4)
        {
          int length = cmdBuff[3];
        }

        else if (cmdIndex == 10)
        {
          int cs = cmdBuff[9];

          byte  checkSum = 0;
          for (int i = 2; i < 9; i++)
          {
            checkSum += cmdBuff[i];
          }
          
          if (cs == checkSum)
          {
            SuccessReceive = true;
            
            team = cmdBuff[4];
            flightStatus = cmdBuff[5];
            energy = cmdBuff[6];
            battery = cmdBuff[7];
            missileQuantity = cmdBuff[8];       
          }
   
          checkHeader = 0;
          cmdIndex = 0;
        }
      }
    }
  }
}





/*********************************************************/

SmartDroneControlClass SmartDroneControl;