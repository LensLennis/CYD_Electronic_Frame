# CYD instructions

The CYD is a cheap TFT Touchscreen display LCD with an ESP32 development board included; it is a great option for building GUI for IoT projects.

## Communication protocol

The ESP32 communicates with the TFT Display and Touchscreen using the SPI communication protocol.

## Prepare User_Setup.h Config File for TFT_eSPI Library

To properly use the TFT_eSPI library, you need a configuration file called User_Setup.h with the right definitions. I’ve already prepared the file so that you don’t have any configuration issues.
You just need to download it and move it to the right folder: go to the Arduino folder, open the libraries, open TFT_eSPI, and overwrite the existing file UserSetup.h.

Libraries:

#include <TFT_eSPI.h> /**< Library for interfacing with LCD displays */
#include <XPT2046_Bitbang.h> /**< Library for interfacing with the touch screen */
#include <SPI.h> /**< Library for SPI communication
#include <SdFat.h> /**< Library for accessing SD cards */
#include <JPEGDEC.h> /**< Library for decoding JPGs */

