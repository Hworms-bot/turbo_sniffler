#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MCP23017.h>


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     13 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Declaration for MCP23017 GPIO expansion header.
#define MCP23017_ADDR 0x22    // I2C address is 0x22 
MCP23017 mcp = MCP23017(MCP23017_ADDR);


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
  
  mcp.init();
  mcp.portMode(MCP23017Port::A, 0b11111111);          //Port A as input
  mcp.portMode(MCP23017Port::B, 0); //Port B as output

  mcp.writeRegister(MCP23017Register::GPIO_A, 0x00);  //Reset port A 
  mcp.writeRegister(MCP23017Register::GPIO_B, 0x00);  //Reset port B

  // GPIO_B reflects the same logic as the input pins state
  mcp.writeRegister(MCP23017Register::IPOL_B, 0x00);
  // Uncomment this line to invert inputs (press a button to lit a led)
  //mcp.writeRegister(MCP23017Register::IPOL_B, 0xFF);
}

void loop() {
    uint8_t currentA;

    currentA = mcp.readPort(MCP23017Port::A);
    mcp.writePort(MCP23017Port::B, currentA);
}
