/*
 * ndef.c
 *
 *  Created on: 25 Jul 2019
 *      Author: KahLok
 */
#include "ndef.h"
#include "nfc.h"


void writeT5TCCFile(ADDRESSING_MODE address_mode){

	int count;
	uint8_t buffer[10];
	buffer[0] = address_mode;
	buffer[1] = 0x45;
	buffer[2] = 0x40;
	buffer[3] = 0x01;
	writeUserMemory(0x00,buffer,4);
}
