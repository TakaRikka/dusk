#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JParticle/JPAEmitterManager.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAParticle.h"
#include "JSystem/JParticle/JPAResourceManager.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/gx.h"

JPAEmitterManager::JPAEmitterManager(u32 i_ptclNum, u32 i_emtrNum, JKRHeap* pHeap, u8 i_gidMax,
                                     u8 i_ridMax) {
    emtrNum = i_emtrNum;
    ptclNum = i_ptclNum;
    gidMax = i_gidMax;
    ridMax = i_ridMax;

    JUT_ASSERT(40, emtrNum && ptclNum && gidMax && ridMax);

    JPABaseEmitter* p_emtr_link = new (pHeap, 0) JPABaseEmitter[emtrNum];
    JUT_ASSERT(44, p_emtr_link);
    for (u32 i = 0; i < emtrNum; i++)
        mFreeEmtrList.prepend(&p_emtr_link[i].mLink);

    JPANode<JPABaseParticle>* p_ptcl_node = new (pHeap, 0) JPANode<JPABaseParticle>[ptclNum];
    JUT_ASSERT(51, p_ptcl_node);
    for (u32 i = 0; i < ptclNum; i++)
        mPtclPool.push_back(&p_ptcl_node[i]);

    pEmtrUseList = new (pHeap, 0) JSUList<JPABaseEmitter>[gidMax];
    JUT_ASSERT(58, pEmtrUseList);
    pResMgrAry = new (pHeap, 0) JPAResourceManager*[ridMax];
    JUT_ASSERT(62, pResMgrAry)
    for (int i = 0; i < ridMax; i++) {
        pResMgrAry[i] = NULL;
    }

    pWd = new (pHeap, 0) JPAEmitterWorkData();
    JUT_ASSERT(67, pWd);
}

/* 8027DEBC-8027DFA0 2787FC 00E4+00 0/0 3/3 0/0 .text
 * createSimpleEmitterID__17JPAEmitterManagerFRCQ29JGeometry8TVec3<f>UsUcUcP18JPAEmitterCallBackP19JPAParticleCallBack
 */
JPABaseEmitter* JPAEmitterManager::createSimpleEmitterID(JGeometry::TVec3<f32> const& pos,
                                                         u16 resID, u8 group_id, u8 res_mgr_id,
                                                         JPAEmitterCallBack* emtrCB,
                                                         JPAParticleCallBack* ptclCB) {
    JUT_ASSERT(88, group_id < gidMax);
    JUT_ASSERT(89, res_mgr_id < ridMax);
    JUT_ASSERT(90, pResMgrAry[res_mgr_id] != NULL);
    JPAResource* pRes = pResMgrAry[res_mgr_id]->getResource(resID);

    if (pRes == NULL) {
        JUT_WARN_DEVICE(94, 3, "JPA : User Index %d is NOT exist\n", resID);
    } else if (mFreeEmtrList.getNumLinks() == 0) {
        JUT_WARN_DEVICE(97, 3, "JPA : Can NOT create emitter more\n");
    } else {
        JSULink<JPABaseEmitter>* pLink = mFreeEmtrList.getFirst();
        mFreeEmtrList.remove(pLink);
        pEmtrUseList[group_id].append(pLink);
        JPABaseEmitter* emtr = pLink->getObject();
        emtr->init(this, pRes);
        emtr->mpPtclPool = &mPtclPool;
        emtr->mpEmtrCallBack = emtrCB;
        emtr->mpPtclCallBack = ptclCB;
        emtr->mGroupID = group_id;
        emtr->mResMgrID = res_mgr_id;
        emtr->mGlobalTrs.set(pos);
        return emtr;
    }

    return NULL;
}

void JPAEmitterManager::calc(u8 group_id) {
    JUT_ASSERT(154, group_id < gidMax);
    JSULink<JPABaseEmitter>* pNext = NULL;
    for (JSULink<JPABaseEmitter>* pLink = pEmtrUseList[group_id].getFirst();
         pLink != pEmtrUseList[group_id].getEnd(); pLink = pNext) {
        pNext = pLink->getNext();

        JPABaseEmitter* emtr = pLink->getObject();

        if (emtr->mpRes->calc(pWd, emtr) && !emtr->checkStatus(0x200))
            forceDeleteEmitter(emtr);
    }
}

void JPAEmitterManager::draw(JPADrawInfo const* drawInfo, u8 group_id) {
    JUT_ASSERT(192, group_id < gidMax);
    drawInfo->getCamMtx(pWd->mPosCamMtx);
    drawInfo->getPrjMtx(pWd->mPrjMtx);
    calcYBBCam();
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXEnableTexOffsets(GX_TEXCOORD0, GX_TRUE, GX_TRUE);
    GXEnableTexOffsets(GX_TEXCOORD1, GX_TRUE, GX_TRUE);
    GXEnableTexOffsets(GX_TEXCOORD2, GX_TRUE, GX_TRUE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetCoPlanar(GX_FALSE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetNumChans(0);

    for (JSULink<JPABaseEmitter>* pLink = pEmtrUseList[group_id].getFirst();
         pLink != pEmtrUseList[group_id].getEnd(); pLink = pLink->getNext()) {
        JPABaseEmitter* emtr = pLink->getObject();
        if (!emtr->checkStatus(0x04)) {
            pWd->mpResMgr = pResMgrAry[emtr->mResMgrID];
            emtr->mpRes->draw(pWd, emtr);
        }
    }
}

void JPAEmitterManager::forceDeleteAllEmitter() {
    for (u8 i = 0; i < gidMax; i++)
        forceDeleteGroupEmitter(i);
}

void JPAEmitterManager::forceDeleteGroupEmitter(u8 group_id) {
    JUT_ASSERT(288, group_id < gidMax);
    while (pEmtrUseList[group_id].getNumLinks())
        forceDeleteEmitter(pEmtrUseList[group_id].getLast()->getObject());
}

void JPAEmitterManager::forceDeleteEmitter(JPABaseEmitter* emtr) {
    emtr->deleteAllParticle();
    emtr->setStatus(0x300);
    pEmtrUseList[emtr->getGroupID()].remove(&emtr->mLink);
    mFreeEmtrList.prepend(&emtr->mLink);
}

void JPAEmitterManager::entryResourceManager(JPAResourceManager* resMgr, u8 resMgrID) {
    JUT_ASSERT_MSG_F(325, resMgrID < ridMax && (pResMgrAry[resMgrID]) == NULL,
                     "res_id %d res_id_max %d array[%d] = %x", resMgrID, ridMax, resMgrID,
                     pResMgrAry[resMgrID]);
    pResMgrAry[resMgrID] = resMgr;
}

void JPAEmitterManager::clearResourceManager(u8 res_mgr_id) {
    JUT_ASSERT(339, res_mgr_id < ridMax);
    for (u8 i = 0; i < gidMax; i++) {
        JSULink<JPABaseEmitter>* pNext = NULL;
        for (JSULink<JPABaseEmitter>* pLink = pEmtrUseList[i].getFirst();
             pLink != pEmtrUseList[i].getEnd(); pLink = pNext) {
            pNext = pLink->getNext();

            if (res_mgr_id == pLink->getObject()->getResourceManagerID())
                forceDeleteEmitter(pLink->getObject());
        }
    }

    pResMgrAry[res_mgr_id] = NULL;
}

void JPAEmitterManager::calcYBBCam() {
    JGeometry::TVec3<float> v(0.0f, pWd->mPosCamMtx[1][1], pWd->mPosCamMtx[2][1]);
    JUT_ASSERT(367, !v.isZero());
    v.normalize();
    pWd->mYBBCamMtx[0][0] = 1.0f;
    pWd->mYBBCamMtx[0][1] = 0.0f;
    pWd->mYBBCamMtx[0][2] = 0.0f;
    pWd->mYBBCamMtx[0][3] = pWd->mPosCamMtx[0][3];
    pWd->mYBBCamMtx[1][0] = 0.0f;
    pWd->mYBBCamMtx[1][1] = v.y;
    pWd->mYBBCamMtx[1][2] = -v.z;
    pWd->mYBBCamMtx[1][3] = pWd->mPosCamMtx[1][3];
    pWd->mYBBCamMtx[2][0] = 0.0f;
    pWd->mYBBCamMtx[2][1] = v.z;
    pWd->mYBBCamMtx[2][2] = v.y;
    pWd->mYBBCamMtx[2][3] = pWd->mPosCamMtx[2][3];
}
