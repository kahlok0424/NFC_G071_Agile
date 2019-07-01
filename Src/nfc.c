/*
 * nfc.c
 *
 *  Created on: Jun 25, 2019
 *      Author: KahLok
 */

#include "nfc.h"
#include "main.h"
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

void I2CRead(uint16_t devAddress,uint16_t memAddress, uint8_t *data, int n){
	HAL_I2C_Mem_Read(&hi2c1, devAddress, memAddress, 2, data, n, 50);
}

void I2CWrite(uint16_t devAddress,uint16_t memAddress, uint8_t *data, int n){
	HAL_I2C_Mem_Write(&hi2c1, devAddress, memAddress, 2, data, n, 50);
}

void currentAddRead(I2C_HandleTypeDef *hi2c, uint16_t devAddress,uint8_t *buffer,int n){

	HAL_I2C_Master_Receive(hi2c, devAddress, buffer,n ,50);
}

/**
 * @description present the i2c password and unlock i2c security session
 * @param device address
 * @param password
 */
void unlockI2CSecurity(uint8_t *password){

	uint8_t pass[17];
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

	I2CWrite(NFC_SYSTEMMEMORY, I2C_PWD, pass,17); //present i2c password
	//HAL_Delay(10);
}

/**
 * @description lock the i2c security session by presenting a wrong password
 * @param device address
 */
void lockI2CSecurity(){

	uint8_t WrongPass[17];

	for(int i = 0; i < 8; i++){
		WrongPass[i] = i+1;
	}
	WrongPass[8] = PRESENTPASS;
	for(int i = 0; i < 8; i++){
		WrongPass[i+9] = i+2;
	}

	I2CWrite(NFC_SYSTEMMEMORY, I2C_PWD, WrongPass,17); //present wrong i2c password
	HAL_Delay(10);
}

/**
 * @description present the old i2c password and write a new password
 * @param device address
 * @param old password
 * @param new password
 */
void changeI2CPassword(uint8_t *oldpass, uint8_t *newPassword){

	unlockI2CSecurity(oldpass); //unlock i2c
	I2CWrite(NFC_SYSTEMMEMORY, I2C_PWD, newPassword,17); //write new password
	lockI2CSecurity();
	HAL_Delay(10);
}

void readI2CPassword(uint8_t *password, uint8_t *ans){
	unlockI2CSecurity(password); //unlock i2c
	I2CRead(NFC_SYSTEMMEMORY, I2C_PWD, ans, 8);
	lockI2CSecurity();
}

/**
 * @description read the value of register into the buffer
 * @param device address
 * @param register address
 * @param buffer
 * @param number of bytes to read
 */
void readSystemMemory(uint16_t regAddress, uint8_t *buffer,int n){

	//presentI2Cpassword(hi2c, NFC_SystemMemory, 0x00);
	I2CRead(NFC_SYSTEMMEMORY, regAddress, buffer, n);
	//lockI2CSecurity(hi2c, NFC_SystemMemory);
}

/**
 * @description write data into the system static register
 * @param device address
 * @param data to write
 */
void writeSystemMemory(uint16_t regAddress, uint8_t *password, uint8_t data){

	uint8_t buffer[1];
	buffer[0] = data;

	unlockI2CSecurity(password);
	I2CWrite(NFC_SYSTEMMEMORY, regAddress, buffer, 1);
	lockI2CSecurity();
}

/**
 * @description write data into the user memory
 * @param area of user memory (1 ~ 4)
 * @param address to write
 * @param pointer to data to write
 * @param number of bytes to write
 */
void writeUserMemory(int area, uint16_t address, uint8_t *data, int n){

	if(area == 1){
		I2CWrite(NFC_USERMEMORY, address, data, n);
	}
	else{
		//waiting to implement
	}
	HAL_Delay(10);
}

/**
 * @description read data of user memory given address
 * @param area of user memory (1 ~ 4)
 * @param address to write
 * @param pointer to data to read
 * @param number of bytes to read
 */
void readUserMemory(int area, uint16_t address, uint8_t *data, int n){

	if(area == 1){
		I2CRead(NFC_USERMEMORY, address, data, n);
	}
	else{
		//waiting to implement
	}
	HAL_Delay(10);
}

void NFC04A1_setRFMode(uint8_t *password, uint8_t mode){
	//unlockI2CSecurity(NFC_SystemMemory,0x00);
	writeSystemMemory(RF_MNGT, password, mode);
	//lockI2CSecurity(NFC_SystemMemory);
}

void NFC04A1_setRFModeDyn(uint8_t *password, uint8_t mode){
	//unlockI2CSecurity(NFC_SystemMemory,0x00);
	writeSystemMemory(RF_MNGT_DYN, password, mode);
	//lockI2CSecurity(NFC_SystemMemory);
}
