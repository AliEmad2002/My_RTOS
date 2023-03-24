/*
 * PendSV_Handler.S
 *
 *  Created on: Mar 21, 2023
 *      Author: Ali Emad Ali
 */


.syntax unified
.cpu cortex-m3
.thumb

.global PendSV_Handler

.text

.type PendSV_Handler, %function
PendSV_Handler:
	bl RTOS_PendSV_Handler		/*	execute c-code of the handler	*/
	mov lr, #0xFFFFFFFD			/*	EXC_RETURN: thread, use PSP		*/
	bx lr						/*	return from handler				*/
