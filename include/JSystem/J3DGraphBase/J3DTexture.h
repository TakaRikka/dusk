#ifndef J3DTEXTURE_H
#define J3DTEXTURE_H

#include "JSystem/J3DGraphBase/J3DStruct.h"
#include "JSystem/J3DAssert.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "global.h"
#include <stdint.h>

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DTexture {
private:
    /* 0x0 */ u16 mNum;
    /* 0x2 */ u16 unk_0x2;
    /* 0x4 */ ResTIMG* mpRes;

public:
    J3DTexture(u16 num, ResTIMG* res) : mNum(num), unk_0x2(0), mpRes(res) {}

    void loadGX(u16, GXTexMapID) const;
    void entryNum(u16);
    void addResTIMG(u16, ResTIMG const*);
    virtual ~J3DTexture() {}

    u16 getNum() const { return mNum; }

    ResTIMG* getResTIMG(u16 index) const {
        J3D_ASSERT_RANGE(72, index < mNum);
        return &mpRes[index];   
    }

    void setResTIMG(u16 index, const ResTIMG& timg) {
        mpRes[index] = timg;
        mpRes[index].imageOffset = ((mpRes[index].imageOffset + (uintptr_t)&timg - (uintptr_t)(mpRes + index)));
        mpRes[index].paletteOffset = ((mpRes[index].paletteOffset + (uintptr_t)&timg - (uintptr_t)(mpRes + index)));
    }
};

extern J3DTexMtxInfo const j3dDefaultTexMtxInfo;

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DTexMtx {
public:
    J3DTexMtx() {
        mTexMtxInfo = j3dDefaultTexMtxInfo;
    }

    J3DTexMtx(const J3DTexMtxInfo& info) {
        mTexMtxInfo = info;
    }

    void load(u32) const;
    void calc(const Mtx);
    void calcTexMtx(const Mtx);
    void calcPostTexMtx(const Mtx);
    void loadTexMtx(u32) const;
    void loadPostTexMtx(u32) const;

    J3DTexMtxInfo& getTexMtxInfo() { return mTexMtxInfo; }
    Mtx& getMtx() { return mMtx; }
    void setEffectMtx(Mtx effectMtx) { mTexMtxInfo.setEffectMtx(effectMtx); }

private:
    /* 0x00 */ J3DTexMtxInfo mTexMtxInfo;
    /* 0x64 */ Mtx mMtx;
};  // Size: 0x94

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DTexCoordInfo {
    /* 0x0 */ u8 mTexGenType;
    /* 0x1 */ u8 mTexGenSrc;
    /* 0x2 */ u8 mTexGenMtx;
    /* 0x3 */ u8 pad;

    J3DTexCoordInfo& operator=(const J3DTexCoordInfo& other) {
        mTexGenType = other.mTexGenType;
        mTexGenSrc = other.mTexGenSrc;
        mTexGenMtx = other.mTexGenMtx;
        pad = other.pad;
        return *this;
    }
};

extern J3DTexCoordInfo const j3dDefaultTexCoordInfo[8];

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DTexCoord : public J3DTexCoordInfo {
    J3DTexCoord() {
        setTexCoordInfo(j3dDefaultTexCoordInfo[0]);
        resetTexMtxReg();
    }
    J3DTexCoord(J3DTexCoordInfo const& info) {
        setTexCoordInfo(info);
        resetTexMtxReg();
    }
    void setTexCoordInfo(J3DTexCoordInfo const& info) {
        __memcpy(this, &info, sizeof(J3DTexCoordInfo));
    }

    u8 getTexGenType() const { return mTexGenType; }
    u8 getTexGenSrc() const { return mTexGenSrc; }
    u8 getTexGenMtx() const { return mTexGenMtx; }
    u32 getTexMtxReg() const { return mTexMtxReg & 0xff; }
    void setTexGenMtx(u8 param_1) { mTexGenMtx = param_1; }
    void setTexMtxReg(u16 reg) { mTexMtxReg = reg; }
    J3DTexCoord& operator=(const J3DTexCoord& other) {
        // Fake match (__memcpy or = doesn't match)
        *(u32*)this = *(u32*)&other;
        return *this;
    }

    void resetTexMtxReg() {
        mTexMtxReg = mTexGenMtx;
    }

    /* 0x4 */ u16 mTexMtxReg;
};  // Size: 0x6

#endif /* J3DTEXTURE_H */
