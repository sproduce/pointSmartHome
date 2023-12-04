#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2023-12-05 02:52:00

#include "Arduino.h"
#include <SPI.h>
#include <mcp2515.h>
#include "eepromUtils.h"

void clearCanChannel() ;
void addCanChannel(uint8_t button, uint16_t channel);
void sendCanChannel(uint8_t button);
void buttonsSetup();
void canInterrupt();
void canClear();
void canRead();
bool buttonRead(struct button *currentButton) ;
void setup() ;
void loop() ;

#include "pointSmartHome.ino"


#endif
