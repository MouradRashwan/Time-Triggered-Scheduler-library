/**
 * main.c
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#include "TTScheduler.h"

#define NO_OF_TASKS     2

#define TASK1_ID        1
#define TASK2_ID        2

void Task_fun1(void);
void Task_fun2(void);

void System_init(void);
void LED_init(void);
void UARTstdio_init(void);

Task_t g_atTask[NO_OF_TASKS] =
{
    { TASK2_ID, 50, Task_fun2, 0, 0 }, // Higher priority
    { TASK1_ID, 1000, Task_fun1, 0, 0 } // Lower priority
};

int main(void)
{
    System_init();
    LED_init();
    UARTstdio_init();

    if (TTScheduler_init(g_atTask, NO_OF_TASKS) == true)
    {
        TTScheduler_start();
    }
    else
    {
        UARTprintf(
                "\n\n <<ERROR>> Scheduler is not initialized properly. \n\n");
    }
}

void Task_fun1(void)
{
    static uint8_t s_ui8Val = 0;

    if (s_ui8Val == 0)
    {
        s_ui8Val = GPIO_PIN_1;
        TTScheduler_suspendTask(TASK2_ID);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, s_ui8Val);
    }
    else
    {
        s_ui8Val = 0;
        TTScheduler_resumeTask(TASK2_ID);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, s_ui8Val);
    }
}

void Task_fun2(void)
{
    UARTprintf("Ticks:%5u - TickPeriod:%u - CyclePeriod:%u\n",
               TTScheduler_getTicks(), TTScheduler_getTickPeriod(),
               TTScheduler_getCyclePeriod());
}

void System_init(void)
{
    SysCtlClockSet(
    SYSCTL_SYSDIV_2_5 | SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN);
}

void LED_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,
    GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1,
                          GPIO_PIN_TYPE_STD_WPD);
}

void UARTstdio_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0))
    {
    }
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }

    GPIOPinConfigure(GPIO_PA0_U0RX); // set PCTL register for PA0 as UARTRx
    GPIOPinConfigure(GPIO_PA1_U0TX); // set PCTL register for PA1 as UARTTx
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1); // enable alternate function on PA0, PA1

    UARTStdioConfig(0, 115200, SysCtlClockGet());
}
