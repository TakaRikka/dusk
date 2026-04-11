#ifndef J3DSYS_H
#define J3DSYS_H

#include <gx.h>
#include <mtx.h>

#include "JSystem/J3DAssert.h"
#include "JSystem/JMath/JMath.h"
#include "dusk/frame_interpolation.h"
#include "dusk/endian.h"

enum J3DSysDrawBuf {
    /* 0x0 */ J3DSysDrawBuf_Opa,
    /* 0x1 */ J3DSysDrawBuf_Xlu,

    /* 0x2 */ J3DSysDrawBuf_MAX
};

class J3DMtxCalc;
class J3DModel;
class J3DMatPacket;
class J3DShapePacket;
class J3DShape;
class J3DDrawBuffer;
class J3DTexture;

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DTexCoordScaleInfo {
    /* 0x0 */ u16 field_0x00;
    /* 0x2 */ u16 field_0x02;
    /* 0x4 */ u16 field_0x04;
    /* 0x6 */ u16 field_0x06;
};

enum J3DSysFlag {
    J3DSysFlag_SkinPosCpu = 0x00000004,
    J3DSysFlag_SkinNrmCpu = 0x00000008,
    J3DSysFlag_PostTexMtx = 0x40000000,
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DSys {
    /* 0x000 */ Mtx mViewMtx;
    /* 0x030 */ J3DMtxCalc* mCurrentMtxCalc;
    /* 0x034 */ u32 mFlags;
    /* 0x038 */ J3DModel* mModel;
    /* 0x03C */ J3DMatPacket* mMatPacket;
    /* 0x040 */ J3DShapePacket* mShapePacket;
    /* 0x044 */ J3DShape* mShape;
    /* 0x048 */ J3DDrawBuffer* mDrawBuffer[J3DSysDrawBuf_MAX];
    /* 0x050 */ u32 mDrawMode;
    /* 0x054 */ u32 mMaterialMode;
    /* 0x058 */ J3DTexture* mTexture;
    /* 0x05C */ u8 unk_0x5c[0x60 - 0x5C];
    /* 0x060 */ u32 mTexCacheRegionNum;
    /* 0x064 */ GXTexRegion mTexCacheRegion[8];
    /* 0x0E4 */ u8 unk_0xe4[0x104 - 0xE4];
    /* 0x104 */ Mtx* mModelDrawMtx;
    /* 0x108 */ Mtx33* mModelNrmMtx;
    /* 0x10C */ void* mVtxPos;
    /* 0x110 */ void* mVtxNrm;
    /* 0x114 */ GXColor* mVtxCol;
#if TARGET_PC
    int mVtxPosNum = 0;
    int mVtxNrmNum = 0;
    int mVtxColNum = 0;
#endif
    /* 0x118 */ BE(Vec)* mNBTScale;

    J3DSys();
    void loadPosMtxIndx(int, u16) const;
    void loadNrmMtxIndx(int, u16) const;
    void setTexCacheRegion(GXTexCacheSize);
    void drawInit();
    void reinitGX();
    void reinitGenMode();
    void reinitLighting();
    void reinitTransform();
    void reinitTexture();
    void reinitTevStages();
    void reinitIndStages();
    void reinitPixelProc();

    enum J3DSysDrawMode {
        J3DSysDrawMode_OpaTexEdge = 3,
        J3DSysDrawMode_Xlu
    };

    MtxP getViewMtx() { return mViewMtx; }

    void setDrawModeOpaTexEdge() { mDrawMode = J3DSysDrawMode_OpaTexEdge; }
    void setDrawModeXlu() { mDrawMode = J3DSysDrawMode_Xlu; }

    void setDrawBuffer(J3DDrawBuffer* buffer, int type) {
        J3D_ASSERT_RANGE(114, type >= 0 && type < J3DSysDrawBuf_MAX);
        J3D_ASSERT_NULLPTR(115, buffer);
        mDrawBuffer[type] = buffer;
    }

    J3DDrawBuffer* getDrawBuffer(int type) {
        J3D_ASSERT_RANGE(121, type >= 0 && type < J3DSysDrawBuf_MAX);
        return mDrawBuffer[type];
    }

    void setMatPacket(J3DMatPacket* pPacket) {
        J3D_ASSERT_NULLPTR(162, pPacket != NULL);
        mMatPacket = pPacket;
    }

    void setShapePacket(J3DShapePacket* pPacket) {
        J3D_ASSERT_NULLPTR(172, pPacket != NULL);
        mShapePacket = pPacket;
    }

    void setModel(J3DModel* pModel) {
        J3D_ASSERT_NULLPTR(200, pModel != NULL);
        mModel = pModel;
    }

    J3DMatPacket* getMatPacket() { return mMatPacket; }
    void setMaterialMode(u32 mode) { mMaterialMode = mode; }

    void setCurrentMtxCalc(J3DMtxCalc * pCalc) {
        J3D_ASSERT_NULLPTR(210, pCalc != NULL);
        mCurrentMtxCalc = pCalc;
    }

    J3DMtxCalc * getCurrentMtxCalc() const { return mCurrentMtxCalc; }

    void setTexture(J3DTexture* pTex) {
        JUT_ASSERT_MSG(220, pTex != NULL, "Error : null pointer.");
        mTexture = pTex;
    }
    J3DTexture* getTexture() { return mTexture; }

    void setNBTScale(BE(Vec)* scale) { mNBTScale = scale; }
    BE(Vec)* getNBTScale() { return mNBTScale; }

    void onFlag(u32 flag) { mFlags |= flag; }

    void offFlag(u32 flag) { mFlags &= ~flag; }

    bool checkFlag(u32 flag) { return mFlags & flag ? true : false; }

    void setModelDrawMtx(Mtx* pMtxArr) {
        J3D_ASSERT_NULLPTR(230, pMtxArr);
        mModelDrawMtx = pMtxArr;
        GXSETARRAY(GX_POS_MTX_ARRAY, mModelDrawMtx, 10 * sizeof(Mtx), sizeof(*mModelDrawMtx), true);
    }

    void setModelNrmMtx(Mtx33* pMtxArr) {
        J3D_ASSERT_NULLPTR(241, pMtxArr);
        mModelNrmMtx = pMtxArr;
        GXSETARRAY(GX_NRM_MTX_ARRAY, mModelNrmMtx, 10 * sizeof(Mtx33), sizeof(*mModelNrmMtx), true);
    }

    void* getVtxPos() { return mVtxPos; }

    void setVtxPos(void* pVtxPos, int num) {
        J3D_ASSERT_NULLPTR(252, pVtxPos != NULL);
        mVtxPos = pVtxPos;
#if TARGET_PC
        mVtxPosNum = num;
#endif
    }

    void* getVtxNrm() { return mVtxNrm; }
    void setVtxNrm(void* pVtxNrm, int num) {
        mVtxNrm = pVtxNrm;
#if TARGET_PC
        mVtxNrmNum = num;
#endif
    }

    void* getVtxCol() { return mVtxCol; }
    void setVtxCol(GXColor* pVtxCol, int num) {
        mVtxCol = pVtxCol;
#if TARGET_PC
        mVtxColNum = num;
#endif
    }

    Mtx& getModelDrawMtx(u16 no) { return mModelDrawMtx[no]; }
    J3DShapePacket* getShapePacket() { return mShapePacket; }

    void setViewMtx(const Mtx m) {
#ifdef TARGET_PC
        Mtx patched;
        if (dusk::frame_interp::lookup_replacement(m, patched)) {
            m = patched;
        }
#endif
        MTXCopy(m, mViewMtx);
    }

    J3DModel* getModel() { return mModel; }

    static Mtx mCurrentMtx;
    static Vec mCurrentS;
    static Vec mParentS;
    static J3DTexCoordScaleInfo sTexCoordScaleTable[8];
};

extern u32 j3dDefaultViewNo;
extern J3DSys j3dSys;

#endif /* J3DSYS_H */
