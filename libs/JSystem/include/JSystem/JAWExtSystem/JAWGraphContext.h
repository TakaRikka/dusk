#ifndef JAWGRAPHCONTEXT_H
#define JAWGRAPHCONTEXT_H

#include "JSystem/JUtility/TColor.h"
#include "JSystem/JGeometry.h"

class J2DPrint;
class JUTResFont;

class JAWGraphContext {
public:
    JAWGraphContext();
    ~JAWGraphContext();
    void reset();
    void color(u8, u8, u8, u8);
    void color(const JUtility::TColor&);
    void locate(int, int);
    void print(char const*, ...);
    void print(int, int, const char*, ...);
    void color(const JUtility::TColor&, const JUtility::TColor&, const JUtility::TColor&, const JUtility::TColor&);
    void fillBox(const JGeometry::TBox2<f32>&);
    void drawFrame(const JGeometry::TBox2<f32>&);
    void line(const JGeometry::TVec2<f32>&, const JGeometry::TVec2<f32>&);
    void setGXforPrint();
    void setGXforDraw();

    /* 0x00 */ J2DPrint* field_0x0;
    /* 0x04 */ JUtility::TColor field_0x4;
    /* 0x08 */ JUtility::TColor field_0x8;
    /* 0x0C */ JUtility::TColor field_0xc;
    /* 0x10 */ JUtility::TColor field_0x10;
    /* 0x14 */ u8 mParentAlpha;
    /* 0x15 */ u8 field_0x15;
    /* 0x16 */ u8 field_0x16;
    /* 0x18 */ int field_0x18;

    void setParentAlpha(u8 alpha) { mParentAlpha = alpha; }

    static JUTResFont* sFont;
    static bool lbl_8074CD30;
};

#endif /* JAWGRAPHCONTEXT_H */
