#ifndef J2DPANE_H
#define J2DPANE_H

#include "JSystem/JGeometry.h"
#include "JSystem/JSupport/JSUList.h"
#include <dolphin/gx.h>
#include "dolphin/mtx.h"

class J2DAnmBase;
class J2DAnmColor;
class J2DAnmTransform;
class J2DAnmTevRegKey;
class J2DAnmTextureSRTKey;
class J2DAnmVisibilityFull;
class J2DAnmVtxColor;
class J2DGrafContext;
class J2DScreen;
class J2DAnmTexPattern;
class JKRArchive;
class JSURandomInputStream;
struct ResFONT;
struct ResTIMG;

enum J2DRotateAxis {
    /* 0x78 */ ROTATE_X = 'x',
    /* 0x79 */ ROTATE_Y = 'y',
    /* 0x7A */ ROTATE_Z = 'z'
};

enum J2DBasePosition {
    J2DBasePosition_0,
    J2DBasePosition_1,
    J2DBasePosition_2,
    J2DBasePosition_3,
    J2DBasePosition_4,
};

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DPaneHeader {
    /* 0x0 */ u32 mKind;
    /* 0x4 */ u32 mSize;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DPane {
public:
    J2DPane();
    J2DPane(J2DPane*, bool, u64, const JGeometry::TBox2<f32>&);
    J2DPane(u64, const JGeometry::TBox2<f32>&);
    J2DPane(J2DPane* other, JSURandomInputStream* stream, u8 arg3);
    void initiate();
    void initialize(J2DPane*, bool, u64, const JGeometry::TBox2<f32>&);
    void initialize(u64 tag, const JGeometry::TBox2<f32>& dim);
    void makePaneStream(J2DPane* other, JSURandomInputStream* stream);
    void changeUseTrans(J2DPane* other);
    bool appendChild(J2DPane* child);
    bool insertChild(J2DPane* before, J2DPane* child);
    void draw(f32 a1, f32 a2, const J2DGrafContext* ctx, bool a4, bool a5);
    void place(const JGeometry::TBox2<f32>& dim);
    JGeometry::TBox2<f32>& getBounds();
    void rotate(f32 offsetX, f32 offsetY, J2DRotateAxis axis, f32 angle);
    void rotate(f32 angle);
    void clip(const JGeometry::TBox2<f32>& bounds);
    void setBasePosition(J2DBasePosition position);
    void setInfluencedAlpha(bool arg1, bool arg2);
    Vec getGlbVtx(u8 arg1) const;
    J2DPane* getFirstChildPane();
    J2DPane* getNextChildPane();
    J2DPane* getParentPane();
    void makePaneExStream(J2DPane* other, JSURandomInputStream* stream);
    void* getPointer(JSURandomInputStream* stream, u32 size, JKRArchive* archive);
    void animationTransform();
    void updateTransform(const J2DAnmTransform* transform);

    virtual ~J2DPane();

    /* vt 0x0C */ virtual u16 getTypeID() const { return 16; }
    /* vt 0x10 */ virtual void move(f32 x, f32 y);
    /* vt 0x14 */ virtual void add(f32 x, f32 y);
    /* vt 0x18 */ virtual void resize(f32 x, f32 y);
    /* vt 0x1C */ virtual void setCullBack(bool cull);
    /* vt 0x20 */ virtual void setCullBack(_GXCullMode cmode);
    /* vt 0x24 */ virtual void setAlpha(u8 alpha) {
        mAlpha = alpha;
    };
    /* vt 0x28 */ virtual bool setConnectParent(bool connected);
    /* vt 0x2C */ virtual void calcMtx() {
        if (mPaneTree.getParent() != NULL) {
            makeMatrix(mTranslateX, mTranslateY);
        }
    }
    /* vt 0x30 */ virtual void update();
    /* vt 0x34 */ virtual void drawSelf(f32 arg1, f32 arg2);
    /* vt 0x38 */ virtual void drawSelf(f32 arg1, f32 arg2, Mtx* mtx);
    /* vt 0x3C */ virtual J2DPane* search(u64 tag);
    /* vt 0x40 */ virtual J2DPane* searchUserInfo(u64 tag);
    /* vt 0x44 */ virtual void makeMatrix(f32 a, f32 b) {
        makeMatrix(a, b, -mBounds.i.x, -mBounds.i.y);
    }
    /* vt 0x48 */ virtual void makeMatrix(f32 a, f32 b, f32 c, f32 d);
    /* vt 0x4C */ virtual bool isUsed(const ResTIMG* timg);
    /* vt 0x50 */ virtual bool isUsed(const ResFONT* font);
    /* vt 0x54 */ virtual void clearAnmTransform();
    /* vt 0x58 */ virtual void rewriteAlpha();
    /* vt 0x5C */ virtual void setAnimation(J2DAnmBase* anm);
    /* vt 0x60 */ virtual void setAnimation(J2DAnmTransform* anm);
    /* vt 0x64 */ virtual void setAnimation(J2DAnmColor* anm) {}
    /* vt 0x68 */ virtual void setAnimation(J2DAnmTexPattern* anm) {}
    /* vt 0x6C */ virtual void setAnimation(J2DAnmTextureSRTKey* anm) {}
    /* vt 0x70 */ virtual void setAnimation(J2DAnmTevRegKey* anm) {}
    /* vt 0x74 */ virtual void setAnimation(J2DAnmVisibilityFull* anm) {}
    /* vt 0x78 */ virtual void setAnimation(J2DAnmVtxColor* anm) {}
    /* vt 0x7C */ virtual const J2DAnmTransform* animationTransform(const J2DAnmTransform* transform);
    /* vt 0x80 */ virtual void setVisibileAnimation(J2DAnmVisibilityFull* visibility);
    /* vt 0x84 */ virtual void setAnimationVF(J2DAnmVisibilityFull* visibility);
    /* vt 0x88 */ virtual void setVtxColorAnimation(J2DAnmVtxColor* vtx_color);
    /* vt 0x8C */ virtual void setAnimationVC(J2DAnmVtxColor* vtx_color);
    /* vt 0x90 */ virtual const J2DAnmTransform* animationPane(const J2DAnmTransform* transform);

    f32 getHeight() const { return mBounds.getHeight(); }
    f32 getWidth() const { return mBounds.getWidth(); }
    JSUTree<J2DPane>* getFirstChild() { return mPaneTree.getFirstChild(); }
    JSUTree<J2DPane>* getEndChild() { return mPaneTree.getEndChild(); }
    const JSUTree<J2DPane>* getPaneTree() { return &mPaneTree; }
    u8 getAlpha() const { return mAlpha; }

    f32 getScaleX() const { return mScaleX; }

    f32 getScaleY() const { return mScaleY; }

    f32 getTranslateX() const { return mTranslateX; }

    f32 getTranslateY() const { return mTranslateY; }

    f32 getRotateZ() const { return mRotateZ; }

    f32 getRotOffsetX() const { return mRotateOffsetX; }

    f32 getRotOffsetY() const { return mRotateOffsetY; }

    MtxP getGlbMtx() { return mGlobalMtx; }

    const JGeometry::TBox2<f32>& getGlbBounds() const { return mGlobalBounds; }

    void setMtx(Mtx m) { MTXCopy(m, mPositionMtx); }

    void translate(f32 transX, f32 transY) {
        mTranslateX = transX;
        mTranslateY = transY;
        calcMtx();
    }

    void scale(f32 scaleH, f32 scaleV) {
        mScaleX = scaleH;
        mScaleY = scaleV;
        calcMtx();
    }

    void rotateY(f32 rotate) {
        mRotateY = rotate;
        calcMtx();
    }

    int getKind() const { return mKind; }

    bool isVisible() const { return mVisible; }

    void show() { mVisible = true; }
    void hide() { mVisible = false; }

    bool isConnectParent() const { return mConnected; }

    u64 getUserInfo() const { return mUserInfoTag; }
    void setUserInfo(u64 info) { mUserInfoTag = info; }
    const Mtx* getMtx() const { return &mPositionMtx; }

    static s16 J2DCast_F32_to_S16(f32 value, u8 arg2);

    static JGeometry::TBox2<f32> static_mBounds;

public:
    /* 0x04 */ u16 field_0x4;
    /* 0x08 */ int mKind;
    /* 0x10 */ u64 mInfoTag;
    /* 0x18 */ u64 mUserInfoTag;
    /* 0x20 */ JGeometry::TBox2<f32> mBounds;
    /* 0x30 */ JGeometry::TBox2<f32> mGlobalBounds;
    /* 0x40 */ JGeometry::TBox2<f32> mClipRect;
    /* 0x50 */ Mtx mPositionMtx;
    /* 0x80 */ Mtx mGlobalMtx;
    /* 0xB0 */ bool mVisible;
    /* 0xB1 */ u8 mCullMode;
    /* 0xB2 */ u8 mAlpha;
    /* 0xB3 */ u8 mColorAlpha;
    /* 0xB4 */ bool mIsInfluencedAlpha;
    /* 0xB5 */ bool mConnected;
    /* 0xB6 */ char mRotAxis;
    /* 0xB7 */ u8 mBasePosition;
    /* 0xB8 */ f32 mRotateX;
    /* 0xBC */ f32 mRotateY;
    /* 0xC0 */ f32 mRotateZ;
    /* 0xC4 */ f32 mRotateOffsetX;
    /* 0xC8 */ f32 mRotateOffsetY;
    /* 0xCC */ f32 mScaleX;
    /* 0xD0 */ f32 mScaleY;
    /* 0xD4 */ f32 mTranslateX;
    /* 0xD8 */ f32 mTranslateY;
    /* 0xDC */ JSUTree<J2DPane> mPaneTree;
    /* 0xF8 */ const J2DAnmTransform* mTransform;

    #if !(PLATFORM_WII || PLATFORM_SHIELD)
    /* 0xFC */ u32 _fc;
    #endif
};

#endif /* J2DPANE_H */
