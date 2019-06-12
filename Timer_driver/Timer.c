/*
 * Timer.c
 *
 *  Created on: Feb 14, 2019
 *      Author: Administrator
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "Timer_defs.h"
#include "Timer.h"

static void (*g_pfnCallBack)(void) = NULL;

static void _TickTimerISR(void)
{
    TimerIntClear(TICK_TIMER_ADDR_BASE, TIMER_TIMA_TIMEOUT);

    if (g_pfnCallBack != NULL)
    {
        g_pfnCallBack();
    }
}

void TickTimer_init(uint32_t ui32TickPeriodInMillis, void (*pfnCallBack)(void))
{
    g_pfnCallBack = pfnCallBack;
    SysCtlPeripheralEnable(TICK_TIMER_CLK_BASE);
    while (!SysCtlPeripheralReady(TICK_TIMER_CLK_BASE))
    {
    }
    TimerConfigure(TICK_TIMER_ADDR_BASE, TIMER_CFG_PERIODIC); // full-width periodic with timeout interrupt for timer0A & in disabled state
    TimerClockSourceSet(TICK_TIMER_ADDR_BASE, TIMER_CLOCK_SYSTEM); // set the input clock to the system clock
    TimerControlStall(TICK_TIMER_ADDR_BASE, TIMER_A, false); // continue counting while in debug mode
    TimerLoadSet(TICK_TIMER_ADDR_BASE, TIMER_A,
                 GET_TICKS_FROM_MILLIS(ui32TickPeriodInMillis)); // 1 tick = 12.5 ns // so // 80000000 ticks = 1 sec
    TimerIntEnable(TICK_TIMER_ADDR_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TICK_TIMER_ADDR_BASE, TIMER_A, _TickTimerISR);
}

void TickTimer_start(void)
{
    TimerEnable(TICK_TIMER_ADDR_BASE, TIMER_A); // enable timer
}

void HardExecTimer_init(void)
{
    SysCtlPeripheralEnable(HARD_EXEC_TIMER_CLK_BASE);
    while (!SysCtlPeripheralReady(HARD_EXEC_TIMER_CLK_BASE))
    {
    }
    TimerConfigure(HARD_EXEC_TIMER_ADDR_BASE, TIMER_CFG_ONE_SHOT); // full-width one-shot for timer1A & in disabled state
    TimerClockSourceSet(HARD_EXEC_TIMER_ADDR_BASE, TIMER_CLOCK_SYSTEM); // set the input clock to the system clock
    TimerControlStall(HARD_EXEC_TIMER_ADDR_BASE, TIMER_A, false); // continue counting while in debug mode
    TimerLoadSet(HARD_EXEC_TIMER_ADDR_BASE, TIMER_A, UINT32_MAX); // 1 tick = 12.5 ns // so // 80000000 ticks = 1 sec
}

void HardExecTimer_start(uint32_t ui32WorstCaseExecTimeInMillis)
{
    TimerLoadSet(HARD_EXEC_TIMER_ADDR_BASE, TIMER_A,
                 GET_TICKS_FROM_MILLIS(ui32WorstCaseExecTimeInMillis));
    TimerEnable(HARD_EXEC_TIMER_ADDR_BASE, TIMER_A); // enable timer
}

bool HardExecTimer_isTimeOut(void)
{
    if (TIMER_TIMA_TIMEOUT & TimerIntStatus(HARD_EXEC_TIMER_ADDR_BASE, false))
    {
        TimerIntClear(HARD_EXEC_TIMER_ADDR_BASE, TIMER_TIMA_TIMEOUT);
        return true;
    }
    else
    {
        return false;
    }
}
