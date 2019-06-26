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
#define I2CPassword		0x0012000000000000

//defines for NFC device register address
#define I2C_PWD		0x0900

//defines for command
#define presentPass		0x09
#define writePass		0x07

//defines for others
#define Itl	2
//functions declaration
void initNFC(I2C_HandleTypeDef *hi2c, uint16_t devAddress);
void currentAddRead(I2C_HandleTypeDef *hi2c, uint16_t devAddress, char *buffer,int n);
void presentI2Cpassword(I2C_HandleTypeDef *hi2c, uint16_t devAddress, int password);


#endif /* NFC_H_ */
