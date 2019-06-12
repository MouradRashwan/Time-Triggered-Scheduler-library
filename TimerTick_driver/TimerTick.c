/*
 * TimerTick.c
 *
 *  Created on: Feb 14, 2019
 *      Author: Administrator
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "TimerTick_driver.h"

static void (*g_pfnCallBack)(void) = NULL;

static void _TimerTickISR(void)
{
    TimerIntClear(TIMER_ADDR_BASE, TIMER_TIMA_TIMEOUT);

    if (g_pfnCallBack != NULL)
    {
        g_pfnCallBack();
    }
}

void TimerTick_init(uint32_t ui32TickPeriodInMillis, void (*pfnCallBack)(void))
{
    g_pfnCallBack = pfnCallBack;
    SysCtlPeripheralEnable(TIMER_CLK_BASE);
    while (!SysCtlPeripheralReady(TIMER_CLK_BASE))
    {
    }
    TimerConfigure(TIMER_ADDR_BASE, TIMER_CFG_PERIODIC); // full-width periodic with timeout interrupt for timer3A & in disabled state
    TimerClockSourceSet(TIMER_ADDR_BASE, TIMER_CLOCK_SYSTEM); // set the input clock to the system clock
    TimerControlStall(TIMER_ADDR_BASE, TIMER_A, false); // continue counting while in debug mode
    TimerLoadSet(TIMER_ADDR_BASE, TIMER_A,
                 GET_TICKS_FROM_MILLIS(ui32TickPeriodInMillis)); // 1 tick = 12.5 ns // so // 80000000 ticks = 1 sec
    TimerIntEnable(TIMER_ADDR_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER_ADDR_BASE, TIMER_A, _TimerTickISR);
}

void TimerTick_start(void)
{
    TimerEnable(TIMER_ADDR_BASE, TIMER_A); // enable timerA
}
