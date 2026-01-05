#ifndef METROTRK_PORTABLE_MEM_TRK_H
#define METROTRK_PORTABLE_MEM_TRK_H

#include "dolphin/types.h"

void* TRK_memset(void* dest, int val, size_t count);
void* TRK_memcpy(void* dest, const void* src, size_t count);
void TRK_fill_mem(void* dest, int val, size_t count);

#endif /* METROTRK_PORTABLE_MEM_TRK_H */
