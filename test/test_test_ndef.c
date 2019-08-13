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

void test_ndef_generateURINdef_given_www_google_com_expect_return_ndef_message(void)
{
  char protocol[20] = "https://www.";
  char link[50] = "google.com";
  char info[10] = "abc";
  uint8_t expected[31] = {0xD1,0x02,0x19,0x53,0x70,0x91,0x01,0x0b,0x55,0x02,0x67,0x6f,0x6f,0x67,0x6c,0x65,
    0x2e,0x63,0x6f,0x6d,0x51,0x01,0x06,0x54,0x02,0x65,0x6e,0x61,0x62,0x63};
  uint8_t *result;
  uint8_t ndef[200];
  uint16_t size;
  size = generateURINdef(protocol,link,info,ndef);

  TEST_ASSERT_EQUAL_UINT16(30,size);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(expected,ndef,30);
}

void test_ndef_generateURINdef_given_www_google_com_no_tittle_expect_return_ndef_message(void)
{
  char protocol[20] = "https://www.";
  char link[50] = "google.com";
  char info[10] = "\0";
  uint8_t expected[31] = {0xD1,0x01,0x0b,0x55,0x02,0x67,0x6f,0x6f,0x67,0x6c,0x65,
    0x2e,0x63,0x6f,0x6d};
  uint8_t *result;
  uint8_t ndef[200];
  uint16_t size;
  size = generateURINdef(protocol,link,info,ndef);

  TEST_ASSERT_EQUAL_UINT16(15,size);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(expected,ndef,15);
}

void test_ndef_generateURINdef_given_www_youtube_com_tittle_expect_return_ndef_message(void)
{
  char protocol[20] = "https://www.";
  char link[50] = "youtube.com";
  char info[10] = "Video Web";
  uint8_t expected[31] = {0xD1,0x02,0x0b,0x55,0x02,0x67,0x6f,0x6f,0x67,0x6c,0x65,
    0x2e,0x63,0x6f,0x6d};
  uint8_t *result;
  uint8_t ndef[200];
  uint16_t size;
  size = generateURINdef(protocol,link,info,ndef);

  TEST_ASSERT_EQUAL_UINT16(37,size);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(expected,ndef,15);
}

void xtest_ndef_generateURINdef_given_file_path_expect_return_ndef_message(void)
{
  char protocol[30] = "file://";
  char link[150] = "/storage/emulated/0/bluetooth/Screenshot_2015-04-15-15-01-58.png";
  char info[30] = "";
  uint8_t expected[31] = {0xD1,0x01,0x41,0x55,29,0x67,0x6f,0x6f,0x67,0x6c,0x65,
    0x2e,0x63,0x6f,0x6d};
  uint8_t *result;
  uint8_t ndef[200];
  uint16_t size;
  size = generateURINdef(protocol,link,info,ndef);

  TEST_ASSERT_EQUAL_UINT16(69,size);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(expected,ndef,15);
}

void test_ndef_writeT5TLVBlock_given_tag5_ndef_format_expect_return_2(void)
{
  uint16_t size;
  uint8_t expect[4] = {0x03,25};

  I2CWrite_ExpectWithArray(NFC_USERMEMORY, 0x04, expect,2,2);

  size = writeT5TLVBlock(NFC_TAG5_TLV_NDEF_MSG,25);

  TEST_ASSERT_EQUAL_UINT16(2,size);
}
