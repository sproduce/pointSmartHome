#include "Arduino.h"
#include <SPI.h>
#include <mcp2515.h>


uint8_t switchPosition = 0b01110011;
struct can_frame canMsgSwitch;
struct can_frame canMsgSystem;
MCP2515 mcp2515(10);


void setup()
{

	canMsgSwitch.can_id = 0x0F6;
	canMsgSwitch.can_dlc = 2;
	canMsgSwitch.data[0] = 1;// current point for test. !!! read from EEPROM
	canMsgSwitch.data[1] = switchPosition;

	mcp2515.reset();
	mcp2515.setBitrate(CAN_125KBPS);
	mcp2515.setNormalMode();
}


void loop()
{
	mcp2515.sendMessage(&canMsgSwitch);
	canMsgSwitch.data[1] = rand();
	delay(1000);

}
