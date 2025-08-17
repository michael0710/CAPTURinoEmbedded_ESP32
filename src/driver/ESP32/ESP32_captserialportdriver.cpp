/* ***************************************************************************
 * M O D U L E   I N F O R M A T I O N * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */
/** 
 * \file
 * \addtogroup ESP32Driver
 * \brief Capturino Serial Port Driver implementation for the ESP32
 * \details As the ESP32 uses the Arduino framework, the Serial Port driver
 *          must be implemented in C++ in order to access any objects. However,
 *          the implemented functions can be called from C code, as the generic
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
#include "captserialportdriver.h"

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
const CaptDriverVersionType CaptDriver_SerialPort_VERSION =
{
    .major = 0,
    .minor = 1
};

/* L O C A L   C O N S T A N T   D E F I N I T I O N S * * * * * * * * * * * */

/* L O C A L   V A R I A B L E   D E F I N I T I O N S * * * * * * * * * * * */
static int mLastRead = -1;
static bool mIsInitialized = false;

/* ***************************************************************************
 * F U N C T I O N S   S E C T I O N * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */

/* L O C A L   F U N C T I O N   P R O T O T Y P E S * * * * * * * * * * * * */

/* L O C A L   I N L I N E   F U N C T I O N   D E F I N I T I O N S * * * * */

/* L O C A L   F U N C T I O N   D E F I N I T I O N S * * * * * * * * * * * */

/* G L O B A L   F U N C T I O N   D E F I N I T I O N S * * * * * * * * * * */
int CaptDriver_SerialPort_init()
{
    Serial.begin(256000);
    mIsInitialized = true;
    return 0;
}

int CaptDriver_SerialPort_sendn(const char* str, size_t n, size_t* sent)
{
    ASSERT(mIsInitialized,  CAPTAPPL_ERRNO_MODULE_NOT_INITIALIZED);
    ASSERT(n > 0,           CAPTAPPL_ERRNO_INVALID_PARAMETER);
    ASSERT(str != nullptr,  CAPTAPPL_ERRNO_INVALID_PARAMETER);

    size_t write = n;

    /* make sure that there is enough memory available so that Serial.write()
     * will return without blocking */
    if (Serial.availableForWrite() < n)
    {
        write = Serial.availableForWrite();
    }

    size_t written = Serial.write(str, write);
    if (sent != nullptr)
    {
        *sent = written;
    }

    /* check if all characters to write have actually been written */
    if (written != n)
    {
        /* not all bytes could be written */
        CaptAppl_Errno_set(CAPTAPPL_ERRNO_INSUFFICIENT_BUFFER);
        return -1;
    }

    return 0;
}

int CaptDriver_SerialPort_sendf(const char* fmt, ...)
{
    CaptAppl_Errno_set(CAPTAPPL_ERRNO_CALL_NOT_IMPLEMENTED);
    return -1;
}

/**
 * \todo as soon as this TODO is removed, the line below is not shown anymore
 *       in the documentation pdf file
 * 
 * Possible values of the CaptAppl_Errno_get() function are:
 * \arg CAPTAPPL_ERRNO_INVALID_PARAMETER if the specified buffer is a
 *      nullpointer or the specified maximum length is 0.
 * \arg CAPTAPPL_ERRNO_INSUFFICIENT_BUFFER if the internal buffer is too small
 *      so that not all bytes could be sent in a non-blocking attempt. In this
 *      case the value of 'sent' must be checked to see how many bytes have
 *      actually been sent. After some time it can be retried to send the
 *      remaining bytes.
 */
int CaptDriver_SerialPort_rcvn(char* in, size_t maxlen, size_t* rcvlen)
{
    ASSERT(mIsInitialized,  CAPTAPPL_ERRNO_MODULE_NOT_INITIALIZED);
    ASSERT(maxlen > 0,      CAPTAPPL_ERRNO_INVALID_PARAMETER);
    ASSERT(in != nullptr,   CAPTAPPL_ERRNO_INVALID_PARAMETER);

    if (mLastRead == -1)
    {
        mLastRead = Serial.read();
    }

    size_t len = 0;
    while (len < maxlen)
    {
        if (mLastRead >= 0)
        {
            in[len] = (char)mLastRead;
            len++;
            mLastRead = Serial.read();
        }
        else
        {
            break;
        }
    }

    if (rcvlen != nullptr)
    {
        *rcvlen = len;
    }

    if (mLastRead >= 0)
    {
        /* specified buffer is too small to hold all bytes to be read */
        CaptAppl_Errno_set(CAPTAPPL_ERRNO_INSUFFICIENT_BUFFER);
        return -1;
    }

    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* ***************************************************************************
 * E N D   O F   F I L E * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */
