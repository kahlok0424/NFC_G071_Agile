#include "NFC_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void unlockI2CSecurity(uint16_t devAddress, uint8_t *password, uint8_t *pass){

	//uint8_t pass[18];
	//allocate the pass into buffer
	/*for(int j = 0; j<2; j++){
		for( i = 0; i < 8; i++){
			pass[i] = password[i];
		}
		pass[8] = PRESENTPASS;
	}*/
	for(int i = 0; i < 8; i++){
		pass[i] = password[i];
	}
	pass[8] = PRESENTPASS;
	for(int i = 0; i < 8; i++){
		pass[i+9] = password[i];
	}
	return pass;
	//HAL_I2C_Mem_Write(hi2c,devAddress, I2C_PWD, Itl, pass,17,50); //present  i2c password
	//HAL_Delay(10);
}
