
#ifndef F_PC_LOAD_H_
#define F_PC_LOAD_H_

#include <dolphin/types.h>

BOOL fpcLd_Use(s16 i_procName);
BOOL fpcLd_IsLoaded(s16 i_procName);
void fpcLd_Free(s16 i_procName);
int fpcLd_Load(s16 i_procName);

#endif
