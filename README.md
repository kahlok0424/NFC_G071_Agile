# NFC_EMULATOR

The objective of this project is to develop a C library to interface with the NFC emulator 
ST25DV04K chip through I2C interface. The project is later
included with a C library to write NDEF messages into the tag and android phone user is able to retrieve the message from the tag.

## Getting Started

A few compronents are needed in this project. 

### Hardwares

 - XNucleo-NFC04A1 (ST25DV04K based) as NFC Tag
 - STM32G071RB as MCU

### Software
 - STM32CubeIDE V1.0.1

### Installing

Due to the return of hardware components, I can't provide a picture guide on how to attach the NFC Tag onto the MCU board. But I will attach it in  the future if I have the chance.
In fact, it is simple, you will just need to locate the pin of NFC tag and find the matching pattern on the MCU board and just plug on it.
Be careful not to bend the pin legs.

You can download the IDE from this website [Download](https://www.st.com/en/development-tools/stm32cubeide.html) 
and begin with the installing of software.

After the installation is done, you can clone this project into your workspace by following this few steps.
1. Go to your workspace, and open `Git Bash`
2. Type this command and enter
```
git clone https://github.com/kahlok0424/NFC_G071_Agile.git
```
3. Open your STM32CubeIDE and add the project into your CubeIde.
4. Connect the STM32G071 nucleo board to your pc.
(Make sure to have the ST-Link driver, you can get it [here](https://www.st.com/en/evaluation-tools/nucleo-g071rb.html#tools-software)
5. Compile
6. Use your phone and go to Google play store to download this [application](https://play.google.com/store/apps/details?id=com.st.st25nfc&hl=en).
 This app is provided by STMicroelectronics.
 7. Open up the app and now you can tag around to read or write the NFC tag.

## Features

- Functions to configure NFC Tag
- Uses STM32 HAL Library
- Generate NDEF Messages and write into Tag.
- Able to tell the android phone what to do when tap on the NFC tag. (etc. write email, call someone, open up application....)
- Partialy working Fast Transfer Mode
- External interrupt
- Energy Harvesting

## Functions and Usage

### Initialize the NFC tag

```c
void initNFC(uint16_t devAddress);
```

Using this function will initialize the NFC tag by checking is the NFC tag available or not, it yes , the leds on NFC tag will all light up, else it will not light up.

### Read and Write into the NFC tag

```c
void readSystemMemory(uint16_t regAddress, uint8_t *buffer,int n);
void writeSystemMemory(uint16_t regAddress,uint8_t *password, uint8_t data);
void readDynamicReg(uint16_t regAddress, uint8_t *buffer);
void writeDynamicReg(uint16_t regAddress, uint8_t data);
void readUserMemory(uint16_t address, uint8_t *data, uint16_t n);
void writeUserMemory(uint16_t address, uint8_t *data, uint16_t n);
```

There are several functions to read or write into the NFC tag, each with different purpose and the purpose is clear by reading the function name.

### Unlock or lock I2C security session of NFC tag and Change I2C password
```c
void unlockI2CSecurity(uint8_t *password);
void lockI2CSecurity();
void changeI2CPassword(uint8_t *oldPass, uint8_t *newPassword);
```
By factory setting, the default password of this NFC TAG (ST25DV04K) is 0x00000000.

### Configure the NFC Tag
```c
void NFC04A1_setRFMode(uint8_t *password, RF_MODE mode);
void NFC04A1_setRFModeDyn(RF_MODE mode);
void enableInterrupt(uint8_t *password);
void disableInterrupt(uint8_t *password);
void configureInterrupt(uint8_t *password, INTERRUPT_MODE mode);
void enableEnergyHarvest(uint8_t *password);
void disableEnergyHarvest(uint8_t *password);
```
The function above are to configure the NFC tag's extra features. More details of what the function do is written in the source code.

### Configure User Area
```c
int  validateArea(uint8_t area1, uint8_t area2, uint8_t area3);
void setDefaultArea(uint8_t *password);
void setArea(uint8_t *password, int area1, int area2, int area3);
```
The user area of st25dv04k can be separated into areas (maximum are 4). Each area can have different size but the total size of all area must be 512bytes and size of each area has to be multiple of 32bytes.
- Example
```c
/* Code to set into 3 areas */
void setArea(password, 64, 32, NA);
/* This code will set the user area into 3, which area1 is 64bytes, area2 is 32bytes, and area 3 is the rest of the user memory( 512 - 64 - 32 = 416). Size of area 3 is 416bytes. */
```

The user area can also be write protected using this function:
```c
void i2CWriteProtectUserArea(uint8_t *password, I2CWRITEPROTECT area);
void rfWriteProtectUserArea(uint8_t *password, RFWRITEPROTECT area1, RFWRITEPROTECT area2, RFWRITEPROTECT area3, RFWRITEPROTECT area4);
```
If the area is write protected, it can only be written when the I2C security session is opened.

### Fast Transfer Mode (Mail Box)
```c
void enableMailBox(uint8_t *password);
void disableMailBox(uint8_t *password);
void setMailBoxTimeout(uint8_t *password, uint8_t wdgTime);
void resetMailBox();
void configFastTransferMode(uint8_t *password, FTM_MODE mode, uint8_t wdgTime);
void readDatafromMailbox(uint16_t address, uint8_t *data, int n); /* needed to improve */
void writeDatatoMailbox(uint8_t *data, int n); /*needed to improve */
void getMailBoxStatus(uint8_t *status);
```
The functions above are to setup the mailbox of this NTF Tag. The mailbox has 256bytes of memory and is not using the EEPROM.
The fast transfer mode allow android user to direct take data from the mailbox, but however this tag is designed in such a way that andoid user need to send out a mailbox read command throught radio frequency in order to access the mailbox. Currently only the app (st25) developed by STMicroelectronics can send the command. This FTM is not applicable for other NFC tag read applications.

## NDEF
```c
uint16_t writeT5TCCFile(ADDRESSING_MODE address_mode, uint16_t ndef_area);
uint8_t writeT5TLVBlock(TAG5_TLV type,uint16_t length);
uint16_t getUriProtocol(char *protocol);
uint16_t generateUriNdef(char *protocol, char *link, char *infomation, uint8_t *ndef);
uint16_t generateMailtoNdef(char *email, char *subject, char *body, uint8_t *ndef);
uint16_t generateLaunchAppNdef(char *app, uint8_t *ndef);
```
The function above are used to build out the functions to write into the NFC Tag. Where the CC file and T5TLV block are the needed block to descripe the format of the NDEF message, so that the android phone knows how to decode the NDEF message. These format are published by the NFC Forum [link](https://nfc-forum.org/) and the tag we are using here is NFC FORUM TYPE 5 TAG.  

```c
void writeUri(char *protocol, char *link, char *tittle);
void writeMailto(char *email, char *subject, char *body);
void writeLaunchApp(char *app);
```
These functions will encode the message that user wants to tell the android phone throught NFC tapping into a NDEF formatted messages and write into the NFC Tag.

- Example
```c
  /* ndef message array of mailto: */
  char email[30] = "kahlok0424@gmail.com";
  char subject[10] = "Test";
  char body[65] = "hello world we are the world, test sending email with content";
  
  writeMailto(email,subject,body);
```
This example will write the encoded NDEF message into the tag and when is tapped by NFC-enabled android phone, it will bring out the email app with written content as shwon below:


## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone whose code was used
* Inspiration
* etc


