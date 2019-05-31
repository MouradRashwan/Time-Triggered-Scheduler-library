/*
 * TTScheduler.c
 *
 *  Created on: Feb 14, 2019
 *      Author: Administrator
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "TTScheduler.h"

static uint32_t g_ui32Ticks = 0;
static uint32_t g_ui32TickPeriod = 0;
static uint32_t g_ui32CyclePeriod = 0;
static Task_t *g_ptTaskArray = NULL;
static uint32_t g_ui32NumOfTasks = 0;

static void Tick_ISR(void)
{
    uint32_t i;

    g_ui32Ticks += g_ui32TickPeriod;

    for (i = 0; i < g_ui32NumOfTasks; i++)
    {
        if (((g_ui32Ticks % g_ptTaskArray[i].ui32PeriodInMillis) == 0)
                && g_ptTaskArray[i].bEnabled == 1)
        {
            g_ptTaskArray[i].pfnTaskFun();
        }
    }

    if (g_ui32Ticks == g_ui32CyclePeriod)
    {
        g_ui32Ticks = 0;
    }
}

void TTScheduler_initStatic(Task_t *ptTaskArray, uint32_t ui32NumOfTasks)
{
    uint32_t i, j, ui32Max = 0, ui32Min = 0xFFFFFFFFU;

    g_ptTaskArray = ptTaskArray;
    g_ui32NumOfTasks = ui32NumOfTasks;

    for (i = 0; i < g_ui32NumOfTasks; i++)
    {
        if (g_ptTaskArray[i].ui32PeriodInMillis < ui32Min)
        {
            ui32Min = g_ptTaskArray[i].ui32PeriodInMillis;
        }
        if (g_ptTaskArray[i].ui32PeriodInMillis > ui32Max)
        {
            ui32Max = g_ptTaskArray[i].ui32PeriodInMillis;
        }
    }

    for (i = ui32Min; i > 0; i--)
    {
        for (j = 0; j < g_ui32NumOfTasks; j++)
        {
            if ((g_ptTaskArray[j].ui32PeriodInMillis % i) != 0)
            {
                break;
            }
        }
        if (j == g_ui32NumOfTasks) /* when all tasks periods are divided by (i) (period / i) */
        {
            g_ui32TickPeriod = i;
            break;
        }
    }

    for (i = ui32Max; i < 0xFFFFFFFFU; i++)
    {
        for (j = 0; j < g_ui32NumOfTasks; j++)
        {
            if ((i % g_ptTaskArray[j].ui32PeriodInMillis) != 0)
            {
                break;
            }
        }
        if (j == g_ui32NumOfTasks) /* when (i) is divided by all tasks periods (i / period) */
        {
            g_ui32CyclePeriod = i;
            break;
        }
    }

    TimerTick_init(g_ui32TickPeriod, Tick_ISR);
}

void TTScheduler_start(void)
{
    TimerTick_start();
    while (1)
    {

    }
}

void TTScheduler_resumeTask(Task_t *ptTask)
{
    ptTask->bEnabled = 1;
}

void TTScheduler_suspendTask(Task_t *ptTask)
{
    ptTask->bEnabled = 0;
}

uint32_t TTScheduler_getTicks(void)
{
    return g_ui32Ticks;
}

uint32_t TTScheduler_getTickPeriod(void)
{
    return g_ui32TickPeriod;
}

uint32_t TTScheduler_getCyclePeriod(void)
{
    return g_ui32CyclePeriod;
}

