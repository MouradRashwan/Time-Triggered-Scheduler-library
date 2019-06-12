/*
 * TTScheduler.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Administrator
 */

#ifndef TTSCHEDULER_H_
#define TTSCHEDULER_H_

typedef struct Task
{
    uint32_t ui32ID;
    uint32_t ui32PeriodInMillis;
    void (*pfnTaskFun)(void);
    bool bEnabled;
} Task_t;

bool TTScheduler_init(Task_t *ptTaskArray, uint32_t ui32NumOfTasks);

void TTScheduler_start(void);

bool TTScheduler_resumeTask(uint32_t ui32ID);

bool TTScheduler_suspendTask(uint32_t ui32ID);

uint32_t TTScheduler_getTicks(void);

uint32_t TTScheduler_getTickPeriod(void);

uint32_t TTScheduler_getCyclePeriod(void);

#endif /* TTSCHEDULER_H_ */
