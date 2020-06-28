#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MCP23017.h>
#include "MCP23008mod.h"


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     13 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Declaration for MCP23017 GPIO expansion header.
#define MCP23017_ADDR 0x22    // I2C address is 0x22 
#define MCP23008_ADDR 0x21

MCP23017 mcp17 = MCP23017(MCP23017_ADDR);
MCP23008mod mcp08;

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
  
  mcp17.init();
  mcp17.portMode(MCP23017Port::A, 0b11111111);          //Port A as input
  mcp17.portMode(MCP23017Port::B, 0); //Port B as output

  mcp17.writeRegister(MCP23017Register::GPIO_A, 0x00);  //Reset port A 
  mcp17.writeRegister(MCP23017Register::GPIO_B, 0x00);  //Reset port B

  // GPIO_B reflects the same logic as the input pins state
  mcp17.writeRegister(MCP23017Register::IPOL_B, 0x00);
  // Uncomment this line to invert inputs (press a button to lit a led)
  //mcp.writeRegister(MCP23017Register::IPOL_B, 0xFF);

  mcp08.begin(MCP23008_ADDR);
  mcp08.pinMode(0, INPUT);
  mcp08.pullUp(0, HIGH);      //turn on a 100K pullup internally
  mcp08.flipPolarity();
}

void loop() {
    uint8_t currentA;

//    currentA = mcp17.readPort(MCP23017Port::A);
//    mcp17.writePort(MCP23017Port::B, currentA);

  mcp17.digitalWrite(11, mcp08.digitalRead(0));
}

void lcdWelcomeScreen(void) {
  display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.println("  COVID ");
  display.println("Ventilator");
   

  display.display();
}
