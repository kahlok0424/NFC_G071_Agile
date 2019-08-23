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
  uint8_t expect[4] = {0xe1,0x40,0x32,0x00};

  I2CWrite_ExpectWithArray(NFC_USERMEMORY, 0x00, expect,4,4);

  writeT5TCCFile(ONE_BYTE_ADDRESSING,400);
}

void test_ndef_writeT5TCCFile_given_two_byte_addressing_expect_correct(void)
{
  uint8_t expect[4] = {0xe2,0x40,0x08,0x00};

  I2CWrite_ExpectWithArray(NFC_USERMEMORY, 0x00, expect,4,4);

  writeT5TCCFile(TWO_BYTE_ADDRESSING,64);
}

void test_ndef_writeT5TCCFile_given_600_bytes_expect_return_ndef_error(void)
{
  uint8_t expect[4] = {0xe2,0x40,0x40,0x00};
  uint16_t state;

  TEST_ASSERT_EQUAL(NDEF_ERROR,writeT5TCCFile(ONE_BYTE_ADDRESSING,600));
}

void test_ndef_writeT5TCCFile_given_FULL_MEMORY_expect_return_ndef_ok(void)
{
  uint8_t expect[4] = {0xe1,0x40,0x40,0x00};
  uint16_t state;
  I2CWrite_ExpectWithArray(NFC_USERMEMORY, 0x00, expect,4,4);

  TEST_ASSERT_EQUAL(NDEF_OK,writeT5TCCFile(ONE_BYTE_ADDRESSING,FULL_MEMORY));
}

void test_ndef_getURIProtocol_given_http_www_expect_return_uri_1(void)
{
  char protocol[20] = "http://www.";

  TEST_ASSERT_EQUAL(URI_0x01,getUriProtocol(protocol));
}

void test_ndef_getURIProtocol_given_https_expect_return_uri_4(void)
{
  char protocol[20] = "https://";

  TEST_ASSERT_EQUAL(URI_0x04,getUriProtocol(protocol));
}

void test_ndef_getURIProtocol_given_www_expect_return_uri_error(void)
{
  char protocol[20] = "www://";

  TEST_ASSERT_EQUAL(URI_ERROR,getUriProtocol(protocol));
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
  size = generateUriNdef(protocol,link,info,ndef);

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
  size = generateUriNdef(protocol,link,info,ndef);

  TEST_ASSERT_EQUAL_UINT16(15,size);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(expected,ndef,15);
}

void test_ndef_generateURINdef_given_www_youtube_com_tittle_expect_return_ndef_message(void)
{
  char protocol[20] = "https://www.";
  char link[50] = "youtube.com";
  char info[10] = "Video Web";
  uint8_t expected[37] = {0xD1,0x02,0x20,0x53,0x70,0x91,0x01,0x0c,0x55,0x02,0x79,
    0x6f,0x75,0x74,0x75,0x62,0x65,0x2e,0x63,0x6f,0x6d,0x51,0x01,0x0c,0x54,0x02,0x65,0x6e,
    0x56,0x69,0x64,0x65,0x6f,0x20,0x57,0x65,0x62};
  uint8_t *result;
  uint8_t ndef[200];
  uint16_t size;
  size = generateUriNdef(protocol,link,info,ndef);

  TEST_ASSERT_EQUAL_UINT16(37,size);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(expected,ndef,37);
}

void xtest_ndef_generateUriNdef_given_file_path_expect_return_ndef_message(void)
{
  char protocol[30] = "file://";
  char link[150] = "/storage/emulated/0/bluetooth/Screenshot_2015-04-15-15-01-58.png";
  char info[30] = "";
  uint8_t expected[31] = {0xD1,0x01,0x41,0x55,29,0x67,0x6f,0x6f,0x67,0x6c,0x65,
    0x2e,0x63,0x6f,0x6d};
  uint8_t *result;
  uint8_t ndef[200];
  uint16_t size;
  size = generateUriNdef(protocol,link,info,ndef);

  TEST_ASSERT_EQUAL_UINT16(69,size);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(expected,ndef,15);
}

void test_ndef_generateMailtoNdef_given_kahlok0424_gmail_subtest_test_body_nfcTest(void)
{
  char email[30] = "kahlok0424@gmail.com";
  char subject[10] = "test";
  char body[50] = "nfc testing 123";
  uint8_t expected[70] = {0xD1,0x01,0x3e,0x55,0x00,0x6d,0x61,0x69,0x6c,0x74,0x6f,
    0x3a,0x6b,0x61,0x68,0x6c,0x6f,0x6b,0x30,0x34,0x32,0x34,0x40,0x67,0x6d,0x61,0x69,0x6c,
    0x2e,0x63,0x6f,0x6d,0x3f,0x73,0x75,0x62,0x6a,0x65,0x63,0x74,0x3d,0x74,0x65,0x73,
    0x74,0x26,0x62,0x6f,0x64,0x79,0x3d,0x6e,0x66,0x63,0x20,0x74};
  uint8_t *result;
  uint8_t ndef[200];
  uint16_t size;
  size = generateMailtoNdef(email,subject,body,ndef);

  TEST_ASSERT_EQUAL_UINT16(66,size);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(expected,ndef,56);
}

void test_ndef_writeT5TLVBlock_given_tag5_ndef_format_expect_return_2(void)
{
  uint16_t size;
  uint8_t expect[4] = {0x03,25};

  I2CWrite_ExpectWithArray(NFC_USERMEMORY, 0x04, expect,2,2);

  size = writeT5TLVBlock(NFC_TAG5_TLV_NDEF_MSG,25);

  TEST_ASSERT_EQUAL_UINT16(2,size);
}
