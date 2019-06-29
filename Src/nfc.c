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

/**
 * @description present the i2c password and unlock i2c security session
 * @param i2c typedef
 * @param device address
 * @param password
 */
void unlockI2CSecurity(I2C_HandleTypeDef *hi2c, uint16_t devAddress, uint8_t *password){

	uint8_t pass[18];
	//allocate the pass into buffer
	/*for(int j = 0; j<2; j++){
		for( i = 0; i < 8; i++){
			pass[i] = password[i];
		}
		pass[8] = PRESENTPASS;
	}*/
	for( i = 0; i < 8; i++){
		pass[i] = password[i];
	}
	pass[8] = PRESENTPASS;
	for(int i = 0; i < 8; i++){
		pass[i+9] = password[i];
	}
	HAL_I2C_Mem_Write(hi2c,devAddress, I2C_PWD, Itl, pass,17,50); //present  i2c password
	//HAL_Delay(10);
}

/**
 * @description lock the i2c security session by presenting a wrong password
 * @param i2c typedef
 * @param device address
 */
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

/**
 * @description present the old i2c password and write a new password
 * @param i2c typedef
 * @param device address
 * @param old password
 * @param new password
 */
void changeI2CPassword(I2C_HandleTypeDef *hi2c, uint16_t devAddress, uint8_t *newPassword){

	unlockI2CSecurity(hi2c, devAddress, 0x00); //unlock i2c
	HAL_I2C_Mem_Write(hi2c,devAddress, I2C_PWD, Itl, newPassword,17,50); //write new password
	lockI2CSecurity(hi2c,devAddress);
	HAL_Delay(10);
}

void readI2CPassword(I2C_HandleTypeDef *hi2c, uint16_t devAddress, uint8_t *password){
	unlockI2CSecurity(hi2c, devAddress, 0x00); //unlock i2c
	readReg(hi2c, devAddress, I2C_PWD, password, 8);
	lockI2CSecurity(hi2c, devAddress);
}

/**
 * @description read the value of register into the buffer
 * @param i2c typedef
 * @param device address
 * @param register address
 * @param buffer
 * @param number of bytes to read
 */
void readReg(I2C_HandleTypeDef *hi2c, uint16_t devAddress, uint16_t regAddress, uint8_t *buffer,int n){

	//presentI2Cpassword(hi2c, NFC_SystemMemory, 0x00);
	HAL_I2C_Mem_Read(hi2c, devAddress, regAddress, Itl, buffer, n, 50);
	//lockI2CSecurity(hi2c, NFC_SystemMemory);
}

/**
 * @description write data into the system static register
 * @param i2c typedef
 * @param device address
 * @param data to write
 */
void writeSystemReg(I2C_HandleTypeDef *hi2c, uint16_t regAddress, uint8_t data){

	uint8_t buffer[1];
	buffer[0] = data;

	unlockI2CSecurity(hi2c, NFC_SystemMemory, 0x00);
	HAL_I2C_Mem_Write(hi2c, NFC_SystemMemory, regAddress, Itl, buffer, 1, 50);
	lockI2CSecurity(hi2c, NFC_SystemMemory);
}

/**
 * @description write data into the user memory
 * @param i2c typedef
 * @param area of user memory (1 ~ 4)
 * @param address to write
 * @param pointer to data to write
 * @param number of bytes to write
 */
void writeUserMemory(I2C_HandleTypeDef *hi2c, int area, uint16_t address, uint8_t *data, int n){

	if(area == 1){
		HAL_I2C_Mem_Write(hi2c, NFC_UserMemory, address, Itl, data, n, 50);
	}
	else{
		//waiting to implement
	}
	HAL_Delay(10);
}

/**
 * @description read data of user memory given address
 * @param i2c typedef
 * @param area of user memory (1 ~ 4)
 * @param address to write
 * @param pointer to data to read
 * @param number of bytes to read
 */
void readUserMemory(I2C_HandleTypeDef *hi2c, int area, uint16_t address, uint8_t *data, int n){

	if(area == 1){
		HAL_I2C_Mem_Read(hi2c, NFC_UserMemory, address, Itl, data, n, 50);
	}
	else{
		//waiting to implement
	}
	HAL_Delay(10);
}

void NFC04A1_setRFMode(I2C_HandleTypeDef *hi2c,uint8_t mode){
	unlockI2CSecurity(hi2c, NFC_SystemMemory,0x00);
	writeSystemReg(hi2c, RF_MNGT,mode);
	lockI2CSecurity(hi2c,NFC_SystemMemory);
}

void NFC04A1_setRFModeDyn(I2C_HandleTypeDef *hi2c,uint8_t mode){
	unlockI2CSecurity(hi2c, NFC_SystemMemory,0x00);
	writeSystemReg(hi2c, RF_MNGT_Dyn,mode);
	lockI2CSecurity(hi2c,NFC_SystemMemory);
}
