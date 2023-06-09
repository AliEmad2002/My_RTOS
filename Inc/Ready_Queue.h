/*
 * Ready_Queue.h
 *
 *  Created on: Mar 20, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MY_RTOS_READY_QUEUE_H_
#define INCLUDE_MY_RTOS_READY_QUEUE_H_


/*******************************************************************************
 * Enqueuing TCB to the ready queue:
 ******************************************************************************/
void RTOS_Ready_Queue_voidEnqueue(RTOS_TCB_t* tcbPtr);

/*******************************************************************************
 * Dequeuing TCB from the ready queue to TCB object:
 ******************************************************************************/
void RTOS_Ready_Queue_voidDequeue(RTOS_TCB_t** distP, u8 pri);

/*******************************************************************************
 * Getting priority of the most urgent available TCB:
 ******************************************************************************/
/*	returns NULL_PRI if there's not any tasks	*/
u8 RTOS_Ready_Queue_u8GetPriOfMostUrgent();

#endif /* INCLUDE_MY_RTOS_READY_QUEUE_H_ */
