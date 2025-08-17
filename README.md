# CAPTURinoEmbedded_ESP32
Driver software to run the CAPTURino application software on a ESP32 platform.

## Utilized IOs of the ESP32:
* `GPIO  1`: Serial TX
* `GPIO  3`: Serial RX
* `GPIO 21`: I2C SDA
* `GPIO 22`: I2C SCL
* `GPIO 19`: SPI CS
* `GPIO  2`: SPI CIPO  ( \todo probably use different pin as this is used for boot mode selection)
* `GPIO  4`: SPI COPI
* `GPIO  0`: SPI CLK   ( \todo probably use different pin as this is used for boot mode selection)
* `GPIO  5`: New message interrupt line
* `GPIO 13`: Identification input 1 (HIGH) (generates an interrupt if changed)
* `GPIO 27`: Identification input 2
* `GPIO 25`: Identification input 3
* `GPIO 26`: Identification input 4

The peripherals are used for the following purposes:
* `Serial communication`: Communication of the CAPTURino hardware to the CAPTURino plugin, or any terminal program to provide a command line interface
* `I2C`: Read and write to the connected EEPROM
* `SPI`: Receive data from the CAN to SPI converter (MCP2515)
* `New message interrupt line`: additonal pin provided by the MCP2515
* `Identification input 1-4`: 4-bit code, used by the application software to determine further ways of identification.

Currently there are only two 4-bit codes implemented:
* `0b0000`: No PHY- or levelshifterboard connected
* `0b0001`: PHY- or levelshifterboard connected. Further information is stored at the EEPROM with I2C address 0b1010000. Within this EEPROM at adress 0x00 there is a 32-bit ID, stored in big endian format, which holds information regarding the bus type that can be captured with the device.
