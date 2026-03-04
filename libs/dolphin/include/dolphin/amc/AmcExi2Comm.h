#ifndef _DOLPHIN_AMC_AMCEXI2COMM_H_
#define _DOLPHIN_AMC_AMCEXI2COMM_H_

#include <dolphin/os.h>
#include <dolphin/amc/AmcTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	AMC_EXI_NO_ERROR = 0,
	AMC_EXI_UNSELECTED,
} AmcExiError;

// ---------------------------------------------------------------------------
//
// void EXI2_Init( volatile unsigned char **inputPendingPtrRef, 
//                 EXICallback monitorCallback );
//
// Description: Initialize the EXI2 driver (without interrupts).  The 
//              parameter 'inputPendingPtrref' is a flag showing whether input
//              is waiting in the EXI2 buffer and 'monitorCallback' is a 
//              pointer to a callback function that is invoked at the end of 
//              the EXI2 ISR.
//
// ---------------------------------------------------------------------------
void EXI2_Init(volatile unsigned char **inputPendingPtrRef, EXICallback monitorCallback);

// ---------------------------------------------------------------------------
//
// void EXI2_EnableInterrupts( void );
//
// Description: Enable EXI2 interrupts.  This function must be called to use 
//              interrupts on the EXI2 interface.  Call this function only 
//              after EXI2_Init() has been invoked.
// 
// ---------------------------------------------------------------------------
void		EXI2_EnableInterrupts( void );



// ---------------------------------------------------------------------------
//
// int EXI2_Poll( void );
//
// Description: Returns the number of bytes waiting to be read in the EXI2 
//              buffer.
//
// ---------------------------------------------------------------------------
int			EXI2_Poll( void );



// ---------------------------------------------------------------------------
//
// AmcExiError EXI2_ReadN( void *bytes, unsigned long length );
//
// Description: Read length bytes and return in bytes[] array.
//
//     Returns: One of AMC_EXI_*.
//
// ---------------------------------------------------------------------------
AmcExiError	EXI2_ReadN( void *bytes, unsigned long length);



// ---------------------------------------------------------------------------
//
// AmcExiError EXI2_WriteN( const void *bytes, unsigned long length );
//
// Description: Write length bytes stored in bytes[] array.
//
//     Returns: One of AMC_EXI_*.
//
// ---------------------------------------------------------------------------
AmcExiError	EXI2_WriteN( const void *bytes, unsigned long length);



// ---------------------------------------------------------------------------
//
// void EXI2_Reserve( void );
//
// Description: Disable non-monitor communications over the EXI2 port.
//              This function must be called before the monitor takes
//              control of the processor.
//
// ---------------------------------------------------------------------------
void		EXI2_Reserve( void );



// ---------------------------------------------------------------------------
//
// void EXI2_Unreserve( void );
//
// Description: Re-enable non-monitor communications over the EXI2 port.
//              This function must be called just before the monitor
//              gives control of the processor back to the application.
//
// ---------------------------------------------------------------------------
void		EXI2_Unreserve( void );



// ---------------------------------------------------------------------------
//
// AmcExiError EXI2_GetStatusReg( u16* pu16StatusReg );
//
// Description: Read and store the value of the status register into
//              *pu16StatusReg.
//
//     Returns: One of AMC_EXI_*.
// ---------------------------------------------------------------------------
AmcExiError	EXI2_GetStatusReg( u16* pu16StatusReg );

#ifdef __cplusplus
}
#endif

#endif
