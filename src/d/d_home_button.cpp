#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_home_button.h"
#include "JSystem/JFramework/JFWDisplay.h"
#include "Z2AudioCS/Z2AudioCS.h"
#include "Z2AudioLib/Z2AudioMgr.h"
#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_lib.h"
#include "m_Re/m_Re_controller_pad.h"

int SECallback(int param_0, int param_1) {
    if (param_0 == 5) {
        Z2AudioMgr::getInterface()->homeMenuSeCallback(param_1);
        return 1;
    } else {
        OSReport("SECallback: %d, %d\n", param_0, param_1);
        return 0;
    }
}

// NONMATCHING - stack
void drawCallback(J2DOrthoGraph& param_0, void* param_1) {
    Mtx sp8;
    if (mDoGph_gInf_c::isWide()) {
        MTXOrtho(sp8, 228.0f, -228.0f, -416.0f, 416.0f, 0.0f, 500.0f);
    } else {
        MTXOrtho(sp8, 228.0f, -228.0f, -(FB_WIDTH / 2), (FB_WIDTH / 2), 0.0f, 500.0f);
    }

    GXSetProjection(sp8, GX_ORTHOGRAPHIC);
    GXClearVtxDesc();
    GXSetVtxAttrFmt(GX_VTXFMT4, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT4, GX_VA_CLR0, GX_CLR_RGB, GX_RGB8, 0);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    GXSetCullMode(GX_CULL_NONE);
    GXLoadPosMtxImm(g_mDoMtx_identity, 3);
    GXSetCurrentMtx(3);

    HBMDraw();
}

dHomeButton_c* dHomeButton_c::m_myObj;

u8 dHomeButton_c::lbl_8053A724;
u8 dHomeButton_c::lbl_8053A725;

void dHomeButton_c::create_(int param_0, void* param_1) {
    initCursorPos();
    initHomeButtonInfo(param_0, param_1);

    m_info.mem = JKRGetRootHeap2()->alloc(0x80000, 0);
    m_info.memSize = 0x80000;
    m_info.pAllocator = 0;

    HBMCreate(&m_info);
}

// NONMATCHING
int dHomeButton_c::execute_() {
    if (m_myObj == NULL) {
        return 0;
    }

    calcCursorPos();

    if (field_0x7c == 0 && !mDoRst::isReset()) {
        bool var_r30 = false;
        for (u32 i = 0; i < 4; i++) {
            if (mReCPd::chkDevTypeValid(i) && mReCPd::getTrigHome(i)) {
                var_r30 = true;
            }
        }

        if (var_r30) {
            mReCPd::enableDpd(0);
            Z2AudioCS::disconnect(0);

            HBMSetAdjustFlag(mDoGph_gInf_c::isWide());
            HBMInit();

            mDoGph_gInf_c::setTickRate(OS_TIMER_CLOCK / 60);
            field_0x7c = 1;
            lbl_8053A724 = 0;
        }
    }

    if (field_0x7c) {
        if (lbl_8053A724 == 0 && mDoRst::isReset()) {
            HBMStartBlackOut();
            onReset();
        }

        if (HBMCalc(&m_controllerData) >= HBM_SELECT_HOMEBTN) {
            int selectBtn = HBMGetSelectBtnNum();
            if (selectBtn == HBM_SELECT_BTN1) {
                mDoRst::onReturnToMenu();
                onReset();
            } else if (selectBtn == HBM_SELECT_BTN2) {
                mDoRst_resetCallBack(-1, NULL);
                onReset();
            }

            Z2AudioCS::connect(0);
            mDoGph_gInf_c::setTickRate(field_0x78);
            field_0x7c = 0;
            mReCPd::setExtensionCallback();
        } else {
            drawCapture(0xFF, drawCallback, NULL);
        }
    }

    return field_0x7c;
}

void dHomeButton_c::initHomeButtonInfo(int param_0, void* i_layoutBuf) {
    m_info.layoutBuf = i_layoutBuf;
    m_info.spkSeBuf = dComIfG_getObjectRes("HomeBtn", "SpeakerSe.arc");
    m_info.msgBuf = dComIfG_getObjectRes("HomeBtn", "home.csv");
    m_info.configBuf = dComIfG_getObjectRes("HomeBtn", "config.txt");
    m_info.sound_callback = SECallback;
    m_info.backFlag = 0;
    m_info.region = param_0;
    m_info.cursor = 0;

    m_info.adjust.x = 1.3684211f;
    m_info.adjust.y = 1.0f;
    m_info.frameDelta = 1.0f;
}

void dHomeButton_c::initCursorPos() {
    for (int i = 0; i < 4; i++) {
        m_controllerData.wiiCon[i].pos.x = 0.0f;
        m_controllerData.wiiCon[i].pos.y = 0.0f;
        m_controllerData.wiiCon[i].use_devtype = 0;
    }
}

void dHomeButton_c::calcCursorPos() {
    for (u32 i = 0; i < 4; i++) {
        u32 sp8;
        s32 result = WPADProbe(i, &sp8);

        if (result == -2 || result == -3 || result == 0) {
            m_controllerData.wiiCon[i].kpad = &mReCPd::m_pad[i].field_0x4[0];
        } else {
            m_controllerData.wiiCon[i].kpad = NULL;
        }
    }
}

void dHomeButton_c::onReset() {
    mDoGph_gInf_c::setFadeColor(*(JUtility::TColor*)&g_blackColor);
    lbl_8053A724 = 1;
}

void dHomeButton_c::create(int param_0, void* param_1) {
    m_myObj = new dHomeButton_c();
    m_myObj->create_(param_0, param_1);
}

int dHomeButton_c::execute() {
    if (m_myObj) {
        return m_myObj->execute_();
    }

    return NULL;
}

// NONMATCHING
void dHomeButton_c::drawCapture(u8 param_0, void (*param_1)(J2DOrthoGraph&, void*), void* param_2) {
    if (lbl_8053A725 == 0) {
        GXSetTexCopySrc(0, 0, FB_WIDTH, FB_HEIGHT);
        GXSetTexCopyDst(FB_WIDTH, FB_HEIGHT, (GXTexFmt)mDoGph_gInf_c::m_fullFrameBufferTimg->format, GX_FALSE);
        GXCopyTex(mDoGph_gInf_c::m_fullFrameBufferTex, GX_FALSE);
        lbl_8053A725 = 1;
    }

    JFWDisplay::getManager()->setClearColor(g_clearColor);
    mDoGph_gInf_c::beginRender();
    GXSetAlphaUpdate(GX_DISABLE);
    j3dSys.drawInit();

    J2DOrthoGraph sp30(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, -1.0f, 1.0f);
    sp30.setOrtho(JGeometry::TBox2<f32>(mDoGph_gInf_c::getMinXF(), mDoGph_gInf_c::getMinYF(),
                                        mDoGph_gInf_c::getMinXF() + mDoGph_gInf_c::getWidthF(),
                                        mDoGph_gInf_c::getMinYF() + mDoGph_gInf_c::getHeightF()),
                  -1.0f, 1.0f);
    sp30.setPort();

    dComIfGp_setCurrentGrafPort(&sp30);
    mDoLib_setResTimgObj(mDoGph_gInf_c::m_fullFrameBufferTimg,
                         &mDoGph_gInf_c::m_fullFrameBufferTexObj, 0, NULL);

    GXLoadTexObj(&mDoGph_gInf_c::m_fullFrameBufferTexObj, GX_TEXMAP0);
    GXSetNumChans(0);
    GXSetNumIndStages(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60);
    GXSetNumTevStages(1);

    GXColor sp14 = {0, 0, 0, param_0};
    GXSetTevColor(GX_TEVREG0, sp14);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_A0, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetZCompLoc(GX_TRUE);
    GXSetZMode(GX_DISABLE, GX_ALWAYS, GX_DISABLE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, g_clearColor);
    GXSetFogRangeAdj(GX_DISABLE, 0, NULL);
    GXSetCullMode(GX_CULL_NONE);
    GXSetDither(GX_TRUE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S8, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition2f32(mDoGph_gInf_c::getMinXF(), mDoGph_gInf_c::getMinYF());
    GXTexCoord2u8(0, 0);

    GXPosition2f32(mDoGph_gInf_c::getMaxXF(), mDoGph_gInf_c::getMinYF());
    GXTexCoord2u8(1, 0);

    GXPosition2f32(mDoGph_gInf_c::getMaxXF(), mDoGph_gInf_c::getMaxYF());
    GXTexCoord2u8(1, 1);

    GXPosition2f32(mDoGph_gInf_c::getMinXF(), mDoGph_gInf_c::getMaxYF());
    GXTexCoord2u8(0, 1);

    if (param_1 != NULL) {
        param_1(sp30, param_2);
    }

    JFWDisplay::getManager()->endRender();
    JFWDisplay::getManager()->setFader(NULL);
}
