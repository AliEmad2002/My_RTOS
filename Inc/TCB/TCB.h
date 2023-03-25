/*
 * TCB.h
 *
 *  Created on: Mar 19, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MY_RTOS_TCB_H_
#define INCLUDE_MY_RTOS_TCB_H_

typedef enum{
	RTOS_TCB_BlockingReason_Time,
	RTOS_TCB_BlockingReason_Mutex
}RTOS_TCB_BlockingReason_t;

typedef struct{
	/*
	 * Pointer to last word in stack.
	 */
	u64* stackPtr;

	/*
	 * Each thread when created is given a unique ID.
	 */
	u8 id;

	/*
	 * Priority of the thread. (0 is the highest).
	 */
	u8 pri;

	/*
	 * Blocking state.
	 */
	b8 isBlocked;

	/*
	 * Blocking reason (if thread is blocked).
	 */
	RTOS_TCB_BlockingReason_t blockingReason;

	/*
	 * Target ready-time (if thread is time blocked).
	 */
	u64 targetReadyTime;

	/*
	 * Pointer to the blocking mutex or semaphore (if any).
	 */
	u8* mutexPtr;

	/*
	 * Stack size (in double words).
	 */
	u32 stackSizeInDWrods;
}RTOS_TCB_t;

/*******************************************************************************
 * Copy info:
 * (does not copy the stack)
 ******************************************************************************/
void RTOS_TCB_voidCpyInfo(RTOS_TCB_t* distPtr, const RTOS_TCB_t* srcPtr);

#endif /* INCLUDE_MY_RTOS_TCB_H_ */
