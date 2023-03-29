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
	/*	disable all interrupts	*/
	cpsid i

	/*	stack from CPU to running thread's TCB	*/
	ldr r0, =ruuningTcbPtr		// r0 = &ruuningTcbPtr
	ldr r0, [r0]				// r0 = ruuningTcbPtr = &(ruuningTcbPtr->stackPtr)

	mrs r1, psp					// r1 = psp
	sub r1, r1, #40				// r1 = psp - 40 (40 stands from ten words)
	str r1, [r0]				// ruuningTcbPtr->stackPtr = psp - 40

	ldr r0, [r0]				// r0 = ruuningTcbPtr->stackPtr

	add r0, r0, #4				// r0 = (u32)(ruuningTcbPtr->stackPtr) + 4
	mrs r1, CONTROL				// r1 = cr
	str r1, [r0]				// *((u32)(ruuningTcbPtr->stackPtr) + 4) = cr

	add r0, r0, #4				// r0 = (u32)(ruuningTcbPtr->stackPtr) + 8
	stm r0, {r4, r5, r6, r7, r8, r9, r10, r11}

	/*
	 * update runningTcb (deals with ready queue and blocked list), and clear
	 * PendSV flag.
	 */
	bl RTOS_PendSV_Handler

	/*	un-stack from stack of updated "runningTcb" to the CPU TODO: use load multiple	*/
	ldr r0, =ruuningTcbPtr		// r0 = &ruuningTcbPtr
	ldr r0, [r0]				// r0 = ruuningTcbPtr = &(ruuningTcbPtr->stackPtr)
	ldr r0, [r0]				// r0 = ruuningTcbPtr->stackPtr

	add r1, r0, #40				// r1 = (u32)(ruuningTcbPtr->stackPtr) + 40
	msr psp, r1					// psp = (u32)(ruuningTcbPtr->stackPtr) + 40

	add r0, r0, #4				// r0 = (u32)(ruuningTcbPtr->stackPtr) + 4
	ldr r1, [r0]				// r1 = *((u32)(ruuningTcbPtr->stackPtr) + 4)
	msr CONTROL, r1				// cr = *((u32)(ruuningTcbPtr->stackPtr) + 4)
	isb							// Instruction Synchronization Barrier

	add r0, r0, #4				// r0 = (u32)(ruuningTcbPtr->stackPtr) + 8
	ldm r0, {r4, r5, r6, r7, r8, r9, r10, r11}

	/*	enable all interrupts	*/
	cpsie i

	mov lr, #0xFFFFFFFD			/*	EXC_RETURN: thread, use PSP		*/
	bx lr						/*	return from handler				*/
