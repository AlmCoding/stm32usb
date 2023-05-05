/*
 * hooks.cpp
 *
 *  Created on: May 5, 2023
 *      Author: Alexander L.
 */

#include "cmsis_os.h"
#include "task.h"

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

void configureTimerForRunTimeStats(void)
{
}

unsigned long getRunTimeCounterValue(void)
{
return 0;
}

void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
