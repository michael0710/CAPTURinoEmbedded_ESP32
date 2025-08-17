/* ***************************************************************************
 * M O D U L E   I N F O R M A T I O N * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */
/**
 * \file
 * \addtogroup ESP32Driver
 * \brief Capturino Direct Sample Driver implementation for the ESP32
 * \details As we currently don't support direct sampling with the ESP32, this
 *          driver is a dummy driver whose init function returns an error. This
 *          signals the application software that this driver is not available.
 */
/* ************************************************************************* */

/* ***************************************************************************
 * I N C L U D E   S E C T I O N * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */

/* P R O J E C T   I N C L U D E S * * * * * * * * * * * * * * * * * * * * * */
#include <stdarg.h>
#include "commontypes.h"
#include "capterrno.h"

/* M O D U L E   H E A D E R   I N C L U D E * * * * * * * * * * * * * * * * */
#include "captdirectsampledriver.h"

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
const CaptDriverVersionType CaptDriver_DirectSample_VERSION =
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
int CaptDriver_DirectSample_init()
{
    CaptAppl_Errno_set(CAPTAPPL_ERRNO_CALL_NOT_IMPLEMENTED);
    return -1;
}

int CaptDriver_DirectSample_stop(uint32_t boardId)
{
    return -1;
}

bool CaptDriver_DirectSample_checkId(uint32_t boardId)
{
    return false;
}

int CaptDriver_DirectSample_setup(uint32_t dlt, size_t paramsCount, ...)
{
    return -1;
}

int CaptDriver_DirectSample_registerNewDataCb(CallbackFuncType cb)
{
    CaptAppl_Errno_set(CAPTAPPL_ERRNO_CALL_NOT_IMPLEMENTED);
    return -1;
}

int CaptDriver_DirectSample_capture()
{
    return -1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* ***************************************************************************
 * E N D   O F   F I L E * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */
