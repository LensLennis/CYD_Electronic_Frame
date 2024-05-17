/**
 * @file Touchscreen_TFT_Display.ino
 * @brief This file contains the code to interface with a TFT display and XPT2046 touchscreen using SPI communication.
 * @author Lena Lorenzo
 * Credits to: Rui Santos & Sara Santos
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 */

#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

TFT_eSPI tft = TFT_eSPI();  /**< TFT display object */

// Touchscreen pins
#define XPT2046_IRQ 36   /**< Touchscreen IRQ pin */
#define XPT2046_MOSI 32  /**< Touchscreen MOSI pin */
#define XPT2046_MISO 39  /**< Touchscreen MISO pin */
#define XPT2046_CLK 25   /**< Touchscreen clock pin */
#define XPT2046_CS 33    /**< Touchscreen chip select pin */

SPIClass touchscreenSPI = SPIClass(VSPI);  /**< SPI object for touchscreen */
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);  /**< Touchscreen object */

#define SCREEN_WIDTH 320   /**< Width of the TFT screen */
#define SCREEN_HEIGHT 240  /**< Height of the TFT screen */
#define FONT_SIZE 2        /**< Font size for text display */

// Touchscreen coordinates: (x, y) and pressure (z)
int x, y, z;

/**
 * @brief Print touchscreen information to the Serial Monitor.
 * 
 * This function prints the X, Y coordinates and the pressure (Z) of the touchscreen
 * to the Serial Monitor.
 * 
 * @param touchX The X coordinate of the touch
 * @param touchY The Y coordinate of the touch
 * @param touchZ The pressure of the touch
 */
void printTouchToSerial(int touchX, int touchY, int touchZ) {
  Serial.print("X = ");
  Serial.print(touchX);
  Serial.print(" | Y = ");
  Serial.print(touchY);
  Serial.print(" | Pressure = ");
  Serial.print(touchZ);
  Serial.println();
}

/**
 * @brief Print touchscreen information to the TFT display.
 * 
 * This function prints the X, Y coordinates and the pressure (Z) of the touchscreen
 * to the TFT display.
 * 
 * @param touchX The X coordinate of the touch
 * @param touchY The Y coordinate of the touch
 * @param touchZ The pressure of the touch
 */
void printTouchToDisplay(int touchX, int touchY, int touchZ) {
  // Clear TFT screen
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);

  int centerX = SCREEN_WIDTH / 2;
  int textY = 80;

  String tempText = "X = " + String(touchX);
  tft.drawCentreString(tempText, centerX, textY, FONT_SIZE);

  textY += 20;
  tempText = "Y = " + String(touchY);
  tft.drawCentreString(tempText, centerX, textY, FONT_SIZE);

  textY += 20;
  tempText = "Pressure = " + String(touchZ);
  tft.drawCentreString(tempText, centerX, textY, FONT_SIZE);
}

/**
 * @brief Setup function initializes serial communication, SPI for touchscreen, and TFT display.
 * 
 * This function is called once when the microcontroller starts.
 * It initializes serial communication at 115200 baud, SPI communication for the touchscreen,
 * and sets up the TFT display. It also displays a welcome message on the TFT screen.
 */
void setup() {
  Serial.begin(115200);

  // Start the SPI for the touchscreen and init the touchscreen
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  // Set the touchscreen rotation in landscape mode
  // Note: in some displays, the touchscreen might be upside down, so you might need to set the rotation to 3: touchscreen.setRotation(3);
  touchscreen.setRotation(1);

  // Start the TFT display
  tft.init();
  // Set the TFT display rotation in landscape mode
  tft.setRotation(1);

  // Clear the screen before writing to it
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);

  // Set X and Y coordinates for center of display
  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;

  tft.drawCentreString("Hello, world!", centerX, 30, FONT_SIZE);
  tft.drawCentreString("Touch screen to test", centerX, centerY, FONT_SIZE);
}

/**
 * @brief Main loop function checks for touchscreen touches and displays coordinates and pressure.
 * 
 * This function runs repeatedly, checking if the touchscreen is touched.
 * If touched, it retrieves the touch coordinates and pressure, maps them to the screen dimensions,
 * and prints the values to the serial monitor and TFT display.
 */
void loop() {
  // Checks if touchscreen was touched, and prints X, Y and Pressure (Z) info on the TFT display and Serial Monitor
  if (touchscreen.tirqTouched() && touchscreen.touched()) {
    // Get touchscreen points
    TS_Point p = touchscreen.getPoint();
    // Calibrate touchscreen points with map function to the correct width and height
    x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    z = p.z;

    printTouchToSerial(x, y, z);
    printTouchToDisplay(x, y, z);

    delay(100);
  }
}
