#include <dolphin/types.h>
#include <d/d_kankyo.h>
#include <d/d_debug_pad.h>
u8 g_printOtherHeapDebug;

dKankyo_HIO_c g_kankyoHIO;

dDebugPad_c dDebugPad;

u32 __OSFpscrEnableBits;

GDLObj* __GDCurrentDL;

// DSP
DSPTaskInfo* __DSP_first_task;
DSPTaskInfo* __DSP_curr_task;
