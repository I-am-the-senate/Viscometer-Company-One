#include "Arduino.h"

class JoystickPos
/*Type to store x and y position of joystic*/
{
private:
    
public:
    int x;
    int y;
    JoystickPos(int,int);
    ~JoystickPos();
};

class Interface
/*Main class that is used to make a interface object which will control all input and output with the user*/
{
private:
    //pins
    int Rx;
    int Ry;

    int xSpeed;
    int ySpeed;

public:
    Interface(int, int);    //Constructor prototype 
    ~Interface();   //Destructor prototype

    JoystickPos getJoyPos();
};
