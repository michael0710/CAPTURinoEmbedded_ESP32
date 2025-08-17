/* ***************************************************************************
 * M O D U L E   I N F O R M A T I O N * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */
/**
 * \file
 * \addtogroup CaptConfig
 * \brief Capturino configuration definition file
 * \details This module provides the possibility to configure the behaviour of
 *          the CAPTURino application.
 * 
 * \note This module shall never be included directly in any source file. Any
 *       source file shall include the captconfiginclude.h file which performs
 *       checks on the defined configuration and issues compiler errors on
 *       invalid configurations
 * 
 * @{
 */
/* ************************************************************************* */

#ifndef CAPTURINO_CFG_H_INCLUDED
#define CAPTURINO_CFG_H_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif

/* G L O B A L   C O N F I G   D E F I N I T I O N S * * * * * * * * * * * * */
#define captENABLED     (1)
#define captDISABLED    (0)

#define CAPTURino_ENABLE_WRITES_2_IDENTIFICATION_EEPROM     captENABLED

/** \todo think about adding configuration options to disable specific link
 *        types */

/** Compiles the CAPTURino software with the feature of printing the capture
 * data in hexadecimal ascii characters to the connected terminal.
 * 
 * \note With this feature enabled, the capture data cannot be read by the
 *       Wireshark plugin.
 */
#define CAPTURino_PRINT_HUMAN_READABLE_CAPTURE_DATA         captDISABLED

/** Compiles the CAPTURino software with the specific capture handler necessary
 * to capture data from the levelshifter-/PHY-board with the ID. */

/** Levelshifter-Board ID 0x00000001 for the RS-485 physical layer.  */
#define CAPTURino_INCLUDE_CAPTBOARDID_00000001              captENABLED

/** PHY-Board ID 0x80000001 for the High-Speed CAN physical layer (ISO 11898-2)
 * realized by MCP2562, combined with the MCP2515 SPI->CAN controller */
#define CAPTURino_INCLUDE_CAPTBOARDID_80000001              captENABLED

/* G L O B A L   C O N S T A N T S   D E C L A R A T I O N S * * * * * * * * */

#ifdef __cplusplus
}
#endif

#endif /* CAPTURINO_CFG_H_INCLUDED */

/**
 * @}
 */
/* ***************************************************************************
 * E N D   O F   F I L E * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *************************************************************************** */
