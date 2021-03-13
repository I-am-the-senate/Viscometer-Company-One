#include "Interface.h"
#include <Adafruit_GFX.h>   
#include <Adafruit_ST7735.h>


Interface::Interface(int RX, int RY, int TFT_CS, int TFT_DC, int TFT_RST)
{
    //Pins definitions
    this->RX = RX;
    this->RY = RY;
    this->TFT_CS = TFT_CS;
    this->TFT_DC = TFT_DC;
    this->TFT_RST = TFT_RST;
    //Set pin modes for joystick control
    pinMode(RX,INPUT);
    pinMode(RY,INPUT);
    //Start up the display
    display = new Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
    display->initR(INITR_BLACKTAB);  // Init ST7735S chip, black tab. TBD: Investigate into what this does
    display->setRotation(1);//Put the screen on Landscape mode
    display->fillScreen(ST77XX_BLACK);
    display->setCursor(18, 30);//Write a start up message on to screen (first by setting the cursor on where we want the text)
    display->setTextColor(ST77XX_WHITE);
    display->setTextSize(3.5);
    display->println("flow.io");
    display->setCursor(15, 80);
    display->setTextColor(ST77XX_WHITE);
    display->setTextSize(1);
    display->println("getting things ready...");
    
    this->xSpeed = 0;//Speeds of movement for joystick
    this->ySpeed = 0;
}

Interface::~Interface()
{
    delete display;//pointers need to be deleted when the object is deconstructed
}
JoystickXY Interface::getJoyPos()
{
    int x = (analogRead(this->RX)*9/1024)-4;//values of x and y axis can take the values -4,-3,-2,-1,0,1,2,3,4. 
    int y = (analogRead(this->RY)*9/1024)-4;//Having 9 levels of precision means having clearly defined resting state and possibility for speeds
    JoystickXY values(x,y);
    return values;
}


JoystickXY::JoystickXY(int x, int y)
{
    this->x = x;
    this->y = y;
}
JoystickXY::~JoystickXY()
{

}