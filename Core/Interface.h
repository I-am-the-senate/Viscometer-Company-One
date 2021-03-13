#ifndef _INTERFACE_H
#define _INTERFACE_H
#include "Arduino.h"
#include <Adafruit_GFX.h>   
#include <Adafruit_ST7735.h>
#include <SPI.h>

class JoystickXY
/*Type to store x and y position of joystic*/
{
private:
    
public:
    int x;
    int y;
    JoystickXY(int,int);
    ~JoystickXY();
};

class Interface
/*Main class that is used to make a interface object which will control all input and output with the user*/
//NOTE!!! Interface Class Needs to be declared as pointer in the main code initally and then created in the setup as its constructor runs program instructions that need to go there!
{
private:
    //pins
    int RX;
    int RY;
    uint8_t TFT_CS;
    uint8_t TFT_DC;
    uint8_t TFT_RST;
    //tft display object
    Adafruit_ST7735 * display;//Needs to be adressed as pointer (IF not it would give error for not giving arguments to contructor here)
    //cursor Speed
    int xSpeed;
    int ySpeed;
    //cursor position
    int cursorX;
    int cursorY;

public:
    Interface(int RX, int RY, int TFT_CS, int TFT_DC, int TFT_RST = -1);    //Constructor prototype|||| By default the screen reset pin will be set to the reset button
    ~Interface();   //Destructor prototype

    JoystickXY getJoyPos();
};
#endif