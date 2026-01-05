#include <cstddef>
#include <dolphin/gx.h>
#include <dolphin/os.h>

#include "__gx.h"

void __GXSetDirtyState(void) {
    u32 dState = __GXData->dirtyState;

    if (dState & 1) {
        __GXSetSUTexRegs();
    }
    if (dState & 2) {
        __GXUpdateBPMask();
    }
    if (dState & 4) {
        __GXSetGenMode();
    }
    if (dState & 8) {
        __GXSetVCD();
    }
    if (dState & 0x10) {
        __GXSetVAT();
    }
    if (dState & 0x18) {
        __GXCalculateVLim();
    }

    __GXData->dirtyState = 0;
}

void GXBegin(GXPrimitive type, GXVtxFmt vtxfmt, u16 nverts) {
    ASSERTMSGLINE(359, vtxfmt < GX_MAX_VTXFMT,   "GXBegin: Format Index is out of range");
    ASSERTMSGLINE(360, !__GXinBegin, "GXBegin: called inside another GXBegin/GXEnd");

    if (__GXData->dirtyState != 0) {
        __GXSetDirtyState();
    }

#if DEBUG
    if (!__GXData->inDispList) {
        __GXVerifyState(vtxfmt);
    }
    __GXinBegin = 1;
#endif

    if (*(u32*)&__GXData->vNumNot == 0) {  // checks both vNum and bpSentNot
        __GXSendFlushPrim();
    }
    GX_WRITE_U8(vtxfmt | type);
    GX_WRITE_U16(nverts);
}

void __GXSendFlushPrim(void) {
    u32 i;
    u32 numD = __GXData->vNum * __GXData->vLim;

    GX_WRITE_U8(0x98);
    GX_WRITE_U16(__GXData->vNum);
    for (i = 0; i < numD; i += 4) {
        GX_WRITE_U32(0);
    }
    __GXData->bpSentNot = 1;
}

void GXSetLineWidth(u8 width, GXTexOffset texOffsets) {
    CHECK_GXBEGIN(440, "GXSetLineWidth");
    SET_REG_FIELD(441, __GXData->lpSize, 8, 0, width);
    SET_REG_FIELD(442, __GXData->lpSize, 3, 16, texOffsets);
    GX_WRITE_RAS_REG(__GXData->lpSize);
    __GXData->bpSentNot = 0;
}

void GXGetLineWidth(u8* width, GXTexOffset* texOffsets) {
    ASSERTMSGLINE(463, width != NULL && texOffsets != NULL, "GXGet*: invalid null pointer");

    *width      = GET_REG_FIELD(__GXData->lpSize, 8, 0);
    *texOffsets = GET_REG_FIELD(__GXData->lpSize, 3, 16);
}

void GXSetPointSize(u8 pointSize, GXTexOffset texOffsets) {
    CHECK_GXBEGIN(484, "GXSetPointSize");
    SET_REG_FIELD(485, __GXData->lpSize, 8, 8, pointSize);
    SET_REG_FIELD(486, __GXData->lpSize, 3, 19, texOffsets);
    GX_WRITE_RAS_REG(__GXData->lpSize);
    __GXData->bpSentNot = 0;
}

void GXGetPointSize(u8* pointSize, GXTexOffset* texOffsets) {
    ASSERTMSGLINE(507, pointSize != NULL && texOffsets != NULL, "GXGet*: invalid null pointer");

    *pointSize  = (int)GET_REG_FIELD(__GXData->lpSize, 8, 8);
    *texOffsets = GET_REG_FIELD(__GXData->lpSize, 3, 19);
}

void GXEnableTexOffsets(GXTexCoordID coord, u8 line_enable, u8 point_enable) {
    CHECK_GXBEGIN(529, "GXEnableTexOffsets");

    ASSERTMSGLINE(531, coord < GX_MAX_TEXCOORD, "GXEnableTexOffsets: Invalid coordinate Id");

    SET_REG_FIELD(533, __GXData->suTs0[coord], 1, 18, line_enable);
    SET_REG_FIELD(534, __GXData->suTs0[coord], 1, 19, point_enable);
    GX_WRITE_RAS_REG(__GXData->suTs0[coord]);
    __GXData->bpSentNot = 0;
}

void GXSetCullMode(GXCullMode mode) {
    GXCullMode hwMode;

    CHECK_GXBEGIN(557, "GXSetCullMode");
#if DEBUG
    switch (mode) {
    case GX_CULL_FRONT: hwMode = GX_CULL_BACK;  break;
    case GX_CULL_BACK:  hwMode = GX_CULL_FRONT; break;
    default:            hwMode = mode;          break;
    }
#else
    hwMode = (mode >> 1) & 1;
    __rlwimi(hwMode, mode, 1, 30, 30);
#endif

    SET_REG_FIELD(570, __GXData->genMode, 2, 14, hwMode);
    __GXData->dirtyState |= 4;
}

void GXGetCullMode(GXCullMode* mode) {
#if DEBUG
    GXCullMode hwMode = GET_REG_FIELD(__GXData->genMode, 2, 14);

    switch (hwMode) {
    case GX_CULL_FRONT: *mode = GX_CULL_BACK;  break;
    case GX_CULL_BACK:  *mode = GX_CULL_FRONT; break;
    default:            *mode = hwMode;        break;
    }
#else
    // fake match?
    GXCullMode hwMode = __GXData->genMode;
    *mode = ((hwMode >> 0xD) & 0x2) | (((((int)hwMode >> 0xE) & 0x2) >> 0x1));
#endif
}

void GXSetCoPlanar(GXBool enable) {
    u32 reg;

    CHECK_GXBEGIN(613, "GXSetCoPlanar");

    SET_REG_FIELD(615, __GXData->genMode, 1, 19, enable);
    reg = 0xFE080000;
    GX_WRITE_RAS_REG(reg);
    GX_WRITE_RAS_REG(__GXData->genMode);
}

void __GXSetGenMode(void) {
    GX_WRITE_RAS_REG(__GXData->genMode);
    __GXData->bpSentNot = 0;
}
