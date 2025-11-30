#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphBase/J3DSys.h"
#include "JSystem/J3DGraphBase/J3DTevs.h"
#include "JSystem/J3DGraphBase/J3DTexture.h"
#include "JSystem/J3DGraphBase/J3DFifo.h"
#include "global.h"

J3DSys j3dSys;

Mtx J3DSys::mCurrentMtx;

Vec J3DSys::mCurrentS;

Vec J3DSys::mParentS;

J3DTexCoordScaleInfo J3DSys::sTexCoordScaleTable[8];

static u8 NullTexData[0x10] ATTRIBUTE_ALIGN(32) = {0};

static Mtx j3dIdentityMtx = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
};

static Mtx23 IndMtx = {
    0.5f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.0f,
};

u32 j3dDefaultViewNo;

static GXColor ColorBlack = {0x00, 0x00, 0x00, 0x00};

static GXColor ColorWhite = {0xFF, 0xFF, 0xFF, 0xFF};

J3DSys::J3DSys() {
    makeTexCoordTable();
    makeTevSwapTable();
    makeAlphaCmpTable();
    makeZModeTable();

    mFlags = 0;
    MTXIdentity(mViewMtx);
    mDrawMode = 1;
    mMaterialMode = 0;
    mModel = NULL;
    mShape = NULL;

    for (int i = 0; i < 2; i++)
        mDrawBuffer[i] = NULL;

    mTexture = NULL;
    mMatPacket = NULL;
    mShapePacket = NULL;
    mModelDrawMtx = NULL;
    mModelNrmMtx = NULL;
    mVtxPos = NULL;
    mVtxNrm = NULL;
    mVtxCol = NULL;

    for (int i = 0; i < 8; i++) {
        sTexCoordScaleTable[i].field_0x00 = 1;
        sTexCoordScaleTable[i].field_0x02 = 1;
        sTexCoordScaleTable[i].field_0x04 = 0;
        sTexCoordScaleTable[i].field_0x06 = 0;
    }
}

void J3DSys::loadPosMtxIndx(int addr, u16 indx) const {
    // J3DFifoLoadPosMtxIndx(indx, addr * 3);  // matches debug, not retail
    J3DFifoLoadIndx(GX_LOAD_INDX_A, indx, 0xB000 | ((u16)(addr * 0x0C)));
}

void J3DSys::loadNrmMtxIndx(int addr, u16 indx) const {
    J3DFifoLoadNrmMtxIndx3x3(indx, addr * 3);
}

void J3DSys::setTexCacheRegion(GXTexCacheSize size) {
    J3D_ASSERT_RANGE(173, size >= 0 && size < 3);

    const u32 kSize[] = {
        0x00008000,
        0x00020000,
        0x00080000,
        0x00000000,
    };

    const u32 kRegionNum[] = {8, 4, 1, 0};

    mTexCacheRegionNum = kRegionNum[size];

    if (checkFlag(0x80000000)) {
        for (u32 i = 0; i < kRegionNum[size]; i++) {
            if (i & 1) {
                GXInitTexCacheRegion(&mTexCacheRegion[i], GX_FALSE, i * kSize[size] + 0x80000, size,
                                     i * kSize[size], size);
                J3DFifoLoadTexCached((GXTexMapID)i, i * kSize[size] + 0x80000, size,
                                     i * kSize[size], size);
            } else {
                GXInitTexCacheRegion(&mTexCacheRegion[i], GX_FALSE, i * kSize[size], size,
                                     i * kSize[size] + 0x80000, size);
                J3DFifoLoadTexCached((GXTexMapID)i, i * kSize[size], size,
                                     i * kSize[size] + 0x80000, size);
            }
        }
    } else {
        for (u32 i = 0; i < kRegionNum[size]; i++) {
            GXInitTexCacheRegion(&mTexCacheRegion[i], GX_FALSE, i * kSize[size], size,
                                 i * kSize[size] + 0x80000, size);
            J3DFifoLoadTexCached((GXTexMapID)i, i * kSize[size], size, i * kSize[size] + 0x80000,
                                 size);
        }
    }
}

void J3DSys::drawInit() {
    GXInvalidateVtxCache();
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_BACK);
    GXSetCoPlanar(GX_FALSE);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetColorUpdate(GX_TRUE);
    GXSetDither(GX_TRUE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetZCompLoc(GX_TRUE);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR1, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA4, 7);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_CLR_RGBA, GX_RGBA4, 7);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX2, GX_CLR_RGBA, GX_RGBA4, 7);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX3, GX_CLR_RGBA, GX_RGBA4, 7);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX4, GX_CLR_RGBA, GX_RGBA4, 7);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX5, GX_CLR_RGBA, GX_RGBA4, 7);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX6, GX_CLR_RGBA, GX_RGBA4, 7);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX7, GX_CLR_RGBA, GX_RGBA4, 7);

    GXSetNumIndStages(0);
    reinitTexture();

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE4, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE5, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE6, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE7, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE8, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE9, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE10, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE11, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE12, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE13, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE14, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE15, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);

    for (u32 i = 0; i < 10; i++)
        GXLoadTexMtxImm(j3dIdentityMtx, GX_TEXMTX0 + i * 3, GX_MTX3x4);

    Mtx23 indTexMtx = {
        {0.5f, 0.0f, 0.0f},
        {0.0f, 0.5f, 0.0f},
    };

    u8 i;
    for (i = 0; i < 3; i++)
        GXSetIndTexMtx((GXIndTexMtxID)(GX_ITM_0 + i), indTexMtx, 1);

    GXSetChanMatColor(GX_COLOR0A0, j3dDefaultColInfo);
    GXSetChanMatColor(GX_COLOR1A1, j3dDefaultColInfo);

    GXSetNumChans(j3dDefaultNumChans);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_CLAMP, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_CLAMP, GX_AF_NONE);

    for (i = 0; i < GX_MAX_TEXMAP; i++)
        GXSetTexCoordGen((GXTexCoordID)i, (GXTexGenType)j3dDefaultTexCoordInfo[i].mTexGenType,
                          (GXTexGenSrc)j3dDefaultTexCoordInfo[i].mTexGenSrc,
                          j3dDefaultTexCoordInfo[i].mTexGenMtx);

    for (i = 0; i < GX_MAX_INDTEXSTAGE; i++)
        GXSetIndTexCoordScale((GXIndTexStageID)i, GX_ITS_1, GX_ITS_1);

    for (i = 0; i < GX_MAX_TEVREG; i++)
        GXSetTevKColor((GXTevKColorID)i, j3dDefaultTevKColor);

    for (i = 0; i < GX_MAX_TEVREG; i++)
        GXSetTevColorS10((GXTevRegID)i, j3dDefaultTevColor);

    for (i = 0; i < GX_MAX_TEVSTAGE; i++) {
        GXSetTevColorIn((GXTevStageID)i, GX_CC_RASC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevColorOp((GXTevStageID)i, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                        GX_TEVPREV);
        GXSetTevAlphaIn((GXTevStageID)i, GX_CA_RASA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevAlphaOp((GXTevStageID)i, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                        GX_TEVPREV);
    }

    for (i = 0; i < GX_MAX_TEVSTAGE; i++)
        GXSetTevSwapMode((GXTevStageID)i, GX_TEV_SWAP0, GX_TEV_SWAP0);

    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);

    for (i = 0; i < GX_MAX_TEVSTAGE; i++)
        GXSetTevIndirect((GXTevStageID)i, GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_NONE, GX_ITM_OFF,
                         GX_ITW_OFF, GX_ITW_OFF, GX_FALSE, GX_FALSE, GX_ITBA_OFF);

    OSInitFastCast();
    setTexCacheRegion(GX_TEXCACHE_32K);
}

void J3DSys::reinitGX() {
    reinitGenMode();
    reinitLighting();
    reinitTransform();
    reinitTexture();
    reinitTevStages();
    reinitIndStages();
    reinitPixelProc();
    GXFlush();
}

void J3DSys::reinitGenMode() {
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);
    GXSetCullMode(GX_CULL_BACK);
    GXSetCoPlanar(GX_FALSE);
}

void J3DSys::reinitLighting() {
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetChanAmbColor(GX_COLOR0A0, ColorBlack);
    GXSetChanAmbColor(GX_COLOR1A1, ColorBlack);
    GXSetChanMatColor(GX_COLOR0A0, ColorWhite);
    GXSetChanMatColor(GX_COLOR1A1, ColorWhite);
}

void J3DSys::reinitTransform() {
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX2, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD3, GX_TG_MTX2x4, GX_TG_TEX3, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD4, GX_TG_MTX2x4, GX_TG_TEX4, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD5, GX_TG_MTX2x4, GX_TG_TEX5, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD6, GX_TG_MTX2x4, GX_TG_TEX6, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD7, GX_TG_MTX2x4, GX_TG_TEX7, GX_IDENTITY);
}

void J3DSys::reinitTexture() {
    GXTexObj texObj;
    GXInitTexObj(&texObj, NullTexData, 4, 4, GX_TF_IA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXLoadTexObj(&texObj, GX_TEXMAP2);
    GXLoadTexObj(&texObj, GX_TEXMAP3);
    GXLoadTexObj(&texObj, GX_TEXMAP4);
    GXLoadTexObj(&texObj, GX_TEXMAP5);
    GXLoadTexObj(&texObj, GX_TEXMAP6);
    GXLoadTexObj(&texObj, GX_TEXMAP7);
}

void J3DSys::reinitTevStages() {
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE4, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE5, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE6, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE7, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE8, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE9, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE10, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE11, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE12, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE13, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE14, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE15, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);

    GXSetTevColor(GX_TEVREG0, ColorWhite);
    GXSetTevColor(GX_TEVREG1, ColorWhite);
    GXSetTevColor(GX_TEVREG2, ColorWhite);
    GXSetTevKColor(GX_KCOLOR0, ColorWhite);
    GXSetTevKColor(GX_KCOLOR1, ColorWhite);
    GXSetTevKColor(GX_KCOLOR2, ColorWhite);
    GXSetTevKColor(GX_KCOLOR3, ColorWhite);

    u32 i;
    u32 numStages = GX_MAX_TEVSTAGE;
    for (i = 0; i < numStages; i++) {
        GXSetTevColorIn((GXTevStageID)i, GX_CC_RASC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevColorOp((GXTevStageID)i, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                        GX_TEVPREV);
        GXSetTevAlphaIn((GXTevStageID)i, GX_CA_RASA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevAlphaOp((GXTevStageID)i, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                        GX_TEVPREV);
        GXSetTevKColorSel((GXTevStageID)i, GX_TEV_KCSEL_1_4);
        GXSetTevKAlphaSel((GXTevStageID)i, GX_TEV_KASEL_1);
        GXSetTevSwapMode((GXTevStageID)i, GX_TEV_SWAP0, GX_TEV_SWAP0);
    }

    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
}

void J3DSys::reinitIndStages() {
    u32 i;
    for (i = 0; i < GX_MAX_TEVSTAGE; i++) {
        GXSetTevDirect((GXTevStageID)i);
    }

    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD0, GX_TEXMAP0);
    GXSetIndTexOrder(GX_INDTEXSTAGE1, GX_TEXCOORD1, GX_TEXMAP1);
    GXSetIndTexOrder(GX_INDTEXSTAGE2, GX_TEXCOORD2, GX_TEXMAP2);
    GXSetIndTexOrder(GX_INDTEXSTAGE3, GX_TEXCOORD3, GX_TEXMAP3);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE1, GX_ITS_1, GX_ITS_1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE2, GX_ITS_1, GX_ITS_1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE3, GX_ITS_1, GX_ITS_1);
    GXSetIndTexMtx(GX_ITM_0, IndMtx, 1);
    GXSetIndTexMtx(GX_ITM_1, IndMtx, 1);
    GXSetIndTexMtx(GX_ITM_2, IndMtx, 1);
}

void J3DSys::reinitPixelProc() {
    GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetDither(GX_TRUE);
    GXSetFog(GX_FOG_NONE, 0.0f, 1.0f, 0.1f, 1.0f, ColorBlack);
    GXSetFogRangeAdj(GX_FALSE, 0, NULL);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetZCompLoc(GX_TRUE);
}
