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


//functions declaration
void initNFC(I2C_HandleTypeDef *hi2c, uint16_t devAddress);
void currentAddRead(I2C_HandleTypeDef *hi2c, uint16_t devAddress, char *buffer,int n,int timeout);


#endif /* NFC_H_ */
