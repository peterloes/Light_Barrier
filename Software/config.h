/***************************************************************************//**
 * @file
 * @brief	Project configuration file
 * @author	Ralf Gerhauser
 * @version	2016-06-22
 *
 * This file allows to set miscellaneous configuration parameters.  It must be
 * included by all modules.
 *
 ****************************************************************************//*
Revision History:
2016-06-22,rage	Derived from project "SNB_Heaven".
*/

#ifndef __INC_config_h
#define __INC_config_h

/*=============================== Header Files ===============================*/

#include <stdio.h>
#include <stdbool.h>
#include "em_device.h"

/*=============================== Definitions ================================*/

/*
 * Basic defines - should all be moved to Generic.h
 */
    /* terminators for lists and strings */

#define	EOL		NULL		/* EndOfList		*/
#define EOS		'\0'		/* EndOfString		*/
#define	NONE		(-1)

    /* macro to calculate the number of elements of an array */
#define ELEM_CNT(array)  (sizeof (array) / sizeof ((array)[0]))

/*
 * Port numbers - used to select a dedicated configuration for IR_LED_PORT_NUM
 */
#define GPIO_PORT_A	0
#define GPIO_PORT_B	1
#define GPIO_PORT_C	2
#define GPIO_PORT_D	3
#define GPIO_PORT_E	4

/*!
 * @brief MPU Clock Configuration.
 *
 * Set to 0 to use the internal RC oscillator, if 1 the external 32MHz XTAL
 * is used.
 */
#define USE_EXT_32MHZ_CLOCK	0

/*
 * Configuration for module "LightBarrier2"
 */
    /*!@brief LETIMER frequency in [Hz]. */
#define LETIMER_COUNTS_PER_SEC	32768

    /*!@brief Define PWM frequency for IR LED in [Hz] */
#define LB2_PWM_FREQ		37835	// nearly 38kHz


/*================================== Macros ==================================*/

    /*! Macro to address a single bit in the I/O range (peripheral range) in
     *  an atomic manner.
     * @param address   I/O register address.
     * @param bitNum    Bit number within this register.
     */
#define IO_BIT_ADDR(address, bitNum)					\
	((__IO uint32_t *) (BITBAND_PER_BASE				\
			+ (((uint32_t)(address)) - PER_MEM_BASE) * 32	\
			+ (bitNum) * 4))

    /*! Shortcut to directly access an I/O-bit. */
#define IO_Bit(regName, bitNum)	*IO_BIT_ADDR(&regName, bitNum)

    /*! Macro to address a single bit in an SRAM variable in an atomic manner.
     * @param address   Address of the variable in SRAM.
     * @param bitNum    Bit number within this variable.
     */
#define SRAM_BIT_ADDR(address, bitNum)					\
	((__IO uint32_t *) (BITBAND_RAM_BASE				\
			+ (((uint32_t)(address)) - RAM_MEM_BASE) * 32	\
			+ (bitNum) * 4))

    /*! Shortcut to directly access a bit in a variable. */
#define Bit(varName, bitNum)	*SRAM_BIT_ADDR(&varName, bitNum)

/*=========================== Typedefs and Structs ===========================*/

/*!@brief Enumeration of the EM1 Modules
 *
 * This is the list of Software Modules that require EM1 to work, i.e. they
 * will not work in EM2 because clocks, etc. would be disabled.  These enums
 * are used to set/clear the appropriate bit in the @ref g_EM1_ModuleMask.
 */
typedef enum
{
    EM1_MOD_IR_LED_TIMER,	//!<  0: 38kHz timer requires EM1 for its clock
    END_EM1_MODULES
} EM1_MODULES;

/*======================== External Data and Routines ========================*/

extern volatile bool	 g_flgIRQ;		// Flag: Interrupt occurred
extern volatile uint16_t g_EM1_ModuleMask;	// Modules that require EM1


#endif /* __INC_config_h */
