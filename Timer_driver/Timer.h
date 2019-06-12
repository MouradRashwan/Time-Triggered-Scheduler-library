/*
 * Timer.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Administrator
 */

#ifndef TIMER_H_
#define TIMER_H_

void TickTimer_init(uint32_t ui32TickPeriodInMillis, void (*pfnCallBack)(void));

void TickTimer_start(void);

void HardExecTimer_init(void);

void HardExecTimer_start(uint32_t ui32WorstCaseExecTimeInMillis);

bool HardExecTimer_isTimeOut(void);

#endif /* TIMER_H_ */
