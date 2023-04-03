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


void RTOS_Mutex_voidTake(u8* mutexPtr)
{
	RTOS_TCB_t* runningTcbPtr = RTOS_Scheduler_ptrGetRunningTcb();
	runningTcbPtr->mutexPtr = mutexPtr;
	__asm volatile ("svc #0");
}

ALWAYS_INLINE void RTOS_Mutex_voidGive(u8* mutexPtr)
{
	(*mutexPtr)++;
}









