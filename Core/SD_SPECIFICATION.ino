
#include <SD.h>
#include <SPI.h>
int CS = 10;
pinMode(CS,OUTPUT);
digitalWrite(10,LOW)
if (SD.begin())
{ 
digitalWrite(10,HIGH)
PinkInterface.promptScreen("Saving");
digitalWrite(10,LOW)
File Latest_test_result;
Latest_test_result = SD.open("latest_test_result.csv",FILE_WRITE);
for(i=0;i<5;i++)
{
Latest_test_result.printIn(Voltages[i])
Latest_test_result.printIn(powers[i])
Latest_test_result.printIn(freqs[i])
Latest_test_result.printIn(OMEGA[i])
}
digitalWrite(10,HIGH)
PinkInterface.promptScreen("Saved");
}
else
{
digitalWrite(10,HIGH)
pinkInterface.promptScreen("SD card initialization failed","check sd");
}
