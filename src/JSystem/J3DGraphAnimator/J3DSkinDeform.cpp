#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphAnimator/J3DSkinDeform.h"
#include "JSystem/J3DGraphAnimator/J3DModel.h"
#include "JSystem/JKernel/JKRHeap.h"
#include <string.h>

J3DSkinNList::J3DSkinNList() {
    field_0x0 = NULL;
    field_0x4 = NULL;
    field_0x8 = NULL;
    field_0xc = NULL;
    field_0x10 = 0;
    field_0x12 = 0;
}

static void J3DPSWeightMTXMultVec(f32 (*param_0)[4], f32 param_1, Vec* param_2, Vec* param_3) {
    f32 f4 = param_0[0][3] + param_0[0][0] * param_2->x;
    f32 f5 = param_0[1][3] + param_0[1][0] * param_2->x;
    f32 f3 = param_0[2][3] + param_0[2][0] * param_2->x;
    f4 += param_0[0][1] * param_2->y;
    f5 += param_0[1][1] * param_2->y;
    f3 += param_0[2][1] * param_2->y;
    f4 += param_0[0][2] * param_2->z;
    f5 += param_0[1][2] * param_2->z;
    f3 += param_0[2][2] * param_2->z;
    param_3->x += f4 * param_1;
    param_3->y += f5 * param_1;
    param_3->z += f3 * param_1;
}

static void J3DPSWeightMTXMultVecSR(f32 (*param_0)[4], f32 param_1, Vec* param_2,
                                    Vec* param_3) {
    f32 f4 = param_0[0][0] * param_2->x;
    f32 f5 = param_0[1][0] * param_2->x;
    f32 f3 = param_0[2][0] * param_2->x;
    f4 += param_0[0][1] * param_2->y;
    f5 += param_0[1][1] * param_2->y;
    f3 += param_0[2][1] * param_2->y;
    f4 += param_0[0][2] * param_2->z;
    f5 += param_0[1][2] * param_2->z;
    f3 += param_0[2][2] * param_2->z;
    param_3->x += f4 * param_1;
    param_3->y += f5 * param_1;
    param_3->z += f3 * param_1;
}

void J3DSkinNList::calcSkin_VtxPosF32(f32 (*param_0)[4], void* param_1, void* param_2) {
    int r29 = field_0x10;
    for (int i = 0; i < r29; i++) {
        Vec* pVec1 = (Vec*)param_1 + field_0x0[i];
        Vec* pVec2 = (Vec*)param_2 + field_0x0[i];
        f32 weight = field_0x8[i];
        J3DPSWeightMTXMultVec(param_0, weight, pVec1, pVec2);
    }
}

void J3DSkinNList::calcSkin_VtxNrmF32(f32 (*param_0)[4], void* param_1, void* param_2) {
    int r29 = field_0x12;
    for (int i = 0; i < r29; i++) {
        Vec* pVec1 = (Vec*)param_1 + field_0x4[i];
        Vec* pVec2 = (Vec*)param_2 + field_0x4[i];
        f32 weight = field_0xc[i];
        J3DPSWeightMTXMultVecSR(param_0, weight, pVec1, pVec2);
    }
}

J3DSkinDeform::J3DSkinDeform() {
    mPosData = NULL;
    mNrmData = NULL;
    mPosMtx = NULL;
    mNrmMtx = NULL;
    mFlags = 3;
    field_0x18 = 1;
    field_0x19 = 0;
    field_0x1c = 0;
    field_0x20 = 0;
    mSkinNList = NULL;
}

u16* J3DSkinDeform::sWorkArea_WEvlpMixMtx[1024];

f32* J3DSkinDeform::sWorkArea_WEvlpMixWeight[1024];

void J3DSkinDeform::initSkinInfo(J3DModelData* pModelData) {
    J3D_ASSERT_NULLPTR(322, pModelData != NULL);

    int vtxNum = pModelData->getVtxNum();
    int wevlpMtxNum = pModelData->getWEvlpMtxNum();
    u16* wevlpMtxIndex = pModelData->getWEvlpMixMtxIndex();
    f32* wevlpMixWeights = pModelData->getWEvlpMixWeight();
    int currentOffset = 0;

    for (int i = 0; i < wevlpMtxNum; i++) {
        sWorkArea_WEvlpMixMtx[i] = wevlpMtxIndex + currentOffset;
        sWorkArea_WEvlpMixWeight[i] = wevlpMixWeights + currentOffset;
        currentOffset += pModelData->getWEvlpMixMtxNum(i);
    }

    if (pModelData->getJointNum() != 0) {
        mSkinNList = new J3DSkinNList[pModelData->getJointNum()];
    }

    for (int i = 0; i < pModelData->getVtxNum(); i++) {
        u16 uVar8 = mPosData[i];
        if (uVar8 != 0xffff) {
            if (!pModelData->getDrawMtxFlag(uVar8)) {
                u16 drawMtxIndex = pModelData->getDrawMtxIndex(uVar8);
                mSkinNList[drawMtxIndex].field_0x10++;
            } else {
                u16 drawMtxIndex = pModelData->getDrawMtxIndex(uVar8);
                int wevlpMtxNum = pModelData->getWEvlpMixMtxNum(drawMtxIndex);
                u16* indices = sWorkArea_WEvlpMixMtx[drawMtxIndex];
                for (int j = 0; j < wevlpMtxNum; j++) {
                    mSkinNList[indices[j]].field_0x10++;
                }
            }
        }
    }

    for (int i = 0; i < pModelData->getNrmNum(); i++) {
        u16 uVar8 = mNrmData[i];
        if (uVar8 != 0xffff) {
            if (!pModelData->getDrawMtxFlag(uVar8)) {
                u16 drawMtxIndex = pModelData->getDrawMtxIndex(uVar8);
                mSkinNList[drawMtxIndex].field_0x12++;
            } else {
                u16 drawMtxIndex = pModelData->getDrawMtxIndex(uVar8);
                int wevlpMtxNum = pModelData->getWEvlpMixMtxNum(drawMtxIndex);
                u16* indices = sWorkArea_WEvlpMixMtx[drawMtxIndex];
                for (int j = 0; j < wevlpMtxNum; j++) {
                    mSkinNList[indices[j]].field_0x12++;
                }
            }
        }
    }

    for (u16 i = 0; i < pModelData->getJointNum(); i++) {
        if (mSkinNList[i].field_0x10) {
            mSkinNList[i].field_0x0 = new u16[mSkinNList[i].field_0x10];
            mSkinNList[i].field_0x8 = new f32[mSkinNList[i].field_0x10];
            mSkinNList[i].field_0x10 = 0;
        }
        if (mSkinNList[i].field_0x12) {
            mSkinNList[i].field_0x4 = new u16[mSkinNList[i].field_0x12];
            mSkinNList[i].field_0xc = new f32[mSkinNList[i].field_0x12];
            mSkinNList[i].field_0x12 = 0;
        }
    }

    for (int i = 0; i < pModelData->getVtxNum(); i++) {
        u16 uVar8 = mPosData[i];
        if (uVar8 != 0xffff) {
            if (!pModelData->getDrawMtxFlag(uVar8)) {
                u16 drawMtxIndex = pModelData->getDrawMtxIndex(uVar8);
                int uVar9 = mSkinNList[drawMtxIndex].field_0x10++;
                mSkinNList[drawMtxIndex].field_0x0[uVar9] = i;
                mSkinNList[drawMtxIndex].field_0x8[uVar9] = 1.0f;
            } else {
                u16 drawMtxIndex = pModelData->getDrawMtxIndex(uVar8);
                int wevlpMtxNum = pModelData->getWEvlpMixMtxNum(drawMtxIndex);
                u16* indices = sWorkArea_WEvlpMixMtx[drawMtxIndex];
                f32* weights = sWorkArea_WEvlpMixWeight[drawMtxIndex];
                for (int j = 0; j < wevlpMtxNum; j++) {
                    int uVar9 = mSkinNList[indices[j]].field_0x10++;
                    mSkinNList[indices[j]].field_0x0[uVar9] = i;
                    mSkinNList[indices[j]].field_0x8[uVar9] = weights[j];
                }
            }
        }
    }

    for (int i = 0; i < pModelData->getNrmNum(); i++) {
        u16 uVar8 = mNrmData[i];
        if (uVar8 != 0xffff) {
            if (!pModelData->getDrawMtxFlag(uVar8)) {
                u16 drawMtxIndex = pModelData->getDrawMtxIndex(uVar8);
                int uVar9 = mSkinNList[drawMtxIndex].field_0x12++;
                mSkinNList[drawMtxIndex].field_0x4[uVar9] = i;
                mSkinNList[drawMtxIndex].field_0xc[uVar9] = 1.0f;
            } else {
                u16 drawMtxIndex = pModelData->getDrawMtxIndex(uVar8);
                int wevlpMtxNum = pModelData->getWEvlpMixMtxNum(drawMtxIndex);
                u16* indices = sWorkArea_WEvlpMixMtx[drawMtxIndex];
                f32* weights = sWorkArea_WEvlpMixWeight[drawMtxIndex];
                for (int j = 0; j < wevlpMtxNum; j++) {
                    int uVar9 = mSkinNList[indices[j]].field_0x12++;
                    mSkinNList[indices[j]].field_0x4[uVar9] = i;
                    mSkinNList[indices[j]].field_0xc[uVar9] = weights[j];
                }
            }
        }
    }
}

u16 J3DSkinDeform::sWorkArea_MtxReg[1024];

int J3DSkinDeform::initMtxIndexArray(J3DModelData* pModelData) {
    J3D_ASSERT_NULLPTR(507, pModelData != NULL);
    if (mPosData != NULL && mNrmData != NULL) {
        return kJ3DError_Success;
    }

    mPosData = new u16[pModelData->getVtxNum()];
    if (mPosData == NULL) {
        return kJ3DError_Alloc;
    }

    for (int i = 0; i < pModelData->getVtxNum(); i++) {
        mPosData[i] = 0xffff;
    }

    if (pModelData->getNrmNum()) {
        mNrmData = new u16[pModelData->getNrmNum()];
        if (mNrmData == NULL) {
            return kJ3DError_Alloc;
        }
        for (int i = 0; i < pModelData->getNrmNum(); i++) {
            mNrmData[i] = 0;
        }
    } else {
        mNrmData = NULL;
    }

    mPosMtx = new Mtx[pModelData->getJointNum()];
    mNrmMtx = new (32) Mtx33[pModelData->getDrawMtxNum()];
    if (mPosMtx == NULL) {
        return kJ3DError_Alloc;
    }
    if (mNrmMtx == NULL) {
        return kJ3DError_Alloc;
    }

    for (u16 i = 0; i < pModelData->getShapeNum(); i++) {
        u32 kSize[4] = {0, 1, 1, 2};
        int pnmtx_num = -1;
        int vtx_num = -1;
        int nrm_num = -1;
        int tex_num = -1;
        int r23 = 0;
        for (GXVtxDescList* vtxDesc = pModelData->getShapeNodePointer(i)->getVtxDesc(); vtxDesc->attr != 0xff; vtxDesc++) {
            switch (vtxDesc->attr) {
            case GX_VA_PNMTXIDX:
                pnmtx_num = r23;
                break;
            case GX_VA_POS:
                vtx_num = r23;
                if (vtxDesc->type != GX_INDEX16) {
                    OSReport(" Invlid Data : CPU Pipeline process GX_INDEX16 Data Only\n");
                    return 6;
                }
                break;
            case GX_VA_NRM:
                nrm_num = r23;
                if (vtxDesc->type != GX_INDEX16) {
                    OSReport(" Invlid Data : CPU Pipeline process GX_INDEX16 Data Only\n");
                    return 6;
                }
                break;
            case GX_VA_TEX0:
                tex_num = r23;
                if (vtxDesc->type != GX_INDEX16) {
                    OSReport(" Invlid Data : CPU Pipeline process GX_INDEX16 Data Only\n");
                    return 6;
                }
                break;
            }
            r23 += kSize[(int)vtxDesc->type];
        }

        for (u16 j = 0; j < (u16)pModelData->getShapeNodePointer(i)->getMtxGroupNum(); j++) {
            J3DShapeMtx* pShapeMtx = pModelData->getShapeNodePointer(i)->getShapeMtx(j);
            u8* pDList = pModelData->getShapeNodePointer(i)->getShapeDraw(j)->getDisplayList();
            u8* pDListPos = pDList;
            int uVar13;
            for (;
                (int)pDListPos - (int)pDList < pModelData->getShapeNodePointer(i)->getShapeDraw(j)->getDisplayListSize();
                pDListPos += r23 * uVar13
                //TODO: This loop's logic has drastically different codegen between GCN and Shield
                //      in a way that so far can't be pinned down as just compiler differences. This
                //      may have been refactored in the J3D version used for Shield, but note that
                //      it's very possible that this is a fakematch and there's some other way of
                //      expressing it that matches for both versions.
#if PLATFORM_GCN
                , pDListPos += 3
#endif
                )
            {
                u8 command = *pDListPos;
#if !PLATFORM_GCN
                pDListPos++;
#endif
                if (command != GX_TRIANGLEFAN && command != GX_TRIANGLESTRIP) {
                    break;
                }

#if PLATFORM_GCN
                uVar13 = *(u16*)(pDListPos + 1);
#else
                uVar13 = *(u16*)pDListPos;
                pDListPos += 2;
#endif
                for (int local_60 = 0; local_60 < uVar13; local_60++) {
#if PLATFORM_GCN
                    u8* iVar5 = ((u8*)(pDListPos + 3) + r23 * local_60);
#else
                    u8* iVar5 = ((u8*)pDListPos + r23 * local_60);
#endif
                    u8 bVar3 = *(u8*)(iVar5 + pnmtx_num) / 3U;
                    u16 vtx_idx = *(u16*)(iVar5 + vtx_num);
                    u16 nrm_idx = *(u16*)(iVar5 + nrm_num);
                    u16 uVar3 = *(u16*)(iVar5 + tex_num);
                    u16 local_76 = pShapeMtx->getUseMtxIndex(bVar3);
                    if (local_76 == 0xffff) {
                        local_76 = sWorkArea_MtxReg[bVar3];
                    } else if (pnmtx_num != -1) {
                        sWorkArea_MtxReg[bVar3] = local_76;
                    }

                    J3D_ASSERT_RANGE(673, local_76 < pModelData->getDrawMtxNum());
                    J3D_ASSERT_RANGE(674, vtx_idx < pModelData->getVtxNum());
                    mPosData[vtx_idx] = local_76;

                    if (nrm_num != -1) {
                        J3D_ASSERT_RANGE(680, nrm_idx < pModelData->getNrmNum());
                        mNrmData[nrm_idx] = local_76;
                    }
                }
            }

            if (nrm_num == -1) {
                pModelData->getShapeNodePointer(i)->onFlag(J3DShpFlag_EnableLod);
                pModelData->getShapeNodePointer(i)->offFlag(J3DShpFlag_SkinNrmCpu);
            }
        }
    }

    for (int i = 0; i < pModelData->getVtxNum(); i++) {
        if (mPosData[i] == 0xffff) {
            field_0x18 = 0x0;
            mPosData[i] = 0;
            OS_REPORT("Error : Invalid Positon Data Exists!.");
            OS_REPORT("Error : Invalid Positon Index = %d\n", i);
        }
    }

    return kJ3DError_Success;
}

// NONMATCHING - instruction ordering/optimization issue, matches debug
// the compiler needs to delay adding +3 to dl until the end of the while loop for the function to match
// but instead it puts the +3 at the start of the for loop and reworks the other instructions
// can get a 99.93% match on retail by moving where dl is incremented, but it seems fake as it breaks debug, and introduces an operand swap on src
void J3DSkinDeform::changeFastSkinDL(J3DModelData* pModelData) {
    J3D_ASSERT_NULLPTR(740, pModelData != NULL);
    for (u16 i = 0; i < pModelData->getShapeNum(); i++) {
        u32 kSize[4] = {0,1,1,2};
        int pnmtxIdxOffs = -1;
        int vtxSize = 0;

        J3DShape* pShapeNode = pModelData->getShapeNodePointer(i);
        for (GXVtxDescList* vtxDesc = pShapeNode->getVtxDesc(); vtxDesc->attr != GX_VA_NULL; vtxDesc++) {
            if (vtxDesc->attr == GX_VA_PNMTXIDX) {
                pnmtxIdxOffs = vtxSize;
            }
            vtxSize += kSize[vtxDesc->type];
        }

        if (pnmtxIdxOffs != -1) {
            for (u16 j = 0; j < (u16)pShapeNode->getMtxGroupNum(); j++) {
                u8* displayListStart = pShapeNode->getShapeDraw(j)->getDisplayList();
                u8* dl = displayListStart;
                u8* dst = displayListStart;
                while ((dl - displayListStart) < pShapeNode->getShapeDraw(j)->getDisplayListSize()) {
                    u8 cmd = *dl;
                    dl++;
                    *dst++ = cmd;

                    if (cmd != GX_TRIANGLEFAN && cmd != GX_TRIANGLESTRIP)
                        break;

                    int vtxCount = *(u16*)dl;
                    dl += 2;
                    *(u16*)dst = vtxCount;
                    dst += 2;

                    for (int k = 0; k < vtxCount; k++) {
                        u8* src = &dl[vtxSize * k];
                        memcpy(dst, src + 1, (int)(vtxSize - 1)); // The -1 is to remove GX_VA_PNMTXIDX
                        dst += (int)(vtxSize - 1);
                    }
                    dl += vtxSize * vtxCount;
                }

                int dlistSize = ((int)dst - (int)displayListStart + 0x1f) & ~0x1f;
                while ((int)dst - (int)displayListStart < pShapeNode->getShapeDraw(j)->getDisplayListSize()) {
                    *dst++ = 0;
                }

                pShapeNode->getShapeDraw(j)->setDisplayListSize(dlistSize);
                DCStoreRange(displayListStart, pShapeNode->getShapeDraw(j)->getDisplayListSize());
            }
        }
    }

    for (u16 i = 0; i < pModelData->getShapeNum(); i++) {
        J3DShape* shape = pModelData->getShapeNodePointer(i);
        GXVtxDescList* desc = shape->getVtxDesc();
        GXVtxDescList* descDst = desc;
        for (; desc->attr != GX_VA_NULL; desc++) {
            if (desc->attr != GX_VA_PNMTXIDX) {
                descDst->attr = desc->attr;
                descDst->type = desc->type;
                descDst++;
            }
        }
        descDst->attr = GX_VA_NULL;
        descDst->type = GX_NONE;
        shape->makeVcdVatCmd();
    }
}

void J3DSkinDeform::calcNrmMtx(J3DMtxBuffer* pMtxBuffer) {
    J3DJointTree* jointTree = pMtxBuffer->getJointTree();
    u16 drawMtxNum = jointTree->getDrawMtxNum();
    for (u16 i = 0; i < drawMtxNum; i++) {
        if (jointTree->getDrawMtxFlag(i) == 0) {
            if (pMtxBuffer->getScaleFlag(jointTree->getDrawMtxIndex(i)) == 1) {
                setNrmMtx(i, pMtxBuffer->getAnmMtx(jointTree->getDrawMtxIndex(i)));
            } else {
                J3DPSCalcInverseTranspose(pMtxBuffer->getAnmMtx(jointTree->getDrawMtxIndex(i)), getNrmMtx(i));
            }
        } else {
            if (pMtxBuffer->getEnvScaleFlag(jointTree->getDrawMtxIndex(i)) == 1) {
                setNrmMtx(i, pMtxBuffer->getWeightAnmMtx(jointTree->getDrawMtxIndex(i)));
            } else {
                J3DPSCalcInverseTranspose(pMtxBuffer->getWeightAnmMtx(jointTree->getDrawMtxIndex(i)), getNrmMtx(i));
            }
        }
    }
}

void J3DSkinDeform::transformVtxPosNrm(J3DModelData* pModelData) {
    if (pModelData->getWEvlpMtxNum() != 0 && field_0x19 == 0) {
        int vtmNum = pModelData->getVtxNum();
        int nrmNum = pModelData->getNrmNum();

        for (int i = 0; i < vtmNum; i++) {
            u16 posIndex = mPosData[i];
            if (pModelData->getDrawMtxFlag(posIndex) == 0) {
                u16 drawMtxIndex = pModelData->getDrawMtxIndex(posIndex);
                Vec* pos = ((Vec*)pModelData->getVtxPosArray()) + i;
                Mtx invMtx;
                MTXInverse(pModelData->getInvJointMtx(drawMtxIndex), invMtx);
                MTXMultVec(invMtx, pos, pos);
            }
        }

        for (int i = 0; i < nrmNum; i++) {
            u16 nrmIndex = mNrmData[i];
            if (pModelData->getDrawMtxFlag(nrmIndex) == 0) {
                u16 drawMtxIndex = pModelData->getDrawMtxIndex(nrmIndex);
                Vec* nrm = ((Vec*)pModelData->getVtxNrmArray()) + i;
                Mtx invMtx;
                MTXInverse(pModelData->getInvJointMtx(drawMtxIndex), invMtx);
                MTXMultVecSR(invMtx, nrm, nrm);
            }
        }

        field_0x19 = 1;
    }
}

void J3DSkinDeform::calcAnmInvJointMtx(J3DMtxBuffer* pMtxBuffer) {
    J3D_ASSERT_NULLPTR(978, pMtxBuffer != NULL);

    if (pMtxBuffer->getJointTree()->getWEvlpMtxNum() != 0) {
        int jointNum = pMtxBuffer->getJointTree()->getJointNum();
        Mtx* anmMtx = (Mtx*)pMtxBuffer->getAnmMtx(0);
        Mtx* invJointMtx = &pMtxBuffer->getJointTree()->getInvJointMtx(0);
        Mtx* posMtx = mPosMtx;
        for (int i = 0; i < jointNum; i++) {
            MTXConcat(anmMtx[i], invJointMtx[i], posMtx[i]);
        }
    }
}

void J3DSkinDeform::deformFastVtxPos_F32(J3DVertexBuffer* pVtxBuffer, J3DMtxBuffer* pMtxBuffer) const {
    pVtxBuffer->swapTransformedVtxPos();
    J3DJointTree* jointTree = pMtxBuffer->getJointTree();
    u32 vtxNum = pVtxBuffer->getVertexData()->getVtxNum();
    int sp14 = jointTree->getDrawMtxNum();
    void* currentVtxPos = pVtxBuffer->getCurrentVtxPos();
    void* transformedVtxPos = pVtxBuffer->getTransformedVtxPos(0);

    J3DFillZero32B(transformedVtxPos, OSRoundUp32B(vtxNum * sizeof(Vec)));

    if (jointTree->getWEvlpMtxNum() != 0) {
        u16 jointNum = jointTree->getJointNum();
        for (u16 i = 0; i < jointNum; i++) {
            J3DSkinNList* skinList = mSkinNList + i;
            skinList->calcSkin_VtxPosF32(mPosMtx[i], currentVtxPos, transformedVtxPos);
        }
    } else {
        u16 jointNum = jointTree->getJointNum();
        for (u16 i = 0; i < jointNum; i++) {
            J3DSkinNList* skinList = mSkinNList + i;
            skinList->calcSkin_VtxPosF32(pMtxBuffer->getAnmMtx(i), currentVtxPos, transformedVtxPos);
        }
    }

    DCStoreRange(pVtxBuffer->getTransformedVtxPos(0), pVtxBuffer->getVertexData()->getVtxNum() * sizeof(Vec));
    pVtxBuffer->setCurrentVtxPos(transformedVtxPos);
}

void J3DSkinDeform::deformFastVtxNrm_F32(J3DVertexBuffer* pVtxBuffer, J3DMtxBuffer* pMtxBuffer) const {
    pVtxBuffer->swapTransformedVtxNrm();
    J3DJointTree* jointTree = pMtxBuffer->getJointTree();
    int nrmNum = pVtxBuffer->getVertexData()->getNrmNum();
    int sp18 = jointTree->getDrawMtxNum();
    void* currentVtxNrm = pVtxBuffer->getCurrentVtxNrm();
    void* transformedVtxNrm = pVtxBuffer->getTransformedVtxNrm(0);

    for (int i = 0; i < nrmNum; i++) {
        ((f32*)transformedVtxNrm)[(i * 3) + 0] = 0.0f;
        ((f32*)transformedVtxNrm)[(i * 3) + 1] = 0.0f;
        ((f32*)transformedVtxNrm)[(i * 3) + 2] = 0.0f;
    }

    if (jointTree->getWEvlpMtxNum() != 0) {
        u16 jointNum = jointTree->getJointNum();
        for (u16 i = 0; i < jointNum; i++) {
            J3DSkinNList* skinList = mSkinNList + i;
            skinList->calcSkin_VtxNrmF32(mPosMtx[i], currentVtxNrm, transformedVtxNrm);
        }
    } else {
        u16 jointNum = jointTree->getJointNum();
        for (u16 i = 0; i < jointNum; i++) {
            J3DSkinNList* skinList = mSkinNList + i;
            skinList->calcSkin_VtxNrmF32(pMtxBuffer->getAnmMtx(i), currentVtxNrm, transformedVtxNrm);
        }
    }

    DCStoreRange(pVtxBuffer->getTransformedVtxNrm(0), pVtxBuffer->getVertexData()->getNrmNum() * sizeof(Vec));
    pVtxBuffer->setCurrentVtxNrm(transformedVtxNrm);
}

void J3DSkinDeform::deformVtxPos_F32(J3DVertexBuffer* pVtxBuffer, J3DMtxBuffer* pMtxBuffer) const {
    Mtx* anmMtx = NULL;
    Mtx* anmMtxs[2];
    anmMtxs[0] = (Mtx*)pMtxBuffer->getAnmMtx(0);
    anmMtxs[1] = (Mtx*)pMtxBuffer->getWeightAnmMtx(0);
    pVtxBuffer->swapTransformedVtxPos();

    J3DJointTree* jointTree = pMtxBuffer->getJointTree();
    int vtxNum = pVtxBuffer->getVertexData()->getVtxNum();
    int sp8 = jointTree->getDrawMtxNum();
    void* currentVtxPos = (void*)pVtxBuffer->getCurrentVtxPos();
    void* transformedVtxPos = (void*)pVtxBuffer->getTransformedVtxPos(0);

    for (int i = 0; i < vtxNum; i++) {
        anmMtx = anmMtxs[jointTree->getDrawMtxFlag(mPosData[i])];
        J3DPSMulMtxVec(anmMtx[jointTree->getDrawMtxIndex(mPosData[i])], (Vec*)(((f32*)currentVtxPos) + (i * 3)), (Vec*)(((f32*)transformedVtxPos) + (i * 3)));
    }

    DCStoreRange(pVtxBuffer->getTransformedVtxPos(0), pVtxBuffer->getVertexData()->getVtxNum() * sizeof(Vec));
    pVtxBuffer->setCurrentVtxPos(transformedVtxPos);
}

void J3DSkinDeform::deformVtxPos_S16(J3DVertexBuffer* pVtxBuffer, J3DMtxBuffer* pMtxBuffer) const {
    Mtx* anmMtx = NULL;
    Mtx* anmMtxs[2];
    anmMtxs[0] = (Mtx*)pMtxBuffer->getAnmMtx(0);
    anmMtxs[1] = (Mtx*)pMtxBuffer->getWeightAnmMtx(0);
    int vtxPosFrac = pVtxBuffer->getVertexData()->getVtxPosFrac();
    J3DGQRSetup7(vtxPosFrac, 7, vtxPosFrac, 7);
    pVtxBuffer->swapTransformedVtxPos();

    J3DJointTree* jointTree = pMtxBuffer->getJointTree();
    int vtxNum = pVtxBuffer->getVertexData()->getVtxNum();
    int sp8 = jointTree->getDrawMtxNum();
    void* currentVtxPos = (void*)pVtxBuffer->getCurrentVtxPos();
    void* transformedVtxPos = (void*)pVtxBuffer->getTransformedVtxPos(0);

    for (int i = 0; i < vtxNum; i++) {
        anmMtx = anmMtxs[jointTree->getDrawMtxFlag(mPosData[i])];
        J3DPSMulMtxVec(anmMtx[jointTree->getDrawMtxIndex(mPosData[i])], (S16Vec*)(((s16*)currentVtxPos) + (i * 3)), (S16Vec*)(((s16*)transformedVtxPos) + (i * 3)));
    }

    DCStoreRange(pVtxBuffer->getTransformedVtxPos(0), pVtxBuffer->getVertexData()->getVtxNum() * sizeof(S16Vec));
    pVtxBuffer->setCurrentVtxPos(transformedVtxPos);
}

void J3DSkinDeform::deformVtxNrm_F32(J3DVertexBuffer* pVtxBuffer) const {
    pVtxBuffer->swapTransformedVtxNrm();
    int nrmNum = pVtxBuffer->getVertexData()->getNrmNum();
    void* currentVtxNrm = (void*)pVtxBuffer->getCurrentVtxNrm();
    void* transformedVtxNrm = (void*)pVtxBuffer->getTransformedVtxNrm(0);

    for (int i = 0; i < nrmNum; i++) {
        J3DPSMulMtxVec(mNrmMtx[mNrmData[i]], (Vec*)((u8*)currentVtxNrm + i * 3 * 4), (Vec*)((u8*)transformedVtxNrm + i * 3 * 4));
    }

    DCStoreRange(pVtxBuffer->getTransformedVtxNrm(0), pVtxBuffer->getVertexData()->getNrmNum() * sizeof(Vec));
    pVtxBuffer->setCurrentVtxNrm(transformedVtxNrm);
}

void J3DSkinDeform::deformVtxNrm_S16(J3DVertexBuffer* pVtxBuffer) const {
    int vtxNrmFrac = pVtxBuffer->getVertexData()->getVtxNrmFrac();
    J3DGQRSetup7(vtxNrmFrac, 7, vtxNrmFrac, 7);
    pVtxBuffer->swapTransformedVtxNrm();

    int nrmNum = pVtxBuffer->getVertexData()->getNrmNum();
    void* currentVtxNrm = (void*)pVtxBuffer->getCurrentVtxNrm();
    void* transformedVtxNrm = (void*)pVtxBuffer->getTransformedVtxNrm(0);

    for (int i = 0; i < nrmNum; i++) {
        J3DPSMulMtxVec(mNrmMtx[mNrmData[i]], (S16Vec*)(((s16*)currentVtxNrm) + (i * 3)), (S16Vec*)(((s16*)transformedVtxNrm) + (i * 3)));
    }

    DCStoreRange(pVtxBuffer->getTransformedVtxNrm(0), pVtxBuffer->getVertexData()->getNrmNum() * sizeof(S16Vec));
    pVtxBuffer->setCurrentVtxNrm(transformedVtxNrm);
}

void J3DSkinDeform::deform(J3DModel* pModel) {
    J3D_ASSERT_NULLPTR(1270, pModel != NULL);

    if (pModel->checkFlag(J3DMdlFlag_SkinPosCpu)) {
        onFlag(2);
    } else {
        offFlag(2);
    }
    
    if (pModel->checkFlag(J3DMdlFlag_SkinNrmCpu)) {
        onFlag(1);
    } else {
        offFlag(1);
    }

    deform(pModel->getVertexBuffer(), pModel->getMtxBuffer());
}

void J3DSkinDeform::deform(J3DVertexBuffer* pVtxBuffer, J3DMtxBuffer* pMtxBuffer) {
    J3D_ASSERT_NULLPTR(1299, pVtxBuffer != NULL);
    J3D_ASSERT_NULLPTR(1300, pMtxBuffer != NULL);

    if (pMtxBuffer->getJointTree()->checkFlag(0x100)) {
        calcAnmInvJointMtx(pMtxBuffer);
    }

    if (checkFlag(2) != 0) {
        if (pMtxBuffer->getJointTree()->checkFlag(0x100)) {
            deformFastVtxPos_F32(pVtxBuffer, pMtxBuffer);
        } else {
            if (pVtxBuffer->getVertexData()->getVtxPosType() == 4) {
                deformVtxPos_F32(pVtxBuffer, pMtxBuffer);
            } else {
                deformVtxPos_S16(pVtxBuffer, pMtxBuffer);
            }
        }
    }

    if (checkFlag(1)) {
        if (pMtxBuffer->getJointTree()->checkFlag(0x100)) {
            deformFastVtxNrm_F32(pVtxBuffer, pMtxBuffer);
        } else {
            calcNrmMtx(pMtxBuffer);
            if (pVtxBuffer->getVertexData()->getVtxNrmType() == 4) {
                deformVtxNrm_F32(pVtxBuffer);
            } else {
                deformVtxNrm_S16(pVtxBuffer);
            }
        }
    }
}

void J3DVtxColorCalc::calc(J3DModel* pModel) {
    J3D_ASSERT_NULLPTR(1351, pModel != NULL);
    calc(pModel->getVertexBuffer());
}

J3DSkinDeform::~J3DSkinDeform() {}
