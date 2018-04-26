#if 0
  #include <SPI.h>
  #include <PN532_SPI.h>
  #include <PN532.h>

  PN532_SPI pn532spi(SPI, 10);
  PN532 nfc(pn532spi);
#elif 0
  #include <PN532_HSU.h>
  #include <PN532.h>

  PN532_HSU pn532hsu(Serial);
  PN532 nfc(pn532hsu);
#else
  #include <Wire.h>
  #include <PN532_I2C.h>
  #include <PN532.h>
  PN532_I2C pn532i2c(Wire);
  PN532 nfc(pn532i2c);
#endif


//////////////////////////////////////////////////////////
char master_password[] = ""; // enter the SHA256 hash here
//////////////////////////////////////////////////////////

const int inBufferLen = 40;
char inData[inBufferLen]; // Allocate some space for the incoming data (buffer) -> 64 bytes for the hash string (32 bytes)


void setup(void) {
  Serial.begin(115200);
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) { // the reader isn't connected
    Serial.println("PN532 isn't connected!!")
    while (1); // halt
  }
  // configure board to read RFID tags
  nfc.SAMConfig();
}


void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Buffer to store the returned UID
  uint8_t uidLength = 0; // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t timeout = 0;
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  // -> currently only 4 bytes work with the PW manager (Mifare Classic)
  if (Comp(master_password) == 0){
    Serial.println("ACK-OK");
    while(true){
      if (timeout >= 9){ // 10 scans of UID, then terminates
        break;
      }
      success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength); // reads the UID
      delay(10);

      if (success) {
        for (uint8_t i = 0; i < uidLength; i++){
          Serial.print(uid[i], HEX); // return the UID
        }
        Serial.print("\n"); // our default endchar is "\n" a newline char
        delay(1500);
        timeout++;
      }
    }
    Serial.println("TERM"); // terminates the connection
  }
  else{
    Serial.println("ACK-NOK");
    delay(1500);
    }
}

char Comp(char* This) {
    byte index = 0;
    char inChar; // Where to store the character read
    while (Serial.available() > 0){ // if there are bytes incoming
        if(index < inBufferLen - 1){ // One less than the size of the array
            inChar = Serial.read(); // Read a character
            inData[index] = inChar; // Store it
            index++; // Increment where to write next
            inData[index] = '\0'; // Null terminate the string
        }
    }
    if (strcmp(inData,This)  == 0){ // compares the 2 strings
        return(0); // they are equal
    }
    else{
        return(1); // they are not equal
    }
}
