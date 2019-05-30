/*
 * TimerTick_defs.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Administrator
 */

#ifndef TIMERTICK_DEFS_H_
#define TIMERTICK_DEFS_H_

#define TIMER_ADDR_BASE     TIMER0_BASE
#define TIMER_CLK_BASE      SYSCTL_PERIPH_TIMER0

#define GET_TICKS_FROM_MILLIS(TIME_ms)  ((TIME_ms) * (SysCtlClockGet() / 1000ULL))

#endif /* TIMERTICK_DEFS_H_ */
