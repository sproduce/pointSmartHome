#include "Arduino.h"
#include <SPI.h>
#include <mcp2515.h>
//https://stackoverflow.com/questions/47981/how-do-i-set-clear-and-toggle-a-single-bit

uint8_t switchPosition = 0b00000000;
struct can_frame canMsgSwitch;
struct can_frame canMsgSystem;
MCP2515 mcp2515(10);

uint8_t oldKey1 = 1,oldKey2 = 1,key1,key2;


void setup()
{

	canMsgSwitch.can_id = 0x0F6;
	canMsgSwitch.can_dlc = 2;
	canMsgSwitch.data[0] = 1;// current point for test. !!! read from EEPROM
	canMsgSwitch.data[1] = switchPosition;

	mcp2515.reset();
	mcp2515.setBitrate(CAN_125KBPS);
	mcp2515.setNormalMode();

	pinMode(2,INPUT_PULLUP);
	pinMode(3,INPUT_PULLUP);
}


void sentMessage(uint8_t currentStatus)
{
	canMsgSwitch.data[1] = currentStatus;
	mcp2515.sendMessage(&canMsgSwitch);
}


void loop()
{
	key1 = digitalRead(2);
	key2 = digitalRead(3);
	if (oldKey1 != key1){
		if (key1){
			switchPosition &= ~(1UL << 7);
		} else {
			switchPosition |= 1UL << 7;
		}
		oldKey1 = key1;
		sentMessage(switchPosition);
	}

	if (oldKey2 != key2){
		if (key2){
			switchPosition &= ~(1UL << 6);
		} else {
			switchPosition |= 1UL << 6;
		}
		oldKey2 = key2;
		sentMessage(switchPosition);
	}
delay(100);

//	mcp2515.sendMessage(&canMsgSwitch);
//	//canMsgSwitch.data[1] = rand();
//	canMsgSwitch.data[1] = switchPosition;
//	delay(1000);

}
