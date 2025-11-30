/**
 * d_error_msg.cpp
 * Disk Error Message screen handler
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include <cstring.h>
#include "JSystem/J2DGraph/J2DOrthoGraph.h"
#include "JSystem/J2DGraph/J2DPicture.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "JSystem/JUtility/JUTResFont.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "d/d_error_msg.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_graphic.h"

#include "assets/black_tex.h"
#include "assets/msg_data.h"
#if VERSION == VERSION_GCN_PAL
#include "assets/msg_data_ge.h"
#include "assets/msg_data_fr.h"
#include "assets/msg_data_sp.h"
#include "assets/msg_data_it.h"
#endif
#include "assets/font_data.h"

#define MSG_READING_DISC 0
#define MSG_COVER_OPEN   1
#define MSG_NO_DISK      2
#define MSG_WRONG_DISK   3
#define MSG_READ_ERROR   4
#define MSG_FATAL_ERROR  5

// made up
struct BMG_INF1 : JUTDataBlockHeader {
    /* 0x08 */ u8 m08[0x10 - 0x08];
    /* 0x10 */ u32 entries[6];
};

#if (VERSION == VERSION_GCN_JPN) || (VERSION == VERSION_WII_JPN)
#define LINE_HEIGHT 30.0f
#define LINE_SPACE  30.0f
#define CHAR_SPACE  3.0f
#else
#define LINE_HEIGHT 23.0f
#define LINE_SPACE  23.0f
#define CHAR_SPACE  1.0f
#endif

static void messageSet(u32 status, bool i_drawBg) {
    BMG_INF1* inf1;
    const char* msg_p;

    #if VERSION == VERSION_GCN_PAL
    if (dComIfGs_getPalLanguage() == dSv_player_config_c::LANGAUGE_GERMAN) {
        inf1 = (BMG_INF1*)&msg_data_ge[0x20];
    } else if (dComIfGs_getPalLanguage() == dSv_player_config_c::LANGAUGE_FRENCH) {
        inf1 = (BMG_INF1*)&msg_data_fr[0x20];
    } else if (dComIfGs_getPalLanguage() == dSv_player_config_c::LANGAUGE_SPANISH) {
        inf1 = (BMG_INF1*)&msg_data_sp[0x20];
    } else if (dComIfGs_getPalLanguage() == dSv_player_config_c::LANGAUGE_ITALIAN) {
        inf1 = (BMG_INF1*)&msg_data_it[0x20];
    } else {
        inf1 = (BMG_INF1*)&msg_data[0x20];
    } 
    #else
    inf1 = (BMG_INF1*)&msg_data[0x20];
    #endif

    msg_p = (const char*)((u8*)inf1->getNext() + sizeof(JUTDataBlockHeader) + inf1->entries[status]);

    JUT_ASSERT(102, strlen(msg_p)-1 < 512);

    J2DTextBox tpane('TEXT1', JGeometry::TBox2<f32>(0.0f, 0.0f, FB_WIDTH, 200.0f), (ResFONT*)font_data, msg_p, 512, HBIND_CENTER, VBIND_CENTER);
    J2DTextBox spane('TEXT2', JGeometry::TBox2<f32>(0.0f, 0.0f, FB_WIDTH, 200.0f), (ResFONT*)font_data, msg_p, 512, HBIND_CENTER, VBIND_CENTER);
    J2DPicture ppane('PICT1', JGeometry::TBox2<f32>(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT), (ResTIMG*)black_tex, NULL);

    JUTResFont font((ResFONT*)font_data, NULL);
    JUTFont* pfont = (JUTFont*)&font;

    JUtility::TColor tcharcolor(0xC8, 0xC8, 0xC8, 0xFF);
    JUtility::TColor scharcolor(0xFF, 0xFF, 0xFF, 0xFF);
    JUtility::TColor tgradcolor(0xFF, 0xFF, 0xFF, 0xFF);
    JUtility::TColor sgradcolor(0xFF, 0xFF, 0xAD, 0xFF);
    JUtility::TColor twhite(0xC8, 0xC8, 0xC8, 0xFF);
    JUtility::TColor swhite(0x00, 0x00, 0x00, 0xFF);
    JUtility::TColor tblack(0x00, 0x00, 0x00, 0x00);
    JUtility::TColor sblack(0x00, 0x00, 0x00, 0x00);

    J2DTextBox::TFontSize size;
    size.mSizeX = 23.0f;
    size.mSizeY = 22.0f;

    tpane.setFontSize(size);
    tpane.setCharSpace(CHAR_SPACE);
    tpane.setLineSpace(LINE_SPACE);
    tpane.setCharColor(tcharcolor);
    tpane.setGradColor(tgradcolor);
    tpane.setBlackWhite(tblack, twhite);
    
    spane.setFontSize(size);
    spane.setCharSpace(CHAR_SPACE);
    spane.setLineSpace(LINE_SPACE);
    spane.setCharColor(scharcolor);
    spane.setGradColor(sgradcolor);
    spane.setBlackWhite(sblack, swhite);

    const int lineMax = 10;

    f32 height = LINE_HEIGHT;
    f32 maxWidth = 0.0f;
    int cnt = 0;
    f32 lineWidth[lineMax];
    for (int i = 0; i < lineMax; i++) {
        lineWidth[i] = 0.0f;
    }

    for (; *msg_p != '\0'; msg_p++) {
        if (*msg_p == '\n') {
            height += LINE_HEIGHT;
            cnt++;
            JUT_ASSERT(191, cnt < lineMax);
            continue;
        }

        int c = (u8)*msg_p;
        #if (VERSION == VERSION_GCN_JPN) || (VERSION == VERSION_WII_JPN)
        if (pfont->isLeadByte_ShiftJIS(c)) {
        #else
        if (pfont->isLeadByte_EUC(c)) {
        #endif
            msg_p++;
            c = (c << 8) | (u8)*msg_p;
        }

        lineWidth[cnt] += CHAR_SPACE + size.mSizeX * ((f32)pfont->getWidth(c) / (f32)pfont->getCellWidth());
    }

    for (int i = 0; i < lineMax; i++) {
        f32 width = lineWidth[i];
        if (maxWidth < width)
            maxWidth = width;
    }

    #if (VERSION == VERSION_GCN_JPN) || (VERSION == VERSION_WII_JPN)
    f32 temp_0 = 0.0f; // fixes load order
    f32 y = temp_0 + ((FB_HEIGHT - height) / 2);

    if (i_drawBg) {
        ppane.mAlpha = 0x82;
        ppane.draw(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, false, false, false);
    }

    spane.draw(2.0f, y + 10.0f + 2.0f, FB_WIDTH, HBIND_CENTER);
    tpane.draw(0.0f, y + 10.0f, FB_WIDTH, HBIND_CENTER);
    #else
    f32 temp_0 = 0.0f; // fixes load order
    f32 x = temp_0 + ((FB_WIDTH - maxWidth) / 2);
    f32 y = temp_0 + ((FB_HEIGHT - height) / 2);

    if (i_drawBg) {
        ppane.mAlpha = 0x82;
        ppane.draw(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, false, false, false);
    }

    #if VERSION == VERSION_GCN_PAL
    if (dComIfGs_getPalLanguage() == dSv_player_config_c::LANGAUGE_ENGLISH) {
        spane.draw(x + 2.0f, y + 10.0f + 2.0f, FB_WIDTH, HBIND_LEFT);
        tpane.draw(x, y + 10.0f, FB_WIDTH, HBIND_LEFT);
    } else {
        spane.draw(2.0f, y + 10.0f + 2.0f, FB_WIDTH, HBIND_CENTER);
        tpane.draw(0.0f, y + 10.0f, FB_WIDTH, HBIND_CENTER);
    }
    #else
    spane.draw(x + 2.0f, y + 10.0f + 2.0f, FB_WIDTH, HBIND_LEFT);
    tpane.draw(x, y + 10.0f, FB_WIDTH, HBIND_LEFT);
    #endif
    #endif
}

void dDvdErrorMsg_c::draw(s32 status) {
    JUtility::TColor backColor = g_clearColor;
    JFWDisplay::getManager()->setClearColor(backColor);
    mDoGph_gInf_c::beginRender();
    GXSetAlphaUpdate(GX_FALSE);
    j3dSys.drawInit();

    J2DOrthoGraph draw2D(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, -1.0f, 1.0f);
    draw2D.setOrtho(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, -1.0f, 1.0f);
    draw2D.setPort();
    dComIfGp_setCurrentGrafPort(&draw2D);

    if (status == DVD_STATE_NO_DISK) {
        messageSet(MSG_NO_DISK, true);
    } else if (status == DVD_STATE_COVER_OPEN) {
        messageSet(MSG_COVER_OPEN, true);
    } else if (status == DVD_STATE_FATAL_ERROR) {
        messageSet(MSG_FATAL_ERROR, true);
    } else if (status == DVD_STATE_WRONG_DISK) {
        messageSet(MSG_WRONG_DISK, true);
    } else if (status == DVD_STATE_RETRY) {
        messageSet(MSG_READ_ERROR, true);
    } else if (status == DVD_STATE_BUSY) {
        messageSet(MSG_READING_DISC, true);
    }

    mDoGph_gInf_c::endRender();
    JFWDisplay::getManager()->resetFader();
}

u8 dDvdErrorMsg_c::execute() {
    static u8 l_dvdError;

    s32 drive_status = DVDGetDriveStatus();
    if (drive_status != DVD_STATE_END && drive_status != DVD_STATE_BUSY && !l_dvdError) {
        l_dvdError = true;
    }

    if (l_dvdError) {
        OSReport_Error("DVD Error !! <%d>\n", drive_status);

        if (drive_status == DVD_STATE_END) {
            l_dvdError = false;
        } else if (mDoRst::isReset() &&
                  (drive_status == DVD_STATE_NO_DISK || drive_status == DVD_STATE_COVER_OPEN || drive_status == DVD_STATE_WRONG_DISK || drive_status == DVD_STATE_RETRY))
        {
            mDoRst::onReturnToMenu();
        } else {
            draw(drive_status);
        }
    }

    return l_dvdError;
}

static u8 l_captureAlpha = 0xFF;

static void drawCapture(u8 alpha) {
    static bool l_texCopied = false;

    if (!l_texCopied) {
        GXSetTexCopySrc(0, 0, FB_WIDTH, FB_HEIGHT);
        GXSetTexCopyDst(FB_WIDTH / 2, FB_HEIGHT / 2, (GXTexFmt)mDoGph_gInf_c::getFrameBufferTimg()->format, GX_TRUE);
        GXCopyTex(mDoGph_gInf_c::getFrameBufferTex(), GX_FALSE);
        l_texCopied = true;
    }

    mDoGph_gInf_c::setClearColor(g_clearColor);
    mDoGph_gInf_c::beginRender();
    GXSetAlphaUpdate(GX_FALSE);
    j3dSys.drawInit();

    GXInitTexObj(mDoGph_gInf_c::getFrameBufferTexObj(), mDoGph_gInf_c::getFrameBufferTex(), FB_WIDTH / 2, FB_HEIGHT / 2, (GXTexFmt)mDoGph_gInf_c::getFrameBufferTimg()->format, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(mDoGph_gInf_c::getFrameBufferTexObj(), GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(mDoGph_gInf_c::getFrameBufferTexObj(), GX_TEXMAP0);
    GXSetNumChans(0);
    GXSetNumIndStages(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);

    GXColor color = {0, 0, 0, alpha};
    GXSetTevColor(GX_TEVREG0, color);
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
    GXSetDither(GX_ENABLE);

    Mtx44 m;
    C_MTXOrtho(m, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 10.0f);
    GXLoadPosMtxImm(g_mDoMtx_identity, GX_PNMTX0);
    GXSetProjection(m, GX_ORTHOGRAPHIC);
    GXSetCurrentMtx(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGB, GX_RGB8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGB8, 0);

    mDoGph_drawFilterQuad(1, 1);
    mDoGph_gInf_c::endRender();
    JFWDisplay::getManager()->resetFader();
}

bool dShutdownErrorMsg_c::execute() {
    if (!mDoRst::isShutdown() && !mDoRst::isReturnToMenu()) {
        return false;
    }

    if (l_captureAlpha == 0xFF) {
        if (Z2AudioMgr::getInterface()->isResetting() && !mDoAud_resetRecover()) {
            drawCapture(l_captureAlpha);
            return true;
        }

        if (mDoAud_zelAudio_c::isInitFlag()) {
            Z2AudioMgr::getInterface()->resetProcess(0x10, true);
        }
    }

    drawCapture(l_captureAlpha);

    if (cLib_chaseUC(&l_captureAlpha, 0, 15) != 0) {
        if (mDoRst::isReturnToMenu()) {
            mDoRst_reset(1, 0x80000000, 0);
        } else {
            mDoRst_reset(1, 1, 1);
        }
    }

    return true;
}
