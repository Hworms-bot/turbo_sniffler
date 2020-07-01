/*!
 * @file Ventilator
 *
 * @section intro_sec Introduction
 *
 * This is a Arduino Program for the Emergency Ventilator Development Board
 *
 * @section author Author
 *
 * Written by Hans-Georg Worms for Emergency Ventilator Project funded by the 
 * Blackwood Lane Winery & University of the Fraser Valley.
 * 
 */
 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MCP23017.h>
#include "AtmegaButton.h"

/*!
 * Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
 */
#define OLED_RESET     13 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/*!
 * Declaration for MCP23017 GPIO expansion header. 
 */
#define MCP23017_ADDR 0x22    // I2C address is 0x22 
MCP23017 mcp17 = MCP23017(MCP23017_ADDR);

/*
 * Declaration for the toggle and button switches
 */
const int startSwitchPin = 11;
const int valve1MedairPin = 12;
const int valve2MedairPin = 13;
const int valve3MedairPin = 2;
const int valve4MedairPin = 3;
const int valve5OxygenPin = 4;
const int valve6OxygenPin = 5;
const int valve7OxygenPin = 6;
const int valve8OxygenPin = 7;
const int valve9InspirePin = 8;
const int valve10ExpirePin = 9;
const int valve10AssistPin = 10;

const int diffPressSW = 0;
const int timerPatientSelect = 1;
const int chargingPowerSensor = 2;
const int upBtn = 3;
const int downBtn = 4;
const int leftBtn = 5;
const int rightBtn = 6;
const int enterBtn = 7;
const int alarmPowerFail = 8;
const int alarmBatteryLow = 9;
const int alarmExpirePressure = 10;
const int alarmInspirePressure = 11;
const int alarm12 = 12;
const int alarm13 = 13;
const int alarm14 = 14;
const int alarm15 = 15;

//////////////////////////////////////////////
///////////////   SETUP   ////////////////////
//////////////////////////////////////////////
void setup() {
  Wire.begin();
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    //Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(100); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  lcdWelcomeScreen();


/*!
 * MCP23017 IC Setup and Initialization
 */
  mcp17.init();
  /*!
   * PinSetup, Inputs and Outputs
   */
  pinMode(startSwitchPin, INPUT);
   
  pinMode(valve1MedairPin, OUTPUT);
  pinMode(valve2MedairPin, OUTPUT);
  pinMode(valve3MedairPin, OUTPUT);
  pinMode(valve4MedairPin, OUTPUT);
  pinMode(valve5OxygenPin, OUTPUT);
  pinMode(valve6OxygenPin, OUTPUT);
  pinMode(valve7OxygenPin, OUTPUT);
  pinMode(valve8OxygenPin, OUTPUT);
  pinMode(valve9InspirePin, OUTPUT);
  pinMode(valve10ExpirePin, OUTPUT);
  pinMode(valve10AssistPin, OUTPUT);
  
  mcp17.pinMode(diffPressSW, INPUT);
  mcp17.pinMode(timerPatientSelect, INPUT);
  mcp17.pinMode(chargingPowerSensor, INPUT);
  mcp17.pinMode(upBtn, INPUT);
  mcp17.pinMode(downBtn, INPUT);
  mcp17.pinMode(leftBtn, INPUT);
  mcp17.pinMode(rightBtn, INPUT);
  mcp17.pinMode(enterBtn, INPUT);
  mcp17.pinMode(alarmPowerFail, OUTPUT);
  mcp17.pinMode(alarmBatteryLow, OUTPUT);
  mcp17.pinMode(alarmExpirePressure, OUTPUT);
  mcp17.pinMode(alarmInspirePressure, OUTPUT);
  mcp17.pinMode(alarm12, OUTPUT);
  mcp17.pinMode(alarm13, OUTPUT);  
  mcp17.pinMode(alarm14, OUTPUT);  
  mcp17.pinMode(alarm15, OUTPUT);  
}

//////////////////////////////////////////////
//////////////   MAIN LOOP   /////////////////
//////////////////////////////////////////////

void loop() {
  bool startState = digitalRead(startSwitchPin); 
  Serial.print("startSwitchPinValue");
  Serial.print(startState);
  Serial.println();
  digitalWrite(valve5OxygenPin, startState);
  digitalWrite(valve6OxygenPin, startState);
  digitalWrite(valve7OxygenPin, startState);
  digitalWrite(valve8OxygenPin, startState);

  bool timer_Patient_SW = mcp17.digitalRead(timerPatientSelect);
  digitalWrite(valve1MedairPin, timer_Patient_SW);
  digitalWrite(valve2MedairPin, timer_Patient_SW);
  digitalWrite(valve3MedairPin, timer_Patient_SW);
  digitalWrite(valve4MedairPin, timer_Patient_SW);

  if (mcp17.digitalRead(upBtn)){
    lcdWriteString("Up", 0, 0, true);
    mcp17.digitalWrite(alarmPowerFail, HIGH);
    mcp17.digitalWrite(alarmBatteryLow, LOW);
    mcp17.digitalWrite(alarmExpirePressure, LOW);
    mcp17.digitalWrite(alarmInspirePressure, LOW);
    mcp17.digitalWrite(alarm12, LOW);
  } else if (mcp17.digitalRead(downBtn)){
    lcdWriteString("DOWN", 0, 0, true);
    mcp17.digitalWrite(alarmBatteryLow, HIGH);
     mcp17.digitalWrite(alarmPowerFail, LOW);
    mcp17.digitalWrite(alarmExpirePressure, LOW);
    mcp17.digitalWrite(alarmInspirePressure, LOW);
    mcp17.digitalWrite(alarm12, LOW);
  } else if(mcp17.digitalRead(leftBtn)){
    lcdWriteString("LEFT", 0, 0, true);
    mcp17.digitalWrite(alarmExpirePressure, HIGH);
    mcp17.digitalWrite(alarmPowerFail, LOW);
    mcp17.digitalWrite(alarmBatteryLow, LOW);
    mcp17.digitalWrite(alarmInspirePressure, LOW);
    mcp17.digitalWrite(alarm12, LOW);
  } else if(mcp17.digitalRead(rightBtn)){
    lcdWriteString("RIGHT", 0, 0, true);
    mcp17.digitalWrite(alarmInspirePressure, HIGH);
    mcp17.digitalWrite(alarmPowerFail, LOW);
    mcp17.digitalWrite(alarmBatteryLow, LOW);
    mcp17.digitalWrite(alarmExpirePressure, LOW);
    mcp17.digitalWrite(alarm12, LOW);
  } else if (mcp17.digitalRead(enterBtn)){
    lcdWriteString("ENTER", 0, 0, true);
    mcp17.digitalWrite(alarm12, HIGH);
    mcp17.digitalWrite(alarmPowerFail, LOW);
    mcp17.digitalWrite(alarmBatteryLow, LOW);
    mcp17.digitalWrite(alarmExpirePressure, LOW);
    mcp17.digitalWrite(alarmInspirePressure, LOW);
  } else{
    lcdWelcomeScreen();
    mcp17.digitalWrite(alarmPowerFail, LOW);
    mcp17.digitalWrite(alarmBatteryLow, LOW);
    mcp17.digitalWrite(alarmExpirePressure, LOW);
    mcp17.digitalWrite(alarmInspirePressure, LOW);
    mcp17.digitalWrite(alarm12, LOW);
  } 

  bool current3 = mcp17.digitalRead(upBtn);
  mcp17.digitalWrite(alarmPowerFail, current3);
  
  Serial.print("Up Button state: ");
  Serial.print(current3);
  Serial.println();
  

  delay(100);    
}



///////////////////////////////////
//////////FUNCTION BLOCKS//////////
///////////////////////////////////



void lcdWelcomeScreen(void) {
  display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.println("  COVID ");
  display.println("Ventilator");
   

  display.display();
}

void lcdWriteString(String message, int curs_x, int curs_y, bool clearDisplay) {
  if(clearDisplay){
    display.clearDisplay();
  }
  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(WHITE, BLACK);        // Draw white text
  display.setCursor(curs_x,curs_y);
  display.println(F("          "));
  display.setCursor(curs_x,curs_y);
  display.println(message);
  //display.print(F("0x")); display.println(45192, HEX);

  display.display();
  //delay(2000);
}
