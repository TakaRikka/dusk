#include <dolphin/types.h>
#include <d/d_kankyo.h>
#include <d/d_debug_pad.h>
u8 g_printOtherHeapDebug;

dKankyo_HIO_c g_kankyoHIO;

dDebugPad_c dDebugPad;

u32 __OSFpscrEnableBits;

GDLObj* __GDCurrentDL;

// DSP
#include <dolphin/dsp.h>
DSPTaskInfo* __DSP_first_task;
DSPTaskInfo* __DSP_curr_task;

// mDo_dvd
#include <m_Do/m_Do_dvd_thread.h>
u8 mDoDvdThd::DVDLogoMode;
