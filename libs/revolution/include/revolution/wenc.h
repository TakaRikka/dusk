#ifndef _REVOLUTION_WENC_H_
#define _REVOLUTION_WENC_H_

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    WENC_FLAG_USER_INFO = (1 << 0),
} WENCFlag;

typedef struct {
    u8 data[32];
} WENCInfo;  // size 0x20

s32 WENCGetEncodeData(WENCInfo* info, u32 flag, const s16* pcmData, s32 samples, u8* adpcmData);

#ifdef __cplusplus
}
#endif

#endif
