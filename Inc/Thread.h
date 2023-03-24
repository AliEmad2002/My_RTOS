/*
 * Thread.h
 *
 *  Created on: Mar 20, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MY_RTOS_THREAD_H_
#define INCLUDE_MY_RTOS_THREAD_H_


/******************************************************************************
 * Creation:
 ******************************************************************************/
/*
 * User passes pointer of a statically allocated, statically defined array, and
 * its length in bytes, to be used as a stack.
 *
 * A previously statically allocated TCB object is initialized with this info,
 * and added to the ready queue.
 */
void RTOS_Thread_voidCreate(
	void (*func)(void), u8 priority, u64* stackArr, u32 stackSizeInDWords);


#endif /* INCLUDE_MY_RTOS_THREAD_H_ */
