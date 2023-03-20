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

















