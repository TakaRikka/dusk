#ifndef _DOLPHIN_GD_BASE_H
#define _DOLPHIN_GD_BASE_H

#include <dolphin/types.h>
#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXCommandList.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*GDOverflowCb)(void);

typedef struct _GDLObj {
	u8* start;
	u32 length;
	u8* ptr;
	u8* top;
} GDLObj;

extern GDLObj* __GDCurrentDL;

void GDInitGDLObj(GDLObj* dl, void* start, u32 length);
void GDFlushCurrToMem(void);
void GDPadCurr32(void);
void GDOverflowed(void);
void GDSetOverflowCallback(GDOverflowCb callback);
GDOverflowCb GDGetOverflowCallback(void);

inline static void GDOverflowCheck(u32 size) {
    if (__GDCurrentDL->ptr + size > __GDCurrentDL->top) {
        GDOverflowed();
    }
}

inline static void __GDWrite(u8 data) {
    *__GDCurrentDL->ptr++ = data;
}

inline static void GDWrite_u8(u8 data) {
    GDOverflowCheck(sizeof(u8));
    __GDWrite(data);
}

inline static void GDWrite_u16(u16 data) {
    GDOverflowCheck(sizeof(u16));
    __GDWrite((u8)((data >> 8)));
    __GDWrite((u8)((data >> 0) & 0xFF));
}

inline static void GDWrite_u24(u32 data) {
    GDOverflowCheck(3);
    __GDWrite((u8)((data >> 16) & 0xFF));
    __GDWrite((u8)((data >> 8) & 0xFF));
    __GDWrite((u8)((data >> 0) & 0xFF));
}

inline static void GDWrite_u32(u32 data) {
    GDOverflowCheck(sizeof(u32));
    __GDWrite((u8)((data >> 24) & 0xFF));
    __GDWrite((u8)((data >> 16) & 0xFF));
    __GDWrite((u8)((data >> 8) & 0xFF));
    __GDWrite((u8)((data >> 0) & 0xFF));
}

inline static void GDWrite_f32(f32 data) {
    union {
        f32 f;
        u32 u;
    } fid;
    fid.f = data;
    GDWrite_u32(fid.u);
}

inline static void GDWriteXFCmdHdr(u16 addr, u8 len) {
    GDWrite_u8(GX_LOAD_XF_REG);
    GDWrite_u16(len - 1);
    GDWrite_u16(addr);
}

inline static void GDWriteXFCmd(u16 addr, u32 val) {
    GDWrite_u8(GX_LOAD_XF_REG);
    GDWrite_u16(0);
    GDWrite_u16(addr);
    GDWrite_u32(val);
}

inline static void GDWriteXFIndxDCmd(u16 addr, u8 len, u16 index) {
    GDWrite_u8(GX_LOAD_INDX_D);
    GDWrite_u16(index);
    GDWrite_u16((len - 1) << 12 | addr);
}

inline static void GDWriteXFIndxACmd(u16 addr, u8 len, u16 index) {
    GDWrite_u8(GX_LOAD_INDX_A);
    GDWrite_u16(index);
    GDWrite_u16(((len - 1) << 12) | addr);
}

inline static void GDWriteXFIndxBCmd(u16 addr, u8 len, u16 index) {
    GDWrite_u8(GX_LOAD_INDX_B);
    GDWrite_u16(index);
    GDWrite_u16(((len - 1) << 12) | addr);
}

inline static void GDWriteXFIndxCCmd(u16 addr, u8 len, u16 index) {
    GDWrite_u8(GX_LOAD_INDX_C);
    GDWrite_u16(index);
    GDWrite_u16(((len - 1) << 12) | addr);
}

inline static void GDWriteCPCmd(u8 addr, u32 val) {
    GDWrite_u8(GX_LOAD_CP_REG);
    GDWrite_u8(addr);
    GDWrite_u32(val);
}

inline static void GDWriteBPCmd(u32 regval) {
    GDWrite_u8(GX_LOAD_BP_REG);
    GDWrite_u32(regval);
}

inline static void GDSetCurrent(GDLObj* dl) {
    __GDCurrentDL = dl;
}

static inline u32 GDGetCurrOffset(void) {
    return (u32)(__GDCurrentDL->ptr - __GDCurrentDL->start);
}

static inline void GDSetCurrOffset(u32 offset) {
    __GDCurrentDL->ptr = __GDCurrentDL->start + offset;
}

static inline void* GDGetCurrPointer(void) {
    return (void*)__GDCurrentDL->ptr;
}

static inline u8* GDGetCurrPointer2(void) {
    return __GDCurrentDL->ptr;
}

static inline u32 GDGetGDLObjOffset(const GDLObj* dl) {
    return (u32)(dl->ptr - dl->start);
}

#ifdef __cplusplus
}
#endif

#endif
