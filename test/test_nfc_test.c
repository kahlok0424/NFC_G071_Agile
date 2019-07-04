#include "unity.h"
#include "nfc.h"
#include "mock_i2c.h"
#include "cmock.h"
#include <stdio.h>
#include <malloc.h>
#include "stdint.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_NFC_test_unlockI2CSecurity_given_correct_password(void)
{
    uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t correct[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t result[17];

    I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, correct,17,17);

    unlockI2CSecurity(password);
    //TEST_ASSERT_EQUAL_UINT8_ARRAY( correct,result,17);
}

void test_NFC_test_unlockI2CSecurity_given_wrong_password_expect_fail(void)
{
    uint8_t password[8] = {0x00,0x00,0x00,0x20,0x30,0x44,0x00,0x00};
    uint8_t correct[17] = {0x00,0x00,0x00,0x20,0x30,0x44,0x00,0x00,0x09,0x00,0x00,0x00,0x20,0x30,0x44,0x00,0x00};
    uint8_t result[17];

    I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, correct,17,17);

    unlockI2CSecurity(password);
    //TEST_ASSERT_EQUAL_UINT8_ARRAY( correct,result,17);
}

void test_NFC_test_lockI2CSecurity_expect_correct(void)
{
    uint8_t correct[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
    uint8_t result[17];

    I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, correct,17,17);

    lockI2CSecurity();
}

void test_NFC_test_changeI2CSecurity_expect_correct(void)
{
    uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t newPass[8] = {0x07,0x07,0x07,0x07,0x08,0x08,0x08,0x08};
    uint8_t correct[17] = {0x07,0x07,0x07,0x07,0x08,0x08,0x08,0x08,0x07,0x07,0x07,0x07,0x07,0x08,0x08,0x08,0x08};
    uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};

    I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
    I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, correct,17,17);
    I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

    changeI2CPassword(password,newPass);
    //TEST_ASSERT_EQUAL_UINT8_ARRAY( correct,result,17);
}

void test_checkAreaSizeValidity_given_128_expect_true(void){

	int check = checkAreaSizeValidity(128);
	TEST_ASSERT_TRUE(check);
}

void test_checkAreaSizeValidity_given_32_expect_true(void){

	int check = checkAreaSizeValidity(32);
	TEST_ASSERT_TRUE(check);
}

void test_checkAreaSizeValidity_given_18_expect_false(void){

	int check = checkAreaSizeValidity(18);
	TEST_ASSERT_FALSE(check);
}

void test_checkAreaSizeValidity_given_133_expect_false(void){

	int check = checkAreaSizeValidity(133);
	TEST_ASSERT_FALSE(check);
}

void test_checkAreaSizeValidity_given_700_expect_false(void){

	int check = checkAreaSizeValidity(700);
	TEST_ASSERT_FALSE(check);
}

void test_set2Area_given_128bytes_expect_write_3(void){

    uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
    uint8_t expectENDA3[1] = { 0xf };
    uint8_t expectENDA1[1] = { 0x3 };

	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expectENDA3,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expectENDA3,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expectENDA1,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

	set2Area(password, 128);
}

void test_set2Area_given_32bytes_expect_write_0(void){

    uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
    uint8_t expectENDA3[1] = { 0xf };
    uint8_t expectENDA1[1] = { 0x0 };

	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expectENDA3,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expectENDA3,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expectENDA1,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

	set2Area(password, 32);
}

void test_set2Area_given_12bytes_expect_restore_default(void){

    uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
    uint8_t expectDefault[1] = { 0xf };

	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expectDefault,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expectDefault,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expectDefault,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

	set2Area(password, 12);
}

void test_set2Area_given_600bytes_expect_restore_default(void){

    uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
    uint8_t expectDefault[1] = { 0xf };

	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expectDefault,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expectDefault,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expectDefault,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

	set2Area(password, 600);
}

void test_set3Area_given_32bytes_and_160bytes_expect_write_0_and_3(void){

    uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
    uint8_t expectENDA3[1] = { 0xf };
    uint8_t expectENDA2[1] = { 0x4 };
    uint8_t expectENDA1[1] = { 0x0 };

	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expectENDA3,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expectENDA2,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expectENDA1,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

	set3Area(password, 32,160);
}

void test_set3Area_given_32bytes_and_160bytes_expect_restore_default(void){

    uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
    uint8_t expectDefault[1] = { 0xf };

	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expectDefault,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expectDefault,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expectDefault,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

	set3Area(password, 128,32);
}

void test_set3Area_given_128bytes_and_128bytes_expect_restore_default(void){

    uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
    uint8_t expectDefault[1] = { 0xf };

	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expectDefault,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expectDefault,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expectDefault,1,1);
	I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

	set3Area(password, 128,32);
}
