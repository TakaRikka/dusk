#ifndef M_DO_M_DO_PRINTF_H
#define M_DO_M_DO_PRINTF_H

#include <cstdarg>
#include "dolphin/types.h"

void my_PutString(const char*);
void mDoPrintf_vprintf_Interrupt(char const*, va_list);
void mDoPrintf_vprintf_Thread(char const*, va_list);
void mDoPrintf_vprintf(const char*, va_list);
extern "C" void mDoPrintf_VReport(const char*, va_list);

extern u32 print_counts;
extern u32 print_errors;
extern u32 print_warings;
extern u32 print_systems;
extern u8 print_initialized;

extern u8 print_mutex_initialized;
extern u8 print_highPriority;
extern u8 print_threadID;
extern u8 print_callerPC;
extern u8 print_callerPCLevel;

#endif /* M_DO_M_DO_PRINTF_H */
