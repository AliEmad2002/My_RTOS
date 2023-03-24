/*
 * TCB_Queue.c
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

/*	SELF	*/
#include "TCB_Queue.h"


/*******************************************************************************
 * Init:
 ******************************************************************************/
void RTOS_TCB_Queue_voidInit(RTOS_TCB_Queue_t* q)
{
	q->head = 0;
	q->tail = 0;
	q->size = 0;
}

/*******************************************************************************
 * Data accessing:
 ******************************************************************************/
RTOS_TCB_t* RTOS_TCB_Queue_ptrFirst(RTOS_TCB_Queue_t* q)
{
	if (q->size == 0)
	{
		trace_printf("TCB queue empty!\n");
		while(1);
	}

	return q->tcbPtrArr[q->head];
}

/*******************************************************************************
 * Data queuing:
 ******************************************************************************/
void RTOS_TCB_Queue_voidEnqueue(RTOS_TCB_Queue_t* q, RTOS_TCB_t* tcbPtr)
{
	if (q->size == RTOS_TCB_QUEUE_MAX_SIZE)
	{
		trace_printf("TCB queue full!\n");
		while(1);
	}

	q->tcbPtrArr[q->tail] = tcbPtr;

	q->tail = (q->tail + 1) % RTOS_TCB_QUEUE_MAX_SIZE;

	q->size++;
}

void RTOS_TCB_Queue_voidDequeue(RTOS_TCB_Queue_t* q)
{
	if (q->size == 0)
	{
		trace_printf("TCB queue empty!\n");
		while(1);
	}

	q->head = (q->head + 1) % RTOS_TCB_QUEUE_MAX_SIZE;

	q->size--;
}





