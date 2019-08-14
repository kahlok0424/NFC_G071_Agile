/*
 * I2C.h
 *
 *  Created on: Jul 4, 2019
 *      Author: user
 */

#ifndef I2C_H_
#define I2C_H_

#include "stdint.h"
//#include "stm32g0xx_hal_def.h"

void I2CRead(uint16_t devAddress,uint16_t memAddress, uint8_t *data, uint16_t n);
void I2CWrite(uint16_t devAddress,uint16_t memAddress, uint8_t *data, uint16_t n);
void NFC_Delay(uint32_t delay);

#endif /* I2C_H_ */
