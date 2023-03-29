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

/*
 * As "My_RTOS" uses static memory allocation, all TCB's are initially allocated
 * in the following array.
 *
 * On thread creation, a non used element of the following array is given to the
 * new thread, and its pointer is enqueued to the ready queue.
 */
static RTOS_TCB_t tcbArr[RTOS_MAX_NUMBER_OF_TCBS];

/******************************************************************************
 * Creation:
 ******************************************************************************/
void RTOS_Thread_voidCreate(
	void (*func)(void), u8 priority, u64* stackArr, u32 stackSizeInDWords)
{
	static u8 id = 0;

	if (id == RTOS_MAX_NUMBER_OF_TCBS)
	{
		trace_printf("Maximum number of tasks has been already created!\n");
		while(1);
	}

	/* get pointer first non-used TCB from "tcbArr"	*/
	RTOS_TCB_t* tcbPtr = &tcbArr[id];

	/*	assign data of the new thread to its TCB	*/
	tcbPtr->id					= id++;
	tcbPtr->pri					= priority;
	tcbPtr->stackSizeInDWrods	= stackSizeInDWords;
	tcbPtr->mutexPtr			= NULL;

	/*
	 * As this thread will first time run on return from scheduler (PendSV)
	 * interrupt handler, the initial stack pointer has to be as if 18 words were
	 * pushed, these are the core registers (stack frame + rest of the registers).
	 *
	 * Info on stack frame:
	 * https://developer.arm.com/documentation/ddi0337/e/Exceptions/Pre-emption/Stacking
	 */
	tcbPtr->stackPtr = (u64*)((u32)stackArr + (u32)(stackSizeInDWords * 8 - 18 * 4));

	/*	PSR bit 24 is always set to 1 (from STM's DocID15491 Rev 6, P19)	*/
	GET_WORD_AT(stackArr, stackSizeInDWords * 2 - 1) = 1 << 24;

	/*	PC points at function of the thread	*/
	GET_WORD_AT(stackArr, stackSizeInDWords * 2 - 2) = (u32)func;

	/*
	 * EXC_RETURN has to indicate returning from PendSV interrupt handler to
	 * thread mode using PSP.
	 * (from STM's DocID15491 Rev 6, P39)
	 */
	GET_WORD_AT(stackArr, stackSizeInDWords * 2 - 18) = 0xFFFFFFF0 | 0b1101;

	/*	CONTROL: unprivileged, use PSP	*/
	GET_WORD_AT(stackArr, stackSizeInDWords * 2 - 17) = 0b11;

	/*	enqueue this TCB in the ready queue	*/
	RTOS_Ready_Queue_voidEnqueue(tcbPtr);
}












