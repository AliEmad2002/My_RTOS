/*
 * Mutex_LEDs.c
 *
 *  Created on: Mar 27, 2023
 *      Author: Ali Emad Ali
 */

#if 1	/*	change this to 1, to use this example	*/

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include <diag/trace.h>
#include "Delay_interface.h"

/*	MCAL	*/
#include "Core_Regs_Interface.h"
#include "SCB_interface.h"
#include "RCC_interface.h"
#include "GPIO_interface.h"

/*	HAL	*/
#include "LED_Interface.h"

/*	RTOS	*/
#include "My_RTOS.h"

#define STACK_SIZE_DW	200

static u64 stack1[STACK_SIZE_DW];
static u64 stack2[STACK_SIZE_DW];
static u64 stackIdle[11];

void func1(void);
void func2(void);
void funcIdle(void);

LED_t led1, led2;

static u8 ledMutex = true; // initially available.

int main(void)
{
	/*	init RCC	*/
	RCC_voidSysClockInit();
	RCC_voidEnablePeripheralClk(RCC_Bus_APB2, RCC_PERIPHERAL_IOPB);

	/*	init LED's	*/
	LED_voidInit(&led1, GPIO_PortName_B, 12, GPIO_OutputLevel_High);
	LED_voidInit(&led2, GPIO_PortName_B, 13, GPIO_OutputLevel_High);

	while(1);

	/*	init tasks of LED1 & LED2	*/
	RTOS_Thread_voidCreate(func1, 0, stack1, STACK_SIZE_DW);
	RTOS_Thread_voidCreate(func2, 0, stack2, STACK_SIZE_DW);

	/*	init scheduler	*/
	RTOS_Scheduler_voidInit(funcIdle, stackIdle, 11);

	while(1)
	{

	}

	return 0;
}

void func1(void)
{
	while(1)
	{
		RTOS_Mutex_u8Take(&ledMutex, 0);

		LED_voidSetActive(&led1);
		LED_voidSetInactive(&led2);

		RTOS_Delay(200);

		RTOS_Mutex_voidGive(&ledMutex);
	}
}

void func2(void)
{
	while(1)
	{
		RTOS_Mutex_u8Take(&ledMutex, 0);

		LED_voidSetActive(&led2);
		LED_voidSetInactive(&led1);

		RTOS_Delay(100);

		RTOS_Mutex_voidGive(&ledMutex);
	}
}

void funcIdle(void)
{
	while(1)
	{
		__asm("WFI");	/*	halts CPU until an interrupt occurs	*/
	}
}


#endif
