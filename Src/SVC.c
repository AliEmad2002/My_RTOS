/*
 * SVC.c
 *
 *  Created on: Mar 26, 2023
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
#include "SVC.h"

static RTOS_SVC_Buffer_t inputBuffer, outputBuffer;

void RTOS_SVC_voidBufferInput(u32 inputVal)
{
	inputBuffer.val = inputVal;
	inputBuffer.isRead = false;
}

void RTOS_SVC_voidIgnoreLastOutput(void)
{
	outputBuffer.isRead = true;
}

b8 RTOS_SVC_b8IsOutputBufferRead(void)
{
	return outputBuffer.isRead;
}

u32 RTOS_SVC_u32ReadOutputBuffer(void)
{
	outputBuffer.isRead = true;
	return outputBuffer.val;
}

void RTOS_SVC_Handler(void)
{
	return;
	/*
	 * input buffer is now expected to be holding address of the mutex - semaphore
	 * wanted by thread, read it, if available:
	 */
	u8* mutexPtr;

	if (inputBuffer.isRead == false)
	{
		mutexPtr = (u8*)inputBuffer.val;
		inputBuffer.isRead = true;
	}

	else
	{
		trace_printf("SVC called with no input!\n");
		while(1);
	}

	/*
	 * if mutex / semaphore is available, take it, and acknowledge that in output
	 * buffer:
	 */
	if (*mutexPtr)
	{
		(*mutexPtr)--;
		outputBuffer.val = 1;
	}

	/*	if not available, acknowledge that in output buffer	*/
	else
	{
		outputBuffer.val = 0;
	}

	/*	label the output buffer as not read	*/
	outputBuffer.isRead = false;

	/*
	 * disable all interrupts, to avoid scheduler execution and context switching
	 * on return from SVC_Handler:
	 */
	//__disable_irq();
}










