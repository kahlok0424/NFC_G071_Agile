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
}URI_Info;

#define SMART_POSTER_TYPE               "Sp"
#define SMART_POSTER_TYPE_LENGTH         2

#define URI_TYPE                        "U"
#define URI_TYPE_LENGTH                  1

#define URI_NONE                 0x00
#define URI_0x01                 0x01
#define URI_0x02                 0x02
#define URI_0x03                 0x03
#define URI_0x04                 0x04
#define URI_0x05                 0x05
#define URI_0x06                 0x06
#define URI_0x07                 0x07
#define URI_0x08                 0x08
#define URI_0x09                 0x09
#define URI_0x0A                 0x0A
#define URI_0x0B                 0x0B
#define URI_0x0C                 0x0C
#define URI_0x0D                 0x0D
#define URI_0x0E                 0x0E
#define URI_0x0F                 0x0F
#define URI_0x10                 0x10
#define URI_0x11                 0x11
#define URI_0x12                 0x12
#define URI_0x13                 0x13
#define URI_0x14                 0x14
#define URI_0x15                 0x15
#define URI_0x16                 0x16
#define URI_0x17                 0x17
#define URI_0x18                 0x18
#define URI_0x19                 0x19
#define URI_0x1A                 0x1A
#define URI_0x1B                 0x1B
#define URI_0x1C                 0x1C
#define URI_0x1D                 0x1D
#define URI_0x1E                 0x1E
#define URI_0x1F                 0x1F
#define URI_0x20                 0x20
#define URI_0x21                 0x21
#define URI_0x22                 0x22
#define URI_0x23                 0x23
#define URI_ERROR                0xff
#define URI_RFU                  0x24

#define URI_0x01_STRING          "http://www.\0"
#define URI_0x02_STRING          "https://www.\0"
#define URI_0x03_STRING          "http://\0"
#define URI_0x04_STRING          "https://\0"
#define URI_0x05_STRING          "tel:\0"
#define URI_0x06_STRING          "mailto:\0"
#define URI_0x07_STRING          "ftp://anonymous:anonymous@\0"
#define URI_0x08_STRING          "ftp://ftp.\0"
#define URI_0x09_STRING          "ftps://\0"
#define URI_0x0A_STRING          "sftp://\0"
#define URI_0x0B_STRING          "smb://\0"
#define URI_0x0C_STRING          "nfs://\0"
#define URI_0x0D_STRING          "ftp://\0"
#define URI_0x0E_STRING          "dav://\0"
#define URI_0x0F_STRING          "news:\0"
#define URI_0x10_STRING          "telnet://\0"
#define URI_0x11_STRING          "imap:\0"
#define URI_0x12_STRING          "rtsp://\0"
#define URI_0x13_STRING          "urn:\0"
#define URI_0x14_STRING          "pop:\0"
#define URI_0x15_STRING          "sip:\0"
#define URI_0x16_STRING          "sips:\0"
#define URI_0x17_STRING          "tftp:\0"
#define URI_0x18_STRING          "btspp://\0"
#define URI_0x19_STRING          "btl2cap://\0"
#define URI_0x1A_STRING          "btgoep://\0"
#define URI_0x1B_STRING          "tcpobex://\0"
#define URI_0x1C_STRING          "irdaobex://\0"
#define URI_0x1D_STRING          "file://\0"
#define URI_0x1E_STRING          "urn:epc:id:\0"
#define URI_0x1F_STRING          "urn:epc:tag\0"
#define URI_0x20_STRING          "urn:epc:pat:\0"
#define URI_0x21_STRING          "urn:epc:raw:\0"
#define URI_0x22_STRING          "urn:epc:\0"
#define URI_0x23_STRING          "urn:nfc:\0"

//Capability container version 1.0 */
#define NFCT5_VERSION_V1_0		0x40

//define for NDEF status
#define NDEF_OK			0x00
#define NDEF_ERROR		0x01

//defines for other
#define FULL_MEMORY		512

//Function declare
uint16_t writeT5TCCFile(ADDRESSING_MODE address_mode, uint16_t ndef_area);
uint16_t getURIProtocol(char *protocol);
uint8_t *writeURI(char *protocol, char *link, char *infomation);

#endif /* NDEF_H_ */
