/*
 * Blocked_List.h
 *
 *  Created on: Mar 23, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MY_RTOS_BLOCKED_LIST_H_
#define INCLUDE_MY_RTOS_BLOCKED_LIST_H_

/*******************************************************************************
 * Adding TCB (pointer) to the blocked list:
 ******************************************************************************/
void RTOS_Blocked_List_voidAdd(RTOS_TCB_t* tcbPtr);

/*******************************************************************************
 * Checking blocking reason of all blocked threads.
 * Unblocks and returns first hit of a now-ready thread.
 ******************************************************************************/
b8 RTOS_Blocked_List_b8Unblock(RTOS_TCB_t** distP);


#endif /* INCLUDE_MY_RTOS_BLOCKED_LIST_H_ */
