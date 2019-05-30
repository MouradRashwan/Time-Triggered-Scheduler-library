/**
 * main.c
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>

#include "driverlib/cpu.h"
#include "driverlib/pin_map.h"
#include "driverlib/eeprom.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/fpu.h"
#include "utils/uartstdio.h"

#include "TTScheduler_driver.h"

#define NO_OF_TASKS     2

/* by default the variadic function allocates 4-bytes (bus-size) for each argument in (...) regarding the type of argument */
uint32_t VarFun_sumArgs(uint32_t ui32Count, ...)
{
    uint32_t i, ui32Sum = 0;
    va_list tVarArgList;

    /* after executing this function the [tVarArgList] will start after this parameter [ui32Count] */
    va_start(tVarArgList, ui32Count);

    for (i = 0; i < ui32Count; i++)
    {
        ui32Sum += va_arg(tVarArgList, uint32_t);
    }

    va_end(tVarArgList);

    return ui32Sum;
}

void Task_fun0(void);

void Task_fun1(void);

void System_init(void);

void delayMillis(uint32_t ui32Millis);

void LED_init(void);

void UARTstdio_init(void);

uint32_t EEPROM_init(void);

const uint32_t g_ui32FlashData = 0;
uint32_t g_ui32DataNoInit __attribute__((noinit));

Task_t g_atTask[NO_OF_TASKS] = { { 1, 1000, Task_fun0 }, { 1, 50, Task_fun1 } };

int main(void)
{

    uint32_t ui32Write = 0, ui32Read = 0, ui32Success = 0;

    System_init();
    EEPROM_init();
    LED_init();
    UARTstdio_init();

    TTScheduler_init(g_atTask, NO_OF_TASKS);
    TTScheduler_start();

    while (1)
    {
        CPUwfi();

//        uint32_t *pui32Data = (uint32_t*) &g_ui32FlashData;
//        (*pui32Data) += 1;
//        g_ui32DataNoInit += 1;
//        UARTprintf("FLASH-DATA = %u %u | NO_INIT_DATA = %u \n", g_ui32FlashData,
//                   *pui32Data, g_ui32DataNoInit);
//        delayMillis(1000);

//        EEPROMProgram(&ui32Write, 0, 4);
//        EEPROMRead(&ui32Read, 0, 4);
//
//        if (ui32Write == ui32Read)
//        {
//            ui32Success = 1;
//        }
//        else
//        {
//            ui32Success = 0;
//        }
//
//        UARTprintf("WRITE = %u | READ = %u | [%u]\n", ui32Write, ui32Read,
//                   ui32Success);
//
//        ui32Write++;
//
//        delayMillis(100);

//        UARTprintf("SUM = %u \n", VarFun_sumArgs(5, 10, 20, 30, 40, 50));
//        delayMillis(1000);
    }
}

void Task_fun0(void)
{
    static uint8_t s_ui8Val = 0;

    if (s_ui8Val == 0)
    {
        s_ui8Val = GPIO_PIN_1;
        TTScheduler_suspendTask(&g_atTask[1]);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, s_ui8Val);
    }
    else
    {
        s_ui8Val = 0;
        TTScheduler_resumeTask(&g_atTask[1]);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, s_ui8Val);
    }
}

void Task_fun1(void)
{
    float dResult = powf(2.3, 3.0);

    UARTprintf("Ticks:%05u - TickPeriod:%u - CyclePeriod:%u - %u.%u \n",
               TTScheduler_getTicks(), TTScheduler_getTickPeriod(),
               TTScheduler_getCyclePeriod(), (uint32_t) dResult,
               (uint32_t) ((dResult - (uint32_t) dResult) * 1000000.0f));
}

void System_init(void)
{
    SysCtlClockSet(
    SYSCTL_SYSDIV_2_5 | SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN);

    FPULazyStackingEnable();
    FPUEnable();
}

void delayMillis(uint32_t ui32Millis)
{
    uint32_t ix;
    uint32_t ui32Period = (SysCtlClockGet() / 1000U) / 3U;
    for (ix = 0; ix < ui32Millis; ix++)
    {
        SysCtlDelay(ui32Period);
    }
}

uint32_t EEPROM_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_EEPROM0))
    {
    }

    return EEPROMInit();
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
