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

void test_enableFTM_expect_pass(void)
{
  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x1};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, MB_MODE, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  enableFTM(password);
}

void test_disableFTM_expect_pass(void)
{
  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x0};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, MB_MODE, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  disableFTM(password);
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
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  setArea(password, 1,2,NA);
}

void test_setArea_given_2_and_4_10_expect_set_4_areas(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = { 0x1,0x5,0xf };

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  setArea(password, 2,4,10);
}

void test_setArea_given_8_and_8_expect_set_2_areas(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = { 0x7,0xf,0xf };

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  setArea(password, 8,8,NA);
}

void test_setArea_given_NA_and_6_expect_restore_default(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = { 0xf,0xf,0xf };

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  setArea(password, NA,6,NA);
}

void test_setArea_given_NA_6_and_9_expect_restore_default(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = { 0xf,0xf,0xf };

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  setArea(password, NA,6,9);
}

void test_setArea_given_2_NA_and_5_expect_set_2_area(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = { 0x1,0xf,0xf };

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  setArea(password, 2,NA,5);
}

void test_setArea_given_2_8_and_10_expect_restore_default(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = { 0xf,0xf,0xf };

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  setArea(password, 2,8,10);
}

void test_setArea_given_2_0_and_10_expect_restore_default(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = { 0xf,0xf,0xf };

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  setArea(password, 2,0,10);
}

void test_setArea_given_0_0_and_10_expect_restore_default(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[3] = { 0xf,0xf,0xf };

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA3, expect+2,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA2, expect+1,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, ENDA1, expect,1,1);
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

  userAreaRWProtection(password,NO_WRITEPROTECT,NO_WRITEPROTECT,NO_WRITEPROTECT,NO_WRITEPROTECT);
}

void test_userAreaRWProtection_given_only_area_1_protect(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x01};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2CSS, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  userAreaRWProtection(password,AREA1_WRITEPROTECT,NO_WRITEPROTECT,NO_WRITEPROTECT,NO_WRITEPROTECT);
}

void test_userAreaRWProtection_given_only_area_2_protect(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x04};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2CSS, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  userAreaRWProtection(password,NO_WRITEPROTECT,AREA2_WRITEPROTECT,NO_WRITEPROTECT,NO_WRITEPROTECT);
}

void test_userAreaRWProtection_given_only_area_3_protect(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x10};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2CSS, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  userAreaRWProtection(password,NO_WRITEPROTECT,NO_WRITEPROTECT,AREA3_WRITEPROTECT,NO_WRITEPROTECT);
}

void test_userAreaRWProtection_given_only_area_4_protect(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x40};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2CSS, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  userAreaRWProtection(password,NO_WRITEPROTECT,NO_WRITEPROTECT,NO_WRITEPROTECT,AREA4_WRITEPROTECT);
}

void test_userAreaRWProtection_given_only_area_1_and_area_2_protect(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x05};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2CSS, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  userAreaRWProtection(password,AREA1_WRITEPROTECT,AREA2_WRITEPROTECT,NO_WRITEPROTECT,NO_WRITEPROTECT);
}

void test_userAreaRWProtection_given_area_1_and_area_2_and_area_4_protect(void){

  uint8_t password[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectUnlock[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t expectLock[17] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t expect[1] = {0x45};

  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectUnlock,17,17);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2CSS, expect,1,1);
  I2CWrite_ExpectWithArray(NFC_SYSTEMMEMORY, I2C_PWD, expectLock,17,17);

  userAreaRWProtection(password,AREA1_WRITEPROTECT,AREA2_WRITEPROTECT,NO_WRITEPROTECT,AREA4_WRITEPROTECT);
}
