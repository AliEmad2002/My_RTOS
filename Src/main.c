/*
 * main.c
 * This is a simple demo for "My_RTOS".
 *
 *  Created on: Mar 18, 2023
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include <diag/trace.h>

/*	MCAL	*/
#include "Core_Regs_Interface.h"
#include "SCB_interface.h"
#include "RCC_interface.h"
#include "GPIO_interface.h"

/*	HAL	*/
#include "LED_Interface.h"

/*	RTOS	*/
#include "RTOS_Config.h"
#include "TCB.h"
#include "TCB_Queue.h"
#include "Ready_Queue.h"
#include "Thread.h"
#include "Sceduler.h"
#include "Delay.h"

/*
 * Tasks stacks are statically allocated by user.
 * (This gives configurability and static allocation)
 */
#define STACK_SIZE_DW	200

static u64 stack1[STACK_SIZE_DW];
static u64 stack2[STACK_SIZE_DW];
static u64 stackIdle[STACK_SIZE_DW];

void func1(void);
void func2(void);
void funcIdle(void);

LED_t led1, led2;

int main(void)
{
	/*	init RCC	*/
	RCC_voidSysClockInit();
	RCC_voidEnablePeripheralClk(RCC_Bus_APB2, RCC_PERIPHERAL_IOPB);

	/*	init LED's	*/
	LED_voidInit(&led1, GPIO_PortName_B, 12, GPIO_OutputLevel_High);
	LED_voidInit(&led2, GPIO_PortName_B, 13, GPIO_OutputLevel_High);

	/*	init tasks of LED1 & LED2	*/
	RTOS_Thread_voidCreate(func1, 0, stack1, STACK_SIZE_DW);
	RTOS_Thread_voidCreate(func2, 0, stack2, STACK_SIZE_DW);

	/*	init scheduler	*/
	RTOS_Sceduler_voidInit(funcIdle, stackIdle, STACK_SIZE_DW);

	while(1)
	{

	}

	return 0;
}

void func1(void)
{
	while(1)
	{
		LED_voidToggle(&led1);

		RTOS_Delay(200);
	}
}

void func2(void)
{
	while(1)
	{
		LED_voidToggle(&led2);

		RTOS_Delay(100);
	}
}

void funcIdle(void)
{
	while(1)
	{
		__asm("WFI");	/*	halts CPU until in interrupt occurs	*/
	}
}





