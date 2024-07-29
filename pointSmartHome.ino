#include <SPI.h>
#include <mcp2515.h>


//https://stackoverflow.com/questions/47981/how-do-i-set-clear-and-toggle-a-single-bit

#include "buttonsConfig.h"


volatile bool canReceived = false;
MCP2515 mcp2515(SPI_CS);
can_frame canData;
_button buttons[COUNT_BUTTONS];

uint16_t newChannel = 0;
uint8_t startProgram = 1;



void clearCanChannel()
{
	for(uint8_t i = 0;i < COUNT_BUTTONS; i++){
		for(uint8_t j = 0; j < COUNT_BUTTON_CHANNELS; j++){
			buttons[i].canChannel[j] = 0;
		}
		buttons[i].type = 0;
	}
}


void addCanChannel(_button *currentButton, uint16_t channel)
{
	uint8_t i;
	for(i=0; i<COUNT_BUTTON_CHANNELS; i++){
		if(channel == currentButton->canChannel[i]){
			if (currentButton->lastDuration < 400){
				currentButton->type = 2; //set key type
			} else {
				currentButton->type = 3;//set switch emulator key
			}
			return;
		}
	}

	for (i = COUNT_BUTTON_CHANNELS -1; i >0; i--){
		currentButton->canChannel[i] = currentButton->canChannel[i-1];
	}
	currentButton->type = 1; // set
	currentButton->canChannel[0] = channel;
}


void sendCanChannel(_button *currentButton){
	for (uint8_t i = 0; i < COUNT_BUTTON_CHANNELS; i++){
		if (currentButton->canChannel[i]){
			canData.can_id = currentButton->canChannel[i];
			canData.can_dlc = 1;
			switch (currentButton->type)
			{
				case 1:
					canData.data[0] = currentButton->status;
				break;
				case 2:
					canData.data[0] = 2;
				break;
				case 3:
					canData.data[0] = 2;
				break;
			}
			mcp2515.sendMessage(&canData);
			delay(1);
		} else {
			break;
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

	for(uint8_t i = 0; i < COUNT_BUTTONS; i++){
		pinMode(buttons[i].channel , INPUT_PULLUP);
		buttons[i].status = !digitalRead(buttons[i].channel);
	}
	readEeprom(buttons);
}


void canInterrupt(){
	canReceived = true;
}


//void canClear(){
//	mcp2515.clearRXnOVR();
//	mcp2515.clearMERR();
//	mcp2515.clearInterrupts();
//}



void canRead(){
	while (mcp2515.readMessage(&canData) == MCP2515::ERROR_OK) {
		switch(canData.can_id) {
		case 0x777 :
			clearEeprom();
			clearCanChannel();
			//Serial.println("clearEeprom");
			break;
		case 0x707 : newChannel = 0;
			startProgram = 1;
			saveEeprom(buttons);
			//Serial.println("saveEeprom");
			break;
		case 0x700 :
			//Serial.println(canData.data[0]);
			newChannel = canData.data[0];
			break;
		}
	}
}




bool buttonRead(_button *currentButton) {

	uint8_t curretnStatus = !digitalRead(currentButton->channel);

	if (curretnStatus != currentButton->status){
		if (!currentButton->changeTime ) {
			currentButton->changeTime = millis();
		}
		if (millis() - currentButton->changeTime > 50){
			currentButton->changeTime = 0;
			currentButton->status = curretnStatus;
			currentButton->lastDuration = millis() - currentButton->startTime;
			currentButton->startTime = millis();
			return true;
		}
	} else {
		currentButton->changeTime = 0;
	}

	return false;
}


void setup()
{
	mcp2515.reset();
	mcp2515.setBitrate(CAN_125KBPS,MCP_8MHZ);
	mcp2515.setConfigMode();
	mcp2515.setFilterMask(MCP2515::MASK0, false, 0x400);
	mcp2515.setFilter(MCP2515::RXF0, false, 0x400);
	mcp2515.setFilterMask(MCP2515::MASK1, false, 0x7ff);
	mcp2515.setNormalMode();
	pinMode(2, INPUT_PULLUP);
	attachInterrupt(0, canInterrupt, FALLING);
	buttonsSetup();
}


void loop()
{

	if (canReceived){
		canReceived = false;
		canRead();
		mcp2515.clearInterrupts();
	}

	for(uint8_t i = 0; i < COUNT_BUTTONS; i++){
		if (buttonRead(&buttons[i])){
			if (newChannel){
				if (startProgram){
					clearCanChannel();
					startProgram = 0;
				}
				addCanChannel(&buttons[i],newChannel);
			} else {
				sendCanChannel(&buttons[i]);
			}
		}
	}

}
