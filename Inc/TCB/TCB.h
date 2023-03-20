/*
 * TCB.h
 *
 *  Created on: Mar 19, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MY_RTOS_TCB_H_
#define INCLUDE_MY_RTOS_TCB_H_


typedef struct{
	/*
	 * Each thread when created is given a unique ID.
	 */
	u8 id;

	/*
	 * Priority of the thread. (0 is the highest).
	 */
	u8 pri;

	/*
	 * Stack size (in bytes).
	 */
	u32 stackSize;

	/*
	 * Pointer to last word in stack.
	 */
	u32* baseStackPtr;
}RTOS_TCB_t;

/*******************************************************************************
 * Copy info:
 * (does not copy the stack)
 ******************************************************************************/
void RTOS_TCB_voidCpyInfo(RTOS_TCB_t* distPtr, const RTOS_TCB_t* srcPtr);

#endif /* INCLUDE_MY_RTOS_TCB_H_ */
