#ifndef _DOLPHIN_AM_INTERNAL_H_
#define _DOLPHIN_AM_INTERNAL_H_

#include <dolphin/am.h>
#include <dolphin/dvd.h>
#include <dolphin/ar.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AM_STACK_ENTRIES 16

typedef struct {
    DVDFileInfo file_handle;
    ARQRequest arq_handle;
    AMCallback callback;
    char* path;
    void* buffer;
    volatile u32 file_length;
    volatile u32 curr_read_offset;
    volatile u32 read_length;
    volatile u32 aram_start_addr;
    volatile u32 curr_aram_offset;
    volatile int poll_flag;
} AMReadInfo;

extern AMReadInfo __AMReadInfo[AM_STACK_ENTRIES];

static void __AM_dvd_callback(s32 result, DVDFileInfo* handle);
static void __AM_arq_callback(u32 task);
static void __AM_arq_poll_callback(u32 task);
u32 __AMPushBuffered(char* path, void* buffer, u32 buffer_size, AMCallback callback, int async_flag);

#ifdef __cplusplus
}
#endif

#endif
