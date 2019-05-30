/*
 * TTScheduler_defs.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Administrator
 */

#ifndef TTSCHEDULER_DEFS_H_
#define TTSCHEDULER_DEFS_H_

typedef struct Task
{
    bool bEnabled;
    uint32_t ui32PeriodInMillis;
    void (*pfnTaskFun)(void);
} Task_t;

#endif /* TTSCHEDULER_DEFS_H_ */
