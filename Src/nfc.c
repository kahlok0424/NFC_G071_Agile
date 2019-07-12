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
void writeUserMemory(uint16_t address, uint8_t *data, int n){

		I2CWrite(NFC_USERMEMORY, address, data, n);
		//waiting to implement
}

/**
 * @description read data of user memory given address
 * @param area of user memory (1 ~ 4)
 * @param address to write
 * @param pointer to data to read
 * @param number of bytes to read
 */
void readUserMemory(uint16_t address, uint8_t *data, int n){

		I2CRead(NFC_USERMEMORY, address, data, n);
		//waiting to implement
}

void NFC04A1_setRFMode(uint8_t *password, RF_MODE mode){
	writeSystemMemory(RF_MNGT, password, mode);
}

void NFC04A1_setRFModeDyn(RF_MODE mode){
	writeDynamicReg(RF_MNGT_DYN, mode);
}

int checkAreaSizeValidity(int size, int max){
	if( (size%32) == 0 && size <= max){
		return 1;
	}
	else{
		return 0;
	}
}

/**
 * @brief configure the user memory into 1 area and the area cover all user memory (default setting)
 * @param password to unlock I2C security
 */
void setDefaultArea(uint8_t *password){

	uint8_t temp[1];
	temp[0] = 0xf;

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
	if( checkAreaSizeValidity(size, MAXUSERMEMORYSIZE) ){
		unlockI2CSecurity(password);
		I2CWrite(NFC_SYSTEMMEMORY,ENDA3, temp, 1);
		I2CWrite(NFC_SYSTEMMEMORY,ENDA2, temp, 1);
		I2CWrite(NFC_SYSTEMMEMORY,ENDA1, temp+1, 1);
		lockI2CSecurity();
	}
	else{
		setDefaultArea(password);
	}
}

void set3Area(uint8_t *password, uint16_t sizeA1, uint16_t sizeA2){

	uint8_t temp[3];
	temp[0] = 0xf;
	temp[1] = (sizeA2/32)-1;
	temp[2] = (sizeA1/32)-1;
	if( checkAreaSizeValidity(sizeA1,MAXUSERMEMORYSIZE) && checkAreaSizeValidity(sizeA2,MAXUSERMEMORYSIZE)){
		if( (sizeA2 > sizeA1)== 1 ){
			unlockI2CSecurity(password);
			I2CWrite(NFC_SYSTEMMEMORY,ENDA3, temp, 1);
			I2CWrite(NFC_SYSTEMMEMORY,ENDA2, temp+1, 1);
			I2CWrite(NFC_SYSTEMMEMORY,ENDA1, temp+2, 1);
			lockI2CSecurity();
		}else{
			setDefaultArea(password);
		}
	}
	else{
		setDefaultArea(password);
	}
}

void set4Area(uint8_t *password, uint16_t sizeA1, uint16_t sizeA2, uint16_t sizeA3){

	uint8_t temp[3];
	temp[0] = sizeA3;
	temp[1] = sizeA2;
	temp[2] = sizeA1;
	if( checkAreaSizeValidity(sizeA1,MAXUSERMEMORYSIZE) && checkAreaSizeValidity(sizeA2,MAXUSERMEMORYSIZE) && checkAreaSizeValidity(sizeA3,MAXUSERMEMORYSIZE)){
		if( ((sizeA3 > sizeA2) == 1) && ((sizeA2 > sizeA1) == 1) ){
			unlockI2CSecurity(password);
			I2CWrite(NFC_SYSTEMMEMORY,ENDA3, temp, 1);
			I2CWrite(NFC_SYSTEMMEMORY,ENDA2, temp+1, 1);
			I2CWrite(NFC_SYSTEMMEMORY,ENDA1, temp+2, 1);
			lockI2CSecurity();
		}else{
			setDefaultArea(password);
		}
	}
	else{
		setDefaultArea(password);
	}
}

int validateArea(uint8_t area1, uint8_t area2, uint8_t area3){

	uint8_t total;
	total = area1 + area2 + area3;

		if(total > 16){
			return 0;
		}else{
			return 1;
		}
}

/**
 * @brief Allocate the user area into number of areas ( 1~4) and with different size each area.
 * @brief Each area must contain at least 1 block (1 block = 32 bytes) so the number of block cannot be 0.
 * @brief Example to specify 2 areas with first area 5 blocks and second area 3 blocks. #setArea(5,3,NA,NA);
 * @brief Specify "NA" in the parameter to indicate no area is allocated or needed.
 * @brief Total number of block in all areas has to be 16 and the last area cannot be specify, it will be allocated with the remaining user memory by default.
 * @param size of area 1, area 2 and area 3 in integer, size of area 4 is the remaining user memory
 */
void setArea(uint8_t *password, int area1, int area2, int area3){

	uint8_t areas[3];

	if(area1 != 0 && area2 != 0 && area3 != 0){

		if (area1 == NA){
			areas[0] = 0xf;
			areas[1] = 0xf;
			areas[2] = 0xf;
		}else if( area2 == NA && validateArea(area1,0,0) ){
			areas[0] = (area1)-1;
			areas[1] = 0xf;
			areas[2] = 0xf;
		}else if(area3 == NA && validateArea(area1, area2,0) ){
			areas[0] = (area1)-1;
			areas[1] = (area2+area1)-1;
			areas[2] = 0xf;
		}else if(area1 != NA && area2 != NA && area3 != NA && validateArea(area1,area2,area3)){
			areas[0] = (area1)-1;
			areas[1] = (area2+area1)-1;
			areas[2] = (area3+area2+area1)-1;
		}else{
			areas[0] = 0xf;
			areas[1] = 0xf;
			areas[2] = 0xf;
		}
	}else{
		areas[0] = 0xf;
		areas[1] = 0xf;
		areas[2] = 0xf;
	}

	unlockI2CSecurity(password);
	I2CWrite(NFC_SYSTEMMEMORY,ENDA3, areas+2, 1);
	I2CWrite(NFC_SYSTEMMEMORY,ENDA2, areas+1, 1);
	I2CWrite(NFC_SYSTEMMEMORY,ENDA1, areas, 1);
	lockI2CSecurity();
}

void userAreaRWProtection(uint8_t *password, WRITEPROTECT area1, WRITEPROTECT area2, WRITEPROTECT area3, WRITEPROTECT area4){

	uint8_t temp[1];
	temp[0] = (area1 | area2 | area3 | area4 );
	unlockI2CSecurity(password);
	I2CWrite(NFC_SYSTEMMEMORY,I2CSS, temp, 1);
	lockI2CSecurity();
}

void enableMailBox(uint8_t *password){

	uint8_t temp[1];
	temp[0] = 0x1;

	unlockI2CSecurity(password);
	I2CWrite(NFC_SYSTEMMEMORY,MB_MODE, temp, 1);
	lockI2CSecurity();
	I2CWrite(NFC_DYNAMICMEMORY,MB_CTRL_DYN, temp, 1);
}

void disableMailBox(uint8_t *password){

	uint8_t temp[1];
	temp[0] = 0x0;

	unlockI2CSecurity(password);
	I2CWrite(NFC_SYSTEMMEMORY,MB_MODE, temp, 1);
	lockI2CSecurity();
	I2CWrite(NFC_DYNAMICMEMORY,MB_CTRL_DYN, temp, 1);
}

void setMailBoxTimeout(uint8_t *password, uint8_t wdgTime){

	uint8_t temp[1];
	temp[0] = wdgTime;
	I2CWrite(NFC_SYSTEMMEMORY,MB_WDG, temp, 1);
}

void configFastTransferMode(uint8_t *password, FTM_MODE mode, uint8_t wdgTime){

	uint8_t temp1[1];
	temp1[0] = mode;
	uint8_t temp2[1];
	temp2[0] = wdgTime;

	unlockI2CSecurity(password);
	if(mode == FTM_ENABLE){
		enableMailBox(password);
	}
	else if(mode == FTM_DISABLE){
		disableMailBox(password);
	}
	setMailBoxTimeout(password, wdgTime);
	lockI2CSecurity();
}

void waitRFReadMessage(){

	uint8_t temp[2];
	temp[0] = 0x2;

	while( (temp[0] & 0x02) ){
		readDynamicReg(MB_CTRL_DYN,temp);
		//HAL_Delay(100);
	}
	readDynamicReg(MB_LEN_DYN,temp+1);
}

void waitRFWriteMessage(){

	uint8_t temp[2];
	temp[0] = 0x4;

	while( (temp[0] & 0x04) ){
		readDynamicReg(MB_CTRL_DYN,temp);
		//HAL_Delay(100);
	}
	readDynamicReg(MB_LEN_DYN,temp+1);
}

void getMailBoxMessage(uint8_t *data){

	uint8_t temp[1];

	readDynamicReg(MB_LEN_DYN,temp);
	readUserMemory(NFC_MAILBOX,data,temp[0]);
}

void enableInterrupt(uint8_t *password){

	uint8_t temp[1];
	temp[0] = GPO_EN;

	unlockI2CSecurity(password);
	I2CWrite(NFC_SYSTEMMEMORY,GPO, temp, 1);
	lockI2CSecurity();
	I2CWrite(NFC_DYNAMICMEMORY,GPO_CTRL_DYN, temp, 1);
}

void disableInterrupt(uint8_t *password){

	uint8_t temp[1];
	temp[0] = GPO_DISABLE;

	unlockI2CSecurity(password);
	I2CWrite(NFC_SYSTEMMEMORY,GPO, temp, 1);
	lockI2CSecurity();
	I2CWrite(NFC_DYNAMICMEMORY,GPO_CTRL_DYN, temp, 1);
}

void configureInterrupt(uint8_t *password, INTERRUPT_MODE mode1, INTERRUPT_MODE mode2, INTERRUPT_MODE mode3, INTERRUPT_MODE mode4, INTERRUPT_MODE mode5, INTERRUPT_MODE mode6, INTERRUPT_MODE mode7){

	uint8_t temp[1];
	temp[0] = 0;

	I2CRead(NFC_SYSTEMMEMORY,GPO, temp, 1);
	temp[0] = mode1 | mode2 | mode3 | mode4 | mode5 | mode6 | mode7 | temp[0];
	unlockI2CSecurity(password);
	I2CWrite(NFC_SYSTEMMEMORY,GPO, temp, 1);
	lockI2CSecurity();
}

