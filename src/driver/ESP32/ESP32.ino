/* ***************************************************************************
 * M O D U L E   I N F O R M A T I O N * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */
/** 
 * \file
 * \addtogroup ESP32Driver
 * \brief Main entry point for the Capturino embedded application running on
 *        the ESP32 platform
 * \details This function is just a wrapper to abstract the main function call
 *          from the underlying platform
 * 
 * External dependencies:
 * - Arduino.h
 *
 */
/* ************************************************************************* */

#include "captmainstmac.h"

void setup() {
  /* call the platform independent main function */
  CaptAppl_MainStmac_main();
}

void loop() {
  /* catch the program here if the main function unexpectedly returns */
}
