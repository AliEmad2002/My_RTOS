/*
 * Mutex.c
 *
 *  Created on: Mar 27, 2023
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include <diag/trace.h>
#include <cmsis_gcc.h>

/*	MCAL	*/
#include "Core_Regs_Interface.h"
#include "SCB_interface.h"
#include "NVIC_interface.h"
#include "STK_interface.h"
#include "RCC_interface.h"

/*	RTOS	*/
#include "RTOS_Config.h"
#include "TCB.h"
#include "TCB_Queue.h"
#include "Ready_Queue.h"
#include "Blocked_List.h"
#include "Thread.h"
#include "Scheduler.h"

/*	SELF	*/
#include "Mutex.h"


u8 RTOS_Mutex_u8Take(u8* mutexPtr, u32 timeOut)
{
	/*	get pointer to the running thread (the one requested the mutex)	*/
	RTOS_TCB_t* runningTcbPtr = RTOS_Scheduler_ptrGetRunningTcb();

	/*
	 * assign these data, so that SVCall and scheduler know exactly what this
	 * thread wants.
	 */
	runningTcbPtr->mutexPtr = mutexPtr;
	runningTcbPtr->targetReadyTime = RTOS_Scheduler_u64GetSystemTime() + timeOut;

	/*	make an SVCall to take mutex / semaphore	*/
	__asm volatile ("svc #1");

	/*	on task resuming after mutex taking or timeout passing	*/
	/*
	 * if "runningTcbPtr->mutexPtr" has been changed to "NULL" by the un-blocking
	 * functionality of the scheduler, it means mutex / semaphore has been taken.
	 */
	if (runningTcbPtr->mutexPtr == NULL)
		return 1;

	/*
	 * Otherwise, it means mutex / semaphore has not been taken int the given
	 * timeout.
	 */
	else
		return 0;
}

ALWAYS_INLINE void RTOS_Mutex_voidGive(u8* mutexPtr)
{
	/*	increment it	*/
	(*mutexPtr)++;

	/*	call the scheduler, to achieve Round-Robin on that mutex	*/
	__asm volatile ("svc #0");
}









