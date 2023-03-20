/*
 * TCB_Queue.h
 *
 *  Created on: Mar 20, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MY_RTOS_TCB_TCB_QUEUE_H_
#define INCLUDE_MY_RTOS_TCB_TCB_QUEUE_H_

#define RTOS_TCB_QUEUE_MAX_SIZE		RTOS_MAX_NUMBER_OF_TASKS_PER_PRIORITY_LEVEL

typedef struct{
	RTOS_TCB_t tcbArr[RTOS_TCB_QUEUE_MAX_SIZE];

	u16 head;
	u16 tail;
	u16 size;
}RTOS_TCB_Queue_t;

/*******************************************************************************
 * Init:
 ******************************************************************************/
void RTOS_TCB_Queue_voidInit(RTOS_TCB_Queue_t* q);

/*******************************************************************************
 * Data accessing:
 ******************************************************************************/
/*
 * copies first of the queue to the location "tcbPtr".
 */
void RTOS_TCB_Queue_voidFirst(RTOS_TCB_Queue_t* q, RTOS_TCB_t* tcbPtr);

/*******************************************************************************
 * Data queuing:
 ******************************************************************************/
void RTOS_TCB_Queue_voidEnqueue(RTOS_TCB_Queue_t* q, RTOS_TCB_t* tcbPtr);

void RTOS_TCB_Queue_voidDequeue(RTOS_TCB_Queue_t* q);





#endif /* INCLUDE_MY_RTOS_TCB_TCB_QUEUE_H_ */
