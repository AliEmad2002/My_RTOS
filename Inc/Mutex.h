/*
 * Mutex.h
 *
 *  Created on: Mar 27, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MY_RTOS_MUTEX_H_
#define INCLUDE_MY_RTOS_MUTEX_H_


void RTOS_Mutex_voidTake(u8* mutexPtr);

void RTOS_Mutex_voidGive(u8* mutexPtr);


#endif /* INCLUDE_MY_RTOS_MUTEX_H_ */
