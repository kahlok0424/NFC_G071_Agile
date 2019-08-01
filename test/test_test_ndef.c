#include "unity.h"
#include "ndef.h"
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

void test_ndef_writeT5TCCFile_given_area_400_expect_correct(void)
{
  uint8_t expect[4] = {0xe1,0x40,0x32,0x01};

  I2CWrite_ExpectWithArray(NFC_USERMEMORY, 0x00, expect,4,4);

  writeT5TCCFile(ONE_BYTE_ADDRESSING,400);
}

void test_ndef_writeT5TCCFile_given_two_byte_addressing_expect_correct(void)
{
  uint8_t expect[4] = {0xe2,0x40,0x08,0x01};

  I2CWrite_ExpectWithArray(NFC_USERMEMORY, 0x00, expect,4,4);

  writeT5TCCFile(TWO_BYTE_ADDRESSING,64);
}

void test_ndef_writeT5TCCFile_given_600_bytes_expect_return_ndef_error(void)
{
  uint8_t expect[4] = {0xe2,0x40,0x40,0x01};
  uint16_t state;

  TEST_ASSERT_EQUAL(NDEF_ERROR,writeT5TCCFile(ONE_BYTE_ADDRESSING,600));
}

void test_ndef_writeT5TCCFile_given_FULL_MEMORY_expect_return_ndef_ok(void)
{
  uint8_t expect[4] = {0xe1,0x40,0x40,0x01};
  uint16_t state;
  I2CWrite_ExpectWithArray(NFC_USERMEMORY, 0x00, expect,4,4);

  TEST_ASSERT_EQUAL(NDEF_OK,writeT5TCCFile(ONE_BYTE_ADDRESSING,FULL_MEMORY));
}

void test_ndef_getURIProtocol_given_http_www_expect_return_uri_1(void)
{
  char protocol[20] = "http://www.";

  TEST_ASSERT_EQUAL(URI_0x01,getURIProtocol(protocol));
}

void test_ndef_getURIProtocol_given_https_expect_return_uri_4(void)
{
  char protocol[20] = "https://";

  TEST_ASSERT_EQUAL(URI_0x04,getURIProtocol(protocol));
}

void test_ndef_getURIProtocol_given_www_expect_return_uri_error(void)
{
  char protocol[20] = "www://";

  TEST_ASSERT_EQUAL(URI_ERROR,getURIProtocol(protocol));
}
