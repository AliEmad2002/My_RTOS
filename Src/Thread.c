/*
 * Thread.c
 *
 *  Created on: Mar 20, 2023
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include <diag/trace.h>

/*	MCAL	*/

/*	RTOS	*/
#include "RTOS_Config.h"
#include "TCB.h"
#include "TCB_Queue.h"
#include "Ready_Queue.h"

/*	SELF	*/
#include "Thread.h"

/******************************************************************************
 * Creation:
 ******************************************************************************/
void RTOS_Thread_voidCreate(
	void (*func)(void), u8 priority, u32* stackArr, u32 stackSizeInBytes)
{
	static u8 id = 0;

	if (id == 255)
	{
		trace_printf("Maximum number of tasks has been already created!\n");
		while(1);
	}

	/*	create temp TCB with these info	*/
	RTOS_TCB_t tcb;

	tcb.id				= id++;
	tcb.pri				= priority;
	tcb.stackSize		= stackSizeInBytes;
	tcb.baseStackPtr	= stackArr + stackSizeInBytes;

	/*
	 * TODO:
	 * init the stack segment:
	 */

	/*	enqueue this TCB in the ready queue	*/
	RTOS_Ready_Queue_voidEnqueue(&tcb);
}












