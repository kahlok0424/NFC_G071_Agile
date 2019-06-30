#ifndef _NFC_TEST_H
#define _NFC_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define NFC_UserMemory		0xA6
#define NFC_SystemMemory	0xAE
#define NFC_DynMemory		  0xA6
#define PRESENTPASS       0x09
#define CHANGEPASS        0x07

void unlockI2CSecurity(uint16_t devAddress, uint8_t *password, uint8_t *pass);
void lockI2CSecurity(uint16_t devAddress, uint8_t *WrongPass);

#endif // _NFC_TEST_H
