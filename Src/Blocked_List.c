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

/*	MCAL	*/

/*	RTOS	*/
#include "RTOS_Config.h"
#include "TCB.h"
#include "Scheduler.h"

/*	SELF	*/
#include "Blocked_List.h"


/*	allocate links statically	*/
static RTOS_BlockedElement_t blockedArr[RTOS_MAX_NUMBER_OF_TCBS] = {0};

/*******************************************************************************
 * Adding TCB (pointer) to the blocked list:
 ******************************************************************************/
void RTOS_Blocked_List_voidAdd(RTOS_TCB_t* tcbPtr)
{
	/*	find index of the first non used element in "blockedArr":	*/
	u8 i = 0;
	while(blockedArr[i].isUsed)
		i++;

	/*	add at it	*/
	blockedArr[i].isUsed = true;
	blockedArr[i].tcbPtr = tcbPtr;
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
		if (blockedArr[i].isUsed)
		{
			/*	get pointer of this TCB	*/
			RTOS_TCB_t* tcbPtr = blockedArr[i].tcbPtr;

			/*	if it was time blocked:	*/
			if (tcbPtr->blockingReason == RTOS_TCB_BlockingReason_Time)
			{
				/*	if ready time has come	*/
				if (tcbPtr->targetReadyTime <= RTOS_Scheduler_u64GetSystemTime())
				{
					blockedArr[i].isUsed = false;
					tcbPtr->isBlocked = false;
					*distP = tcbPtr;
					return true;
				}
			}

			/*	otherwise, if blocking reason was mutex or  semaphore	*/
			else
			{
				/*	if mutex or semaphore is available	*/
				if (*(tcbPtr->mutexPtr))
				{
					/*
					 * take it.
					 * TODO: since mutex and semaphore are not yet implemented,
					 * this is just a primary imagination.
					 */
					(*(tcbPtr->mutexPtr))--;
					tcbPtr->isBlocked = false;
					*distP = tcbPtr;
					return true;
				}
			}
		}
	}

	/*	if no blocked threads were removed from the blocking list	*/
	return false;
}













