#include <cstddef>
#include <revolution/gx.h>
#include <revolution/os.h>

#include "__gx.h"

static void __GXSetTexGen(u32 tgState);
static void __GXSetLightChan(u32 chState);
static void __GXSetAmbMat(u32 amState);

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

    dState &= 0xFFFFFF00;

    if (dState) {
        u32 dStateLocal;

        dStateLocal = dState & 0x00000F00;

        if (dStateLocal) {
            __GXSetAmbMat(dStateLocal);
        }
        
        dStateLocal = dState & (0x01000000|0x0000F000);

        if (dStateLocal) {
            __GXSetLightChan(dStateLocal);
        }

        dStateLocal = dState & (0x02000000|0x00FF0000);

        if (dStateLocal) {
            __GXSetTexGen(dStateLocal);
        }

        dStateLocal = dState & 0x04000000;
        if (dStateLocal) {
            __GXSetMatrixIndex(GX_VA_PNMTXIDX);
            __GXSetMatrixIndex(GX_VA_TEX4MTXIDX);
        }

        dStateLocal = dState & 0x10000000;
        if (dStateLocal) {
            __GXSetViewport();
        }

        dStateLocal = dState & 0x8000000;
        if (dStateLocal) {
            __GXSetProjection();
        } 

        __GXData->bpSentNot = GX_TRUE;
    }

    __GXData->dirtyState = 0;
}

static void __GXSetTexGen(u32 tgState) {
    u32 d, i, tgIndx = 0x1040;
    u32 dtgIndx = 0x1050;

    if (tgState & 0x2000000) {
        d = __GXData->genMode & 0xF;
        GX_WRITE_XF_REG(0x103F, d, 0);
    }

    d = (tgState & 0xFF0000) >> 16;
    i = 0;
    while (d) {
        if (d & 0x1) {
            GX_WRITE_XF_REG(tgIndx, __GXData->texGenCtrl[i], 0);
            GX_WRITE_XF_REG(dtgIndx, __GXData->texGenCtrl2[i], 0);
        }
    
        dtgIndx++;
        tgIndx++;
        i++;
        d >>= 1;    
    }
}

static void __GXSetLightChan(u32 chState) {
    u32 d, i, chIndx = 0x100E;

    if (chState & 0x1000000) {
        d = ((__GXData->genMode & 0x70) >> 4);
        GX_WRITE_XF_REG(0x1009, d, 0);
    }

    d = (chState & 0xF000) >> 12;
    i = 0;

    while (d) {
        if (d & 0x1) {
            GX_WRITE_XF_REG(chIndx, __GXData->chanCtrl[i], 0);
        }

        i++;
        chIndx++;
        d >>= 1;
    }
}

static void __GXSetAmbMat(u32 amState) {
    if (amState & 0x100) {
        GX_WRITE_XF_REG(0x100A, __GXData->ambColor[0], 0);
    }

    if (amState & 0x200) {
        GX_WRITE_XF_REG(0x100B, __GXData->ambColor[1], 0);
    }

    if (amState & 0x400) {
        GX_WRITE_XF_REG(0x100C, __GXData->matColor[0], 0);
    }

    if (amState & 0x800) {
        GX_WRITE_XF_REG(0x100D, __GXData->matColor[1], 0);
    }
}

void GXBegin(GXPrimitive type, GXVtxFmt vtxfmt, u16 nverts) {
    ASSERTMSGLINE(368, vtxfmt < GX_MAX_VTXFMT,   "GXBegin: Format Index is out of range");
    ASSERTMSGLINE(369, !__GXinBegin, "GXBegin: called inside another GXBegin/GXEnd");

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
    CHECK_GXBEGIN(447, "GXSetLineWidth");
    SC_SU_LPSIZE_SET_LSIZE(448, __GXData->lpSize, width);
    SC_SU_LPSIZE_SET_LTOFF(449, __GXData->lpSize, texOffsets);
    GX_WRITE_RAS_REG(__GXData->lpSize);
    __GXData->bpSentNot = 0;
}

void GXGetLineWidth(u8* width, GXTexOffset* texOffsets) {
    ASSERTMSGLINE(463, width != NULL && texOffsets != NULL, "GXGet*: invalid null pointer");

    *width      = SU_LPSIZE_GET_LSIZE(__GXData->lpSize);
    *texOffsets = SU_LPSIZE_GET_LTOFF(__GXData->lpSize);
}

void GXSetPointSize(u8 pointSize, GXTexOffset texOffsets) {
    CHECK_GXBEGIN(491, "GXSetPointSize");
    SC_SU_LPSIZE_SET_PSIZE(492, __GXData->lpSize, pointSize);
    SC_SU_LPSIZE_SET_PTOFF(493, __GXData->lpSize, texOffsets);
    GX_WRITE_RAS_REG(__GXData->lpSize);
    __GXData->bpSentNot = 0;
}

void GXGetPointSize(u8* pointSize, GXTexOffset* texOffsets) {
    ASSERTMSGLINE(507, pointSize != NULL && texOffsets != NULL, "GXGet*: invalid null pointer");

    *pointSize  = SU_LPSIZE_GET_PSIZE(__GXData->lpSize);
    *texOffsets = SU_LPSIZE_GET_PTOFF(__GXData->lpSize);
}

void GXEnableTexOffsets(GXTexCoordID coord, u8 line_enable, u8 point_enable) {
    CHECK_GXBEGIN(536, "GXEnableTexOffsets");

    ASSERTMSGLINE(538, coord < GX_MAX_TEXCOORD, "GXEnableTexOffsets: Invalid coordinate Id");

    SC_SU_TS0_SET_LF(540, __GXData->suTs0[coord], line_enable);
    SC_SU_TS0_SET_PF(541, __GXData->suTs0[coord], point_enable);
    GX_WRITE_RAS_REG(__GXData->suTs0[coord]);
    __GXData->bpSentNot = 0;
}

void GXSetCullMode(GXCullMode mode) {
    GXCullMode hwMode;

    CHECK_GXBEGIN(564, "GXSetCullMode");
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

    SC_GEN_MODE_SET_REJECT_EN(577, __GXData->genMode, hwMode);
    __GXData->dirtyState |= 4;
}

void GXGetCullMode(GXCullMode* mode) {
#if DEBUG
    GXCullMode hwMode = GEN_MODE_GET_REJECT_EN(__GXData->genMode);

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

    CHECK_GXBEGIN(620, "GXSetCoPlanar");

    SC_GEN_MODE_SET_ZFREEZE(622, __GXData->genMode, enable);
    reg = 0xFE080000;
    GX_WRITE_RAS_REG(reg);
    GX_WRITE_RAS_REG(__GXData->genMode);
    __GXData->bpSentNot = 0;
}

void __GXSetGenMode(void) {
    GX_WRITE_RAS_REG(__GXData->genMode);
    __GXData->bpSentNot = 0;
}
