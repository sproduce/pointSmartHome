#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2024-04-22 23:13:01

#include "Arduino.h"
#include <SPI.h>
#include <mcp2515.h>
#include "buttonsConfig.h"

void clearCanChannel() ;
void addCanChannel(_button *currentButton, uint16_t channel) ;
void sendCanChannel(_button *currentButton);
void buttonsSetup();
void canInterrupt();
void canRead();
bool buttonRead(_button *currentButton) ;
void setup() ;
void loop() ;

#include "pointSmartHome.ino"


#endif
