#include "Arduino.h"
#include "Keys.h"
#include <EEPROM.h>
#include <SPI.h>
#include <mcp2515.h>
//https://stackoverflow.com/questions/47981/how-do-i-set-clear-and-toggle-a-single-bit

uint8_t switchPosition = 0b00000000;
uint32_t canIdSystem = 0x81;// служебные сообщения
uint32_t canIdSwitch = 0x82;// канал о состоянии выключателей
struct can_frame canMsg;

MCP2515 mcp2515(10);

uint8_t oldKey1 = 1,oldKey2 = 1,key1,key2;
uint8_t switchArray[] ={2,3,4,5,6,7,8,9};
uint8_t pointId;



uint8_t getPointId(void)
{
	return EEPROM.read(0);
}



void setup()
{
	mcp2515.reset();
	mcp2515.setBitrate(CAN_125KBPS);
	mcp2515.setNormalMode();

	pointId = getPointId();

	for(uint8_t i : switchArray)
	{
		pinMode(i,INPUT_PULLUP);
	}
}


void sendMessage(uint8_t currentStatus)
{
	canMsg.can_id = canIdSwitch;
	canMsg.can_dlc = 2;
	canMsg.data[0] = pointId;
	canMsg.data[1] = currentStatus;
	mcp2515.sendMessage(&canMsg);
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
		sendMessage(switchPosition);
	}

	if (oldKey2 != key2){
		if (key2){
			switchPosition &= ~(1UL << 6);
		} else {
			switchPosition |= 1UL << 6;
		}
		oldKey2 = key2;
		sendMessage(switchPosition);
	}
delay(100);

//	mcp2515.sendMessage(&canMsgSwitch);
//	//canMsgSwitch.data[1] = rand();
//	canMsgSwitch.data[1] = switchPosition;
//	delay(1000);

}
