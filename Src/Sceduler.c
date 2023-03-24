/*
 * Sceduler.c
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

/*	RTOS	*/
#include "RTOS_Config.h"
#include "TCB.h"
#include "TCB_Queue.h"
#include "Ready_Queue.h"
#include "Blocked_List.h"
#include "Thread.h"

/*	SELF	*/
#include "Sceduler.h"

static u64 elapsedTicks = 0;

static RTOS_TCB_t* ruuningTcbPtr;

/*******************************************************************************
 * SysTick handler:
 ******************************************************************************/
void SysTick_Handler(void)
{
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
ALWAYS_INLINE u64 RTOS_Sceduler_u64GetSystemTime(void)
{
	return elapsedTicks;
}

/*******************************************************************************
 * Running TCB getting:
 ******************************************************************************/
ALWAYS_INLINE RTOS_TCB_t* RTOS_Sceduler_ptrGetRunningTcb(void)
{
	return ruuningTcbPtr;
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
ALWAYS_INLINE_STATIC void CPU_to_TCB(void)
{
	ruuningTcbPtr->stackPtr = (u64*)((u32)Core_Regs_u32ReadPSP() - 10 * 4);

	GET_WORD_AT(ruuningTcbPtr->stackPtr, 2) = Core_Regs_u32ReadGPR4();
	GET_WORD_AT(ruuningTcbPtr->stackPtr, 3) = Core_Regs_u32ReadGPR5();
	GET_WORD_AT(ruuningTcbPtr->stackPtr, 4) = Core_Regs_u32ReadGPR6();
	GET_WORD_AT(ruuningTcbPtr->stackPtr, 5) = Core_Regs_u32ReadGPR7();
	GET_WORD_AT(ruuningTcbPtr->stackPtr, 6) = Core_Regs_u32ReadGPR8();
	GET_WORD_AT(ruuningTcbPtr->stackPtr, 7) = Core_Regs_u32ReadGPR9();
	GET_WORD_AT(ruuningTcbPtr->stackPtr, 8) = Core_Regs_u32ReadGPR10();
	GET_WORD_AT(ruuningTcbPtr->stackPtr, 9) = Core_Regs_u32ReadGPR11();

	GET_WORD_AT(ruuningTcbPtr->stackPtr, 1) = (u32)(Core_Regs_u8ReadCR());
}

ALWAYS_INLINE_STATIC void TCB_to_CPU(void)
{
	/*
	 * change PSP to the pointer of R0 in "runningTcb" stack frame, such that on
	 * return from handler, when the processor un-stacks R0-R3, R12, LR, PC and
	 * PSR, they are a copy of the ones stored in the stack of "runningTcb"
	 */
	Core_Regs_voidWritePSP((u32)ruuningTcbPtr->stackPtr + 10 * 4);

	/*
	 * As stack frame does not cover all core registers, copy R4-R11 from tcbC
	 * to CPU
	 */
	Core_Regs_voidWriteGPR4(GET_WORD_AT(ruuningTcbPtr->stackPtr, 2));
	Core_Regs_voidWriteGPR5(GET_WORD_AT(ruuningTcbPtr->stackPtr, 3));
	Core_Regs_voidWriteGPR6(GET_WORD_AT(ruuningTcbPtr->stackPtr, 4));
	Core_Regs_voidWriteGPR7(GET_WORD_AT(ruuningTcbPtr->stackPtr, 5));
	Core_Regs_voidWriteGPR8(GET_WORD_AT(ruuningTcbPtr->stackPtr, 6));
	Core_Regs_voidWriteGPR9(GET_WORD_AT(ruuningTcbPtr->stackPtr, 7));
	Core_Regs_voidWriteGPR10(GET_WORD_AT(ruuningTcbPtr->stackPtr, 8));
	Core_Regs_voidWriteGPR11(GET_WORD_AT(ruuningTcbPtr->stackPtr, 9));

	/*	same with CONTROL register	*/
	Core_Regs_voidWriteCR(GET_WORD_AT(ruuningTcbPtr->stackPtr, 1));
}

void RTOS_PendSV_Handler(void)
{
	/*	disable all interrupts	*/
	__disable_irq();

	static b8 firstTime = true;

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
	 * if it as the very first time PendSV interrupt handler is executed, there's
	 * no need for stacking the currently running task, as there's no such one.
	 */
	if (firstTime)
	{
		firstTime = false;
	}

	/*	other wise, process the currently running task	*/
	else
	{
		/*
		 * If priority of the most urgent ready task is less (larger in value) than
		 * that of the running task, no switching to happen.
		 *
		 * (notice that if they are equal, a switch will occur to achieve the Round-
		 * Robin balancing)
		 *
		 * And, if runningTcb has not been blocked
		 */
		if (ruuningTcbPtr->pri < priHighest && ruuningTcbPtr->isBlocked == false)
		{
			/*	Clear PendSV flag	*/
			SCB_CLR_PENDSV;

			return;
		}

		/*	otherwise	*/

		/*	stack core registers from CPU to ruuningTcb	*/
		CPU_to_TCB();

		if (ruuningTcbPtr->isBlocked)
			RTOS_Blocked_List_voidAdd(ruuningTcbPtr);

		else
			RTOS_Ready_Queue_voidEnqueue(ruuningTcbPtr);
	}

	/*
	 * dequeue the current most urgent TCB from the ready queue to runningTcb
	 */
	RTOS_Ready_Queue_voidDequeue(&ruuningTcbPtr, priHighest);

	/*
	 * Un-stack core registers:
	 *
	 * TODO:
	 *    As this function ("RTOS_PendSV_Handler()") is called in the assembly
	 * written "PendSV_Handler", stack-frame is stacked before calling,
	 * and gets un-stacked after returning, which overwrites what "TCB_to_CPU()"
	 * has done here.
	 *    To solve this problem, let the un-stacking from TCB to CPU take place
	 * after returning from "RTOS_PendSV_Handler()".
	 */
	TCB_to_CPU();

	/*	wait for an empty pipeline (recommended after CONTROL write)	*/
	__ISB();

	/*	Clear PendSV flag	*/
	SCB_CLR_PENDSV;

	/*	enable all interrupts	*/
	__enable_irq();
}

/*******************************************************************************
 * Init:
 ******************************************************************************/
void RTOS_Sceduler_voidIdleTaskFunction(void)
{
	while(1);
}

static u64 idleTaskStackArr[RTOS_IDLE_TASK_STACK_SIZE_IN_DWORDS];

void RTOS_Sceduler_voidInitSysTick(void)
{
	STK_voidInit();
	STK_voidReload(1000000);
	STK_voidEnableSysTick();

	NVIC_voidSetInterruptPriority(NVIC_Interrupt_Systick, 0, 0);
	NVIC_voidEnableInterrupt(NVIC_Interrupt_Systick);
}

void RTOS_Sceduler_voidInit(void)
{
	/*	disable all interrupts	*/
	__disable_irq();

	/*	init interrupt priority grouping	*/
	SCB_voidSetPriorityGroupsAndSubGroupsNumber(SCB_PRIGROUP_group4_sub4);

	/*	init SysTick and its interrupt	*/
	RTOS_Sceduler_voidInitSysTick();

	/*	init PendSV interrupt	*/
	NVIC_voidSetInterruptPriority(NVIC_Interrupt_PendSV, 3, 3);
	NVIC_voidEnableInterrupt(NVIC_Interrupt_PendSV);

	/*	create idle task	*/
	RTOS_Thread_voidCreate(
		RTOS_Sceduler_voidIdleTaskFunction,
		RTOS_MAX_NUMBER_OF_PRIORITY_LEVELS - 1,
		idleTaskStackArr, RTOS_IDLE_TASK_STACK_SIZE_IN_DWORDS);

	/*	pend PendSV interrupt	*/
	SCB_SET_PENDSV;

	/*	enable all interrupts	*/
	__enable_irq();
}
















