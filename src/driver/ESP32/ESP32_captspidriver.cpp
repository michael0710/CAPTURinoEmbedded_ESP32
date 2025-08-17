/* ***************************************************************************
 * M O D U L E   I N F O R M A T I O N * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */
/**
 * \file
 * \addtogroup ESP32Driver
 * \brief Capturino SPI Driver implementation for the ESP32
 * \details As the ESP32 uses the Arduino framework, the SPI driver must be
 *          implemented in C++ in order to access any objects. However, the
 *          implemented functions can be called from C code, as the generic
 *          header wraps it in an extern "C" block.
 * 
 * External dependencies introduced by the source file:
 * - Arduino.h
 * - SPI.h
 */
/* ************************************************************************* */

/* ***************************************************************************
 * I N C L U D E   S E C T I O N * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */

/* P R O J E C T   I N C L U D E S * * * * * * * * * * * * * * * * * * * * * */
#include <Arduino.h>
#include <SPI.h>
#include "commontypes.h"
#include "capterrno.h"

/* M O D U L E   H E A D E R   I N C L U D E * * * * * * * * * * * * * * * * */
#include "captspidriver.h"

/* ***************************************************************************
 * D E F I N E   S E C T I O N * * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */

/* L O C A L   C O N F I G   D E F I N I T I O N S * * * * * * * * * * * * * */
#define SPI_NEW_MSG_INT_PIN (5)
#define SPI_CS_PIN          (19)
#define SPI_CLK_PIN         (0)
#define SPI_CIPO_PIN        (2)
#define SPI_COPI_PIN        (4)

/* L O C A L   M A C R O   D E F I N I T I O N S * * * * * * * * * * * * * * */

/* ***************************************************************************
 * T Y P E D E F   S E C T I O N * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */

/* L O C A L   T Y P E D E F S * * * * * * * * * * * * * * * * * * * * * * * */

/* ***************************************************************************
 * V A R I A B L E S   A N D   C O N S T A N T S   S E C T I O N * * * * * * *
 *************************************************************************** */

/* G L O B A L   V A R I A B L E   D E F I N I T I O N S * * * * * * * * * * */
const CaptDriverVersionType CaptDriver_Spi_VERSION =
{
    .major = 0,
    .minor = 1
};

/* L O C A L   C O N S T A N T   D E F I N I T I O N S * * * * * * * * * * * */

/* L O C A L   V A R I A B L E   D E F I N I T I O N S * * * * * * * * * * * */
static CallbackFuncType mActiveCb = NULL;
static PinCallbackEventType mIntEvt = EVT_FALLING;
static PinCallbackEventType mWorkaroundActiveEvt = EVT_FALLING;
static SPIClass mSpi(VSPI);
static uint32_t mClockFreq = 0;
static uint8_t mMode = 0;
static uint8_t mBitOrder = 0;
static SpiCsPolarityType mCsPolarity = SPI_CS_ACTIVE_HIGH;

/* ***************************************************************************
 * F U N C T I O N S   S E C T I O N * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */

/* L O C A L   F U N C T I O N   P R O T O T Y P E S * * * * * * * * * * * * */

/* L O C A L   I N L I N E   F U N C T I O N   D E F I N I T I O N S * * * * */

/* L O C A L   F U N C T I O N   D E F I N I T I O N S * * * * * * * * * * * */
void LocalIntHandler()
{
    if (mActiveCb != NULL)
    {
        bool isInterruptEvent = false;
        switch (mIntEvt)
        {
            /** \todo as the ESP32 has problems using multiple edge triggered
             * interrupts, think about implementing the workaround here. */
            case EVT_FALLING:
                detachInterrupt(digitalPinToInterrupt(SPI_NEW_MSG_INT_PIN));
                if (mWorkaroundActiveEvt == EVT_LEVEL_LOW)
                {
                    isInterruptEvent = true;
                    attachInterrupt(digitalPinToInterrupt(SPI_NEW_MSG_INT_PIN), LocalIntHandler, ONHIGH);
                    mWorkaroundActiveEvt = EVT_LEVEL_HIGH;
                }
                else
                {
                    attachInterrupt(digitalPinToInterrupt(SPI_NEW_MSG_INT_PIN), LocalIntHandler, ONLOW);
                    mWorkaroundActiveEvt = EVT_LEVEL_HIGH;
                }
                break;

            case EVT_RISING:
                detachInterrupt(digitalPinToInterrupt(SPI_NEW_MSG_INT_PIN));
                if (mWorkaroundActiveEvt == EVT_LEVEL_LOW)
                {
                    attachInterrupt(digitalPinToInterrupt(SPI_NEW_MSG_INT_PIN), LocalIntHandler, ONHIGH);
                    mWorkaroundActiveEvt = EVT_LEVEL_HIGH;
                }
                else
                {
                    isInterruptEvent = true;
                    attachInterrupt(digitalPinToInterrupt(SPI_NEW_MSG_INT_PIN), LocalIntHandler, ONLOW);
                    mWorkaroundActiveEvt = EVT_LEVEL_HIGH;
                }
                break;

            case EVT_BOTH:
                detachInterrupt(digitalPinToInterrupt(SPI_NEW_MSG_INT_PIN));
                isInterruptEvent = true;
                if (mWorkaroundActiveEvt == EVT_LEVEL_LOW)
                {
                    attachInterrupt(digitalPinToInterrupt(SPI_NEW_MSG_INT_PIN), LocalIntHandler, ONHIGH);
                    mWorkaroundActiveEvt = EVT_LEVEL_HIGH;
                }
                else
                {
                    attachInterrupt(digitalPinToInterrupt(SPI_NEW_MSG_INT_PIN), LocalIntHandler, ONLOW);
                    mWorkaroundActiveEvt = EVT_LEVEL_HIGH;
                }
                break;

            case EVT_LEVEL_HIGH:
            case EVT_LEVEL_LOW:
                isInterruptEvent = true;
                break;
        }
    
        if (isInterruptEvent)
        {
            mActiveCb();
        }
    }
}

/* G L O B A L   F U N C T I O N   D E F I N I T I O N S * * * * * * * * * * */
int CaptDriver_Spi_init()
{
    pinMode(SPI_NEW_MSG_INT_PIN, INPUT_PULLDOWN);
    /* note: seems like MODE0 and MSBFIRST are hardcoded values for the ESP32.
       see: https://github.com/espressif/arduino-esp32/blob/master/libraries/SPI/src/SPI.cpp
       NO! There are member-functions setDataMode and setBitOrder */
    mSpi.begin(SPI_CLK_PIN, SPI_CIPO_PIN, SPI_COPI_PIN, SPI_CS_PIN);
    pinMode(SPI_CS_PIN, OUTPUT);
    return 0;
}

int CaptDriver_Spi_setup(uint32_t          clockfreq,
                         SpiModeType       mode,
                         SpiBitOrderType   bitorder,
                         SpiCsPolarityType csPolarity)
{
    mClockFreq = clockfreq;
    switch(mode)
    {
        case SPI_CFG_MODE0:
            mMode = SPI_MODE0;
            break;

        case SPI_CFG_MODE1:
            mMode = SPI_MODE1;
            break;

        case SPI_CFG_MODE2:
            mMode = SPI_MODE2;
            break;

        case SPI_CFG_MODE3:
            mMode = SPI_MODE3;
            break;

        default:
            CaptAppl_Errno_set(CAPTAPPL_ERRNO_INVALID_PARAMETER);
            return -1;
    }

    switch(bitorder)
    {
        case SPI_CFG_MSBFIRST:
            mBitOrder = MSBFIRST;
            break;

        case SPI_CFG_LSBFIRST:
            mBitOrder = LSBFIRST;
            break;

        default:
            CaptAppl_Errno_set(CAPTAPPL_ERRNO_INVALID_PARAMETER);
            return -1;
    }

    mCsPolarity = csPolarity;
    digitalWrite(SPI_CS_PIN, (mCsPolarity == SPI_CS_ACTIVE_HIGH) ? LOW : HIGH);
    return 0;
}

int CaptDriver_Spi_execute(const uint8_t* txData,
                                 size_t   txLen,
                                 uint8_t* rxData,
                                 size_t   rxLen)
{
    digitalWrite(SPI_CS_PIN, (mCsPolarity == SPI_CS_ACTIVE_HIGH) ? HIGH : LOW);
    mSpi.beginTransaction(SPISettings(mClockFreq, mBitOrder, mMode));
    for (size_t i = 0; i < txLen; i++)
    {
        mSpi.transfer(txData[i]);
    }
    for (size_t i = 0; i < rxLen; i++)
    {
        rxData[i] = mSpi.transfer(txData[i]);
    }
    mSpi.endTransaction();
    digitalWrite(SPI_CS_PIN, (mCsPolarity == SPI_CS_ACTIVE_HIGH) ? LOW : HIGH);
    return 0;
}

int CaptDriver_Spi_registerNewDataPinCb(CallbackFuncType     cb,
                                        PinCallbackEventType evt)
{
    if (mActiveCb != NULL)
    {
        detachInterrupt(digitalPinToInterrupt(SPI_NEW_MSG_INT_PIN));
    }

    if (cb != NULL)
    {
        int mode;
        switch (evt)
        {
            case EVT_FALLING:
                mode = ONLOW; /* see workaround in the local interrupt handler */
                break;
            case EVT_RISING:
                mode = ONHIGH; /* see workaround in the local interrupt handler */
                break;
            case EVT_BOTH:
                mode = ONLOW; /* see workaround in the local interrupt handler */
                break;
            case EVT_LEVEL_HIGH:
                mode = ONHIGH;
                break;
            case EVT_LEVEL_LOW:
                mode = ONLOW;
                break;
            default:
                CaptAppl_Errno_set(CAPTAPPL_ERRNO_INVALID_PARAMETER);
                return -1;
        }
        attachInterrupt(digitalPinToInterrupt(SPI_NEW_MSG_INT_PIN), LocalIntHandler, mode);
        mActiveCb = cb;
        mIntEvt = evt;
    }
    else
    {
        mActiveCb = NULL;
    }

    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* ***************************************************************************
 * E N D   O F   F I L E * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */
