#ifndef J3DMATERIALANM_H
#define J3DMATERIALANM_H

#include "JSystem/J3DGraphAnimator/J3DAnimation.h"

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DMatColorAnm {
public:
    ~J3DMatColorAnm() {}

    J3DMatColorAnm() : field_0x0(0), mAnmFlag(1), mAnmColor(NULL) {}

    J3DMatColorAnm(u16 param_1, J3DAnmColor* pAnmColor) {
        field_0x0 = param_1;
        mAnmFlag = 1;
        mAnmColor = pAnmColor;
        J3D_ASSERT_NULLPTR(56, pAnmColor != NULL);
    }

    void operator=(J3DMatColorAnm const& other) {
        mAnmColor = other.mAnmColor;
        field_0x0 = other.field_0x0;
        mAnmFlag = other.mAnmFlag;
    }

    void setAnmFlag(bool flag) { mAnmFlag = flag; }
    bool getAnmFlag() const { return mAnmFlag; }

    void calc(GXColor* pColor) const {
        J3D_ASSERT_NULLPTR(507, pColor != NULL);
        mAnmColor->getColor(field_0x0, pColor);
    }

private:
    /* 0x0 */ u16 field_0x0;
    /* 0x2 */ u16 mAnmFlag;
    /* 0x4 */ J3DAnmColor* mAnmColor;
};  // Size: 0x8

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DTexMtxAnm {
public:
    ~J3DTexMtxAnm() {}
    J3DTexMtxAnm() : field_0x0(0), mAnmFlag(1), mAnmTransform(NULL) {}

    J3DTexMtxAnm(u16 param_1, J3DAnmTextureSRTKey* pSRTKey) {
        field_0x0 = param_1;
        mAnmFlag = 1;
        mAnmTransform = pSRTKey;
        J3D_ASSERT_NULLPTR(134, pSRTKey != NULL);
    }

    void operator=(J3DTexMtxAnm const& other) {
        mAnmTransform = other.mAnmTransform;
        field_0x0 = other.field_0x0;
        mAnmFlag = other.mAnmFlag;
    }

    void setAnmFlag(bool flag) { mAnmFlag = flag; }

    void calc(J3DTextureSRTInfo* pSRTInfo) const {
        J3D_ASSERT_NULLPTR(519, pSRTInfo != NULL);
        mAnmTransform->getTransform(field_0x0, pSRTInfo);
    }

    bool getAnmFlag() const { return mAnmFlag; }

private:
    /* 0x0 */ u16 field_0x0;
    /* 0x2 */ u16 mAnmFlag;
    /* 0x4 */ J3DAnmTextureSRTKey* mAnmTransform;
};  // Size: 0x8

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DTexNoAnm {
public:
    ~J3DTexNoAnm() {}
    J3DTexNoAnm() : field_0x4(0), mAnmFlag(1), mAnmTexPattern(NULL) {}

    J3DTexNoAnm(u16 param_1, J3DAnmTexPattern* pAnmTexPattern) {
        field_0x4 = param_1;
        mAnmFlag = 1;
        mAnmTexPattern = pAnmTexPattern;
        J3D_ASSERT_NULLPTR(214, pAnmTexPattern != NULL);
    }

    virtual void calc(u16* param_0) const { mAnmTexPattern->getTexNo(field_0x4, param_0); }

    void operator=(J3DTexNoAnm const& other) {
        mAnmTexPattern = other.mAnmTexPattern;
        field_0x4 = other.field_0x4;
        mAnmFlag = other.mAnmFlag;
    }

    void setAnmFlag(bool flag) { mAnmFlag = flag; }
    bool getAnmFlag() const { return mAnmFlag; }
    J3DAnmTexPattern* getAnmTexPattern() { return mAnmTexPattern; }

private:
    /* 0x4 */ u16 field_0x4;
    /* 0x6 */ u16 mAnmFlag;
    /* 0x8 */ J3DAnmTexPattern* mAnmTexPattern;
};  // Size: 0xC

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DTevColorAnm {
public:
    ~J3DTevColorAnm() {}
    J3DTevColorAnm() : field_0x0(0), mAnmFlag(1), mAnmTevReg(NULL) {}

    J3DTevColorAnm(u16 param_1, J3DAnmTevRegKey* pTevRegKey) {
        field_0x0 = param_1;
        mAnmFlag = 1;
        mAnmTevReg = pTevRegKey;
        J3D_ASSERT_NULLPTR(293, pTevRegKey != NULL);
    }

    void operator=(J3DTevColorAnm const& other) {
        mAnmTevReg = other.mAnmTevReg;
        field_0x0 = other.field_0x0;
        mAnmFlag = other.mAnmFlag;
    }

    void setAnmFlag(bool flag) { mAnmFlag = flag; }
    bool getAnmFlag() const { return mAnmFlag; }

    void calc(GXColorS10* pColor) const {
        J3D_ASSERT_NULLPTR(545, pColor != NULL);
        mAnmTevReg->getTevColorReg(field_0x0, pColor);
    }

private:
    /* 0x0 */ u16 field_0x0;
    /* 0x2 */ u16 mAnmFlag;
    /* 0x4 */ J3DAnmTevRegKey* mAnmTevReg;
};  // Size: 0x8

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DTevKColorAnm {
public:
    ~J3DTevKColorAnm() {}
    J3DTevKColorAnm() : field_0x0(0), mAnmFlag(1), mAnmTevReg(NULL) {}

    J3DTevKColorAnm(u16 param_1, J3DAnmTevRegKey* pTevRegKey) {
        field_0x0 = param_1;
        mAnmFlag = 1;
        mAnmTevReg = pTevRegKey;
        J3D_ASSERT_NULLPTR(371, pTevRegKey != NULL);
    }

    void operator=(J3DTevKColorAnm const& other) {
        mAnmTevReg = other.mAnmTevReg;
        field_0x0 = other.field_0x0;
        mAnmFlag = other.mAnmFlag;
    }

    void setAnmFlag(bool flag) { mAnmFlag = flag; }
    bool getAnmFlag() const { return mAnmFlag; }

    void calc(GXColor* pColor) const {
        J3D_ASSERT_NULLPTR(558, pColor != NULL);
        mAnmTevReg->getTevKonstReg(field_0x0, pColor);
    }

private:
    /* 0x0 */ u16 field_0x0;
    /* 0x2 */ u16 mAnmFlag;
    /* 0x4 */ J3DAnmTevRegKey* mAnmTevReg;
};  // Size: 0x8

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DMaterialAnm {
public:
    J3DMaterialAnm() { initialize(); }

    void initialize();
    void setMatColorAnm(int, J3DMatColorAnm*);
    void setTexMtxAnm(int, J3DTexMtxAnm*);
    void setTexNoAnm(int, J3DTexNoAnm*);
    void setTevColorAnm(int, J3DTevColorAnm*);
    void setTevKColorAnm(int, J3DTevKColorAnm*);

    virtual ~J3DMaterialAnm() {}
    virtual void calc(J3DMaterial*) const;

    const J3DTexMtxAnm& getTexMtxAnm(int i) const { return mTexMtxAnm[i]; }

private:
    /* 0x04 */ J3DMatColorAnm mMatColorAnm[2];
    /* 0x14 */ J3DTexMtxAnm mTexMtxAnm[8];
    /* 0x54 */ J3DTexNoAnm mTexNoAnm[8];
    /* 0xB4 */ J3DTevColorAnm mTevColorAnm[4];
    /* 0xD4 */ J3DTevKColorAnm mTevKColorAnm[4];
};  // Size: 0xF4

#endif /* J3DMATERIALANM_H */
