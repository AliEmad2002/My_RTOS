/*
 * Delay.c
 *
 *  Created on: Mar 24, 2023
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include <diag/trace.h>
#include "cmsis_gcc.h"

/*	MCAL	*/
#include "SCB_interface.h"

/*	RTOS	*/
#include "RTOS_Config.h"
#include "TCB.h"
#include "Blocked_List.h"
#include "Scheduler.h"

/*	SELF	*/
#include "Delay.h"




void RTOS_Delay(u32 systemTicks)
{
	/*	get pointer to the running TCB	*/
	RTOS_TCB_t* runningTcbPtr = RTOS_Scheduler_ptrGetRunningTcb();

	/*	update its blocking state and blocking info	*/
	runningTcbPtr->blockingReason = RTOS_TCB_BlockingReason_Time;
	runningTcbPtr->targetReadyTime = RTOS_Scheduler_u64GetSystemTime() + systemTicks;
	runningTcbPtr->isBlocked = true;

	/*
	 * block from here, to avoid execution of any instructions in the task
	 * after "RTOS_Delay()" was called, in the time before scheduler takes control.
	 */
	while(RTOS_Scheduler_u64GetSystemTime() < runningTcbPtr->targetReadyTime)
	{
		//trace_printf("f1 waiting\n");
	}
}










