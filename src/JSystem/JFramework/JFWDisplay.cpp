#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JFramework/JFWDisplay.h"
#include "JSystem/J2DGraph/J2DOrthoGraph.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "JSystem/JUtility/JUTProcBar.h"
#include "dolphin/gx.h"
#include <dolphin/vi.h>
#include "global.h"

void JFWDisplay::ctor_subroutine(bool enableAlpha) {
    mEnableAlpha = enableAlpha;
    mClamp = GX_CLAMP_TOP | GX_CLAMP_BOTTOM;
    mClearColor.set(0, 0, 0, 0);
    mZClear = 0xFFFFFF;
    mGamma = 0;
    mFader = NULL;
    mFrameRate = 1;
    mTickRate = 0;
    mCombinationRatio = 0.0f;
    field_0x30 = 0;
    field_0x2c = OSGetTick();
    field_0x34 = 0;
    field_0x48 = 0;
    field_0x4a = 0;
    mDrawDoneMethod = UNK_METHOD_0;
    clearEfb_init();
    JUTProcBar::create();
    JUTProcBar::clear();
    field_0x38 = 1;
    field_0x3c = 0;
    field_0x40 = 0;
    field_0x44 = 0;
}

static Mtx e_mtx ATTRIBUTE_ALIGN(32) = {
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
};

static u8 clear_z_TX[64] ATTRIBUTE_ALIGN(32) = {
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

JFWDisplay::JFWDisplay(JKRHeap* p_heap, JUTXfb::EXfbNumber xfb_num, bool enableAlpha) {
    ctor_subroutine(enableAlpha);
    mXfbManager = JUTXfb::createManager(p_heap, xfb_num);
}

JFWDisplay::~JFWDisplay() {
    if (JUTVideo::getManager() != NULL) {
        waitBlanking(2);
    }
    JUTProcBar::destroy();
    JUTXfb::destroyManager();
    mXfbManager = NULL;
}

JFWDisplay* JFWDisplay::sManager;

JFWDisplay* JFWDisplay::createManager(_GXRenderModeObj const* p_rObj, JKRHeap* p_heap,
                                      JUTXfb::EXfbNumber xfb_num, bool enableAlpha) {
    if (p_rObj != NULL) {
        JUTVideo::getManager()->setRenderMode(p_rObj);
    }

    if (sManager == NULL) {
        sManager = new JFWDisplay(p_heap, xfb_num, enableAlpha);
    }

    return sManager;
}

static void callDirectDraw() {
    JUTChangeFrameBuffer(JUTXfb::getManager()->getDrawingXfb(),
                         JUTVideo::getManager()->getEfbHeight(),
                         JUTVideo::getManager()->getFbWidth());
    JUTAssertion::flushMessage();
}

void JFWDisplay::prepareCopyDisp() {
    u16 width = JUTVideo::getManager()->getFbWidth();
    u16 height = JUTVideo::getManager()->getEfbHeight();
    f32 y_scaleF = GXGetYScaleFactor(height, JUTVideo::getManager()->getXfbHeight());
    u16 line_num = GXGetNumXfbLines(height, y_scaleF);

    GXSetCopyClear(mClearColor, mZClear);
    GXSetDispCopySrc(0, 0, width, height);
    GXSetDispCopyDst(width, line_num);
    GXSetDispCopyYScale(y_scaleF);
    VIFlush();
    GXSetCopyFilter((GXBool)JUTVideo::getManager()->isAntiAliasing(),
                    JUTVideo::getManager()->getSamplePattern(), GX_ENABLE,
                    JUTVideo::getManager()->getVFilter());
    GXSetCopyClamp((GXFBClamp)mClamp);
    GXSetDispCopyGamma((GXGamma)mGamma);
    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    if (mEnableAlpha) {
        GXSetAlphaUpdate(GX_ENABLE);
    }
}

void JFWDisplay::drawendXfb_single() {
    JUTXfb* manager = JUTXfb::getManager();
    if (manager->getDrawingXfbIndex() >= 0) {
        prepareCopyDisp();
        JFWDrawDoneAlarm();
        GXFlush();
        manager->setDrawnXfbIndex(manager->getDrawingXfbIndex());
    }
}

void JFWDisplay::exchangeXfb_double() {
    JUTXfb* xfbMng = JUTXfb::getManager();

    if (xfbMng->getDrawnXfbIndex() == xfbMng->getDisplayingXfbIndex()) {
        if (xfbMng->getDrawingXfbIndex() >= 0) {
            if (field_0x44 != NULL) {
                field_0x44();
            }

            prepareCopyDisp();
            GXCopyDisp(xfbMng->getDrawingXfb(), GX_TRUE);
            if (mDrawDoneMethod == UNK_METHOD_0) {
                xfbMng->setDrawnXfbIndex(xfbMng->getDrawingXfbIndex());
                GXDrawDone();
                JUTVideo::dummyNoDrawWait();
            } else {
                JUTVideo::drawDoneStart();
            }

            if (mDrawDoneMethod == UNK_METHOD_0) {
                callDirectDraw();
            }
        }
        s16 cur_xfb_index = xfbMng->getDrawingXfbIndex();
        xfbMng->setDrawnXfbIndex(cur_xfb_index);
        xfbMng->setDrawingXfbIndex(cur_xfb_index >= 0 ? cur_xfb_index ^ 1 : 0);
    } else {
        clearEfb(mClearColor);
        if (xfbMng->getDrawingXfbIndex() < 0) {
            xfbMng->setDrawingXfbIndex(0);
        }
    }
}

void JFWDisplay::exchangeXfb_triple() {
    JUTXfb* xfbMng = JUTXfb::getManager();

    if (xfbMng->getDrawingXfbIndex() >= 0) {
        callDirectDraw();
    }

    xfbMng->setDrawnXfbIndex(xfbMng->getDrawingXfbIndex());

    s16 drawing_idx = xfbMng->getDrawingXfbIndex() + 1;
    do {
        if (drawing_idx >= 3 || drawing_idx < 0) {
            drawing_idx = 0;
        }
    } while (drawing_idx == xfbMng->getDisplayingXfbIndex());
    xfbMng->setDrawingXfbIndex(drawing_idx);
}

void JFWDisplay::copyXfb_triple() {
    JUTXfb* xfbMng = JUTXfb::getManager();

    if (xfbMng->getDrawingXfbIndex() >= 0) {
        if (field_0x44 != NULL) {
            field_0x44();
        }
        prepareCopyDisp();
        GXCopyDisp(xfbMng->getDrawingXfb(), GX_TRUE);
        GXPixModeSync();
    }
}

void JFWDisplay::preGX() {
    GXInvalidateTexAll();
    GXInvalidateVtxCache();

    if (JUTVideo::getManager()->isAntiAliasing()) {
        GXSetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
        GXSetDither(GX_ENABLE);
    } else {
        if (mEnableAlpha) {
            GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
            GXSetDither(GX_ENABLE);
        } else {
            GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
            GXSetDither(GX_DISABLE);
        }
    }
}

void JFWDisplay::endGX() {
    f32 width = JUTVideo::getManager()->getFbWidth();
    f32 height = JUTVideo::getManager()->getEfbHeight();

    J2DOrthoGraph ortho(0.0f, 0.0f, width, height, -1.0f, 1.0f);

    if (mFader != NULL) {
        ortho.setPort();
        mFader->control();
    }
    ortho.setPort();
    JUTDbPrint::getManager()->flush();

    if (JUTConsoleManager::getManager() != NULL) {
        ortho.setPort();
        JUTConsoleManager::getManager()->draw();
    }

    ortho.setPort();
    JUTProcBar::getManager()->draw();

    if (mDrawDoneMethod != UNK_METHOD_0 || JUTXfb::getManager()->getBufferNum() == 1) {
        JUTAssertion::flushMessage_dbPrint();
    }
    GXFlush();
}

void JFWDisplay::beginRender() {
    if (field_0x40) {
        JUTProcBar::getManager()->wholeLoopEnd();
    }

    if (field_0x40) {
        JUTProcBar::getManager()->wholeLoopStart();
        JUTProcBar::getManager()->idleStart();
    }

    waitForTick(mTickRate, mFrameRate);
    JUTVideo::getManager()->waitRetraceIfNeed();

    OSTick tick = OSGetTick();
    field_0x30 = tick - field_0x2c;
    field_0x2c = tick;
    field_0x34 = field_0x2c - JUTVideo::getVideoLastTick();

    if (field_0x40) {
        JUTProcBar::getManager()->idleEnd();
    }

    if (field_0x40) {
        JUTProcBar::getManager()->gpStart();

        JUTXfb* xfbMng = JUTXfb::getManager();
        switch (xfbMng->getBufferNum()) {
        case 1:
            if (xfbMng->getSDrawingFlag() != 2) {
                xfbMng->setSDrawingFlag(1);
                clearEfb(mClearColor);
            } else {
                xfbMng->setSDrawingFlag(1);
            }
            xfbMng->setDrawingXfbIndex(field_0x48);
            break;
        case 2:
            exchangeXfb_double();
            break;
        case 3:
            exchangeXfb_triple();
            break;
        default:
            break;
        }
    }

    field_0x3c++;
    field_0x40 = field_0x3c >= field_0x38;

    if (field_0x40) {
        field_0x3c = 0;
    }

    if (field_0x40) {
        clearEfb();
        preGX();
    }
}

void JFWDisplay::endRender() {
    endGX();

    if (field_0x40) {
        switch (JUTXfb::getManager()->getBufferNum()) {
        case 1:
            drawendXfb_single();
        case 2:
            break;
        case 3:
            copyXfb_triple();
        default:
            break;
        }
    }

    JUTProcBar::getManager()->cpuStart();
    calcCombinationRatio();
}

void JFWDisplay::endFrame() {
    JUTProcBar::getManager()->cpuEnd();

    if (field_0x40) {
        JUTProcBar::getManager()->gpWaitStart();
        switch (JUTXfb::getManager()->getBufferNum()) {
        case 1:
            break;
        case 2:
            JFWDrawDoneAlarm();
            GXFlush();
            break;
        case 3:
            JFWDrawDoneAlarm();
            GXFlush();
            break;
        default:
            break;
        }

        JUTProcBar::getManager()->gpWaitEnd();
        JUTProcBar::getManager()->gpEnd();
    }

    if (field_0x40) {
        static u32 prevFrame = VIGetRetraceCount();;
        u32 retrace_cnt = VIGetRetraceCount();
        JUTProcBar::getManager()->setCostFrame(retrace_cnt - prevFrame);
        prevFrame = retrace_cnt;
    }
}

void JFWDisplay::waitBlanking(int param_0) {
    while (param_0-- > 0) {
        waitForTick(mTickRate, mFrameRate);
    }
}

static void waitForTick(u32 p1, u16 p2) {
    if (p1 != 0) {
        static OSTime nextTick = OSGetTime();
        OSTime time = OSGetTime();
        while (time < nextTick) {
            JFWDisplay::getManager()->threadSleep((nextTick - time));
            time = OSGetTime();
        }
        nextTick = time + p1;
    }
    else {
        static u32 nextCount = VIGetRetraceCount();
        u32 uVar1 = (p2 == 0) ? 1 : p2;
        OSMessage msg;
        do {
            if (!OSReceiveMessage(JUTVideo::getManager()->getMessageQueue(), &msg, OS_MESSAGE_BLOCK)) {
                msg = 0;
            }
        } while (((int)msg - (int)nextCount) < 0);
        nextCount = (int)msg + uVar1;
    }
}

JSUList<JFWAlarm> JFWAlarm::sList(false);

static void JFWThreadAlarmHandler(OSAlarm* p_alarm, OSContext* p_ctx) {
    JFWAlarm* alarm = static_cast<JFWAlarm*>(p_alarm);
    alarm->removeLink();
    OSResumeThread(alarm->getThread());
}

void JFWDisplay::threadSleep(s64 time) {
    JFWAlarm alarm;
    alarm.createAlarm();
    alarm.setThread(OSGetCurrentThread());
    s32 status = OSDisableInterrupts();
    alarm.appendLink();

    OSSetAlarm(&alarm, time, JFWThreadAlarmHandler);
    OSSuspendThread(alarm.getThread());
    OSRestoreInterrupts(status);
}

static GXTexObj clear_z_tobj;

void JFWDisplay::clearEfb_init() {
    GXInitTexObj(&clear_z_tobj, &clear_z_TX, 4, 4, GX_TF_Z24X8, GX_REPEAT, GX_REPEAT, GX_FALSE);
    GXInitTexObjLOD(&clear_z_tobj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE,
                    GX_ANISO_1);
}

void JFWDisplay::clearEfb() {
    clearEfb(mClearColor);
}

void JFWDisplay::clearEfb(GXColor color) {
    int width = JUTVideo::getManager()->getFbWidth();
    int height = JUTVideo::getManager()->getEfbHeight();

    clearEfb(0, 0, width, height, color);
}

void JFWDisplay::clearEfb(int param_0, int param_1, int param_2, int param_3, GXColor color) {
    u16 width;
    u16 height;
    Mtx44 mtx;

    JUTVideo::getManager()->getBounds(width, height);

    C_MTXOrtho(mtx, 0.0f, height, 0.0f, width, 0.0f, 1.0f);
    GXSetProjection(mtx, GX_ORTHOGRAPHIC);
    GXSetViewport(0.0f, 0.0f, width, height, 0.0f, 1.0f);
    GXSetScissor(0, 0, width, height);

    GXLoadPosMtxImm(e_mtx, GX_PNMTX0);
    GXSetCurrentMtx(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGB, GX_RGBX8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGB565, 0);
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, GX_DISABLE, 125);
    GXLoadTexObj(&clear_z_tobj, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetZTexture(GX_ZT_REPLACE, GX_TF_Z24X8, 0);
    GXSetZCompLoc(GX_DISABLE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_NOOP);

    if (mEnableAlpha) {
        GXSetAlphaUpdate(GX_ENABLE);
        GXSetDstAlpha(GX_ENABLE, color.a);
    }
    GXSetZMode(GX_ENABLE, GX_ALWAYS, GX_ENABLE);
    GXSetCullMode(GX_CULL_BACK);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2u16(param_0, param_1);
    GXTexCoord2u8(0, 0);

    GXPosition2u16(param_0 + param_2, param_1);
    GXTexCoord2u8(1, 0);

    GXPosition2u16(param_0 + param_2, param_1 + param_3);
    GXTexCoord2u8(1, 1);

    GXPosition2u16(param_0, param_1 + param_3);
    GXTexCoord2u8(0, 1);

    GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z24X8, 0);
    GXSetZCompLoc(GX_ENABLE);
    if (mEnableAlpha) {
        GXSetDstAlpha(GX_DISABLE, color.a);
    }
}

void JFWDisplay::calcCombinationRatio() {
    u32 vidInterval = JUTVideo::getVideoInterval();
    s32 unk30 = field_0x30 * 2;

    s32 i = vidInterval;
    for (; i < unk30; i += vidInterval) {
    }

    s32 tmp = (i - unk30) - field_0x34;
    if (tmp < 0) {
        tmp += vidInterval;
    }
    mCombinationRatio = (f32)tmp / (f32)field_0x30;
    if (mCombinationRatio > 1.0f) {
        mCombinationRatio = 1.0f;
    }
}

static void JFWDrawDoneAlarm() {
    JFWAlarm alarm;
    s32 status = OSDisableInterrupts();
    alarm.createAlarm();
    alarm.appendLink();
    OSSetAlarm(&alarm, 0.5 * (*(u32*)0x800000F8 / 4), JFWGXAbortAlarmHandler);
    GXDrawDone();
    alarm.cancelAlarm();
    alarm.removeLink();
    OSRestoreInterrupts(status);
}

static void JFWGXAbortAlarmHandler(OSAlarm* param_0, OSContext* param_1) {
    diagnoseGpHang();
    GXAbortFrame();
    GXWGFifo.u8 = 0x61;
    GXWGFifo.u32 = 0x5800000F;

    GXFifoObj* fifo = GXGetCPUFifo();
    if (fifo != NULL) {
        void* base = GXGetFifoBase(fifo);
        u32 size = GXGetFifoSize(fifo);
        GXInit(base, size);
    }
    GXSetDrawDone();
}

static void diagnoseGpHang() {
    u32 sp28;
    u32 sp24;
    u32 sp20;
    u32 sp1C;
    u32 sp18;
    u32 sp14;
    u32 sp10;
    u32 spC;
    bool readIdle;
    bool commandIdle;
    bool sp8;

    GXReadXfRasMetric(&sp24, &sp28, &sp1C, &sp20);
    GXReadXfRasMetric(&sp14, &sp18, &spC, &sp10);

    u32 temp_r31 = sp28 == sp18;
    u32 temp_r30 = sp24 == sp14;
    u32 temp_r0 = sp20 != sp10;
    u32 temp_r0_2 = sp1C != spC;

    GXGetGPStatus((GXBool*)&sp8, (GXBool*)&sp8, (GXBool*)&readIdle, (GXBool*)&commandIdle,
                  (GXBool*)&sp8);

    OSReport("GP status %d%d%d%d%d%d --> ", readIdle, commandIdle, temp_r31, temp_r30, temp_r0,
             temp_r0_2);

    if (!temp_r30 && temp_r0) {
        OSReport("GP hang due to XF stall bug.\n");
    } else if (!temp_r31 && temp_r30 && temp_r0) {
        OSReport("GP hang due to unterminated primitive.\n");
    } else if (!commandIdle && temp_r31 && temp_r30 && temp_r0) {
        OSReport("GP hang due to illegal instruction.\n");
    } else if (readIdle && commandIdle && temp_r31 && temp_r30 && temp_r0 && temp_r0_2) {
        OSReport("GP appears to be not hung (waiting for input).\n");
    } else {
        OSReport("GP is in unknown state.\n");
    }
}
