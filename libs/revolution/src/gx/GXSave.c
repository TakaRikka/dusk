#if DEBUG

#include <revolution/gx.h>
#include <revolution/os.h>

#include "__gx.h"

static const u8* dlist;
static u32 dlistSize;
static u32 bytesRead;

// prototypes
void __GXShadowIndexState(u32 idx_reg, u32 reg_data);

static u8 __ReadMem(void* ptr, u32 sz) {
    const u8* src;
    u8* dst;
    u32 i;

    if (sz > dlistSize - bytesRead) {
        return FALSE;
    }

    src = dlist;
    dst = ptr;
    for (i = 0; i < sz; i++) {
        *dst++ = *src++;
    }
    bytesRead += sz;
    dlist += sz;
    return TRUE;
}

inline void DPF(char*, ...) {
    u8 unused[4];
}

static void __SaveCPRegs(u8 reg, u8 vatIdx, u32 data) {
    s32 idx;

    DPF("\tCP Stream Reg[0x%x] = 0x%x\n", reg, data);

    switch (reg) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        break;
    case 5:
        __GXData->vcdLo = data;
        break;
    case 6:
        __GXData->vcdHi = data;
        break;
    case 7:
        __GXData->vatA[vatIdx] = data;
        break;
    case 8:
        __GXData->vatB[vatIdx] = data;
        break;
    case 9:
        __GXData->vatC[vatIdx] = data;
        break;
    case 10:
        idx = vatIdx - 0x15;
        if ((idx >= 0) && (idx < 4)) {
            __GXData->indexBase[idx] = data;
        }
        break;
    case 11:
        idx = vatIdx - 0x15;
        if ((idx >= 0) && (idx < 4)) {
            __GXData->indexStride[idx] = data;
        }
        break;
    default:
        if (__gxVerif->verifyLevel >= __gxvWarnLev[GXWARN_DL_INV_CMD]) {
            __GX_WARN(GXWARN_DL_INV_CMD);
        }
        OSReport("[Invalid CP Stream Register Address 0x%x\n]", reg);
        break;
    }
}

static void __ReconstVtxStatus(u8 vatIdx) {
    u32 vat;

    if (GET_REG_FIELD(__GXData->vcdLo, 2, 11) & 3) {
        vat = __GXData->vatA[vatIdx];
        if ((vat >> 9) & 1) {
            __GXData->hasNrms = 0;
            __GXData->hasBiNrms = 1;
        } else {
            __GXData->hasNrms = 1;
            __GXData->hasBiNrms = 0;
        }
    }
}

static u32 vtxCompSize[5] = { 1, 1, 2, 2, 4 };
static int clrCompSize[6] = { 2, 3, 4, 2, 3, 4 };

static u32 GetAttrSize(u8 vatIdx, u32 attrIdx) {
    u32 vcd;
    u32 vat;
    u32 nc;

    switch (attrIdx) {
    case 0:
        return GET_REG_FIELD(__GXData->vcdLo, 1, 0) ? 1 : 0;
    case 1:
        return GET_REG_FIELD(__GXData->vcdLo, 1, 1) ? 1 : 0;
    case 2:
        return GET_REG_FIELD(__GXData->vcdLo, 1, 2) ? 1 : 0;
    case 3:
        return GET_REG_FIELD(__GXData->vcdLo, 1, 3) ? 1 : 0;
    case 4:
        return GET_REG_FIELD(__GXData->vcdLo, 1, 4) ? 1 : 0;
    case 5:
        return GET_REG_FIELD(__GXData->vcdLo, 1, 5) ? 1 : 0;
    case 6:
        return GET_REG_FIELD(__GXData->vcdLo, 1, 6) ? 1 : 0;
    case 7:
        return GET_REG_FIELD(__GXData->vcdLo, 1, 7) ? 1 : 0;
    case 8:
        return GET_REG_FIELD(__GXData->vcdLo, 1, 8) ? 1 : 0;
    case 9:
        vcd = __GXData->vcdLo;
        vat = __GXData->vatA[vatIdx & 0xFF];
        switch (GET_REG_FIELD(vcd, 2, 9)) {
        case 0:
            return 0;
        case 2:
            return 1;
        case 3:
            return 2;
        case 1:
            return ((vat & 1) + 2) * vtxCompSize[(vat >> 1) & 7];
        }
        break;
    case 10:
        vcd = __GXData->vcdLo;
        vat = __GXData->vatA[vatIdx & 0xFF];

        switch (GET_REG_FIELD(vcd, 2, 11)) {
        case 0:
            return 0;
        case 2:
            if ((vat >> 9) & 1 && vat >> 31) {
                nc = 3;
            } else {
                nc = 1;
            }
            return nc;
        case 3:
            if ((vat >> 9) & 1 && vat >> 31) {
                nc = 6;
            } else {
                nc = 2;
            }
            return nc;
        case 1:
            if ((vat >> 9) & 1) {
                nc = 9;
            } else {
                nc = 3;
            }
            return nc * vtxCompSize[(vat >> 10) & 7];
        }
        break;
    case 11:
        switch (GET_REG_FIELD(__GXData->vcdLo, 2, 13)) {
        case 0:
            return 0;
        case 2:
            return 1;
        case 3:
            return 2;
        case 1:
            vat = __GXData->vatA[vatIdx];
            return clrCompSize[(vat >> 14) & 7];
        }
        break;
    case 12:
        switch (GET_REG_FIELD(__GXData->vcdLo, 2, 15)) {
        case 0:
            return 0;
        case 2:
            return 1;
        case 3:
            return 2;
        case 1:
            vat = __GXData->vatA[vatIdx];
            return clrCompSize[(vat >> 18) & 7];
        }
        break;
    case 13:
        vcd = __GXData->vcdHi;
        vat = __GXData->vatA[vatIdx & 0xFF];
        switch (GET_REG_FIELD(vcd, 2, 0)) {
        case 0:
            return 0;
        case 2:
            return 1;
        case 3:
            return 2;
        case 1:
            return (((vat >> 21) & 1) + 1) * vtxCompSize[(vat >> 22) & 7];
        }
        break;
    case 14:
        vcd = __GXData->vcdHi;
        vat = __GXData->vatB[vatIdx & 0xFF];
        switch (GET_REG_FIELD(vcd, 2, 2)) {
        case 0:
            return 0;
        case 2:
            return 1;
        case 3:
            return 2;
        case 1:
            return (((vat >> 0) & 1) + 1) * vtxCompSize[(vat >> 1) & 7];
        }
        break;
    case 15:
        vcd = __GXData->vcdHi;
        vat = __GXData->vatB[vatIdx & 0xFF];
        switch (GET_REG_FIELD(vcd, 2, 4)) {
        case 0:
            return 0;
        case 2:
            return 1;
        case 3:
            return 2;
        case 1:
            return (((vat >> 9) & 1) + 1) * vtxCompSize[(vat >> 10) & 7];
        }
        break;
    case 16:
        vcd = __GXData->vcdHi;
        vat = __GXData->vatB[vatIdx & 0xFF];
        switch (GET_REG_FIELD(vcd, 2, 6)) {
        case 0:
            return 0;
        case 2:
            return 1;
        case 3:
            return 2;
        case 1:
            return (((vat >> 18) & 1) + 1) * vtxCompSize[(vat >> 19) & 7];
        }
        break;
    case 17:
        vcd = __GXData->vcdHi;
        vat = __GXData->vatB[vatIdx & 0xFF];
        switch (GET_REG_FIELD(vcd, 2, 8)) {
        case 0:
            return 0;
        case 2:
            return 1;
        case 3:
            return 2;
        case 1:
            return (((vat >> 27) & 1) + 1) * vtxCompSize[(vat >> 28) & 7];
        }
        break;
    case 18:
        vcd = __GXData->vcdHi;
        vat = __GXData->vatC[vatIdx & 0xFF];
        switch (GET_REG_FIELD(vcd, 2, 10)) {
        case 0:
            return 0;
        case 2:
            return 1;
        case 3:
            return 2;
        case 1:
            return (((vat >> 5) & 1) + 1) * vtxCompSize[(vat >> 6) & 7];
        }
        break;
    case 19:
        vcd = __GXData->vcdHi;
        vat = __GXData->vatC[vatIdx & 0xFF];
        switch (GET_REG_FIELD(vcd, 2, 12)) {
        case 0:
            return 0;
        case 2:
            return 1;
        case 3:
            return 2;
        case 1:
            return (((vat >> 14) & 1) + 1) * vtxCompSize[(vat >> 15) & 7];
        }
        break;
    case 20:
        vcd = __GXData->vcdHi;
        vat = __GXData->vatC[vatIdx & 0xFF];
        switch (GET_REG_FIELD(vcd, 2, 14)) {
        case 0:
            return 0;
        case 2:
            return 1;
        case 3:
            return 2;
        case 1:
            return (((vat >> 23) & 1) + 1) * vtxCompSize[(vat >> 24) & 7];
        }
        break;
    }
    return 0;
}

static void __ParseVertexData(u8 vatIdx) {
    u16 vcnt;
    GXAttr attrIdx;
    u32 vsize;

    if (__ReadMem(&vcnt, 2)) {
        vsize = 0;
        for (attrIdx = 0; attrIdx < GX_VA_MAX_ATTR; attrIdx++) {
            if (attrIdx != GX_VA_NBT) {
                vsize += GetAttrSize(vatIdx, attrIdx);
            }
        }
        vsize *= vcnt;
        dlist += vsize;
        bytesRead += vsize;
    }
}

void __GXShadowDispList(void* list, u32 nbytes) {
    u8 cmd;
    u8 cmdOp;
    u8 vatIdx;
    u32 reg32;
    u32 d32;
    u8 reg8;
    u8 cpAddr;
    u32 i;
    u32 addr;
    u32 cnt;

    if (__gxVerif->verifyLevel == GX_WARN_NONE) {
        return;
    }

    dlist = list;
    dlistSize = nbytes;
    bytesRead = 0;

    DPF("Displaylist IN\n");

    while (dlistSize > bytesRead) {
        if (!__ReadMem(&cmd, 1)) {
            break;
        }
        cmdOp = (u32)GET_REG_FIELD((u32)cmd, 5, 3);
        vatIdx = cmd & 7;
        switch (cmdOp) {
        case 0:
        case 9:
            break;
        case 16:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
            __ReconstVtxStatus(vatIdx);
            __GXVerifyState(vatIdx);
            __ParseVertexData(vatIdx);
            break;
        case 1:
            if (__ReadMem(&reg8, 1) && __ReadMem(&d32, 4)) {
                vatIdx = reg8 & 0xF;
                cpAddr = (reg8& 0xF0) >> 4;
                __SaveCPRegs(cpAddr, vatIdx, d32);
            }
            break;
        case 2:
            if (__ReadMem(&reg32, 4)) {
                cnt = GET_REG_FIELD(reg32, 4, 16) + 1;
                addr = (u16)reg32;
                DPF("\tXFReg = 0x%x, Cnt = %d\n", addr, cnt);
                for (i = 0; i < cnt; i++) {
                    if (__ReadMem(&d32, 4)) {
                        DPF("\tXFData = 0x%x\n", d32);
                        VERIF_MTXLIGHT(addr, d32);
                        addr++;
                    }
                }
            }
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            if (__ReadMem(&reg32, 4)) {
                DPF("\tXF_INDEX_LOAD: = 0x%x\n", reg32);
                __GXShadowIndexState(cmdOp, reg32);
            }
            break;
        case 8:
            if (__gxVerif->verifyLevel >= __gxvWarnLev[GXWARN_DL_NESTED]) {
                __GX_WARN(GXWARN_DL_NESTED);
            }
            return;
        case 12:
        case 13:
            if (__ReadMem(&reg32, 4)) {
                DPF("\tSU Bypass = 0x%x\n", reg32);
                __gxVerif->rasRegs[(reg32 >> 24) & 0xFF] = reg32;
            }
            break;
        default:
            if (__gxVerif->verifyLevel >= __gxvWarnLev[GXWARN_DL_INV_CMD]) {
                __GX_WARN(GXWARN_DL_INV_CMD);
            }
            OSReport("[Bad Display List Command: 0x%02X\n]", cmdOp);
            break;
        }
    }

    DPF("Displaylist OUT\n");
}

void __GXShadowIndexState(u32 idx_reg, u32 reg_data) {
    u32* basePtr;
    u32* memAddr;
    u32 cnt;
    u32 stride;
    u32 addr;
    u32 data;
    u32 index;
    u32 i;

    i = idx_reg - 4;
    basePtr = OSPhysicalToCached(__GXData->indexBase[i]);
    stride = __GXData->indexStride[i];
    addr = reg_data & 0xFFF;
    cnt = (reg_data >> 12) & 0xF;
    index = reg_data >> 16;
    memAddr = (u32*)((u8*)basePtr + (index * stride));
    cnt++;

    while (cnt-- != 0) {
        data = *memAddr;
        VERIF_MTXLIGHT(addr, data);
        memAddr = (u32*)((u8*)memAddr + stride);
        addr++;
    }

    &data;  // needed to match
}

void __GXPrintShadowState(void) {
    u32 i;
    u32 j;

    OSReport("CP State:\n");
    OSReport("\tvcdLo = 0x%x\n", __GXData->vcdLo);
    OSReport("\tvcdHi = 0x%x\n", __GXData->vcdHi);
    OSReport("\thasBiNrms = 0x%x\n", __GXData->hasBiNrms);

    for (i = 0; i < 8; i++) {
        OSReport("\tVertex Format %d:\n", i);
        OSReport("\t\tvatA = 0x%x\n", __GXData->vatA[i]);
        OSReport("\t\tvatB = 0x%x\n", __GXData->vatB[i]);
        OSReport("\t\tvatC = 0x%x\n", __GXData->vatC[i]);
    }

    OSReport("\n-------------------------------------\n");
    OSReport("XF Pos/Tex Matrix State:\n");

    for (i = 0; i < 256; i += 4) {
        if (__gxVerif->xfMtxDirty[i]) {
            OSReport("\tXF_MATRIX[%d] = ", i);
            OSReport("%f, %f, %f, %f\n", *(f32*)&__gxVerif->xfMtx[i], *(f32*)&__gxVerif->xfMtx[i + 1], *(f32*)&__gxVerif->xfMtx[i + 2], *(f32*)&__gxVerif->xfMtx[i + 3]);
        }
    }

    OSReport("\n-------------------------------------\n");
    OSReport("XF Normal Matrix State:\n");

    for (i = 0; i < 96; i += 3) {
        if (__gxVerif->xfNrmDirty[i]) {
            OSReport("\tXF_NRM_MTX[%d] = ", i);
            OSReport("%f, %f, %f\n", *(f32*)&__gxVerif->xfMtx[i], *(f32*)&__gxVerif->xfMtx[i + 1], *(f32*)&__gxVerif->xfMtx[i + 2]);
        }
    }

    OSReport("\n-------------------------------------\n");
    OSReport("XF Light State:\n");

    for (i = 0; i < 128; i += 16) {
        if (__gxVerif->xfLightDirty[i]) {
            OSReport("\tXF_LIGHT[%d]:\n", i >> 4);
            for (j = 0; j < 4; j++) {
                OSReport("\t\tparam[%d] = 0x%x\n", j, __gxVerif->xfLight[i + j]);
            }
            for (j = 4; j < 16; j++) {
                OSReport("\t\tparam[%d] = %Lg\n", j, *(f32*)&__gxVerif->xfLight[i + j]);
            }
        }
    }

    OSReport("\n-------------------------------------\n");
    OSReport("XF Register State:\n");

    for (i = 0; i < 80; i++) {
        if (__gxVerif->xfRegsDirty[i]) {
            OSReport("\tXF_REG[0x%x] = 0x%x (%f)\n", i, __gxVerif->xfRegs[i], *(f32*)&__gxVerif->xfRegs[i]);
        }
    }

    OSReport("\n-------------------------------------\n");
    OSReport("Raster Registers State:\n");

    for (i = 0; i < 256; i++) {
        OSReport("\tRAS_REG[0x%x] = 0x%x\n", i, __gxVerif->rasRegs[i]);
    }

    OSReport("\n-------------------------------------\n");
}

#endif
