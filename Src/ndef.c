/*
 * ndef.c
 *
 *  Created on: 25 Jul 2019
 *      Author: KahLok
 */
#include "ndef.h"
#include "nfc.h"


void writeCCFile(ADDRESSING_MODE address_mode){

	int count;
	uint8_t buffer[10];
	writeUserMemory(0x00,buffer,count);
}
