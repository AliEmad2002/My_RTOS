/*
 * Sceduler.h
 *
 *  Created on: Mar 20, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MY_RTOS_SCEDULER_H_
#define INCLUDE_MY_RTOS_SCEDULER_H_


/*******************************************************************************
 * System time getting:
 ******************************************************************************/
u64 RTOS_Sceduler_u64GetSystemTime(void);

/*******************************************************************************
 * Running TCB getting:
 ******************************************************************************/
RTOS_TCB_t* RTOS_Sceduler_ptrGetRunningTcb(void);

/*******************************************************************************
 * Init:
 ******************************************************************************/
/*
 * Takes data of the idle task. that is to be context switched to when all other
 * tasks are blocked. (its priority = lowest priority level)
 */
void RTOS_Sceduler_voidInit(
	void (*idleFunc)(void), u64* idleStackArr, u32 idleStackSizeInDWords);



#endif /* INCLUDE_MY_RTOS_SCEDULER_H_ */
