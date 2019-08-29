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

- Complete functions to configure NFC Tag
- Uses STM32 HAL Library
- Generate NDEF Messages and write into Tag.
- Able to tell the android phone what to do when tap on the NFC tag. (etc. write email, call someone, open up application....)

## Functions and how to use.





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


