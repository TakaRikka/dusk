#ifndef J3DGD_H
#define J3DGD_H

#include <dolphin/gx.h>
#include <dolphin/gd.h>

inline void J3DGDWrite_u8(u8 data) {
    __GDWrite(data);
}

inline void J3DGDWrite_u16(u16 data) {
    __GDWrite((u8)((data >> 8)));
    __GDWrite((u8)((data >> 0) & 0xFF));
}

inline void J3DGDWrite_u32(u32 data) {
    __GDWrite((u8)((data >> 24) & 0xFF));
    __GDWrite((u8)((data >> 16) & 0xFF));
    __GDWrite((u8)((data >> 8) & 0xFF));
    __GDWrite((u8)((data >> 0) & 0xFF));
}

inline void J3DGDWrite_f32(f32 data) {
    union {
        f32 f;
        u32 u;
    } fid;
    fid.f = data;
    J3DGDWrite_u32(fid.u);
}

inline void J3DGDWriteBPCmd(u32 regval) {
    J3DGDWrite_u8(GX_LOAD_BP_REG);
    J3DGDWrite_u32(regval);
}

inline void J3DGDWriteXFCmd(u16 addr, u32 val) {
    J3DGDWrite_u8(GX_LOAD_XF_REG);
    J3DGDWrite_u16(0);
    J3DGDWrite_u16(addr);
    J3DGDWrite_u32(val);
}

inline void J3DGDWriteXFCmdHdr(u16 addr, u8 len) {
    J3DGDWrite_u8(GX_LOAD_XF_REG);
    J3DGDWrite_u16(len - 1);
    J3DGDWrite_u16(addr);
}

inline void J3DGXCmd1f32ptr(f32* data) {
    GXCmd1u32(*(u32*)data);
}

inline void J3DGXCmd1f32(f32 data) {
    union {
        f32 f;
        u32 u;
    } fid;
    fid.f = data;
    GXCmd1u32(fid.u);
}

inline void J3DGDWriteCPCmd(u8 reg, u32 value) {
    J3DGDWrite_u8(8);
    J3DGDWrite_u8(reg);
    J3DGDWrite_u32(value);
}

void J3DGDSetGenMode(u8 texGenNum, u8 colorChanNum, u8 tevStageNum, u8 IndTexStageNum,
                     GXCullMode cullMode);
void J3DGDSetGenMode_3Param(u8 texGenNum, u8 tevStageNum, u8 indTexStageNum);
void J3DGDSetLightAttn(GXLightID, f32, f32, f32, f32, f32, f32);
void J3DGDSetLightColor(GXLightID, GXColor);
void J3DGDSetLightPos(GXLightID, f32, f32, f32);
void J3DGDSetLightDir(GXLightID, f32, f32, f32);
void J3DGDSetVtxAttrFmtv(GXVtxFmt, GXVtxAttrFmtList const*, bool);
void J3DGDSetTexCoordGen(GXTexGenType, GXTexGenSrc);
void J3DGDSetTexCoordScale2(GXTexCoordID, u16, u8, u8, u16, u8, u8);
void J3DGDSetTexLookupMode(GXTexMapID, GXTexWrapMode, GXTexWrapMode, GXTexFilter, GXTexFilter, f32,
                           f32, f32, u8, u8, GXAnisotropy);
void J3DGDSetTexImgAttr(GXTexMapID, u16, u16, GXTexFmt);
void J3DGDSetTexImgPtr(GXTexMapID, void*);
void J3DGDSetTexImgPtrRaw(GXTexMapID, u32);
void J3DGDSetTexTlut(GXTexMapID, u32, GXTlutFmt);
void J3DGDLoadTlut(void*, u32, GXTlutSize);
void J3DGDSetIndTexMtx(GXIndTexMtxID, f32 (*)[3], s8);
void J3DGDSetIndTexCoordScale(GXIndTexStageID, GXIndTexScale, GXIndTexScale, GXIndTexScale,
                              GXIndTexScale);
void J3DGDSetIndTexOrder(u32, GXTexCoordID, GXTexMapID, GXTexCoordID, GXTexMapID, GXTexCoordID,
                         GXTexMapID, GXTexCoordID, GXTexMapID);
void J3DGDSetTevOrder(GXTevStageID, GXTexCoordID, GXTexMapID, GXChannelID, GXTexCoordID, GXTexMapID,
                      GXChannelID);
void J3DGDSetTevKColor(GXTevKColorID, GXColor);
void J3DGDSetTevColorS10(GXTevRegID, GXColorS10);
void J3DGDSetFog(GXFogType, f32, f32, f32, f32, GXColor);
void J3DGDSetFogRangeAdj(GXBool, u16, _GXFogAdjTable*);

inline void J3DGDSetNumChans(u8 numChans) {
    J3DGDWriteXFCmd(0x1009, numChans);
}

inline void J3DGDSetNumTexGens(u8 numTexGens) {
    J3DGDWriteXFCmd(0x103f, numTexGens);
}

inline void J3DGDSetTevKonstantSel_SwapModeTable(GXTevStageID stage, GXTevKColorSel colorSel1, GXTevKAlphaSel alphaSel1, GXTevKColorSel colorSel2, GXTevKAlphaSel alphaSel2, GXTevColorChan chan1, GXTevColorChan chan2) {
    J3DGDWriteBPCmd((stage / 2 + 0xf6) << 24 | (chan1 | chan2 << 2 | colorSel1 << 4 | alphaSel1 << 9 | colorSel2 << 14 | alphaSel2 << 19) & 0x00FFFFFF);
}

inline void J3DGDSetAlphaCompare(GXCompare cmp0, u8 ref0, GXAlphaOp op, GXCompare cmp1, u8 ref1) {
    J3DGDWriteBPCmd(ref0 | ref1 << 8 | cmp0 << 16 | cmp1 << 19 | op << 22 | 0xF3 << 24);
}

inline void J3DGDSetBlendMode(GXBlendMode mode, GXBlendFactor srcFactor, GXBlendFactor dstFactor, GXLogicOp logicOp) {
    J3DGDWriteBPCmd(0xFE00FFE3);
    J3DGDWriteBPCmd(
        (mode == GX_BM_BLEND || mode == GX_BM_SUBTRACT) << 0 |
        (mode == GX_BM_LOGIC) << 1 |
        dstFactor << 5 |
        srcFactor << 8 |
        (mode == GX_BM_SUBTRACT) << 11 |
        logicOp << 12 |
        0x41 << 24);
}

inline void J3DGDSetBlendMode(GXBlendMode mode, GXBlendFactor srcFactor, GXBlendFactor dstFactor, GXLogicOp logicOp, u8 ditherEnable) {
    J3DGDWriteBPCmd(0xFE00FFE7);
    J3DGDWriteBPCmd(
        u32(mode == GX_BM_BLEND || mode == GX_BM_SUBTRACT) << 0 |
        (mode == GX_BM_LOGIC) << 1 |
        ditherEnable << 2 |
        dstFactor << 5 |
        srcFactor << 8 |
        (mode == GX_BM_SUBTRACT) << 11 |
        logicOp << 12 |
        0x41 << 24);
}

inline void J3DGDSetZMode(u8 compareEnable, GXCompare func, u8 writeEnable) {
    J3DGDWriteBPCmd(compareEnable | func << 1 | writeEnable << 4 | 0x40 << 24);
}

inline void J3DGDSetZCompLoc(u32 compLocEnable) {
    J3DGDWriteBPCmd(0xFE000040);
    J3DGDWriteBPCmd(compLocEnable << 6 | 0x43 << 24);
}


#endif /* J3DGD_H */
