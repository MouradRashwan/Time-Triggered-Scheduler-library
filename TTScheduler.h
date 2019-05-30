/*
 * TTScheduler.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Administrator
 */

#ifndef TTSCHEDULER_H_
#define TTSCHEDULER_H_

void TTScheduler_initStatic(Task_t *ptTaskArray, uint32_t ui32NumOfTasks);

void TTScheduler_initDynamic(Task_t *ptTaskArray, uint32_t ui32NumOfTasks);

void TTScheduler_start(void);

void TTScheduler_resumeTask(Task_t *ptTask);

void TTScheduler_suspendTask(Task_t *ptTask);

uint32_t TTScheduler_getTicks(void);

uint32_t TTScheduler_getTickPeriod(void);

uint32_t TTScheduler_getCyclePeriod(void);

#endif /* TTSCHEDULER_H_ */
