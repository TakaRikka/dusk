#ifndef J2DGRAFCONTEXT_H
#define J2DGRAFCONTEXT_H

#include "JSystem/JGeometry.h"
#include "JSystem/JUtility/TColor.h"
#include "dolphin/mtx.h"

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DGrafContext {
public:
    struct Blend {
        /* 0x0 */ u8 mType;
        /* 0x1 */ u8 mSrcFactor;
        /* 0x2 */ u8 mDstFactor;
    };


    J2DGrafContext(f32 x, f32 y, f32 width, f32 height);
    void scissor(JGeometry::TBox2<f32> const& bounds);
    void scissor(f32 x, f32 y, f32 width, f32 height) {
        scissor(JGeometry::TBox2<f32>(x, y, x + width, y + height));
    }

    void setColor(JUtility::TColor c) { this->setColor(c, c, c, c); }
    void setColor(JUtility::TColor colorTL, JUtility::TColor colorTR,
                                 JUtility::TColor colorBR, JUtility::TColor colorBL);
    void setLineWidth(u8);
    void fillBox(JGeometry::TBox2<f32> const& box);
    void drawFrame(JGeometry::TBox2<f32> const& box);
    void line(JGeometry::TVec2<f32> start, JGeometry::TVec2<f32> end);
    void lineTo(JGeometry::TVec2<f32> pos);
    void lineTo(f32 x, f32 y) { this->lineTo(JGeometry::TVec2<f32>(x, y)); }
    void moveTo(f32 x, f32 y) { this->moveTo(JGeometry::TVec2<f32>(x, y)); }

    void moveTo(JGeometry::TVec2<f32> pos) { mPrevPos = pos; }

    virtual ~J2DGrafContext() {}
    virtual void place(JGeometry::TBox2<f32> const& bounds);
    virtual void place(f32 x, f32 y, f32 width, f32 height) {
        JGeometry::TBox2<f32> box(x, y, x + width, y + height);
        this->place(box);
    }
    virtual void setPort();
    virtual void setup2D();
    virtual void setScissor();
    virtual s32 getGrafType() const { return 0; }
    virtual void setLookat() {}

public:
    /* 0x04 */ JGeometry::TBox2<f32> mBounds;
    /* 0x14 */ JGeometry::TBox2<f32> mScissorBounds;
    /* 0x24 */ JUtility::TColor mColorTL;
    /* 0x28 */ JUtility::TColor mColorTR;
    /* 0x2C */ JUtility::TColor mColorBR;
    /* 0x30 */ JUtility::TColor mColorBL;
    /* 0x34 */ u8 mLineWidth;
    /* 0x38 */ JGeometry::TVec2<f32> mPrevPos;
    /* 0x40 */ Mtx44 mMtx44;
    /* 0x80 */ Mtx mPosMtx;
    /* 0xB0 */ Blend field_0xb0;
    /* 0xB3 */ Blend mLinePart;
    /* 0xB6 */ Blend mBoxPart;
};

#endif /* J2DGRAFCONTEXT_H */
