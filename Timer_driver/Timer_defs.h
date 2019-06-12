/*
 * Timer_defs.h
 *
 *  Created on: Jun 12, 2019
 *      Author: Administrator
 */

#ifndef TIMER_DEFS_H_
#define TIMER_DEFS_H_

#define GET_TICKS_FROM_MILLIS(TIME_ms)  ((TIME_ms) * (SysCtlClockGet() / 1000ULL))

#define TICK_TIMER_ADDR_BASE            TIMER0_BASE
#define TICK_TIMER_CLK_BASE             SYSCTL_PERIPH_TIMER0

#define HARD_EXEC_TIMER_ADDR_BASE       TIMER1_BASE
#define HARD_EXEC_TIMER_CLK_BASE        SYSCTL_PERIPH_TIMER1

#endif /* TIMER_DEFS_H_ */
