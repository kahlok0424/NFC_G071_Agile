/*
 * ndef.h
 *
 *  Created on: 25 Jul 2019
 *      Author: KahLok
 */

#ifndef NDEF_H_
#define NDEF_H_

#include "stdint.h"

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

typedef struct
{
  char protocol[80];
  char URI_Message[400];
  char Information[400];
}sURI_Info;

//Capability container version 1.0 */
#define NFCT5_VERSION_V1_0		0x40

//define for NDEF status
#define NDEF_OK			0x00
#define NDEF_ERROR		0x01

//defines for other
#define FULL_MEMORY		512

//Function declare
uint16_t writeT5TCCFile(ADDRESSING_MODE address_mode, uint16_t ndef_area);

#endif /* NDEF_H_ */
