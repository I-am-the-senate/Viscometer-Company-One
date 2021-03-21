#include "Interface.h"

Interface * myTest; //Interface class needs to be adressed as a pointer since its constructor 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  myTest = new Interface (A1,A0,10,9,-1);//PUT at start of set up. Will show an initial screen message while the rest of the rest of the machine preps
  Serial.println("Interface prepared");
  float testdataX [6] = {0.0,500.0,1000.0,1500.0,2000.0,2500.0};
  float testdataY [6] = {1.435,2.33,3.686,6.426,5.444,8.42};
  myTest->graphView(testdataX,testdataY);
}

void loop() {
  // put your main code here, to run repeatedly:

}
