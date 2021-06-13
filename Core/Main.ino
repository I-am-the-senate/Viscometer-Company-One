                                                                             //PROJEKT FLOWIO
                                                                 //AUTHOR Barbero Carlos Yu gu Adam Nowek
#include "Interface.h"
#include "Keypad.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS A5
#include <Adafruit_INA260.h>
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
int PT1 = 1;//peltier pins 1 and 2
int PT2 = 4;
float testdataX [10] = {0.0,500.0,1000.0,1500.0,2000.0,2500.0,3000,3500,4000,4500};
float testdataY [10] = {1.435,2.33,3.686,6.426,5.444,8.42,9.23,10.55,12.33,13.22};
byte rowPins[4] = {0, 5, 6, 7};
byte colPins[4] = {8, A2, A3,A4};
Interface pinkInterface = Interface (rowPins,colPins,A1,A0,10,9,-1);

int analogPin = 3;
float t=0;
float tp=0;
float Voltages[] = {50, 100, 150, 200, 250};
float powers[]={0,0,0,0,0};
float freqs[]={0,0,0,0,0};
Adafruit_INA260 ina260 = Adafruit_INA260();//stuff for motor control
float freq=0;

int wantedTemp = 1998;//just in case, we start it at the ambient value. It is an int for the desired temperature in degrees C
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
  
  pinMode(2, INPUT);
pinMode(analogPin, OUTPUT);
attachInterrupt(digitalPinToInterrupt(2), addRotation, RISING);//stuff for motor control
}

void addRotation() {
 t=millis();}

float Getfreq(){
 freq=0;
 int i=0;
 while(i!=1){
 noInterrupts();
if(tp!=0 and t!=tp)
{
  freq=1000/(t-tp);
  i++;
}
tp=t;
interrupts();
delay (1);
  }
  return freq;
}

void test(){
for (int i=4;i>=0; i--)
{analogWrite(analogPin, Voltages[i]);
delay (500);
freqs[i]=Getfreq();
powers[i]=ina260.readPower();
Serial.print("freq:");
Serial.print(freqs[i]);
Serial.print("Current: ");
  Serial.print(ina260.readCurrent());
  Serial.println(" mA");

  Serial.print("Bus Voltage: ");
  Serial.print(ina260.readBusVoltage());
  Serial.println(" mV");

  Serial.print("Power: ");
  Serial.print(ina260.readPower());
  Serial.println(" mW");

}}

void loop() {
  // put your main code here, to run repeatedly:
  pinkInterface.promptScreen("Set-Up", " Prepare sample and water\n bath.\n When finished, press D.");
  do // we cannot boil the water bath. 1998 is the secret value for Ambient
  {
    wantedTemp = pinkInterface.tempSelect();
    if(wantedTemp >= 100 && wantedTemp != 1998){
      pinkInterface.promptScreen("Warning"," Temperature selected is\n above 100 C. Water boils, select another temp");
    }
  }while (wantedTemp >= 100 && wantedTemp != 1998);
  pinkInterface.promptScreen("Ready"," Ready to start test.\n press D to start");
  //Getting to temperature phase
  pinkInterface.heatingScreen();//with these couple of lines we put a screen while it reaches a target temperature, and we can update the displayed current temp
  //temperature control starts
  if (wantedTemp != 1998)
  {
  for (;int(sensors.getTempCByIndex(0)) != wantedTemp;)
  {
    sensors.requestTemperatures();
  if (int(sensors.getTempCByIndex(0))<wantedTemp ){
  analogWrite (1,255);// Heat up
  analogWrite (2,0);
}
else if (int(sensors.getTempCByIndex(0))>wantedTemp){
analogWrite (1,0);//cool down
analogWrite (2,255);
}
else {
analogWrite (1,0);//do nothing
analogWrite (2,0);
}
  pinkInterface.updateHeatingScreen(int(sensors.getTempCByIndex(0)));
  }
  }

  //Experiment phase (take the meassurements while keeping temperature)
  pinkInterface.experimentScreen();
  delay(1000);//replace this with the code loop needed
  
  
  //Data Analysis phase
  pinkInterface.graphView(testdataX,testdataY);//The number of datapoints required can be easily changed hardcoded
  if (pinkInterface.questionScreen("Save?"," Do you want to save\n to an SD?")){
    pinkInterface.saveScreen();
    delay(1000); //replace this with code to save to SD
  }
  pinkInterface.promptScreen("Finished", " You can now unplug the\n device.\n Or press D to start over");
  
}
