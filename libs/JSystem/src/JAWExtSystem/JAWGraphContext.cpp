#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAWExtSystem/JAWGraphContext.h"
#include "JSystem/J2DGraph/J2DPrint.h"
#include "JSystem/JUtility/JUTResFont.h"

JUTResFont* JAWGraphContext::sFont;
 bool JAWGraphContext::lbl_8074CD30;

JAWGraphContext::JAWGraphContext() :
    field_0x0(NULL),
    field_0x4(255, 255, 255, 255),
    field_0x8(255, 255, 255, 255),
    field_0xc(255, 255, 255, 255),
    field_0x10(255, 255, 255, 255) {
    mParentAlpha = 255;
    field_0x15 = 6;
    field_0x16 = 6;
    field_0x18 = 0;
    if (!sFont) {
        sFont = new JUTResFont((ResFONT*)JUTResFONT_Ascfont_fix12, NULL);
    }
    field_0x0 = new J2DPrint(sFont, JUtility::TColor(255, 255, 255, 255), JUtility::TColor(255, 255, 255, 255));
    field_0x0->initiate();
    locate(0, 0);
}

JAWGraphContext::~JAWGraphContext() {}

void JAWGraphContext::reset() {
    locate(0, 0);
    color(255, 255, 255, 255);
    setGXforPrint();
}

void JAWGraphContext::color(u8 r, u8 g, u8 b, u8 a) {
    color(JUtility::TColor(r, g, b, a));
}

void JAWGraphContext::color(const JUtility::TColor& c) {
    color(c, c, c, c);
}

void JAWGraphContext::locate(int param_1, int param_2) {
    field_0x0->locate(param_1 * field_0x0->getFont()->getWidth(), (param_2 + 1) * field_0x0->getFont()->getHeight());
}

void JAWGraphContext::print(char const* msg, ...) {
    va_list args;
    if (field_0x0->getCursorV() > 640.0f) {
        return;
    }
    if (field_0x18) {
        setGXforPrint();
    }
    va_start(args, msg);
    u8 alpha = field_0x4.a * mParentAlpha / 255;
    field_0x0->print_va(alpha, msg, args);
    va_end(args);
}

void JAWGraphContext::print(int param_1, int param_2, const char* msg, ...) {
    va_list args;
    locate(param_1, param_2);
    if (field_0x0->getCursorV() > 640.0f) {
        return;
    }
    if (field_0x18) {
        setGXforPrint();
    }
    va_start(args, msg);
    u8 alpha = field_0x4.a * mParentAlpha / 255;
    field_0x0->print_va(alpha, msg, args);
    va_end(args);
}

void JAWGraphContext::color(const JUtility::TColor& param_1, const JUtility::TColor& param_2, const JUtility::TColor& param_3, const JUtility::TColor& param_4) {
    field_0x4 = param_1;
    field_0x8 = param_2;
    field_0xc = param_3;
    field_0x10 = param_4;
    field_0x0->setCharColor(param_1);
    field_0x0->setGradColor(param_2);
}

void JAWGraphContext::fillBox(const JGeometry::TBox2<f32>& box) {
    if (field_0x18 != 1) {
        setGXforDraw();
    }
    JUtility::TColor color0(field_0x4);
    JUtility::TColor color1(field_0x8);
    JUtility::TColor color2(field_0xc);
    JUtility::TColor color3(field_0x10);
    if (mParentAlpha != 255) {
        color0.a = field_0x4.a * mParentAlpha / 255;
        color1.a = field_0x8.a * mParentAlpha / 255;
        color2.a = field_0xc.a * mParentAlpha / 255;
        color3.a = field_0x10.a * mParentAlpha / 255;
    }
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s16(box.i.x, box.i.y, 0);
    GXColor1u32(color0);
    GXPosition3s16(box.f.x, box.i.y, 0);
    GXColor1u32(color2);
    GXPosition3s16(box.f.x, box.f.y, 0);
    GXColor1u32(color3);
    GXPosition3s16(box.i.x, box.f.y, 0);
    GXColor1u32(color1);
    GXEnd();
}

void JAWGraphContext::drawFrame(const JGeometry::TBox2<f32>& box) {
    if (field_0x18 != 1) {
        setGXforDraw();
    }
    JUtility::TColor color0(field_0x4);
    JUtility::TColor color1(field_0x8);
    JUtility::TColor color2(field_0xc);
    JUtility::TColor color3(field_0x10);
    if (mParentAlpha != 255) {
        color0.a = field_0x4.a * mParentAlpha / 255;
        color1.a = field_0x8.a * mParentAlpha / 255;
        color2.a = field_0xc.a * mParentAlpha / 255;
        color3.a = field_0x10.a * mParentAlpha / 255;
    }
    GXBegin(GX_LINESTRIP, GX_VTXFMT0, 5);
    GXPosition3s16(box.i.x, box.i.y, 0);
    GXColor1u32(color0);
    GXPosition3s16(box.f.x, box.i.y, 0);
    GXColor1u32(color2);
    GXPosition3s16(box.f.x, box.f.y, 0);
    GXColor1u32(color3);
    GXPosition3s16(box.i.x, box.f.y, 0);
    GXColor1u32(color1);
    GXPosition3s16(box.i.x, box.i.y, 0);
    GXColor1u32(color0);
    GXEnd();
}

void JAWGraphContext::line(const JGeometry::TVec2<f32>& p1, const JGeometry::TVec2<f32>& p2) {
    if (field_0x18 != 1) {
        setGXforDraw();
    }
    JUtility::TColor color0(field_0x4);
    JUtility::TColor color1(field_0x8);
    if (mParentAlpha != 255) {
        color0.a = field_0x4.a * mParentAlpha / 255;
        color1.a = field_0x8.a * mParentAlpha / 255;
    }
    GXBegin(GX_LINES, GX_VTXFMT0, 2);
    GXPosition3s16(p1.x, p1.y, 0);
    GXColor1u32(color0);
    GXPosition3s16(p2.x, p2.y, 0);
    GXColor1u32(color1);
    GXEnd();
}

void JAWGraphContext::setGXforPrint() {
    GXSetNumChans(1);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGBX8, 15);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    field_0x18 = 0;
}

void JAWGraphContext::setGXforDraw() {
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetPointSize(field_0x15, GX_TO_ZERO);
    GXSetLineWidth(field_0x16, GX_TO_ZERO);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    field_0x18 = 1;
}
