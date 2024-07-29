#ifndef BUTTONSCONFIG_H_
#define BUTTONSCONFIG_H_


#include <EEPROM.h>


#define VERSION 1
#define COUNT_BUTTONS 6
#define COUNT_BUTTON_CHANNELS 8


#if VERSION == 1
	#define BUTTON_1 6 //d6
	#define BUTTON_2 7 //d7
	#define BUTTON_3 8 //d8
	#define BUTTON_4 14 //a0
	#define BUTTON_5 15 //a1
	#define BUTTON_6 16 //a2
	#define SPI_CS 3
#endif

#if VERSION == 2
	#define BUTTON_1 7 //d7
	#define BUTTON_2 8 //d8
	#define BUTTON_3 9 //d9
	#define BUTTON_4 15 //a1
	#define BUTTON_5 16 //a2
	#define BUTTON_6 17 //a3
	#define SPI_CS 10
#endif


struct _button {
	uint8_t channel;// arduino pin number
	uint16_t canChannel[COUNT_BUTTON_CHANNELS];
	uint8_t status;
	uint8_t type;
	uint32_t changeTime;
	uint32_t startTime;
	uint32_t lastDuration;

	_button()
	{
		status = 0;
		type = 0;// key = 1 switch = 2 keyLikeSwitch = 3
		changeTime = 0;
		startTime = 0;
		lastDuration = 0;
	}

};


void clearEeprom(){
	for (uint16_t i = 0 ; i < EEPROM.length() ; i++) {
	    EEPROM.write(i, 0);
	  }
}




void readEeprom(_button *buttons)
{
	uint8_t saveByte = 1;
	for (uint8_t i = 0;i<COUNT_BUTTONS;i++){
		for(uint8_t j = 0;j<COUNT_BUTTON_CHANNELS;j++){
			EEPROM.get(saveByte,buttons[i].canChannel[j]);
			saveByte += 2;
		}

		EEPROM.get(saveByte,buttons[i].type);
		saveByte += 1;
	}

}



void saveEeprom(_button *buttons)
{
	uint8_t saveByte = 1;
	for (uint8_t i = 0;i<COUNT_BUTTONS;i++){
		for(uint8_t j = 0;j<COUNT_BUTTON_CHANNELS;j++){
			EEPROM.put(saveByte,buttons[i].canChannel[j]);
			saveByte +=2;
		}
		EEPROM.put(saveByte,buttons[i].type);
		saveByte +=1;
	}

}

#endif
