/*
 * ndef.h
 *
 *  Created on: 25 Jul 2019
 *      Author: KahLok
 */

#ifndef NDEF_H_
#define NDEF_H_

typedef enum
{
  T5T_NO_NDEF = 0,  /**< No data detected in the tag. */
  T5T_INITIALIZED,  /**< Capability container detected. */
  T5T_READ_WRITE,   /**< Read-Write data detected. */
  T5T_READ          /**< Read-Only data message detected. */
} NDEF_T5T_State;

typedef enum addressing_mode{
	 ONE_BYTE_ADDRESSING 		= 0xe1,
	 TWO_BYTE_ADDRESSING		= 0xe2,
}ADDRESSING_MODE;

//define for NDEF status
#define NDEF_OK			0x00
#define NDEF_ERROR		0x01

//Function declare
void writeT5TCCFile(ADDRESSING_MODE address_mode);

#endif /* NDEF_H_ */
