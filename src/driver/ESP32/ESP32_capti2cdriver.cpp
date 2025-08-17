/* ***************************************************************************
 * M O D U L E   I N F O R M A T I O N * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */
/** 
 * \file
 * \addtogroup ESP32Driver
 * \brief Capturino Digital I2C Driver implementation for the ESP32
 * \details As the ESP32 uses the Arduino framework, the I2C driver must be
 *          implemented in C++ in order to access any objects. However, the
 *          implemented functions can be called from C code, as the generic
 *          header wraps it in an extern "C" block.
 *
 * External dependencies introduced by the source file:
 * - Arduino.h
 * - Wire.h
 */
/* ************************************************************************* */

/* ***************************************************************************
 * I N C L U D E   S E C T I O N * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */

/* P R O J E C T   I N C L U D E S * * * * * * * * * * * * * * * * * * * * * */
#include <Arduino.h>
#include <Wire.h>
#include "captconfiginclude.h"
#include "commontypes.h"
#include "capterrno.h"

/* M O D U L E   H E A D E R   I N C L U D E * * * * * * * * * * * * * * * * */
#include "capti2cdriver.h"

/* ***************************************************************************
 * D E F I N E   S E C T I O N * * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */

/* L O C A L   C O N F I G   D E F I N I T I O N S * * * * * * * * * * * * * */
#define I2C_EEPROM_ADDR (0x50)

#define I2C_SDA_PIN     (21)
#define I2C_SCL_PIN     (22)
#define I2C_CLK_SPEED   (100000)
#define I2C_ADDR_MASK   (0xFE)  /* probably move to the captphyidfcn module */
#define I2C_RW_MASK     (0x01)  /* probably not needed */

/* L O C A L   M A C R O   D E F I N I T I O N S * * * * * * * * * * * * * * */

/* ***************************************************************************
 * T Y P E D E F   S E C T I O N * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */

/* L O C A L   T Y P E D E F S * * * * * * * * * * * * * * * * * * * * * * * */

/* ***************************************************************************
 * V A R I A B L E S   A N D   C O N S T A N T S   S E C T I O N * * * * * * *
 *************************************************************************** */

/* G L O B A L   V A R I A B L E   D E F I N I T I O N S * * * * * * * * * * */
const CaptDriverVersionType CaptDriver_I2c_VERSION =
{
    .major = 0,
    .minor = 1
};

/* L O C A L   C O N S T A N T   D E F I N I T I O N S * * * * * * * * * * * */

/* L O C A L   V A R I A B L E   D E F I N I T I O N S * * * * * * * * * * * */
static bool mIsInitialized = false;

/* ***************************************************************************
 * F U N C T I O N S   S E C T I O N * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */

/* L O C A L   F U N C T I O N   P R O T O T Y P E S * * * * * * * * * * * * */

/* L O C A L   I N L I N E   F U N C T I O N   D E F I N I T I O N S * * * * */

/* L O C A L   F U N C T I O N   D E F I N I T I O N S * * * * * * * * * * * */

/* G L O B A L   F U N C T I O N   D E F I N I T I O N S * * * * * * * * * * */
int CaptDriver_I2c_init()
{
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, I2C_CLK_SPEED);
    mIsInitialized = true;
    return 0;
}

int CaptDriver_I2c_readData(uint8_t* data, size_t* readLen, size_t dataLen)
{
    *readLen = 0;
    while (Wire.available() && (*readLen < dataLen))
    {
        data[(*readLen)++] = Wire.read();
    }
    return 0;
}

int CaptDriver_I2c_requestData(uint8_t devAddr,
                               uint16_t memAddr,
                               uint8_t len)
{
    /** \todo check if the memAddr wraps around at the end of each 64-bit page */
    /* set the desired memory address with a pseudo-write instruction */
    Wire.beginTransmission(devAddr);
    Wire.write((memAddr & 0xFF00) >> 8);
    Wire.write((memAddr & 0x00FF) >> 0);
    Wire.endTransmission(false);
    /* the requestFrom function blocks until the requested data is available.
       think about implementing a better solution with the esp32 idf */
    int rv = Wire.requestFrom(devAddr, len, true);
    return (rv == len) ? 0 : -1;
}

int CaptDriver_I2c_sendData(uint8_t devAddr,
                            uint16_t memAddr,
                            uint8_t* data,
                            uint8_t len)
{
#if CAPTURino_ENABLE_WRITES_2_IDENTIFICATION_EEPROM == captENABLED
    Wire.beginTransmission(devAddr);
    Wire.write((memAddr & 0xFF00) >> 8);
    Wire.write((memAddr & 0x00FF) >> 0);
    Wire.write(data, len);
    int rv = Wire.endTransmission(true);
    return (rv == 0) ? 0 : -1;
#else
    CaptAppl_Errno_set(CAPTAPPL_ERRNO_CALL_NOT_IMPLEMENTED);
    return -1;
#endif
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* ***************************************************************************
 * E N D   O F   F I L E * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */
