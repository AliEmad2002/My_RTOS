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
	RTOS_TCB_t* runningTcbPtr = RTOS_Scheduler_ptrGetRunningTcb();

	runningTcbPtr->mutexPtr = mutexPtr;
	runningTcbPtr->targetReadyTime = RTOS_Scheduler_u64GetSystemTime() + timeOut;

	__asm volatile ("svc #1");
}

ALWAYS_INLINE void RTOS_Mutex_voidGive(u8* mutexPtr)
{
	/*	increment it	*/
	(*mutexPtr)++;

	/*	call the scheduler, to achieve Round-Robin on that mutex	*/
	__asm volatile ("svc #0");
}









