/*****************************
//SmartInvertor 1 & 2 Step All Example
//2015-06-09 RoboLink
******************************/
#include <SmartInventor.h>
#include <Servo.h>
#include <EEPROM.h>

#include "ProgramSelection.h"

//1 step include
#include "MouseBot.h"       //1
#include "SumoBot.h"        //2
#include "CrabBot.h"        //3
#include "SoccerBot.h"      //4
#include "Segway.h"         //5 
#include "Trailer.h"        //6
#include "Viking.h"         //7
#include "TurretBot.h"      //8
#include "RiceBarleyBot.h"  //9
#include "GorillarBot.h"    //10
#include "BugBot.h"         //11
#include "LineTracer.h"     //12
#include "RemoconCar.h"


//2 step include
#include "Bike.h"               //1
#include "DuneBuggy.h"          //2
#include "GripBot.h"            //3
#include "Rodeo.h"              //4
#include "UpgradeMouseBot.h"    //5
#include "HandballBot.h"        //6
#include "UpgradeSumoBot.h"     //7
#include "ShootingBot.h"        //8
#include "MelodyBot.h"          //9
#include "LiftBot.h"            //10
#include "ServoAngleTuner.h"    //12


#define NORMAL  0
#define UPGRADE 1

#define DipSw1  8
#define DipSw2  9
#define DipSw3  10

void setup()
{
  pinMode(DipSw1, INPUT_PULLUP);
  pinMode(DipSw2, INPUT_PULLUP);
  pinMode(DipSw3, INPUT_PULLUP);
  delay(500);
}

void loop()
{
  if (digitalRead(DipSw3) == LOW) RemoconCar(); //Remote Car - dipSw3 UP

  //step 1 select - dipSw2 Down
  else if (digitalRead(DipSw2) == HIGH)
  {
    byte programNumber =  ProgramSelection(NORMAL);  //sensor or remocon use

    if (programNumber == 1)  MouseBot();
    else if (programNumber == 2)  SumoBot();
    else if (programNumber == 3)  CrabBot();
    else if (programNumber == 4)  SoccerBot();
    else if (programNumber == 5)  Segway();
    else if (programNumber == 6)  Trailer();
    else if (programNumber == 7)  Viking();
    else if (programNumber == 8)  TurretBot();
    else if (programNumber == 9)  RiceBarleyBot();
    else if (programNumber == 10)  GorillarBot();
    else if (programNumber == 11)  BugBot();
    else if (programNumber == 12)  LineTracer();
    else RemoconCar();  //RemoteCar
  }

  //step 2 select - dipSw2 UP
  else if (digitalRead(DipSw2) == LOW)
  {
    byte programNumber =  ProgramSelection(UPGRADE);  //remocon only use

    if (programNumber == 1)  Bike();
    else if (programNumber == 2)  DuneBuggy();
    else if (programNumber == 3)  GripBot();
    else if (programNumber == 4)  Rodeo();
    else if (programNumber == 5)  UpgradeMouseBot();
    else if (programNumber == 6)  HandballBot(); //no test
    else if (programNumber == 7)  UpgradeSumoBot();
    else if (programNumber == 8)  ShootingBot(); //no test
    else if (programNumber == 9)  MelodyBot();
    else if (programNumber == 10)  LiftBot(); //no test
    else if (programNumber == 11)  LineTracer(); ////linetracer mode(grip)
    else if (programNumber == 12)  ServoAngleTuner();
    else RemoconCar();  //RemoteCar
  }

}
