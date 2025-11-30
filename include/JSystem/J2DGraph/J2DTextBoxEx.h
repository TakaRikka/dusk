#ifndef J2DTEXTBOXEX_H
#define J2DTEXTBOXEX_H

#include "JSystem/J2DGraph/J2DTextBox.h"

class J2DMaterial;

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DTextBoxEx : public J2DTextBox {
public:
    enum stage_enum {
        STAGE_0,
        STAGE_1,
        STAGE_2,
    };

    J2DTextBoxEx(J2DPane*, JSURandomInputStream*, u32, J2DMaterial*);
    void setTevOrder(bool);
    void setTevStage(bool);
    void setStage(J2DTevStage*, J2DTextBoxEx::stage_enum);
    bool getBlackWhite(JUtility::TColor*, JUtility::TColor*) const;
    bool isSetBlackWhite(JUtility::TColor, JUtility::TColor) const;

    virtual ~J2DTextBoxEx();
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
    virtual void draw(f32, f32);
    virtual void draw(f32, f32, f32, J2DTextBoxHBinding);
    virtual void setFont(JUTFont*);
    virtual JUTFont* getFont() const;
    virtual bool setBlack(JUtility::TColor);
    virtual bool setWhite(JUtility::TColor);
    virtual bool setBlackWhite(JUtility::TColor, JUtility::TColor);
    virtual JUtility::TColor getBlack() const;
    virtual JUtility::TColor getWhite() const;
    virtual J2DMaterial* getMaterial() const { return mMaterial; }

private:
    /* 0x138 */ J2DMaterial* mMaterial;
    /* 0x13C */ u16 field_0x13c;
    /* 0x13E */ u16 field_0x13e;
    /* 0x140 */ u8 field_0x140;
    /* 0x144 */ J2DAnmVisibilityFull* mVisibilityAnm;
};  // Size: 0x148

#endif /* J2DTEXTBOXEX_H */
