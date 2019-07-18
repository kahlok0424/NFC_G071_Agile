/*
 * I2C.c
 *
 *  Created on: Jul 4, 2019
 *      Author: user
 */

#include "i2c.h"
//#include "stm32g0xx_hal.h"

void I2CRead(uint16_t devAddress,uint16_t memAddress, uint8_t *data, int n){
	HAL_I2C_Mem_Read(&hi2c1, devAddress, memAddress, 2, data, n, 50);
	HAL_Delay(10);
}

void I2CWrite(uint16_t devAddress,uint16_t memAddress, uint8_t *data, int n){
	HAL_I2C_Mem_Write(&hi2c1, devAddress, memAddress, 2, data, n, 50);
	HAL_Delay(10);
}

void NFC_Delay(uint32_t delay){
	HAL_Delay(delay);
}
