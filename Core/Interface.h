#ifndef _INTERFACE_H
#define _INTERFACE_H
#include "Arduino.h"
#include <Adafruit_GFX.h>   
#include <Adafruit_ST7735.h>
#include <Keypad.h>
#include <SPI.h>

//Constants to be defined. MAY BE CHANGED BASED ON OUR PARAMETERS
#define NUMBER_OF_VALUES  10 //Number of datapoints that will be obtained to display on graph
#define SCREEN_WIDTH  160
#define SCREEN_HEIGHT  128
#define DATA_GRAPH_ORIGINX 10
#define DATA_GRAPH_ORIGINY 88
#define DATA_GRAPH_MAXY 5
//commonly repeated messages (to save program memory)
#define COMPANY_NAME "flow.io"
//Keypad constants
#define KEYROWS 4
#define KEYCOLS 4

class JoystickXY
/*Type to store x and y position of joystic*/
{
private:
    
public:
    int x;
    int y;
    JoystickXY(int,int);
    JoystickXY(JoystickXY&);
    ~JoystickXY();
};

class Interface
/*Main class that is used to make a interface object which will control all input and output with the user*/
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
    //General bool to check whether the user has requested to be out of a window
    bool windowStop;
    //cursor Speed
    int xSpeed;
    int ySpeed;
    //cursor position
    int cursorX;
    int cursorY;
    //For graphing
    double xScale;
    double yScale;
    int xGraphCoordinates[NUMBER_OF_VALUES];
    int yGraphCoordinates[NUMBER_OF_VALUES];
    int graphCursorX;
    int graphCursorY;
    //Joystick position object
    JoystickXY * joyPosForMovement;
    //Keypad workings
    char keys[KEYROWS][KEYCOLS];
    byte rowPins[KEYROWS];
    byte colPins[KEYCOLS];
    char keyPressed; //key that has been pressed
    //Temperature selection

public:
    Interface(byte rowPins[KEYROWS], byte colPins[KEYCOLS],int RX, int RY, int TFT_CS, int TFT_DC, int TFT_RST = -1);    //Constructor prototype|||| By default the screen reset pin will be set to the reset button
    ~Interface();   //Destructor prototype
    init();
    loadingScreen();
    heatingScreen();
    saveScreen();
    updateHeatingScreen(int temperature);
    experimentScreen();
    promptScreen(String title, String message);
    bool questionScreen(String title, String message);
    int tempSelect();
    graphView(float xValues[NUMBER_OF_VALUES], float yValues[NUMBER_OF_VALUES]);//Swithes to a graph visualization screen
    JoystickXY getJoyPos();//Position of Joystick
    Keypad * keypad;//keypad object
};
#endif
