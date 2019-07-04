/*
 * nfc.c
 *
 *  Created on: Jun 25, 2019
 *      Author: KahLok
 */

#include "nfc.h"
//#include "main.h"
#include "i2c.h"
//#include "stm32g0xx_hal.h"

/*void initNFC(I2C_HandleTypeDef *hi2c, uint16_t devAddress){

	if(HAL_I2C_IsDeviceReady(hi2c, devAddress,3,50) == 0){
		HAL_GPIO_WritePin(NFC_LED1_PORT,NFC_LED1,1);
		//HAL_GPIO_WritePin(NFC_LED2_PORT,NFC_LED2,1);
	}
	else{
		HAL_GPIO_WritePin(NFC_LED1_PORT,NFC_LED1,0);
		HAL_GPIO_WritePin(NFC_LED1_PORT,NFC_LED2,0);
		HAL_GPIO_WritePin(NFC_LED2_PORT,NFC_LED3,0);
	}
}*/

/**
 * @description present the i2c password and unlock i2c security session
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
		WrongPass[i+9] = i+3;
	}

	I2CWrite(NFC_SYSTEMMEMORY, I2C_PWD, WrongPass,17); //present wrong i2c password
}

/**
 * @description present the old i2c password and write a new password
 * @param old password
 * @param new password
 */
void changeI2CPassword(uint8_t *oldpass, uint8_t *newPassword){

	uint8_t temp[17];

	for(int i = 0; i < 8; i++){
		temp[i] = newPassword[i];
	}
	temp[8] = CHANGEPASS;
	for(int i = 0; i < 8; i++){
		temp[i+9] = newPassword[i];
	}
	unlockI2CSecurity(oldpass); //unlock i2c
	I2CWrite(NFC_SYSTEMMEMORY, I2C_PWD, temp,17); //write new password
	lockI2CSecurity();
}

void readI2CPassword(uint8_t *password, uint8_t *ans){
	unlockI2CSecurity(password); //unlock i2c
	I2CRead(NFC_SYSTEMMEMORY, I2C_PWD, ans, 8);
	lockI2CSecurity();
}

/**
 * @description read the value of register into the buffer
 * @param register address
 * @param buffer
 * @param number of bytes to read
 */
void readSystemMemory(uint16_t regAddress, uint8_t *buffer,int n){

	I2CRead(NFC_SYSTEMMEMORY, regAddress, buffer, n);
}

/**
 * @description write data into the system static register
 * @param register address
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
 * @description read the value of dynamic register into the buffer
 * @param register address
 * @param buffer
 */
void readDynamicReg(uint16_t regAddress, uint8_t *buffer){

	I2CRead(NFC_DYNAMICMEMORY, regAddress, buffer, 1);
}

/**
 * @description write data into the dyanmic register
 * @param register address
 * @param data to write
 */
void writeDynamicReg(uint16_t regAddress, uint8_t data){

	uint8_t buffer[1];
	buffer[0] = data;

	I2CWrite(NFC_DYNAMICMEMORY, regAddress, buffer, 1);
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
}

void NFC04A1_setRFMode(uint8_t *password, RF_MODE mode){
	writeSystemMemory(RF_MNGT, password, mode);
}

void NFC04A1_setRFModeDyn(RF_MODE mode){
	writeDynamicReg(RF_MNGT_DYN, mode);
}

void configFTM(uint8_t *password, FTM_MODE mode, uint8_t wdgTime){

	uint8_t temp1[1];
	temp1[0] = mode;
	uint8_t temp2[1];
	temp2[0] = wdgTime;

	unlockI2CSecurity(password);
	I2CWrite(NFC_SYSTEMMEMORY,MB_MODE, temp1, 1);
	I2CWrite(NFC_SYSTEMMEMORY,MB_WDG,temp2, 1);
	lockI2CSecurity();
}

int checkAreaSizeValidity(int size){
	if( (size%32) == 0 && size <= 512){
		return 1;
	}
	else{
		return 0;
	}
}

/**
 * @brief configure the user memory into 1 area and the area cover all user memory
 * @param password to unlock I2C security
 */
void set1Area(uint8_t *password){

	uint8_t temp[2];
	temp[0] = 0x0f;
	//temp[1] = (size/32)-1;

	unlockI2CSecurity(password);
	I2CWrite(NFC_SYSTEMMEMORY,ENDA3, temp, 1);
	I2CWrite(NFC_SYSTEMMEMORY,ENDA2, temp, 1);
	I2CWrite(NFC_SYSTEMMEMORY,ENDA1, temp, 1);
	lockI2CSecurity();
}

/**
 * @brief configure the user memory into 2 area, area size must be multiple of 32bytes
 * @param password to unlock I2C security
 * @param size of area in bytes (multiple of 32)
 */
void set2Area(uint8_t *password, uint16_t size){

	uint8_t temp[2];
	temp[0] = 0xf;
	temp[1] = (size/32)-1;
	if( checkAreaSizeValidity(size) ){
		unlockI2CSecurity(password);
		I2CWrite(NFC_SYSTEMMEMORY,ENDA3, temp, 1);
		I2CWrite(NFC_SYSTEMMEMORY,ENDA2, temp, 1);
		I2CWrite(NFC_SYSTEMMEMORY,ENDA1, temp+1, 1);
		lockI2CSecurity();
	}
	else{
		set1Area(password);
	}
}

void set3Area(uint8_t *password, uint16_t sizeA1, uint16_t sizeA2){

	uint8_t temp[3];
	temp[0] = 0xf;
	temp[1] = (sizeA2/32)-1;
	temp[2] = (sizeA1/32)-1;
	if( checkAreaSizeValidity(sizeA1) && checkAreaSizeValidity(sizeA2)){
		if( (sizeA2 > sizeA1)== 1 ){
			unlockI2CSecurity(password);
			I2CWrite(NFC_SYSTEMMEMORY,ENDA3, temp, 1);
			I2CWrite(NFC_SYSTEMMEMORY,ENDA2, temp+1, 1);
			I2CWrite(NFC_SYSTEMMEMORY,ENDA1, temp+2, 1);
			lockI2CSecurity();
		}else{
			set1Area(password);
		}
	}
	else{
		set1Area(password);
	}
}

void set4Area(uint8_t *password, uint16_t sizeA1, uint16_t sizeA2, uint16_t sizeA3){

	uint8_t temp[3];
	temp[0] = sizeA3;
	temp[1] = sizeA2;
	temp[2] = sizeA1;
	if( checkAreaSizeValidity(sizeA1) && checkAreaSizeValidity(sizeA2) && checkAreaSizeValidity(sizeA3)){
		if( ((sizeA3 > sizeA2) == 1) && ((sizeA2 > sizeA1) == 1) ){
			unlockI2CSecurity(password);
			I2CWrite(NFC_SYSTEMMEMORY,ENDA3, temp, 1);
			I2CWrite(NFC_SYSTEMMEMORY,ENDA2, temp+1, 1);
			I2CWrite(NFC_SYSTEMMEMORY,ENDA1, temp+2, 1);
			lockI2CSecurity();
		}else{
			set1Area(password);
		}
	}
	else{
		set1Area(password);
	}
}
