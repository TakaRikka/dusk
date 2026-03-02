#include <revolution/gx.h>
#include <revolution/os.h>

#include "__gx.h"

#define CHECK_ATTRPTR(line, attrPtr) ASSERTMSGLINE(line, (attrPtr) != NULL, "GXSetVtxDescv: attrPtr is NULL")
#define CHECK_ATTRNAME(line, attr)   ASSERTMSGLINE(line, (attr) >= GX_VA_PNMTXIDX, "GXSetVtxDesc: Invalid vertex attribute name")
#define CHECK_ATTRNAME2(line, attr)  ASSERTMSGLINE(line, (attr) <= GX_VA_TEX7 || (attr) == GX_VA_NBT, "GXSetVtxDesc: Invalid vertex attribute name")
#define CHECK_ATTRNAME3(line, attr)  ASSERTMSGLINE(line, (attr) >= GX_VA_PNMTXIDX && (attr) < GX_VA_MAX_ATTR, "GXSetVtxDesc: Invalid vertex attribute name")
#define CHECK_ATTRNAME4(line, attr)  ASSERTMSGLINE(line, ((attr) >= GX_VA_POS && (attr) <= GX_VA_TEX7) || (attr) == GX_VA_NBT, "GXSetVtxAttrFmt: Invalid vertex attribute name")
#define CHECK_ATTRNAME5(line, attr)  ASSERTMSGLINE(line, (attr) >= GX_VA_POS && (attr) <= GX_LIGHT_ARRAY, "GXSetArray: Invalid vertex attribute name")
#define CHECK_ATTRTYPE(line, type)   ASSERTMSGLINE(line, (type) >= GX_NONE && (type) <= GX_INDEX16, "GXSetVtxDesc: Invalid vertex attribute type")
#define CHECK_VTXFMT(line, vtxfmt)   ASSERTMSGLINE(line, (vtxfmt) < GX_MAX_VTXFMT, "GXSetVtxAttrFmt: Format Index is out of range")
#define CHECK_FRAC(line, frac)       ASSERTMSGLINE(line, (frac) < 32, "GXSetVtxAttrFmt: Frac value is >= 32")
#define CHECK_LISTPTR(line, list)    ASSERTMSGLINE(line, (list) != NULL, "GXSetVtxAttrFmt: list pointer is NULL")
#define CHECK_MTXIDX(line, attr, type)    ASSERTMSGLINE(line, (attr) > GX_VA_TEX7MTXIDX || (type) <= GX_VA_TEX0MTXIDX, "GXSetVtxDesc: GX_VA_*MTXIDX accepts GX_NONE or GX_DIRECT only")

static void __GXXfVtxSpecs(void) {
    u32 nCols;
    u32 nNrm;
    u32 nTex;
    u32 reg;

    nNrm = __GXData->hasBiNrms ? 2 : __GXData->hasNrms ? 1 : 0;

#if DEBUG
    nCols = CP_VCD_REG_LO_GET_COL0(__GXData->vcdLo) ? 1 : 0;
    nCols += CP_VCD_REG_LO_GET_COL1(__GXData->vcdLo) ? 1 : 0;
#else
    nCols = (u32)__cntlzw((__GXData->vcdLo & CP_VCD_REG_LO_COLALL_MASK)>>CP_VCD_REG_LO_COLALL_SHIFT);
    nCols = (33 - nCols) >> 1;
#endif

#if DEBUG
    nTex = 0;
    nTex += CP_VCD_REG_HI_GET_TEX0(__GXData->vcdHi) ? 1 : 0;
    nTex += CP_VCD_REG_HI_GET_TEX1(__GXData->vcdHi) ? 1 : 0;
    nTex += CP_VCD_REG_HI_GET_TEX2(__GXData->vcdHi) ? 1 : 0;
    nTex += CP_VCD_REG_HI_GET_TEX3(__GXData->vcdHi) ? 1 : 0;
    nTex += CP_VCD_REG_HI_GET_TEX4(__GXData->vcdHi) ? 1 : 0;
    nTex += CP_VCD_REG_HI_GET_TEX5(__GXData->vcdHi) ? 1 : 0;
    nTex += CP_VCD_REG_HI_GET_TEX6(__GXData->vcdHi) ? 1 : 0;
    nTex += CP_VCD_REG_HI_GET_TEX7(__GXData->vcdHi) ? 1 : 0;
#else
    nTex = (u32)__cntlzw((__GXData->vcdHi & CP_VCD_REG_HI_TEXALL_MASK)>>CP_VCD_REG_HI_TEXALL_SHIFT);
    nTex = (33 - nTex) >> 1;
#endif

    reg = XF_INVERTEXSPEC_F(nCols, nNrm, nTex);
    GX_WRITE_XF_REG(0x1008, reg, 0);
    __GXData->bpSentNot = 1;
}

static inline void SETVCDATTR(GXAttr Attr, GXAttrType Type) {
    switch (Attr) {
    case GX_VA_PNMTXIDX:   SC_CP_VCD_REG_LO_SET_PMIDX(219, __GXData->vcdLo, Type); break;
    case GX_VA_TEX0MTXIDX: SC_CP_VCD_REG_LO_SET_T0MIDX(220, __GXData->vcdLo, Type); break;
    case GX_VA_TEX1MTXIDX: SC_CP_VCD_REG_LO_SET_T1MIDX(221, __GXData->vcdLo, Type); break;
    case GX_VA_TEX2MTXIDX: SC_CP_VCD_REG_LO_SET_T2MIDX(222, __GXData->vcdLo, Type); break;
    case GX_VA_TEX3MTXIDX: SC_CP_VCD_REG_LO_SET_T3MIDX(223, __GXData->vcdLo, Type); break;
    case GX_VA_TEX4MTXIDX: SC_CP_VCD_REG_LO_SET_T4MIDX(224, __GXData->vcdLo, Type); break;
    case GX_VA_TEX5MTXIDX: SC_CP_VCD_REG_LO_SET_T5MIDX(225, __GXData->vcdLo, Type); break;
    case GX_VA_TEX6MTXIDX: SC_CP_VCD_REG_LO_SET_T6MIDX(226, __GXData->vcdLo, Type); break;
    case GX_VA_TEX7MTXIDX: SC_CP_VCD_REG_LO_SET_T7MIDX(227, __GXData->vcdLo, Type); break;
    case GX_VA_POS:        SC_CP_VCD_REG_LO_SET_POS(228, __GXData->vcdLo, Type); break;
    case GX_VA_NRM:
        if (Type != GX_NONE) {
            __GXData->hasNrms = 1;
            __GXData->hasBiNrms = 0;
            __GXData->nrmType = Type;
        } else {
            __GXData->hasNrms = 0;
        }
        break;
    case GX_VA_NBT:
        if (Type != GX_NONE) {
            __GXData->hasBiNrms = 1;
            __GXData->hasNrms = 0;
            __GXData->nrmType = Type;
        } else {
            __GXData->hasBiNrms = 0;
        }
        break;
    case GX_VA_CLR0: SC_CP_VCD_REG_LO_SET_COL0(253, __GXData->vcdLo, Type); break;
    case GX_VA_CLR1: SC_CP_VCD_REG_LO_SET_COL1(254, __GXData->vcdLo, Type); break;
    case GX_VA_TEX0: SC_CP_VCD_REG_HI_SET_TEX0(255, __GXData->vcdHi, Type); break;
    case GX_VA_TEX1: SC_CP_VCD_REG_HI_SET_TEX1(256, __GXData->vcdHi, Type); break;
    case GX_VA_TEX2: SC_CP_VCD_REG_HI_SET_TEX2(257, __GXData->vcdHi, Type); break;
    case GX_VA_TEX3: SC_CP_VCD_REG_HI_SET_TEX3(258, __GXData->vcdHi, Type); break;
    case GX_VA_TEX4: SC_CP_VCD_REG_HI_SET_TEX4(259, __GXData->vcdHi, Type); break;
    case GX_VA_TEX5: SC_CP_VCD_REG_HI_SET_TEX5(260, __GXData->vcdHi, Type); break;
    case GX_VA_TEX6: SC_CP_VCD_REG_HI_SET_TEX6(261, __GXData->vcdHi, Type); break;
    case GX_VA_TEX7: SC_CP_VCD_REG_HI_SET_TEX7(262, __GXData->vcdHi, Type); break;
    }
}

void GXSetVtxDesc(GXAttr attr, GXAttrType type) {
    CHECK_GXBEGIN(271, "GXSetVtxDesc");
    CHECK_ATTRNAME(274, attr);
    CHECK_ATTRNAME2(276, attr);
    CHECK_ATTRTYPE(278, type);
    CHECK_MTXIDX(281, attr, type);

    SETVCDATTR(attr, type);
    if (__GXData->hasNrms || __GXData->hasBiNrms) {
        SC_CP_VCD_REG_LO_SET_NRM(287, __GXData->vcdLo, __GXData->nrmType);
    } else {
        SC_CP_VCD_REG_LO_SET_NRM(0, __GXData->vcdLo, 0);
    }
    __GXData->dirtyState |= 8;
}

void GXSetVtxDescv(const GXVtxDescList *attrPtr) {
    CHECK_GXBEGIN(313, "GXSetVtxDescv");
    CHECK_ATTRPTR(314, attrPtr);

    while (attrPtr->attr != GX_VA_NULL) {
        CHECK_ATTRNAME(318, attrPtr->attr);
        CHECK_ATTRNAME2(321, attrPtr->attr);
        CHECK_ATTRTYPE(324, attrPtr->type);
        SETVCDATTR(attrPtr->attr, attrPtr->type);
        attrPtr++;
    }

    if (__GXData->hasNrms || __GXData->hasBiNrms) {
        SC_CP_VCD_REG_LO_SET_NRM(333, __GXData->vcdLo, __GXData->nrmType);
    } else {
        SC_CP_VCD_REG_LO_SET_NRM(0, __GXData->vcdLo, 0);
    }
    __GXData->dirtyState |= 8;
}

void __GXSetVCD(void) {
    GX_WRITE_CP_STRM_REG_alt(CP_VCD_LO, 0, __GXData->vcdLo, -12);
    GX_WRITE_CP_STRM_REG_alt(CP_VCD_HI, 0, __GXData->vcdHi, -12);
    __GXXfVtxSpecs();
}

void __GXCalculateVLim(void) {
    static u8 tbl1[] = { 0, 4, 1, 2 };
    static u8 tbl2[] = { 0, 8, 1, 2 };
    static u8 tbl3[] = { 0, 12, 1, 2 };

    if (__GXData->vNum != 0) {
        GXCompCnt nc;
        u32 vlm;
        u32 b;
        u32 vl;
        u32 vh;
        u32 va;

        vl = __GXData->vcdLo;
        vh = __GXData->vcdHi;
        va = __GXData->vatA[0];
        nc = CP_VAT_REG_A_GET_NRMCNT(va);

        vlm  = ((vl >> 0) & 0x1);
        vlm += ((vl >> 1) & 0x1);
        vlm += ((vl >> 2) & 0x1);
        vlm += ((vl >> 3) & 0x1);
        vlm += ((vl >> 4) & 0x1);
        vlm += ((vl >> 5) & 0x1);
        vlm += ((vl >> 6) & 0x1);
        vlm += ((vl >> 7) & 0x1);
        vlm += ((vl >> 8) & 0x1);
        vlm += tbl3[((vl >> 9) & 0x3)];
        
        b = (u32)((nc == GX_NRM_NBT) ? 3 : 1);

        vlm += tbl3[((vl >> 11) & 0x3)] * b;
        vlm += tbl1[((vl >> 13) & 0x3)];
        vlm += tbl1[((vl >> 15) & 0x3)];
        vlm += tbl2[((vh >> (0*2)) & 0x3)];
        vlm += tbl2[((vh >> (1*2)) & 0x3)];
        vlm += tbl2[((vh >> (2*2)) & 0x3)];
        vlm += tbl2[((vh >> (3*2)) & 0x3)];
        vlm += tbl2[((vh >> (4*2)) & 0x3)];
        vlm += tbl2[((vh >> (5*2)) & 0x3)];
        vlm += tbl2[((vh >> (6*2)) & 0x3)];
        vlm += tbl2[((vh >> (7*2)) & 0x3)];

        __GXData->vLim = (u16)vlm;
    }
}

void GXGetVtxDesc(GXAttr attr, GXAttrType* type) {
    u32 cpType;

    CHECK_GXBEGIN(465, "GXGetVtxDesc");
    CHECK_ATTRNAME3(467, attr);

    switch (attr) {
    case GX_VA_PNMTXIDX:   cpType = CP_VCD_REG_LO_GET_PMIDX(__GXData->vcdLo); break;
    case GX_VA_TEX0MTXIDX: cpType = CP_VCD_REG_LO_GET_T0MIDX(__GXData->vcdLo); break;
    case GX_VA_TEX1MTXIDX: cpType = CP_VCD_REG_LO_GET_T1MIDX(__GXData->vcdLo); break;
    case GX_VA_TEX2MTXIDX: cpType = CP_VCD_REG_LO_GET_T2MIDX(__GXData->vcdLo); break;
    case GX_VA_TEX3MTXIDX: cpType = CP_VCD_REG_LO_GET_T3MIDX(__GXData->vcdLo); break;
    case GX_VA_TEX4MTXIDX: cpType = CP_VCD_REG_LO_GET_T4MIDX(__GXData->vcdLo); break;
    case GX_VA_TEX5MTXIDX: cpType = CP_VCD_REG_LO_GET_T5MIDX(__GXData->vcdLo); break;
    case GX_VA_TEX6MTXIDX: cpType = CP_VCD_REG_LO_GET_T6MIDX(__GXData->vcdLo); break;
    case GX_VA_TEX7MTXIDX: cpType = CP_VCD_REG_LO_GET_T7MIDX(__GXData->vcdLo); break;
    case GX_VA_POS:        cpType = CP_VCD_REG_LO_GET_POS(__GXData->vcdLo); break;
    case GX_VA_NRM:        cpType = __GXData->hasNrms ? CP_VCD_REG_LO_GET_NRM(__GXData->vcdLo) : 0; break;
    case GX_VA_NBT:        cpType = __GXData->hasBiNrms ? CP_VCD_REG_LO_GET_NRM(__GXData->vcdLo) : 0; break;
    case GX_VA_CLR0:       cpType = CP_VCD_REG_LO_GET_COL0(__GXData->vcdLo); break;
    case GX_VA_CLR1:       cpType = CP_VCD_REG_LO_GET_COL1(__GXData->vcdLo); break;
    case GX_VA_TEX0:       cpType = CP_VCD_REG_HI_GET_TEX0(__GXData->vcdHi);  break;
    case GX_VA_TEX1:       cpType = CP_VCD_REG_HI_GET_TEX1(__GXData->vcdHi);  break;
    case GX_VA_TEX2:       cpType = CP_VCD_REG_HI_GET_TEX2(__GXData->vcdHi);  break;
    case GX_VA_TEX3:       cpType = CP_VCD_REG_HI_GET_TEX3(__GXData->vcdHi);  break;
    case GX_VA_TEX4:       cpType = CP_VCD_REG_HI_GET_TEX4(__GXData->vcdHi);  break;
    case GX_VA_TEX5:       cpType = CP_VCD_REG_HI_GET_TEX5(__GXData->vcdHi); break;
    case GX_VA_TEX6:       cpType = CP_VCD_REG_HI_GET_TEX6(__GXData->vcdHi); break;
    case GX_VA_TEX7:       cpType = CP_VCD_REG_HI_GET_TEX7(__GXData->vcdHi); break;
    default: cpType = 0; break;
    }
    *type = cpType;
}

void GXGetVtxDescv(GXVtxDescList* vcd) {
    GXAttr attr;

    CHECK_GXBEGIN(518, "GXGetVtxDescv");
    CHECK_ATTRPTR(520, vcd);

    for (attr = GX_VA_PNMTXIDX; attr <= GX_VA_TEX7; attr++) {
        vcd[attr].attr = attr;
        GXGetVtxDesc(attr, &vcd[attr].type);
    }

    vcd[attr].attr = GX_VA_NBT;
    GXGetVtxDesc(GX_VA_NBT, &vcd[attr].type);

    attr++;
    vcd[attr].attr = GX_VA_NULL;
}

void GXClearVtxDesc(void) {
    CHECK_GXBEGIN(550, "GXClearVtxDesc");
    __GXData->vcdLo = 0;
    SC_CP_VCD_REG_LO_SET_POS(0, __GXData->vcdLo, GX_DIRECT);
    __GXData->vcdHi = 0;
    __GXData->hasNrms = 0;
    __GXData->hasBiNrms = 0;
    __GXData->dirtyState |= 8;
}

static inline void SETVAT(u32* va, u32* vb, u32* vc, GXAttr attr, GXCompCnt cnt, GXCompType type, u8 shft) {
    switch (attr) {
    case GX_VA_POS:
        SC_CP_VAT_REG_A_SET_POSCNT(590, *va, cnt);
        SC_CP_VAT_REG_A_SET_POSFMT(591, *va, type);
        SC_CP_VAT_REG_A_SET_POSSHFT(592, *va, shft);
        break;
    case GX_VA_NRM:
    case GX_VA_NBT:
        SC_CP_VAT_REG_A_SET_NRMFMT(600, *va, type);
        if (cnt == GX_NRM_NBT3) {
            SC_CP_VAT_REG_A_SET_NRMCNT(0, *va, 1);
            SC_CP_VAT_REG_A_SET_NORMALINDEX3(0, *va, 1);
        } else {
            SC_CP_VAT_REG_A_SET_NRMCNT(606, *va, cnt);
            SC_CP_VAT_REG_A_SET_NORMALINDEX3(606, *va, 0);
        }
        break;
    case GX_VA_CLR0:
        SC_CP_VAT_REG_A_SET_COL0CNT(612, *va, cnt);
        SC_CP_VAT_REG_A_SET_COL0FMT(613, *va, type);
        break;
    case GX_VA_CLR1:
        SC_CP_VAT_REG_A_SET_COL1CNT(616, *va, cnt);
        SC_CP_VAT_REG_A_SET_COL1FMT(617, *va, type);
        break;
    case GX_VA_TEX0:
        SC_CP_VAT_REG_A_SET_TEX0CNT(620, *va, cnt);
        SC_CP_VAT_REG_A_SET_TEX0FMT(621, *va, type);
        SC_CP_VAT_REG_A_SET_TEX0SHFT(622, *va, shft);
        break;
    case GX_VA_TEX1:
        SC_CP_VAT_REG_B_SET_TEX1CNT(625, *vb, cnt);
        SC_CP_VAT_REG_B_SET_TEX1FMT(626, *vb, type);
        SC_CP_VAT_REG_B_SET_TEX1SHFT(627, *vb, shft);
        break;
    case GX_VA_TEX2:
        SC_CP_VAT_REG_B_SET_TEX2CNT(630, *vb, cnt);
        SC_CP_VAT_REG_B_SET_TEX2FMT(631, *vb, type);
        SC_CP_VAT_REG_B_SET_TEX2SHFT(632, *vb, shft);
        break;
    case GX_VA_TEX3:
        SC_CP_VAT_REG_B_SET_TEX3CNT(635, *vb, cnt);
        SC_CP_VAT_REG_B_SET_TEX3FMT(636, *vb, type);
        SC_CP_VAT_REG_B_SET_TEX3SHFT(637, *vb, shft);
        break;
    case GX_VA_TEX4:
        SC_CP_VAT_REG_B_SET_TEX4CNT(640, *vb, cnt);
        SC_CP_VAT_REG_B_SET_TEX4FMT(641, *vb, type);
        SC_CP_VAT_REG_C_SET_TEX4SHFT(642, *vc, shft);
        break;
    case GX_VA_TEX5:
        SC_CP_VAT_REG_C_SET_TEX5CNT(645, *vc, cnt);
        SC_CP_VAT_REG_C_SET_TEX5FMT(646, *vc, type);
        SC_CP_VAT_REG_C_SET_TEX5SHFT(647, *vc, shft);
        break;
    case GX_VA_TEX6:
        SC_CP_VAT_REG_C_SET_TEX6CNT(650, *vc, cnt);
        SC_CP_VAT_REG_C_SET_TEX6FMT(651, *vc, type);
        SC_CP_VAT_REG_C_SET_TEX6SHFT(652, *vc, shft);
        break;
    case GX_VA_TEX7:
        SC_CP_VAT_REG_C_SET_TEX7CNT(655, *vc, cnt);
        SC_CP_VAT_REG_C_SET_TEX7FMT(656, *vc, type);
        SC_CP_VAT_REG_C_SET_TEX7SHFT(657, *vc, shft);
        break;
    }
}

void GXSetVtxAttrFmt(GXVtxFmt vtxfmt, GXAttr attr, GXCompCnt cnt, GXCompType type, u8 frac) {
    u32* va;
    u32* vb;
    u32* vc;

    CHECK_GXBEGIN(673, "GXSetVtxAttrFmt");
    CHECK_VTXFMT(674, vtxfmt);
    CHECK_ATTRNAME4(678, attr);
    CHECK_FRAC(679, frac);

    va = &__GXData->vatA[vtxfmt];
    vb = &__GXData->vatB[vtxfmt];
    vc = &__GXData->vatC[vtxfmt];
    SETVAT(va, vb, vc, attr, cnt, type, frac);

#if DEBUG
    __GXVerifyVATImm(attr, cnt, type, frac);
#endif

    __GXData->dirtyState |= 0x10;
    __GXData->dirtyVAT |= (u8)(1 << (u8)vtxfmt);
}

void GXSetVtxAttrFmtv(GXVtxFmt vtxfmt, const GXVtxAttrFmtList* list) {
    u32* va;
    u32* vb;
    u32* vc;

    CHECK_GXBEGIN(720, "GXSetVtxAttrFmtv");
    CHECK_LISTPTR(721, list);
    CHECK_VTXFMT(722, vtxfmt);

    va = &__GXData->vatA[vtxfmt];
    vb = &__GXData->vatB[vtxfmt];
    vc = &__GXData->vatC[vtxfmt];

    while (list->attr != GX_VA_NULL) {
        CHECK_ATTRNAME4(732, list->attr);
        CHECK_FRAC(733, list->frac);
        SETVAT(va, vb, vc, list->attr, list->cnt, list->type, list->frac);
#if DEBUG
        __GXVerifyVATImm(list->attr, list->cnt, list->type, list->frac);
#endif
        list++;
    }
    __GXData->dirtyState |= 0x10;
    __GXData->dirtyVAT |= (u8)(1 << (u8)vtxfmt);
}

void __GXSetVAT(void) {
    s32 i;
    u32 dirty = __GXData->dirtyVAT;
    
    i = 0;
    do {
        if (dirty & 1) {
            GX_WRITE_SOME_REG4(8, i | 0x70, __GXData->vatA[i], i - 12);
            GX_WRITE_SOME_REG4(8, i | 0x80, __GXData->vatB[i], i - 12);
            GX_WRITE_SOME_REG4(8, i | 0x90, __GXData->vatC[i], i - 12);
        }

        dirty >>= 1;
        i++;
    } while (dirty != 0);

    GX_WRITE_U8(0);
    __GXData->dirtyVAT = 0;
}

static inline u8 GetFracForNrm(GXCompType type) {
    u8 frac;

    switch (type) {
    case GX_S8:
        frac = 6;
        break;
    case GX_S16:
        frac = 14;
        break;
    default:
    case GX_U16:
        frac = 0;
        break;
    }

    return frac;
}

void GXGetVtxAttrFmt(GXVtxFmt fmt, GXAttr attr, GXCompCnt* cnt, GXCompType* type, u8* frac) {
    u32* va;
    u32* vb;
    u32* vc;

    CHECK_GXBEGIN(844, "GXGetVtxAttrFmt");
    CHECK_VTXFMT(845, fmt);

    va = &__GXData->vatA[fmt];
    vb = &__GXData->vatB[fmt];
    vc = &__GXData->vatC[fmt];

    switch (attr) {
    case GX_VA_POS:
        *cnt  = CP_VAT_REG_A_GET_POSCNT(*va);
        *type = CP_VAT_REG_A_GET_POSFMT(*va);
        *frac = CP_VAT_REG_A_GET_POSSHFT(*va);
        return;
    case GX_VA_NRM:
    case GX_VA_NBT:
        *cnt = CP_VAT_REG_A_GET_NRMCNT(*va);
        if (*cnt == GX_TEX_ST && CP_VAT_REG_A_GET_NORMALINDEX3(*va)) {
            *cnt = GX_NRM_NBT3;
        }
        *type = CP_VAT_REG_A_GET_NRMFMT(*va);
        *frac = GetFracForNrm(*type);
        return;
    case GX_VA_CLR0:
        *cnt  = CP_VAT_REG_A_GET_COL0CNT(*va);
        *type = CP_VAT_REG_A_GET_COL0FMT(*va);
        *frac = 0;
        return;
    case GX_VA_CLR1:
        *cnt  = CP_VAT_REG_A_GET_COL1CNT(*va);
        *type = CP_VAT_REG_A_GET_COL1FMT(*va);
        *frac = 0;
        return;
    case GX_VA_TEX0:
        *cnt  = CP_VAT_REG_A_GET_TEX0CNT(*va);
        *type = CP_VAT_REG_A_GET_TEX0FMT(*va);
        *frac = CP_VAT_REG_A_GET_TEX0SHFT(*va);
        return;
    case GX_VA_TEX1:
        *cnt  = CP_VAT_REG_B_GET_TEX1CNT(*vb);
        *type = CP_VAT_REG_B_GET_TEX1FMT(*vb);
        *frac = CP_VAT_REG_B_GET_TEX1SHFT(*vb);
        return;
    case GX_VA_TEX2:
        *cnt  = CP_VAT_REG_B_GET_TEX2CNT(*vb);
        *type = CP_VAT_REG_B_GET_TEX2FMT(*vb);
        *frac = CP_VAT_REG_B_GET_TEX2SHFT(*vb);
        return;
    case GX_VA_TEX3:
        *cnt  = CP_VAT_REG_B_GET_TEX3CNT(*vb);
        *type = CP_VAT_REG_B_GET_TEX3FMT(*vb);
        *frac = CP_VAT_REG_B_GET_TEX3SHFT(*vb);
        return;
    case GX_VA_TEX4:
        *cnt  = CP_VAT_REG_B_GET_TEX4CNT(*vb);
        *type = CP_VAT_REG_B_GET_TEX4FMT(*vb);
        *frac = CP_VAT_REG_C_GET_TEX4SHFT(*vc);
        return;
    case GX_VA_TEX5:
        *cnt  = CP_VAT_REG_C_GET_TEX5CNT(*vc);
        *type = CP_VAT_REG_C_GET_TEX5FMT(*vc);
        *frac = CP_VAT_REG_C_GET_TEX5SHFT(*vc);
        return;
    case GX_VA_TEX6:
        *cnt  = CP_VAT_REG_C_GET_TEX6CNT(*vc);
        *type = CP_VAT_REG_C_GET_TEX6FMT(*vc);
        *frac = CP_VAT_REG_C_GET_TEX6SHFT(*vc);
        return;
    case GX_VA_TEX7:
        *cnt  = CP_VAT_REG_C_GET_TEX7CNT(*vc);
        *type = CP_VAT_REG_C_GET_TEX7FMT(*vc);
        *frac = CP_VAT_REG_C_GET_TEX7SHFT(*vc);
        return;
    default:
        *cnt = GX_POS_XYZ;
        *type = GX_U8;
        *frac = 0;
        return;
    }
}

void GXGetVtxAttrFmtv(GXVtxFmt fmt, GXVtxAttrFmtList* vat) {
    GXAttr attr;

    CHECK_GXBEGIN(941, "GXGetVtxAttrFmtv");
    CHECK_LISTPTR(942, vat);
    CHECK_VTXFMT(943, fmt);

    for (attr = GX_VA_POS; attr <= GX_VA_TEX7; attr++) {
        vat->attr = attr;
        GXGetVtxAttrFmt(fmt, attr, &vat->cnt, &vat->type, &vat->frac);
        vat++;
    }

    vat->attr = GX_VA_NULL;
}

void GXSetArray(GXAttr attr, void* base_ptr, u8 stride) {
    GXAttr cpAttr;
    u32 phyAddr;

    CHECK_GXBEGIN(974, "GXSetArray");
    attr = (attr == GX_VA_NBT) ? GX_VA_NRM : attr;

    CHECK_ATTRNAME5(977, attr);
    cpAttr = (GXAttr)(attr - GX_VA_POS);
    phyAddr = GX_PHY_ADDR(base_ptr);

    GX_WRITE_CP_STRM_REG_alt2(CP_ARRAY_BASE, cpAttr, phyAddr, cpAttr - 12);
    GX_WRITE_CP_STRM_REG_alt3(CP_ARRAY_STRIDE, cpAttr, (u32)stride, cpAttr - 12);
}

void GXInvalidateVtxCache(void) {
    CHECK_GXBEGIN(999, "GXInvalidateVtxCache");
    GX_WRITE_U8(0x48);
}

void GXSetTexCoordGen2(GXTexCoordID dst_coord, GXTexGenType func, GXTexGenSrc src_param, u32 mtx, GXBool normalize, u32 pt_texmtx) {
    u32 reg = 0;
    u32 row;
    u32 bumprow;
    u32 form;
    GXAttr mtxIdAttr;

    CHECK_GXBEGIN(1041, "GXSetTexCoordGen");
    ASSERTMSGLINE(1042, dst_coord < GX_MAX_TEXCOORD, "GXSetTexCoordGen: Invalid coordinate Id");

    form = 0;
    row = bumprow = 5;
    switch (src_param) {
    case GX_TG_POS:     row = 0; form = 1; break;
    case GX_TG_NRM:     row = 1; form = 1; break;
    case GX_TG_BINRM:   row = 3; form = 1; break;
    case GX_TG_TANGENT: row = 4; form = 1; break;
    case GX_TG_COLOR0:  row = 2; break;
    case GX_TG_COLOR1:  row = 2; break;
    case GX_TG_TEX0:    row = 5; break;
    case GX_TG_TEX1:    row = 6; break;
    case GX_TG_TEX2:    row = 7; break;
    case GX_TG_TEX3:    row = 8; break;
    case GX_TG_TEX4:    row = 9; break;
    case GX_TG_TEX5:    row = 10; break;
    case GX_TG_TEX6:    row = 11; break;
    case GX_TG_TEX7:    row = 12; break;
    case GX_TG_TEXCOORD0: bumprow = 5; break;
    case GX_TG_TEXCOORD1: bumprow = 6; break;
    case GX_TG_TEXCOORD2: bumprow = 7; break;
    case GX_TG_TEXCOORD3: bumprow = 8; break;
    case GX_TG_TEXCOORD4: bumprow = 9; break;
    case GX_TG_TEXCOORD5: bumprow = 10; break;
    case GX_TG_TEXCOORD6: bumprow = 11; break;
    default:
        ASSERTMSGLINE(1070, 0, "GXSetTexCoordGen: Invalid source parameter");
        break;
    }

    switch (func) {
    case GX_TG_MTX2x4:
        SC_XF_TEX_SET_PROJECTION(1080, reg, 0);
        SC_XF_TEX_SET_INPUT_FORM(1080, reg, form);
        SC_XF_TEX_SET_TEXGEN_TYPE(1082, reg, 0);
        SC_XF_TEX_SET_SOURCE_ROW(1082, reg, row);
        break;
    case GX_TG_MTX3x4:
        SC_XF_TEX_SET_PROJECTION(1087, reg, 1);
        SC_XF_TEX_SET_INPUT_FORM(1087, reg, form);
        SC_XF_TEX_SET_TEXGEN_TYPE(1087, reg, 0);
        SC_XF_TEX_SET_SOURCE_ROW(1089, reg, row);
        break;
    case GX_TG_BUMP0:
    case GX_TG_BUMP1:
    case GX_TG_BUMP2:
    case GX_TG_BUMP3:
    case GX_TG_BUMP4:
    case GX_TG_BUMP5:
    case GX_TG_BUMP6:
    case GX_TG_BUMP7:
        ASSERTMSGLINE(1102, src_param >= 12 && src_param <= 18, "GXSetTexCoordGen:  Bump source texture value is invalid");
        SC_XF_TEX_SET_PROJECTION(1104, reg, 0);
        SC_XF_TEX_SET_INPUT_FORM(1104, reg, form);
        SC_XF_TEX_SET_TEXGEN_TYPE(1106, reg, 1);
        SC_XF_TEX_SET_SOURCE_ROW(1106, reg, row);
        SC_XF_TEX_SET_BUMP_MAP_SOURCE(1107, reg, src_param - 12);
        SC_XF_TEX_SET_BUMP_MAP_LIGHT(1108, reg, func - 2);
        break;
    case GX_TG_SRTG:
        SC_XF_TEX_SET_PROJECTION(1113, reg, 0);
        SC_XF_TEX_SET_INPUT_FORM(1113, reg, form);
        if (src_param == GX_TG_COLOR0) {
            SC_XF_TEX_SET_TEXGEN_TYPE(0, reg, 2);
        } else {
            SC_XF_TEX_SET_TEXGEN_TYPE(0, reg, 3);
        }
        SC_XF_TEX_SET_SOURCE_ROW(0, reg, 2);
        break;
    default:
        ASSERTMSGLINE(1124, 0, "GXSetTexCoordGen:  Invalid function");
        break;
    }

    __GXData->texGenCtrl[dst_coord] = reg;
    __GXData->dirtyState |= (0x10000 << dst_coord);

    reg = 0;
    SC_XF_DUALTEX_F_SET_DUALMATRIX_ADRS(1143, reg, pt_texmtx - 64);
    SC_XF_DUALTEX_F_SET_NORMAL_ENABLE(1144, reg, normalize);
    __GXData->texGenCtrl2[dst_coord] = reg;

    switch (dst_coord) {
    case GX_TEXCOORD0: SC_CP_MATIDX_REG_A_SET_TEX0IDX(1158, __GXData->matIdxA, mtx); break;
    case GX_TEXCOORD1: SC_CP_MATIDX_REG_A_SET_TEX1IDX(1159, __GXData->matIdxA, mtx); break;
    case GX_TEXCOORD2: SC_CP_MATIDX_REG_A_SET_TEX2IDX(1160, __GXData->matIdxA, mtx); break;
    case GX_TEXCOORD3: SC_CP_MATIDX_REG_A_SET_TEX3IDX(1161, __GXData->matIdxA, mtx); break;
    case GX_TEXCOORD4: SC_CP_MATIDX_REG_B_SET_TEX4IDX(1162, __GXData->matIdxB, mtx); break;
    case GX_TEXCOORD5: SC_CP_MATIDX_REG_B_SET_TEX5IDX(1163, __GXData->matIdxB, mtx); break;
    case GX_TEXCOORD6: SC_CP_MATIDX_REG_B_SET_TEX6IDX(1164, __GXData->matIdxB, mtx); break;
    default:           SC_CP_MATIDX_REG_B_SET_TEX7IDX(1165, __GXData->matIdxB, mtx); break;
    }

    mtxIdAttr = dst_coord + 1;
    __GXData->dirtyState |= 0x4000000;
}

void GXSetNumTexGens(u8 nTexGens) {
    CHECK_GXBEGIN(1183, "GXSetNumTexGens");

    SC_GEN_MODE_SET_NTEX(1185, __GXData->genMode, nTexGens);
    __GXData->dirtyState |= 0x2000004;
}
