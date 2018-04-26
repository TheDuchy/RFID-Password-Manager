# RFID Password Manager
Uses RFID chips to authenticate and decrypt passwords, then copies them to clipboard
## Installation
First, install the PN532 Arduino library. You can download it from the library manager inside the IDE or from [here](https://github.com/adafruit/Adafruit-PN532)
After that go into the Client directory
Run `python3 setup.py`, go through the setup process.
Copy the SHA256 hash of the master password to clipboard and put it into the arduino sketch and select your prefered method of communication (SPI/I2C) upload it to the arduino.
You are good to go!
## Usage
Simply enter:
`cd Client`
`python3 main.py`
And follow the instructions on screen.
