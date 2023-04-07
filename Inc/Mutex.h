/*
 * Mutex.h
 *
 *  Created on: Mar 27, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MY_RTOS_MUTEX_H_
#define INCLUDE_MY_RTOS_MUTEX_H_


/*
 * "timeOut" is number of RTOS ticks to wait if the mutex / semaphore was not
 * available.
 *
 * For infinite wait: "timeOut" = 0
 *
 * returns 1 if mutex / semaphore was successfully taken, 0 otherwwise.
 */
u8 RTOS_Mutex_u8Take(u8* mutexPtr, u32 timeOut);

void RTOS_Mutex_voidGive(u8* mutexPtr);


#endif /* INCLUDE_MY_RTOS_MUTEX_H_ */
