#include "Arduino.h"
#include <SPI.h>
#include <mcp2515.h>
#include "eepromUtils.h"
//https://stackoverflow.com/questions/47981/how-do-i-set-clear-and-toggle-a-single-bit


#define VERSION 1
#define COUNT_BUTTONS 8
#define COUNT_BUTTON_CHANNELS 8

#if VERSION == 1

	#define BUTTON_1 6 //d6
	#define BUTTON_2 7 //d7
	#define BUTTON_3 8 //d8
	#define BUTTON_4 10 //d10
	#define BUTTON_5 14 //a0
	#define BUTTON_6 15 //a1
	#define BUTTON_7 16 //a2
	#define BUTTON_8 17 //a3
	#define SPI_CS 3
#endif

#if VERSION == 2

	#define BUTTON_1 5 //d5
	#define BUTTON_2 6 //d6
	#define BUTTON_3 7 //d7
	#define BUTTON_4 8 //d8
	#define BUTTON_5 9 //d9
	#define BUTTON_6 15 //a1
	#define BUTTON_7 16 //a2
	#define BUTTON_8 17 //a3
	#define SPI_CS 10
#endif



volatile bool canReceived = false;
MCP2515 mcp2515(SPI_CS);
can_frame canData;

struct button {
	uint8_t channel = 0;
	uint8_t status = false;
	uint32_t changeTime = 0;
} buttons[COUNT_BUTTONS];


uint16_t canChannel[COUNT_BUTTONS][COUNT_BUTTON_CHANNELS];
uint16_t newChannel = 0;



void clearCanChannel()
{
	for(uint8_t i = 0;i < COUNT_BUTTONS; i++){
		for(uint8_t j = 0; j < COUNT_BUTTON_CHANNELS; j++){
			canChannel[i][j] = 0;
		}
	}
}


void addCanChannel(uint8_t button, uint16_t channel){
	for (uint8_t i = COUNT_BUTTON_CHANNELS -1; i >0; i--){
		canChannel[button][i] = canChannel[button][i-1];
	}
	canChannel[button][0] = channel;
}



void sendCanChannel(uint8_t button){
	for (uint8_t i = 0; i < COUNT_BUTTON_CHANNELS; i++){
		//Serial.println(canChannel[button][i]);
		if (canChannel[button][i]){
			canData.can_id = canChannel[button][i];
			canData.can_dlc = 1;
			canData.data[0] = 0b01;
			mcp2515.sendMessage(&canData);
			delay(2);
		}
	}
}




void buttonsSetup(){
	buttons[0].channel = BUTTON_1;
	buttons[1].channel = BUTTON_2;
	buttons[2].channel = BUTTON_3;
	buttons[3].channel = BUTTON_4;
	buttons[4].channel = BUTTON_5;
	buttons[5].channel = BUTTON_6;
	buttons[6].channel = BUTTON_7;
	buttons[7].channel = BUTTON_8;
	for(uint8_t i = 0; i < COUNT_BUTTONS; i++){
		pinMode(buttons[i].channel , INPUT_PULLUP);
		buttons[i].status = !digitalRead(buttons[i].channel);
	}
}

void canInterrupt(){
	canReceived = true;
}


void canClear(){
	mcp2515.clearRXnOVR();
	mcp2515.clearMERR();
	mcp2515.clearInterrupts();
}



void canRead(){
	while (mcp2515.readMessage(&canData) == MCP2515::ERROR_OK) {
		switch(canData.can_id) {
		case 0x777 : clearCanChannel();
			//Serial.println("clearEeprom");
			break;
		case 0x707 : newChannel = 0;
			saveEeprom(canChannel);
			//Serial.println("saveEeprom");
			break;
		case 0x700 :
			//Serial.println(canData.data[0]);
			newChannel = canData.data[0];
			break;
		}
	}
}




bool buttonRead(struct button * currentButton) {

uint8_t curretnStatus = !digitalRead(currentButton->channel);

	if (curretnStatus != currentButton->status){
		if (!currentButton->changeTime ) {
			currentButton->changeTime = millis();
		}
		if (millis() - currentButton->changeTime > 100){
			currentButton->changeTime = 0;
			currentButton->status = curretnStatus;
			return true;
		}
	} else {
		if (currentButton->changeTime > 110){
			currentButton->changeTime = 0;
		}
	}


	return false;
}


void setup()
{
	mcp2515.reset();
	mcp2515.setBitrate(CAN_50KBPS);
	mcp2515.setConfigMode();
	mcp2515.setFilterMask(MCP2515::MASK0, false, 0x400);
	mcp2515.setFilter(MCP2515::RXF0, false, 0x400);
	mcp2515.setFilterMask(MCP2515::MASK1, false, 0x7ff);
	mcp2515.setNormalMode();
	//Serial.begin(9600);
	pinMode(2, INPUT_PULLUP);
	attachInterrupt(0, canInterrupt, FALLING);
	buttonsSetup();
	readEeprom(canChannel);
}


void loop()
{

	if (canReceived){
		canReceived = false;
		canRead();
	}

	for(uint8_t i = 0; i < COUNT_BUTTONS; i++){
		if (buttonRead(&buttons[i])){

			if (newChannel){
				//Serial.println("button setup");
				addCanChannel(i,newChannel);
			} else {
				sendCanChannel(i);
			}
		}
	}





}
