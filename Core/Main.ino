#include "Interface.h"

Interface * myTest; //Interface class needs to be adressed as a pointer since its constructor 

void setup() {
  // put your setup code here, to run once:
  myTest = new Interface (A1,A0,10,9,-1);//PUT at start of set up. Will show an initial screen message while the rest of the rest of the machine preps
}

void loop() {
  // put your main code here, to run repeatedly:

}
