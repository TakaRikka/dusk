#ifndef F_PC_PAUSE_
#define F_PC_PAUSE_

#include <dolphin/types.h>

int fpcPause_IsEnable(void* pProc, u8 expected);
int fpcPause_Enable(void* pProc, u8 pauseMask);
int fpcPause_Disable(void* pProc, u8 pauseMask);
void fpcPause_Init(void* pProc);

#endif
