#include "Interface.h"

#include "Arduino.h"

Interface::Interface(int Rx, int Ry)
{
    //Pins definitions
    this->Rx = Rx;
    this->Ry = Ry;
    //Set pin modes
    pinMode(Rx,INPUT);
    pinMode(Ry,INPUT);
    
    this->xSpeed = 0;//Speeds of movement for joystick
    this->ySpeed = 0;
}

Interface::~Interface()
{

}
JoystickPos Interface::getJoyPos()
{
    int x = (analogRead(this->Rx)*9/1024)-4;//values of x and y axis can take the values -4,-3,-2,-1,0,1,2,3,4. 
    int y = (analogRead(this->Ry)*9/1024)-4;//Having 9 levels of precision means having clearly defined resting state and possibility for speeds
    JoystickPos values(x,y);
    return values;
}


JoystickPos::JoystickPos(int x, int y)
{
    this->x = x;
    this->y = y;
}
JoystickPos::~JoystickPos()
{

}