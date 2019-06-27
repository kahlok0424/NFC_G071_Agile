/*
 * nfc.h
 *
 *  Created on: Jun 25, 2019
 *      Author: KahLok
 */

#ifndef NFC_H_
#define NFC_H_

#include "main.h"
#include "stdint.h"
#include "stm32g0xx_hal_def.h"

//define for leds on nfc
#define NFC_LED1		NFC_LED1_Pin
#define NFC_LED2		NFC_LED2_Pin
#define NFC_LED3		NFC_LED3_Pin
#define NFC_LED1_PORT	NFC_LED1_GPIO_Port
#define NFC_LED2_PORT	NFC_LED2_GPIO_Port
#define NFC_LED3_PORT	NFC_LED3_GPIO_Port

//defines for i2c password
#define I2CPassword		0x0000

//NFC device static register address
#define GPO				0x0000
#define IT_TIME			0x0001
#define EH_MODE			0x0002
#define RF_MNGT			0x0003
#define RFA1SS			0x0004
#define ENDA1			0x0005
#define RFA2SS			0x0006
#define ENDA2			0x0007
#define RFA3SS			0x0008
#define ENDA3			0x0009
#define RFA4SS			0x000A
#define I2CSS			0x000B
#define LOCK_CCFILE		0x000C
#define MB_MODE			0x000D
#define MB_WDG			0x000E
#define LOCK_CFG		0x000F
#define LOCK_DSFID		0x0010
#define LOCK_AFI		0x0011
#define DSFID			0x0012
#define AFI				0x0013
#define MEM_SIZE		0x0014
#define BLK_SIZE		0x0016
#define IC_REF			0x0017
#define UID				0x0018
#define IC_REV			0x0020
#define I2C_PWD			0x0900

//NFC device dynamic registers address
#define RF_MNGT_Dyn		0x2003
#define I2C_SSO_Dyn		0x2004    //i2c security session status
#define IT_STS_Dyn		0x2005

//defines for command
#define presentPass		0x09
#define writePass		0x07
#define RF_ENABLE		0x00
#define RF_DISABLE		0x01    //rf command interpreted, not executed
#define RF_SLEEP		0x03	//disable all RF communication

//defines for others
#define Itl	2

//functions declaration
void initNFC(I2C_HandleTypeDef *hi2c, uint16_t devAddress);
void currentAddRead(I2C_HandleTypeDef *hi2c, uint16_t devAddress,uint8_t *buffer,int n);
void unlockI2CSecurity(I2C_HandleTypeDef *hi2c, uint16_t devAddress, int password);
void lockI2CSecurity(I2C_HandleTypeDef *hi2c, uint16_t devAddress);
void changeI2CPassword(I2C_HandleTypeDef *hi2c, uint16_t devAddress, uint8_t *newPassword);
void readI2CPassword(I2C_HandleTypeDef *hi2c, uint16_t devAddress, uint8_t *password);
void readReg(I2C_HandleTypeDef *hi2c, uint16_t devAddress, uint16_t regAddress, uint8_t *buffer,int n);
void writeSystemReg(I2C_HandleTypeDef *hi2c, uint16_t regAddress, uint8_t data);
void writeUserMemory(I2C_HandleTypeDef *hi2c, int area, uint16_t address, uint8_t *data, int n);
void readUserMemory(I2C_HandleTypeDef *hi2c, int area, uint16_t address, uint8_t *data, int n);
void NFC04A1_setRFMode(I2C_HandleTypeDef *hi2c, uint8_t mode);
void NFC04A1_setRFModeDyn(I2C_HandleTypeDef *hi2c,uint8_t mode);

#endif /* NFC_H_ */
