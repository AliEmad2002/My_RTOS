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
void RTOS_Sceduler_voidInit(void);



#endif /* INCLUDE_MY_RTOS_SCEDULER_H_ */
