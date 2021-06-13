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
    this->loadingScreen();
}
Interface::loadingScreen()
{
    display->fillScreen(ST77XX_WHITE);
    display->setCursor(18, 30);//Write a start up message on to screen (first by setting the cursor on where we want the text)
    display->setTextColor(ST77XX_BLACK);
    display->setTextSize(3.5);
    display->println(COMPANY_NAME);
    display->setCursor(15, 80);
    display->setTextSize(1);
    display->println("Loading...");
}
Interface::heatingScreen()
{
    display->fillScreen(ST77XX_WHITE);
    display->setCursor(18, 30);//Write a start up message on to screen (first by setting the cursor on where we want the text)
    display->setTextColor(ST77XX_BLACK);
    display->setTextSize(3.5);
    display->println(COMPANY_NAME);
    display->setCursor(15, 80);
    display->setTextSize(1);
    display->println("Reaching temp...");
    display->setCursor(10,110);
    display->println("Current temperature:");
}
Interface::updateHeatingScreen(int temperature)
{
    //prints new value of current temperature on heating screen
    display->fillRect(130,110,SCREEN_WIDTH-130,10,ST77XX_WHITE);//clear previous displayed temperature
    display->setCursor(130,110);
    display->println(String(temperature));

}
Interface::experimentScreen()
{
    display->fillScreen(ST77XX_WHITE);
    display->setCursor(18, 30);//Write a start up message on to screen (first by setting the cursor on where we want the text)
    display->setTextColor(ST77XX_BLACK);
    display->setTextSize(3.5);
    display->println(COMPANY_NAME);
    display->setCursor(15, 80);
    display->setTextSize(1);
    display->println("conducting experiment...");
}
Interface::saveScreen()
{
    display->fillScreen(ST77XX_WHITE);
    display->setCursor(18, 30);//Write a start up message on to screen (first by setting the cursor on where we want the text)
    display->setTextColor(ST77XX_BLACK);
    display->setTextSize(3.5);
    display->println(COMPANY_NAME);
    display->setCursor(15, 80);
    display->setTextSize(1);
    display->println("saving to SD...");
}
Interface::promptScreen(String title,String message)
{
    /*writes a message with a Title and a content on the screen. Waits until D is pressed to continue. Text string needs to be manually adjusted until no cuts are made in the word
    and a space is recommended before each line of the message*/
    display->fillScreen(ST77XX_WHITE);
    display->setCursor(1, 2);//Write a start up message on to screen (first by setting the cursor on where we want the text)
    display->setTextColor(ST77XX_BLACK);
    display->setTextSize(3);
    display->println(title);
    display->setCursor(0, 35);
    display->setTextSize(1);
    display->println(message);
    display->setCursor(110, 100);
    display->println("D-Done");
    while(windowStop == false){
        keyPressed = this->keypad->getKey();
        if (keyPressed != NO_KEY){
            if (keyPressed == 'D'){
                windowStop = true;
            }
        }
    }
    windowStop = false;
    
}
bool Interface::questionScreen(String title,String message)
{
    /*writes a message with a Title and a content on the screen. Waits until A or B is pressed to continue. Text string needs to be manually adjusted until no cuts are made in the word
    and a space is recommended before each line of the message.
    Returns True if A is pressed and false if B is pressed*/
    windowStop = false;
    display->fillScreen(ST77XX_WHITE);
    display->setCursor(1, 2);//Write a start up message on to screen (first by setting the cursor on where we want the text)
    display->setTextColor(ST77XX_BLACK);
    display->setTextSize(3);
    display->println(title);
    display->setCursor(0, 35);
    display->setTextSize(1);
    display->println(message);
    display->setCursor(30, 100);
    display->println("A-Yes");
    display->setCursor(100, 100);
    display->println("B-No");
    while(windowStop == false){
        keyPressed = this->keypad->getKey();
        if (keyPressed != NO_KEY){
            if (keyPressed == 'A'){
                return true;
            }if (keyPressed == 'B'){
                return false;
            }
        }
    }
}
Interface::tempSelect()
{
    /*this screen lets you choose a meassurement temperature with the keypad. It returns the temperature value as an int. IF the return value is 1998, it means not to use temperature control
    (ambient)*/
    int temperature = 0;
    int counter = 0;
    //Set up what doesn't change in the screen
    display->fillScreen(ST7735_WHITE);
    display->setTextColor(ST77XX_BLACK);
    display->setTextSize(2);
    display->setCursor(1, 5);
    display->println("Test Temp. C");
    display->setTextSize(1);
    display->setCursor(10, 100);
    display->println("A-Ambient Temp");
    display->setCursor(110, 100);
    display->println("D-Done");
    display->setCursor(10, 110);
    display->println("*-Erase");
    display->setCursor(110, 110); 

    display->drawRect(20,30,SCREEN_WIDTH-53,50,ST77XX_RED);

    display->setTextSize(6);//text size for numbers on screen
    while(windowStop == false){
        //main loop of temperature selection screen

        if(counter == 0){
            display->setCursor(22, 35);
            display->println("000");
        }else if(counter == 1){
            display->setCursor(22, 35);
            display->println("00"+String(temperature));
        }else if(counter == 2){
            display->setCursor(22, 35);
            display->println("0"+String(temperature));
        }else if(counter == 3){
            display->setCursor(22, 35);
            display->println(String(temperature));
        }
        //check keypad and act accordingly
        keyPressed = this->keypad->getKey();
        if (keyPressed != NO_KEY){
            if (keyPressed == 'D'){
                windowStop = true;
            }else if(keyPressed == '*'){
                counter = 0;
                temperature = 0;
                display->fillRect(21,31,SCREEN_WIDTH-55,48,ST77XX_WHITE);//clear previous displayed temperature
            }else if(keyPressed == 'A'){
                counter = 3;
                temperature = 1998;
                windowStop = true;
            }if (counter < 3 && (keyPressed=='0'||keyPressed=='1'||keyPressed=='2'||keyPressed=='3'||keyPressed=='4'||keyPressed=='5'||keyPressed=='6'||keyPressed=='7'||keyPressed=='8'||keyPressed=='9')){
                display->fillRect(21,31,SCREEN_WIDTH-55,48,ST77XX_WHITE);//clear previous displayed temperature
                counter++;
                temperature = temperature * 10 + int(keyPressed)-48;//-48 factor is due to the string() conversion
            }
        }
    }
    windowStop = false;
    return temperature;
}
Interface::graphView(float xValues[NUMBER_OF_VALUES], float yValues[NUMBER_OF_VALUES])
{
    /*This function opens up a screen to view a graph of the data provided as parameters (x is strain rate, y is shear stress).
    The cursor can be moved to see the values by using the joystick and one can exit out with the key pad*/
    float maxValX = xValues[0];
    float minValX = xValues[0];
    float maxValY = yValues[0];
    float minValY = yValues[0];
    for (int i = 0; i < NUMBER_OF_VALUES; i++) {
      maxValX = max(xValues[i],maxValX);
      minValX = min(xValues[i],minValX);
      maxValY = max(yValues[i],maxValY);
      minValY = min(yValues[i],minValY);
      }
    xScale = (float(SCREEN_WIDTH)-(DATA_GRAPH_ORIGINX+10))/(maxValX-minValX);// determine scaling down of values into pixels leaving some space for the axis labels
    yScale = (DATA_GRAPH_ORIGINY+10-DATA_GRAPH_MAXY)/(maxValY-minValY);//Same for the y values
    
    //Determine the positions where the data points shall be plotted on the screen
    for(int i = 0; i <= NUMBER_OF_VALUES-1; i++){
        xGraphCoordinates[i] = (((xValues[i]-xValues[0])*xScale))+DATA_GRAPH_ORIGINX;
        yGraphCoordinates[i] = (DATA_GRAPH_ORIGINY)-(((yValues[i]-yValues[0])*yScale));
    }
    //Screen Prep----------------------------------------------------------------------------------------------------------------------------------------------------
    display->fillScreen(ST77XX_WHITE);
    display->setTextColor(ST77XX_BLACK);
    display->setTextSize(1);
    //Istruction print --------------------------------------------------------------------------------------------------------------------------------------------
    display->setCursor(120, 100);
    display->println("D-Done");
    //Main Loop------------------------------------------------------------------------------------------------------------------------------------------------------------
    while(windowStop == false){
        //Clear up cursor from last position
        display->fillCircle(graphCursorX,graphCursorY,2,ST77XX_WHITE);
        //axis lines
        display->drawFastHLine(0,DATA_GRAPH_ORIGINY+5,SCREEN_WIDTH,ST7735_BLACK);
        display->drawFastVLine(DATA_GRAPH_ORIGINX-5,0,DATA_GRAPH_ORIGINY+10,ST7735_BLACK);
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
        display->drawCircle(graphCursorX,graphCursorY,2,ST7735_RED);
        //Show position of cursor
        display->setCursor(2, 110);
        display->fillRect(70,110,SCREEN_WIDTH-50,20,ST77XX_WHITE);//clear previous numbers
        display->println("Strain Rate:"+String(((graphCursorX-DATA_GRAPH_ORIGINX)/xScale)+xValues[0]) + "\nShear Stress:" + String(((DATA_GRAPH_ORIGINY-graphCursorY)/yScale)+yValues[0]));
        //check key input
        keyPressed = this->keypad->getKey();
        if (keyPressed != NO_KEY){
            if (keyPressed == 'D'){
                windowStop = true;
            }
        }
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
