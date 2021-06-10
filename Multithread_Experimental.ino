#include <SCoop.h>
#include <Adafruit_INA260.h>
int analogPin = 3;
int t=0;
int tp=0;
int powers[]={0,0,0,0,0};
int freqs[]={0,0,0,0,0};
Adafruit_INA260 ina260 = Adafruit_INA260();
int freq=0;

defineTask(TaskTest1);
defineTask(TaskTest2);
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

void addRotation() {
 
 
 t=millis();
 
}
void TaskTest1::setup()
{
  
Serial.begin(9600);
while (!Serial)
{
sleep(10);
}
Serial.println("Adafruit INA260 Test");
 if (!ina260.begin())
{Serial.println("Couldn't find INA260 chip");
while (1);
 }
}
void TaskTest1::loop()
{
 Serial.print("Current: ");
Serial.print(ina260.readCurrent());
Serial.println(" mA");
 Serial.print("Bus Voltage: ");
Serial.print(ina260.readBusVoltage());
Serial.println(" mV");
 Serial.print("Power: ");
Serial.print(ina260.readPower());
Serial.println(" mW");
 Serial.println();
sleep(1000);
}

void TaskTest2::setup()
{
  
Serial.begin(9600);
pinMode(analogPin, OUTPUT);
attachInterrupt(digitalPinToInterrupt(2), addRotation, RISING);

}

void TaskTest2::loop()
{
for (int i=255;i>=150; i-50)
{
analogWrite(analogPin,i);
sleep(500);
freqs[i]=Getfreq();
powers[i]=ina260.readPower();
Serial.print("freq:");
Serial.print(freqs[i]);

  Serial.print("\n");
}
}
void setup()
{
  mySCoop.start();
}
  
void loop()
{
  
  yield();
}
