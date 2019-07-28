/*
 * nfc.h
 *
 *  Created on: Jun 25, 2019
 *      Author: KahLok
 */

#ifndef NFC_H_
#define NFC_H_

//#include "main.h"
#include "stdint.h"
//#include "stm32g0xx_hal_def.h"

//define for leds on nfc
#define NFC_LED1		NFC_LED1_Pin
#define NFC_LED2		NFC_LED2_Pin
#define NFC_LED3		NFC_LED3_Pin
#define NFC_LED1_PORT	NFC_LED1_GPIO_Port
#define NFC_LED2_PORT	NFC_LED2_GPIO_Port
#define NFC_LED3_PORT	NFC_LED3_GPIO_Port

//defines for NFC status
#define NFC_OK			0x00
#define NFC_ERROR		0x01

//defines for NFC04A device address
#define NFC_USERMEMORY		0xA6
#define NFC_SYSTEMMEMORY	0xAE
#define NFC_DYNAMICMEMORY	0xA6
#define NFC_MAILBOX			0x2008

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
#define UID_ADDRESS		0x0018
#define IC_REV			0x0020
#define I2C_PWD			0x0900

//NFC device dynamic registers address
#define GPO_CTRL_DYN	0x2000
#define EH_CTRL_DYN		0x2002
#define RF_MNGT_DYN		0x2003
#define I2C_SSO_DYN		0x2004    //i2c security session status
#define IT_STS_DYN		0x2005
#define MB_CTRL_DYN		0x2006
#define MB_LEN_DYN		0x2007

//defines for command
#define PRESENTPASS		0x09
#define CHANGEPASS		0x07

//Masking for mail box status register
#define MAILBOX_ENABLE_MASK				0x01
#define HOST_PUT_MESSAGE_MASK			0x02
#define RF_PUT_MESSAGE_MASK				0x04
#define HOST_MISS_MESSAGE_MASK			0x10
#define RF_MISS_MESSAGE_MASK			0x20
#define HOST_CURRENT_MESSAGE_MASK		0x40
#define RF_CURRENT_MESSAGE_MASK			0x80

//RF mode typedef
typedef enum rf_mode{
	 RF_ENABLE 	= 0x00,
	 RF_DISABLE = 0x01,
	 RF_SLEEP	= 0x03,
}RF_MODE;

//Fast transfer mode typedef
typedef enum ftm_mode{
	 FTM_DISABLE 	= 0x00,
	 FTM_ENABLE		= 0x01,
}FTM_MODE;

typedef enum interrupt_mode{
	 RF_USER_EN 			= 0x01,
	 RF_ACTIVITY_EN 		= 0x02,
	 RF_INTERRUPT_EN 		= 0x04,
	 FIELD_CHANGE_EN 		= 0x08,
	 RF_PUT_MSG_EN 			= 0x10,
	 RF_GET_MSG_EN	 		= 0x20,
	 RF_WRITE_EN 			= 0x40,
	 INTERRUPT_EN 			= 0x80,
	 NOT_USED				= 0x00,
}INTERRUPT_MODE;

typedef enum gpo_mode{
	 GPO_EN 			= 0x80,
	 GPO_DISABLE		= 0x00,
}GPO_MODE;

typedef enum i2cwriteProtect{
	 NO_WRITEPROTECT		= 0x00,
	 AREA1_WRITEPROTECT 	= 0x01,
	 AREA2_WRITEPROTECT		= 0x04,
	 AREA3_WRITEPROTECT		= 0x10,
	 AREA4_WRITEPROTECT		= 0x40,
}I2CWRITEPROTECT;

typedef enum rfwriteProtect{
	 RFAREA_NOPROTECT 		 	= 0x00,
	 RFAREA_OPENWITHPASSWORD1 	= 0x01,
	 RFAREA_OPENWITHPASSWORD2 	= 0x02,
	 RFAREA_OPENWITHPASSWORD3 	= 0x03,
	 RFAREA_WRITEPROTECT		= 0x04,
	 RFAREA_WRITEFORBIDDEN		= 0x0c,
}RFWRITEPROTECT;

//defines for others
#define MAXUSERMEMORYSIZE	512
#define NA					-1		//Not available

//functions declaration
//void initNFC(I2C_HandleTypeDef *hi2c, uint16_t devAddress); //comment for development
void I2CRead(uint16_t devAddress, uint16_t memAddress, uint8_t *data, int n);
void I2CWrite(uint16_t devAddress, uint16_t memAddress, uint8_t *data, int n);
void unlockI2CSecurity(uint8_t *password);
void lockI2CSecurity();
void changeI2CPassword(uint8_t *oldPass, uint8_t *newPassword);
void readI2CPassword(uint8_t *password, uint8_t *ans);
void readSystemMemory(uint16_t regAddress, uint8_t *buffer,int n);
void writeSystemMemory(uint16_t regAddress,uint8_t *password, uint8_t data);
void readDynamicReg(uint16_t regAddress, uint8_t *buffer);
void writeDynamicReg(uint16_t regAddress, uint8_t data);
void readUserMemory(uint16_t address, uint8_t *data, int n);
void writeUserMemory(uint16_t address, uint8_t *data, int n);
void readDeviceUID(uint8_t *uid);
void NFC04A1_setRFMode(uint8_t *password, RF_MODE mode);
void NFC04A1_setRFModeDyn(RF_MODE mode);
int  checkAreaSizeValidity(int size, int max);
void setDefaultArea(uint8_t *password);
void set2Area(uint8_t *password, uint16_t size);
void set3Area(uint8_t *password, uint16_t sizeA1, uint16_t sizeA2);
void set4Area(uint8_t *password, uint16_t sizeA1, uint16_t sizeA2, uint16_t sizeA3);
int  validateArea(uint8_t area1, uint8_t area2, uint8_t area3);
void setArea(uint8_t *password, int area1, int area2, int area3);
void i2CWriteProtectUserArea(uint8_t *password, I2CWRITEPROTECT area);
void rfWriteProtectUserArea(uint8_t *password, RFWRITEPROTECT area1, RFWRITEPROTECT area2, RFWRITEPROTECT area3, RFWRITEPROTECT area4);
void enableMailBox(uint8_t *password);
void disableMailBox(uint8_t *password);
void setMailBoxTimeout(uint8_t *password, uint8_t wdgTime);
void resetMailBox();
void configFastTransferMode(uint8_t *password, FTM_MODE mode, uint8_t wdgTime);
void waitRFReadMessage();
void waitRFWriteMessage();
void readDatafromMailbox(uint16_t address, uint8_t *data, int n);
void writeDatatoMailbox(uint8_t *data, int n);
void getMailBoxStatus(uint8_t *status);
void getMailBoxMessage(uint8_t *data);
void enableInterrupt(uint8_t *password);
void disableInterrupt(uint8_t *password);
void configureInterrupt(uint8_t *password, INTERRUPT_MODE mode);
void enableEnergyHarvest(uint8_t *password);
void disableEnergyHarvest(uint8_t *password);
#endif /* NFC_H_ */
