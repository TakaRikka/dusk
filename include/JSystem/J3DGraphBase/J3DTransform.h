#ifndef J3DTRANSFORM_H
#define J3DTRANSFORM_H

#include <dolphin/mtx.h>

struct J3DTextureSRTInfo;

/**
 * @ingroup jsystem-j3d
 * 
 */

struct J3DTransformInfo {
    /* 0x00 */ Vec mScale;
    /* 0x0C */ S16Vec mRotation;
    /* 0x14 */ Vec mTranslate;

    /* inline J3DTransformInfo& operator=(const J3DTransformInfo& b) {
        mScale = b.mScale;
        mRotation = b.mRotation;
        mTranslate = b.mTranslate;
        return *this;
    } */

#ifdef __MWERKS__
    inline J3DTransformInfo& operator=(const __REGISTER J3DTransformInfo& b) {
        __REGISTER const J3DTransformInfo& var_r31 = b;
        __REGISTER J3DTransformInfo& var_r30 = *this;
        __REGISTER f32 var_f31;
        asm {
            psq_l var_f31, J3DTransformInfo.mScale(var_r31), 0, 0
            psq_st var_f31, J3DTransformInfo.mScale(var_r30), 0, 0
        }
        mScale.z = b.mScale.z;
        *(u32*)&mRotation = *(u32*)&b.mRotation;
        mRotation.z = b.mRotation.z;
        asm {
            psq_l var_f31, J3DTransformInfo.mTranslate(var_r31), 0, 0
            psq_st var_f31, J3DTransformInfo.mTranslate(var_r30), 0, 0
        }
        mTranslate.z = b.mTranslate.z;
        return *this;
    }
#endif
};  // Size: 0x20

extern J3DTransformInfo const j3dDefaultTransformInfo;
extern Vec const j3dDefaultScale;
extern Mtx const j3dDefaultMtx;
extern f32 PSMulUnit01[];

void J3DGQRSetup7(u32 param_0, u32 param_1, u32 param_2, u32 param_3);
void J3DCalcBBoardMtx(f32 (*)[4]);
void J3DCalcYBBoardMtx(f32 (*)[4]);
void J3DPSCalcInverseTranspose(f32 (*param_0)[4], f32 (*param_1)[3]);
void J3DGetTranslateRotateMtx(const J3DTransformInfo&, Mtx);
void J3DGetTranslateRotateMtx(s16, s16, s16, f32, f32, f32, Mtx);
void J3DGetTextureMtx(const J3DTextureSRTInfo&, const Vec&, f32 (*)[4]);
void J3DGetTextureMtxOld(const J3DTextureSRTInfo&, const Vec&, f32 (*)[4]);
void J3DGetTextureMtxMaya(const J3DTextureSRTInfo&, f32 (*)[4]);
void J3DGetTextureMtxMayaOld(const J3DTextureSRTInfo& param_0, f32 (*)[4]);
void J3DScaleNrmMtx(f32 (*)[4], const Vec&);
void J3DScaleNrmMtx33(f32 (*)[3], const Vec&);
void J3DMtxProjConcat(f32 (*)[4], f32 (*)[4], f32 (*)[4]);
void J3DPSMtxArrayConcat(f32 (*)[4], f32 (*)[4], f32 (*)[4], u32);

inline void J3DPSMtx33Copy(__REGISTER Mtx3P src, __REGISTER Mtx3P dst) {
#ifdef __MWERKS__
    __REGISTER f32 fr4;
    __REGISTER f32 fr3;
    __REGISTER f32 fr2;
    __REGISTER f32 fr1;
    __REGISTER f32 fr0;

    asm {
        psq_l fr4, 0(src), 0, 0
        psq_l fr3, 8(src), 0, 0
        psq_l fr2, 0x10(src), 0, 0
        psq_l fr1, 0x18(src), 0, 0
        lfs fr0, 0x20(src)
        psq_st fr4, 0(dst), 0, 0
        psq_st fr3, 8(dst), 0, 0
        psq_st fr2, 0x10(dst), 0, 0
        psq_st fr1, 0x18(dst), 0, 0
        stfs fr0, 0x20(dst)
    }
#endif
}

inline void J3DPSMtx33CopyFrom34(__REGISTER MtxP src, __REGISTER Mtx3P dst) {
#ifdef __MWERKS__
    __REGISTER f32 x_y1;
    __REGISTER f32 z1;
    __REGISTER f32 x_y2;
    __REGISTER f32 z2;
    __REGISTER f32 x_y3;
    __REGISTER f32 z3;
    asm {
        psq_l x_y1, 0(src), 0, 0
        lfs z1, 8(src)
        psq_l x_y2, 16(src), 0, 0
        lfs z2, 0x18(src)
        psq_l x_y3, 32(src), 0, 0
        lfs z3, 0x28(src)
        psq_st x_y1, 0(dst), 0, 0
        stfs z1, 8(dst)
        psq_st x_y2, 12(dst), 0, 0
        stfs z2, 0x14(dst)
        psq_st x_y3, 24(dst), 0, 0
        stfs z3, 0x20(dst)
    }
#endif
}

inline void J3DPSMulMtxVec(__REGISTER MtxP mtx, __REGISTER Vec* vec, __REGISTER Vec* dst) {
#ifdef __MWERKS__
    asm {
        psq_l f0, 0(vec), 0, 0
        psq_l f2, 0(mtx), 0, 0
        psq_l f1, 8(vec), 1, 0
        ps_mul f4, f2, f0
        psq_l f3, 8(mtx), 0, 0
        ps_madd f5, f3, f1, f4
        psq_l f8, 16(mtx), 0, 0
        ps_sum0 f6, f5, f6, f5
        psq_l f9, 24(mtx), 0, 0
        ps_mul f10, f8, f0
        psq_st f6, 0(dst), 1, 0
        ps_madd f11, f9, f1, f10
        psq_l f2, 32(mtx), 0, 0
        ps_sum0 f12, f11, f12, f11
        psq_l f3, 40(mtx), 0, 0
        ps_mul f4, f2, f0
        psq_st f12, 4(dst), 1, 0
        ps_madd f5, f3, f1, f4
        ps_sum0 f6, f5, f6, f5
        psq_st f6, 8(dst), 1, 0
    }
#endif
}

inline void J3DPSMulMtxVec(__REGISTER MtxP mtx, __REGISTER S16Vec* vec, __REGISTER S16Vec* dst) {
#ifdef __MWERKS__
    asm {
        psq_l f0, 0(vec), 0, 7
        psq_l f2, 0(mtx), 0, 0
        psq_l f1, 4(vec), 1, 7
        ps_mul f4, f2, f0
        psq_l f3, 8(mtx), 0, 0
        ps_madd f5, f3, f1, f4
        psq_l f8, 16(mtx), 0, 0
        ps_sum0 f6, f5, f6, f5
        psq_l f9, 24(mtx), 0, 0
        ps_mul f10, f8, f0
        psq_st f6, 0(dst), 1, 7
        ps_madd f11, f9, f1, f10
        psq_l f2, 32(mtx), 0, 0
        ps_sum0 f12, f11, f12, f11
        psq_l f3, 40(mtx), 0, 0
        ps_mul f4, f2, f0
        psq_st f12, 2(dst), 1, 7
        ps_madd f5, f3, f1, f4
        ps_sum0 f6, f5, f6, f5
        psq_st f6, 4(dst), 1, 7
    }
#endif
}

inline void J3DPSMulMtxVec(__REGISTER Mtx3P mtx, __REGISTER Vec* vec, __REGISTER Vec* dst) {
#ifdef __MWERKS__
    asm {
        lis r6, PSMulUnit01@ha
        psq_l f0, 0(vec), 0, 0
        addi r6, r6, PSMulUnit01@l
        psq_l f2, 0(mtx), 0, 0
        psq_l f13, 0(r6), 0, 0
        psq_l f1, 8(vec), 1, 0
        ps_add f1, f13, f1
        psq_l f3, 8(mtx), 1, 0
        ps_mul f4, f2, f0
        psq_l f8, 12(mtx), 0, 0
        ps_madd f5, f3, f1, f4
        ps_sum0 f6, f5, f6, f5
        psq_l f9, 20(mtx), 1, 0
        ps_mul f10, f8, f0
        psq_st f6, 0(dst), 1, 0
        ps_madd f11, f9, f1, f10
        psq_l f2, 24(mtx), 0, 0
        ps_sum0 f12, f11, f12, f11
        psq_l f3, 32(mtx), 1, 0
        ps_mul f4, f2, f0
        psq_st f12, 4(dst), 1, 0
        ps_madd f5, f3, f1, f4
        ps_sum0 f6, f5, f6, f5
        psq_st f6, 8(dst), 1, 0
    }
#endif
}

inline void J3DPSMulMtxVec(__REGISTER Mtx3P mtx, __REGISTER S16Vec* vec, __REGISTER S16Vec* dst) {
#ifdef __MWERKS__
    asm {
        lis r6, PSMulUnit01@ha
        psq_l f0, 0(vec), 0, 7
        addi r6, r6, PSMulUnit01@l
        psq_l f2, 0(mtx), 0, 0
        psq_l f13, 0(r6), 0, 0
        psq_l f1, 4(vec), 1, 7
        ps_add f1, f13, f1
        psq_l f3, 8(mtx), 1, 0
        ps_mul f4, f2, f0
        psq_l f8, 12(mtx), 0, 0
        ps_madd f5, f3, f1, f4
        ps_sum0 f6, f5, f6, f5
        psq_l f9, 20(mtx), 1, 0
        ps_mul f10, f8, f0
        psq_st f6, 0(dst), 1, 7
        ps_madd f11, f9, f1, f10
        psq_l f2, 24(mtx), 0, 0
        ps_sum0 f12, f11, f12, f11
        psq_l f3, 32(mtx), 1, 0
        ps_mul f4, f2, f0
        psq_st f12, 2(dst), 1, 7
        ps_madd f5, f3, f1, f4
        ps_sum0 f6, f5, f6, f5
        psq_st f6, 4(dst), 1, 7
    }
#endif
}

#endif /* J3DTRANSFORM_H */
