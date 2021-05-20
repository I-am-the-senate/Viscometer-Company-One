                                                                             //PROJEKT FLOWIO
                                                                 //AUTHOR Barbero Carlos Yu gu Adam Nowek
#include "Interface.h"
#include "Keypad.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS A5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
int PT1 = 1;//peltier pins 1 and 2
int PT2 = 2;
int Tset;
float testdataX [6] = {0.0,500.0,1000.0,1500.0,2000.0,2500.0};
float testdataY [6] = {1.435,2.33,3.686,6.426,5.444,8.42};
byte rowPins[4] = {0, 5, 6, 7};
byte colPins[4] = {8, A2, A3,A4};
Interface pinkInterface = Interface (rowPins,colPins,A1,A0,10,9,-1);

int wantedTemp = 1998;//just in case, we start it at the ambient value. It is an int for the desired temperature in degrees C
int currentTemp = 0;//to keep current temperature
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinkInterface.init();//PUT at start of set up. Will show an initial screen message while the rest of the rest of the machine preps
  Serial.println("Interface prepared");
  
  /*Put here any set up code needed that will run while the loading screen is shown*/
  //peltier initialization part
pinMode (PT1,OUTPUT);
pinMode (PT2,OUTPUT); 

analogWrite (1,0);//initialize to ground
analogWrite (2,0);
  //peltier initialization complete
}

void loop() {
  // put your main code here, to run repeatedly:
  pinkInterface.promptScreen("Set-Up", " Please insert sample into machine.\n The sample should cover\n the bob.\n Make sure the water bath\n is filled.\n When finished, press D.");
  do // we cannot boil the water bath. 1998 is the secret value for Ambient
  {
    wantedTemp = pinkInterface.tempSelect();
    if(wantedTemp >= 100 && wantedTemp != 1998){
      pinkInterface.promptScreen("Warning"," Temperature selected is\n above max temperature of\n 100 C. To prevent water\n boiling you need to\n select another\n temperature.");
    }
  }while (wantedTemp >= 100 && wantedTemp != 1998);
  pinkInterface.promptScreen("Ready"," Ready to start test.\n press D to carry out the\n test");
  //Getting to temperature phase
  pinkInterface.heatingScreen();//with these couple of lines we put a screen while it reaches a target temperature, and we can update the displayed current temp
  //temperature control starts
  
  for (;INT(sensors.getTempCByIndex(0)) != wantedTemp;)
  {
  if (INT(sensors.getTempCByIndex(0))<wantedTemp ){
  analogWrite (1,255);// Heat up
  analogWrite (2,0);
}
else if (INT(sensors.getTempCByIndex(0))>wantedTemp){
analogWrite (1,0);//cool down
analogWrite (2,255);
}
else {
analogWrite (1,0);//do nothing
analogWrite (2,0);
}
//  for motor contorl part to start
  pinkInterface.updateHeatingScreen(currentTemp);//here it is convenient to put some sort of while loop to check wether the temperature has been kepts within a range for a time or similar
  delay(1000);//placeholder
  }
  

  //Experiment phase (take the meassurements while keeping temperature)
  pinkInterface.experimentScreen();
  delay(1000);//replace this with the code loop needed
  
  if (pinkInterface.questionScreen("Complete"," Now you can view your\n data.\n Do you want to save it\n to an SD first?")){
    pinkInterface.saveScreen();
    delay(1000); //replace this with code to save to SD
  }
  //Data Analysis phase
  pinkInterface.graphView(testdataX,testdataY);//The number of datapoints required can be easily changed hardcoded

  pinkInterface.promptScreen("Finished", " You can now unplug the\n device.\n Alternatively, if you\n want to start over,\n you can press D");
}
