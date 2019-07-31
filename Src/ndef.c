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

	uint8_t buffer[4];
	buffer[0] = address_mode;

	if( (ndef_area%8) != 0 || ndef_area > 512){
		return NDEF_ERROR;
	}
	else{
		buffer[2] = ndef_area/8;
		//byte 1 = NFC forum tag type V version , v 1.0
		buffer[1] = NFCT5_VERSION_V1_0;
		//byte 2 = additional features, Multiple block read command = 0x01
		buffer[3] = 0x01;
		writeUserMemory(0x00,buffer,4);
		return NDEF_OK;
	}

}

uint16_t writeURI(){

}
