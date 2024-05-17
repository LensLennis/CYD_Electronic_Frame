/**
 * @file SD_Card_Init.ino
 * @brief This file contains the code to initialize and interact with an SD card using SPI communication.
 * @author Lena Lorenzo
 */

#define SD_MOSI 23  /**< Master Out Slave In (MOSI) pin for SD card communication */
#define SD_MISO 19  /**< Master In Slave Out (MISO) pin for SD card communication */
#define SD_SCK 18   /**< Serial Clock (SCK) pin for SD card communication */
#define SD_CS 5     /**< Chip Select (CS) pin for SD card communication */

/**
 * @brief Setup function initializes serial communication and the SD card.
 * 
 * This function is called once when the microcontroller starts.
 * It initializes serial communication at 9600 baud and attempts to initialize the SD card.
 */
void setup() {
  Serial.begin(9600); 
  // SD initialization
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
  delay(100);
  if (SD_init() == 1) {
    Serial.println("Card Mount Failed");
  } else {
    Serial.println("initialize SD Card successfully");
  }
}

/**
 * @brief Initialize the SD card.
 * 
 * This function attempts to initialize the SD card and checks for its type and size.
 * It also lists the contents of the root directory.
 * 
 * @return int Returns 1 if initialization fails, 0 if successful.
 */
int SD_init() {
  if (!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return 1;
  }
  uint8_t cardType = SD.cardType();
  
  if (cardType == CARD_NONE) {
    Serial.println("No TF card attached");
    return 1;
  }
  
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("TF Card Size: %lluMB\n", cardSize);
  listDir(SD, "/", 2);

  listDir(SD, "/", 0);
  //createDir(SD, "/mydir");
  listDir(SD, "/", 0);
  // removeDir(SD, "/mydir");
  listDir(SD, "/", 2);
  // writeFile(SD, "/hello.txt", "Hello ");
  //appendFile(SD, "/hello.txt", "World!\n");
  //readFile(SD, "/hello.txt");
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
  Serial.println("SD init over.");
  
  return 0;
}

/**
 * @brief List directory contents.
 * 
 * This function lists the contents of a directory on the SD card, including nested directories up to a specified level.
 * 
 * @param fs Filesystem object representing the SD card.
 * @param dirname Path to the directory to list.
 * @param levels Levels of subdirectories to list (0 for none, higher values for deeper levels).
 */
void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  // Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    // Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }
  
  File file = root.openNextFile();
  // i = 0;
  while (file) {
    if (file.isDirectory()) {
      // Serial.print("  DIR : ");
      // Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("FILE: ");
      Serial.print(file.name());
      // lcd.setCursor(0, 2 * i);
      // lcd.printf("FILE:%s", file.name());

      Serial.print("SIZE: ");
      Serial.println(file.size());
      // lcd.setCursor(180, 2 * i);
      // lcd.printf("SIZE:%d", file.size());
      // i += 16;
    }
    file = root.openNextFile();
  }
}
