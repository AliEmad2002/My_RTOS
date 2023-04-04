/*
 * Scheduler.h
 *
 *  Created on: Mar 20, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MY_RTOS_SCHEDULER_H_
#define INCLUDE_MY_RTOS_SCHEDULER_H_


/*******************************************************************************
 * System time getting:
 ******************************************************************************/
u64 RTOS_Scheduler_u64GetSystemTime(void);

/*******************************************************************************
 * Running TCB getting:
 ******************************************************************************/
RTOS_TCB_t* RTOS_Scheduler_ptrGetRunningTcb(void);

/*******************************************************************************
 * SVC handler imm values and their functions:
 *
 * 0: Schedule (i.e.: pend PendSV interrupt).
 * 1: Take mutex or semaphore.
 ******************************************************************************/

/*******************************************************************************
 * Init:
 ******************************************************************************/
/*
 * Takes data of the idle task. that is to be context switched to when all other
 * tasks are blocked. (its priority = lowest priority level)
 */
void RTOS_Scheduler_voidInit(
	void (*idleFunc)(void), u64* idleStackArr, u32 idleStackSizeInDWords);



#endif /* INCLUDE_MY_RTOS_SCHEDULER_H_ */
