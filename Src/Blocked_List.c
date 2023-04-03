/*
 * Blocked_List.c
 *
 *  Created on: Mar 23, 2023
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include <diag/trace.h>
#include "cmsis_gcc.h"

/*	MCAL	*/

/*	RTOS	*/
#include "RTOS_Config.h"
#include "TCB.h"
#include "Scheduler.h"

/*	SELF	*/
#include "Blocked_List.h"


/*	allocate links statically	*/
static RTOS_TCB_t* blockedTcbPtrArr[RTOS_MAX_NUMBER_OF_TCBS] = {NULL};

/*******************************************************************************
 * Adding TCB (pointer) to the blocked list:
 ******************************************************************************/
void RTOS_Blocked_List_voidAdd(RTOS_TCB_t* tcbPtr)
{
	/*	find index of the first non used element in "blockedArr":	*/
	u8 i = 0;
	while(blockedTcbPtrArr[i] != NULL)
		i++;

	/*	add at it	*/
	blockedTcbPtrArr[i] = tcbPtr;
}

/*******************************************************************************
 * Checking blocking reason of all blocked threads.
 * Unblocks and returns first hit of a now-ready thread.
 ******************************************************************************/
b8 RTOS_Blocked_List_b8Unblock(RTOS_TCB_t** distP)
{
	/*
	 * for every element in "blockedArr":
	 * TODO: this is a huge complexity for such a task!
	 */
	for (u8 i = 0; i < RTOS_MAX_NUMBER_OF_TCBS; i++)
	{
		/*	if there's a blocked thread at this index:	*/
		if (blockedTcbPtrArr[i] != NULL)
		{
			/*	get pointer of this TCB	*/
			RTOS_TCB_t* tcbPtr = blockedTcbPtrArr[i];

			/*	if it was time blocked:	*/
			if (tcbPtr->blockingReason == RTOS_TCB_BlockingReason_Time)
			{
				/*	if ready time has come	*/
				if (tcbPtr->targetReadyTime <= RTOS_Scheduler_u64GetSystemTime())
				{
					blockedTcbPtrArr[i] = NULL;
					tcbPtr->isBlocked = false;
					*distP = tcbPtr;
					return true;
				}
			}

			/*	otherwise, if blocking reason was mutex or  semaphore	*/
			else
			{
				if (*(tcbPtr->mutexPtr))
				{
					(*(tcbPtr->mutexPtr))--;
					blockedTcbPtrArr[i] = NULL;
					tcbPtr->isBlocked = false;
					tcbPtr->mutexPtr = NULL;
					*distP = tcbPtr;
					return true;
				}
			}
		}
	}

	/*	if no blocked threads were removed from the blocking list	*/
	return false;
}













