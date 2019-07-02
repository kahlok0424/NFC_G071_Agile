#include "NFC_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void unlockI2CSecurity(uint8_t *password, uint8_t *pass){

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
}

void lockI2CSecurity(uint8_t *WrongPass){

	//uint8_t WrongPass[17];

	for(int i = 0; i < 8; i++){
		WrongPass[i] = i+1;
	}
	WrongPass[8] = PRESENTPASS;
	for(int i = 0; i < 8; i++){
		WrongPass[i+9] = i+2;
	}
}

void changeI2CSecurity(uint8_t *oldPass, uint8_t *newPass, uint8_t *ans){

	//uint8_t WrongPass[17];

	for(int i = 0; i < 8; i++){
		ans[i] = newPass[i];
	}
	ans[8] = CHANGEPASS;
	for(int i = 0; i < 8; i++){
		ans[i+9] = newPass[i];
	}
}
