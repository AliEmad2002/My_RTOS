/*
 * Scheduler.c
 *
 *  Created on: Mar 20, 2023
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
#include "Mutex.h"

/*	SELF	*/
#include "Scheduler.h"

static u64 elapsedTicks = 0;

RTOS_TCB_t* runningTcbPtr;

/*******************************************************************************
 * SysTick handler:
 ******************************************************************************/
void SysTick_Handler(void)
{
	//trace_printf("                    hey from SysTick handler!\n");
	/*	pend PendSV interrupt	*/
	SCB_SET_PENDSV;

	/*	increment count of elapsed RTOS ticks	*/
	elapsedTicks++;

	/*	clear STK pending flag	*/
	STK_b8GetAndClearInterruptFlag();
}

/*******************************************************************************
 * System time getting:
 ******************************************************************************/
ALWAYS_INLINE u64 RTOS_Scheduler_u64GetSystemTime(void)
{
	return elapsedTicks;
}

/*******************************************************************************
 * Running TCB getting:
 ******************************************************************************/
ALWAYS_INLINE RTOS_TCB_t* RTOS_Scheduler_ptrGetRunningTcb(void)
{
	return runningTcbPtr;
}

/*******************************************************************************
 * PendSV handler:
 * Also consider: "PendSV_Handler.S"
 *
 * The reason I split this code to two parts, .S and .c, is that the handler when
 * implemented in .c, when exiting, "LR" is unstacked with whatever value the
 * processor has stacked at handler's entry. Therefore, I couldn't write My_RTOS
 * costum EXC_RETURN value to "LR", because it will be omitted by the unstacked
 * one any way.
 * While, assembly coding reveals this whole scene, and "LR" could be safely
 * written.
 ******************************************************************************/
void RTOS_PendSV_Handler(void)
{
	/*	unblock blocked tasks which deserve unblocking	*/
	while(1)
	{
		RTOS_TCB_t* newUnblockedTcbPtr;

		/*	if there's not, break	*/
		if (!RTOS_Blocked_List_b8Unblock(&newUnblockedTcbPtr))
			break;

		/*	other wise, add "newUnblockedTcb" to the ready queue	*/
		RTOS_Ready_Queue_voidEnqueue(newUnblockedTcbPtr);
	}

	/*	get priority of the most urgent ready task	*/
	u8 priHighest = RTOS_Ready_Queue_u8GetPriOfMostUrgent();

	/*
	 * If priority of the most urgent ready task is less (larger in value) than
	 * that of the running task, no switching to happen.
	 *
	 * (notice that if they are equal, a switch will occur to achieve the Round-
	 * Robin balancing)
	 *
	 * And, if runningTcb has not been blocked
	 */
	if (runningTcbPtr->pri < priHighest && runningTcbPtr->isBlocked == false)
	{
		/*	Clear PendSV flag	*/
		SCB_CLR_PENDSV;

		return;
	}

	/*	otherwise	*/

	if (runningTcbPtr->isBlocked)
		RTOS_Blocked_List_voidAdd(runningTcbPtr);

	else
		RTOS_Ready_Queue_voidEnqueue(runningTcbPtr);

	/*
	 * dequeue the current most urgent TCB from the ready queue to runningTcb
	 */
	RTOS_Ready_Queue_voidDequeue(&runningTcbPtr, priHighest);

	/*	Clear PendSV flag	*/
	SCB_CLR_PENDSV;
}

/*******************************************************************************
 * SVC handler:
 ******************************************************************************/
void SVC_Handler(void)
{
	/*
	 * So far, SVC is called for only taking semaphore or mutex, which pointer
	 * will be initially set in "runningTcbPtr->mutexPtr"
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
 * Init:
 ******************************************************************************/
void RTOS_Scheduler_voidInitSysTick(void)
{
	STK_voidInit();
	STK_voidReload(RCC_u32GetSysInClk() / 1000);
	STK_voidEnableSysTick();

	NVIC_voidSetInterruptPriority(NVIC_Interrupt_Systick, 1, 0);
	NVIC_voidEnableInterrupt(NVIC_Interrupt_Systick);
}

void RTOS_Scheduler_voidInit(
	void (*idleFunc)(void), u64* idleStackArr, u32 idleStackSizeInDWords)
{
	/*	disable all interrupts	*/
	__disable_irq();

	/*	init interrupt priority grouping	*/
	SCB_voidSetPriorityGroupsAndSubGroupsNumber(SCB_PRIGROUP_group16_sub0);

	/*	init SVC interrupt	*/
	NVIC_voidSetInterruptPriority(NVIC_Interrupt_SVCall, 0, 0);

	/*	init SysTick and its interrupt	*/
	RTOS_Scheduler_voidInitSysTick();

	/*	init PendSV interrupt	*/
	NVIC_voidSetInterruptPriority(NVIC_Interrupt_PendSV, 15, 0);
	NVIC_voidEnableInterrupt(NVIC_Interrupt_PendSV);

	/*	create idle task	*/
	RTOS_Thread_voidCreate(
		idleFunc,
		RTOS_MAX_NUMBER_OF_PRIORITY_LEVELS - 1,
		idleStackArr, idleStackSizeInDWords);

	/*	running TCB is initially the idle one	*/
	RTOS_Ready_Queue_voidDequeue(&runningTcbPtr, RTOS_MAX_NUMBER_OF_PRIORITY_LEVELS - 1);
	Core_Regs_voidWritePSP((u32)(runningTcbPtr->stackPtr) + 40);

	/*	pend PendSV interrupt	*/
	SCB_SET_PENDSV;

	/*	enable all interrupts	*/
	__enable_irq();
}
















