#ifndef _DOLPHIN_DEMOAVX_H_
#define _DOLPHIN_DEMOAVX_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void DEMOAVXAttach(void* buffer, u32 num_frames);
u32 DEMOAVXGetNumFilled(void);
u32 DEMOAVXGetFrameCounter(void);
u32 DEMOAVXRefreshBuffer(u32* start_index, u32* end_index);
void DEMOAVXInit(s16* left, s16* right, u32 size);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_DEMOAVX_H_
