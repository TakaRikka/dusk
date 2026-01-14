#ifndef J2DWINDOW_H
#define J2DWINDOW_H

#include "JSystem/J2DGraph/J2DMaterial.h"
#include "JSystem/J2DGraph/J2DPane.h"

class JSURandomInputStream;
class JUTTexture;

enum J2DTextureBase {
    TEXTUREBASE_0 = 0,
    TEXTUREBASE_1 = 1,
    TEXTUREBASE_2 = 2,
    TEXTUREBASE_3 = 3,
};

enum J2DWindowMirror {
    WINDOWMIRROR_39 = 39,
    WINDOWMIRROR_114 = 114,
    WINDOWMIRROR_141 = 141,
    WINDOWMIRROR_216 = 216,
};

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
    J2DWindow(u64, const JGeometry::TBox2<f32>&, const char*, J2DTextureBase, const ResTLUT*);
    void initiate(const ResTIMG*, const ResTIMG*, const ResTIMG*, const ResTIMG*, const ResTLUT*, J2DWindowMirror, const JGeometry::TBox2<f32>&);
    void private_readStream(J2DPane*, JSURandomInputStream*, JKRArchive*);
    void initinfo();
    void initinfo2();
    static J2DWindowMirror convertMirror(J2DTextureBase);
    void draw_private(JGeometry::TBox2<f32> const&, JGeometry::TBox2<f32> const&);
    void setContentsColor(JUtility::TColor, JUtility::TColor, JUtility::TColor,
                                         JUtility::TColor);
    void drawFrameTexture(JUTTexture*, f32, f32, f32, f32, u16, u16, u16, u16, bool);
    void drawFrameTexture(JUTTexture*, f32, f32, bool, bool, bool);
    void drawContentsTexture(f32, f32, f32, f32);
    void setTevMode(JUTTexture*, JUtility::TColor, JUtility::TColor);

    virtual ~J2DWindow();
    /* vt 0x0C */ virtual u16 getTypeID() const { return 17; }
    /* vt 0x18 */ virtual void resize(f32, f32);
    /* vt 0x34 */ virtual void drawSelf(f32, f32);
    /* vt 0x38 */ virtual void drawSelf(f32, f32, Mtx*);
    /* vt 0x94 */ virtual void draw(JGeometry::TBox2<f32> const&);
    /* vt 0x98 */ virtual void draw(JGeometry::TBox2<f32> const&, JGeometry::TBox2<f32> const&);
    /* vt 0x9C */ virtual void draw(f32 left, f32 top, f32 width, f32 height) { draw(JGeometry::TBox2<f32>(left, top, left + width, top + height)); }
    /* vt 0xA0 */ virtual bool setBlack(JUtility::TColor);
    /* vt 0xA4 */ virtual bool setWhite(JUtility::TColor);
    /* vt 0xA8 */ virtual bool setBlackWhite(JUtility::TColor, JUtility::TColor);
    /* vt 0xAC */ virtual JUtility::TColor getBlack() const;
    /* vt 0xB0 */ virtual JUtility::TColor getWhite() const;
    /* vt 0xB4 */ virtual JUTTexture* getFrameTexture(u8, u8) const;
    /* vt 0xB8 */ virtual JUTTexture* getContentsTexture(u8 param_1) const {
        if (param_1 != 0) {
            return NULL;
        }
        return field_0x110;
    }
    /* vt 0xBC */ virtual void getMaterial(J2DWindow::TMaterial& mat) const {
        mat.field_0x0 = NULL;
        mat.field_0x4 = NULL;
        mat.field_0x8 = NULL;
        mat.field_0xc = NULL;
        mat.field_0x10 = NULL;
    }
    /* vt 0xC0 */ virtual J2DMaterial* getFrameMaterial(u8) const { return NULL; }
    /* vt 0xC4 */ virtual J2DMaterial* getContentsMaterial() const { return NULL; }
    /* vt 0xC8 */ virtual void drawContents(JGeometry::TBox2<f32> const&);
    /* vt 0x4C */ virtual bool isUsed(const ResTIMG*);
    /* vt 0x50 */ virtual bool isUsed(const ResFONT* font) { return J2DPane::isUsed(font); }
    /* vt 0x58 */ virtual void rewriteAlpha() {}

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
