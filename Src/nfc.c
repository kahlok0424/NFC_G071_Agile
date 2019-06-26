/*
 * nfc.c
 *
 *  Created on: Jun 25, 2019
 *      Author: KahLok
 */

#include "nfc.h"
#include "stm32g0xx_hal.h"

void initNFC(I2C_HandleTypeDef *hi2c, uint16_t devAddress){

	if(HAL_I2C_IsDeviceReady(hi2c, devAddress,3,50) == HAL_OK){
		HAL_GPIO_WritePin(NFC_LED1_PORT,NFC_LED1,1);
		HAL_GPIO_WritePin(NFC_LED2_PORT,NFC_LED2,1);
		//HAL_GPIO_WritePin(NFC_LED3_PORT,NFC_LED3,1);
	}
	else{
		HAL_GPIO_WritePin(NFC_LED1_PORT,NFC_LED1,1);
	}
}


void currentAddRead(I2C_HandleTypeDef *hi2c, uint16_t devAddress, char *buffer,int n){

	HAL_I2C_Master_Receive(hi2c, devAddress, buffer,n ,50);
}

void presentI2Cpassword(I2C_HandleTypeDef *hi2c, uint16_t devAddress, int password){
	char *temp = password;
	char pass[17];
	//allocate the password into buffer
	for(int i = 0; i < 8; i++){
		pass[i] = temp[i];
	}
	pass[8] = presentPass;

	for(int i = 0; i < 8; i++){
		pass[i+8] = temp;
		temp++;
	}

	//HAL_I2C_Mem_Write(hi2c,NFC_SystemMemory, I2C_PWD, Itl, pass,17,50); //present  i2c password

}

