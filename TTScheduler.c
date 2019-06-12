/*
 * TTScheduler.c
 *
 *  Created on: Feb 14, 2019
 *      Author: Administrator
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Timer_driver/Timer.h"

#include "TTScheduler.h"

static bool g_bIsValidToRun;
static Task_t *g_ptTaskArray;
static uint32_t g_ui32NumOfTasks;
static uint32_t g_ui32TicksInMillis;
static uint32_t g_ui32TickPeriodInMillis;
static uint32_t g_ui32CyclePeriodInMillis;

static void _TickTimerCallBAck(void)
{
    uint32_t i;

    g_ui32TicksInMillis += g_ui32TickPeriodInMillis;

    for (i = 0; i < g_ui32NumOfTasks; i++)
    {
        if (((g_ui32TicksInMillis % g_ptTaskArray[i].ui32PeriodInMillis) == 0)
                && (g_ptTaskArray[i].bEnabled == 1))
        {
            if (g_ptTaskArray[i].pfnTaskFun != NULL)
            {
                /*
                 *
                 * Run this task in a specific time intervals to minimize Jitter for the next Hard RealTime Task.
                 *
                 */
                HardExecTimer_start(g_ptTaskArray[i].ui32WorstCaseExecTimeInMillis);

                g_ptTaskArray[i].pfnTaskFun();

                while (!HardExecTimer_isTimeOut())
                {
                }
            }
        }
    }

    if (g_ui32TicksInMillis == g_ui32CyclePeriodInMillis)
    {
        g_ui32TicksInMillis = 0;
    }
}

bool TTScheduler_init(Task_t *ptTaskArray, uint32_t ui32NumOfTasks)
{
    uint32_t i, j, ui32Max = 0, ui32Min = UINT32_MAX;

    g_ui32NumOfTasks = ui32NumOfTasks;
    g_ptTaskArray = ptTaskArray;

    if (g_ptTaskArray == NULL)
    {
        g_bIsValidToRun = false;
        return g_bIsValidToRun;
    }

    for (i = 0; i < g_ui32NumOfTasks; i++)
    {
        g_ptTaskArray[i].bEnabled = true;

        if (g_ptTaskArray[i].ui32PeriodInMillis < ui32Min)
        {
            ui32Min = g_ptTaskArray[i].ui32PeriodInMillis;
        }
        if (g_ptTaskArray[i].ui32PeriodInMillis > ui32Max)
        {
            ui32Max = g_ptTaskArray[i].ui32PeriodInMillis;
        }

        if (g_ptTaskArray[i].ui32PeriodInMillis
                < g_ptTaskArray[i].ui32WorstCaseExecTimeInMillis)
        {
            g_bIsValidToRun = false;
            return g_bIsValidToRun;
        }

        for (j = i + 1; j < g_ui32NumOfTasks; j++)
        {
            if (g_ptTaskArray[i].ui32ID == g_ptTaskArray[j].ui32ID)
            {
                g_bIsValidToRun = false;
                return g_bIsValidToRun;
            }
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
            g_ui32TickPeriodInMillis = i;
            break;
        }
    }

    for (i = ui32Max; i < UINT32_MAX; i++)
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
            g_ui32CyclePeriodInMillis = i;
            break;
        }
    }

    HardExecTimer_init();
    TickTimer_init(g_ui32TickPeriodInMillis, _TickTimerCallBAck);

    g_bIsValidToRun = true;
    return g_bIsValidToRun;
}

void TTScheduler_start(void)
{
    if (g_bIsValidToRun)
    {
        TickTimer_start();
    }

    while (1)
    {

    }
}

bool TTScheduler_resumeTask(uint32_t ui32ID)
{
    uint32_t i;
    bool bTaskFound;

    for (i = 0; i < g_ui32NumOfTasks; i++)
    {
        if (g_ptTaskArray[i].ui32ID == ui32ID)
        {
            g_ptTaskArray[i].bEnabled = true;
            bTaskFound = true;
            break;
        }
    }

    if (i == g_ui32NumOfTasks)
    {
        bTaskFound = false;
    }

    return bTaskFound;
}

bool TTScheduler_suspendTask(uint32_t ui32ID)
{
    uint32_t i;
    bool bTaskFound;

    for (i = 0; i < g_ui32NumOfTasks; i++)
    {
        if (g_ptTaskArray[i].ui32ID == ui32ID)
        {
            g_ptTaskArray[i].bEnabled = false;
            bTaskFound = true;
            break;
        }
    }

    if (i == g_ui32NumOfTasks)
    {
        bTaskFound = false;
    }

    return bTaskFound;
}

uint32_t TTScheduler_getTicks(void)
{
    return g_ui32TicksInMillis;
}

uint32_t TTScheduler_getTickPeriod(void)
{
    return g_ui32TickPeriodInMillis;
}

uint32_t TTScheduler_getCyclePeriod(void)
{
    return g_ui32CyclePeriodInMillis;
}

