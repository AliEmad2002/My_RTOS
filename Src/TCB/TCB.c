/*
 * TCB.c
 *
 *  Created on: Mar 20, 2023
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include <diag/trace.h>

/*	MCAL	*/

/*	RTOS	*/
#include "RTOS_Config.h"

/*	SELF	*/
#include "TCB.h"

/*******************************************************************************
 * Copy info:
 * (does not copy the stack)
 ******************************************************************************/
void RTOS_TCB_voidCpyInfo(RTOS_TCB_t* distPtr, const RTOS_TCB_t* srcPtr)
{
	distPtr->id					= srcPtr->id;
	distPtr->pri				= srcPtr->pri;
	distPtr->stackSizeInDWrods	= srcPtr->stackSizeInDWrods;
	distPtr->stackPtr			= srcPtr->stackPtr;
}
