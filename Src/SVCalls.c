/*
 * SVCalls.c
 *
 *  Created on: Apr 6, 2023
 *      Author: Ali Emad
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
#include "Mutex.h"
#include "Scheduler.h"

/*	SELF	*/
#include "SVCalls.h"


/*******************************************************************************
 * Buffers for communicating with this driver:
 ******************************************************************************/
static u32 inputBuffer;
static u32 outputBuffer;

ALWAYS_INLINE void RTOS_SVCalls_voidSetInputBuffer(u32 val)
{
	inputBuffer = val;
}

ALWAYS_INLINE u32 RTOS_SVCalls_u32GetOutputBuffer(void)
{
	return outputBuffer;
}

/*******************************************************************************
 * SVC handler's associated functions:
 ******************************************************************************/
static void SVC_TakeMutex(void)
{
	RTOS_TCB_t* runningTcbPtr = RTOS_Scheduler_ptrGetRunningTcb();

	/*
	 * taking semaphore or mutex, which pointer will be initially set in
	 * "runningTcbPtr->mutexPtr"
	 */
	/*	if mutex / semaphore is available	*/
	if (*(runningTcbPtr->mutexPtr))
	{
		/*	take it	*/
		(*(runningTcbPtr->mutexPtr))--;
		/*	return to continue in the thread that've requested it	*/
		return;
	}

	/*
	 * otherwise, block that thread, and execute scheduler on return.
	 * (pend PendSV interrupt)
	 */
	runningTcbPtr->blockingReason = RTOS_TCB_BlockingReason_Mutex;
	runningTcbPtr->isBlocked = true;

	SCB_SET_PENDSV;
}

/*******************************************************************************
 * SVC handler:
 ******************************************************************************/
void SVC_Handler(void)
{
	/**
	 * Read SVCall imm integer value.
	 * (It is read by reading the SVC instruction that moved control to this
	 * handler. i.e.: it is retrieved from the stacked PC - 1 instruction size)
	 **/

	u64* returnStackPtr;
	/*
	 * get the stack pointer used on return from handler (process or main).
	 * (By reading the bit EXC_RETURN[2], i.e.: LR[2])
	 */
	u32 lr = Core_Regs_u32ReadLR();
	u8 returnStack = GET_BIT(lr, 2);
	if (returnStack)
		returnStackPtr = (u64*)Core_Regs_u32ReadPSP();
	else
		returnStackPtr = (u64*)Core_Regs_u32ReadMSP();

	/*
	 * get the program counter used on return from handler.
	 * (it is the 7th word in the stack frame)
	 */
	u32 returnPc = GET_WORD_AT(returnStackPtr, 6);

	/*
	 * get address of the instruction that moved control to this handler.
	 * (i.e.: SVCall instruction)
	 */
	u32* svcAddress = (u32*)(returnPc - 2);

	/*
	 * Finally, get imm value.
	 * SVC instruction format: https://developer.arm.com/documentation/ddi0406/c/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/SVC--previously-SWI-?lang=en
	 */
	volatile u8 imm = (u8)((*svcAddress) & 0xFF);

	switch(imm)
	{
	case 0:
		/*	Schedule	*/
		SCB_SET_PENDSV;
		break;

	case 1:
		SVC_TakeMutex();
		break;
	}
}













