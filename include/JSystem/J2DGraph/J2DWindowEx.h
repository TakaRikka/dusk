#ifndef J2DWINDOWEX_H
#define J2DWINDOWEX_H

#include "JSystem/J2DGraph/J2DWindow.h"

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DWindowEx : public J2DWindow {
public:
    enum stage_enum {
        STAGE_ENUM_0,
        STAGE_ENUM_1,
        STAGE_ENUM_2,
        STAGE_ENUM_3,
        STAGE_ENUM_4,
    };

    J2DWindowEx(J2DPane*, JSURandomInputStream*, u32, J2DMaterial*);
    void setMinSize();
    void draw_private(JGeometry::TBox2<f32> const&, JGeometry::TBox2<f32> const&);
    void drawFrameTexture(f32, f32, f32, f32, u16, u16, u16, u16, J2DMaterial*,
                                         bool);
    void setTevOrder(bool);
    void setTevStage(bool);
    void setStage(J2DTevStage*, J2DWindowEx::stage_enum);
    bool getBlackWhite(JUtility::TColor*, JUtility::TColor*) const;
    bool isSetBlackWhite(JUtility::TColor, JUtility::TColor) const;
    bool isNeedSetAnm(u8);

    virtual ~J2DWindowEx();
    virtual void setCullBack(bool);
    virtual void setCullBack(_GXCullMode);
    virtual void setAlpha(u8);
    virtual void drawSelf(f32, f32, f32 (*)[3][4]);
    virtual bool isUsed(ResTIMG const*);
    virtual bool isUsed(ResFONT const*);
    virtual void rewriteAlpha();
    virtual void setAnimation(J2DAnmBase*);
    virtual void setAnimation(J2DAnmTransform* transform) { J2DPane::setAnimation(transform); }
    virtual void setAnimation(J2DAnmColor*);
    virtual void setAnimation(J2DAnmTexPattern*);
    virtual void setAnimation(J2DAnmTextureSRTKey*);
    virtual void setAnimation(J2DAnmTevRegKey*);
    virtual void setAnimation(J2DAnmVisibilityFull*);
    virtual void setAnimation(J2DAnmVtxColor*);
    virtual const J2DAnmTransform* animationPane(J2DAnmTransform const*);
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

private:
    /* 0x148 */ J2DMaterial* mFrameMaterial[4];
    /* 0x158 */ u16 field_0x158[4];
    /* 0x160 */ J2DMaterial* mContentsMaterial;
    /* 0x164 */ u16 field_0x164;
    /* 0x166 */ u16 field_0x166;
    /* 0x168 */ u16 field_0x168[4];
    /* 0x170 */ u8 field_0x170;
    /* 0x174 */ J2DAnmVisibilityFull* mAnmVisibilityFull;
    /* 0x178 */ J2DAnmVtxColor* mAnmVtxColor;
    /* 0x17C */ u8 field_0x17c;
};

#endif /* J2DWINDOWEX_H */
