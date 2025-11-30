/**
 * m_Do_graphic.cpp
 * Graphics Management Functions
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DOrthoGraph.h"
#include "JSystem/J2DGraph/J2DPrint.h"
#include "JSystem/JFramework/JFWSystem.h"
#include "JSystem/JParticle/JPADrawInfo.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "JSystem/JUtility/JUTProcBar.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_menu_collect.h"
#include <dolphin/base/PPCArch.h>
#include "f_ap/f_ap_game.h"
#include "f_op/f_op_camera_mng.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_machine.h"
#include "m_Do/m_Do_main.h"
#include "d/d_debug_viewer.h"
#include "d/d_meter2_info.h"
#include "d/d_s_play.h"
#include "DynamicLink.h"

#if PLATFORM_WII || PLATFORM_SHIELD
#include <revolution/sc.h>
#endif

#if PLATFORM_WII
#include "d/d_cursor_mng.h"
#endif

class mDoGph_HIO_c : public JORReflexible {
public:
    mDoGph_HIO_c() {
        id = 0;
    }

    void entryHIO() {
        if (id == 0) {
            id = mDoHIO_CREATE_CHILD("グラフィック", this);
        }
    }

    void listenPropertyEvent(const JORPropertyEvent*) {}
    void genMessage(JORMContext*) {}

    /* 0x4 */ s8 id;
};

static void drawQuad(f32 param_0, f32 param_1, f32 param_2, f32 param_3) {
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2f32(param_0, param_1);
    GXPosition2f32(param_2, param_1);
    GXPosition2f32(param_2, param_3);
    GXPosition2f32(param_0, param_3);
    GXEnd();
}

#if DEBUG
class dDlst_heapMap_c : public dDlst_base_c {
public:
    dDlst_heapMap_c() {
        m_heap = NULL;
    }

    void set(JKRExpHeap*, f32, f32, f32, f32);

    virtual void draw();

    /* 0x04 */ JKRExpHeap* m_heap;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
};

void dDlst_heapMap_c::draw() {
    JUT_ASSERT(111, m_heap != NULL);

    static const GXColor l_noUsedColor = {0x00, 0x00, 0x80, 0xC8};
    static const GXColor l_usedColor = {0xFF, 0x00, 0x00, 0xC8};
    static const GXColor l_tempColor = {0x00, 0xFF, 0x00, 0xC8};

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXSetZCompLoc(GX_ENABLE);
    GXSetZMode(GX_DISABLE, GX_ALWAYS, GX_DISABLE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, g_clearColor);
    GXSetCullMode(GX_CULL_NONE);
    GXSetDither(GX_ENABLE);
    GXSetClipMode(GX_CLIP_DISABLE);

    GXLoadPosMtxImm(cMtx_getIdentity(), 0);
    GXSetCurrentMtx(0);

    GXSetTevColor(GX_TEVREG0, l_noUsedColor);
    drawQuad(field_0x8, field_0xc, field_0x10, field_0x14);

    f32 var_f29 = field_0x10 - field_0x8;
    f32 sp4C = field_0x14 - field_0xc;
    f32 sp48 = var_f29 * sp4C;
    
    uintptr_t start_addr = (uintptr_t)m_heap->getStartAddr();
    uintptr_t end_addr = (uintptr_t)m_heap->getEndAddr();
    u32 sp40 = end_addr - start_addr;
    f32 sp3C = sp48 / (f32)sp40;

    for (JKRExpHeap::CMemBlock* block = m_heap->getUsedFirst(); block != NULL; block = block->getNextBlock()) {
        JUT_ASSERT(162, block->isValid());

        GXSetTevColor(GX_TEVREG0, block->isTempMemBlock() ? l_tempColor : l_usedColor);

        u32 sp38 = (m_heap->getSize(block + 1) + block->getAlignment()) + 0x10;
        f32 var_f30 = (f32)sp38 * sp3C;
        uintptr_t sp34 = (uintptr_t)block - start_addr;
        f32 sp30 = (f32)sp34 * sp3C;
        
        f32 var_f28 = std::floor(sp30 / var_f29);

        f32 sp2C = sp30 - (var_f29 * var_f28);
        f32 sp28 = field_0x8 + sp2C;

        f32 var_f31 = field_0xc + var_f28;
        f32 var_f27 = 1.0f + var_f31;
        f32 sp24 = var_f29 - sp2C;

        if (var_f30 <= sp24) {
            drawQuad(sp28, var_f31, sp28 + var_f30, var_f27);
        } else {
            if (sp24 > 0.0f) {
                drawQuad(sp28, var_f31, field_0x10, var_f27);
                var_f30 -= sp24;
                var_f31 = var_f27;
            }

            var_f28 = std::floor(var_f30 / var_f29);
            if (var_f28 > 0.0f) {
                var_f27 = var_f31 + var_f28;
                drawQuad(field_0x8, var_f31, field_0x10, var_f27);
                var_f30 -= var_f29 * var_f28;
                var_f31 = var_f27;
            }
            drawQuad(field_0x8, var_f31, field_0x8 + var_f30, 1.0f + var_f31);
        }
    }

    dComIfGp_getCurrentGrafPort()->setup2D();
}

void dDlst_heapMap_c::set(JKRExpHeap* i_heap, f32 param_1, f32 param_2, f32 param_3, f32 param_4) {
    m_heap = i_heap;
    field_0x8 = param_1;
    field_0xc = param_2;
    field_0x10 = param_3;
    field_0x14 = param_4;
}

static dDlst_heapMap_c l_heapMap;
static int l_heapMapMode;

static void drawHeapMap() {
    if (mDoCPd_c::getHoldL(PAD_3) && mDoCPd_c::getHoldR(PAD_3) && mDoCPd_c::getTrigY(PAD_3)) {
        l_heapMapMode = (l_heapMapMode + 1) % 7;
        if (l_heapMapMode != 0) {
            JKRExpHeap* heap = NULL;
            if (l_heapMapMode == 1) {
                heap = mDoExt_getArchiveHeap();
                OSReport_Error("アーカイブヒープマップ表示\n");
            } else if (l_heapMapMode == 3) {
                heap = mDoExt_getGameHeap();
                OSReport_Error("ゲームヒープマップ表示\n");
            } else if (l_heapMapMode == 2) {
                heap = (JKRExpHeap*)DynamicModuleControlBase::getHeap();
                OSReport_Error("ダイナミックリンクヒープマップ表示\n");
            } else if (l_heapMapMode == 4) {
                heap = mDoExt_getZeldaHeap();
                OSReport_Error("ゼルダヒープマップ表示\n");
            } else if (l_heapMapMode == 5) {
                heap = mDoExt_getJ2dHeap();
                OSReport_Error("Ｊ２Ｄヒープマップ表示\n");
            } else if (l_heapMapMode == 6) {
                heap = mDoExt_getCommandHeap();
                OSReport_Error("コマンドヒープマップ表示\n");
            }

            l_heapMap.set(heap, 300.0f, 300.0f, 600.0f, 390.0f);
        }
    }

    if (l_heapMapMode != 0) {
        dComIfGd_set2DXlu(&l_heapMap);
    }
}

#endif

static ResTIMG* createTimg(u16 width, u16 height, u32 format) {
    u32 bufferSize = GXGetTexBufferSize(width, height, format, GX_FALSE, 0) + 0x20;
    ResTIMG* timg;

    #if PLATFORM_GCN
    timg = (ResTIMG*)JKRAlloc(bufferSize, 0x20);
    #else
    timg = (ResTIMG*)JKRHeap::getRootHeap2()->alloc(bufferSize, 0x20);
    #endif

    if (timg == NULL) {
        return NULL;
    }

    cLib_memSet(timg, 0, bufferSize);
    timg->format = format;
    timg->alphaEnabled = false;
    timg->width = width;
    timg->height = height;
    timg->minFilter = GX_LINEAR;
    timg->magFilter = GX_LINEAR;
    timg->mipmapCount = 1;
    timg->imageOffset = 0x20;
    return timg;
}

JUTFader* mDoGph_gInf_c::mFader;

#if PLATFORM_WII || PLATFORM_SHIELD
ResTIMG* mDoGph_gInf_c::m_fullFrameBufferTimg;
void* mDoGph_gInf_c::m_fullFrameBufferTex;
#endif

ResTIMG* mDoGph_gInf_c::mFrameBufferTimg;

void* mDoGph_gInf_c::mFrameBufferTex;

ResTIMG* mDoGph_gInf_c::mZbufferTimg;

void* mDoGph_gInf_c::mZbufferTex;

f32 mDoGph_gInf_c::mFadeRate;

f32 mDoGph_gInf_c::mFadeSpeed;

GXColor mDoGph_gInf_c::mBackColor = {0, 0, 0, 0};

GXColor mDoGph_gInf_c::mFadeColor = {0, 0, 0, 0};

/* 80450BE4 0001+00 data_80450BE4 None */
/* 80450BE5 0001+00 data_80450BE5 None */
/* 80450BE6 0001+00 mFade__13mDoGph_gInf_c None */
/* 80450BE7 0001+00 data_80450BE7 None */
u8 mDoGph_gInf_c::mBlureFlag;

u8 mDoGph_gInf_c::mBlureRate;

u8 mDoGph_gInf_c::mFade;

bool mDoGph_gInf_c::mAutoForcus;

void mDoGph_gInf_c::create() {
    #if PLATFORM_WII || PLATFORM_SHIELD
    VISetTrapFilter(0);
    #endif

    #if PLATFORM_GCN
    JFWDisplay::createManager(JKRHeap::sCurrentHeap, JUTXfb::UNK_2, true);
    #else
    JFWDisplay::createManager(JKRHeap::getRootHeap2(), JUTXfb::UNK_2, true);
    #endif

    JFWDisplay::getManager()->setDrawDoneMethod(JFWDisplay::UNK_METHOD_1);

    JUTFader* faderPtr = new JUTFader(0, 0, JUTVideo::getManager()->getRenderMode()->fbWidth,
                                      JUTVideo::getManager()->getRenderMode()->efbHeight,
                                      JUtility::TColor(0, 0, 0, 0));
    JUT_ASSERT(352, faderPtr != NULL);
    setFader(faderPtr);
    JFWDisplay::getManager()->setFader(faderPtr);
    JUTProcBar::getManager()->setVisibleHeapBar(false);
    JUTProcBar::getManager()->setVisible(false);
    JUTDbPrint::getManager()->setVisible(false);

    #if PLATFORM_WII || PLATFORM_SHIELD
    m_fullFrameBufferTimg = createTimg(FB_WIDTH, FB_HEIGHT, 6);
    JUT_ASSERT(366, m_fullFrameBufferTimg != NULL);
    m_fullFrameBufferTex = (char*)m_fullFrameBufferTimg + sizeof(ResTIMG);
    #endif

    mFrameBufferTimg = createTimg(FB_WIDTH / 2, FB_HEIGHT / 2, 6);
    JUT_ASSERT(374, mFrameBufferTimg != NULL);
    mFrameBufferTex = (char*)mFrameBufferTimg + sizeof(ResTIMG);

    mZbufferTimg = createTimg(FB_WIDTH / 2, FB_HEIGHT / 2, 3);
    JUT_ASSERT(381, mZbufferTimg != NULL);
    mZbufferTex = (char*)mZbufferTimg + sizeof(ResTIMG);

    J2DPrint::setBuffer(0x400);
    mBlureFlag = false;
    mFade = 0;

    mBackColor = g_clearColor;
    mFadeColor = g_clearColor;

    #if PLATFORM_WII || PLATFORM_SHIELD
    if (SCGetAspectRatio() == 0) {
        offWide();
    } else {
        onWide();
    }
    #endif

    VISetBlack(TRUE);
}

static bool data_80450BE8;

void mDoGph_gInf_c::beginRender() {
    #if PLATFORM_WII || PLATFORM_SHIELD
    VISetTrapFilter(fapGmHIO_getTrapFilter() ? 1 : 0);
    VISetGamma(fapGmHIO_getGamma());
    #endif

    if (data_80450BE8) {
        JUTXfb::getManager()->setDrawingXfbIndex(-1);
    }

    JFWDisplay::getManager()->beginRender();

    #if PLATFORM_WII || PLATFORM_SHIELD
    VIEnableDimming(1);
    #endif
}

#if PLATFORM_WII || PLATFORM_SHIELD
void mDoGph_gInf_c::resetDimming() {
    VIEnableDimming(0);
}
#endif

void mDoGph_gInf_c::fadeOut(f32 fadeSpeed, GXColor& fadeColor) {
    mFade = 1;
    mFadeSpeed = fadeSpeed;
    mFadeColor = fadeColor;
    mFadeRate = fadeSpeed >= 0.0f ? 0.0f : 1.0f;
}

void mDoGph_gInf_c::fadeOut_f(f32 fadeSpeed, GXColor& fadeColor) {
    mFade = 129;
    mFadeSpeed = fadeSpeed;
    mFadeColor = fadeColor;
    mFadeRate = fadeSpeed >= 0.0f ? 0.0f : 1.0f;
}

void mDoGph_gInf_c::onBlure() {
    onBlure(cMtx_getIdentity());
}

#if PLATFORM_WII || PLATFORM_SHIELD
GXTexObj mDoGph_gInf_c::m_fullFrameBufferTexObj;
#endif

GXTexObj mDoGph_gInf_c::mFrameBufferTexObj;

GXTexObj mDoGph_gInf_c::mZbufferTexObj;

mDoGph_gInf_c::bloom_c mDoGph_gInf_c::m_bloom;

Mtx mDoGph_gInf_c::mBlureMtx;

void mDoGph_gInf_c::onBlure(const Mtx m) {
    mBlureFlag = true;
    setBlureMtx(m);
}

void mDoGph_gInf_c::fadeOut(f32 fadeSpeed) {
    fadeOut(fadeSpeed, g_clearColor);
}

void darwFilter(GXColor matColor) {
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetZCompLoc(GX_ENABLE);
    GXSetZMode(GX_DISABLE, GX_ALWAYS, GX_DISABLE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, g_clearColor);
    GXSetFogRangeAdj(GX_DISABLE, 0, NULL);
    GXSetCullMode(GX_CULL_NONE);
    GXSetDither(GX_ENABLE);
    GXSetNumIndStages(0);

    Mtx44 mtx;
    C_MTXOrtho(mtx, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 10.0f);
    GXSetProjection(mtx, GX_ORTHOGRAPHIC);
    GXLoadPosMtxImm(cMtx_getIdentity(), GX_PNMTX0);
    GXSetChanMatColor(GX_COLOR0A0, matColor);
    GXSetCurrentMtx(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGB8, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s8(0, 0, -5);
    GXPosition3s8(1, 0, -5);
    GXPosition3s8(1, 1, -5);
    GXPosition3s8(0, 1, -5);
    GXEnd();
}

void mDoGph_gInf_c::calcFade() {
    if (mDoGph_gInf_c::mFade != 0) {
        mFadeRate += mFadeSpeed;

        if (mFadeRate < 0.0f) {
            mFadeRate = 0.0f;
            mDoGph_gInf_c::mFade = 0;
        } else {
            if (mFadeRate > 1.0f) {
                mFadeRate = 1.0f;
            }
        }
        mFadeColor.a = 255.0f * mFadeRate;
    } else {
        if (dComIfG_getBrightness() != 255) {
            mFadeColor.r = 0;
            mFadeColor.g = 0;
            mFadeColor.b = 0;
            mFadeColor.a = 255 - dComIfG_getBrightness();
        } else {
            mFadeColor.a = 0;
        }
    }

    if (mFadeColor.a != 0) {
        darwFilter(mFadeColor);
    }
}

#if PLATFORM_WII || PLATFORM_SHIELD
u32 mDoGph_gInf_c::csr_c::m_blurID;

void mDoGph_gInf_c::csr_c::particleExecute() {
    dComIfGp_particle_levelExecute(m_blurID);
}
#endif

#if WIDESCREEN_SUPPORT
u8 mDoGph_gInf_c::mWideZoom;

int mDoGph_gInf_c::m_minX;

int mDoGph_gInf_c::m_minY;

f32 mDoGph_gInf_c::m_minXF;

f32 mDoGph_gInf_c::m_minYF;

#if PLATFORM_WII || PLATFORM_SHIELD
mDoGph_gInf_c::csr_c* mDoGph_gInf_c::m_baseCsr;

mDoGph_gInf_c::csr_c* mDoGph_gInf_c::m_csr;
#endif

#if PLATFORM_SHIELD
JKRHeap* mDoGph_gInf_c::m_heap;
#endif

u8 mDoGph_gInf_c::mWide = 1;

f32 mDoGph_gInf_c::m_aspect = 1.3571428f;

f32 mDoGph_gInf_c::m_scale = 1.0f;

f32 mDoGph_gInf_c::m_invScale = 1.0f;

int mDoGph_gInf_c::m_maxX = 608 - 1;

int mDoGph_gInf_c::m_maxY = 448 - 1;

int mDoGph_gInf_c::m_width = 608;

int mDoGph_gInf_c::m_height = 448;

f32 mDoGph_gInf_c::m_maxXF = 608.0f - 1;

f32 mDoGph_gInf_c::m_maxYF = 448.0f - 1;

f32 mDoGph_gInf_c::m_widthF = 608.0f;

f32 mDoGph_gInf_c::m_heightF = 448.0f;

struct tvSize {
    u16 width;
    u16 height;
};
const tvSize l_tvSize[2] = {
    {608, 448},
    {808, 448},
};

void mDoGph_gInf_c::setTvSize() {
    const tvSize* tvsize = &l_tvSize[mWide];

    m_width = tvsize->width;
    m_height = tvsize->height;
    m_minX = -((m_width - 608) / 2);
    m_minY = -((m_height - 448) / 2);
    m_maxX = m_minX + m_width;
    m_maxY = m_minY + m_height;

    m_widthF = m_width;
    m_heightF = m_height;
    m_minXF = m_minX;
    m_minYF = m_minY;
    m_maxXF = m_maxX;
    m_maxYF = m_maxY;

    m_aspect = m_widthF / m_heightF;
    m_scale = m_aspect / 1.3571428f;
    m_invScale = 1.0f / m_scale;
}

void mDoGph_gInf_c::onWide() {
    mWide = TRUE;
    setTvSize();
    dMeter2Info_onWide2D();
}

void mDoGph_gInf_c::offWide() {
    mWide = FALSE;
    setTvSize();
    dMeter2Info_offWide2D();
}

void mDoGph_gInf_c::onWideZoom() {
    mWideZoom = TRUE;
}

void mDoGph_gInf_c::offWideZoom() {
    mWideZoom = FALSE;
}

BOOL mDoGph_gInf_c::isWideZoom() {
    return isWide() && mWideZoom;
}

u8 mDoGph_gInf_c::isWide() {
    return mWide == TRUE;
}

void mDoGph_gInf_c::setWideZoomProjection(Mtx44& m) {
    if (isWideZoom()) {
        f32 sp20 = m[0][0];
        f32 sp1C = m[0][2];
        f32 sp18 = m[1][1];
        f32 sp14 = m[1][2];
        f32 sp10 = m[2][2];
        f32 spC = m[2][3];
        
        f32 temp_f31 = spC / (sp10 - 1.0f);
        f32 sp8 = spC / sp10;

        f32 temp_f30 = ((temp_f31 * (1.0f + sp14)) / sp18);
        f32 temp_f29 = ((temp_f31 * (sp14 - 1.0f)) / sp18);
        f32 temp_f28 = ((temp_f31 * (sp1C - 1.0f)) / sp20);
        f32 temp_f27 = ((temp_f31 * (1.0f + sp1C)) / sp20);

        temp_f30 *= getInvScale();
        temp_f29 *= getInvScale();
        temp_f28 *= getInvScale();
        temp_f27 *= getInvScale();

        m[0][0] = (2.0f * temp_f31) / (temp_f27 - temp_f28);
        m[0][1] = 0.0f;
        m[0][2] = (temp_f27 + temp_f28) / (temp_f27 - temp_f28);
        m[0][3] = 0.0f;

        m[1][0] = 0.0f;
        m[1][1] = (2.0f * temp_f31) / (temp_f30 - temp_f29);
        m[1][2] = (temp_f30 + temp_f29) / (temp_f30 - temp_f29);
        m[1][3] = 0.0f;

        m[2][0] = 0.0f;
        m[2][1] = 0.0f;
        m[2][2] = -temp_f31 / (sp8 - temp_f31);
        m[2][3] = -(sp8 * temp_f31) / (sp8 - temp_f31);

        m[3][0] = 0.0f;
        m[3][1] = 0.0f;
        m[3][2] = -1.0f;
        m[3][3] = 0.0f;
    }
}

void mDoGph_gInf_c::setWideZoomLightProjection(Mtx& m) {
    if (isWideZoom()) {
        f32 temp_f27 = m[0][0];
        f32 temp_f26 = m[0][2];
        f32 temp_f25 = m[1][1];
        f32 temp_f24 = m[1][2];

        f32 temp_f31 = (1.0f + temp_f24) / temp_f25;
        f32 temp_f30 = (temp_f24 - 1.0f) / temp_f25;
        f32 temp_f29 = (temp_f26 - 1.0f) / temp_f27;
        f32 temp_f28 = (1.0f + temp_f26) / temp_f27;

        temp_f31 *= getInvScale();
        temp_f30 *= getInvScale();
        temp_f29 *= getInvScale();
        temp_f28 *= getInvScale();

        m[0][0] = 2.0f / (temp_f28 - temp_f29);
        m[0][1] = 0.0f;
        m[0][2] = (temp_f28 + temp_f29) / (temp_f28 - temp_f29);
        m[0][3] = 0.0f;

        m[1][0] = 0.0f;
        m[1][1] = 2.0f / (temp_f31 - temp_f30);
        m[1][2] = (temp_f31 + temp_f30) / (temp_f31 - temp_f30);
        m[1][3] = 0.0f;

        m[2][0] = 0.0f;
        m[2][1] = 0.0f;
        m[2][2] = -1.0f;
        m[2][3] = 0.0f;
    }
}
#endif

#if PLATFORM_WII || PLATFORM_SHIELD
void mDoGph_gInf_c::entryBaseCsr(mDoGph_gInf_c::csr_c* i_entry) {
    JUT_ASSERT(876, m_baseCsr == NULL);
    m_baseCsr = i_entry;
    m_csr = i_entry;
}
#endif

void mDoGph_BlankingON() {}

void mDoGph_BlankingOFF() {}

static void dScnPly_BeforeOfPaint() {
    dComIfGd_reset();

    #if DEBUG
    dDbVw_deleteDrawPacketList();
    #endif
}

int mDoGph_BeforeOfDraw() {
    dScnPly_BeforeOfPaint();
    return 1;
}

int mDoGph_AfterOfDraw() {
    if (fapGmHIO_isMenu()) {
        JUTProcBar::getManager()->setVisible(false);
        JUTProcBar::getManager()->setVisibleHeapBar(false);
        JUTDbPrint::getManager()->setVisible(true);
    } else {
        int sysConsole_visible = JFWSystem::getSystemConsole()->isVisible();
        s32 port3_connected = mDoCPd_c::isConnect(PAD_3);

        BOOL procBar_visible = port3_connected && fapGmHIO_getMeter() && !sysConsole_visible;
        BOOL console_visible = port3_connected && fapGmHIO_isPrint();

        // Dev mode check
        if (!mDoMain::developmentMode) {
            procBar_visible = false;
            console_visible = false;
        }

        JUTProcBar::getManager()->setVisible(procBar_visible);
        JUTProcBar::getManager()->setVisibleHeapBar(procBar_visible);
        JUTDbPrint::getManager()->setVisible(console_visible);
    }

    GXSetZCompLoc(GX_ENABLE);
    GXSetZMode(GX_DISABLE, GX_ALWAYS, GX_DISABLE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, g_clearColor);
    GXSetFogRangeAdj(GX_DISABLE, 0, NULL);
    GXSetCoPlanar(GX_DISABLE);
    GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z8, 0);
    GXSetDither(GX_ENABLE);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetCullMode(GX_CULL_NONE);

    #if WIDESCREEN_SUPPORT
    struct viwidth {
        u16 unk_0x0;
        u16 unk_0x2;
    };
    static const viwidth l_viWidth[2] = {
        {670, 666},
        {686, 682},
    };

    const viwidth* viWidth = &l_viWidth[0];
    if (mDoGph_gInf_c::isWide()) {
        viWidth++;
    }

    GXRenderModeObj* renderObj = mDoMch_render_c::getRenderModeObj();
    if (renderObj->viTVmode != VI_TVMODE_PAL_INT) {
        renderObj->viWidth = viWidth->unk_0x0;
        renderObj->viXOrigin = (720 - renderObj->viWidth) / 2;
    } else {
        renderObj->viWidth = viWidth->unk_0x2;
        renderObj->viXOrigin = (720 - renderObj->viWidth) / 2;
    }
    #endif

    JUTVideo::getManager()->setRenderMode(mDoMch_render_c::getRenderModeObj());
    mDoGph_gInf_c::endFrame();
    return 1;
}

#if PLATFORM_WII
void drawFilterQuad(s8 param_0, s8 param_1) {
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s8(0, 0, -5);
    GXTexCoord2s8(0, 0);
    GXPosition3s8(param_0, 0, -5);
    GXTexCoord2s8(1, 0);
    GXPosition3s8(param_0, param_1, -5);
    GXTexCoord2s8(1, 1);
    GXPosition3s8(0, param_1, -5);
    GXTexCoord2s8(0, 1);
    GXEnd();
}

// mapping to simplify call changes between wii / other platforms
#define mDoGph_drawFilterQuad drawFilterQuad
#endif

static void drawDepth2(view_class* param_0, view_port_class* param_1, int param_2) {
    static GXColorS10 l_tevColor0 = {0, 0, 0, 0};

    if (daPy_getLinkPlayerActorClass() != NULL) {
        u8 sp8 = 1;
        #if DEBUG
        if (g_envHIO.mOther.mDepthOfField) {
        #endif
            if (mDoGph_gInf_c::isAutoForcus()) {
                f32 sp4C[7];
                f32 sp34[6];
                f32 sp1C;
                f32 sp18;
                f32 sp14;
                GXGetProjectionv(sp4C);
                GXGetViewportv(sp34);
                GXProject(param_0->lookat.center.x, param_0->lookat.center.y,
                        param_0->lookat.center.z, param_0->viewMtx, sp4C, sp34, &sp1C, &sp18,
                        &sp14);
                
                param_2 = (0xFF0000 - (int)(16777215.0f * sp14)) >> 8;
                param_2 = cLib_minMaxLimit<int>(param_2, -0x400, 0);
            }

            fopAc_ac_c* player_p = dComIfGp_getPlayer(0);
            camera_class* camera_p = (camera_class*)dComIfGp_getCamera(0);
            f32 var_f31;
            f32 var_f29;
            f32 var_f28 = -255.0f;

            if (dCam_getBody()->Mode() != 4 && dCam_getBody()->Mode() != 7) {
                int cam_id = dComIfGp_getPlayerCameraID(0);
                camera_class* temp_r4 = dComIfGp_getCamera(cam_id);
                dAttention_c* attention = dComIfGp_getAttention();

                f32 var_f30;
                if (temp_r4 != NULL) {
                    var_f30 = fopCamM_GetFovy(temp_r4);
                } else {
                    var_f30 = 48.0f;
                }
                var_f30 = 60.0f / var_f30;

                if (attention->LockonTruth()) {
                    fopAc_ac_c* atn_actor =
                        fopAcM_SearchByID(daPy_getLinkPlayerActorClass()->getAtnActorID());

                    if (atn_actor != NULL) {
                        cXyz sp28;
                        sp28 = atn_actor->eyePos;
                        if (std::fabs(sp28.y - camera_p->lookat.eye.y) < 400.0f) {
                            sp28.y = camera_p->lookat.eye.y;
                        }

                        var_f29 = atn_actor->current.pos.abs(camera_p->lookat.eye);
                        var_f31 = var_f29 / ((SREG_F(2) + 280.0f) * var_f30);
                        var_f31 -= 0.8f;
                        if (var_f31 < 0.0f) {
                            var_f31 = 0.0f;
                        } else if (var_f31 > 1.0f) {
                            var_f31 = 1.0f;
                        }
                        var_f28 = -180.0f - 75.0f * var_f31;
                    }
                } else if (dComIfGp_event_runCheck() && var_f30 < 3.0f &&
                        g_env_light.field_0x126c < 999999.0f)
                {
                    var_f29 = g_env_light.field_0x126c;
                    var_f31 = var_f29 / ((SREG_F(2) + 80.0f) * var_f30);
                    var_f31 -= 0.8f;
                    if (var_f31 < 0.0f) {
                        var_f31 = 0.0f;
                    } else if (var_f31 > 1.0f) {
                        var_f31 = 1.0f;
                    }
                    var_f28 = -180.0f - 75.0f * var_f31;
                }
            }

            cLib_addCalc(&g_env_light.field_0x1264, var_f28, SREG_F(5) + 0.1f, SREG_F(4) + 100.0f, 0.0001f);
            l_tevColor0.a = g_env_light.field_0x1264;
            if (l_tevColor0.a <= -254) {
                l_tevColor0.a = -255;
            }

            s16 x_orig = (int)param_1->x_orig & ~7;
            s16 y_orig = (int)param_1->y_orig & ~7;
            s16 y_orig_pos = y_orig < 0 ? 0 : y_orig;

            s16 width = (int)param_1->width & ~7;
            s16 height = (int)param_1->height & ~7;

            void* zBufferTex = (void*)mDoGph_gInf_c::getZbufferTex();
            void* frameBufferTex = (void*)mDoGph_gInf_c::getFrameBufferTex();

            if (y_orig < 0) {
                height += y_orig;
                y_orig = -y_orig >> 1;
                zBufferTex =
                    (char*)zBufferTex + GXGetTexBufferSize(FB_WIDTH / 2, y_orig, GX_TF_IA8, GX_FALSE, 0);
                frameBufferTex =
                    (char*)frameBufferTex +
                    GXGetTexBufferSize(FB_WIDTH / 2, y_orig, mDoGph_gInf_c::getFrameBufferTimg()->format,
                                    GX_FALSE, 0);
            }

            u16 halfWidth = width >> 1;
            u16 halfHeight = height >> 1;
            GXRenderModeObj* sp24 = JUTGetVideoManager()->getRenderMode();
            GXSetCopyFilter(GX_FALSE, NULL, GX_TRUE, sp24->vfilter);
            GXSetTexCopySrc(x_orig, y_orig_pos, width, height);
            GXSetTexCopyDst(halfWidth, halfHeight, GX_TF_Z16, GX_TRUE);
            GXCopyTex(zBufferTex, GX_FALSE);
            GXSetTexCopySrc(x_orig, y_orig_pos, width, height);
            GXSetTexCopyDst(halfWidth, halfHeight,
                            (GXTexFmt)mDoGph_gInf_c::getFrameBufferTimg()->format, GX_TRUE);
            GXCopyTex(frameBufferTex, GX_FALSE);
            GXInitTexObj(mDoGph_gInf_c::getZbufferTexObj(), zBufferTex, halfWidth, halfHeight,
                        GX_TF_IA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
            GXInitTexObjLOD(mDoGph_gInf_c::getZbufferTexObj(), GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f,
                            GX_FALSE, GX_FALSE, GX_ANISO_1);
            GXInitTexObj(mDoGph_gInf_c::getFrameBufferTexObj(), frameBufferTex, halfWidth, halfHeight,
                        (GXTexFmt)mDoGph_gInf_c::getFrameBufferTimg()->format, GX_CLAMP, GX_CLAMP,
                        GX_FALSE);
            GXInitTexObjLOD(mDoGph_gInf_c::getFrameBufferTexObj(), GX_LINEAR, GX_LINEAR, 0.0f, 0.0f,
                            0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
            GXPixModeSync();
            GXInvalidateTexAll();
            GXLoadTexObj(mDoGph_gInf_c::getFrameBufferTexObj(), GX_TEXMAP1);
            GXLoadTexObj(mDoGph_gInf_c::getZbufferTexObj(), GX_TEXMAP0);

            if (0.0f != g_env_light.mDemoAttentionPoint) {
                if (g_env_light.mDemoAttentionPoint >= 0.0f) {
                    l_tevColor0.a = -254.0f + 509.0f * g_env_light.mDemoAttentionPoint;
                } else {
                    l_tevColor0.a = -254.0f + 509.0f * (1.0f + g_env_light.mDemoAttentionPoint);
                }
            }

            #if DEBUG
            if (g_kankyoHIO.navy.demo_adjust_SW) {
                l_tevColor0.a = g_kankyoHIO.navy.demo_focus_pos;
            }
            #endif

            GXSetTevColorS10(GX_TEVREG0, l_tevColor0);
            GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_ALPHA, GX_CH_GREEN, GX_CH_BLUE, GX_CH_RED);
            GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP3);
            GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_KONST, GX_CA_TEXA, GX_CA_KONST, GX_CA_ZERO);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_A8_EQ, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);

            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_A0);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_4, GX_TRUE, GX_TEVPREV);

            GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
            GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
            GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

            GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD2, GX_TEXMAP1, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_CPREV, GX_CC_TEXC, GX_CC_HALF, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
            GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

            GXSetZCompLoc(GX_TRUE);
            GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
            if (g_env_light.mDemoAttentionPoint >= 0.0f) {
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
                GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
            } else {
                GXSetBlendMode(GX_BM_BLEND, GX_BL_INVSRCALPHA, GX_BL_SRCALPHA, GX_LO_CLEAR);
                GXSetAlphaCompare(GX_LESS, 0xff, GX_AOP_OR, GX_LESS, 0xff);
            }

            GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, g_clearColor);
            GXSetCullMode(GX_CULL_NONE);
            GXSetDither(GX_TRUE);
            GXSetNumIndStages(0);
            Mtx44 ortho;
            C_MTXOrtho(ortho, param_1->y_orig, param_1->y_orig + param_1->height, param_1->x_orig,
                    param_1->x_orig + param_1->width, 0.0f, 10.0f);
            GXLoadPosMtxImm(cMtx_getIdentity(), 0);

            #if DEBUG
            mDoMtx_stack_c::transS(g_kankyoHIO.navy.demo_focus_offset_x, g_kankyoHIO.navy.demo_focus_offset_y, 0.0f);
            #else
            mDoMtx_stack_c::transS(0.0025f, 0.0025f, 0.0f);
            #endif
            GXLoadTexMtxImm(mDoMtx_stack_c::get(), 0x1e, GX_MTX2x4);

            #if DEBUG
            mDoMtx_stack_c::transS(-g_kankyoHIO.navy.demo_focus_offset_x, -g_kankyoHIO.navy.demo_focus_offset_y, 0.0f);
            #else
            mDoMtx_stack_c::transS(-0.0025f, -0.0025f, 0.0f);
            #endif
            GXLoadTexMtxImm(mDoMtx_stack_c::get(), 0x21, GX_MTX2x4);

            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_S8, 0);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c);
            GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e);
            GXSetTexCoordGen(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, 0x21);
            GXSetNumChans(0);
            GXSetNumTexGens(3);
            GXSetNumTevStages(4);
            GXSetProjection(ortho, GX_ORTHOGRAPHIC);
            GXSetCurrentMtx(0);

            if (l_tevColor0.a > -255 && sp8 == 1) {
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                GXPosition3s16(x_orig, y_orig_pos, -5);
                GXTexCoord2s8(0, 0);
                GXPosition3s16(width, y_orig_pos, -5);
                GXTexCoord2s8(1, 0);
                GXPosition3s16(width, height, -5);
                GXTexCoord2s8(1, 1);
                GXPosition3s16(x_orig, height, -5);
                GXTexCoord2s8(0, 1);
                GXEnd();
            }

            GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
            GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
            GXSetProjection(param_0->projMtx, GX_PERSPECTIVE);
        #if DEBUG
        }
        #endif
    }
}

static void trimming(view_class* param_0, view_port_class* param_1) {
    (void)param_0;

    s16 y_orig = (int)param_1->y_orig & ~7;
    s16 y_orig_pos = y_orig < 0 ? 0 : y_orig;
    if ((y_orig_pos == 0) && (param_1->scissor.y_orig != param_1->y_orig ||
                              (param_1->scissor.height != param_1->height)))
    {
        s32 sc_top = (int)param_1->scissor.y_orig;
        s32 sc_bottom = param_1->scissor.y_orig + param_1->scissor.height;
        GXSetNumChans(1);
        GXSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetZCompLoc(1);
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
        GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, g_clearColor);
        GXSetCullMode(GX_CULL_NONE);
        GXSetDither(GX_TRUE);
        GXSetNumIndStages(0);
        Mtx44 ortho;
        C_MTXOrtho(ortho, 0.0f, FB_HEIGHT, 0.0f, FB_WIDTH, 0.0f, 10.0f);
        GXLoadPosMtxImm(cMtx_getIdentity(), 0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
        GXSetProjection(ortho, GX_ORTHOGRAPHIC);
        GXSetCurrentMtx(0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 8);
        GXPosition3s16(0, 0, -5);
        GXPosition3s16(FB_WIDTH, 0, -5);
        GXPosition3s16(FB_WIDTH, sc_top, -5);
        GXPosition3s16(0, sc_top, -5);
        GXPosition3s16(0, sc_bottom, -5);
        GXPosition3s16(FB_WIDTH, sc_bottom, -5);
        GXPosition3s16(FB_WIDTH, FB_HEIGHT, -5);
        GXPosition3s16(0, FB_HEIGHT, -5);
        GXEnd();
    }
    GXSetScissor(param_1->scissor.x_orig, param_1->scissor.y_orig, param_1->scissor.width,
                 param_1->scissor.height);
}

#if !PLATFORM_WII
void mDoGph_drawFilterQuad(s8 param_0, s8 param_1) {
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2s8(0, 0);
    GXTexCoord2s8(0, 0);
    GXPosition2s8(param_0, 0);
    GXTexCoord2s8(1, 0);
    GXPosition2s8(param_0, param_1);
    GXTexCoord2s8(1, 1);
    GXPosition2s8(0, param_1);
    GXTexCoord2s8(0, 1);
    GXEnd();
}
#endif

void mDoGph_gInf_c::bloom_c::create() {
    if (m_buffer == NULL) {
        u32 size = GXGetTexBufferSize(FB_WIDTH / 2, FB_HEIGHT / 2, 6, GX_FALSE, 0);
        m_buffer = mDoExt_getArchiveHeap()->alloc(size, -32);
        JUT_ASSERT(1621, m_buffer != NULL);

        mEnable = false;
        mMode = 0;
        mPoint = 128;
        mBlureSize = 64;
        mBlureRatio = 128;
        mBlendColor = (GXColor){255, 255, 255, 255};
    }
}

void mDoGph_gInf_c::bloom_c::remove() {
    if (m_buffer != NULL) {
        mDoExt_getArchiveHeap()->free(m_buffer);
        m_buffer = NULL;
    }
    mMonoColor.a = 0;
}

void mDoGph_gInf_c::bloom_c::draw() {
    bool enabled = mEnable && m_buffer != NULL;
    if (mMonoColor.a != 0 || enabled) {
        GXSetViewport(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, 0.0f, 1.0f);
        GXSetScissor(0, 0, FB_WIDTH, FB_HEIGHT);
        GXLoadTexObj(getFrameBufferTexObj(), GX_TEXMAP0);
        GXSetNumChans(0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c);
        GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_GREEN);
        GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
        GXSetZCompLoc(1);
        GXSetZMode(0, GX_ALWAYS, 0);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, g_clearColor);
        GXSetFogRangeAdj(0, 0, 0);
        GXSetCullMode(GX_CULL_NONE);
        GXSetDither(1);
        Mtx44 ortho;
        C_MTXOrtho(ortho, 0.0f, 4.0f, 0.0f, 4.0f, 0.0f, 10.0f);
        GXLoadPosMtxImm(cMtx_getIdentity(), 0);
        GXSetProjection(ortho, GX_ORTHOGRAPHIC);
        GXSetCurrentMtx(0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        #if PLATFORM_WII
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGB8, 0);
        #else
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGB, GX_RGB8, 0);
        #endif
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGB8, 0);
        if (mMonoColor.a != 0) {
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C2, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A2);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP1, GX_TEV_SWAP1);
            GXSetTevColor(GX_TEVREG2, mMonoColor);
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
            mDoGph_drawFilterQuad(4, 4);
        }
        if (enabled) {
            GXSetTexCopySrc(0, 0, FB_WIDTH / 2, FB_HEIGHT / 2);
            GXSetTexCopyDst(FB_WIDTH / 2, FB_HEIGHT / 2, GX_TF_RGBA8, 0);
            GXCopyTex(m_buffer, 0);
            GXSetNumTevStages(3);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_TEXA, GX_CC_HALF, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP1, GX_TEV_SWAP1);
            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_TEXC, GX_CC_CPREV, GX_CC_HALF, GX_CC_C0);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP3, GX_TEV_SWAP3);
            GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_TEXC, GX_CC_CPREV, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
            GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_OR);
            GXColorS10 tevColor0 = {-mPoint, -mPoint, -mPoint, 0x40};
            GXSetTevColorS10(GX_TEVREG0, tevColor0);
            GXColor tevColor1 = {mBlureRatio, mBlureRatio, mBlureRatio, mBlureRatio};
            GXSetTevColor(GX_TEVREG1, tevColor1);
            GXPixModeSync();
            mDoGph_drawFilterQuad(2, 2);
            GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
            GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
            GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
            void* zBufferTex = getZbufferTex();
            GXSetTexCopySrc(0, 0, FB_WIDTH / 2, FB_HEIGHT / 2);
            GXSetTexCopyDst(FB_WIDTH / 4, FB_HEIGHT / 4, GX_TF_RGBA8, GX_TRUE);
            GXCopyTex(zBufferTex, 0);
            GXTexObj auStack_c0;
            GXInitTexObj(&auStack_c0, zBufferTex, FB_WIDTH / 4, FB_HEIGHT / 4, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP,
                         GX_FALSE);
            GXInitTexObjLOD(&auStack_c0, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE,
                            GX_ANISO_1);
            GXLoadTexObj(&auStack_c0, GX_TEXMAP0);
            GXSetNumTexGens(8);
            u32 iVar11 = 0x1e;
            int sVar10 = 0;
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c);
            for (int texCoord = (int)GX_TEXCOORD1; texCoord < (int)GX_MAX_TEXCOORD; texCoord++) {
                GXSetTexCoordGen((GXTexCoordID)texCoord, GX_TG_MTX2x4, GX_TG_TEX0, iVar11);
                f32 dVar15 = mBlureSize * (1.0f / 6400.0f);
                mDoMtx_stack_c::transS((dVar15 * cM_scos(sVar10)) * getInvScale(),
                                       dVar15 * cM_ssin(sVar10), 0.0f);
                GXLoadTexMtxImm(mDoMtx_stack_c::get(), iVar11, GX_MTX2x4);

                iVar11 += 3;
                sVar10 += 0x2492;
            }
            GXSetNumTevStages(8);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_A1, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            for (int tevStage = (int)GX_TEVSTAGE1; tevStage < 8; tevStage++) {
                GXSetTevOrder((GXTevStageID)tevStage, (GXTexCoordID)tevStage, GX_TEXMAP0,
                              GX_COLOR_NULL);
                GXSetTevColorIn((GXTevStageID)tevStage, GX_CC_ZERO, GX_CC_TEXC, GX_CC_A1,
                                GX_CC_CPREV);
                GXSetTevColorOp((GXTevStageID)tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                                GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaIn((GXTevStageID)tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
                                GX_CA_A0);
                GXSetTevAlphaOp((GXTevStageID)tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                                GX_TRUE, GX_TEVPREV);
            }
            GXPixModeSync();
            mDoGph_drawFilterQuad(1, 1);
            GXSetTexCopySrc(0, 0, FB_WIDTH / 4, FB_HEIGHT / 4);
            GXSetTexCopyDst(FB_WIDTH / 8, FB_HEIGHT / 8, GX_TF_RGBA8, GX_TRUE);
            GXCopyTex(zBufferTex, GX_FALSE);
            GXTexObj auStack_e0;
            GXInitTexObj(&auStack_e0, zBufferTex, FB_WIDTH / 8, FB_HEIGHT / 8, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP,
                         GX_FALSE);
            GXInitTexObjLOD(&auStack_c0, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE,
                            GX_ANISO_1);
            GXLoadTexObj(&auStack_e0, GX_TEXMAP0);
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
            GXPixModeSync();
            GXInvalidateTexAll();
            mDoGph_drawFilterQuad(1, 1);
            GXSetTexCopySrc(0, 0, FB_WIDTH / 4, FB_HEIGHT / 4);
            GXSetTexCopyDst(FB_WIDTH / 4, FB_HEIGHT / 4, GX_TF_RGBA8, GX_FALSE);
            GXCopyTex(zBufferTex, GX_FALSE);
            GXInitTexObj(&auStack_e0, m_buffer, FB_WIDTH / 2, FB_HEIGHT / 2, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP,
                         GX_FALSE);
            GXInitTexObjLOD(&auStack_e0, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE,
                            GX_ANISO_1);
            GXLoadTexObj(&auStack_e0, GX_TEXMAP0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ONE, GX_LO_OR);
            mDoGph_drawFilterQuad(2, 2);
            GXLoadTexObj(&auStack_c0, GX_TEXMAP0);
            GXSetTevColor(GX_TEVREG0, mBlendColor);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXSetBlendMode(GX_BM_BLEND, mMode == 1 ? GX_BL_INVDSTCLR : GX_BL_ONE, GX_BL_SRCALPHA, GX_LO_OR);
            GXPixModeSync();
            GXInvalidateTexAll();
            mDoGph_drawFilterQuad(4, 4);
        }
    }
}

static void retry_captue_frame(view_class* param_0, view_port_class* param_1, int param_2) {
    (void)param_0;
    (void)param_2;

    s16 x_orig = (int)param_1->x_orig & 0xFFFFFFF8;
    s16 y_orig = (int)param_1->y_orig & 0xFFFFFFF8;
    s16 y_orig_pos = y_orig < 0 ? 0 : y_orig;
    s16 width = (int)param_1->width & 0xFFFFFFF8;
    s16 height = (int)param_1->height & 0xFFFFFFF8;
    void* tex = (void*)mDoGph_gInf_c::getFrameBufferTex();
    u16 var_r24;
    u16 var_r23;

    if (!dComIfGp_isPauseFlag()) {
        if (y_orig < 0) {
            height += y_orig;
            y_orig = -y_orig >> 1;
            tex = (char*)tex + GXGetTexBufferSize(FB_WIDTH / 2, y_orig,
                                                  mDoGph_gInf_c::getFrameBufferTimg()->format,
                                                  GX_FALSE, 0);
        }

        var_r24 = width >> 1;
        var_r23 = height >> 1;
        GXSetTexCopySrc(x_orig, y_orig_pos, width, height);
        GXSetTexCopyDst(var_r24, var_r23, (GXTexFmt)mDoGph_gInf_c::getFrameBufferTimg()->format, GX_TRUE);
        GXCopyTex(tex, GX_FALSE);
        GXPixModeSync();
        GXInvalidateTexAll();
    }
}

static void motionBlure(view_class* param_0) {
    if (g_env_light.is_blure) {
        GXLoadTexObj(mDoGph_gInf_c::getFrameBufferTexObj(), GX_TEXMAP0);
        GXColor local_60;
        local_60.a = mDoGph_gInf_c::getBlureRate();
        GXSetNumChans(0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e);
        GXSetNumTevStages(1);
        GXSetTevColor(GX_TEVREG0, local_60);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetZCompLoc(1);
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, g_clearColor);
        GXSetCullMode(GX_CULL_NONE);
        GXSetDither(GX_TRUE);
        Mtx44 ortho;
        C_MTXOrtho(ortho, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 10.0f);
        GXLoadPosMtxImm(cMtx_getIdentity(), 0);
        GXLoadTexMtxImm(mDoGph_gInf_c::getBlureMtx(), 0x1e, GX_MTX2x4);
        GXSetProjection(ortho, GX_ORTHOGRAPHIC);
        GXSetCurrentMtx(0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        #if PLATFORM_WII
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGB8, 0);
        #else
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGB, GX_RGB8, 0);
        #endif
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGB8, 0);
        mDoGph_drawFilterQuad(1, 1);
        GXSetProjection(param_0->projMtx, GX_PERSPECTIVE);
    }
    if (mDoGph_gInf_c::isBlure()) {
        g_env_light.is_blure = 1;
    } else {
        g_env_light.is_blure = 0;
    }
}

static void setLight() {
    GXLightObj obj;

    GXInitLightPos(&obj, -35000.0f, 0.0f, -30000.0f);
    GXInitLightDir(&obj, 0.0f, 0.0f, 0.0f);
    GXInitLightColor(&obj, g_whiteColor);
    GXInitLightDistAttn(&obj, 0.0f, 0.0f, GX_DA_GENTLE);
    GXInitLightSpot(&obj, 0.0f, GX_SP_FLAT);
    GXLoadLightObjImm(&obj, GX_LIGHT0);
}

static void captureScreenSetProjection(Mtx44& m) {
    // DEBUG NONMATCHING
}

static void captureScreenSetPort() {
    Mtx44 m;
    captureScreenSetProjection(m);
}

static void captureScreenSetScissor(scissor_class* scissor) {
    // DEBUG NONMATCHING
}

static void captureScreenPerspDrawInfo(JPADrawInfo&) {
    // DEBUG NONMATCHING
}

static void drawItem3D() {
    Mtx item_mtx;
    dMenu_Collect3D_c::setupItem3D(item_mtx);

    #if DEBUG
    captureScreenSetPort();
    #endif

    setLight();
    j3dSys.setViewMtx(item_mtx);
    GXSetClipMode(GX_CLIP_DISABLE);
    dComIfGd_drawListItem3d();
    GXSetClipMode(GX_CLIP_ENABLE);
    j3dSys.reinitGX();
}

int mDoGph_Painter() {
    #if DEBUG
    drawHeapMap();
    #endif

    dComIfGp_particle_calcMenu();

    JFWDisplay::getManager()->setFader(mDoGph_gInf_c::getFader());
    mDoGph_gInf_c::setClearColor(mDoGph_gInf_c::getBackColor());
    mDoGph_gInf_c::beginRender();

    #if DEBUG
    fapGm_HIO_c::startCpuTimer();
    #endif

    GXSetAlphaUpdate(GX_DISABLE);
    mDoGph_gInf_c::setBackColor(g_clearColor);

    j3dSys.drawInit();
    GXSetDither(GX_ENABLE);

    J2DOrthoGraph ortho(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, -1.0f, 1.0f);
    ortho.setOrtho(mDoGph_gInf_c::getMinXF(), mDoGph_gInf_c::getMinYF(),
                   mDoGph_gInf_c::getWidthF(), mDoGph_gInf_c::getHeightF(),
                   -1.0f, 1.0f);
    ortho.setPort();

    #if DEBUG
    captureScreenSetPort();
    #endif

    dComIfGp_setCurrentGrafPort(&ortho);
    dComIfGd_drawCopy2D();

    #if DEBUG
    // "↓↓↓↓↓↓↓↓↓↓ CPU time measuring start ↓↓↓↓↓↓↓↓↓↓"
    fapGm_HIO_c::printCpuTimer("\n↓↓↓↓↓↓↓↓↓↓　ＣＰＵ時間計測開始　↓↓↓↓↓↓↓↓↓↓\n");

    // "drawing up to 2D drawing for screen capture (Rendering)"
    fapGm_HIO_c::stopCpuTimer("画面キャプチャー用２Ｄ描画まで（レンダリング）");
    #endif

    if (dComIfGp_getWindowNum() != 0) {
        dDlst_window_c* window_p = dComIfGp_getWindow(0);
        int camera_id = window_p->getCameraID();
        camera_class* camera_p = dComIfGp_getCamera(camera_id);

        if (camera_p != NULL) {
            #if DEBUG
            fapGm_HIO_c::startCpuTimer();
            #endif

            dComIfGd_imageDrawShadow(camera_p->viewMtx);

            #if DEBUG
            // "drawing Shadow Texture (Rendering)"
            fapGm_HIO_c::stopCpuTimer("影テクスチャー描画（レンダリング）");

            fapGm_HIO_c::startCpuTimer();
            #endif

            view_port_class* view_port = window_p->getViewPort();

            if (view_port->x_orig != 0.0f || view_port->y_orig != 0.0f) {
                view_port_class new_port;
                new_port.x_orig = 0.0f;
                new_port.y_orig = 0.0f;
                new_port.width = FB_WIDTH;
                new_port.height = FB_HEIGHT;
                new_port.near_z = view_port->near_z;
                new_port.far_z = view_port->far_z;
                new_port.scissor = view_port->scissor;

                view_port = &new_port;
            }

            #if DEBUG
            captureScreenSetScissor(&view_port->scissor);
            #endif

            GXSetViewport(view_port->x_orig, view_port->y_orig, view_port->width,
                          view_port->height, view_port->near_z, view_port->far_z);
            GXSetScissor(view_port->x_orig, view_port->y_orig, view_port->width,
                         view_port->height);

            JPADrawInfo draw_info(camera_p->viewMtx, camera_p->fovy, camera_p->aspect);

            #if WIDESCREEN_SUPPORT
            if (mDoGph_gInf_c::isWideZoom()) {
                Mtx44 sp140;
                draw_info.getPrjMtx(sp140);

                sp140[0][0] *= 2.0f;
                sp140[0][2] = 0.0f;
                sp140[1][1] *= -2.0f;
                sp140[1][2] = 0.0f;
                sp140[2][2] = -2.0f;
                mDoGph_gInf_c::setWideZoomProjection(sp140);

                sp140[0][0] *= 0.5f;
                sp140[0][2] = (0.5f * sp140[0][2]) - 0.5f;
                sp140[1][1] *= -0.5f;
                sp140[1][2] = (-0.5f * sp140[1][2]) - 0.5f;
                sp140[2][2] = 0.0f;
                draw_info.setPrjMtx(sp140);
            }
            #endif

            #if DEBUG
            captureScreenPerspDrawInfo(draw_info);
            #endif

            dComIfGp_setCurrentWindow(window_p);
            dComIfGp_setCurrentView(camera_p);
            dComIfGp_setCurrentViewport(view_port);
            GXSetProjection(camera_p->projMtx, GX_PERSPECTIVE);
            
            #if DEBUG
            captureScreenSetProjection(camera_p->projMtx);
            #endif

            PPCSync();

            j3dSys.setViewMtx(camera_p->viewMtx);
            dKy_setLight();
            dComIfGd_drawOpaListSky();
            dComIfGd_drawXluListSky();

            GXSetClipMode(GX_CLIP_ENABLE);

            #if DEBUG
            // "drawing up to Background (Translucent) (Rendering)"
            fapGm_HIO_c::stopCpuTimer("背景（半透明）描画まで（レンダリング）");

            fapGm_HIO_c::startCpuTimer();
            #endif

            dComIfGd_drawOpaListBG();
            dComIfGd_drawOpaListDarkBG();
            dComIfGd_drawOpaListMiddle();

            #if DEBUG
            if (fapGmHIO_getParticle())
            #endif
            {
                dComIfGp_particle_drawFogPri0_B(&draw_info);
            }

            #if DEBUG
            if (fapGmHIO_getParticle())
            #endif
            {
                dComIfGp_particle_drawNormalPri0_B(&draw_info);
            }

            #if DEBUG
            // "drawing up to Terrain (Opaque)"
            fapGm_HIO_c::stopCpuTimer("地形（不透明）描画２まで（レンダリング）");

            fapGm_HIO_c::startCpuTimer();
            #endif

            dComIfGd_drawShadow(camera_p->viewMtx);

            #if DEBUG
            // "shadow drawing (Rendering)"
            fapGm_HIO_c::stopCpuTimer("影描画（レンダリング）");

            fapGm_HIO_c::startCpuTimer();
            #endif

            dComIfGd_drawOpaList();

            #if DEBUG
            if (g_kankyoHIO.navy.field_0x30d) {
                if (dKy_darkworld_check() != TRUE) {
                    dComIfGd_drawOpaListDark();
                }
            } else
            #endif
            {
                dComIfGd_drawOpaListDark();
            }

            dComIfGd_drawOpaListPacket();
            
            #if DEBUG
            // "drawing up to special-use drawing (Opaque) except J3D (Rendering)"
            fapGm_HIO_c::stopCpuTimer("Ｊ３Ｄ以外などの特殊用（不透明）描画まで（レンダリング）");

            fapGm_HIO_c::startCpuTimer();
            #endif

            dComIfGd_drawXluListBG();
            dComIfGd_drawXluListDarkBG();

            #if DEBUG
            if (fapGmHIO_getParticle())
            #endif
            {
                dComIfGp_particle_drawFogPri0_A(&draw_info);
                dComIfGp_particle_drawNormalPri0_A(&draw_info);
            }

            #if DEBUG
            // "drawing up to Terrain (Translucent)"
            fapGm_HIO_c::stopCpuTimer("地形（半透明）描画２まで（レンダリング）");

            fapGm_HIO_c::startCpuTimer();
            #endif

            dComIfGd_drawXluList();

            #if DEBUG
            if (g_kankyoHIO.navy.field_0x30d) {
                if (dKy_darkworld_check() != TRUE) {
                    dComIfGd_drawXluListDark();
                }
            } else
            #endif
            {
                dComIfGd_drawXluListDark();
            }

            #if DEBUG
            // "drawing up to Object (Translucent)"
            fapGm_HIO_c::stopCpuTimer("オブジェクト（半透明）描画２まで（レンダリング）");
            #endif

            j3dSys.reinitGX();
            GXSetClipMode(GX_CLIP_ENABLE);

            if (!dComIfGp_isPauseFlag()) {
                #if DEBUG
                fapGm_HIO_c::startCpuTimer();
                #endif

                motionBlure(camera_p);

                #if DEBUG
                // "blur filter (Rendering)"
                fapGm_HIO_c::stopCpuTimer("ブラーフィルター（レンダリング）");

                fapGm_HIO_c::startCpuTimer();
                #endif

                drawDepth2(camera_p, view_port, dComIfGp_getCameraZoomForcus(camera_id));
                GXInvalidateTexAll();
                GXSetClipMode(GX_CLIP_ENABLE);

                #if DEBUG
                // "depth of field (Rendering)"
                fapGm_HIO_c::stopCpuTimer("被写界深度フィルター（レンダリング）");

                fapGm_HIO_c::startCpuTimer();
                #endif

                #if DEBUG
                if (g_kankyoHIO.navy.field_0x30d == 0 || dKy_darkworld_check() != TRUE)
                #endif
                {
                    if (g_env_light.is_blure == 0) {
                        dComIfGd_drawOpaListInvisible();
                        dComIfGd_drawXluListInvisible();
                    }
                }
                

                #if DEBUG
                // "drawing up to projection (Translucent)"
                fapGm_HIO_c::stopCpuTimer("投影用（半透明）描画まで（レンダリング）");

                fapGm_HIO_c::startCpuTimer();
                #endif

                #if DEBUG
                if (fapGmHIO_getParticle())
                #endif
                {
                    dComIfGp_particle_drawFogPri4(&draw_info);
                    dComIfGp_particle_drawProjection(&draw_info);
                }

                #if DEBUG
                // "drawing up to projection particle (Rendering)"
                fapGm_HIO_c::stopCpuTimer("投影パーティクル描画まで（レンダリング）");

                fapGm_HIO_c::startCpuTimer();
                #endif

                dComIfGd_drawListZxlu();

                #if DEBUG
                // "drawing up to 2-draw Z-update translucent (Rendering)"
                fapGm_HIO_c::stopCpuTimer("２度描きＺ更新半透明描画まで（レンダリング）");

                fapGm_HIO_c::startCpuTimer();
                #endif

                GXSetClipMode(GX_CLIP_ENABLE);

                #if DEBUG
                if (g_kankyoHIO.navy.field_0x30d) {
                    if (dKy_darkworld_check() != TRUE) {
                        dComIfGd_drawOpaListFilter();
                    }
                } else
                #endif
                {
                    dComIfGd_drawOpaListFilter();
                }

                #if DEBUG
                // "drawing up to filter draw (Rendering)"
                fapGm_HIO_c::stopCpuTimer("フィルター用描画まで（レンダリング）");

                fapGm_HIO_c::startCpuTimer();
                #endif

                GXSetClipMode(GX_CLIP_ENABLE);

                #if DEBUG
                if (fapGmHIO_getParticle())
                #endif
                {
                    dComIfGp_particle_drawFogPri1(&draw_info);
                    dComIfGp_particle_draw(&draw_info);
                    dComIfGp_particle_drawFogPri2(&draw_info);
                    dComIfGp_particle_drawFog(&draw_info);
                    dComIfGp_particle_drawFogPri3(&draw_info);
                    dComIfGp_particle_drawP1(&draw_info);
                    dComIfGp_particle_drawDarkworld(&draw_info);
                }

                #if DEBUG
                // "drawing up to dark world particle (Rendering)"
                fapGm_HIO_c::stopCpuTimer("闇世界でもカラーのパーティクル描画まで（レンダリング）");

                fapGm_HIO_c::startCpuTimer();
                #endif

                retry_captue_frame(camera_p, view_port, dComIfGp_getCameraZoomForcus(camera_id));

                #if DEBUG
                // "Frame Buffer capture 2nd time (Rendering)"
                fapGm_HIO_c::stopCpuTimer("フレームバッファキャプチャー２回目（レンダリング）");

                fapGm_HIO_c::startCpuTimer();
                #endif

                GXSetClipMode(GX_CLIP_ENABLE);

                #if DEBUG
                if (g_kankyoHIO.navy.field_0x30d == 0 || dKy_darkworld_check() != TRUE)
                #endif
                {
                    if (g_env_light.is_blure == 1) {
                        dComIfGd_drawOpaListInvisible();
                        dComIfGd_drawXluListInvisible();
                    }
                }

                #if DEBUG
                if (fapGmHIO_getParticle())
                #endif
                {
                    dComIfGp_particle_drawScreen(&draw_info);
                }

                #if DEBUG
                // "drawing up to full projection particle (Rendering)"
                fapGm_HIO_c::stopCpuTimer("完全投影用パーティクル描画まで（レンダリング）");

                fapGm_HIO_c::startCpuTimer();
                #endif

                GXSetClipMode(GX_CLIP_ENABLE);

                dComIfGd_drawIndScreen();

                if (strcmp(dComIfGp_getStartStageName(), "F_SP124") == 0) {
                    retry_captue_frame(camera_p, view_port,
                                       dComIfGp_getCameraZoomForcus(camera_id));
                }

                GXSetViewport(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, 0.0f, 1.0f);

                Mtx m2;
                Mtx44 m;
                C_MTXPerspective(m, AREG_F(8) + 60.0f, mDoGph_gInf_c::getAspect(), 1.0f, 100000.0f);
                GXSetProjection(m, GX_PERSPECTIVE);
                cXyz sp38c(0.0f, 0.0f, AREG_F(7) + -2.0f);
                cXyz sp398(0.0f, 1.0f, 0.0f);

                cMtx_lookAt(m2, &sp38c, &cXyz::Zero, &sp398, 0);
                j3dSys.setViewMtx(m2);
                dComIfGd_drawXluList2DScreen();

                j3dSys.setViewMtx(camera_p->viewMtx);
                GXSetProjection(camera_p->projMtx, GX_PERSPECTIVE);

                #if DEBUG
                // "drawing up to full projection screen (Rendering)"
                fapGm_HIO_c::stopCpuTimer("完全投影用スクリーン描画まで（レンダリング）");

                fapGm_HIO_c::startCpuTimer();
                #endif

                j3dSys.reinitGX();

                if ((g_env_light.camera_water_in_status || !strcmp(dComIfGp_getStartStageName(), "D_MN08")))
                {
                    u8 enable = mDoGph_gInf_c::getBloom()->getEnable();
                    GXColor color = *mDoGph_gInf_c::getBloom()->getMonoColor();
                    if (color.a != 0 || enable) {
                        retry_captue_frame(camera_p, view_port,
                                           dComIfGp_getCameraZoomForcus(camera_id));
                    }
                }

                #if DEBUG
                // "Frame Buffer capture 3rd time (Rendering)"
                fapGm_HIO_c::stopCpuTimer("※フレームバッファキャプチャー３回目（レンダリング）");

                fapGm_HIO_c::startCpuTimer();
                #endif

                mDoGph_gInf_c::getBloom()->draw();
                j3dSys.setViewMtx(camera_p->viewMtx);
                GXSetProjection(camera_p->projMtx, GX_PERSPECTIVE);

                #if DEBUG
                if (g_kankyoHIO.navy.field_0x30d != 0 && dKy_darkworld_check() == TRUE) {
                    dComIfGd_drawOpaListDark();
                    dComIfGd_drawXluListDark();
                    retry_captue_frame(camera_p, view_port,
                                       dComIfGp_getCameraZoomForcus(camera_id));
                    dComIfGd_drawOpaListInvisible();
                    dComIfGd_drawXluListInvisible();
                    dComIfGd_drawOpaListFilter();
                }
                #endif

                dComIfGd_drawOpaList3Dlast();

                #if DEBUG
                // "saturation add filter (Rendering)"
                fapGm_HIO_c::stopCpuTimer("飽和加算フィルター（レンダリング）");

                fapGm_HIO_c::startCpuTimer();
                #endif

                #if DEBUG
                if (fapGmHIO_getParticle())
                #endif
                {
                    #if WIDESCREEN_SUPPORT
                    if (mDoGph_gInf_c::isWideZoom()) {
                        ortho.setOrtho(0.0f, 0.0f, 608.0f, 448.0f, 100000.0f, -100000.0f);
                    } else
                    #endif
                    {
                        ortho.setOrtho(mDoGph_gInf_c::getMinXF(), mDoGph_gInf_c::getMinYF(),
                                       mDoGph_gInf_c::getWidthF(), mDoGph_gInf_c::getHeightF(),
                                       100000.0f, -100000.0f);
                    }
                    ortho.setPort();

                    Mtx m3;
                    MTXTrans(m3, FB_WIDTH / 2, FB_HEIGHT / 2, 0.0f);
                    JPADrawInfo draw_info2(m3, 0.0f, FB_HEIGHT, 0.0f, FB_WIDTH);
                    dComIfGp_particle_draw2Dgame(&draw_info2);
                }

                trimming(camera_p, view_port);

                if (strcmp(dComIfGp_getStartStageName(), "F_SP127") != 0 &&
                    (mDoGph_gInf_c::isFade() & 0x80) == 0)
                {
                    mDoGph_gInf_c::calcFade();
                }

                #if DEBUG
                // "color fade draw (Rendering)"
                fapGm_HIO_c::stopCpuTimer("カラーフェード描画（レンダリング）");
                #endif
            }
        }
    }

    #if DEBUG
    fapGm_HIO_c::startCpuTimer();
    #endif

    #if PLATFORM_WII
    if (data_8053a730) {
        GXSetTexCopySrc(0, 0, FB_WIDTH, FB_HEIGHT);
        GXSetTexCopyDst(FB_WIDTH, FB_HEIGHT, (GXTexFmt)mDoGph_gInf_c::m_fullFrameBufferTimg->format, 0);
        GXCopyTex(mDoGph_gInf_c::m_fullFrameBufferTex, 0);
        GXPixModeSync();
        GXInvalidateTexAll();

        mDoLib_setResTimgObj(mDoGph_gInf_c::m_fullFrameBufferTimg, &mDoGph_gInf_c::m_fullFrameBufferTexObj, 0, NULL);
        GXLoadTexObj(&mDoGph_gInf_c::m_fullFrameBufferTexObj, GX_TEXMAP0);

        GXSetNumChans(0);
        GXSetNumIndStages(0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
        GXSetZCompLoc(GX_ENABLE);
        GXSetZMode(GX_DISABLE, GX_ALWAYS, GX_DISABLE);
        GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, g_clearColor);
        GXSetFogRangeAdj(GX_DISABLE, 0, NULL);
        GXSetCullMode(GX_CULL_NONE);
        GXSetDither(GX_ENABLE);
        
        Mtx44 mtx;
        MTXOrtho(mtx, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 10.0f);
        GXSetProjection(mtx, GX_ORTHOGRAPHIC);
        GXLoadPosMtxImm(cMtx_getIdentity(), GX_PNMTX0);
        GXSetCurrentMtx(0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S8, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGB8, 0);
        drawFilterQuad(1, 1);
    }
    #endif

    GXSetClipMode(GX_CLIP_ENABLE);
    dDlst_list_c::calcWipe();
    j3dSys.reinitGX();

    ortho.setOrtho(mDoGph_gInf_c::getMinXF(), mDoGph_gInf_c::getMinYF(),
                   mDoGph_gInf_c::getWidthF(), mDoGph_gInf_c::getHeightF(),
                   100000.0f, -100000.0f);
    ortho.setPort();

    #if DEBUG
    captureScreenSetPort();
    #endif

    #if DEBUG
    if (fapGmHIO_get2Ddraw())
    #endif
    {
        Mtx m4;
        cMtx_copy(j3dSys.getViewMtx(), m4);

        Mtx m5;
        MTXTrans(m5, FB_WIDTH / 2, FB_HEIGHT / 2, 0.0f);

        JPADrawInfo draw_info3(m5, 0.0f, FB_HEIGHT, 0.0f, FB_WIDTH);

        if (!dComIfGp_isPauseFlag()) {
            dComIfGp_particle_draw2Dback(&draw_info3);
        }

        dComIfGp_particle_draw2DmenuBack(&draw_info3);
        ortho.setPort();

        dComIfGd_draw2DOpa();
        drawItem3D();
        ortho.setPort();

        #if DEBUG
        captureScreenSetPort();
        #endif

        dComIfGd_draw2DOpaTop();
        dComIfGd_draw2DXlu();

        if (!dComIfGp_isPauseFlag()) {
            dComIfGp_particle_draw2Dfore(&draw_info3);
        }

        if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0 || (mDoGph_gInf_c::isFade() & 0x80) != 0)
        {
            mDoGph_gInf_c::calcFade();
        }

        dComIfGp_particle_draw2DmenuFore(&draw_info3);
        j3dSys.setViewMtx(m4);
    }

    #if DEBUG
    // "drawing up to 2D-fore particle (Rendering)"
    fapGm_HIO_c::stopCpuTimer("２Ｄ前（？）パーティクル描画まで（レンダリング）");
    #endif

    mDoGph_gInf_c::endRender();

    #if WIDESCREEN_SUPPORT
    mDoGph_gInf_c::offWideZoom();
    #endif
    return 1;
}

#if DEBUG
mDoGph_HIO_c mDoGph_HIO;
#endif

int mDoGph_Create() {
    JKRSolidHeap* heap = mDoExt_createSolidHeapToCurrent(0, NULL, 0);
    mDoGph_gInf_c::create();
    dComIfGd_init();
    u32 var_r30 = mDoExt_adjustSolidHeap(heap);
    mDoExt_restoreCurrentHeap();

    OS_REPORT("mDoGph_Create 使用ヒープサイズ=%08x\n", var_r30);
    #if PLATFORM_SHIELD
    mDoGph_gInf_c::setHeap(heap);
    #endif

    #if DEBUG
    mDoGph_HIO.entryHIO();
    #endif
    return 1;
}
