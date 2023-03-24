/*
 * Ready_Queue.c
 *
 *  Created on: Mar 20, 2023
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
#include "TCB_Queue.h"

/*	SELF	*/
#include "Ready_Queue.h"


/*
 * "readyQueueArr" is an array of queues, each queue is responsible for one
 * priority, i.e.: "readyQueueArr[0]" is for tasks of 0 priority, and so on.
 *
 * Notice: This implementation saves a lot of sorting time, and even more, it
 * eases the implementation of "Round-Robin" balancing algorithm.
 */
static RTOS_TCB_Queue_t readyQueueArr[RTOS_MAX_NUMBER_OF_PRIORITY_LEVELS];


/*******************************************************************************
 * Enqueuing TCB to the ready queue:
 ******************************************************************************/
void RTOS_Ready_Queue_voidEnqueue(RTOS_TCB_t* tcbPtr)
{
	/*	get pointer to the queue responsible for this TCB	*/
	RTOS_TCB_Queue_t* qPtr = &readyQueueArr[tcbPtr->pri];

	/*	enqueue	*/
	RTOS_TCB_Queue_voidEnqueue(qPtr, tcbPtr);
}

/*******************************************************************************
 * Dequeuing TCB from the ready queue to TCB object:
 ******************************************************************************/
void RTOS_Ready_Queue_voidDequeue(RTOS_TCB_t** distP, u8 pri)
{
	/*	get pointer to the queue responsible for this TCB	*/
	RTOS_TCB_Queue_t* qPtr = &readyQueueArr[pri];

	/*
	 * copy first of that queue to the argumented location.
	 *
	 * (just pointer is copied, as "RTOS_TCB_Queue_t" deals with data of type:
	 * "RTOS_TCB_t*")
	 */
	*distP = RTOS_TCB_Queue_ptrFirst(qPtr);

	/*	dequeue	*/
	RTOS_TCB_Queue_voidDequeue(qPtr);
}

/*******************************************************************************
 * Getting priority of the most urgent available TCB:
 ******************************************************************************/
u8 RTOS_Ready_Queue_u8GetPriOfMostUrgent()
{
	for (u16 i = 0; i < RTOS_MAX_NUMBER_OF_PRIORITY_LEVELS; i++)
	{
		if (readyQueueArr[i].size > 0)
			return i;
	}

	return NULL_PRI;
}















