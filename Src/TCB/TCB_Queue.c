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
 * Head and tail manipulation:
 ******************************************************************************/
ALWAYS_INLINE_STATIC void RTOS_TCB_Queue_voidIncHead(RTOS_TCB_Queue_t* q)
{
	if (q->head == RTOS_TCB_QUEUE_MAX_SIZE - 1)
		q->head = 0;

	else
		q->head++;
}

ALWAYS_INLINE_STATIC void RTOS_TCB_Queue_voidIncTail(RTOS_TCB_Queue_t* q)
{
	if (q->tail == RTOS_TCB_QUEUE_MAX_SIZE - 1)
		q->tail = 0;

	else
		q->tail++;
}

/*******************************************************************************
 * Data accessing:
 ******************************************************************************/
void RTOS_TCB_Queue_voidFirst(RTOS_TCB_Queue_t* q, RTOS_TCB_t* tcbPtr)
{
	if (q->size == 0)
	{
		trace_printf("TCB queue empty!\n");
		while(1);
	}

	RTOS_TCB_t* src = &(q->tcbArr[q->head]);

	RTOS_TCB_voidCpyInfo(tcbPtr, src);
}

/*******************************************************************************
 * Data queuing:
 ******************************************************************************/
void RTOS_TCB_Queue_voidEnqueue(RTOS_TCB_Queue_t* q, RTOS_TCB_t* tcb)
{
	if (q->size == RTOS_TCB_QUEUE_MAX_SIZE)
	{
		trace_printf("TCB queue full!\n");
		while(1);
	}

	RTOS_TCB_t* dist = &(q->tcbArr[q->tail]);

	RTOS_TCB_voidCpyInfo(dist, tcb);

	RTOS_TCB_Queue_voidIncTail(q);

	q->size++;
}

void RTOS_TCB_Queue_voidDequeue(RTOS_TCB_Queue_t* q)
{
	if (q->size == 0)
	{
		trace_printf("TCB queue empty!\n");
		while(1);
	}

	RTOS_TCB_Queue_voidIncHead(q);

	q->size--;
}





