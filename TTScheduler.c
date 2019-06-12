/*
 * TTScheduler.c
 *
 *  Created on: Feb 14, 2019
 *      Author: Administrator
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "TimerTick_driver/TimerTick_driver.h"

#include "TTScheduler.h"

static Task_t *g_ptTaskArray = NULL;
static uint32_t g_ui32NumOfTasks = 0;
static uint32_t g_ui32TicksInMillis = 0;
static uint32_t g_ui32TickPeriodInMillis = 0;
static uint32_t g_ui32CyclePeriodInMillis = 0;

static void _TimerTickCallBack(void)
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
                g_ptTaskArray[i].pfnTaskFun();
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
        return false;
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

        for (j = i + 1; j < g_ui32NumOfTasks; j++)
        {
            if (g_ptTaskArray[i].ui32ID == g_ptTaskArray[j].ui32ID)
            {
                return false;
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

    TimerTick_init(g_ui32TickPeriodInMillis, _TimerTickCallBack);

    return true;
}

void TTScheduler_start(void)
{
    TimerTick_start();

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

