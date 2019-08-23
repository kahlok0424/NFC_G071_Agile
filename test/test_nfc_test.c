#include "unity.h"
#include "nfc.h"
#include "mock_i2c.h"
#include "cmock.h"
#include <stdio.h>
#include <malloc.h>
#include "stdint.h"
#include "string.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_NFC_test_unlockI2CSecurity_given_correct_password(void)
{
    uint8_t password[8] = {0x01,0x02,0x03,0x04,0x09,0x08,0x77,0x66};
    uint8_t correct[17] = {0x01,0x02,0x03,0x04,0x09,0x08,0x77,0x66,0x09,0x01,0x02,0x03,0x04,0x09,0x08,0x77,0x66};
    uint8_t result[17];

    I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, correct,17,17);

    unlockI2CSecurity(password);
}

void test_NFC_test_unlockI2CSecurity_given_wrong_password_expect_fail(void)
{
    uint8_t password[8] = {0x00,0x00,0x00,0x20,0x30,0x44,0x00,0x00};
    uint8_t correct[17] = {0x00,0x00,0x00,0x20,0x30,0x44,0x00,0x00,0x09,0x00,0x00,0x00,0x20,0x30,0x44,0x00,0x00};
    uint8_t result[17];

    I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, correct,17,17);

    unlockI2CSecurity(password);
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
    uint8_t password[8] = {0x01,0x00,0x01,0x00,0x02,0x00,0x02,0x00};
    uint8_t expectUnlock[17] = {0x01,0x00,0x01,0x00,0x02,0x00,0x02,0x00,0x09,0x01,0x00,0x01,0x00,0x02,0x00,0x02,0x00};
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

  int check = checkAreaSizeValidity(128,512);
  TEST_ASSERT_TRUE(check);
}

void test_checkAreaSizeValidity_given_32_expect_true(void){

  int check = checkAreaSizeValidity(32,512);
  TEST_ASSERT_TRUE(check);
}

void test_checkAreaSizeValidity_given_18_expect_false(void){

  int check = checkAreaSizeValidity(18,512);
  TEST_ASSERT_FALSE(check);
}

void test_checkAreaSizeValidity_given_133_expect_false(void){

  int check = checkAreaSizeValidity(133,512);
  TEST_ASSERT_FALSE(check);
}

void test_checkAreaSizeValidity_given_700_expect_false(void){

  int check = checkAreaSizeValidity(700,512);
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

void test_validateArea_given_1_5_10_expect_true(void){

  int check = validateArea(1,4,10);
  TEST_ASSERT_TRUE(check);
}

void test_validateArea_given_1_10_10_expect_false(void){

  int check = validateArea(1,10,10);
  TEST_ASSERT_FALSE(check);
}

void test_setArea_given_1_and_2_expect_set_2_areas(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = { 0x0,0x2,0xf };

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  setArea(password, 1,2,NA);
}

void test_setArea_given_2_and_4_10_expect_set_4_areas(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = { 0x1,0x5,0xf };

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  setArea(password, 2,4,10);
}

void test_setArea_given_2_and_1_NA_expect_set_3_areas(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = { 0x1,0x2,0xf };

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  setArea(password, 2,1,NA);
}

void test_setArea_given_8_and_8_expect_set_2_areas(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = { 0x7,0xf,0xf };

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  setArea(password, 8,8,NA);
}

void test_setArea_given_NA_and_6_expect_restore_default(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = { 0xf,0xf,0xf };

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  setArea(password, NA,6,NA);
}

void test_setArea_given_NA_6_and_9_expect_restore_default(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = { 0xf,0xf,0xf };

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  setArea(password, NA,6,9);
}

void test_setArea_given_2_NA_and_5_expect_set_2_area(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = { 0x1,0xf,0xf };

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  setArea(password, 2,NA,5);
}

void test_setArea_given_2_8_and_10_expect_restore_default(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = { 0xf,0xf,0xf };

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  setArea(password, 2,8,10);
}

void test_setArea_given_2_0_and_10_expect_restore_default(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = { 0xf,0xf,0xf };

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  setArea(password, 2,0,10);
}

void test_setArea_given_0_0_and_10_expect_restore_default(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = { 0xf,0xf,0xf };

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  setArea(password, 0,0,10);
}

void test_userAreaRWProtection_given_no_protection(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x0};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2CSS, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  i2CWriteProtectUserArea(password,NO_WRITEPROTECT);
}

void test_userAreaRWProtection_given_only_area_1_protect(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x01};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2CSS, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  i2CWriteProtectUserArea(password,AREA1_WRITEPROTECT);
}

void test_userAreaRWProtection_given_only_area_2_protect(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x04};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2CSS, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  i2CWriteProtectUserArea(password,AREA2_WRITEPROTECT);
}

void test_userAreaRWProtection_given_only_area_3_protect(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x10};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2CSS, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  i2CWriteProtectUserArea(password,AREA3_WRITEPROTECT);
}

void test_userAreaRWProtection_given_only_area_4_protect(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x40};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2CSS, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  i2CWriteProtectUserArea(password,AREA4_WRITEPROTECT);
}

void test_userAreaRWProtection_given_only_area_1_and_area_2_protect(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x05};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2CSS, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  i2CWriteProtectUserArea(password,AREA1_WRITEPROTECT|AREA2_WRITEPROTECT);
}

void test_userAreaRWProtection_given_area_1_and_area_2_and_area_4_protect(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x45};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2CSS, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  i2CWriteProtectUserArea(password,AREA1_WRITEPROTECT|AREA2_WRITEPROTECT|AREA4_WRITEPROTECT);
}

void test_rfWriteProtectUserArea_given_area_1_and_area_2_and_area_4_protect(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[2] = {0x04,0x00};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, RFA1SS, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, RFA2SS, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, RFA3SS, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, RFA4SS, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  rfWriteProtectUserArea(password,RFAREA_WRITEPROTECT,RFAREA_WRITEPROTECT,RFAREA_NOPROTECT,RFAREA_WRITEPROTECT);
}

void test_rfWriteProtectUserArea_given_area1_forbiden_area2_no_protect_area3_and_area4_protect(void){

  uint8_t password[8] = {0x01,0x02,0x03,0x04,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x01,0x02,0x03,0x04,0x00,0x00,0x00,0x00,0x09,0x01,0x02,0x03,0x04,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = {0x0c,0x00,0x04};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, RFA1SS, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, RFA2SS, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, RFA3SS, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, RFA4SS, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  rfWriteProtectUserArea(password,RFAREA_WRITEFORBIDDEN,RFAREA_NOPROTECT,RFAREA_WRITEPROTECT,RFAREA_WRITEPROTECT);
}

void test_enableMailbox_expect_pass(void)
{
  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x1};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, MB_MODE, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);
  I2CWrite_ExpectWithArray(NFC_DYNAMICMEMORY,MB_CTRL_DYN, expect,1, 1);

  enableMailBox(password);
}

void test_disableMailbox_expect_pass(void)
{
  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x0};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, MB_MODE, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);
  I2CWrite_ExpectWithArray(NFC_DYNAMICMEMORY,MB_CTRL_DYN, expect,1, 1);

  disableMailBox(password);
}

void test_resetMailBox_expect_pass(void){

  uint8_t expectOn[1] = {0x1};
  uint8_t expectOff[1] = {0x0};

  I2CWrite_ExpectWithArray(NFC_DYNAMICMEMORY,MB_CTRL_DYN, expectOff,1, 1);
  I2CWrite_ExpectWithArray(NFC_DYNAMICMEMORY,MB_CTRL_DYN, expectOn,1, 1);

  resetMailBox();
}

void test_configFastTransferMode_FTM_ENABLE_expect_pass(void)
{
  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[2] = {0x1,0x0};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, MB_MODE, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);
  I2CWrite_ExpectWithArray(NFC_DYNAMICMEMORY,MB_CTRL_DYN, expect,1, 1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, MB_WDG, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  configFastTransferMode(password, FTM_ENABLE,0x00);
}

void test_configFastTransferMode_FTM_DISABLE_expect_pass(void)
{
  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[2] = {0x0,0x11};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, MB_MODE, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);
  I2CWrite_ExpectWithArray(NFC_DYNAMICMEMORY,MB_CTRL_DYN, expect,1, 1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, MB_WDG, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  configFastTransferMode(password, FTM_DISABLE,0x11);
}

void test_enableInterrupt_expect_pass(void)
{
  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x80};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, GPO, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);
  I2CWrite_ExpectWithArray(NFC_DYNAMICMEMORY, GPO_CTRL_DYN, expect,1,1);

  enableInterrupt(password);
}

void test_disableInterrupt_expect_pass(void)
{
  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x0};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, GPO, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);
  I2CWrite_ExpectWithArray(NFC_DYNAMICMEMORY, GPO_CTRL_DYN, expect,1,1);

  disableInterrupt(password);
}

void test_configureInterrupt_given_RF_USER_EN_and_RF_ACTIVITY_EN_expect_correct(void)
{
  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x03};
  uint8_t expectZero[1] = {0x0};

  I2CRead_ExpectWithArray(NFC_SYSTEMMEMORY, GPO, expectZero,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, GPO, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  configureInterrupt(password,RF_USER_EN|RF_ACTIVITY_EN);
}

void test_configureInterrupt_given_RF_INTERRUPT_EN_and_RF_PUT_MSG_EN_and_FIELD_CHANGE_expect_correct(void)
{
  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x1c};
  uint8_t expectZero[1] = {0x0};

  I2CRead_ExpectWithArray(NFC_SYSTEMMEMORY, GPO, expectZero,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, GPO, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  configureInterrupt(password,RF_INTERRUPT_EN|RF_PUT_MSG_EN|FIELD_CHANGE_EN);
}

void test_configureInterrupt_given_all_disable_expect_correct(void)
{
  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x00};
  uint8_t expectZero[1] = {0x0};

  I2CRead_ExpectWithArray(NFC_SYSTEMMEMORY, GPO, expectZero,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, GPO, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  configureInterrupt(password,NOT_USED);
}

void test_configureInterrupt_given_all_enabled_expect_correct(void)
{
  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x7f};
  uint8_t expectZero[1] = {0x0};

  I2CRead_ExpectWithArray(NFC_SYSTEMMEMORY, GPO, expectZero,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, GPO, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  configureInterrupt(password,RF_USER_EN|RF_ACTIVITY_EN|RF_INTERRUPT_EN|FIELD_CHANGE_EN|RF_PUT_MSG_EN|RF_GET_MSG_EN|RF_WRITE_EN);
}

void test_enableEnergyHarvest_expect_pass(void)
{
  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[2] = {0x00,0x01};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, EH_MODE, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);
  I2CWrite_ExpectWithArray(NFC_DYNAMICMEMORY, EH_CTRL_DYN, expect+1,1,1);

  enableEnergyHarvest(password);
}

void test_disableEnergyHarvest_expect_pass(void)
{
  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[2] = {0x01,0x00};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, EH_MODE, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);
  I2CWrite_ExpectWithArray(NFC_DYNAMICMEMORY, EH_CTRL_DYN, expect+1,1,1);

  disableEnergyHarvest(password);
}
