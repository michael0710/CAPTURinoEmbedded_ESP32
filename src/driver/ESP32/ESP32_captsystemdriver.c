/* ***************************************************************************
 * M O D U L E   I N F O R M A T I O N * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */
/**
 * \file
 * \addtogroup ESP32Driver
 * \brief Capturino System Driver implementation for the ESP32
 * \details \todo
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
#include "captsystemdriver.h"

/* ***************************************************************************
 * D E F I N E   S E C T I O N * * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */

/* L O C A L   C O N F I G   D E F I N I T I O N S * * * * * * * * * * * * * */

/* L O C A L   M A C R O   D E F I N I T I O N S * * * * * * * * * * * * * * */

/* ***************************************************************************
 * T Y P E D E F   S E C T I O N * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */

/* L O C A L   T Y P E D E F S * * * * * * * * * * * * * * * * * * * * * * * */

/* ***************************************************************************
 * V A R I A B L E S   A N D   C O N S T A N T S   S E C T I O N * * * * * * *
 *************************************************************************** */

/* G L O B A L   V A R I A B L E   D E F I N I T I O N S * * * * * * * * * * */
const CaptDriverVersionType CaptDriver_System_VERSION =
{
    .major = 0,
    .minor = 1
};

/* L O C A L   C O N S T A N T   D E F I N I T I O N S * * * * * * * * * * * */

/* L O C A L   V A R I A B L E   D E F I N I T I O N S * * * * * * * * * * * */

/* ***************************************************************************
 * F U N C T I O N S   S E C T I O N * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */

/* L O C A L   F U N C T I O N   P R O T O T Y P E S * * * * * * * * * * * * */

/* L O C A L   I N L I N E   F U N C T I O N   D E F I N I T I O N S * * * * */

/* L O C A L   F U N C T I O N   D E F I N I T I O N S * * * * * * * * * * * */

/* G L O B A L   F U N C T I O N   D E F I N I T I O N S * * * * * * * * * * */
int CaptDriver_System_init()
{
    return 0;
}

int CaptDriver_System_millis(uint32_t* timeMS)
{
    if (timeMS == NULL)
    {
        CaptAppl_Errno_set(CAPTAPPL_ERRNO_INVALID_PARAMETER);
        return -1;
    }
    *timeMS = millis();
    return 0;
}

int CaptDriver_System_micros(uint32_t* timeUS)
{
    if (timeUS == NULL)
    {
        CaptAppl_Errno_set(CAPTAPPL_ERRNO_INVALID_PARAMETER);
        return -1;
    }
    *timeUS = micros();
    return 0;
}

const char* CaptDriver_System_getPlatformString()
{
    return "ESP32";
}

int CaptDriver_System_setFaultLed()
{
    CaptAppl_Errno_set(CAPTAPPL_ERRNO_CALL_NOT_IMPLEMENTED);
    return -1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* ***************************************************************************
 * E N D   O F   F I L E * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */
