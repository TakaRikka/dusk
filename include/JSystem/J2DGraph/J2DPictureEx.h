#ifndef J2DPICTUREEX_H
#define J2DPICTUREEX_H

#include "JSystem/J2DGraph/J2DPicture.h"
#include "JSystem/J2DGraph/J2DTevs.h"

class J2DMaterial;

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DPictureEx : public J2DPicture {
public:
    enum stage_enum {
        STAGE_0,
        STAGE_1,
        STAGE_2,
        STAGE_3,
        STAGE_4,
        STAGE_5,
        STAGE_6,
        STAGE_7,
    };

    J2DPictureEx(J2DPane*, JSURandomInputStream*, u32, J2DMaterial*);
    void insertCommon(u8, f32);
    bool isInsert(u8) const;
    bool isRemove(u8) const;
    void setTevOrder(u8, u8, bool);
    void setTevStage(u8, u8, bool);
    void setStage(J2DTevStage*, J2DPictureEx::stage_enum);
    void setTevKColor(u8);
    void setTevKColorSel(u8);
    void setTevKAlphaSel(u8);
    void shiftSetBlendRatio(u8, f32, bool, bool);
    bool getBlackWhite(JUtility::TColor*, JUtility::TColor*) const;
    bool isSetBlackWhite(JUtility::TColor, JUtility::TColor) const;

    virtual ~J2DPictureEx();
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
    virtual void initiate(ResTIMG const*, ResTLUT const*);
    virtual bool prepareTexture(u8);
    virtual bool append(ResTIMG const*, f32);
    virtual bool append(ResTIMG const*, JUTPalette*, f32);
    virtual bool append(char const*, f32);
    virtual bool append(char const*, JUTPalette*, f32);
    virtual bool append(JUTTexture*, f32);
    virtual void prepend(ResTIMG const*, f32);
    virtual void prepend(ResTIMG const*, JUTPalette*, f32);
    virtual void prepend(char const*, f32);
    virtual void prepend(char const*, JUTPalette*, f32);
    virtual void prepend(JUTTexture*, f32);
    virtual bool insert(ResTIMG const*, u8, f32);
    virtual bool insert(ResTIMG const*, JUTPalette*, u8, f32);
    virtual bool insert(char const*, u8, f32);
    virtual bool insert(char const*, JUTPalette*, u8, f32);
    virtual bool insert(JUTTexture*, u8, f32);
    virtual bool remove(u8);
    virtual bool remove();
    virtual bool remove(JUTTexture*);
    virtual void draw(f32, f32, bool, bool, bool);
    virtual void draw(f32, f32, u8, bool, bool, bool);
    virtual void draw(f32, f32, f32, f32, bool, bool, bool);
    virtual void drawOut(f32, f32, f32, f32, f32, f32);
    virtual void drawOut(f32, f32, f32, f32, f32, f32, f32, f32);
    virtual void drawOut(JGeometry::TBox2<f32> const&, JGeometry::TBox2<f32> const&);
    virtual void load(_GXTexMapID, u8);
    virtual void load(u8);
    virtual void setBlendColorRatio(f32, f32);
    virtual void setBlendAlphaRatio(f32, f32);
    virtual const ResTIMG* changeTexture(ResTIMG const*, u8);
    virtual const ResTIMG* changeTexture(char const*, u8);
    virtual const ResTIMG* changeTexture(ResTIMG const*, u8, JUTPalette*);
    virtual const ResTIMG* changeTexture(char const*, u8, JUTPalette*);
    virtual JUTTexture* getTexture(u8) const;
    virtual u8 getTextureCount() const;
    virtual bool setBlack(JUtility::TColor);
    virtual bool setWhite(JUtility::TColor);
    virtual bool setBlackWhite(JUtility::TColor, JUtility::TColor);
    virtual JUtility::TColor getBlack() const;
    virtual JUtility::TColor getWhite() const;
    virtual J2DMaterial* getMaterial() const { return mMaterial; }
    virtual void drawFullSet(f32, f32, f32, f32, f32 (*)[3][4]);
    virtual void drawTexCoord(f32, f32, f32, f32, s16, s16, s16, s16, s16, s16, s16,
                                             s16, f32 (*)[3][4]);
    virtual u8 getUsableTlut(u8);

private:
    /* 0x150 */ J2DMaterial* mMaterial;
    /* 0x154 */ u16 field_0x154;
    /* 0x156 */ u16 field_0x156;
    /* 0x158 */ u16 field_0x158[4];
    /* 0x160 */ f32 field_0x160[6];  // related to blend color ratio
    /* 0x178 */ f32 field_0x178[6];  // related to blend alpha ratio
    /* 0x190 */ u8 field_0x190;
    /* 0x194 */ J2DAnmVisibilityFull* field_0x194;
    /* 0x198 */ J2DAnmVtxColor* field_0x198;
    /* 0x19C */ u8 field_0x19c;
};

#endif /* J2DPICTUREEX_H */
