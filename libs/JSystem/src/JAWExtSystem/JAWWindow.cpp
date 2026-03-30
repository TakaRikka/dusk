#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAWExtSystem/JAWWindow.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include "JSystem/JUtility/JUTResFont.h"
#include "JSystem/JGeometry.h"

JAWWindow::JAWWindow(const char* param_1, int param_2, int param_3) :
    m_drawWindow('WIN1', JGeometry::TBox2<f32>(0.0f, 0.0f, param_2, param_3), "frame_lu.bti"),
    m_titleText('TITL', JGeometry::TBox2<f32>(10.0f, 10.0f, 640.0f, 170.0f), (const ResFONT*)JUTResFONT_Ascfont_fix16, param_1, -1, HBIND_LEFT, VBIND_TOP),
    m_windowText(this),
    m_windowColor0(JUtility::TColor(0, 0, 0, 255)),
    m_windowColor1(JUtility::TColor(0, 0, 0, 255)),
    m_windowColor2(JUtility::TColor(0, 0, 0, 255)),
    m_windowColor3(JUtility::TColor(0, 0, 0, 255)) {
    field_0x3e8 = 0;
    m_isInit = FALSE;
    m_drawWindow.setContentsColor(m_windowColor0, m_windowColor2, m_windowColor1, m_windowColor3);
    m_titleText.setCharColor(JUtility::TColor(0, 255, 0, 255));
    m_titleText.setGradColor(JUtility::TColor(255, 255, 255, 255));
    m_drawWindow.appendChild(&m_titleText);
    m_drawWindow.appendChild(&m_windowText);
}

JAWWindow::~JAWWindow() {}

BOOL JAWWindow::initIf() {
    if (m_isInit) {
        return TRUE;
    } else {
        m_isInit = TRUE;
        return onInit();
    }
}

BOOL JAWWindow::onInit() { return TRUE; }

void JAWWindow::setTitleColor(const JUtility::TColor& param_1, const JUtility::TColor& param_2) {
    m_titleText.setCharColor(param_1);
    m_titleText.setGradColor(param_2);
}

void JAWWindow::setWindowColor(const JUtility::TColor& param_1, const JUtility::TColor& param_2, const JUtility::TColor& param_3, const JUtility::TColor& param_4) {
    m_windowColor0 = param_1;
    m_windowColor1 = param_2;
    m_windowColor2 = param_3;
    m_windowColor3 = param_4;
    m_drawWindow.setContentsColor(m_windowColor0, m_windowColor2, m_windowColor1, m_windowColor3);
}

void JAWWindow::onDraw(JAWGraphContext*) {}

void JAWWindow::move(f32 param_1, f32 param_2) {
    m_drawWindow.move(param_1, param_2);
}

void JAWWindow::addPosition(f32 param_1, f32 param_2) {
    m_drawWindow.add(param_1, param_2);
}

void JAWWindow::addSize(f32 width, f32 height) {
    JGeometry::TBox2<f32> bounds = m_drawWindow.getBounds();
    f32 newWidth = width + bounds.getWidth();
    f32 newHeight = height + bounds.getHeight();
    if (newWidth < 36.0f) {
        newWidth = 36.0f;
    } else if (newWidth > 640.0f) {
        newWidth = 640.0f;
    }
    if (newHeight < 36.0f) {
        newHeight = 36.0f;
    } else if (newHeight > 480.0f) {
        newHeight = 480.0f;
    }
    m_drawWindow.resize(newWidth, newHeight);
}

JUtility::TColor JAWWindow::convJudaColor(u16 param_1) {
    return JUtility::TColor((param_1 & 0xf800) >> 8, (param_1 & 0x7c0) >> 3, (param_1 & 0x3e) << 2, 255);
}

void JAWWindow::padProc(const JUTGamePad& pad) {
    u32 trigger = pad.getTrigger();
    u32 release = pad.getRelease();
    u32 button = pad.getButton();
    u32 repeat = pad.getRepeat();
    onPadProc(pad);
    if (trigger & PAD_BUTTON_A) {
        onTrigA(pad);
    }
    if (trigger & PAD_BUTTON_B) {
        onTrigB(pad);
    }
    if (trigger & PAD_BUTTON_X) {
        onTrigX(pad);
    }
    if (trigger & PAD_BUTTON_Y) {
        onTrigY(pad);
    }
    if (trigger & PAD_BUTTON_MENU) {
        onTrigMenu(pad);
    }
    if (trigger & PAD_TRIGGER_L) {
        onTrigL(pad);
    }
    if (trigger & PAD_TRIGGER_Z) {
        onTrigZ(pad);
    }
    if (trigger & PAD_BUTTON_UP) {
        onTrigUp(pad);
    }
    if (trigger & PAD_BUTTON_DOWN) {
        onTrigDown(pad);
    }
    if (trigger & PAD_BUTTON_LEFT) {
        onTrigLeft(pad);
    }
    if (trigger & PAD_BUTTON_RIGHT) {
        onTrigRight(pad);
    }
    if (release & PAD_BUTTON_A) {
        onReleaseA(pad);
    }
    if (release & PAD_BUTTON_B) {
        onReleaseB(pad);
    }
    if (release & PAD_BUTTON_X) {
        onReleaseX(pad);
    }
    if (release & PAD_BUTTON_Y) {
        onReleaseY(pad);
    }
    if (release & PAD_BUTTON_MENU) {
        onReleaseMenu(pad);
    }
    if (release & PAD_TRIGGER_L) {
        onReleaseL(pad);
    }
    if (release & PAD_TRIGGER_Z) {
        onReleaseZ(pad);
    }
    if (release & PAD_BUTTON_UP) {
        onReleaseUp(pad);
    }
    if (release & PAD_BUTTON_DOWN) {
        onReleaseDown(pad);
    }
    if (release & PAD_BUTTON_LEFT) {
        onReleaseLeft(pad);
    }
    if (release & PAD_BUTTON_RIGHT) {
        onReleaseRight(pad);
    }
    if (repeat & PAD_BUTTON_A) {
        onKeyA(pad);
    }
    if (repeat & PAD_BUTTON_B) {
        onKeyB(pad);
    }
    if (repeat & PAD_BUTTON_X) {
        onKeyX(pad);
    }
    if (repeat & PAD_BUTTON_Y) {
        onKeyY(pad);
    }
    if (repeat & PAD_BUTTON_MENU) {
        onKeyMenu(pad);
    }
    if (repeat & PAD_TRIGGER_L) {
        onKeyL(pad);
    }
    if (repeat & PAD_TRIGGER_Z) {
        onKeyZ(pad);
    }
    if (repeat & PAD_BUTTON_UP) {
        onKeyUp(pad);
    }
    if (repeat & PAD_BUTTON_DOWN) {
        onKeyDown(pad);
    }
    if (repeat & PAD_BUTTON_LEFT) {
        onKeyLeft(pad);
    }
    if (repeat & PAD_BUTTON_RIGHT) {
        onKeyRight(pad);
    }
}

JAWWindow::TWindowText::TWindowText(JAWWindow* window) :
    J2DPane('TEXT', JGeometry::TBox2<f32>(10.0f, 30.0f, 650.0f, 510.0f)),
    m_pParent(window),
    m_point(0, 0) {
}

JAWWindow::TWindowText::~TWindowText() {}

void JAWWindow::TWindowText::drawSelf(f32 param_1, f32 param_2) {
    Mtx mtx;
    MTXIdentity(mtx);
    drawSelf(param_1, param_2, &mtx);
}

void JAWWindow::TWindowText::drawSelf(f32, f32, Mtx* param_3) {
    m_graf.reset();
    MTXTrans(*param_3, -m_point.x, -m_point.y, 0.0f);
    Mtx stack_48;
    MTXConcat(*param_3, mGlobalMtx, stack_48);
    GXLoadPosMtxImm(stack_48, 0);
    m_graf.setParentAlpha(mColorAlpha);
    JUT_ASSERT(209, m_pParent != NULL);
    m_pParent->setMatrix(stack_48);
    m_pParent->onDraw(&m_graf);
}

static void dummy(J2DPane* pane, J2DWindow* window) {
    pane->rewriteAlpha();
}
