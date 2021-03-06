/* Copyright (c) 2007 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT. 
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRENTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 * $LastChangedRevision: 2477 $
 */ 
 
/** @file
 * @ingroup L01
 * Target specific include directives. The file name is standardised, but the
 * contents will vary according to the target. Separate the files by locating 
 * them in different folders. The goal is to avoid the use \#ifdef for
 * conditional compilation, which is difficult to maintain and prone to errors.
 *
 * This target include contains the files for nRF24L01.
 *
 * @author Per Kristian Schanke
 */
 
#ifndef TARGET_INCLUDES_H__
#define TARGET_INCLUDES_H__

#include <Cygnal\c8051f320.h>
#include "hal_nrf.h"

#define LED_ON 0          /**< The value on the line that turn the LED on */
#define LED_OFF 1         /**< The value on the line that turn the LED off */

sbit LED1 = P1^0;         /**< LED1 on BFB */
sbit LED2 = P1^1;         /**< LED2 on BFB */
sbit LED3 = P1^2;         /**< LED3 on BFB */

sbit B1  = P2^1;          /**< Button 1 on BFB */
sbit B2  = P2^2;          /**< Button 2 on BFB */
sbit B3  = P2^3;          /**< Button 3 on BFB */

#define B_PRESSED 0       /**< The value on the line when a button is pressed*/

#define RADIO_ACTIVITY() (IE0 == 1) /**< The register on the radio indicating 
                                     * activity on the radio */

/** How to reset the activity register */
#define RESET_RADIO_ACTIVITY() (IE0 = 0)

#define INTERRUPT_T0 INTERRUPT_TIMER0  /**< Definition of interrupt names */

#define TIMER1_OVERFLOW() (TF1 == 1)   /**< Checks for overflow on timer1 */

/** This is a constant showing how many clock cycles that is counted
 * by a timer to make up 1ms.
 *
 * Calculation of this constant:@code
      Ocilatorspeed / Clock divider / 1000ms/sec
      12MHz         / 12            / 1000ms/sec = 1000
 @endcode
 */
#define CYCLES_PR_MS 1000

/** The maximum runtime in ms that a timer might run. Calculated by 
 * taking the maximum number your timer might take and divide that by
 * @b CYCLES_PR_MS. On the nRF24L01 evaluation kit this is 0xFFFF / 1000 = 
 * 65 (aprox).
 */
#define MAX_RUNTIME 65

/** This constant defines the value the timer should be set to for running for
 * the maximum time.
 */
#define MAX_TIME (0 - MAX_RUNTIME * CYCLES_PR_MS)

#define GLOBAL_INT_ENABLE() do{EA = 1;}while(0) /**< Enable all interrupts */
#define GLOBAL_INT_DISABLE() do{EA = 0;}while(0)/**< Disable all interrupts */


/** Start timer0. */
#define T0_START() do{  TR0 = 1; \
                        ET0 = 1; \
                        EA  = 1; \
                        }while(0)
/** Stop timer0. */
#define T0_STOP() do{ TR0 = 0; \
                      ET0 = 0; \
                      }while(0)
/** Start timer1. */
#define T1_START() do{  TR1 = 1; \
                        ET1 = 0; \
                        TF1 = 0; \
                        EA  = 1; \
                        }while(0)
/** Stop timer1. */
#define T1_STOP() do{ TR1 = 0; \
                      ET1 = 0; \
                      }while(0)

/** Initialise Timer 1 in mode 1 (16-bit timer) */
#define T1_MODE1() (TMOD |= 0x10)
/** Sets the low bit of timer 1 */
#define T1_SET_LB(x) (TL1 = x)
/** Sets the high bit of timer 1 */
#define T1_SET_HB(x) (TH1 = x)

#endif // TARGET_INCLUDES_H__
