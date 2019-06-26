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
	}
	else{
		HAL_GPIO_WritePin(NFC_LED1_PORT,NFC_LED1,1);
	}
}

void currentAddRead(I2C_HandleTypeDef *hi2c, uint16_t devAddress,uint8_t *buffer,int n){

	HAL_I2C_Master_Receive(hi2c, devAddress, buffer,n ,50);
}

void presentI2Cpassword(I2C_HandleTypeDef *hi2c, uint16_t devAddress, int password){
	//char *temp;
	//*temp = password;
	uint8_t pass[17];
	  pass[0] = 0x00;
	  pass[1] = 0x00;
	  pass[2] = 0x00;
	  pass[3] = 0x00;
	  pass[4] = 0x00;
	  pass[5] = 0x00;
	  pass[6] = 0x00;
	  pass[7] = 0x00;
	  pass[8] = 0x09;
	  pass[9] = 0x00;
	  pass[10] = 0x00;
	  pass[11] = 0x00;
	  pass[12] = 0x00;
	  pass[13] = 0x00;
	  pass[14] = 0x00;
	  pass[15] = 0x00;
	  pass[16] = 0x00;
	//allocate the pass into buffer
	/*for(int i = 0; i < 8; i++){
		pass[i] = *temp;
		//temp++;
	}
	pass[8] = presentPass;

	for(int i = 0; i < 8; i++){
		pass[i+8] = *temp;
		//temp++;
	}*/
	HAL_I2C_Mem_Write(hi2c,devAddress, I2C_PWD, Itl, pass,17,50); //present  i2c password
	HAL_Delay(10);
}

void lockI2CSecurity(I2C_HandleTypeDef *hi2c, uint16_t devAddress){

	uint8_t WrongPass[17];
	WrongPass[0] = 0x01;
	WrongPass[1] = 0x02;
	WrongPass[2] = 0x03;
	WrongPass[3] = 0x04;
	WrongPass[4] = 0x00;
	WrongPass[5] = 0x00;
	WrongPass[6] = 0x00;
	WrongPass[7] = 0x00;
	WrongPass[8] = 0x09;
	WrongPass[9] = 0x00;
	WrongPass[10] = 0x04;
	WrongPass[11] = 0x05;
	WrongPass[12] = 0x06;
	WrongPass[13] = 0x07;
	WrongPass[14] = 0x00;
	WrongPass[15] = 0x00;
	WrongPass[16] = 0x00;

	HAL_I2C_Mem_Write(hi2c,devAddress, I2C_PWD, Itl, WrongPass,17,50); //present wrong i2c password
	HAL_Delay(10);
}

void readReg(I2C_HandleTypeDef *hi2c, uint16_t devAddress, uint16_t regAddress, uint8_t *buffer,int n){

	//presentI2Cpassword(hi2c, NFC_SystemMemory, 0x00);
	HAL_I2C_Mem_Read(hi2c, devAddress, regAddress, Itl, buffer, n, 50);
	//lockI2CSecurity(hi2c, NFC_SystemMemory);
}

void configSystemReg(I2C_HandleTypeDef *hi2c, uint16_t regAddress, uint8_t data){

	uint8_t buffer[1];
	buffer[0] = data;

	presentI2Cpassword(hi2c, NFC_SystemMemory, 0x00);
	HAL_I2C_Mem_Write(hi2c, NFC_SystemMemory, regAddress, Itl, buffer, 1, 50);
	lockI2CSecurity(hi2c, NFC_SystemMemory);
}

void writeUserMemory(I2C_HandleTypeDef *hi2c, int area, uint16_t address, uint8_t *data, int n){

	if(area == 1){
		HAL_I2C_Mem_Write(hi2c, NFC_UserMemory, address, Itl, data, n, 50);
	}
	else{
		//waiting to implement
	}
	HAL_Delay(10);
}

void readUserMemory(I2C_HandleTypeDef *hi2c, int area, uint16_t address, uint8_t *data, int n){

	if(area == 1){
		HAL_I2C_Mem_Read(hi2c, NFC_UserMemory, address, Itl, data, n, 50);
	}
	else{
		//waiting to implement
	}
	HAL_Delay(10);
}
