#include <LinearRegression.h>
float OMEGA[5];
float Viscosity;
float Target0;
LinearRegression LR;
Serial.begin(9600);   
// Translate frequency to angular velocity
for (int i=0;i<5;i++)
{
  OMEGA[i] = 1/6.28 freqs[i];

//Perform linear regression

LR.learn(OMEGA[i],/*CALIBRATION DATA GOES HERE*/powers[i]/OMEGA[i]);
}
//RESULT FORMULA 
Serial.print("Y = ");
Serial.print(values[0]);
Serial.print("*X + ");
Serial.println(values[1]); 
Viscosity = values[0]
