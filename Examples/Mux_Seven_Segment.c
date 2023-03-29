/*
 * Mux_Seven_Segment.c
 *
 *  Created on: Mar 25, 2023
 *      Author: Ali Emad Ali
 *
 *  - This code would have two tasks, both would want to print certain value
 *    on two-digit 7-segment display.
 *
 *  - 7-segment display is Mutex-protected.
 *
 *  - The two 7-segment displays are multiplexed, a third task would operate them.
 *
 *  - The first mentioned two tasks communicate with the third through a buffer
 *    of length 1.
 */

#if 0

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
#include "Seven_Segment_interface.h"

/*	RTOS	*/
#include "My_RTOS.h"

/*	Tasks and their stack segments	*/
void func1(void);
void func2(void);
void funcPrint(void);
void funcIdle(void);

#define TASK_STACK_SIZE_DW		200
static u64 stack1[TASK_STACK_SIZE_DW];
static u64 stack2[TASK_STACK_SIZE_DW];
static u64 stackPrint[TASK_STACK_SIZE_DW];
static u64 stackIdle[11];

/*	Seven segment object	*/
Seven_Segment_t sevenSegment;

/*	Mutex	*/
static u8 sevenSegmentMutex = 1;	// initially available

/*	message buffer (Queue)	*/
RTOS_MsgBuffer_t msgBuffer;

int main(void)
{
	/*	init RCC	*/
	RCC_voidSysClockInit();
	RCC_voidEnablePeripheralClk(RCC_Bus_APB2, RCC_PERIPHERAL_IOPB);

	/*	init seven segment object	*/


	/*	init tasks	*/
	RTOS_Thread_voidCreate(func1,		1, stack1, 		TASK_STACK_SIZE_DW);
	RTOS_Thread_voidCreate(func2, 		1, stack2, 		TASK_STACK_SIZE_DW);
	RTOS_Thread_voidCreate(funcPrint, 	0, stackPrint, 	TASK_STACK_SIZE_DW);

	/*	init scheduler	*/
	RTOS_Scheduler_voidInit(funcIdle, stackIdle, 11);

	while(1)
	{

	}

	return 0;
}

void func1(void)
{

}

void func2(void)
{

}

void funcPrint(void)
{

}

void funcIdle(void)
{
	while(1)
	{
		__asm("WFI");	/*	halts CPU until in interrupt occurs	*/
	}
}

















#endif
