/*
 * eepromUtils.h
 *
 *  Created on: 7 нояб. 2023 г.
 *      Author: volodia
 */

#ifndef EEPROMUTILS_H_
#define EEPROMUTILS_H_
#include <EEPROM.h>

void readEeprom(const uint16_t (&channels)[8][8])
{
	uint8_t saveByte = 1;
		for (uint8_t i = 0;i<8;i++){
			for(uint8_t j = 0;j<8;j++){
				EEPROM.get(saveByte, channels[i][j]);
				saveByte +=2;
			}
		}

}



void saveEeprom(const uint16_t (&channels)[8][8])
{
	uint8_t saveByte = 1;
	for (uint8_t i = 0;i<8;i++){
		for(uint8_t j = 0;j<8;j++){
			EEPROM.put(saveByte, channels[i][j]);
			saveByte +=2;
		}
	}

}


#endif /* EEPROMUTILS_H_ */
