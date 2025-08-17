/* ***************************************************************************
 * M O D U L E   I N F O R M A T I O N * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */
/** 
 * \file
 * \addtogroup ESP32Driver
 * \brief Capturino Digital IO Driver implementation for the ESP32
 * \details As the ESP32 uses the Arduino framework, the IO driver must be
 *          implemented in C++ in order to access any objects. However, the
 *          implemented functions can be called from C code, as the generic
 *          header wraps it in an extern "C" block.
 *
 * External dependencies introduced by the source file:
 * - Arduino.h
 */
/* ************************************************************************* */

/* ***************************************************************************
 * I N C L U D E   S E C T I O N * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */

/* P R O J E C T   I N C L U D E S * * * * * * * * * * * * * * * * * * * * * */
#include <Arduino.h>
#include "commontypes.h"
#include "capterrno.h"

/* M O D U L E   H E A D E R   I N C L U D E * * * * * * * * * * * * * * * * */
#include "captiodriver.h"

/* ***************************************************************************
 * D E F I N E   S E C T I O N * * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */

/* L O C A L   C O N F I G   D E F I N I T I O N S * * * * * * * * * * * * * */
#define BASIC_IDFCN_PIN1 (13)
#define BASIC_IDFCN_PIN2 (27)
#define BASIC_IDFCN_PIN3 (25)
#define BASIC_IDFCN_PIN4 (26)

/* L O C A L   M A C R O   D E F I N I T I O N S * * * * * * * * * * * * * * */

/* ***************************************************************************
 * T Y P E D E F   S E C T I O N * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */

/* L O C A L   T Y P E D E F S * * * * * * * * * * * * * * * * * * * * * * * */

/* ***************************************************************************
 * V A R I A B L E S   A N D   C O N S T A N T S   S E C T I O N * * * * * * *
 *************************************************************************** */

/* G L O B A L   V A R I A B L E   D E F I N I T I O N S * * * * * * * * * * */
const CaptDriverVersionType CaptDriver_Io_VERSION =
{
    .major = 0,
    .minor = 1
};

/* L O C A L   C O N S T A N T   D E F I N I T I O N S * * * * * * * * * * * */

/* L O C A L   V A R I A B L E   D E F I N I T I O N S * * * * * * * * * * * */
static bool mIsInitialized = false;
static PinStateType mRegisteredInterruptLevel = PIN_HIGH;
static CallbackFuncType mActiveCb = NULL;

/* ***************************************************************************
 * F U N C T I O N S   S E C T I O N * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */

/* L O C A L   F U N C T I O N   P R O T O T Y P E S * * * * * * * * * * * * */

/* L O C A L   I N L I N E   F U N C T I O N   D E F I N I T I O N S * * * * */

/* L O C A L   F U N C T I O N   D E F I N I T I O N S * * * * * * * * * * * */
static void BasicIdfcn_Handler()
{
    /* The following lines simulate edge triggered interrupts.
     * For further information see:
     * https://docs.espressif.com/projects/esp-chip-errata/en/latest/esp32/03-errata-description/index.html#gpio-within-the-same-group-of-gpio-pins-edge-interrupts-cannot-be-used-together-with-other-interrupts */
    detachInterrupt(digitalPinToInterrupt(BASIC_IDFCN_PIN1));
    if (mRegisteredInterruptLevel == PIN_HIGH)
    {
        attachInterrupt(digitalPinToInterrupt(BASIC_IDFCN_PIN1), BasicIdfcn_Handler, ONLOW);
        mRegisteredInterruptLevel = PIN_LOW;
    }
    else
    {
        attachInterrupt(digitalPinToInterrupt(BASIC_IDFCN_PIN1), BasicIdfcn_Handler, ONHIGH);
        mRegisteredInterruptLevel = PIN_HIGH;
    }
    
    if (mActiveCb != NULL)
    {
        mActiveCb();
    }
}

/* G L O B A L   F U N C T I O N   D E F I N I T I O N S * * * * * * * * * * */
int CaptDriver_Io_init()
{
    pinMode(BASIC_IDFCN_PIN1, INPUT_PULLDOWN);
    pinMode(BASIC_IDFCN_PIN2, INPUT_PULLDOWN);
    pinMode(BASIC_IDFCN_PIN3, INPUT_PULLDOWN);
    pinMode(BASIC_IDFCN_PIN4, INPUT_PULLDOWN);
    mIsInitialized = true;
    mRegisteredInterruptLevel = PIN_HIGH;
    attachInterrupt(digitalPinToInterrupt(BASIC_IDFCN_PIN1), BasicIdfcn_Handler, ONHIGH);
    return 0;
}

/**
 * Having the interrupt enabled caused some problems in the EM-Laboratory.
 * Probably some EMC caused this interrupt to trigger over and over again.
 * Think about monitoring the amount of interrupts and disable the interrupts
 * temporarily if the number is too high.
 */
int CaptDriver_Io_registerBasicIdfcnChangedCb(CallbackFuncType cb)
{
    mActiveCb = cb;
    return 0;
}

int CaptDriver_Io_getBasicIdfcnPinVals(uint8_t *pinVals)
{
    *pinVals = 0x00;
    *pinVals |= (digitalRead(BASIC_IDFCN_PIN1) == HIGH) ? (1 << 0) : 0x0;
    *pinVals |= (digitalRead(BASIC_IDFCN_PIN2) == HIGH) ? (1 << 1) : 0x0;
    *pinVals |= (digitalRead(BASIC_IDFCN_PIN3) == HIGH) ? (1 << 2) : 0x0;
    *pinVals |= (digitalRead(BASIC_IDFCN_PIN4) == HIGH) ? (1 << 3) : 0x0;
    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* ***************************************************************************
 * E N D   O F   F I L E * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */
