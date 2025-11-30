#ifndef J2DSCREEN_H
#define J2DSCREEN_H

#include "JSystem/J2DGraph/J2DManage.h"
#include "JSystem/J2DGraph/J2DPane.h"
#include "JSystem/JUtility/TColor.h"

class J2DMaterial;
class JUTNameTab;

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DScrnHeader {
    /* 0x00 */ u32 mTag;
    /* 0x04 */ u32 mType;
    /* 0x08 */ u32 mFileSize;
    /* 0x0C */ u32 mBlockNum;
    /* 0x10 */ u8 padding[0x10];
};

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DScrnInfoHeader {
    /* 0x0 */ u32 mTag;
    /* 0x4 */ u32 mSize;
    /* 0x8 */ u16 mWidth;
    /* 0xA */ u16 mHeight;
    /* 0xC */ u32 mColor;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DScrnBlockHeader {
    /* 0x00 */ u32 mTag;
    /* 0x04 */ s32 mSize;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DScreen : public J2DPane {
public:
    virtual ~J2DScreen();
    virtual u16 getTypeID() const;
    virtual void calcMtx();
    virtual void drawSelf(f32, f32, Mtx*);
    virtual J2DPane* search(u64);
    virtual J2DPane* searchUserInfo(u64);
    virtual bool isUsed(ResTIMG const*);
    virtual bool isUsed(ResFONT const*);
    virtual void clearAnmTransform() { J2DPane::clearAnmTransform(); }
    virtual void setAnimation(J2DAnmColor*);
    virtual void setAnimation(J2DAnmTransform* i_bck) { J2DPane::setAnimation(i_bck); }
    virtual void setAnimation(J2DAnmTextureSRTKey*);
    virtual void setAnimation(J2DAnmVtxColor*);
    virtual void setAnimation(J2DAnmTexPattern*);
    virtual void setAnimation(J2DAnmVisibilityFull*);
    virtual void setAnimation(J2DAnmTevRegKey*);
    virtual void setAnimation(J2DAnmBase*);
    virtual void setAnimationVF(J2DAnmVisibilityFull*);
    virtual void setAnimationVC(J2DAnmVtxColor*);
    virtual J2DPane* createPane(J2DScrnBlockHeader const&, JSURandomInputStream*,
                                               J2DPane*, u32);
    virtual J2DPane* createPane(J2DScrnBlockHeader const&, JSURandomInputStream*,
                                               J2DPane*, u32, JKRArchive*);

    J2DScreen();
    void clean();
    bool setPriority(char const*, u32, JKRArchive*);
    bool setPriority(JSURandomInputStream*, u32, JKRArchive*);
    bool private_set(JSURandomInputStream*, u32, JKRArchive*);
    bool checkSignature(JSURandomInputStream*);
    bool getScreenInformation(JSURandomInputStream*);
    s32 makeHierarchyPanes(J2DPane*, JSURandomInputStream*, u32, JKRArchive*);
    void draw(f32, f32, J2DGrafContext const*);
    J2DResReference* getResReference(JSURandomInputStream*, u32);
    bool createMaterial(JSURandomInputStream*, u32, JKRArchive*);
    static void* getNameResource(char const*);
    void animation();

    void setScissor(bool i_scissor) { mScissor = i_scissor; }

    static J2DDataManage* getDataManage() { return mDataManage; }

    static J2DDataManage* mDataManage;

    /* 0x100 */ bool mScissor;
    /* 0x102 */ u16 mMaterialNum;
    /* 0x104 */ J2DMaterial* mMaterials;
    /* 0x108 */ J2DResReference* mTexRes;
    /* 0x10C */ J2DResReference* mFontRes;
    /* 0x110 */ JUTNameTab* mNameTable;
    /* 0x114 */ JUtility::TColor mColor;
};

#endif /* J2DSCREEN_H */
