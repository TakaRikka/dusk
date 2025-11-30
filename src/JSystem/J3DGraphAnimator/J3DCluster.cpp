#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphAnimator/J3DCluster.h"
#include "JSystem/J3DGraphAnimator/J3DAnimation.h"
#include "JSystem/J3DGraphAnimator/J3DModel.h"
#include "JSystem/JMath/JMATrigonometric.h"
#include "dolphin/base/PPCArch.h"

J3DDeformData::J3DDeformData() {
    mClusterNum = 0;
    mClusterKeyNum = 0;
    mClusterVertexNum = 0;
    mClusterPointer = NULL;
    mClusterKeyPointer = NULL;
    mClusterVertex = NULL;
    mVtxPosNum = 0;
    mVtxNrmNum = 0;
    mVtxPos = NULL;
    mVtxNrm = NULL;
    mClusterName = NULL;
    mClusterKeyName = NULL;
}

void J3DDeformData::offAllFlag(u32 i_flag) {
    for (u16 i = 0; i < mClusterNum; i++) {
        mClusterPointer[i].getDeformer()->offFlag(i_flag);
    }
}

void J3DDeformData::deform(J3DModel* model) {
    J3D_ASSERT_NULLPTR(110, model);

    deform(model->getVertexBuffer());
}

void J3DDeformData::deform(J3DVertexBuffer* buffer) {
    J3D_ASSERT_NULLPTR(141, buffer);

    buffer->swapVtxPosArrayPointer();
    buffer->swapVtxNrmArrayPointer();

    for (u16 i = 0; i < mClusterNum; i++) {
        mClusterPointer[i].getDeformer()->deform(buffer, i);
    }

    DCStoreRangeNoSync(buffer->getVtxPosArrayPointer(0),
                       buffer->getVertexData()->getVtxNum() * sizeof(Vec));
    DCStoreRangeNoSync(buffer->getVtxNrmArrayPointer(0),
                       buffer->getVertexData()->getNrmNum() * sizeof(Vec));
    PPCSync();

    buffer->setCurrentVtxPos(buffer->getVtxPosArrayPointer(0));
    buffer->setCurrentVtxNrm(buffer->getVtxNrmArrayPointer(0));
}

void J3DDeformData::setAnm(J3DAnmCluster* anm) {
    for (u16 i = 0; i < mClusterNum; i++) {
        mClusterPointer[i].getDeformer()->setAnmCluster(anm);
    }
}

J3DDeformer::J3DDeformer(J3DDeformData* data) {
    mDeformData = data;
    mAnmCluster = NULL;
    field_0x8 = NULL;
    field_0xc = NULL;
    mFlags = 3;
}

void J3DDeformer::deform(J3DVertexBuffer* buffer, u16 param_1) {
    J3D_ASSERT_NULLPTR(222, buffer);

    u16 var_r31 = 0;
    if (mAnmCluster != NULL) {
        for (u16 i = 0; i < param_1; i++) {
            var_r31 += mDeformData->getClusterPointer(i)->mKeyNum;
        }

        u16 num = mDeformData->getClusterPointer(param_1)->mKeyNum;
        for (u16 i = 0; i < num; i++) {
            field_0x8[i] = mAnmCluster->getWeight(var_r31++);
        }

        deform(buffer, param_1, field_0x8);
    }
}

void J3DDeformer::deform_VtxPosF32(J3DVertexBuffer* i_buffer, J3DCluster* i_cluster,
                                   J3DClusterKey* i_key, f32* i_weights) {
    J3DClusterKey* key;
    int posNum = i_cluster->mPosNum;
    int keyNum = i_cluster->mKeyNum;
    f32* vtxPosArray = (f32*)i_buffer->getVtxPosArrayPointer(0);
    f32* deformVtxPos = mDeformData->getVtxPos();
    u16* iVar9 = i_cluster->field_0x18;

    for (int i = 0; i < posNum; i++) {
        int index = iVar9[i] * 3;
        vtxPosArray[index] = 0.0f;
        vtxPosArray[index + 1] = 0.0f;
        vtxPosArray[index + 2] = 0.0f;
    }

    f32 local_58[2] = {1.0f, -1.0f};
    
    for (u16 i = 0; i < posNum; i++) {
        int index = i_cluster->field_0x18[i] * 3;
        for (u16 j = 0; j < keyNum; j++) {
            int uVar8;
            int uVar7;
            key = &i_key[j];
            uVar8 = uVar7 = ((u16*)key->field_0x4)[i];
            uVar7 &= ~0xE000;
            uVar7 *= 3;
            f32 deform0 = deformVtxPos[uVar7];
            f32 deform1 = deformVtxPos[uVar7 + 1];
            f32 deform2 = deformVtxPos[uVar7 + 2];
            deform0 *= local_58[((uVar8 & 0x8000) >> 0xF)];
            deform1 *= local_58[((uVar8 & 0x4000) >> 0xE)];
            deform2 *= local_58[((uVar8 & 0x2000) >> 0xD)];
            vtxPosArray[index] += deform0 * i_weights[j];
            vtxPosArray[index + 1] += deform1 * i_weights[j];
            vtxPosArray[index + 2] += deform2 * i_weights[j];
        }
    }
}

// NONMATCHING one missing mr
void J3DDeformer::deform_VtxNrmF32(J3DVertexBuffer* i_buffer, J3DCluster* i_cluster,
                                   J3DClusterKey* i_key, f32* i_weights) {
    f32* vtxNrmArray = (f32*)i_buffer->getVtxNrmArrayPointer(0);
    f32* deformVtxNrm = mDeformData->getVtxNrm();
    f32* iVar13 = field_0xc;
    u16 keyNum = i_cluster->mKeyNum;
    int uVar2 = i_cluster->field_0x16;

    for (u16 i = 0; i < i_cluster->field_0x14; i++) {
        int index = i * 3;
        iVar13[index] = 0.0f;
        iVar13[index + 1] = 0.0f;
        iVar13[index + 2] = 0.0f;
        for (u16 j = 0; j < keyNum; j++) {
            J3DClusterKey* key = &i_key[j];
            int uVar3 = ((u16*)key->field_0x8)[i];
            int uVar4 = uVar3;
            uVar3 &= ~0xE000;
            uVar3 *= 3;
            Vec deform0;
            if (uVar4 & 0x8000) {
                deform0.x = -deformVtxNrm[uVar3];
            } else {
                deform0.x = deformVtxNrm[uVar3];
            }
            if (uVar4 & 0x4000) {
                deform0.y = -deformVtxNrm[uVar3 + 1];
            } else {
                deform0.y = deformVtxNrm[uVar3 + 1];
            }
            if (uVar4 & 0x2000) {
                deform0.z = -deformVtxNrm[uVar3 + 2];
            } else {
                deform0.z = deformVtxNrm[uVar3 + 2];
            }
            iVar13[index] += deform0.x * i_weights[j];
            iVar13[index + 1] += deform0.y * i_weights[j];
            iVar13[index + 2] += deform0.z * i_weights[j];
        }
        normalize(&iVar13[index]);
    }

    for (u16 i = 0; i < uVar2; i++) {
        J3DClusterVertex* clusterVtx = &i_cluster->mClusterVertex[i];
        Vec vec;
        vec.x = 0.0f;
        vec.y = 0.0f;
        vec.z = 0.0f;
        f32 scale = 1.0f / clusterVtx->mNum;
        for (u16 j = 0; j < clusterVtx->mNum; j++) {
            int index = clusterVtx->field_0x4[j] * 3;
            vec.x += scale * iVar13[index];
            vec.y += scale * iVar13[index + 1];
            vec.z += scale * iVar13[index + 2];
        }
        normalize((f32*)&vec);

        for (u16 j = 0; j < clusterVtx->mNum; j++) {
            u16 tmp = clusterVtx->field_0x8[j];
            if (tmp == 0xffff) {
                continue;
            }
            int index = tmp * 3;
            u16 iVar4 = clusterVtx->field_0x4[j];
            int index2 = iVar4 * 3;
            
            f32 dot = vec.x * iVar13[index2] + vec.y * iVar13[index2 + 1]
                                             + vec.z * iVar13[index2 + 2];
            f32 angle;
            if (dot >= 1.0f) {
                angle = 0.0f;
            } else if (dot > -1.0f) {
                angle = JMath::acosDegree(dot);
            } else {
                angle = 180.0f;
            }

            if (angle <= i_cluster->mMinAngle) {
                vtxNrmArray[index] = vec.x;
                vtxNrmArray[index + 1] = vec.y;
                vtxNrmArray[index + 2] = vec.z;
            } else if (angle > i_cluster->mMaxAngle) {
                vtxNrmArray[index] = iVar13[iVar4 * 3];
                vtxNrmArray[index + 1] = iVar13[iVar4 * 3 + 1];
                vtxNrmArray[index + 2] = iVar13[iVar4 * 3 + 2];
            } else {
                f32 weights[2];
                weights[0] = (angle - i_cluster->mMinAngle)
                    / (i_cluster->mMaxAngle - i_cluster->mMinAngle);
                weights[1] = 1.0f - weights[0];
                vtxNrmArray[index] = weights[0] * iVar13[index2] + weights[1] * vec.x;
                vtxNrmArray[index + 1] = weights[0] * iVar13[index2 + 1] + weights[1] * vec.y;
                vtxNrmArray[index + 2] = weights[0] * iVar13[index2 + 2] + weights[1] * vec.z;
            }
        }
    }
}

void J3DDeformer::deform(J3DVertexBuffer* i_buffer, u16 param_1, f32* i_weights) {
    J3D_ASSERT_NULLPTR(505, i_buffer != NULL);

    if (checkFlag(2)) {
        JUT_ASSERT_MSG(512, i_buffer->getVertexData()->getVtxPosType() == 4, "Error : Invalid Verex Format");
        if (i_buffer->getVertexData()->getVtxPosType() == 4) {
            J3DCluster* cluster = mDeformData->getClusterPointer(param_1);
            u16 offset = 0;
            J3DClusterKey* clusterKey = NULL;

            for (u16 i = 0; i < param_1; i++) {
                offset += mDeformData->getClusterPointer(i)->mKeyNum + 1;
            }

            clusterKey = mDeformData->getClusterKeyPointer(offset);
            
            int var_r23 = cluster->mKeyNum;
            normalizeWeight(var_r23, i_weights);
            deform_VtxPosF32(i_buffer, cluster, clusterKey, i_weights);

            if (checkFlag(1) && cluster->mFlags != 0 && i_buffer->getVertexData()->getVtxNrmType() == 4)
            {
                deform_VtxNrmF32(i_buffer, cluster, clusterKey, i_weights);
            }
        }
    }
}

void J3DDeformer::normalizeWeight(int i_keyNum, f32* i_weights) {
    f32 totalWeight = 0.0f;
    for (u16 i = 0; i < i_keyNum; i++) {
        totalWeight += i_weights[i];
    }

    f32 scale = 1.0f / totalWeight;
    for (u16 i = 0; i < i_keyNum; i++) {
        i_weights[i] *= scale;
    }
}
