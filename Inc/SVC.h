/*
 * SVC.h
 *
 *  Created on: Mar 26, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MY_RTOS_SVC_H_
#define INCLUDE_MY_RTOS_SVC_H_


typedef struct{
	u32 val;
	b8 isRead;
}RTOS_SVC_Buffer_t;

void RTOS_SVC_voidBufferInput(u32 input);

void RTOS_SVC_voidIgnoreLastOutput(void);

b8 RTOS_SVC_b8IsOutputBufferRead(void);

u32 RTOS_SVC_u32ReadOutputBuffer(void);

#endif /* INCLUDE_MY_RTOS_SVC_H_ */
