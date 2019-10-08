/***************************************************************************//**
 * @file
 * @brief	Header file of module LightBarrier2.c
 * @author	Ralf Gerhauser
 * @version	2016-11-18
 ****************************************************************************//*
Revision History:
2016-11-18,rage	Changes for hardware platform Light_Barrier_2015_3:
		- Support 2nd light barrier on PD3.
		- The signal to enable the IR receivers is now high-active.
2016-07-14,rage	Changed GPIO_PORT_X defines to gpioPortX to prevent IAR compiler
		warning, except IR_LED_PORT_NUM which uses GPIO_PORT_X to select
		a dedicated pin, timer, and CC channel configuration.
2016-06-22,rage	Initial version.
*/

#ifndef __INC_LightBarrier2_h
#define __INC_LightBarrier2_h

/*=============================== Header Files ===============================*/

#include "em_device.h"
#include "em_gpio.h"
#include "config.h"		// include project configuration parameters

/*=============================== Definitions ================================*/

/*!@brief Power-on duration in [ms] for IR LED and receiver circuit.
 * Default is 6ms.
 */
#ifndef LB2_POWER_ON
  #define LB2_POWER_ON		 6
#endif

/*!@brief Power-off duration in [ms] for IR LED and receiver circuit.
 * Default is 94ms to perform a 100ms rate (power-on time is 6ms).
 */
#ifndef LB2_POWER_OFF
  #define LB2_POWER_OFF		94
#endif

/*!@brief PWM frequency for the Infrared LED in [Hz]
 * Default is 38kHz.
 */
#ifndef LB2_PWM_FREQ
  #define LB2_PWM_FREQ		38000
#endif


/*!@brief Port and pin selection for the GPIO that enables the Infrared Receiver
 * circuits.  These defines can also be set in the project configuration file
 * "config.h".  Default is PD6.  This pin is high-active.
 */
#ifndef IR_RX_ENA_PORT
  #define IR_RX_ENA_PORT	gpioPortD
#endif
#ifndef IR_RX_ENA_PIN
  #define IR_RX_ENA_PIN		6
#endif


/*!@brief Port and pin selection for the input signal from Infrared Receiver #1
 * circuit.  These defines can also be set in the project configuration file
 * "config.h".  Default is PD2.
 */
#ifndef IR1_RX_INP_PORT
  #define IR1_RX_INP_PORT	gpioPortD
#endif
#ifndef IR1_RX_INP_PIN
  #define IR1_RX_INP_PIN	2
#endif


/*!@brief Port and pin selection for the input signal from Infrared Receiver #2
 * circuit.  These defines can also be set in the project configuration file
 * "config.h".  Default is PD3.
 */
#ifndef IR2_RX_INP_PORT
  #define IR2_RX_INP_PORT	gpioPortD
#endif
#ifndef IR2_RX_INP_PIN
  #define IR2_RX_INP_PIN	3
#endif


/*!@brief Port and pin selection for the Infrared LEDs which are hardware
 * controlled by one of the timers. These defines can also be set in the
 * project configuration file "config.h".  Default is PD1.
 * The following GPIO pins are valid: PA0, PA1, PA2, PA8, PA9, PA10, PC8, PC9,
 * PC10, PC13, PC14, PC15, PD1, PD2, and PD3.
 *
 * Use defines GPIO_PORT_A to E, do *not* use gpioPortA to E.
 */
#ifndef IR_LED_PORT_NUM
  #define IR_LED_PORT_NUM	GPIO_PORT_D
#endif
#ifndef IR_LED_PIN
  #define IR_LED_PIN		1
#endif

/* automatically selects the right timer, CC channel, and routing */
#if IR_LED_PORT_NUM == GPIO_PORT_A  &&  IR_LED_PIN >= 0  &&  IR_LED_PIN <= 2
  #define IR_LED_PORT		gpioPortA
  #define IR_LED_TIMER_NUM	0
  #define IR_LED_CC_LOC		TIMER_ROUTE_LOCATION_LOC0
  #define IR_LED_CC_NUM		(IR_LED_PIN)
#elif IR_LED_PORT_NUM == GPIO_PORT_A  &&  IR_LED_PIN >= 8  &&  IR_LED_PIN <= 10
  #define IR_LED_PORT		gpioPortA
  #define IR_LED_TIMER_NUM	2
  #define IR_LED_CC_LOC		TIMER_ROUTE_LOCATION_LOC0
  #define IR_LED_CC_NUM		(IR_LED_PIN - 8)
#elif IR_LED_PORT_NUM == GPIO_PORT_C  &&  IR_LED_PIN >= 8  &&  IR_LED_PIN <= 10
  #define IR_LED_PORT		gpioPortC
  #define IR_LED_TIMER_NUM	2
  #define IR_LED_CC_LOC		TIMER_ROUTE_LOCATION_LOC2
  #define IR_LED_CC_NUM		(IR_LED_PIN - 8)
#elif IR_LED_PORT_NUM == GPIO_PORT_C  &&  IR_LED_PIN >= 13  &&  IR_LED_PIN <= 15
  #define IR_LED_PORT		gpioPortC
  #define IR_LED_TIMER_NUM	1
  #define IR_LED_CC_LOC		TIMER_ROUTE_LOCATION_LOC0
  #define IR_LED_CC_NUM		(IR_LED_PIN - 13)
#elif IR_LED_PORT_NUM == GPIO_PORT_D  &&  IR_LED_PIN >= 1  &&  IR_LED_PIN <= 3
  #define IR_LED_PORT		gpioPortD
  #define IR_LED_TIMER_NUM	0
  #define IR_LED_CC_LOC		TIMER_ROUTE_LOCATION_LOC3
  #define IR_LED_CC_NUM		(IR_LED_PIN - 1)
#elif IR_LED_PORT_NUM == GPIO_PORT_E  &&  IR_LED_PIN >= 10  &&  IR_LED_PIN <= 12
  #define IR_LED_PORT		gpioPortE
  #define IR_LED_TIMER_NUM	1
  #define IR_LED_CC_LOC		TIMER_ROUTE_LOCATION_LOC1
  #define IR_LED_CC_NUM		(IR_LED_PIN - 10)
#else
  #error "Invalid selection for IR_LED_PORT and/or IR_LED_PIN"
#endif

/* resulting defines from the configuration above */
#if IR_LED_TIMER_NUM == 0
  #define IR_LED_TIMER		TIMER0
  #define IR_LED_CMU_CLOCK	cmuClock_TIMER0
#elif IR_LED_TIMER_NUM == 1
  #define IR_LED_TIMER		TIMER1
  #define IR_LED_CMU_CLOCK	cmuClock_TIMER1
#elif IR_LED_TIMER_NUM == 2
  #define IR_LED_TIMER		TIMER2
  #define IR_LED_CMU_CLOCK	cmuClock_TIMER2
#endif

#if IR_LED_CC_NUM == 0
  #define IR_LED_CC_PEN		TIMER_ROUTE_CC0PEN
#elif IR_LED_CC_NUM == 1
  #define IR_LED_CC_PEN		TIMER_ROUTE_CC1PEN
#elif IR_LED_CC_NUM == 2
  #define IR_LED_CC_PEN		TIMER_ROUTE_CC2PEN
#endif

/* check if required defines are set */
#ifndef IR_LED_TIMER
  #error "Define IR_LED_TIMER could not be set, check IR_LED_PORT, IR_LED_PIN"
#endif
#ifndef IR_LED_CC_PEN
  #error "Define IR_LED_CC_PEN could not be set, check IR_LED_PORT, IR_LED_PIN"
#endif

/*================================ Global Data ===============================*/

extern volatile uint32_t  g_LB_ActiveMask;

/*================================ Prototypes ================================*/

/* Initialize Light Barrier hardware */
void	LB2_Init (void);

/*
 * External light Barrier handler, called from interrupt service routine.
 * This call-out routine must be provided by another module.
 */
void	LP2_SignalState (int num, bool state);


#endif /* __INC_LightBarrier2_h */
