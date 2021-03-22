#include "Interface.h"
#include <Adafruit_GFX.h>   
#include <Adafruit_ST7735.h>


Interface::Interface(byte rowPins[KEYROWS], byte colPins[KEYCOLS],int RX, int RY, int TFT_CS, int TFT_DC, int TFT_RST)
{
    //Pins definitions
    this->RX = RX;
    this->RY = RY;
    this->TFT_CS = TFT_CS;
    this->TFT_DC = TFT_DC;
    this->TFT_RST = TFT_RST;
    this->xSpeed = 0;//Speeds of movement for joystick
    this->ySpeed = 0;
    graphCursorX = DATA_GRAPH_ORIGINX+10;
    graphCursorY = DATA_GRAPH_ORIGINY-10;
    joyPosForMovement = new JoystickXY(DATA_GRAPH_ORIGINX,DATA_GRAPH_ORIGINY);
    //Set window stop to false by default
    windowStop = false;
    //prepare keypad
    rowPins = rowPins; //connect to the row pinouts of the keypad
    colPins = colPins; //connect to the column pinouts of the keypad
    keys[0][0] = '1';//Keys must be assigned individually as an initializer list cannot be assigned to an array after declaration
    keys[0][1] = '2';
    keys[0][2] = '3';
    keys[0][3] = 'A';
    keys[1][0] = '4';
    keys[1][1] = '5';
    keys[1][2] = '6';
    keys[1][3] = 'B';
    keys[2][0] = '7';
    keys[2][1] = '8';
    keys[2][2] = '9';
    keys[2][3] = 'C';
    keys[3][0] = '*';
    keys[3][1] = '0';
    keys[3][2] = '#';
    keys[3][3] = 'D';
    /*
    keys = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
    };*/
    keypad = new Keypad(makeKeymap(keys), rowPins, colPins, KEYROWS, KEYCOLS);
    
}
Interface::~Interface()
{
    delete display;//pointers need to be deleted when the object is deconstructed
    delete joyPosForMovement;
    delete keypad;
}
Interface::init()
{
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
}
Interface::graphView(float xValues[NUMBER_OF_VALUES], float yValues[NUMBER_OF_VALUES])
{
    /*This function opens up a screen to view a graph of the data provided as parameters (x is strain rate, y is shear stress).
    The cursor can be moved to see the values by using the joystick and one can exit out with the key pad*/
    xScale = (float(SCREEN_WIDTH)-(DATA_GRAPH_ORIGINX+10))/(xValues[NUMBER_OF_VALUES-1]-xValues[0]);// determine scaling down of values into pixels leaving some space for the axis labels
    yScale = (DATA_GRAPH_ORIGINY+10)/(yValues[NUMBER_OF_VALUES-1]-yValues[0]);//Same for the y values

    //Determine the positions where the data points shall be plotted on the screen
    for(int i = 0; i <= NUMBER_OF_VALUES-1; i++){
        xGraphCoordinates[i] = (((xValues[i]-xValues[0])*xScale))+DATA_GRAPH_ORIGINX;
        yGraphCoordinates[i] = (DATA_GRAPH_ORIGINY)-(((yValues[i]-yValues[0])*yScale));
        Serial.println(yGraphCoordinates[i]);
    }
    //Screen Prep----------------------------------------------------------------------------------------------------------------------------------------------------
    display->fillScreen(ST77XX_BLACK);
    display->setTextColor(ST77XX_GREEN);
    display->setTextSize(1);
    //Main Loop------------------------------------------------------------------------------------------------------------------------------------------------------------
    while(windowStop == false){
        //Clear up cursor from last position
        display->fillCircle(graphCursorX,graphCursorY,2,ST7735_BLACK);
        //axis lines
        display->drawFastHLine(0,DATA_GRAPH_ORIGINY+5,SCREEN_WIDTH,ST7735_WHITE);
        display->drawFastVLine(DATA_GRAPH_ORIGINX-5,0,DATA_GRAPH_ORIGINY+10,ST7735_WHITE);
        //Draw points on graph
        for(int i = 0; i <= NUMBER_OF_VALUES-1; i++){
            display->fillCircle(xGraphCoordinates[i], yGraphCoordinates[i], 3, ST7735_BLUE);
        }
        //Check Input
        *joyPosForMovement = getJoyPos();
        //Update Cursor position on graph
        if (joyPosForMovement->x > 0 && graphCursorX <= SCREEN_WIDTH){
            graphCursorX += joyPosForMovement->x;
        }else if (joyPosForMovement->x < 0 && graphCursorX >= DATA_GRAPH_ORIGINX){
            graphCursorX += joyPosForMovement->x;
        }
        if (joyPosForMovement->y > 0 && graphCursorY <= DATA_GRAPH_ORIGINY){
            graphCursorY += joyPosForMovement->y;
        }else if (joyPosForMovement->y < 0 && graphCursorY >= 0){
            graphCursorY += joyPosForMovement->y;
        }
        //Draw cursor
        display->drawCircle(graphCursorX,graphCursorY,2,ST7735_GREEN);
        //Show position of cursor
        display->setCursor(2, 110);
        display->fillRect(70,110,SCREEN_WIDTH-50,20,ST7735_BLACK);//clear previous numbers
        display->println("Strain Rate:"+String(((graphCursorX-DATA_GRAPH_ORIGINX)/xScale)+xValues[0]) + "\nShear Stress:" + String(((DATA_GRAPH_ORIGINY-graphCursorY)/yScale)+yValues[0]));
    }
    windowStop = false;

}
JoystickXY Interface::getJoyPos()
{
    int x = (analogRead(this->RX)*9/1024)-4;//values of x and y axis can take the values -4,-3,-2,-1,0,1,2,3,4. 
    int y = (analogRead(this->RY)*9/1024)-4;//Having 9 levels of precision means having clearly defined resting state and possibility for speeds
    JoystickXY values(x,y);
    return values;
}


JoystickXY::JoystickXY(int x = DATA_GRAPH_ORIGINX, int y = DATA_GRAPH_ORIGINY)
{
    this->x = x;
    this->y = y;
}
JoystickXY::JoystickXY(JoystickXY& jstick)
{
    this->x = jstick.x;
    this->y = jstick.y;
}

JoystickXY::~JoystickXY()
{

}