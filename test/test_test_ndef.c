#include "unity.h"
#include "ndef.h"
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

void test_ndef_writeT5TCCFile_given_area_400_expect_correct(void)
{
  uint8_t expect[4] = {0xe1,0x40,0x32,0x01};

  I2CWrite_ExpectWithArray(0xa6, 0x00, expect,4,4);

  writeT5TCCFile(ONE_BYTE_ADDRESSING,400);
}

void test_ndef_writeT5TCCFile_given_two_byte_addressing_expect_correct(void)
{
  uint8_t expect[4] = {0xe2,0x40,0x40,0x01};

  I2CWrite_ExpectWithArray(0xa6, 0x00, expect,4,4);

  writeT5TCCFile(TWO_BYTE_ADDRESSING,FULL_USER_MEMORY);
}

void test_ndef_writeT5TCCFile_given_600_bytes_expect_return_ndef_error(void)
{
  uint8_t expect[4] = {0xe2,0x40,0x40,0x01};
  uint16_t state;
  //I2CWrite_ExpectWithArray(0xa6, 0x00, expect,4,4);


  TEST_ASSERT_EQUAL(NDEF_ERROR,writeT5TCCFile(ONE_BYTE_ADDRESSING,600));
}
