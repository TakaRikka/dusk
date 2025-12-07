
#ifndef F_PC_DEBUG_SV_H_
#define F_PC_DEBUG_SV_H_

#include <dolphin/dolphin.h>

#if DEBUG

char* fpcDbSv_getNameString(s16 i_name);

typedef void (*fpcDbSv_callback)(void* i_process);
extern const fpcDbSv_callback g_fpcDbSv_service[50];

#endif

#endif
