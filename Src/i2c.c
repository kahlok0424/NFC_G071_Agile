/*
 * I2C.c
 *
 *  Created on: Jul 4, 2019
 *      Author: user
 */

#include "main.h"
#include "i2c.h"

void I2CRead(uint16_t devAddress,uint16_t memAddress, uint8_t *data, uint16_t n){
	HAL_I2C_Mem_Read(&hi2c1, devAddress, memAddress, 2, data, n, 50);
	HAL_Delay(10);
}

void I2CWrite(uint16_t devAddress,uint16_t memAddress, uint8_t *data, uint16_t n){
	HAL_I2C_Mem_Write(&hi2c1, devAddress, memAddress, 2, data, n, 50);
	HAL_Delay(10);
}

void NFC_Delay(uint32_t delay){
	HAL_Delay(delay);
}


int deviceReady(uint16_t devAddress){
	if(HAL_I2C_IsDeviceReady(&hi2c1, devAddress,3,50) == HAL_OK){
		return 1;
	}
	else{
		return 0;
	}
}

/*void gpio_WritePin(int port, int led, int value){

	HAL_GPIO_WritePin(port,led,value);
}*/

void initNFC(uint16_t devAddress){

	if(deviceReady(devAddress) == 1){
		HAL_GPIO_WritePin(NFC_LED1_GPIO_Port,NFC_LED1_Pin,1);
		HAL_GPIO_WritePin(NFC_LED2_GPIO_Port,NFC_LED2_Pin,1);
		HAL_GPIO_WritePin(NFC_LED3_GPIO_Port,NFC_LED3_Pin,1);
	}
	else{
		HAL_GPIO_WritePin(NFC_LED1_GPIO_Port,NFC_LED1_Pin,0);
		HAL_GPIO_WritePin(NFC_LED2_GPIO_Port,NFC_LED2_Pin,0);
		HAL_GPIO_WritePin(NFC_LED3_GPIO_Port,NFC_LED3_Pin,0);
	}
}
