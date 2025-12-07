#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphAnimator/J3DMtxBuffer.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "JSystem/J3DGraphLoader/J3DModelLoader.h"
#include "JSystem/JKernel/JKRHeap.h"

Mtx J3DMtxBuffer::sNoUseDrawMtx;

Mtx33 J3DMtxBuffer::sNoUseNrmMtx;

Mtx* J3DMtxBuffer::sNoUseDrawMtxPtr = &J3DMtxBuffer::sNoUseDrawMtx;

Mtx33* J3DMtxBuffer::sNoUseNrmMtxPtr = &J3DMtxBuffer::sNoUseNrmMtx;

// force .sdata2 order
f32 dummy1() {
    return 1.0f;
}

f32 dummy0() {
    return 0.0f;
}

void J3DMtxBuffer::initialize() {
    mJointTree = NULL;
    mpScaleFlagArr = NULL;
    mpEvlpScaleFlagArr = NULL;
    mpAnmMtx = NULL;
    mpWeightEvlpMtx = NULL;
    mpDrawMtxArr[0] = NULL;
    mpDrawMtxArr[1] = NULL;
    mpNrmMtxArr[0] = NULL;
    mpNrmMtxArr[1] = NULL;
    mpBumpMtxArr[0] = NULL;
    mpBumpMtxArr[1] = NULL;
    mMtxNum = 1;
    mCurrentViewNo = 0;
    mpUserAnmMtx = NULL;
}

enum {
    J3DMdlDataFlag_ConcatView = 0x10,
    J3DMdlDataFlag_NoAnimation = 0x100,
};

s32 J3DMtxBuffer::create(J3DModelData* pModelData, u32 mtxNum) {
    J3D_ASSERT_NULLPTR(76, pModelData != NULL);
    J3D_ASSERT_NONZEROARG(77, mtxNum != 0);
    
    s32 ret = kJ3DError_Success;
    mMtxNum = mtxNum;
    mJointTree = &pModelData->getJointTree();

    ret = createAnmMtx(pModelData);
    if (ret != kJ3DError_Success)
        return ret;

    ret = createWeightEnvelopeMtx(pModelData);
    if (ret != kJ3DError_Success)
        return ret;

    if (pModelData->checkFlag(J3DMdlDataFlag_NoAnimation)) {
        setNoUseDrawMtx();
    } else {
        u32 loadType = getMdlDataFlag_MtxLoadType(pModelData->getFlag());
        switch (loadType) {
        case J3DMdlDataFlag_ConcatView:
            ret = setNoUseDrawMtx();
            break;
        case 0:
        default:
            ret = createDoubleDrawMtx(pModelData, mtxNum);
            break;
        }
    }

    if (ret != kJ3DError_Success)
        return ret;

    if (pModelData->getFlag() & J3DMdlDataFlag_ConcatView) {
        pModelData->setBumpFlag(0);
    } else {
        ret = createBumpMtxArray(pModelData, mtxNum);

        if (ret != kJ3DError_Success)
            return ret;
    }

    return ret;
}

J3DError J3DMtxBuffer::createAnmMtx(J3DModelData* pModelData) {
    if (pModelData->getJointNum() != 0) {
        mpScaleFlagArr = new u8[pModelData->getJointNum()];
        mpAnmMtx = new Mtx[pModelData->getJointNum()];
        mpUserAnmMtx = mpAnmMtx;
    }

    if (mpScaleFlagArr == NULL)
        return kJ3DError_Alloc;

    if (mpAnmMtx == NULL) {
        return kJ3DError_Alloc;
    }

    return kJ3DError_Success;
}

s32 J3DMtxBuffer::createWeightEnvelopeMtx(J3DModelData* pModelData) {
    if (pModelData->getWEvlpMtxNum() != 0) {
        mpEvlpScaleFlagArr = new u8[pModelData->getWEvlpMtxNum()];
        mpWeightEvlpMtx = new Mtx[pModelData->getWEvlpMtxNum()];
    }

    if (pModelData->getWEvlpMtxNum() != 0 && mpEvlpScaleFlagArr == NULL)
        return kJ3DError_Alloc;
    if (pModelData->getWEvlpMtxNum() != 0 && mpWeightEvlpMtx == NULL)
        return kJ3DError_Alloc;

    return kJ3DError_Success;
}

s32 J3DMtxBuffer::setNoUseDrawMtx() {
    mpDrawMtxArr[0] = mpDrawMtxArr[1] = &sNoUseDrawMtxPtr;
    mpNrmMtxArr[0] = mpNrmMtxArr[1] = &sNoUseNrmMtxPtr;
    mpBumpMtxArr[0] = mpBumpMtxArr[1] = NULL;
    return kJ3DError_Success;
}

s32 J3DMtxBuffer::createDoubleDrawMtx(J3DModelData* pModelData, u32 mtxNum) {
    if (mtxNum != 0) {
        for (s32 i = 0; i < 2; i++) {
            mpDrawMtxArr[i] = new Mtx*[mtxNum];
            mpNrmMtxArr[i] = new Mtx33*[mtxNum];
            mpBumpMtxArr[i] = NULL;
        }
    }

    if (mtxNum != 0) {
        for (s32 i = 0; i < 2; i++) {
            if (mpDrawMtxArr[i] == NULL)
                return kJ3DError_Alloc;
            if (mpNrmMtxArr[i] == NULL)
                return kJ3DError_Alloc;
        }
    }

    for (s32 i = 0; i < 2; i++) {
        for (u32 j = 0; j < mtxNum; j++) {
            if (pModelData->getDrawMtxNum() != 0) {
                mpDrawMtxArr[i][j] = new (0x20) Mtx[pModelData->getDrawMtxNum()];
                mpNrmMtxArr[i][j] = new (0x20) Mtx33[pModelData->getDrawMtxNum()];
            }
        }
    }

    for (s32 i = 0; i < 2; i++) {
        for (u32 j = 0; j < mtxNum; j++) {
            if (pModelData->getDrawMtxNum() != 0) {
                if (mpDrawMtxArr[i][j] == NULL)
                    return kJ3DError_Alloc;
                if (mpNrmMtxArr[i][j] == NULL)
                    return kJ3DError_Alloc;
            }
        }
    }

    return kJ3DError_Success;
}

s32 J3DMtxBuffer::createBumpMtxArray(J3DModelData* i_modelData, u32 mtxNum) {
    J3D_ASSERT_NULLPTR(295, i_modelData != NULL);

    if (i_modelData->getModelDataType() == 0) {
        u16 bumpMtxNum = 0;
        u16 materialCount = 0;
        u16 materialNum = i_modelData->getMaterialNum();
        for (u16 j = 0; j < materialNum; j++) {
            J3DMaterial* material = i_modelData->getMaterialNodePointer(j);
            if (material->getNBTScale()->mbHasScale == true) {
                bumpMtxNum += material->getShape()->countBumpMtxNum();
                materialCount++;
            }
        }

        if (bumpMtxNum != 0 && mtxNum != 0) {
            for (int i = 0; i < 2; i++) {
                mpBumpMtxArr[i] = new Mtx33**[(u16)materialCount];
                if (mpBumpMtxArr[i] == NULL) {
                    return kJ3DError_Alloc;
                }
            }
        }

        for (int i = 0; i < 2; i++) {
            u32 offset = 0;
            u16 materialNum = i_modelData->getMaterialNum();
            for (u16 j = 0; j < materialNum; j++) {
                J3DMaterial* material = i_modelData->getMaterialNodePointer(j);
                if (material->getNBTScale()->mbHasScale == true) {
                    mpBumpMtxArr[i][offset] = new Mtx33*[mtxNum];
                    if (mpBumpMtxArr[i][offset] == NULL) {
                        return kJ3DError_Alloc;
                    }
                    material->getShape()->setBumpMtxOffset(offset);
                    offset++;
                }
            }
        }

        for (int i = 0; i < 2; i++) {
            u32 offset = 0;
            u16 materialNum = i_modelData->getMaterialNum();
            for (u16 j = 0; j < materialNum; j++) {
                J3DMaterial* material = i_modelData->getMaterialNodePointer((u16)j);
                if (material->getNBTScale()->mbHasScale == true) {
                    for (int k = 0; k < mtxNum; k++) {
                        mpBumpMtxArr[i][offset][k] = new (0x20) Mtx33[i_modelData->getDrawMtxNum()];
                        if (mpBumpMtxArr[i][offset][k] == NULL) {
                            return kJ3DError_Alloc;
                        }
                    }
                    offset++;
                }
            }
        }

        if (materialCount != 0) {
            i_modelData->setBumpFlag(1);
        }
    }

    return kJ3DError_Success;
}

static f32 J3DUnit01[] = { 0.0f, 1.0f };

void J3DMtxBuffer::calcWeightEnvelopeMtx() {
    __REGISTER MtxP weightAnmMtx;
    __REGISTER Mtx* worldMtx;
    __REGISTER Mtx* invMtx;
    __REGISTER f32 weight;
    int idx;
    int j;
    int mixNum;
    int i;
    int max;
    u16* indices;
    f32* weights;
    u8* pScale;

    #if DEBUG || !__MWERKS__
    __REGISTER Mtx mtx;
    #else
    __REGISTER f32 var_f1;
    __REGISTER f32 var_f2;
    __REGISTER f32 var_f3;
    __REGISTER f32 var_f4;
    __REGISTER f32 var_f5;
    __REGISTER f32 var_f6;
    __REGISTER f32 var_f7;
    __REGISTER f32 var_f8;
    __REGISTER f32 var_f9;
    __REGISTER f32 var_f10;
    __REGISTER f32 var_f11;
    __REGISTER f32 var_f12;
    __REGISTER f32 var_f13;
    __REGISTER f32 var_f31;
    __REGISTER f32 var_f30;
    __REGISTER f32 var_f29;
    __REGISTER f32 var_f28;
    __REGISTER f32 var_f27;
    __REGISTER f32* var_r7 = J3DUnit01;
    #endif

    i = -1;
    max = mJointTree->getWEvlpMtxNum();
    indices = mJointTree->getWEvlpMixMtxIndex() - 1;
    weights = mJointTree->getWEvlpMixWeight() - 1;

    #if !DEBUG && __MWERKS__
    asm {
        psq_l var_f27, 0x0(var_r7), 0, 0 /* qr0 */
        ps_merge00 var_f10, var_f27, var_f27
        ps_merge00 var_f12, var_f27, var_f27
        ps_merge00 var_f31, var_f27, var_f27
    }
    #endif

    while (++i < max) {
        pScale = &mpEvlpScaleFlagArr[i];
        *pScale = 1;
        weightAnmMtx = mpWeightEvlpMtx[i];

        #if DEBUG || !__MWERKS__
        weightAnmMtx[0][0] = weightAnmMtx[0][1] = weightAnmMtx[0][2] = weightAnmMtx[0][3] = 
        weightAnmMtx[1][0] = weightAnmMtx[1][1] = weightAnmMtx[1][2] = weightAnmMtx[1][3] = 
        weightAnmMtx[2][0] = weightAnmMtx[2][1] = weightAnmMtx[2][2] = weightAnmMtx[2][3] = 0.0f;
        #else
        asm {
            ps_merge00 var_f9, var_f27, var_f27
            ps_merge00 var_f11, var_f27, var_f27
            ps_merge00 var_f13, var_f27, var_f27
        }
        #endif

        j = 0;
        mixNum = mJointTree->getWEvlpMixMtxNum(i);
        do {
            idx = *++indices;
            worldMtx = &mpAnmMtx[idx];
            invMtx = &mJointTree->getInvJointMtx((u16)idx);

            #if DEBUG || !__MWERKS__
            MTXConcat(*worldMtx, *invMtx, mtx);
            #else
            // Fakematch? Doesn't match if worldMtx and invMtx are used directly.
            __REGISTER void* var_r5 = worldMtx;
            __REGISTER void* var_r6 = invMtx;
            asm {
                psq_l var_f2, 0x0(var_r6), 0, 0 /* qr0 */
                psq_l var_f1, 0x0(var_r5), 0, 0 /* qr0 */
                psq_l var_f3, 0x10(var_r5), 0, 0 /* qr0 */
                psq_l var_f5, 0x20(var_r5), 0, 0 /* qr0 */
                ps_muls0 var_f8, var_f2, var_f1
                psq_l var_f6, 0x10(var_r6), 0, 0 /* qr0 */
                ps_muls0 var_f30, var_f2, var_f3
                ps_muls0 var_f29, var_f2, var_f5
                psq_l var_f7, 0x20(var_r6), 0, 0 /* qr0 */
                ps_madds1 var_f8, var_f6, var_f1, var_f8
                psq_l var_f2, 0x8(var_r5), 0, 0 /* qr0 */
                ps_madds1 var_f30, var_f6, var_f3, var_f30
                psq_l var_f4, 0x18(var_r5), 0, 0 /* qr0 */
                ps_madds1 var_f29, var_f6, var_f5, var_f29
                psq_l var_f6, 0x28(var_r5), 0, 0 /* qr0 */
                ps_madds0 var_f8, var_f7, var_f2, var_f8
            }
            #endif

            weight = *++weights;

            #if DEBUG || !__MWERKS__
            weightAnmMtx[0][0] += mtx[0][0] * weight;
            weightAnmMtx[0][1] += mtx[0][1] * weight;
            weightAnmMtx[0][2] += mtx[0][2] * weight;
            weightAnmMtx[0][3] += mtx[0][3] * weight;
            weightAnmMtx[1][0] += mtx[1][0] * weight;
            weightAnmMtx[1][1] += mtx[1][1] * weight;
            weightAnmMtx[1][2] += mtx[1][2] * weight;
            weightAnmMtx[1][3] += mtx[1][3] * weight;
            weightAnmMtx[2][0] += mtx[2][0] * weight;
            weightAnmMtx[2][1] += mtx[2][1] * weight;
            weightAnmMtx[2][2] += mtx[2][2] * weight;
            weightAnmMtx[2][3] += mtx[2][3] * weight;
            #else
            asm {
                ps_madds0 var_f30, var_f7, var_f4, var_f30
                ps_madds0 var_f29, var_f7, var_f6, var_f29
                psq_l var_f7, 0x8(var_r6), 0, 0 /* qr0 */
                ps_madds0 var_f9, var_f8, weight, var_f9
                ps_madds0 var_f11, var_f30, weight, var_f11
                ps_madds0 var_f13, var_f29, weight, var_f13
                psq_l var_f8, 0x18(var_r6), 0, 0 /* qr0 */
                ps_muls0 var_f30, var_f7, var_f1
                ps_muls0 var_f29, var_f7, var_f3
                ps_muls0 var_f28, var_f7, var_f5
                psq_l var_f7, 0x28(var_r6), 0, 0 /* qr0 */
                psq_st var_f9, 0x0(weightAnmMtx), 0, 0 /* qr0 */
                ps_madds1 var_f30, var_f8, var_f1, var_f30
                ps_madds1 var_f29, var_f8, var_f3, var_f29
                ps_madds1 var_f28, var_f8, var_f5, var_f28
                ps_madds0 var_f30, var_f7, var_f2, var_f30
                ps_madds0 var_f29, var_f7, var_f4, var_f29
                ps_madds0 var_f28, var_f7, var_f6, var_f28
                psq_st var_f11, 0x10(weightAnmMtx), 0, 0 /* qr0 */
                psq_st var_f13, 0x20(weightAnmMtx), 0, 0 /* qr0 */
                ps_madd var_f30, var_f27, var_f2, var_f30
                ps_madd var_f29, var_f27, var_f4, var_f29
                ps_madd var_f28, var_f27, var_f6, var_f28
                ps_madds0 var_f10, var_f30, weight, var_f10
                ps_madds0 var_f12, var_f29, weight, var_f12
                ps_madds0 var_f31, var_f28, weight, var_f31
            }
            #endif

            *pScale &= mpScaleFlagArr[idx];
        } while (++j < mixNum);

        #if !DEBUG && __MWERKS__
        asm {
            psq_st var_f10, 0x8(weightAnmMtx), 0, 0 /* qr0 */
            ps_merge00 var_f10, var_f27, var_f27
            psq_st var_f12, 0x18(weightAnmMtx), 0, 0 /* qr0 */
            ps_merge00 var_f12, var_f27, var_f27
            psq_st var_f31, 0x28(weightAnmMtx), 0, 0 /* qr0 */
            ps_merge00 var_f31, var_f27, var_f27
        }
        #endif
    }
}

void J3DMtxBuffer::calcDrawMtx(u32 mdlFlag, Vec const& param_1, Mtx const& param_2) {
    Mtx* sp24, *sp20;
    int sp1C = 0;
    sp20 = mpAnmMtx;
    sp24 = mpWeightEvlpMtx;

    MtxP viewMtx;
    Mtx viewBaseMtx;
    u16 fullWgtNum;

    switch (mdlFlag) {
    case 0:
        viewMtx = j3dSys.getViewMtx();
        fullWgtNum = mJointTree->getDrawFullWgtMtxNum();
        for (u16 i = 0; i < fullWgtNum; i++) {
            MTXConcat(viewMtx, getAnmMtx(mJointTree->getDrawMtxIndex(i)), *getDrawMtx(i));
        }
        if (mJointTree->getDrawMtxNum() > fullWgtNum) {
            J3DPSMtxArrayConcat(viewMtx, *mpWeightEvlpMtx, *getDrawMtx(fullWgtNum),
                                mJointTree->getWEvlpMtxNum());
        }
        return;
    case 1:
        fullWgtNum = mJointTree->getDrawFullWgtMtxNum();
        for (u16 i = 0; i < fullWgtNum; i++) {
            MTXCopy(getAnmMtx(mJointTree->getDrawMtxIndex(i)), *getDrawMtx(i));
        }
        fullWgtNum = mJointTree->getDrawFullWgtMtxNum();
        for (u16 i = 0; i < fullWgtNum; i++) {
            MTXCopy(getWeightAnmMtx(i), *getDrawMtx(mJointTree->getDrawFullWgtMtxNum() + i));
        }
        break;
    case 2:
        J3DCalcViewBaseMtx(j3dSys.getViewMtx(), param_1, param_2, viewBaseMtx);
        fullWgtNum = mJointTree->getDrawFullWgtMtxNum();
        for (u16 i = 0; i < fullWgtNum; i++) {
            MTXConcat(viewBaseMtx, getAnmMtx(mJointTree->getDrawMtxIndex(i)), *getDrawMtx(i));
        }
        if (mJointTree->getDrawMtxNum() > mJointTree->getDrawFullWgtMtxNum()) {
            J3DPSMtxArrayConcat(viewBaseMtx, *mpWeightEvlpMtx,
                                *getDrawMtx(mJointTree->getDrawFullWgtMtxNum()),
                                mJointTree->getWEvlpMtxNum());
        }
        return;
    default:
        JUT_ASSERT_MSG(778, 0, "Error : model flag is invalid.")
    }
}

void J3DMtxBuffer::calcNrmMtx() {
    u16 drawMtxNum = mJointTree->getDrawMtxNum();
    for (u16 i = 0; i < drawMtxNum; i++) {
        if (mJointTree->getDrawMtxFlag(i) == 0) {
            if (getScaleFlag(mJointTree->getDrawMtxIndex(i)) == 1) {
                setNrmMtx(i, *getDrawMtx(i));
            } else {
                J3DPSCalcInverseTranspose(*getDrawMtx(i), *getNrmMtx(i));
            }
        } else {
            if (getEnvScaleFlag(mJointTree->getDrawMtxIndex(i)) == 1) {
                setNrmMtx(i, *getDrawMtx(i));
            } else {
                J3DPSCalcInverseTranspose(*getDrawMtx(i), *getNrmMtx(i));
            }
        }
    }
}

void J3DMtxBuffer::calcBBoardMtx() {
    u16 drawMtxNum = mJointTree->getDrawMtxNum();
    for (u16 i = 0; i < drawMtxNum; i++) {
        if (mJointTree->getDrawMtxFlag(i) == 0) {
            u16 index = mJointTree->getDrawMtxIndex(i);
            if (mJointTree->getJointNodePointer(index)->getMtxType() == 1) {
                MtxP drawMtx = *getDrawMtx(i);
                J3DCalcBBoardMtx(drawMtx);

                Mtx33* nrmMtx = getNrmMtx(i);
                (*nrmMtx)[0][0] = 1.0f / drawMtx[0][0];
                (*nrmMtx)[0][1] = 0.0f;
                (*nrmMtx)[0][2] = 0.0f;
                (*nrmMtx)[1][0] = 0.0f;
                (*nrmMtx)[1][1] = 1.0f / drawMtx[1][1];
                (*nrmMtx)[1][2] = 0.0f;
                (*nrmMtx)[2][0] = 0.0f;
                (*nrmMtx)[2][1] = 0.0f;
                (*nrmMtx)[2][2] = 1.0f / drawMtx[2][2];
            } else if (mJointTree->getJointNodePointer(index)->getMtxType() == 2) {
                MtxP drawMtx = *getDrawMtx(i);
                J3DCalcYBBoardMtx(drawMtx);
                Mtx33* nrmMtx = getNrmMtx(i);
                J3DPSCalcInverseTranspose(drawMtx, *nrmMtx);
            }
        }
    }
}

void J3DCalcViewBaseMtx(Mtx view, Vec const& scale, const Mtx& base, Mtx dst) {
    Mtx m;

    m[0][0] = base[0][0] * scale.x;
    m[0][1] = base[0][1] * scale.y;
    m[0][2] = base[0][2] * scale.z;
    m[0][3] = base[0][3];

    m[1][0] = base[1][0] * scale.x;
    m[1][1] = base[1][1] * scale.y;
    m[1][2] = base[1][2] * scale.z;
    m[1][3] = base[1][3];

    m[2][0] = base[2][0] * scale.x;
    m[2][1] = base[2][1] * scale.y;
    m[2][2] = base[2][2] * scale.z;
    m[2][3] = base[2][3];

    MTXConcat(view, m, dst);
}
