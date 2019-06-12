/*
 * TimerTick.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Administrator
 */

#ifndef TIMERTICK_H_
#define TIMERTICK_H_

void TimerTick_init(uint32_t ui32TickPeriodInMillis, void (*pfnCallBack)(void));

void TimerTick_start(void);

#endif /* TIMERTICK_H_ */
