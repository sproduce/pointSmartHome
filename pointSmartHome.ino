#include "Arduino.h"
#include "Keys.h"
#include <EEPROM.h>
#include <SPI.h>
#include <mcp2515.h>
//https://stackoverflow.com/questions/47981/how-do-i-set-clear-and-toggle-a-single-bit

#define COUNT_BUTTON 8



struct can_frame canMsg;
volatile bool canReceived = false;



struct button {
	uint8_t channel = 0;
	uint8_t status = false;
	uint32_t changeTime = 0;
} buttons[COUNT_BUTTON];


MCP2515 mcp2515(10);


void buttonsInit(){

	for(uint8_t i = 0; i < COUNT_BUTTON; i++){
		if (buttons[i].channel){
			pinMode(buttons[i].channel , INPUT_PULLUP);
		}
	}
}




void setup()
{
	mcp2515.reset();
	mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ);
	mcp2515.setNormalMode();
	Serial.begin(9600);
}


void loop()
{



}
