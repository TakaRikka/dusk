#ifndef J2DWINDOW_H
#define J2DWINDOW_H

#include "JSystem/J2DGraph/J2DMaterial.h"
#include "JSystem/J2DGraph/J2DPane.h"

class JSURandomInputStream;
class JUTTexture;

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DWindow : public J2DPane {
public:
    struct TMaterial {
        /* 0x00 */ J2DMaterial* field_0x0;
        /* 0x04 */ J2DMaterial* field_0x4;
        /* 0x08 */ J2DMaterial* field_0x8;
        /* 0x0C */ J2DMaterial* field_0xc;
        /* 0x10 */ J2DMaterial* field_0x10;
    };  // Size: 0x14

    struct TContentsColor {
        TContentsColor() {}
        /* 0x0 */ JUtility::TColor field_0x0;
        /* 0x4 */ JUtility::TColor field_0x4;
        /* 0x8 */ JUtility::TColor field_0x8;
        /* 0xC */ JUtility::TColor field_0xc;
    };  // Size: 0x10

    J2DWindow(J2DPane*, JSURandomInputStream*, J2DMaterial*);
    J2DWindow(J2DPane*, JSURandomInputStream*, JKRArchive*);
    J2DWindow();
    void private_readStream(J2DPane*, JSURandomInputStream*, JKRArchive*);
    void initinfo2();
    void draw_private(JGeometry::TBox2<f32> const&, JGeometry::TBox2<f32> const&);
    void setContentsColor(JUtility::TColor, JUtility::TColor, JUtility::TColor,
                                         JUtility::TColor);
    void drawFrameTexture(JUTTexture*, f32, f32, f32, f32, u16, u16, u16, u16, bool);
    void drawFrameTexture(JUTTexture*, f32, f32, bool, bool, bool);
    void drawContentsTexture(f32, f32, f32, f32);
    void setTevMode(JUTTexture*, JUtility::TColor, JUtility::TColor);

    virtual ~J2DWindow();
    virtual u16 getTypeID() const;
    virtual void resize(f32, f32);
    virtual void drawSelf(f32, f32);
    virtual void drawSelf(f32, f32, Mtx*);
    virtual bool isUsed(ResTIMG const*);
    virtual bool isUsed(ResFONT const*);
    virtual void rewriteAlpha();
    virtual void draw(JGeometry::TBox2<f32> const&);
    virtual void draw(JGeometry::TBox2<f32> const&, JGeometry::TBox2<f32> const&);
    virtual void draw(f32, f32, f32, f32);
    virtual bool setBlack(JUtility::TColor);
    virtual bool setWhite(JUtility::TColor);
    virtual bool setBlackWhite(JUtility::TColor, JUtility::TColor);
    virtual JUtility::TColor getBlack() const;
    virtual JUtility::TColor getWhite() const;
    virtual JUTTexture* getFrameTexture(u8, u8) const;
    virtual JUTTexture* getContentsTexture(u8) const;
    virtual void getMaterial(J2DWindow::TMaterial&) const;
    virtual J2DMaterial* getFrameMaterial(u8) const;
    virtual J2DMaterial* getContentsMaterial() const;
    virtual void drawContents(JGeometry::TBox2<f32> const&);

    bool isField0x145Set(u8 flag) { return field_0x145 & flag; }

    void getContentsColor(TContentsColor& param_0) const {
        param_0.field_0x0 = field_0x128;
        param_0.field_0x4 = field_0x12C;
        param_0.field_0x8 = field_0x130;
        param_0.field_0xc = field_0x134;
    }
    void setContentsColor(JUtility::TColor param_0) {
        setContentsColor(param_0, param_0, param_0, param_0);
    }

protected:
    /* 0x100 */ JUTTexture* field_0x100;
    /* 0x104 */ JUTTexture* field_0x104;
    /* 0x108 */ JUTTexture* field_0x108;
    /* 0x10C */ JUTTexture* field_0x10c;
    /* 0x110 */ JUTTexture* field_0x110;
    /* 0x114 */ JGeometry::TBox2<f32> field_0x114;
    /* 0x124 */ JUTPalette* mPalette;
    /* 0x128 */ JUtility::TColor field_0x128;
    /* 0x12C */ JUtility::TColor field_0x12C;
    /* 0x130 */ JUtility::TColor field_0x130;
    /* 0x134 */ JUtility::TColor field_0x134;
    /* 0x138 */ JUtility::TColor mWhite;
    /* 0x13C */ JUtility::TColor mBlack;
    /* 0x140 */ s16 field_0x140;
    /* 0x142 */ s16 field_0x142;
    /* 0x144 */ u8 field_0x144;
    /* 0x145 */ u8 field_0x145;
};  // Size: 0x148

#endif /* J2DWINDOW_H */
