/***************************************************************************//**
 * @file
 * @brief	Test application for Module LightBarrier2
 * @author	Ralf Gerhauser
 * @version	2016-11-18
 *
 * This program is a test application for the software module LightBarrier2.
 * The default configuration of this application requires the hardware platform
 * "Light_Barrier_2015_3".
 *
 ****************************************************************************//*
Revision History:
2018-05-14,rage PC0 signals the current state of light barrier 1
                and light barrier 2.
2016-11-18,rage	Changes for hardware platform Light_Barrier_2015_3:
		PA9 is used as ouput for the second light barrier (LB#2).
2016-06-22,rage	Initial version.
*/

/*=============================== Header Files ===============================*/

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"

#include "config.h"
#include "drivers/LightBarrier2.h"

/*================================ Global Data ===============================*/

/*! @brief Flag to indicate that an Interrupt occurred in the meantime.
 *
 * This flag must be set <b>true</b> by any interrupt service routine that
 * requires actions in the service execution loop of main().  This prevents
 * the system from entering sleep mode, so the action can be taken before.
 */
volatile bool		g_flgIRQ;

/*! @brief Modules that require EM1.
 *
 * This global variable is a bit mask for all modules that require EM1.
 * Standard peripherals would stop working in EM2 because clocks, etc. are
 * disabled.  Therefore it is required for software modules that make use
 * of such devices, to set the appropriate bit in this mask, as long as they
 * need EM1.  This prevents the power management of this application to enter
 * EM2.  The enumeration @ref EM1_MODULES lists those modules.
 * Low-Power peripherals, e.g. the LEUART still work in EM1.
 *
 * Examples:
 *
   @code
   // Module RFID requires EM1, set bit in bit mask
   Bit(g_EM1_ModuleMask, EM1_MOD_RFID) = 1;
   ...
   // Module RFID is no longer active, clear bit in bit mask
   Bit(g_EM1_ModuleMask, EM1_MOD_RFID) = 0;
   @endcode
 */
volatile uint16_t	g_EM1_ModuleMask;

/*=========================== Forward Declarations ===========================*/

static void cmuSetup(void);


/**************************************************************************//**
 * @brief  Main function
 * Main is called from __iar_program_start, see assembly startup file
 *****************************************************************************/
int main(void)
{
    /* Initialize chip - handle erratas */
    CHIP_Init();

    /* Set up clocks */
    cmuSetup();

    /* Configure PA8 to signal the current state of light barrier 1 */
    GPIO_PinModeSet(gpioPortA, 8, gpioModePushPull, 0);

    /* Configure PA9 to signal the current state of light barrier 2 */
    GPIO_PinModeSet(gpioPortA, 9, gpioModePushPull, 0);

    /* Configure PC0 to show the state of light barrier 1 and light barrier 2 */
    GPIO_PinModeSet(gpioPortC, 0, gpioModePushPull, 0);
    
    /* Initialize light barrier hardware */
    LB2_Init();


    /* ============================================ *
     * ========== Service Execution Loop ========== *
     * ============================================ */
    while (1)
    {
	/* Perform here any Actions to be executed periodically */


	/*
	 * Check for current power mode:  If a minimum of one active module
	 * requires EM1, i.e. <g_EM1_ModuleMask> is not 0, this will be
	 * entered.  If no one requires EM1 activity, EM2 is entered.
	 */
	if (! g_flgIRQ)		// enter EM only if no IRQ occurred
	{
	    if (g_EM1_ModuleMask)
		EMU_EnterEM1();		// EM1 - Sleep Mode
	    else
		EMU_EnterEM2(true);	// EM2 - Deep Sleep Mode
	}
	else
	{
	    g_flgIRQ = false;	// clear flag to enter EM the next time
	}
    }
}

/******************************************************************************
 * @brief   Configure Clocks
 *
 * This local routine is called once from main() to configure all required
 * clocks of the EFM32 device.
 *
 *****************************************************************************/
static void cmuSetup(void)
{
    /* Start LFXO and wait until it is stable */
    CMU_OscillatorEnable(cmuOsc_LFXO, true, true);

#if USE_EXT_32MHZ_CLOCK
    /* Start HFXO and wait until it is stable */
    CMU_OscillatorEnable(cmuOsc_HFXO, true, true);

    /* Select HFXO as clock source for HFCLK */
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);

    /* Disable HFRCO */
    CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);
#endif

    /* Route the LFXO clock to the RTC/LETIMER and set the prescaler */
    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);	// LETIMER

    /* Enable clock to low energy modules */
    CMU_ClockEnable(cmuClock_CORELE, true);

    /* Enable clock for HF peripherals (TIMER) */
    CMU_ClockEnable(cmuClock_HFPER, true);

    /* Enable clock to GPIO */
    CMU_ClockEnable(cmuClock_GPIO, true);
}


/******************************************************************************
 * @brief   Call-out Routine from Module LightBarrier2
 *
 * This routine is called from Module LightBarrier2 whenever the state of a
 * light barrier has changed.
 *
 * @param[in] num
 * 	Number (index) of the light barrier: 0 for LB#1, and 1 for LB#2.
 *
 * @param[in] state
 * 	Current state of the light barrier: true if beam is interrupted,
 * 	false if not.
 *
 * @warning:
 * 	This routine is called in interrupt context, therefore care has to be
 * 	taken not to execute non-interrupt save routines, or functions that
 * 	need too much execution time.
 *
 *****************************************************************************/
void LP2_SignalState (int num, bool state)
{
int   port = 8 + num;	// PA8 for LB0, PA9 for LB1

    if (state)
    {
	// true: light barrier active (beam is interrupted)
	GPIO_PinOutClear(gpioPortA, port);	// set output to LOW
        
        // true: light barrier active (beam is interrupted)
	GPIO_PinOutSet(gpioPortC, 0);	// set output to HIGH
    }
    else
    {
	// false: light barrier inactive (beam is not interrupted)
	GPIO_PinOutSet(gpioPortA, port);	// set output to HIGH
        
        // false: light barrier inactive (beam is not interrupted)
	GPIO_PinOutClear(gpioPortC, 0);	// set output to LOW
    }
}
