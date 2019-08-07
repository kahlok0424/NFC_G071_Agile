/*
 * ndef.c
 *
 *  Created on: 25 Jul 2019
 *      Author: KahLok
 */
#include "ndef.h"
#include "nfc.h"
#include <stdio.h>
#include <string.h>

/*
 * @brief Ndef area has to be multiple of 8bytes
 */
uint16_t writeT5TCCFile(ADDRESSING_MODE address_mode, uint16_t ndef_area){

	uint8_t buffer[4];
	buffer[0] = address_mode;

	if( (ndef_area%8) != 0 || ndef_area > 512){
		return NDEF_ERROR;
	}
	else{
		buffer[2] = ndef_area/8;
		//byte 1 = NFC forum tag type V version , v 1.0
		buffer[1] = NFCT5_VERSION_V1_0;
		//byte 2 = additional features, Multiple block read command = 0x01
		buffer[3] = 0x01;
		writeUserMemory(0x00,buffer,4);
		return NDEF_OK;
	}

}

uint8_t *writeURI(char *protocol, char *link, char *tittle){

	static uint8_t ndef[200];
	uint16_t uriType;
	uint32_t uriSize,tittleSize,totalSize, index =0;

	/* An URI can be included in a smart poster to add text to give instruction to user for instance */
	/*
	 *  RECORD HEADER | RECORD PAYLOAD
	 *  -------------------------------
	 *  RECORD HEADER = FLAGS | TYPE LENGTH | PAYLOAD LENGTH x4 | ID LENGTH | PAYLOAD TYPE | PAYLOAD ID
	 *  FLAGS = MB | ME | CF | SR | ID LENGTH | TNF ( Well Known type = 0x01)
	 *  PAY LOAD TYPE = "U": URI 0x55, "T": test 0x54, "Sp": smart poster 0x5370
	 */

	uriType = getURIProtocol(protocol);

	 if( uriType != URI_ERROR )
	   uriSize = 1 + strlen(link);
	 else /*: 1+protocol+URI else*/
	   uriSize = 1 + strlen(protocol) + strlen(link);

	 /*Check if smart poster type is needed*/
	 if(tittle[0] != '\0'){
		 tittleSize = 1 + 2 + strlen(tittle);
		 totalSize = 4 + uriSize + 4 + tittleSize;

		 //Smart Poster Header
		 if(totalSize > 255){
			 ndef[index++] = 0xC1;
			 ndef[index++] = SMART_POSTER_TYPE_LENGTH;
			 ndef[index++] = (totalSize & 0xFF000000) >> 24;
			 ndef[index++] = (totalSize & 0x00FF0000) >> 16;
			 ndef[index++] = (totalSize & 0x0000FF00) >> 8;
			 ndef[index++] = totalSize & 0x000000FF;

		 }
		 else{
			 ndef[index++] = 0xD1;
			 ndef[index++] = SMART_POSTER_TYPE_LENGTH;
			 ndef[index++] = (uint8_t)totalSize;
		 }
		 memcpy(&ndef[index] , SMART_POSTER_TYPE,SMART_POSTER_TYPE_LENGTH);
		 index += SMART_POSTER_TYPE_LENGTH;
	 }

	  /* URI header */
	  ndef[index] = 0x81;
	  if( uriSize < 256 ) ndef[index] |= 0x10;                      // Set the SR bit
	  if( tittle[0] == '\0' ) ndef[index] |= 0x40;       // Set the ME bit
	  index++;

	  ndef[index++] = URI_TYPE_LENGTH;
	  if( uriSize > 255 )
	  {
	    ndef[index++] = (uriSize & 0xFF000000) >> 24;
	    ndef[index++] = (uriSize & 0x00FF0000) >> 16;
	    ndef[index++] = (uriSize & 0x0000FF00) >> 8;
	    ndef[index++] = uriSize & 0x000000FF;
	  }
	  else
	  {
		  ndef[index++] = (uint8_t)uriSize;
	  }
	  memcpy( &ndef[index], URI_TYPE, URI_TYPE_LENGTH );
	  index += URI_TYPE_LENGTH;

	  ndef[index++] = uriType;	//the URI identification code of protocol
	  memcpy( &ndef[index], link, strlen(link) );
	  index += strlen(link);

	  //Information header
	  if(tittle[0] != '\0'){
		  if(tittleSize> 255){
			  ndef[index++] = 0x41;
			  ndef[index++] = TEXT_TYPE_LENGTH;
			  ndef[index++] = (tittleSize & 0xFF000000) >> 24;
			  ndef[index++] = (tittleSize & 0x00FF0000) >> 16;
			  ndef[index++] = (tittleSize & 0x0000FF00) >> 8;
			  ndef[index++] = tittleSize & 0x000000FF;
		  }
		  else{
			  ndef[index++] = 0x51;
			  ndef[index++] = TEXT_TYPE_LENGTH;
			  ndef[index++] = tittleSize;
		  }

		  memcpy( &ndef[index], TEXT_TYPE, TEXT_TYPE_LENGTH );
		  index+=TEXT_TYPE_LENGTH;
		  ndef[index++] = ISO_ENGLISH_CODE_LENGTH; /* UTF-8 with x byte language code */
		  memcpy( &ndef[index], ISO_ENGLISH_CODE, ISO_ENGLISH_CODE_LENGTH );
		  index += ISO_ENGLISH_CODE_LENGTH;

		  //Information payload
		  memcpy( &ndef[index], tittle, strlen(tittle) );
		  index+= strlen(tittle);
	  }


	 return ndef;
}

uint16_t getURIProtocol(char *protocol){

	  if( !memcmp( protocol, URI_0x01_STRING, strlen(URI_0x01_STRING) ) ) return URI_0x01;
	  else if( !memcmp( protocol, URI_0x02_STRING, strlen(URI_0x02_STRING) ) ) return URI_0x02;
	  else if( !memcmp( protocol, URI_0x03_STRING, strlen(URI_0x03_STRING) ) ) return URI_0x03;
	  else if( !memcmp( protocol, URI_0x04_STRING, strlen(URI_0x04_STRING) ) ) return URI_0x04;
	  else if( !memcmp( protocol, URI_0x05_STRING, strlen(URI_0x05_STRING) ) ) return URI_0x05;
	  else if( !memcmp( protocol, URI_0x06_STRING, strlen(URI_0x06_STRING) ) ) return URI_0x06;
	  else if( !memcmp( protocol, URI_0x07_STRING, strlen(URI_0x07_STRING) ) ) return URI_0x07;
	  else if( !memcmp( protocol, URI_0x08_STRING, strlen(URI_0x08_STRING) ) ) return URI_0x08;
	  else if( !memcmp( protocol, URI_0x09_STRING, strlen(URI_0x09_STRING) ) ) return URI_0x09;
	  else if( !memcmp( protocol, URI_0x0A_STRING, strlen(URI_0x0A_STRING) ) ) return URI_0x0A;
	  else if( !memcmp( protocol, URI_0x0B_STRING, strlen(URI_0x0B_STRING) ) ) return URI_0x0B;
	  else if( !memcmp( protocol, URI_0x0C_STRING, strlen(URI_0x0C_STRING) ) ) return URI_0x0C;
	  else if( !memcmp( protocol, URI_0x0D_STRING, strlen(URI_0x0D_STRING) ) ) return URI_0x0D;
	  else if( !memcmp( protocol, URI_0x0E_STRING, strlen(URI_0x0E_STRING) ) ) return URI_0x0E;
	  else if( !memcmp( protocol, URI_0x0F_STRING, strlen(URI_0x0F_STRING) ) ) return URI_0x0F;
	  else if( !memcmp( protocol, URI_0x10_STRING, strlen(URI_0x10_STRING) ) ) return URI_0x10;
	  else if( !memcmp( protocol, URI_0x11_STRING, strlen(URI_0x11_STRING) ) ) return URI_0x11;
	  else if( !memcmp( protocol, URI_0x12_STRING, strlen(URI_0x12_STRING) ) ) return URI_0x12;
	  else if( !memcmp( protocol, URI_0x13_STRING, strlen(URI_0x13_STRING) ) ) return URI_0x13;
	  else if( !memcmp( protocol, URI_0x14_STRING, strlen(URI_0x14_STRING) ) ) return URI_0x14;
	  else if( !memcmp( protocol, URI_0x15_STRING, strlen(URI_0x15_STRING) ) ) return URI_0x15;
	  else if( !memcmp( protocol, URI_0x16_STRING, strlen(URI_0x16_STRING) ) ) return URI_0x16;
	  else if( !memcmp( protocol, URI_0x17_STRING, strlen(URI_0x17_STRING) ) ) return URI_0x17;
	  else if( !memcmp( protocol, URI_0x18_STRING, strlen(URI_0x18_STRING) ) ) return URI_0x18;
	  else if( !memcmp( protocol, URI_0x19_STRING, strlen(URI_0x19_STRING) ) ) return URI_0x19;
	  else if( !memcmp( protocol, URI_0x1A_STRING, strlen(URI_0x1A_STRING) ) ) return URI_0x1A;
	  else if( !memcmp( protocol, URI_0x1B_STRING, strlen(URI_0x1B_STRING) ) ) return URI_0x1B;
	  else if( !memcmp( protocol, URI_0x1C_STRING, strlen(URI_0x1C_STRING) ) ) return URI_0x1C;
	  else if( !memcmp( protocol, URI_0x1D_STRING, strlen(URI_0x1D_STRING) ) ) return URI_0x1D;
	  else if( !memcmp( protocol, URI_0x1E_STRING, strlen(URI_0x1E_STRING) ) ) return URI_0x1E;
	  else if( !memcmp( protocol, URI_0x1F_STRING, strlen(URI_0x1F_STRING) ) ) return URI_0x1F;
	  else if( !memcmp( protocol, URI_0x20_STRING, strlen(URI_0x20_STRING) ) ) return URI_0x20;
	  else if( !memcmp( protocol, URI_0x21_STRING, strlen(URI_0x21_STRING) ) ) return URI_0x21;
	  else if( !memcmp( protocol, URI_0x22_STRING, strlen(URI_0x22_STRING) ) ) return URI_0x22;
	  else if( !memcmp( protocol, URI_0x23_STRING, strlen(URI_0x23_STRING) ) ) return URI_0x23;
	  else return URI_ERROR;

}
