/*
 * ndef.c
 *
 *  Created on: 25 Jul 2019
 *      Author: KahLok
 */
#include "ndef.h"
#include "nfc.h"

/*
 * @brief Ndef area has to be multiple of 8bytes
 */
uint16_t writeT5TCCFile(ADDRESSING_MODE address_mode, uint16_t ndef_area){

	int count;
	uint8_t buffer[10];
	buffer[0] = address_mode;

	if( (ndef_area%8) != 0){
		return NDEF_ERROR;
	}
	else{
		buffer[2] = ndef_area/8;
	}
	buffer[1] = 0x40;
	buffer[3] = 0x01;
	writeUserMemory(0x00,buffer,4);
}
