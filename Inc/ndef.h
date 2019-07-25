/*
 * ndef.h
 *
 *  Created on: 25 Jul 2019
 *      Author: KahLok
 */

#ifndef NDEF_H_
#define NDEF_H_

typedef enum addressing_mode{
	 ONE_BYTE_ADDRESSING 		= 0xe1,
	 TWO_BYTE_ADDRESSING		= 0xe2,
}ADDRESSING_MODE;

//Function declare
void writeT5TCCFile(ADDRESSING_MODE address_mode);

#endif /* NDEF_H_ */
