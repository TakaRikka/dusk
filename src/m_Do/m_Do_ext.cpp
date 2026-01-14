/**
 * m_Do_ext.cpp
 * Model, Animation, and Heap Functions
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include <dolphin/gf/GFPixel.h>
#include <dolphin/gx.h>
#include "JSystem/J3DGraphAnimator/J3DMaterialAnm.h"
#include "JSystem/J3DGraphBase/J3DDrawBuffer.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "JSystem/J3DGraphLoader/J3DMaterialFactory.h"
#include "JSystem/JKernel/JKRAssertHeap.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "JSystem/JUtility/JUTCacheFont.h"
#include "JSystem/JUtility/JUTResFont.h"
#include "Z2AudioLib/Z2Creature.h"
#include "d/d_com_inf_game.h"
#include "global.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_main.h"
#include "m_Do/m_Do_mtx.h"
#include <cstdio>

namespace m_Do_ext {
u8 CurrentHeapAdjustVerbose;
u8 HeapAdjustVerbose;
u8 HeapAdjustQuiet;
}

static intptr_t safeGameHeapSize = -1;

static void mDoExt_setJ3DData(Mtx mtx, const J3DTransformInfo* transformInfo, u16 param_2) {
    bool local_28;
    if (cM3d_IsZero(transformInfo->mScale.x - 1.0f) && cM3d_IsZero(transformInfo->mScale.y - 1.0f) && cM3d_IsZero(transformInfo->mScale.z - 1.0f)) {
        j3dSys.getModel()->setScaleFlag(param_2, 1);
        local_28 = true;
    } else {
        j3dSys.getModel()->setScaleFlag(param_2, 0);
        local_28 = false;
    }
    mtx[0][3] = transformInfo->mTranslate.x;
    mtx[1][3] = transformInfo->mTranslate.y;
    mtx[2][3] = transformInfo->mTranslate.z;

    if (!local_28) {
        f32* mtx_p = (f32*)mtx;
        *mtx_p *= transformInfo->mScale.x;
        mtx_p++;
        *mtx_p *= transformInfo->mScale.y;
        mtx_p++;
        *mtx_p *= transformInfo->mScale.z;
        mtx_p += 2;
        *mtx_p *= transformInfo->mScale.x;
        mtx_p++;
        *mtx_p *= transformInfo->mScale.y;
        mtx_p++;
        *mtx_p *= transformInfo->mScale.z;
        mtx_p += 2;
        *mtx_p *= transformInfo->mScale.x;
        mtx_p++;
        *mtx_p *= transformInfo->mScale.y;
        mtx_p++;
        *mtx_p *= transformInfo->mScale.z;
    }
    if (j3dSys.getModel()->getModelData()->getJointNodePointer(param_2)->getScaleCompensate() == 1) {
        f32* mtx_p = (f32*)mtx;
        Vec sp0C;
        sp0C.x = 1.0f / J3DSys::mParentS.x;
        sp0C.y = 1.0f / J3DSys::mParentS.y;
        sp0C.z = 1.0f / J3DSys::mParentS.z;
        *mtx_p++ *= sp0C.x;
        *mtx_p++ *= sp0C.x;
        *mtx_p++ *= sp0C.x;
        *mtx_p++;
        *mtx_p++ *= sp0C.y;
        *mtx_p++ *= sp0C.y;
        *mtx_p++ *= sp0C.y;
        *mtx_p++;
        *mtx_p++ *= sp0C.z;
        *mtx_p++ *= sp0C.z;
        *mtx_p++ *= sp0C.z;
    }
    mDoMtx_concat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
    j3dSys.getModel()->setAnmMtx(param_2, J3DSys::mCurrentMtx);
    J3DSys::mParentS.x = transformInfo->mScale.x;
    J3DSys::mParentS.y = transformInfo->mScale.y;
    J3DSys::mParentS.z = transformInfo->mScale.z;
}

static BOOL isCurrentSolidHeap() {
    JKRHeap* heap = JKRGetCurrentHeap();
    if (heap->getHeapType() != 'SLID') {
        OS_REPORT_ERROR("ソリッドヒープちゃうがな！\n");
        return FALSE;
    }
    return TRUE;
}

int mDoExt_baseAnm::initPlay(s16 i_frameMax, int i_attribute, f32 i_rate, s16 i_startF,
                             s16 i_endF) {
    mFrameCtrl.setStart(i_startF);

    if (i_endF < 0) {
        mFrameCtrl.init(i_frameMax);
    } else {
        mFrameCtrl.init(i_endF);
    }

    mFrameCtrl.setAttribute(i_attribute);
    mFrameCtrl.setRate(i_rate);

    if (i_rate >= 0.0f) {
        mFrameCtrl.setFrame(i_startF);
    } else {
        mFrameCtrl.setFrame(mFrameCtrl.getEnd());
    }

    mFrameCtrl.setLoop(mFrameCtrl.getFrame());

    return 1;
}

int mDoExt_baseAnm::play() {
    mFrameCtrl.update();
    return isStop();
}

int mDoExt_bpkAnm::init(J3DMaterialTable* i_matTable, J3DAnmColor* i_bpk, int i_anmPlay,
                        int i_attribute, f32 i_rate, s16 i_startF, s16 i_endF) {
    JUT_ASSERT(371, (i_anmPlay == FALSE || getFrameCtrl() != NULL || isCurrentSolidHeap()) && i_matTable != NULL && i_bpk != NULL);

    mpAnm = i_bpk;
    mpAnm->searchUpdateMaterialID(i_matTable);

    if (i_anmPlay) {
        return initPlay(mpAnm->getFrameMax(), i_attribute < 0 ? mpAnm->getAttribute() : i_attribute,
                        i_rate, i_startF, i_endF);
    }

    return 1;
}

void mDoExt_bpkAnm::entry(J3DMaterialTable* i_matTable, f32 i_frame) {
    mpAnm->setFrame(i_frame);
    i_matTable->entryMatColorAnimator(mpAnm);
}

int mDoExt_btpAnm::init(J3DMaterialTable* i_matTable, J3DAnmTexPattern* i_btp, int i_anmPlay,
                        int i_attribute, f32 i_rate, s16 i_startF, s16 i_endF) {
    JUT_ASSERT(420, (i_anmPlay == FALSE || getFrameCtrl() != NULL || isCurrentSolidHeap()) && i_matTable != NULL && i_btp != NULL);

    mpAnm = i_btp;
    mpAnm->searchUpdateMaterialID(i_matTable);

    if (i_anmPlay) {
        return initPlay(mpAnm->getFrameMax(), i_attribute < 0 ? mpAnm->getAttribute() : i_attribute, i_rate, i_startF, i_endF);
    }

    return 1;
}

void mDoExt_btpAnm::entry(J3DMaterialTable* i_matTable, s16 i_frame) {
    mpAnm->setFrame(i_frame);
    i_matTable->entryTexNoAnimator(mpAnm);
}

int mDoExt_btkAnm::init(J3DMaterialTable* i_matTable, J3DAnmTextureSRTKey* i_btk, int i_anmPlay,
                        int i_attribute, f32 i_rate, s16 i_startF, s16 i_endF) {
    JUT_ASSERT(468, (i_anmPlay == FALSE || getFrameCtrl() != NULL || isCurrentSolidHeap()) && i_matTable != NULL && i_btk != NULL);

    mpAnm = i_btk;
    mpAnm->searchUpdateMaterialID(i_matTable);

    if (i_anmPlay) {
        return initPlay(mpAnm->getFrameMax(), i_attribute < 0 ? mpAnm->getAttribute() : i_attribute, i_rate, i_startF, i_endF);
    }

    return 1;
}

void mDoExt_btkAnm::entry(J3DMaterialTable* i_matTable, f32 i_frame) {
    mpAnm->setFrame(i_frame);
    i_matTable->entryTexMtxAnimator(mpAnm);
}

int mDoExt_brkAnm::init(J3DMaterialTable* i_matTable, J3DAnmTevRegKey* i_brk, int i_anmPlay,
                        int i_attribute, f32 i_rate, s16 i_startF, s16 i_endF) {
    JUT_ASSERT(516, (i_anmPlay == FALSE || getFrameCtrl() != NULL || isCurrentSolidHeap()) && i_matTable != NULL && i_brk != NULL);

    mpAnm = i_brk;
    mpAnm->searchUpdateMaterialID(i_matTable);

    if (i_anmPlay) {
        return initPlay(mpAnm->getFrameMax(), i_attribute < 0 ? mpAnm->getAttribute() : i_attribute, i_rate, i_startF, i_endF);
    }

    return 1;
}

void mDoExt_brkAnm::entry(J3DMaterialTable* i_matTable, f32 i_frame) {
    mpAnm->setFrame(i_frame);
    i_matTable->entryTevRegAnimator(mpAnm);
}

void dummy3(J3DModel* i_model, void* i_bva, bool i_modify) {
    JUT_ASSERT(0, (i_modify || isCurrentSolidHeap()) && i_model != NULL && i_bva != NULL);
}

int mDoExt_bckAnm::init(J3DAnmTransform* i_bck, int i_play, int i_attr, f32 i_rate,
                        s16 i_startF, s16 i_endF, bool i_modify) {
    JUT_ASSERT(614, (i_modify || isCurrentSolidHeap()) && i_bck != NULL);
    mAnmTransform = i_bck;
    if (!i_modify) {
        mAnm = new J3DMtxCalcAnimation<J3DMtxCalcAnimationAdaptorDefault<J3DMtxCalcCalcTransformMaya>, J3DMtxCalcJ3DSysInitMaya>(mAnmTransform);
        if (!mAnm) {
            return 0;
        }
    } else {
        mAnm->setAnmTransform(mAnmTransform);
    }
    if (i_play) {
        return initPlay(mAnmTransform->getFrameMax(), i_attr < 0 ? mAnmTransform->getAttribute() : i_attr, i_rate, i_startF, i_endF);
    }
    return 1;
}

void mDoExt_bckAnm::changeBckOnly(J3DAnmTransform* i_bck) {
    mAnmTransform = i_bck;
    JUT_ASSERT(646, mAnm != 0);
    mAnm->setAnmTransform(mAnmTransform);
}

void dummy4(J3DModel* i_model, void* i_bxa, bool i_modify) {
    JUT_ASSERT(0, (i_modify || isCurrentSolidHeap()) && i_model != NULL && i_bxa != NULL);
}

void mDoExt_bckAnm::entry(J3DModelData* i_modelData, f32 i_frame) {
    mAnmTransform->setFrame(i_frame);
    i_modelData->getJointNodePointer(0)->setMtxCalc(mAnm);
}

void mDoExt_bckAnm::entryJoint(J3DModelData* i_modelData, u16 i_jntNo, f32 i_frame) {
    mAnmTransform->setFrame(i_frame);
    i_modelData->getJointNodePointer(i_jntNo)->setMtxCalc(mAnm);
}

int mDoExt_blkAnm::init(J3DDeformData* i_deformData, J3DAnmCluster* i_blk, int i_anmPlay,
                        int i_attribute, f32 i_rate, s16 i_start, s16 param_6) {
    JUT_ASSERT(747, (i_anmPlay == FALSE || getFrameCtrl() != NULL || isCurrentSolidHeap()) && i_deformData != NULL && i_blk != NULL);

    mpAnm = i_blk;
    i_deformData->setAnm(mpAnm);

    if (i_anmPlay) {
        return initPlay(mpAnm->getFrameMax(), i_attribute < 0 ? mpAnm->getAttribute() : i_attribute, i_rate, i_start, param_6);
    }

    return 1;
}

static void mDoExt_changeMaterial(J3DModel* i_model) {
    J3DModelData* model_data = i_model->getModelData();

    for (u16 i = 0; i < model_data->getMaterialNum(); i++) {
        model_data->getMaterialNodePointer(i)->change();
    }
}

void mDoExt_modelTexturePatch(J3DModelData* i_modelData) {
    j3dSys.setTexture(i_modelData->getTexture());

    for (u16 i = 0; i < i_modelData->getMaterialNum(); i++) {
        J3DMaterial* mat = i_modelData->getMaterialNodePointer(i);
        J3DTevBlock* tev = mat->getTevBlock();
        J3DDisplayListObj* dlObj = mat->getSharedDisplayListObj();

        BOOL ret = OSDisableInterrupts();
        GDInitGDLObj(&J3DDisplayListObj::sGDLObj, dlObj->getDisplayList(0),
                     dlObj->getDisplayListSize());
        GDSetCurrent(&J3DDisplayListObj::sGDLObj);
        tev->patchTexNoAndTexCoordScale();
        OSRestoreInterrupts(ret);
        GDSetCurrent(NULL);
    }
}

static void dummy1(J3DModel* model, J3DMaterial* material) {
    J3DMatPacket* packet = model->getMatPacket(0);
    packet->beginPatch();
    material->getTevBlock()->patch();
    material->getColorBlock()->patchLight();
    material->getTexGenBlock();
    material->getPEBlock()->patch();
    packet->endPatch();
}

static inline void modelMtxErrorCheck(J3DModel* i_model) {
#if DEBUG
    if (i_model->getBaseTRMtx()[0][0] == FLT_MAX) {
        i_model->getBaseTRMtx()[0][0] = 1.0f;
        JUT_WARN(883, "%s", "Model Matrix Not Initialize !\n");
    }
#endif
}

static void mDoExt_modelDiff(J3DModel* i_model) {
    modelMtxErrorCheck(i_model);
    i_model->calcMaterial();
    i_model->diff();
    i_model->entry();
}

void mDoExt_modelUpdate(J3DModel* i_model) {
    modelMtxErrorCheck(i_model);

    J3DModelData* model_data = i_model->getModelData();

    if (model_data->getMaterialNodePointer(0)->getSharedDisplayListObj() != NULL &&
        !model_data->isLocked())
    {
        i_model->calc();
        mDoExt_modelDiff(i_model);
    } else {
        i_model->update();
        i_model->lock();
    }

    i_model->viewCalc();
}

void mDoExt_modelUpdateDL(J3DModel* i_model) {
    modelMtxErrorCheck(i_model);

    J3DModelData* model_data = i_model->getModelData();

    if (model_data->getMaterialNodePointer(0)->getSharedDisplayListObj() != NULL &&
        !model_data->isLocked())
    {
        i_model->calc();
        mDoExt_modelDiff(i_model);
    } else {
        i_model->unlock();
        i_model->update();
        i_model->lock();
    }

    i_model->viewCalc();
}

void mDoExt_modelEntryDL(J3DModel* i_model) {
    modelMtxErrorCheck(i_model);

    J3DModelData* model_data = i_model->getModelData();

    if (model_data->getMaterialNodePointer(0)->getSharedDisplayListObj() != NULL &&
        !model_data->isLocked())
    {
        mDoExt_modelDiff(i_model);
    } else {
        i_model->unlock();
        i_model->entry();
        i_model->lock();
    }

    i_model->viewCalc();
}

void mDoExt_btkAnmRemove(J3DModelData* i_modelData) {
    for (u16 i = 0; i < i_modelData->getMaterialNum(); i++) {
        J3DMaterialAnm* matAnm = i_modelData->getMaterialNodePointer(i)->getMaterialAnm();

        if (matAnm != NULL) {
            for (u16 j = 0; (int)j < 8; j++) {
                matAnm->setTexMtxAnm(j, NULL);
            }
        }
    }
}

void mDoExt_brkAnmRemove(J3DModelData* i_modelData) {
    for (u16 i = 0; i < i_modelData->getMaterialNum(); i++) {
        J3DMaterialAnm* matAnm = i_modelData->getMaterialNodePointer(i)->getMaterialAnm();

        if (matAnm != NULL) {
            for (u16 j = 0; (int)j < 4; j++) {
                matAnm->setTevColorAnm(j, NULL);
                matAnm->setTevKColorAnm(j, NULL);
            }
        }
    }
}

// this needs a lot of work still
static void dummy2() {
    FORCE_DONT_INLINE;
    FORCE_DONT_INLINE;
    FORCE_DONT_INLINE;
    FORCE_DONT_INLINE;
    FORCE_DONT_INLINE;
    FORCE_DONT_INLINE;
    FORCE_DONT_INLINE;
    FORCE_DONT_INLINE;
    FORCE_DONT_INLINE;
    FORCE_DONT_INLINE;
    FORCE_DONT_INLINE;
    FORCE_DONT_INLINE;
    FORCE_DONT_INLINE;
    FORCE_DONT_INLINE;

    ((J3DMaterial*)NULL)->getMaterialMode();

    J3DGXColor gxColor;
    J3DColorChan colorChan;
    J3DTexCoord texCoord;
    J3DGXColorS10 gxColorS10;
    J3DTevStageInfo tevStageInfo;
    J3DTevStage* stage_p = NULL;
    stage_p->setTevStageInfo(tevStageInfo);
    J3DTevStage tevStage;
    J3DIndTevStage indStage;
    J3DIndTevStage indTevStage;
    J3DTevOrder tevOrder;
    J3DTevSwapModeTable tevSwapModeTable;
    J3DIndTexOrder indTexOrder;

    J3DIndTexMtx* indTexMtx = new J3DIndTexMtx();
    J3DIndTexCoordScale* indTexCoordScale = new J3DIndTexCoordScale();
    J3DColorBlock* colorBlock = NULL;
    colorBlock->setColorChanNum((u8)NULL);
    colorBlock->setMatColor(0, gxColor);
    colorBlock->setColorChan(0, colorChan);
    colorBlock->setAmbColor(0, gxColor);

    J3DTexGenBlock* texGenBlock = NULL;
    texGenBlock->setTexGenNum((u32)NULL);
    texGenBlock->setTexCoord(0, NULL);

    J3DTevBlock* tevBlock = NULL;
    tevBlock->setTevStageNum((u8)0);
    tevBlock->setTevColor(0, J3DGXColorS10());
    tevBlock->setTevKColor(0, gxColor);
    tevBlock->setTevOrder(0, tevOrder);
    tevBlock->setTevKColorSel(0, (u8)0);
    tevBlock->setTevKAlphaSel(0, (u8)0);
    tevBlock->setTevSwapModeTable(0, tevSwapModeTable);
    tevBlock->setTexNo(0, (u16)0);
    tevBlock->setTevStage(0, tevStage);
    tevBlock->setIndTevStage(0, indTevStage);

    J3DIndBlock* indBlock = NULL;
    indBlock->setIndTexStageNum(0);
    indBlock->setIndTexMtx(0, *indTexMtx);
    indBlock->setIndTexCoordScale(0, *indTexCoordScale);

    J3DPEBlock* peBlock = NULL;
    J3DAlphaComp alphaComp;
    peBlock->setAlphaComp(alphaComp);
    J3DBlend blend;
    peBlock->setBlend(blend);
    J3DZMode zMode;
    peBlock->setZMode(zMode);
    u8 compLoc = 0;
    peBlock->setZCompLoc(compLoc);

    colorBlock->getColorChanNum();
    colorBlock->getMatColor(0);
    colorBlock->getColorChan(0);
    colorBlock->getAmbColor(0);

    texGenBlock->getTexGenNum();
    texGenBlock->getTexCoord(0);
    texGenBlock->getTexMtx(0);

    tevBlock->getTevStageNum();
    tevBlock->getTevColor(0);
    tevBlock->getTevKColor(0);
    tevBlock->getTevOrder(0);
    tevBlock->getTevKColorSel(0);
    tevBlock->getTevKAlphaSel(0);
    tevBlock->getTevSwapModeTable(0);
    tevBlock->getTexNo(0);
    tevBlock->getTevStage(0);
    tevBlock->getIndTevStage(0);

    indBlock = NULL;
    indBlock->getIndTexStageNum();
    indBlock->getIndTexOrder(0);
    indBlock->getIndTexMtx(0);
    indBlock->getIndTexCoordScale(0);

    indTexOrder.getMap();
    indTexOrder.getCoord();

    peBlock = NULL;
    peBlock->getFog();
    peBlock->getAlphaComp();
    peBlock->getBlend();
    peBlock->getZMode();
    peBlock->getZCompLoc();

    ((J3DMaterial*)NULL)->setMaterialAnm(NULL);
}

int mDoExt_invisibleModel::create(J3DModel* i_model, u8 param_1) {
    J3DModelData* model_data = i_model->getModelData();

    mpPackets = new mDoExt_invJntPacket[model_data->getJointNum()];
    if (mpPackets == NULL) {
        return 0;
    }

    mModel = i_model;
    mDoExt_invJntPacket* packet = mpPackets;

    for (u16 i = 0; i < model_data->getJointNum(); i++) {
        packet->setJoint(mModel, i, param_1);
        packet++;
    }

    return 1;
}

void mDoExt_invisibleModel::entryJoint(cXyz* param_0) {
    J3DModelData* modelData = mModel->getModelData();
    mDoExt_invJntPacket* packet = mpPackets;

    if (param_0 == NULL) {
        cXyz tmp;
        for (u16 i = 0; i < modelData->getJointNum(); i++) {
            MtxP anmMtx = mModel->getAnmMtx(i);
            tmp.set(anmMtx[0][3], anmMtx[1][3], anmMtx[2][3]);
            dComIfGd_entryZSortListZxlu(packet, tmp);
            packet++;
        }
    } else {
        for (u16 i = 0; i < modelData->getJointNum(); i++) {
            dComIfGd_entryZSortListZxlu(packet, *param_0);
            packet++;
        }
    }
}

void mDoExt_invisibleModel::entryDL(cXyz* param_0) {
    J3DDrawBuffer* buffer0 = j3dSys.getDrawBuffer(0);
    J3DDrawBuffer* buffer1 = j3dSys.getDrawBuffer(1);
    dComIfGd_setListZxlu();
    mDoExt_modelEntryDL(mModel);
    entryJoint(param_0);
    j3dSys.setDrawBuffer(buffer0, 0);
    j3dSys.setDrawBuffer(buffer1, 1);
}

void mDoExt_setupShareTexture(J3DModelData* i_modelData, J3DModelData* i_shareModelData) {
    JUT_ASSERT(1547, i_modelData != NULL && i_shareModelData != NULL);
    J3DTexture* texture = i_modelData->getTexture();
    JUT_ASSERT(1549, texture != NULL);
    JUTNameTab* textureName = i_modelData->getTextureName();
    JUT_ASSERT(1551, textureName != NULL);
    J3DTexture* shareTexture = i_shareModelData->getTexture();
    JUT_ASSERT(1553, shareTexture != NULL);
    JUTNameTab* shareTextureName = i_shareModelData->getTextureName();
    JUT_ASSERT(1555, shareTextureName != NULL)

    bool bvar = false;
    for (u16 i = 0; i < texture->getNum(); i++) {
        if (texture->getResTIMG(i)->imageOffset == 0) {
            for (u16 j = 0; j < shareTexture->getNum(); j++) {
                if (!strcmp(textureName->getName(i), shareTextureName->getName(j))) {
                    JUT_ASSERT(1564, shareTexture->getResTIMG(j)->imageOffset != 0);
                    texture->setResTIMG(i, *shareTexture->getResTIMG(j));
                    bvar = true;
                }
            }
        }
    }

    if (bvar) {
        j3dSys.setTexture(texture);

        for (u16 i = 0; i < i_modelData->getMaterialNum(); i++) {
            J3DMaterial* mat = i_modelData->getMaterialNodePointer(i);
            J3DTevBlock* tevBlock = mat->getTevBlock();
            JUT_ASSERT(1577, tevBlock != NULL);
            J3DDisplayListObj* dlObj = mat->getSharedDisplayListObj();

            BOOL ret = OSDisableInterrupts();
            GDInitGDLObj(&J3DDisplayListObj::sGDLObj, dlObj->getDisplayList(0),
                         dlObj->getDisplayListSize());
            GDSetCurrent(&J3DDisplayListObj::sGDLObj);
            tevBlock->patchTexNoAndTexCoordScale();
            OSRestoreInterrupts(ret);
            GDSetCurrent(NULL);
        }
    }
}

static const char* dummy3() {
    // MWCC ignores mapping of some japanese characters using the
    // byte 0x5C (ASCII '\'). This is why this string is hex-encoded.

    // "ソリッドヒープちゃうがな！\n"
    // "This isn't a solid heap! \n"
    return "\x83\x5C\x83\x8A\x83\x62\x83\x68\x83\x71\x81\x5B\x83\x76\x82\xBF\x82\xE1\x82\xA4\x82"
        "\xAA\x82\xC8\x81\x49\x0A";
}

void mDoExt_setupStageTexture(J3DModelData* i_modelData) {
    J3DTexture* texture_p = i_modelData->getTexture();

    if (texture_p != NULL) {
        JUTNameTab* tex_name_p = i_modelData->getTextureName();
        if (tex_name_p != NULL) {
            bool set_timg = false;

            for (u16 i = 0; i < texture_p->getNum(); i++) {
                if (texture_p->getResTIMG(i)->imageOffset == 0) {
                    char res_name[64];
                    snprintf(res_name, sizeof(res_name), "%s.bti", tex_name_p->getName(i));
                    OS_REPORT("%d:設定テクスチャー名<%s>\n", i, res_name);

                    ResTIMG* timg = (ResTIMG*)dComIfG_getStageRes(res_name);
                    JUT_ASSERT(1623, timg != NULL);
                    texture_p->setResTIMG(i, *timg);
                    set_timg = true;
                }
            }

            if (set_timg) {
                j3dSys.setTexture(texture_p);

                for (u16 i = 0; i < i_modelData->getMaterialNum(); i++) {
                    J3DMaterial* mat_p = i_modelData->getMaterialNodePointer(i);
                    J3DTevBlock* block_p = mat_p->getTevBlock();

                    if (block_p != NULL) {
                        J3DDisplayListObj* dlObj_p = mat_p->getSharedDisplayListObj();

                        if (dlObj_p != NULL) {
                            BOOL state = OSDisableInterrupts();

                            GDInitGDLObj(&J3DDisplayListObj::sGDLObj, dlObj_p->getDisplayList(0),
                                         dlObj_p->getDisplayListSize());
                            GDSetCurrent(&J3DDisplayListObj::sGDLObj);
                            block_p->patchTexNoAndTexCoordScale();

                            OSRestoreInterrupts(state);
                            GDSetCurrent(NULL);
                        }
                    }
                }
            }
        }
    }
}

static JKRAssertHeap* AssertHeap;

JKRAssertHeap* mDoExt_createAssertHeap(JKRHeap* parentHeap) {
    JUT_ASSERT(1666, AssertHeap == NULL);
    AssertHeap = JKRAssertHeap::create(parentHeap);
    return AssertHeap;
}

JKRAssertHeap* mDoExt_getAssertHeap() {
    return AssertHeap;
}

static JKRExpHeap* DbPrintHeap;

JKRExpHeap* mDoExt_createDbPrintHeap(u32 heapSize, JKRHeap* parentHeap) {
    JUT_ASSERT(1693, DbPrintHeap == NULL || heapSize == 0);
    DbPrintHeap = JKRExpHeap::create(heapSize, parentHeap, true);
    return DbPrintHeap;
}

JKRExpHeap* mDoExt_getDbPrintHeap() {
    return DbPrintHeap;
}

JKRExpHeap* gameHeap;

JKRExpHeap* mDoExt_createGameHeap(u32 heapSize, JKRHeap* parentHeap) {
    JUT_ASSERT(1739, gameHeap == NULL || heapSize == 0);
    gameHeap = JKRExpHeap::create(heapSize, parentHeap, true);
    gameHeap->setAllocationMode(JKRExpHeap::ALLOC_MODE_1);
    return gameHeap;
}

JKRExpHeap* mDoExt_getGameHeap() {
    return gameHeap;
}

void mDoExt_setSafeGameHeapSize() {
    if (safeGameHeapSize < 0) {
        safeGameHeapSize = mDoExt_getGameHeap()->getTotalUsedSize();
        OS_REPORT("++++++++++++++++++++++++++++++ GameHeap Rock !!\n");
        mDoExt_getGameHeap()->dump();
        mDoExt_getGameHeap()->changeGroupID(7);
    }
}

void dummy5() {
    JUT_ASSERT(0, safeGameHeapSize >= 0);
    OS_REPORT("++++++++++++++++++++++++++++++ safeGameHeapSize <%x>\n", safeGameHeapSize);
}

size_t mDoExt_getSafeGameHeapSize() {
    return safeGameHeapSize;
}

JKRExpHeap* zeldaHeap;
intptr_t safeZeldaHeapSize = -1;

JKRExpHeap* mDoExt_createZeldaHeap(u32 heapSize, JKRHeap* parentHeap) {
    JUT_ASSERT(1815, zeldaHeap == NULL || heapSize == 0);
    zeldaHeap = JKRExpHeap::create(heapSize, parentHeap, true);
    return zeldaHeap;
}

JKRExpHeap* mDoExt_getZeldaHeap() {
    return zeldaHeap;
}

void mDoExt_setSafeZeldaHeapSize() {
    if (safeGameHeapSize < 0) {
        safeGameHeapSize = mDoExt_getGameHeap()->getTotalUsedSize();
        OS_REPORT("++++++++++++++++++++++++++++++ ZeldaHeap Rock !!\n");
        mDoExt_getGameHeap()->dump();
        mDoExt_getGameHeap()->changeGroupID(7);
    }
}

void mDoExt_addSafeZeldaHeapSize(intptr_t size) {
    JUT_ASSERT(1869, safeZeldaHeapSize >= 0);
    safeZeldaHeapSize -= size;
    OS_REPORT("++++++++++++++++++++++++++++++ addSafeZeldaHeapSize <%x>\n", -size);
    mDoExt_getZeldaHeap()->changeGroupID(mDoExt_getZeldaHeap()->getCurrentGroupId() + 1);
}

intptr_t mDoExt_getSafeZeldaHeapSize() {
    return safeZeldaHeapSize;
}

JKRExpHeap* commandHeap;
intptr_t safeCommandHeapSize = -1;

JKRExpHeap* mDoExt_createCommandHeap(u32 heapSize, JKRHeap* parentHeap) {
    JUT_ASSERT(1894, commandHeap == 0 || heapSize == 0);
    commandHeap = JKRExpHeap::create(heapSize, parentHeap, true);
    return commandHeap;
}

JKRExpHeap* mDoExt_getCommandHeap() {
    return commandHeap;
}

void mDoExt_setSafeCommandHeapSize() {
    if (safeCommandHeapSize < 0) {
        safeCommandHeapSize = mDoExt_getGameHeap()->getTotalUsedSize();
        OS_REPORT("++++++++++++++++++++++++++++++ CommandHeap Rock !!\n");
        mDoExt_getGameHeap()->dump();
        mDoExt_getGameHeap()->changeGroupID(7);
    }
}

intptr_t mDoExt_getSafeCommandHeapSize() {
    return safeCommandHeapSize;
}

JKRExpHeap* archiveHeap;
intptr_t safeArchiveHeapSize = -1;

JKRExpHeap* mDoExt_createArchiveHeap(u32 heapSize, JKRHeap* parentHeap) {
    JUT_ASSERT(1966, archiveHeap == 0 || heapSize == 0);
    archiveHeap = JKRExpHeap::create(heapSize, parentHeap, true);
    archiveHeap->setAllocationMode(JKRExpHeap::ALLOC_MODE_1);
    return archiveHeap;
}

void mDoExt_setSafeArchiveHeapSize() {
    if (safeArchiveHeapSize < 0) {
        safeArchiveHeapSize = mDoExt_getArchiveHeap()->getTotalUsedSize();
        OS_REPORT("++++++++++++++++++++++++++++++ ArchiveHeap Rock !!\n");
        mDoExt_getArchiveHeap()->dump();
        mDoExt_getArchiveHeap()->changeGroupID(7);
    }
}

void dummy6() {
    JUT_ASSERT(0, safeArchiveHeapSize >= 0);
    OS_REPORT("++++++++++++++++++++++++++++++ safeArchiveHeapSize <%x>\n", safeArchiveHeapSize);
}

intptr_t mDoExt_getSafeArchiveHeapSize() {
    return safeArchiveHeapSize;
}

JKRExpHeap* mDoExt_getArchiveHeap() {
    JKRExpHeap* heap = archiveHeap;
    return heap;
}

JKRExpHeap* mDoExt_getArchiveHeapPtr() {
    return archiveHeap;
}

static JKRExpHeap* j2dHeap;
intptr_t safeJ2dHeapSize = -1;

JKRExpHeap* mDoExt_createJ2dHeap(u32 heapSize, JKRHeap* parentHeap) {
    JUT_ASSERT(2059, j2dHeap == 0 || heapSize == 0);
    j2dHeap = JKRExpHeap::create(heapSize, parentHeap, true);
    j2dHeap->setAllocationMode(JKRExpHeap::ALLOC_MODE_1);
    return j2dHeap;
}

void mDoExt_setSafeJ2dHeapSize() {
    if (safeJ2dHeapSize < 0) {
        safeJ2dHeapSize = mDoExt_getJ2dHeap()->getTotalUsedSize();
        OS_REPORT("++++++++++++++++++++++++++++++ j2dHeap Rock !!\n");
        mDoExt_getJ2dHeap()->dump();
        mDoExt_getJ2dHeap()->changeGroupID(7);
    }
}

void dummy7() {
    JUT_ASSERT(0, safeJ2dHeapSize >= 0);
    OS_REPORT("++++++++++++++++++++++++++++++ safeJ2dHeapSize <%x>\n", safeJ2dHeapSize);
}

intptr_t mDoExt_getSafeJ2dHeapSize() {
    return safeJ2dHeapSize;
}

JKRExpHeap* mDoExt_getJ2dHeap() {
    JKRExpHeap* heap = j2dHeap;
    return heap;
}

static JKRExpHeap* HostIOHeap;

JKRHeap* mDoExt_createHostIOHeap(u32 heapSize, JKRHeap* parentHeap) {
    JUT_ASSERT(2142, HostIOHeap == 0 || heapSize == 0);
    HostIOHeap = JKRExpHeap::create(heapSize, parentHeap, true);
    return HostIOHeap;
}

JKRExpHeap* mDoExt_getHostIOHeap() {
    return HostIOHeap;
}

static JKRHeap* situationHeap;

void dummy8(u32 heapSize) {
    int situationHeapLocked = 0;
    JUT_ASSERT(0, situationHeap == 0 || heapSize == 0);
    JUT_ASSERT(0, situationHeapLocked == 0);
    JUT_ASSERT(0, situationHeap);
    JUT_ASSERT(0, situationHeapLocked);
    JKRHeap* heap = NULL;
    JUT_ASSERT(0, heap == situationHeap);
}

static JKRHeap* mDoExt_SaveCurrentHeap;

JKRSolidHeap* mDoExt_createSolidHeap(u32 i_size, JKRHeap* i_parent, u32 i_alignment) {
    if (i_parent == NULL) {
        i_parent = JKRGetCurrentHeap();
    }

    JKRSolidHeap* createdHeap;
    if (i_size == 0 || i_size == -1) {
        #if DEBUG
        if (mDoExt::HeapAdjustVerbose) {
            // "mDoExt_createSolidHeap size not set"
            OS_REPORT("\x1b[44mmDoExt_createSolidHeap サイズ未設定\n\x1b[m");
            // "Ensure that the maximum available space is allocated %08x"
            OS_REPORT("最大空き容量確保します %08x\n\x1b[m", i_parent->getFreeSize());
        }
        #endif
        createdHeap = JKRCreateSolidHeap(-1, i_parent, false);
    } else {
        u32 solidHeapSize = ALIGN_NEXT(sizeof(JKRSolidHeap), 0x10);
        i_size = ALIGN_NEXT(i_size, 0x10);
        i_size += solidHeapSize;

        if (0x10 < i_alignment) {
            i_size = (i_alignment - 0x10 + i_size);
        }
        createdHeap = JKRCreateSolidHeap(i_size, i_parent, false);
    }

    if (createdHeap != NULL) {
        JKRSetErrorFlag(createdHeap, true);
        #if DEBUG
        if (mDoExt::HeapAdjustVerbose != 0) {
            u32 solidHeapSize = createdHeap->getHeapSize();
            OS_REPORT(
                "JKRCreateSolidHeap %08x i_size=%08x solidHeapSize=%08x\n",
                createdHeap,
                i_size,
                solidHeapSize
            );
        }
        #endif
    }

#if DEBUG
    if (createdHeap == NULL) {
        OS_WARNING(
            "mDoExt_createMaxSolidHeap : ソリッドヒープ%fKの確保に失敗 連続空き容量=%fK 残り空き容量=%f\n",
            i_size / 1024.0f,
            i_parent->getFreeSize() / 1024.0f,
            i_parent->getTotalFreeSize() / 1024.0f
        );
        static BOOL displayed = FALSE;
        if (displayed == FALSE) {
            displayed = TRUE;
            i_parent->dump_sort();
        }
        return createdHeap;
    }
#endif

    return createdHeap;
}

JKRSolidHeap* mDoExt_createSolidHeapFromGame(u32 i_size, u32 i_alignment) {
    return mDoExt_createSolidHeap(i_size, mDoExt_getGameHeap(), i_alignment);
}

JKRSolidHeap* mDoExt_createSolidHeapFromSystem(u32 i_size, u32 i_alignment) {
    return mDoExt_createSolidHeap(i_size, mDoExt_getZeldaHeap(), i_alignment);
}

JKRSolidHeap* mDoExt_createSolidHeapToCurrent(JKRHeap** o_heap, u32 i_size, JKRHeap* i_parent,
                                              u32 i_alignment) {
    if (o_heap != NULL) {
        *o_heap = JKRGetCurrentHeap();
    }

    JKRSolidHeap* resultHeap = mDoExt_createSolidHeap(i_size, i_parent, i_alignment);
    if (resultHeap != NULL) {
        mDoExt_setCurrentHeap(resultHeap);
    }

    return resultHeap;
}

JKRSolidHeap* mDoExt_createSolidHeapToCurrent(u32 i_size, JKRHeap* i_parent, u32 i_alignment) {
    JUT_ASSERT(2427, OSGetCurrentThread() == &mainThread);
    JUT_ASSERT(2428, mDoExt_SaveCurrentHeap == NULL);
    JKRSolidHeap* heap =
        mDoExt_createSolidHeapToCurrent(&mDoExt_SaveCurrentHeap, i_size, i_parent, i_alignment);
    if (mDoExt::CurrentHeapAdjustVerbose) {
        // "Saving the current heap %08x"
        OS_REPORT("カレントヒープを保存 %08x\n", mDoExt_SaveCurrentHeap);
    }
    return heap;
}

JKRSolidHeap* mDoExt_createSolidHeapFromGameToCurrent(JKRHeap** o_heap, u32 i_size,
                                                      u32 i_alignment) {
    JKRExpHeap* gameHeap = mDoExt_getGameHeap();
    JKRSolidHeap* solidHeap =
        mDoExt_createSolidHeapToCurrent(o_heap, i_size, gameHeap, i_alignment);
    return solidHeap;
}

JKRSolidHeap* mDoExt_createSolidHeapFromGameToCurrent(u32 i_size, u32 i_alignment) {
    JKRExpHeap* gameHeap = mDoExt_getGameHeap();
    JKRSolidHeap* solidHeap = mDoExt_createSolidHeapToCurrent(i_size, gameHeap, i_alignment);
    return solidHeap;
}

u32 mDoExt_adjustSolidHeap(JKRSolidHeap* i_heap) {
    if (i_heap == NULL) {
        return -1;
    }

    JKRHeap* parentHeap = i_heap->getParent();
    if (parentHeap == NULL || parentHeap->getHeapType() != 'EXPH') {
        // "Can't adjust because parent heap isn't Exp %08x %08x\n"
        OSReport_Error("親ヒープがExpでないのでアジャストできません %08x %08x\n", i_heap,
                       parentHeap);
        return -1;
    }

    u32 estimatedSize = i_heap->getHeapSize();
    s32 result = i_heap->adjustSize();
    if (result < 0) {
        // "adjustSize failure %08x\n"
        OSReport_Error("adjustSize失敗 %08x\n", i_heap);
        return -1;
    }
    u32 actualSize = i_heap->getHeapSize();

    #if DEBUG
    if (mDoExt::HeapAdjustVerbose) {
        // "\x1B[33mSolid heap estimate: %08x actual: %08x\n\x1B[m"
        OS_REPORT("\x1B[33mソリッドヒープの見積もり %08x 実際 %08x\n\x1B[m", estimatedSize, actualSize);

        if (estimatedSize > 0x400 && estimatedSize > (actualSize + 0x400)) {
            // "\x1B[33mThe estimate is %x bytes too large (%5.2f times). This could lead to degraded memory efficiency.\n\x1B[m"
            OS_REPORT("\x1B[33m見積もりが %x バイト大きい(%5.2f倍)です。メモリ効率悪化が懸念されます。\n\x1B[m", estimatedSize - actualSize, (f32)estimatedSize / (f32)actualSize);
        } else if (estimatedSize >= (actualSize + 0x20)) {
            // "\x1B[36mThe estimate is %x bytes too large. That's pretty good.\n\x1B[m"
            OS_REPORT("\x1B[36m見積もりが %x バイト大きいです。だいぶいい感じです。\n\x1B[m", estimatedSize - actualSize);
        } else {
            // "\x1B[32mThe estimate is spot on! Perfect.\n\x1B[m"
            OS_REPORT("\x1B[32m見積もりが、ジャストミート！完璧です。\n\x1B[m");
        }
    }

    if (mDoExt::HeapAdjustVerbose) {
        OS_REPORT("JKRSolidHeap::adjustSize %08x (%08x bytes)\n", (u32)i_heap, result);
    }
    #endif

    // this probably indicates that 0x80 is some constant, maybe from a sizeof(JKRSolidHeap)
    // with alignment?
    u32 r25 = 0x80;
    if (result >= r25) {
        return result - r25;
    }

    return result;
}

u32 mDoExt_adjustSolidHeapToSystem(JKRSolidHeap* i_heap) {
    u32 result = mDoExt_adjustSolidHeap(i_heap);
    mDoExt_restoreCurrentHeap();
    return result;
}

void mDoExt_destroySolidHeap(JKRSolidHeap* i_heap) {
    JKRDestroySolidHeap(i_heap);
    if (g_printOtherHeapDebug) {
        // "Solid heap destruction"
        OS_REPORT("mDoExt_destroySolidHeap ソリッドヒープ破壊 %08x\n", i_heap);
    }
}

void mDoExt_destroyExpHeap(JKRExpHeap* i_heap) {
    ASSERTLINE(2576, i_heap != mDoExt_getCurrentHeap());
    JKRDestroyExpHeap(i_heap);
#if DEBUG
    if (mDoExt::HeapAdjustVerbose) {
        OS_REPORT("mDoExt_destroyExpHeap Expヒープ破壊 %08x\n", i_heap);
    }
#endif
}

JKRHeap* mDoExt_setCurrentHeap(JKRHeap* heap) {
    JUT_ASSERT(2594, heap != NULL);
    JKRHeap* result = JKRSetCurrentHeap(heap);
    if (mDoExt::CurrentHeapAdjustVerbose) {
        // "Changing the current heap to the specified heap %08x->%08x"
        OS_REPORT("カレントヒープを指定ヒープに変更 %08x->%08x\n", result, heap);
    }
    return result;
}

JKRHeap* mDoExt_getCurrentHeap() {
    return JKRGetCurrentHeap();
}

void mDoExt_restoreCurrentHeap() {
    JUT_ASSERT(2711, OSGetCurrentThread() == &mainThread);
    JUT_ASSERT(2712, mDoExt_SaveCurrentHeap != FALSE);
    JKRHeap* result = JKRSetCurrentHeap(mDoExt_SaveCurrentHeap);
    if (mDoExt::CurrentHeapAdjustVerbose) {
        // "The current heap has been restored %08x->%08x"
        OS_REPORT("カレントヒープを復帰しました %08x->%08x\n", result, mDoExt_SaveCurrentHeap);
    }
    mDoExt_SaveCurrentHeap = NULL;
}

int mDoExt_resIDToIndex(JKRArchive* p_archive, u16 id) {
    JKRArchive::SDIFileEntry* res = p_archive->findIdResource(id);

    if (res == NULL) {
        return -1;
    }

    return res - p_archive->mFiles;
}

void mDoExt_MtxCalcAnmBlendTbl::calc() {
    u16 jntNo = getJoint()->getJntNo();
    j3dSys.setCurrentMtxCalc(this);
    if (mNum == 1) {
        J3DTransformInfo info1;
        mAnmRatio->getAnmTransform()->getTransform(jntNo, &info1);
        J3DMtxCalcCalcTransformMaya::calcTransform(info1);
        return;
    }
    J3DTransformInfo info2;
    Quaternion quat1;
    Quaternion quat2;
    Quaternion quat3;
    mAnmRatio->getAnmTransform()->getTransform(jntNo, &info2);
    JMAEulerToQuat(info2.mRotation.x, info2.mRotation.y, info2.mRotation.z, &quat1);
    f32 f30 = mAnmRatio->getRatio();
    quat3 = quat1;
    for (int i = 1; i < mNum; i++) {
        if (mAnmRatio[i].getAnmTransform() != NULL) {
            J3DTransformInfo info3;
            mAnmRatio[i].getAnmTransform()->getTransform(jntNo, &info3);
            f32 ratio = mAnmRatio[i].getRatio();
            f30 = 1.0f - ratio;
            JMAEulerToQuat(info3.mRotation.x, info3.mRotation.y, info3.mRotation.z, &quat2);
            JMAQuatLerp(&quat1, &quat2, ratio, &quat3);
            quat1 = quat3;
            info2.mTranslate.x = info2.mTranslate.x * f30 + info3.mTranslate.x * ratio;
            info2.mTranslate.y = info2.mTranslate.y * f30 + info3.mTranslate.y * ratio;
            info2.mTranslate.z = info2.mTranslate.z * f30 + info3.mTranslate.z * ratio;
            info2.mScale.x = info2.mScale.x * f30 + info3.mScale.x * ratio;
            info2.mScale.y = info2.mScale.y * f30 + info3.mScale.y * ratio;
            info2.mScale.z = info2.mScale.z * f30 + info3.mScale.z * ratio;
        }
    }
    Mtx mtx;
    mDoMtx_quat(mtx, &quat3);
    mDoExt_setJ3DData(mtx, &info2, jntNo);
}


void mDoExt_MtxCalcAnmBlendTblOld::calc() {
    u16 jntNo = getJoint()->getJntNo();
    j3dSys.setCurrentMtxCalc(this);
    J3DModelData* modelData = j3dSys.getModel()->getModelData();
    J3DTransformInfo info1;
    Quaternion quat1;
    Quaternion quat2;
    Quaternion quat3;
    mAnmRatio->getAnmTransform()->getTransform(jntNo, &info1);
    JMAEulerToQuat(info1.mRotation.x, info1.mRotation.y, info1.mRotation.z, &quat1);
    f32 var_f31;
    f32 var_f30 = mAnmRatio[0].getRatio();
    quat3 = quat1;
    for (int i = 1; i < mNum; i++) {
        if (mAnmRatio[i].getAnmTransform()) {
            J3DTransformInfo info2;
            mAnmRatio[i].getAnmTransform()->getTransform(jntNo, &info2);
            var_f31 = mAnmRatio[i].getRatio();
            var_f30 = 1.0f - var_f31;
            JMAEulerToQuat(info2.mRotation.x, info2.mRotation.y, info2.mRotation.z, &quat2);
            JMAQuatLerp(&quat1, &quat2, var_f31, &quat3);
            quat1 = quat3;
            info1.mTranslate.x = info1.mTranslate.x * var_f30 + info2.mTranslate.x * var_f31;
            info1.mTranslate.y = info1.mTranslate.y * var_f30 + info2.mTranslate.y * var_f31;
            info1.mTranslate.z = info1.mTranslate.z * var_f30 + info2.mTranslate.z * var_f31;
            info1.mScale.x = info1.mScale.x * var_f30 + info2.mScale.x * var_f31;
            info1.mScale.y = info1.mScale.y * var_f30 + info2.mScale.y * var_f31;
            info1.mScale.z = info1.mScale.z * var_f30 + info2.mScale.z * var_f31;
        }
    }

    J3DTransformInfo* oldTransInfo = mOldFrame->getOldFrameTransInfo(jntNo);
    Quaternion* oldQuat = mOldFrame->getOldFrameQuaternion(jntNo);
    if (mOldFrame->getOldFrameFlg()) {
        if (mOldFrame->getOldFrameRate() > 0.0f && mOldFrame->getOldFrameStartJoint() <= jntNo && mOldFrame->getOldFrameEndJoint() > jntNo) {
            var_f30 = mOldFrame->getOldFrameRate();
            var_f31 = 1.0f - var_f30;
            JMAQuatLerp(oldQuat, &quat1, var_f31, &quat3);
            info1.mTranslate.x = info1.mTranslate.x * var_f31 + oldTransInfo->mTranslate.x * var_f30;
            info1.mTranslate.y = info1.mTranslate.y * var_f31 + oldTransInfo->mTranslate.y * var_f30;
            info1.mTranslate.z = info1.mTranslate.z * var_f31 + oldTransInfo->mTranslate.z * var_f30;
            info1.mScale.x = info1.mScale.x * var_f31 + oldTransInfo->mScale.x * var_f30;
            info1.mScale.y = info1.mScale.y * var_f31 + oldTransInfo->mScale.y * var_f30;
            info1.mScale.z = info1.mScale.z * var_f31 + oldTransInfo->mScale.z * var_f30;
        }
    } else if (jntNo == modelData->getJointNum() - 1) {
        mOldFrame->onOldFrameFlg();
    }
    Mtx mtx;
    mDoMtx_quat(mtx, &quat3);
    mDoExt_setJ3DData(mtx, &info1, jntNo);
    *oldQuat = quat3;
    *oldTransInfo = info1;
    if (jntNo == modelData->getJointNum() - 1) {
        mOldFrame->decOldFrameMorfCounter();
    }
}

void mDoExt_MtxCalcOldFrame::initOldFrameMorf(f32 param_0, u16 frameStartJoint, u16 frameEndJoint) {
    if (param_0 > 0.0f) {
        mOldFrameMorfCounter = param_0;
        field_0x8 = 1.0f / param_0;
        mOldFrameRate = 1.0f;
        field_0x10 = 1.0f;
        field_0x14 = 1.0f;
        decOldFrameMorfCounter();
    } else {
        mOldFrameMorfCounter = 0.0f;
        field_0x8 = 0.0f;
        mOldFrameRate = 0.0f;
        field_0x10 = 0.0f;
        field_0x14 = 0.0f;
    }

    mOldFrameStartJoint = frameStartJoint;
    mOldFrameEndJoint = frameEndJoint;
}

void mDoExt_MtxCalcOldFrame::decOldFrameMorfCounter() {
    if (mOldFrameMorfCounter > 0.0f) {

        mOldFrameMorfCounter -= 1.0f;
        if (mOldFrameMorfCounter <= 0.0f) {
            mOldFrameMorfCounter = 0.0f;
            field_0x8 = 0.0f;
            mOldFrameRate = 0.0f;
        }

        field_0x14 = field_0x10;
        field_0x10 = mOldFrameMorfCounter * field_0x8;

        if (field_0x14 > 0.0f) {
            mOldFrameRate = 1.0f - (field_0x14 - field_0x10) / field_0x14;
        } else {
            mOldFrameRate = 0.0f;
        }
    }
}

mDoExt_morf_c::mDoExt_morf_c() {
    mpModel = NULL;
    mpAnm = NULL;
    mpTransformInfo = NULL;
    mpQuat = NULL;
}

mDoExt_morf_c::~mDoExt_morf_c() {
    /* empty function */
}

void mDoExt_morf_c::setMorf(f32 i_morf) {
    if (mPrevMorf < 0.0f || i_morf == 0.0f) {
        mCurMorf = 1.0f;
    } else {
        mCurMorf = 0.0f;
        mMorfStep = 0.0f;
        field_0x34 = 1.0f / i_morf;
    }

    mPrevMorf = mCurMorf;
}

void mDoExt_morf_c::frameUpdate() {
    if (mCurMorf < 1.0f) {
        mPrevMorf = mCurMorf;

        if (field_0x34 < 0.0f) {
            cLib_chaseF(&mMorfStep, 1.0f, -field_0x34);
            mCurMorf += mMorfStep * ((1.0f - mCurMorf) * mMorfStep);
        } else {
            cLib_chaseF(&mCurMorf, 1.0f, field_0x34);
        }
    }

    mFrameCtrl.update();
}

mDoExt_McaMorf::mDoExt_McaMorf(J3DModelData* modelData, mDoExt_McaMorfCallBack1_c* callback1,
                                   mDoExt_McaMorfCallBack2_c* callback2, J3DAnmTransform* anmTransform,
                                   int param_4, f32 param_5, int param_6, int param_7, int param_8,
                                   void* param_9, u32 param_10, u32 param_11) {
    field_0x50 = false;
    field_0x51 = false;
    field_0x52 = false;
    create(modelData, callback1, callback2, anmTransform, param_4, param_5, param_6, param_7, param_8, param_9, param_10, param_11);
}

mDoExt_McaMorf::~mDoExt_McaMorf() {
    if (field_0x50 && mpSound != NULL) {
        mpSound->stopAnime();
    }
}

int mDoExt_McaMorf::create(J3DModelData* modelData, mDoExt_McaMorfCallBack1_c* callback1,
                           mDoExt_McaMorfCallBack2_c* callback2, J3DAnmTransform* anmTransform,
                           int param_4, f32 param_5, int param_6, int param_7, int param_8,
                           void* param_9, u32 param_10, u32 param_11) {
    mpModel = NULL;
    mpSound = NULL;
    mpTransformInfo = NULL;
    mpQuat = NULL;
    if (!modelData) {
        return 0;
    }
    if (modelData->getMaterialNodePointer(0)->getSharedDisplayListObj() && param_10 == 0) {
        if (modelData->isLocked()) {
            param_10 = 0x20000;
        } else {
            param_10 = 0x80000;
        }
    }
    mpModel = mDoExt_J3DModel__create(modelData, param_10, param_11);
    if (!mpModel) {
        return 0;
    }
    if (param_10 != 0x80000) {
        mDoExt_changeMaterial(mpModel);
    }
    if (param_9 == NULL && anmTransform) {
        param_9 = ((mDoExt_transAnmBas*)anmTransform)->getBas();
        if (param_9) {
            param_8 = 1;
        }
    }
    if (param_8) {
        mpSound = new mDoExt_zelAnime();
        if (!mpSound) {
            goto cleanup;
        }
    }
    setAnm(anmTransform, param_4, 0.0f, param_5, param_6, param_7, param_9);
    mPrevMorf = -1.0f;
    mpTransformInfo = new J3DTransformInfo[modelData->getJointNum()];
    if (!mpTransformInfo) {
        goto cleanup;
    }
    mpQuat = new Quaternion[modelData->getJointNum()];
    if (mpQuat) {
        J3DTransformInfo* info = mpTransformInfo;
        Quaternion* quat = mpQuat;
        J3DModelData* r23 = mpModel->getModelData();
        int jointNum = r23->getJointNum();
        for (int i = 0; i < jointNum; i++) {
            J3DJoint* joint = r23->getJointNodePointer(i);
            J3DTransformInfo& transInfo = joint->getTransformInfo();
            *info = transInfo;
            JMAEulerToQuat(info->mRotation.x, info->mRotation.y, info->mRotation.z, quat);
            info++;
            quat++;
        }
        mpCallback1 = callback1;
        mpCallback2 = callback2;
        return 1;
    }
    cleanup:
    if (mpSound) {
        mpSound->stopAnime();
        mpSound = NULL;
    }
    if (mpTransformInfo) {
        mpTransformInfo = NULL;
    }
    if (mpQuat) {
        mpQuat = NULL;
    }
    if (mpModel) {
        mpModel = NULL;
    }
    return 0;
}

void mDoExt_McaMorf::calc() {
    if (mpModel == NULL) {
        return;
    }

    u16 jntNo = getJoint()->getJntNo();
    j3dSys.setCurrentMtxCalc(this);

    J3DTransformInfo sp48;
    J3DTransformInfo sp28;
    J3DTransformInfo* var_r29;
    if (mpTransformInfo == NULL) {
        var_r29 = &sp48;
    } else {
        var_r29 = &mpTransformInfo[jntNo];
    }

    Quaternion sp18;
    Quaternion* var_r27;
    if (mpQuat == NULL) {
        var_r27 = &sp18;
    } else {
        var_r27 = &mpQuat[jntNo];
    }

    if (mpAnm == NULL) {
        *var_r29 = mpModel->getModelData()->getJointNodePointer(jntNo)->getTransformInfo();
        if (mpCallback1 != NULL) {
            mpCallback1->execute(jntNo, var_r29);
        }

        JMAEulerToQuat(var_r29->mRotation.x, var_r29->mRotation.y, var_r29->mRotation.z, var_r27);
        J3DMtxCalcCalcTransformMaya::calcTransform(*var_r29);
    } else if (mCurMorf >= 1.0f || mpTransformInfo == NULL || mpQuat == NULL) {
        getTransform(jntNo, var_r29);
        if (mpCallback1 != NULL) {
            mpCallback1->execute(jntNo, var_r29);
        }
        JMAEulerToQuat(var_r29->mRotation.x, var_r29->mRotation.y, var_r29->mRotation.z, var_r27);
        J3DMtxCalcCalcTransformMaya::calcTransform(*var_r29);
    } else {
        f32 var_f31;
        if (field_0x52) {
            var_f31 = 1.0f;
        } else {
            var_f31 = (mCurMorf - mPrevMorf) / (1.0f - mPrevMorf);
        }
        f32 var_f30 = 1.0f - var_f31;

        Quaternion sp8;
        getTransform(jntNo, &sp28);

        if (mpCallback1 != NULL) {
            mpCallback1->execute(jntNo, &sp28);
        }

        JMAEulerToQuat(sp28.mRotation.x, sp28.mRotation.y, sp28.mRotation.z, &sp8);
        JMAQuatLerp(var_r27, &sp8, var_f31, var_r27);

        Mtx sp68;
        mDoMtx_quat(sp68, var_r27);

        var_r29->mTranslate.x = var_r29->mTranslate.x * var_f30 + sp28.mTranslate.x * var_f31;
        var_r29->mTranslate.y = var_r29->mTranslate.y * var_f30 + sp28.mTranslate.y * var_f31;
        var_r29->mTranslate.z = var_r29->mTranslate.z * var_f30 + sp28.mTranslate.z * var_f31;
        var_r29->mScale.x = var_r29->mScale.x * var_f30 + sp28.mScale.x * var_f31;
        var_r29->mScale.y = var_r29->mScale.y * var_f30 + sp28.mScale.y * var_f31;
        var_r29->mScale.z = var_r29->mScale.z * var_f30 + sp28.mScale.z * var_f31;
        mDoExt_setJ3DData(sp68, var_r29, jntNo);
    }

    if (mpCallback2 != NULL) {
        mpCallback2->execute(jntNo);
    }
}

void mDoExt_McaMorf::setAnm(J3DAnmTransform* param_0, int param_1, f32 param_2, f32 param_3,
                                f32 param_4, f32 param_5, void* param_6) {
    mpAnm = param_0;
    setStartFrame(param_4);
    if (param_5 < 0.0f) {
        if (!mpAnm) {
            mFrameCtrl.init(0);
        } else {
            mFrameCtrl.init(mpAnm->getFrameMax());
        }
    } else {
        mFrameCtrl.init((s16)param_5);
    }
    if (param_0 && param_1 < 0) {
        param_1 = param_0->getAttribute();
    }
    setPlayMode(param_1);
    setPlaySpeed(param_3);
    if (param_3 >= 0.0f) {
        setFrame(param_4);
    } else {
        setFrame(getEndFrame());
    }
    setLoopFrame(getFrame());
    setMorf(param_2);
    if (mpSound) {
        if (param_6 == NULL && param_0) {
            param_6 = ((mDoExt_transAnmBas*)param_0)->getBas();
        }
        mpSound->field_0x48 = param_6;
        if (mpSound->field_0x48) {
            mpSound->initAnime(param_6, getPlaySpeed() >= 0.0f, getLoopFrame(), 0.0f);
        } else {
            mpSound->stopAnime();
        }
    }
}

u32 mDoExt_McaMorf::play(Vec* param_0, u32 param_1, s8 param_2) {
    frameUpdate();
    if (mpSound != NULL && mpSound->field_0x48 != NULL && param_0 != NULL) {
        mpSound->updateAnime(getFrame(), getPlaySpeed());
        mpSound->framework(param_1, param_2);
        field_0x50 = true;
    }
    return isStop();
}

void mDoExt_McaMorf::entryDL() {
    if (mpModel != NULL) {
        mDoExt_modelEntryDL(mpModel);
    }
}

void mDoExt_McaMorf::modelCalc() {
    if (mpModel != NULL) {
        if (mpAnm != NULL) {
            mpAnm->setFrame(mFrameCtrl.getFrame());
        }

        mpModel->getModelData()->getJointNodePointer(0)->setMtxCalc(this);
        mpModel->calc();
    }
}

void mDoExt_McaMorf::getTransform(u16 param_0, J3DTransformInfo* param_1) {
    mpAnm->getTransform(param_0, param_1);
    if (field_0x51) {
        if (param_0 == 0) {
            param_1->mTranslate.x *= mTranslateScale.x;
            param_1->mTranslate.y *= mTranslateScale.y;
            param_1->mTranslate.z *= mTranslateScale.z;
        } else {
            J3DTransformInfo& info = mpModel->getModelData()->getJointNodePointer(param_0)->getTransformInfo();
            param_1->mTranslate.x = info.mTranslate.x;
            param_1->mTranslate.y = info.mTranslate.y;
            param_1->mTranslate.z = info.mTranslate.z;
        }
    }
}

mDoExt_McaMorfSO::mDoExt_McaMorfSO(J3DModelData* param_0, mDoExt_McaMorfCallBack1_c* param_1,
                                   mDoExt_McaMorfCallBack2_c* param_2, J3DAnmTransform* param_3,
                                   int param_4, f32 param_5, int param_6, int param_7,
                                   Z2Creature* param_8, u32 i_modelFlag, u32 i_differedDlistFlag) {
    mTranslate = false;
    mMorfNone = false;
    create(param_0, param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8,
           i_modelFlag, i_differedDlistFlag);
}

mDoExt_McaMorfSO::~mDoExt_McaMorfSO() {
    stopZelAnime();
}

int mDoExt_McaMorfSO::create(J3DModelData* i_modelData, mDoExt_McaMorfCallBack1_c* param_1,
                             mDoExt_McaMorfCallBack2_c* param_2, J3DAnmTransform* param_3,
                             int param_4, f32 param_5, int param_6, int param_7,
                             Z2Creature* i_sound, u32 i_modelFlag, u32 i_differedDlistFlag) {
    mpModel = NULL;
    mpTransformInfo = NULL;
    mpQuat = NULL;
    mpSound = NULL;

    if (i_modelData == NULL) {
        return 0;
    }

    if (i_modelData->getMaterialNodePointer(0)->getSharedDisplayListObj() != NULL &&
        i_modelFlag == 0)
    {
        if (i_modelData->isLocked()) {
            i_modelFlag = 0x20000;
        } else {
            i_modelFlag = 0x80000;
        }
    }

    mpModel = mDoExt_J3DModel__create(i_modelData, i_modelFlag, i_differedDlistFlag);
    if (mpModel == NULL) {
        return 0;
    }

    if (i_modelFlag != 0x80000) {
        mDoExt_changeMaterial(mpModel);
    }

    mpSound = i_sound;

    if (param_3 != NULL) {
        mpBas = ((mDoExt_transAnmBas*)param_3)->getBas();
    }

    setAnm(param_3, param_4, 0.0f, param_5, param_6, param_7);
    mPrevMorf = -1.0f;

    mpTransformInfo = new J3DTransformInfo[i_modelData->getJointNum()];
    if (mpTransformInfo != NULL) {
        mpQuat = new Quaternion[i_modelData->getJointNum()];

        if (mpQuat != NULL) {
            J3DTransformInfo* transInfo_p = mpTransformInfo;
            Quaternion* quat = mpQuat;
            J3DModelData* modelData = mpModel->getModelData();
            int jointNum = modelData->getJointNum();

            for (int i = 0; i < jointNum; i++) {
                J3DJoint* joint = modelData->getJointNodePointer(i);
                J3DTransformInfo& transInfo = joint->getTransformInfo();;
                *transInfo_p = transInfo;
                JMAEulerToQuat(transInfo_p->mRotation.x, transInfo_p->mRotation.y,
                               transInfo_p->mRotation.z, quat);

                transInfo_p++;
                quat++;
            }

            mpCallback1 = param_1;
            mpCallback2 = param_2;
            return 1;
        }
    }

    if (mpTransformInfo != NULL) {
        mpTransformInfo = NULL;
    }

    if (mpQuat != NULL) {
        mpQuat = NULL;
    }

    if (mpModel != NULL) {
        mpModel = NULL;
    }

    return 0;
}

void mDoExt_McaMorfSO::calc() {
    if (mpModel != NULL) {
        u16 jnt_no = getJoint()->getJntNo();
        j3dSys.setCurrentMtxCalc(this);

        J3DTransformInfo trans;
        J3DTransformInfo* trans_p;
        if (mpTransformInfo == NULL) {
            trans_p = &trans;
        } else {
            trans_p = &mpTransformInfo[jnt_no];
        }

        Quaternion quat;
        Quaternion* quat_p;
        if (mpQuat == NULL) {
            quat_p = &quat;
        } else {
            quat_p = &mpQuat[jnt_no];
        }

        if (mpAnm == NULL) {
            *trans_p = mpModel->getModelData()->getJointNodePointer(jnt_no)->getTransformInfo();

            if (mpCallback1 != NULL) {
                mpCallback1->execute(jnt_no, trans_p);
            }

            JMAEulerToQuat(trans_p->mRotation.x, trans_p->mRotation.y, trans_p->mRotation.z,
                           quat_p);
            J3DMtxCalcCalcTransformMaya::calcTransform(*trans_p);
        } else if (mCurMorf >= 1.0f || mpTransformInfo == NULL || mpQuat == NULL) {
            getTransform(jnt_no, trans_p);

            if (mpCallback1 != NULL) {
                mpCallback1->execute(jnt_no, trans_p);
            }

            JMAEulerToQuat(trans_p->mRotation.x, trans_p->mRotation.y, trans_p->mRotation.z,
                           quat_p);
            J3DMtxCalcCalcTransformMaya::calcTransform(*trans_p);
        } else {
            f32 lerp_factor;
            if (mMorfNone) {
                lerp_factor = 1.0f;
            } else {
                lerp_factor = (mCurMorf - mPrevMorf) / (1.0f - mPrevMorf);
            }
            f32 inv_lerp_factor = 1.0f - lerp_factor;

            J3DTransformInfo trans2;
            getTransform(jnt_no, &trans2);

            if (mpCallback1 != NULL) {
                mpCallback1->execute(jnt_no, &trans2);
            }

            Quaternion quat2;
            JMAEulerToQuat(trans2.mRotation.x, trans2.mRotation.y, trans2.mRotation.z, &quat2);
            JMAQuatLerp(quat_p, &quat2, lerp_factor, quat_p);

            Mtx mtx;
            mDoMtx_quat(mtx, quat_p);

            trans_p->mTranslate.x = trans_p->mTranslate.x * inv_lerp_factor
                                    + trans2.mTranslate.x * lerp_factor;
            trans_p->mTranslate.y = trans_p->mTranslate.y * inv_lerp_factor
                                    + trans2.mTranslate.y * lerp_factor;
            trans_p->mTranslate.z = trans_p->mTranslate.z * inv_lerp_factor
                                    + trans2.mTranslate.z * lerp_factor;
            trans_p->mScale.x = trans_p->mScale.x * inv_lerp_factor
                                + trans2.mScale.x * lerp_factor;
            trans_p->mScale.y = trans_p->mScale.y * inv_lerp_factor
                                + trans2.mScale.y * lerp_factor;
            trans_p->mScale.z = trans_p->mScale.z * inv_lerp_factor
                                + trans2.mScale.z * lerp_factor;

            mDoExt_setJ3DData(mtx, trans_p, jnt_no);
        }

        if (mpCallback2 != NULL) {
            mpCallback2->execute(jnt_no);
        }
    }
}

void mDoExt_McaMorfSO::setAnm(J3DAnmTransform* i_anm, int i_attr, f32 i_morf, f32 i_rate,
                              f32 i_start, f32 i_end) {
    mpAnm = i_anm;
    setStartFrame(i_start);

    if (i_end < 0.0f) {
        if (mpAnm == NULL) {
            mFrameCtrl.init(0);
        } else {
            mFrameCtrl.init(mpAnm->getFrameMax());
        }
    } else {
        mFrameCtrl.init((s16)i_end);
    }

    if (i_anm != NULL && i_attr < 0) {
        i_attr = i_anm->getAttribute();
    }

    setPlayMode(i_attr);
    setPlaySpeed(i_rate);

    if (i_rate >= 0.0f) {
        setFrame(i_start);
    } else {
        setFrame(getEndFrame());
    }

    setLoopFrame(getFrame());
    setMorf(i_morf);

    if (mpSound != NULL) {
        if (i_anm != NULL) {
            mpBas = static_cast<mDoExt_transAnmBas*>(i_anm)->getBas();
        } else {
            mpBas = NULL;
        }

        if (mpBas != NULL) {
            mpSound->initAnime(mpBas, getPlaySpeed() >= 0.0f, getLoopFrame(), getFrame());
        }
    }
}

int mDoExt_McaMorfSO::play(u32 param_0, s8 param_1) {
    frameUpdate();

    if (mpSound != NULL) {
        mpSound->framework(param_0, param_1);

        if (mpBas != NULL) {
            mpSound->updateAnime(getFrame(), getPlaySpeed());
        }
    }

    return isStop();
}

void mDoExt_McaMorfSO::updateDL() {
    if (mpModel != NULL) {
        if (mpAnm != NULL) {
            mpAnm->setFrame(mFrameCtrl.getFrame());
        }

        mpModel->getModelData()->getJointNodePointer(0)->setMtxCalc(this);
        mDoExt_modelUpdateDL(mpModel);
        mPrevMorf = mCurMorf;
    }
}

void mDoExt_McaMorfSO::entryDL() {
    if (mpModel != NULL) {
        mDoExt_modelEntryDL(mpModel);
    }
}

void mDoExt_McaMorfSO::modelCalc() {
    if (mpModel != NULL) {
        if (mpAnm != NULL) {
            mpAnm->setFrame(mFrameCtrl.getFrame());
        }

        mpModel->getModelData()->getJointNodePointer(0)->setMtxCalc(this);
        mpModel->calc();
    }
}

void mDoExt_McaMorfSO::getTransform(u16 param_0, J3DTransformInfo* param_1) {
    mpAnm->getTransform(param_0, param_1);
    if (mTranslate) {
        if (param_0 == 0) {
            param_1->mTranslate.x *= mTranslateScale.x;
            param_1->mTranslate.y *= mTranslateScale.y;
            param_1->mTranslate.z *= mTranslateScale.z;
        } else {
            J3DTransformInfo& info = mpModel->getModelData()->getJointNodePointer(param_0)->getTransformInfo();
            param_1->mTranslate.x = info.mTranslate.x;
            param_1->mTranslate.y = info.mTranslate.y;
            param_1->mTranslate.z = info.mTranslate.z;
        }
    }
}

void mDoExt_McaMorfSO::stopZelAnime() {
    if (mpSound != NULL) {
        mpSound->deleteObject();
    }
}

mDoExt_McaMorf2::mDoExt_McaMorf2(J3DModelData* param_0, mDoExt_McaMorfCallBack1_c* param_1,
                                 mDoExt_McaMorfCallBack2_c* param_2, J3DAnmTransform* param_3,
                                 J3DAnmTransform* param_4, int param_5, f32 param_6, int param_7,
                                 int param_8, Z2Creature* param_9, u32 param_10, u32 param_11) {
    create(param_0, param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8, param_9,
           param_10, param_11);
}

mDoExt_McaMorf2::~mDoExt_McaMorf2() {
    stopZelAnime();
}

 int mDoExt_McaMorf2::create(J3DModelData* param_0, mDoExt_McaMorfCallBack1_c* param_1,
                                 mDoExt_McaMorfCallBack2_c* param_2, J3DAnmTransform* param_3,
                                 J3DAnmTransform* param_4, int param_5, f32 param_6, int param_7,
                                 int param_8, Z2Creature* param_9, u32 param_10, u32 param_11) {
    mpModel = NULL;
    mpTransformInfo = NULL;
    mpQuat = NULL;
    mpSound = NULL;

    if (param_0 == NULL) {
        return 0;
    }

    if (param_0->getMaterialNodePointer(0)->getSharedDisplayListObj() != NULL && param_10 == 0) {
        if (param_0->isLocked()) {
            param_10 = 0x20000;
        } else {
            param_10 = 0x80000;
        }
    }

    mpModel = mDoExt_J3DModel__create(param_0, param_10, param_11);
    if (mpModel == NULL) {
        return 0;
    }

    if (param_10 != 0x80000) {
        mDoExt_changeMaterial(mpModel);
    }

    mpSound = param_9;

    setAnm(param_3, param_4, 0.0f, param_5, 0.0f, param_6, param_7, param_8);
    mPrevMorf = -1.0f;

    mpTransformInfo = new J3DTransformInfo[param_0->getJointNum()];
    if (mpTransformInfo == NULL) {
        ERROR_EXIT();
        return 0;
    }

    mpQuat = new Quaternion[param_0->getJointNum()];
    if (mpQuat == NULL) {
        ERROR_EXIT();
        return 0;
    }

    J3DTransformInfo* var_r29 = mpTransformInfo;
    Quaternion* var_r26 = mpQuat;
    J3DModelData* model_data = mpModel->getModelData();
    int sp3C = model_data->getJointNum();

    for (int i = 0; i < sp3C; i++) {
        J3DJoint* sp30 = model_data->getJointNodePointer(i);
        J3DTransformInfo* sp2C = &sp30->getTransformInfo();

        *var_r29 = *sp2C;
        JMAEulerToQuat(var_r29->mRotation.x, var_r29->mRotation.y, var_r29->mRotation.z, var_r26);

        var_r29++;
        var_r26++;
    }

    mpCallback1 = param_1;
    mpCallback2 = param_2;
    return 1;
}

void mDoExt_McaMorf2::ERROR_EXIT() {
    if (mpTransformInfo != NULL) {
        mpTransformInfo = NULL;
    }

    if (mpQuat != NULL) {
        mpQuat = NULL;
    }

    if (mpModel != NULL) {
        mpModel = NULL;
    }
}

void mDoExt_McaMorf2::calc() {
    if (mpModel != NULL) {
        u16 jnt_no = J3DMtxCalc::getJoint()->getJntNo();
        j3dSys.setCurrentMtxCalc(this);

        J3DTransformInfo spF0[2];
        Mtx spC0;

        J3DTransformInfo spA0;
        J3DTransformInfo sp80;
        J3DTransformInfo* var_r30;
        if (mpTransformInfo == NULL) {
            var_r30 = &spA0;
        } else {
            var_r30 = &mpTransformInfo[jnt_no];
        }

        Quaternion sp60[2];
        Quaternion sp40[2];

        Quaternion sp30;
        Quaternion sp20;
        Quaternion* var_r27;
        f32 var_f31;
        f32 var_f30;
        f32 var_f29;
        f32 sp18[2];
        f32 sp10[2];
        if (mpQuat == NULL) {
            var_r27 = &sp30;
        } else {
            var_r27 = &mpQuat[jnt_no];
        }

        if (mpAnm == NULL) {
            *var_r30 = mpModel->getModelData()->getJointNodePointer(jnt_no)->getTransformInfo();

            if (mpCallback1 != NULL) {
                mpCallback1->execute(jnt_no, var_r30);
            }

            JMAEulerToQuat(var_r30->mRotation.x, var_r30->mRotation.y, var_r30->mRotation.z,
                           var_r27);
            J3DMtxCalcCalcTransformMaya::calcTransform(*var_r30);
        } else if (mCurMorf >= 1.0f || mpTransformInfo == NULL || mpQuat == NULL) {
            mpAnm->getTransform(jnt_no, &spF0[0]);
            if (field_0x40 == NULL) {
                if (mpCallback1 != NULL) {
                    mpCallback1->execute(jnt_no, &spF0[0]);
                }

                JMAEulerToQuat(spF0[0].mRotation.x, spF0[0].mRotation.y, spF0[0].mRotation.z,
                               var_r27);
                J3DMtxCalcCalcTransformMaya::calcTransform(spF0[0]);
                *var_r30 = spF0[0];
            } else {
                field_0x40->getTransform(jnt_no, &spF0[1]);

                sp18[0] = 1.0f - mAnmRate;
                sp18[1] = mAnmRate;

                var_r30->mScale.x = spF0[0].mScale.x * sp18[0]
                                    + spF0[1].mScale.x * sp18[1];
                var_r30->mScale.y = spF0[0].mScale.y * sp18[0]
                                    + spF0[1].mScale.y * sp18[1];
                var_r30->mScale.z = spF0[0].mScale.z * sp18[0]
                                    + spF0[1].mScale.z * sp18[1];
                var_r30->mTranslate.x = spF0[0].mTranslate.x * sp18[0]
                                    + spF0[1].mTranslate.x * sp18[1];
                var_r30->mTranslate.y = spF0[0].mTranslate.y * sp18[0]
                                        + spF0[1].mTranslate.y * sp18[1];
                var_r30->mTranslate.z = spF0[0].mTranslate.z * sp18[0]
                                        + spF0[1].mTranslate.z * sp18[1];

                for (int i = 0; i < 2; i++) {
                    JMAEulerToQuat(spF0[i].mRotation.x, spF0[i].mRotation.y, spF0[i].mRotation.z, &sp60[i]);
                }

                var_f29 = sp18[1] / (sp18[0] + sp18[1]);
                JMAQuatLerp(&sp60[0], &sp60[1], var_f29, var_r27);

                mDoMtx_quat(spC0, var_r27);
                mDoExt_setJ3DData(spC0, var_r30, jnt_no);
            }
        } else if (field_0x40 == NULL) {
            var_f31 = (mCurMorf - mPrevMorf) / (1.0f - mPrevMorf);
            var_f30 = 1.0f - var_f31;

            mpAnm->getTransform(jnt_no, &sp80);
            if (mpCallback1 != NULL) {
                mpCallback1->execute(jnt_no, &sp80);
            }

            JMAEulerToQuat(sp80.mRotation.x, sp80.mRotation.y, sp80.mRotation.z, &sp20);
            JMAQuatLerp(var_r27, &sp20, var_f31, var_r27);
            mDoMtx_quat(spC0, var_r27);

            var_r30->mTranslate.x = var_r30->mTranslate.x * var_f30
                                    + sp80.mTranslate.x * var_f31;
            var_r30->mTranslate.y = var_r30->mTranslate.y * var_f30
                                    + sp80.mTranslate.y * var_f31;
            var_r30->mTranslate.z = var_r30->mTranslate.z * var_f30
                                    + sp80.mTranslate.z * var_f31;
            var_r30->mScale.x = var_r30->mScale.x * var_f30
                                + sp80.mScale.x * var_f31;
            var_r30->mScale.y = var_r30->mScale.y * var_f30
                                + sp80.mScale.y * var_f31;
            var_r30->mScale.z = var_r30->mScale.z * var_f30
                                + sp80.mScale.z * var_f31;

            mDoExt_setJ3DData(spC0, var_r30, jnt_no);
        } else {
            mpAnm->getTransform(jnt_no, &spF0[0]);
            field_0x40->getTransform(jnt_no, &spF0[1]);

            sp10[0] = 1.0f - mAnmRate;
            sp10[1] = mAnmRate;

            sp80.mScale.x = spF0[0].mScale.x * sp10[0]
                                + spF0[1].mScale.x * sp10[1];
            sp80.mScale.y = spF0[0].mScale.y * sp10[0]
                                + spF0[1].mScale.y * sp10[1];
            sp80.mScale.z = spF0[0].mScale.z * sp10[0]
                                + spF0[1].mScale.z * sp10[1];
            sp80.mTranslate.x = spF0[0].mTranslate.x * sp10[0]
                                + spF0[1].mTranslate.x * sp10[1];
            sp80.mTranslate.y = spF0[0].mTranslate.y * sp10[0]
                                    + spF0[1].mTranslate.y * sp10[1];
            sp80.mTranslate.z = spF0[0].mTranslate.z * sp10[0]
                                    + spF0[1].mTranslate.z * sp10[1];

            for (int i = 0; i < 2; i++) {
                JMAEulerToQuat(spF0[i].mRotation.x, spF0[i].mRotation.y, spF0[i].mRotation.z, &sp40[i]);
            }

            var_f31 = sp10[1] / (sp10[0] + sp10[1]);
            JMAQuatLerp(&sp40[0], &sp40[1], var_f31, &sp20);

            var_f31 = (mCurMorf - mPrevMorf) / (1.0f - mPrevMorf);
            var_f30 = 1.0f - var_f31;
            JMAQuatLerp(var_r27, &sp20, var_f31, var_r27);

            var_r30->mTranslate.x = var_r30->mTranslate.x * var_f30
                                    + sp80.mTranslate.x * var_f31;
            var_r30->mTranslate.y = var_r30->mTranslate.y * var_f30
                                    + sp80.mTranslate.y * var_f31;
            var_r30->mTranslate.z = var_r30->mTranslate.z * var_f30
                                    + sp80.mTranslate.z * var_f31;
            var_r30->mScale.x = var_r30->mScale.x * var_f30
                                + sp80.mScale.x * var_f31;
            var_r30->mScale.y = var_r30->mScale.y * var_f30
                                + sp80.mScale.y * var_f31;
            var_r30->mScale.z = var_r30->mScale.z * var_f30
                                + sp80.mScale.z * var_f31;

            mDoMtx_quat(spC0, var_r27);
            mDoExt_setJ3DData(spC0, var_r30, jnt_no);
        }

        if (mpCallback2 != NULL) {
            mpCallback2->execute(jnt_no);
        }
    }
}

void mDoExt_McaMorf2::setAnm(J3DAnmTransform* param_0, J3DAnmTransform* param_1, f32 param_2,
                             int i_attr, f32 i_morf, f32 i_speed, f32 i_start, f32 i_end) {
    mpAnm = param_0;
    field_0x40 = param_1;
    mAnmRate = param_2;

    setStartFrame(i_start);

    if (i_end < 0.0f) {
        if (mpAnm == NULL) {
            mFrameCtrl.init(0);
        } else {
            mFrameCtrl.init(mpAnm->getFrameMax());
        }
    } else {
        mFrameCtrl.init((s16)i_end);
    }

    if (i_attr < 0) {
        i_attr = param_0->getAttribute();
    }

    setPlayMode(i_attr);
    setPlaySpeed(i_speed);

    if (i_speed >= 0.0f) {
        setFrame(i_start);
    } else {
        setFrame(getEndFrame());
    }

    setLoopFrame(getFrame());
    setMorf(i_morf);

    if (mpSound != NULL) {
        if (param_2 < 0.5f) {
            mpBas = ((mDoExt_transAnmBas*)param_0)->getBas();
        } else {
            mpBas = ((mDoExt_transAnmBas*)param_1)->getBas();
        }

        if (mpBas != NULL) {
            mpSound->initAnime(mpBas, getPlaySpeed() >= 0.0f, getLoopFrame(), getFrame());
        }
    }
}

void mDoExt_McaMorf2::setAnmRate(f32 param_0) {
    void* pBas = NULL;
    mAnmRate = param_0;
    if (mpSound != NULL) {
        if (mAnmRate < 0.5f) {
            if (mpAnm != NULL) {
                pBas = ((mDoExt_transAnmBas*)mpAnm)->getBas();
            }
        } else if (field_0x40 != NULL) {
            pBas = ((mDoExt_transAnmBas*)field_0x40)->getBas();
        }
        if (pBas != mpBas) {
            if (pBas != NULL) {
                mpBas = pBas;
                mpSound->initAnime(mpBas, getPlaySpeed() >= 0.0f, getLoopFrame(), getFrame());
            } else {
                mpBas = NULL;
            }
        }
    }
}

int mDoExt_McaMorf2::play(u32 param_0, s8 param_1) {
    frameUpdate();

    if (mpSound != NULL) {
        mpSound->framework(param_0, param_1);

        if (mpBas != NULL) {
            mpSound->updateAnime(getFrame(), getPlaySpeed());
        }
    }

    return isStop();
}

void mDoExt_McaMorf2::entryDL() {
    if (mpModel != NULL) {
        mDoExt_modelEntryDL(mpModel);
    }
}

void mDoExt_McaMorf2::modelCalc() {
    if (mpModel != NULL) {
        if (mpAnm != NULL) {
            mpAnm->setFrame(mFrameCtrl.getFrame());
        }

        if (field_0x40 != NULL) {
            field_0x40->setFrame(mFrameCtrl.getFrame());
        }

        mpModel->getModelData()->getJointNodePointer(0)->setMtxCalc(this);
        mpModel->calc();
    }
}

void mDoExt_McaMorf2::stopZelAnime() {
    if (mpSound != NULL) {
        mpSound->deleteObject();
    }
}

void mDoExt_offCupOnAupPacket::draw() {
    GFSetBlendModeEtc(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_CLEAR, 0, 1, 1);
}

void mDoExt_onCupOffAupPacket::draw() {
    GFSetBlendModeEtc(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_CLEAR, 1, 0, 1);
}

void mDoExt_invJntPacket::draw() {
    J3DShape::resetVcdVatCache();

    if (field_0x16) {
        J3DModelData* sp20 = field_0x10->getModelData();
        J3DJoint* sp1C = sp20->getJointNodePointer(field_0x14);

        for (J3DMaterial* mesh = sp1C->getMesh(); mesh != NULL; mesh = mesh->getNext()) {
            mesh->load();
            J3DMatPacket* sp18 = field_0x10->getMatPacket(mesh->getIndex());
            sp18->callDL();
            GFSetZMode(1, GX_LEQUAL, 1);
            GFSetBlendModeEtc(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_CLEAR, 0, 0, 1);

            J3DShapePacket* shapePkt = sp18->getShapePacket();
            JUT_ASSERT(5011, shapePkt != NULL);
            shapePkt->getShape()->loadPreDrawSetting();

            do {
                if (!shapePkt->getShape()->checkFlag(1)) {
                    if (shapePkt->getDisplayListObj() != NULL) {
                        shapePkt->getDisplayListObj()->callDL();
                    }

                    shapePkt->drawFast();
                }

                shapePkt = (J3DShapePacket*)shapePkt->getNextPacket();
            } while (shapePkt != NULL);
        }
    } else {
        static u8 l_invisibleMat[] ATTRIBUTE_ALIGN(32) = {
            0x10, 0x00, 0x00, 0x10, 0x0E, 0x00, 0x00, 0x04, 0x00, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00,
            0x00, 0x04, 0x00, 0x61, 0x28, 0x38, 0x00, 0x00, 0x61, 0xC0, 0x08, 0xFF, 0xFC, 0x61, 0xC1,
            0x08, 0xFF, 0xF0, 0x61, 0xF3, 0x7F, 0x00, 0x00, 0x61, 0x43, 0x00, 0x00, 0x41, 0x61, 0x40,
            0x00, 0x00, 0x17, 0x61, 0xEE, 0x00, 0x00, 0x00, 0x61, 0xEF, 0x00, 0x00, 0x00, 0x61, 0xF0,
            0x00, 0x00, 0x00, 0x61, 0xF1, 0x00, 0x00, 0x00, 0x61, 0xF2, 0x00, 0x00, 0x00, 0x61, 0x41,
            0x00, 0x00, 0x04, 0x10, 0x00, 0x00, 0x10, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
            0x10, 0x09, 0x00, 0x00, 0x00, 0x01, 0x61, 0x00, 0x00, 0x40, 0x10, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        GXCallDisplayList(l_invisibleMat, 0x80);

        J3DModelData* sp14 = field_0x10->getModelData();
        J3DJoint* sp10 = sp14->getJointNodePointer(field_0x14);
        J3DMaterial* mesh = sp10->getMesh();

        sp14->getShapeNodePointer(0)->loadPreDrawSetting();

        for (; mesh != NULL; mesh = mesh->getNext()) {
            J3DShape* spC = mesh->getShape();
            if (!spC->checkFlag(1)) {
                J3DShapePacket* shapePkt = field_0x10->getShapePacket(spC->getIndex());
                shapePkt->drawFast();
            }
        }
    }

    J3DShape::resetVcdVatCache();
    GFSetBlendModeEtc(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_CLEAR, 1, 0, 1);
}

int mDoExt_3Dline_c::init(u16 param_0, int param_1, BOOL param_2) {
    field_0x0 = new cXyz[param_0];
    if (field_0x0 == NULL) {
        return 0;
    }

    if (param_1 != 0) {
        field_0x4 = new f32[param_0];
        if (field_0x4 == NULL) {
            return 0;
        }
    } else {
        field_0x4 = NULL;
    }

    int sp20 = param_0 * 2;

    field_0x8 = new cXyz[sp20];
    if (field_0x8 == NULL) {
        return 0;
    }

    field_0xc = new cXyz[sp20];
    if (field_0xc == NULL) {
        return 0;
    }

    field_0x10 = new mDoExt_3Dline_field_0x10_c[sp20];
    if (field_0x10 == NULL) {
        return 0;
    }

    field_0x14 = new mDoExt_3Dline_field_0x10_c[sp20];
    if (field_0x14 == NULL) {
        return 0;
    }

    if (param_2) {
        field_0x18 = new mDoExt_3Dline_field_0x18_c[sp20];
        if (field_0x18 == NULL) {
            return 0;
        }

        field_0x1c = new mDoExt_3Dline_field_0x18_c[sp20];
        if (field_0x1c == NULL) {
            return 0;
        }

        mDoExt_3Dline_field_0x18_c* var_r28 = field_0x18;
        mDoExt_3Dline_field_0x18_c* var_r27 = field_0x1c;
        for (s32 i = 0; i < param_0; i++) {
            var_r28++->field_0x0 = 0.0f;
            var_r27++->field_0x0 = 0.0f;

            var_r28++->field_0x0 = 1.0f;
            var_r27++->field_0x0 = 1.0f;
        }
    }

    return 1;
}

int mDoExt_3DlineMat0_c::init(u16 param_0, u16 param_1, int param_2) {
    field_0x10 = param_0;
    field_0x12 = param_1;

    field_0x18 = new mDoExt_3Dline_c[param_0];
    if (field_0x18 == NULL) {
        return 0;
    }

    for (int i = 0; i < param_0; i++) {
        if (!field_0x18[i].init(param_1, param_2, FALSE)) {
            return 0;
        }
    }

    field_0x4 = NULL;
    field_0x16 = 0;
    return 1;
}

static u8 l_matDL[132] ATTRIBUTE_ALIGN(32) = {
    0x08, 0x30, 0x3C, 0xF3, 0xCF, 0x00, 0x10, 0x00, 0x00, 0x10, 0x18, 0x3C, 0xF3, 0xCF, 0x00,
    0x10, 0x00, 0x00, 0x10, 0x0E, 0x00, 0x00, 0x7F, 0x32, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00,
    0x00, 0x05, 0x00, 0x10, 0x00, 0x00, 0x10, 0x0C, 0xFF, 0xFF, 0xFF, 0xFF, 0x61, 0x28, 0x38,
    0x00, 0x00, 0x61, 0xC0, 0x28, 0xF6, 0xAF, 0x61, 0xC1, 0x08, 0xFF, 0xE0, 0x61, 0x43, 0x00,
    0x00, 0x41, 0x61, 0x40, 0x00, 0x00, 0x17, 0x61, 0x41, 0x00, 0x00, 0x0C, 0x61, 0xF3, 0x7F,
    0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10,
    0x09, 0x00, 0x00, 0x00, 0x01, 0x61, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 l_mat1DL[141] ATTRIBUTE_ALIGN(32) = {
    0x10, 0x00, 0x00, 0x10, 0x40, 0xFF, 0xFF, 0x42, 0x80, 0x08, 0x30, 0x3C, 0xF3, 0xCF, 0x00, 0x10,
    0x00, 0x00, 0x10, 0x18, 0x3C, 0xF3, 0xCF, 0x00, 0x10, 0x00, 0x00, 0x10, 0x0E, 0x00, 0x00, 0x7F,
    0x32, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x05, 0x00, 0x10, 0x00, 0x00, 0x10, 0x0C, 0xFF,
    0xFF, 0xFF, 0xFF, 0x61, 0x28, 0x38, 0x00, 0x40, 0x61, 0xC0, 0x28, 0xFA, 0x8F, 0x61, 0xC1, 0x08,
    0xFF, 0xF0, 0x61, 0x43, 0x00, 0x00, 0x41, 0x61, 0x40, 0x00, 0x00, 0x17, 0x61, 0x41, 0x00, 0x00,
    0x0C, 0x61, 0xF3, 0x7F, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x3F, 0x00, 0x00, 0x00, 0x01, 0x10,
    0x00, 0x00, 0x10, 0x09, 0x00, 0x00, 0x00, 0x01, 0x61, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

void mDoExt_3DlineMat0_c::setMaterial() {
    j3dSys.reinitGX();
    GXSetNumIndStages(0);
    dKy_setLight_again();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxDesc(GX_VA_NRM, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_CLR_RGB, GX_RGB8, 6);
    dKy_GxFog_set();
    GXCallDisplayList(l_matDL, 0x80);
    GXLoadPosMtxImm(j3dSys.getViewMtx(), GX_PNMTX0);
    GXLoadNrmMtxImm(cMtx_getIdentity(), GX_PNMTX0);
}

void mDoExt_3DlineMat0_c::draw() {
    GXSetTevColor(GX_TEVREG2, field_0x8);

    if (field_0xc != NULL) {
        dKy_Global_amb_set(field_0xc);
    }

    mDoExt_3Dline_c* var_r28 = field_0x18;
    int var_r26 = (field_0x14 << 1) & 0xFFFF;

    for (int i = 0; i < field_0x10; i++) {
        GXSetArray(GX_VA_POS, ((mDoExt_3Dline_c*)((intptr_t)var_r28 + field_0x16 * 4))->field_0x8, sizeof(cXyz));
        GXSetArray(GX_VA_NRM, ((mDoExt_3Dline_c*)((intptr_t)var_r28 + field_0x16 * 4))->field_0x10, 3);

        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, var_r26);
        for (u16 j = 0; j < (u16)var_r26; j++) {
            GXPosition1x16(j);
            GXNormal1x16(j);

            GXPosition1x16(++j);
            GXNormal1x16(j);
        }
        GXEnd();
        var_r28++;
    }

    field_0x16 ^= (u8)1;
}

void mDoExt_3DlineMat0_c::update(int param_0, f32 param_1, GXColor& param_2, u16 param_3,
                                     dKy_tevstr_c* param_4) {
    field_0x8 = param_2;
    field_0xc = param_4;

    if (param_0 < 0) {
        field_0x14 = field_0x12;
    } else if (param_0 > field_0x12) {
        field_0x14 = field_0x12;
    } else {
        field_0x14 = (u16)param_0;
    }

    view_class* sp_2c = dComIfGd_getView();

    mDoExt_3Dline_c* sp_28 = field_0x18;

    f32 local_f30 = param_3 != 0 ? param_1 / param_3 : 0.0f;

    u32 sp_24 = (field_0x14 << 1) * 12;
    u32 sp_20 = (field_0x14 << 1) * 3;

    cXyz sp_12c;
    cXyz sp_120;
    cXyz sp_114;
    cXyz sp_108;

    cXyz* local_r27;
    cXyz* local_r26;
    cXyz* sp_1c;

    mDoExt_3Dline_field_0x10_c* local_r30;
    mDoExt_3Dline_field_0x10_c* local_r29;
    mDoExt_3Dline_field_0x10_c* sp_18;

    for (s32 sp_14 = 0; sp_14 < field_0x10; sp_14++) {
        local_r27 = sp_28->field_0x0;

        sp_1c = ((mDoExt_3Dline_c*)((intptr_t)sp_28 + (field_0x16 << 2)))->field_0x8;
        local_r26 = sp_1c;
        sp_18 = ((mDoExt_3Dline_c*)((intptr_t)sp_28 + (field_0x16 << 2)))->field_0x10;
        local_r30 = sp_18;

        local_r29 = local_r30 + 1;

        f32 local_f31 = param_1;

        sp_120 = local_r27[1] - local_r27[0];
        sp_12c = local_r27[0] - sp_2c->lookat.eye;
        sp_120 = sp_120.outprod(sp_12c);
        sp_120.normalizeZP();

        local_r30->x = sp_120.x * 64.0f;
        local_r30->y = sp_120.y * 64.0f;
        local_r30->z = sp_120.z * 64.0f;
        local_r29->x = -local_r30->x;
        local_r29->y = -local_r30->y;
        local_r29->z = -local_r30->z;

        sp_120 *= local_f31;
        *local_r26++ = *local_r27 + sp_120;
        *local_r26++ = *local_r27 - sp_120;
        local_r27++;
        sp_114 = *local_r27 + sp_120;
        sp_108 = *local_r27 - sp_120;

        for (s32 local_r19 = field_0x14 - 2; local_r19 > 0; local_r19--) {
            if (local_r19 < param_3) {
                local_f31 -= local_f30;
            }

            sp_120 = local_r27[1] - local_r27[0];
            sp_12c = local_r27[0] - sp_2c->lookat.eye;
            sp_120 = sp_120.outprod(sp_12c);
            sp_120.normalizeZP();

            local_r30 += 2;
            local_r29 += 2;

            local_r30->x = sp_120.x * 64.0f;
            local_r30->y = sp_120.y * 64.0f;
            local_r30->z = sp_120.z * 64.0f;
            local_r29->x = -local_r30->x;
            local_r29->y = -local_r30->y;
            local_r29->z = -local_r30->z;

            sp_120 *= local_f31;
            sp_114 += *local_r27 + sp_120;
            sp_108 += *local_r27 - sp_120;
            *local_r26++ = sp_114 * 0.5f;
            *local_r26++ = sp_108 * 0.5f;

            local_r27++;

            sp_114 = *local_r27 + sp_120;
            sp_108 = *local_r27 - sp_120;
        }
        local_r29 += 1;
        local_r29->x = local_r30->x;
        local_r29->y = local_r30->y;
        local_r29->z = local_r30->z;
        local_r30 += 1;
        local_r29 += 1;
        local_r29->x = local_r30->x;
        local_r29->y = local_r30->y;
        local_r29->z = local_r30->z;
        if (param_3 != 0) {
            *local_r26++ = *local_r27;
            *local_r26 = *local_r27;
        } else {
            *local_r26++ = sp_114;
            *local_r26 = sp_108;
        }
        DCStoreRangeNoSync(sp_1c, sp_24);
        DCStoreRangeNoSync(sp_18, sp_20);
        sp_28++;
    }
}

void mDoExt_3DlineMat0_c::update(int param_0, GXColor& param_2, dKy_tevstr_c* param_4) {
    field_0x8 = param_2;
    field_0xc = param_4;

    if (param_0 < 0) {
        field_0x14 = field_0x12;
    } else if (param_0 > field_0x12) {
        field_0x14 = field_0x12;
    } else {
        field_0x14 = (u16)param_0;
    }

    view_class* sp_34 = dComIfGd_getView();

    mDoExt_3Dline_c* sp_30 = field_0x18;

    u32 sp_2c = (field_0x14 << 1) * 12;
    u32 sp_28 = (field_0x14 << 1) * 3;

    cXyz sp_12c;
    cXyz sp_120;
    cXyz sp_114;
    cXyz sp_108;

    cXyz* local_r27;
    cXyz* sp_24;
    cXyz* sp_20;

    mDoExt_3Dline_field_0x10_c* local_r30;
    mDoExt_3Dline_field_0x10_c* local_r29;
    mDoExt_3Dline_field_0x10_c* sp_1c;
    f32* size_p;

    for (s32 sp_14 = 0; sp_14 < field_0x10; sp_14++) {
        local_r27 = sp_30->field_0x0;

        size_p = sp_30->field_0x4;

        JUT_ASSERT(5445, size_p != NULL);

        sp_20 = ((mDoExt_3Dline_c*)((intptr_t)sp_30 + (field_0x16 << 2)))->field_0x8;
        sp_24 = sp_20;
        sp_1c = ((mDoExt_3Dline_c*)((intptr_t)sp_30 + (field_0x16 << 2)))->field_0x10;
        local_r30 = sp_1c;

        local_r29 = local_r30 + 1;

        sp_120 = local_r27[1] - local_r27[0];
        sp_12c = local_r27[0] - sp_34->lookat.eye;
        sp_120 = sp_120.outprod(sp_12c);
        sp_120.normalizeZP();

        local_r30->x = sp_120.x * 64.0f;
        local_r30->y = sp_120.y * 64.0f;
        local_r30->z = sp_120.z * 64.0f;
        local_r29->x = -local_r30->x;
        local_r29->y = -local_r30->y;
        local_r29->z = -local_r30->z;

        sp_120 *= *size_p;
        *sp_24++ = *local_r27 + sp_120;
        *sp_24++ = *local_r27 - sp_120;
        local_r27++;
        size_p++;
        sp_114 = *local_r27 + sp_120;
        sp_108 = *local_r27 - sp_120;

        for (s32 local_r19 = field_0x14 - 2; local_r19 > 0; local_r19--) {
            sp_120 = local_r27[1] - local_r27[0];
            sp_12c = local_r27[0] - sp_34->lookat.eye;
            sp_120 = sp_120.outprod(sp_12c);
            sp_120.normalizeZP();

            local_r30 += 2;
            local_r29 += 2;

            local_r30->x = sp_120.x * 64.0f;
            local_r30->y = sp_120.y * 64.0f;
            local_r30->z = sp_120.z * 64.0f;
            local_r29->x = -local_r30->x;
            local_r29->y = -local_r30->y;
            local_r29->z = -local_r30->z;

            sp_120 *= *size_p;
            sp_114 += *local_r27 + sp_120;
            sp_108 += *local_r27 - sp_120;
            *sp_24++ = sp_114 * 0.5f;
            *sp_24++ = sp_108 * 0.5f;

            local_r27++;
            size_p++;

            sp_114 = *local_r27 + sp_120;
            sp_108 = *local_r27 - sp_120;
        }
        local_r29 += 1;
        local_r29->x = local_r30->x;
        local_r29->y = local_r30->y;
        local_r29->z = local_r30->z;
        local_r30 += 1;
        local_r29 += 1;
        local_r29->x = local_r30->x;
        local_r29->y = local_r30->y;
        local_r29->z = local_r30->z;
        *sp_24++ = sp_114;
        *sp_24 = sp_108;
        DCStoreRangeNoSync(sp_20, sp_2c);
        DCStoreRangeNoSync(sp_1c, sp_28);
        sp_30++;
    }
}

int mDoExt_3DlineMat1_c::init(u16 param_0, u16 param_1, ResTIMG* param_2, int param_3) {
    mNumLines = param_0;
    field_0x32 = param_1;
    mpLines = new mDoExt_3Dline_c[param_0];
    if (mpLines == NULL) {
        return 0;
    }

    for (s32 iVar2 = 0; iVar2 < param_0; iVar2++) {
        if (mpLines[iVar2].init(param_1, param_3, TRUE) == 0) {
            return 0;
        }
    }

    field_0x4 = 0;
    mIsDrawn = 0;

    GXInitTexObj(&mTextureObject, (void*)((intptr_t)param_2 + param_2->imageOffset), param_2->width,
                 param_2->height, (GXTexFmt)param_2->format, (GXTexWrapMode)param_2->wrapS,
                 (GXTexWrapMode)param_2->wrapT, param_2->mipmapCount > 1 ? GX_TRUE : GX_FALSE);
    GXInitTexObjLOD(&mTextureObject, (GXTexFilter)param_2->minFilter,
                    (GXTexFilter)param_2->magFilter, param_2->minLOD * 0.125f,
                    param_2->maxLOD * 0.125f, param_2->LODBias * 0.01f, (u8)param_2->biasClamp,
                    (u8)param_2->doEdgeLOD, (GXAnisotropy)param_2->maxAnisotropy);

    return 1;
}

void mDoExt_3DlineMat1_c::setMaterial() {
    j3dSys.reinitGX();
    GXSetNumIndStages(0);
    dKy_setLight_again();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxDesc(GX_VA_NRM, GX_INDEX16);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_CLR_RGB, GX_RGB8, 6);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);
    dKy_GxFog_set();
    GXCallDisplayList(l_mat1DL, 0x80);
    GXLoadPosMtxImm(j3dSys.getViewMtx(), GX_PNMTX0);
    GXLoadNrmMtxImm(cMtx_getIdentity(), GX_PNMTX0);
}

void mDoExt_3DlineMat1_c::draw() {
    GXLoadTexObj(&mTextureObject, GX_TEXMAP0);
    GXSetTexCoordScaleManually(GX_TEXCOORD0, 1, GXGetTexObjWidth(&mTextureObject), GXGetTexObjHeight(&mTextureObject));
    GXSetTevColor(GX_TEVREG2, mColor);
    if (mpTevStr != NULL) {
        dKy_Global_amb_set(mpTevStr);
    }
    mDoExt_3Dline_c* lines = mpLines;
    u16 vert_num = field_0x34 << 1;
    for (s32 i = 0; i < mNumLines; i++) {
        GXSetArray(GX_VA_POS, ((mDoExt_3Dline_c*)((s32)lines + (mIsDrawn << 2)))->field_0x8, 0xC);
        GXSetArray(GX_VA_NRM, ((mDoExt_3Dline_c*)((s32)lines + (mIsDrawn << 2)))->field_0x10, 0x3);
        GXSetArray(GX_VA_TEX0, ((mDoExt_3Dline_c*)((s32)lines + (mIsDrawn << 2)))->field_0x18, 0x8);
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, vert_num);

        u16 j = 0;
        while (j < vert_num) {
            GXPosition1x16(j);
            GXNormal1x16(j);
            GXTexCoord1x16(j);
            GXPosition1x16(++j);
            GXNormal1x16(j);
            GXTexCoord1x16(j);
            j++;
        }
        GXEnd();
        lines++;
    }
    GXSetTexCoordScaleManually(GX_TEXCOORD0, 0, 0, 0);
    mIsDrawn ^= (u8)1;
}

void mDoExt_3DlineMat1_c::update(int param_0, f32 param_1, _GXColor& param_2, u16 param_3,
                                     dKy_tevstr_c* param_4) {
    mColor = param_2;
    this->mpTevStr = param_4;
    if (param_0 < 0) {
        field_0x34 = field_0x32;
    } else if (param_0 > field_0x32) {
        field_0x34 = field_0x32;
    } else {
        field_0x34 = param_0;
    }
    view_class* sp_3c = dComIfGd_getView();
    mDoExt_3Dline_c* sp_38 = mpLines;
    f32 local_f27 = param_3 != 0 ? param_1 / param_3 : 0.0f;

    u32 sp_34 = (field_0x34 << 1) * 12;
    u32 sp_30 = (field_0x34 << 1) * 3;
    u32 sp_2c = (field_0x34 << 1) * 8;

    cXyz sp_13c;
    cXyz sp_130;
    cXyz sp_124;
    cXyz sp_118;

    cXyz* local_r27;
    cXyz* sp_28;
    cXyz* sp_24;

    mDoExt_3Dline_field_0x10_c* local_r30;
    mDoExt_3Dline_field_0x10_c* local_r28;
    mDoExt_3Dline_field_0x10_c* sp_20;

    mDoExt_3Dline_field_0x18_c* sp_1c;

    mDoExt_3Dline_field_0x18_c* sp_18;

    f32 local_f29;
    f32 local_f31 = 0.0f;

    for (s32 sp_14 = 0; sp_14 < mNumLines; sp_14++) {
        local_r27 = sp_38[0].field_0x0;
        sp_24 = ((mDoExt_3Dline_c*)((intptr_t)sp_38 + (mIsDrawn << 2)))->field_0x8;
        sp_28 = sp_24;

        sp_20 = ((mDoExt_3Dline_c*)((intptr_t)sp_38 + (mIsDrawn << 2)))->field_0x10;
        local_r30 = sp_20;
        local_r28 = local_r30 + 1;

        sp_18 = ((mDoExt_3Dline_c*)((intptr_t)sp_38 + (mIsDrawn << 2)))->field_0x18;
        sp_1c = sp_18;

        local_f29 = param_1;

        sp_1c++->field_0x4 = local_f31;
        sp_1c++->field_0x4 = local_f31;

        sp_130 = local_r27[1] - local_r27[0];
        f32 local_f30 = sp_130.abs();

        if (param_1 < 8.0f) {
            local_f31 += local_f30 * 0.1f;
        } else {
            local_f31 += local_f30 * 0.02f * (8.0f / param_1);
        }

        sp_13c = *local_r27 - sp_3c->lookat.eye;
        sp_130 = sp_130.outprod(sp_13c);
        sp_130.normalizeZP();

        local_r30->x = sp_130.x * 64.0f;
        local_r30->y = sp_130.y * 64.0f;
        local_r30->z = sp_130.z * 64.0f;
        local_r28->x = -local_r30->x;
        local_r28->y = -local_r30->y;
        local_r28->z = -local_r30->z;

        sp_130 *= local_f29;
        *sp_28++ = *local_r27 + sp_130;
        *sp_28++ = *local_r27 - sp_130;
        local_r27++;
        sp_124 = *local_r27 + sp_130;
        sp_118 = *local_r27 - sp_130;

        for (s32 sp_10 = field_0x34 - 2; sp_10 > 0; sp_10--) {
            if (sp_10 < param_3) {
                local_f29 -= local_f27;
            }

            sp_1c++->field_0x4 = local_f31;
            sp_1c++->field_0x4 = local_f31;

            sp_130 = local_r27[1] - local_r27[0];
            local_f30 = sp_130.abs();

            if (param_1 < 8.0f) {
                local_f31 += local_f30 * 0.1f;
            } else {
                local_f31 += local_f30 * 0.02f * (8.0f / param_1);
            }

            sp_13c = local_r27[0] - sp_3c->lookat.eye;
            sp_130 = sp_130.outprod(sp_13c);
            sp_130.normalizeZP();

            local_r30 += 2;
            local_r28 += 2;

            local_r30->x = sp_130.x * 64.0f;
            local_r30->y = sp_130.y * 64.0f;
            local_r30->z = sp_130.z * 64.0f;
            local_r28->x = -local_r30->x;
            local_r28->y = -local_r30->y;
            local_r28->z = -local_r30->z;

            sp_130 *= local_f29;
            sp_124 += *local_r27 + sp_130;
            sp_118 += *local_r27 - sp_130;
            *sp_28++ = sp_124 * 0.5f;
            *sp_28++ = sp_118 * 0.5f;

            local_r27++;
            sp_124 = local_r27[0] + sp_130;
            sp_118 = local_r27[0] - sp_130;
        }

        sp_1c++->field_0x4 = local_f31;
        sp_1c->field_0x4 = local_f31;

        local_r28 += 1;
        local_r28->x = local_r30->x;
        local_r28->y = local_r30->y;
        local_r28->z = local_r30->z;
        local_r30 += 1;
        local_r28 += 1;
        local_r28->x = local_r30->x;
        local_r28->y = local_r30->y;
        local_r28->z = local_r30->z;

        if (param_3 != 0) {
            *sp_28++ = *local_r27;
            *sp_28 = *local_r27;
        } else {
            *sp_28++ = sp_124;
            *sp_28 = sp_118;
        }
        DCStoreRangeNoSync(sp_24, sp_34);
        DCStoreRangeNoSync(sp_20, sp_30);
        DCStoreRangeNoSync(sp_18, sp_2c);
        sp_38++;
    }
}
#include "assets/l_mat2DL__d_a_grass.h"

void mDoExt_3DlineMat2_c::setMaterial() {
    j3dSys.reinitGX();
    GXSetNumIndStages(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxDesc(GX_VA_NRM, GX_INDEX16);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_RGB8, 6);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);
    dKy_GxFog_set();
    GXCallDisplayList(NULL, 0x80); // DEBUG NONMATCHING - this is supposed to reference l_mat2DL
    GXLoadPosMtxImm(j3dSys.getViewMtx(), 0);
    GXLoadNrmMtxImm(cMtx_getIdentity(), 0);
}

void mDoExt_3DlineMat1_c::update(int param_0, _GXColor& param_2, dKy_tevstr_c* param_4) {
    mColor = param_2;
    this->mpTevStr = param_4;
    if (param_0 < 0) {
        field_0x34 = field_0x32;
    } else if (param_0 > field_0x32) {
        field_0x34 = field_0x32;
    } else {
        field_0x34 = param_0;
    }
    view_class* stack_3c = dComIfGd_getView();
    mDoExt_3Dline_c* sp_38 = mpLines;
    f32 local_f30;
    f32 local_f29;

    u32 sp_34 = (field_0x34 << 1) * 12;
    u32 sp_30 = (field_0x34 << 1) * 3;
    u32 sp_2c = (field_0x34 << 1) * 8;
    cXyz sp_13c;
    cXyz sp_130;
    cXyz sp_124;
    cXyz sp_118;
    cXyz* local_r27;
    cXyz* sp_28;
    cXyz* sp_24;
    mDoExt_3Dline_field_0x10_c* local_r30;
    mDoExt_3Dline_field_0x10_c* local_r28;
    mDoExt_3Dline_field_0x10_c* sp_20;
    mDoExt_3Dline_field_0x18_c* sp_1c;
    mDoExt_3Dline_field_0x18_c* sp_18;
    f32 local_f31 = 0.0f;
    f32* size_p;
    for (s32 sp_14 = 0; sp_14 < mNumLines; sp_14++) {
        local_r27 = sp_38[0].field_0x0;
        size_p = sp_38->field_0x4;
        JUT_ASSERT(5875, size_p != NULL);
        sp_24 = ((mDoExt_3Dline_c*)((intptr_t)sp_38 + (mIsDrawn << 2)))->field_0x8;
        sp_28 = sp_24;
        sp_20 = ((mDoExt_3Dline_c*)((intptr_t)sp_38 + (mIsDrawn << 2)))->field_0x10;
        local_r30 = sp_20;
        local_r28 = local_r30 + 1;
        sp_18 = ((mDoExt_3Dline_c*)((intptr_t)sp_38 + (mIsDrawn << 2)))->field_0x18;
        sp_1c = sp_18;
        sp_1c++->field_0x4 = local_f31;
        sp_1c++->field_0x4 = local_f31;
        sp_130 = local_r27[1] - local_r27[0];
        local_f30 = sp_130.abs();
        local_f31 += local_f30 * 0.1f;
        sp_13c = local_r27[0] - stack_3c->lookat.eye;
        sp_130 = sp_130.outprod(sp_13c);
        sp_130.normalizeZP();
        local_r30->x = sp_130.x * 64.0f;
        local_r30->y = sp_130.y * 64.0f;
        local_r30->z = sp_130.z * 64.0f;
        local_r28->x = -local_r30->x;
        local_r28->y = -local_r30->y;
        local_r28->z = -local_r30->z;
        sp_130 *= *size_p;
        *sp_28++ = local_r27[0] + sp_130;
        *sp_28++ = local_r27[0] - sp_130;
        local_r27++;
        size_p++;
        sp_124 = local_r27[0] + sp_130;
        sp_118 = local_r27[0] - sp_130;
        for (s32 sp_10 = field_0x34 - 2; sp_10 > 0; sp_10--) {
            sp_1c++->field_0x4 = local_f31;
            sp_1c++->field_0x4 = local_f31;
            sp_130 = local_r27[1] - local_r27[0];
            local_f30 = sp_130.abs();
            local_f31 += local_f30 * 0.1f;
            sp_13c = local_r27[0] - stack_3c->lookat.eye;
            sp_130 = sp_130.outprod(sp_13c);
            sp_130.normalizeZP();
            local_r30 += 2;
            local_r28 += 2;
            local_r30->x = sp_130.x * 64.0f;
            local_r30->y = sp_130.y * 64.0f;
            local_r30->z = sp_130.z * 64.0f;
            local_r28->x = -local_r30->x;
            local_r28->y = -local_r30->y;
            local_r28->z = -local_r30->z;
            sp_130 *= *size_p;
            sp_124 += local_r27[0] + sp_130;
            sp_118 += local_r27[0] - sp_130;
            *sp_28++ = sp_124 * 0.5f;
            *sp_28++ = sp_118 * 0.5f;
            local_r27++;
            size_p++;
            sp_124 = local_r27[0] + sp_130;
            sp_118 = local_r27[0] - sp_130;
        }

        sp_1c++->field_0x4 = local_f31;
        sp_1c->field_0x4 = local_f31;

        local_r28 += 1;
        local_r28->x = local_r30->x;
        local_r28->y = local_r30->y;
        local_r28->z = local_r30->z;
        local_r30 += 1;
        local_r28 += 1;
        local_r28->x = local_r30->x;
        local_r28->y = local_r30->y;
        local_r28->z = local_r30->z;

        *sp_28++ = sp_124;
        *sp_28 = sp_118;
        DCStoreRangeNoSync(sp_24, sp_34);
        DCStoreRangeNoSync(sp_20, sp_30);
        DCStoreRangeNoSync(sp_18, sp_2c);
        sp_38 += 1;
    }
}

void mDoExt_3DlineMatSortPacket::setMat(mDoExt_3DlineMat_c* i_3DlineMat) {
    if (mp3DlineMat == NULL) {
        dComIfGd_getListPacket()->entryImm(this, 0);
    }
    i_3DlineMat->field_0x4 = mp3DlineMat;
    mp3DlineMat = i_3DlineMat;
}

void mDoExt_3DlineMatSortPacket::draw() {
    mp3DlineMat->setMaterial();
    mDoExt_3DlineMat_c* lineMat = mp3DlineMat;
    do {
        lineMat->draw();
        lineMat = lineMat->field_0x4;
    } while (lineMat != NULL);
    J3DShape::resetVcdVatCache();
}

#if DEBUG
mDoExt_cube8pPacket::mDoExt_cube8pPacket(cXyz* i_points, const GXColor& i_color) {
    cXyz* pnt_array = mPoints;

    for (int i = 0; i < 8; i++) {
        *(pnt_array)++ = *(i_points)++;
    }

    DCStoreRangeNoSync(mPoints, sizeof(cXyz) * 8);
    mColor = i_color;
}

void drawCube(MtxP mtx, cXyz* pos, const GXColor& color) {
    GXSetArray(GX_VA_POS, pos, sizeof(cXyz));
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP,
                  GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetCullMode(GX_CULL_BACK);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 14);
    GXPosition1x8(4);
    GXPosition1x8(6);
    GXPosition1x8(5);
    GXPosition1x8(7);
    GXPosition1x8(3);
    GXPosition1x8(6);
    GXPosition1x8(2);
    GXPosition1x8(4);
    GXPosition1x8(0);
    GXPosition1x8(5);
    GXPosition1x8(1);
    GXPosition1x8(3);
    GXPosition1x8(0);
    GXPosition1x8(2);
    GXEnd();
}

void mDoExt_cube8pPacket::draw() {
    drawCube(j3dSys.getViewMtx(), mPoints, mColor);
}

mDoExt_cubePacket::mDoExt_cubePacket(cXyz& i_position, cXyz& i_size, csXyz& i_angle, const GXColor& i_color) {
    mPosition = i_position;
    mSize = i_size;
    mAngle = i_angle;
    mColor = i_color;
}

void mDoExt_cubePacket::draw() {
    static cXyz l_pos[8] = {
        cXyz(-1.0f, 1.0f, -1.0f), cXyz(1.0f, 1.0f, -1.0f),   cXyz(-1.0f, 1.0f, 1.0f),
        cXyz(1.0f, 1.0f, 1.0f),   cXyz(-1.0f, -1.0f, -1.0f), cXyz(1.0f, -1.0f, -1.0f),
        cXyz(-1.0f, -1.0f, 1.0f), cXyz(1.0f, -1.0f, 1.0f),
    };

    mDoMtx_stack_c::transS(mPosition.x, mPosition.y, mPosition.z);
    mDoMtx_stack_c::XYZrotM(mAngle.x, mAngle.y, mAngle.z);
    mDoMtx_stack_c::scaleM(mSize.x, mSize.y, mSize.z);
    mDoMtx_stack_c::revConcat(j3dSys.getViewMtx());
    drawCube(mDoMtx_stack_c::get(), l_pos, mColor);
}

mDoExt_quadPacket::mDoExt_quadPacket(cXyz* i_points, const GXColor& i_color, u8 i_clipZ) {
    cXyz* pnt_array = mPoints;

    for (int i = 0; i < 4; i++) {
        *(pnt_array)++ = *(i_points)++;
    }

    DCStoreRangeNoSync(mPoints, sizeof(cXyz) * 4);
    mColor = i_color;
    mClipZ = i_clipZ;
}

void mDoExt_quadPacket::draw() {
    GXSetArray(GX_VA_POS, mPoints, sizeof(cXyz));
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP,
                  GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColor(GX_TEVREG0, mColor);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);

    if (mClipZ) {
        GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    } else {
        GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
    }

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetCullMode(GX_CULL_BACK);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXLoadPosMtxImm(j3dSys.getViewMtx(), 0);
    GXSetCurrentMtx(0);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition1x8(0);
    GXPosition1x8(1);
    GXPosition1x8(2);
    GXPosition1x8(3);
    GXEnd();
}

mDoExt_trianglePacket::mDoExt_trianglePacket(cXyz* i_points, const GXColor& i_color, u8 i_clipZ) {
    cXyz* pnt_array = mPoints;

    for (int i = 0; i < 3; i++) {
        *(pnt_array)++ = *(i_points)++;
    }

    DCStoreRangeNoSync(mPoints, sizeof(cXyz) * 3);
    mColor = i_color;
    mClipZ = i_clipZ;
}

void mDoExt_trianglePacket::draw() {
    j3dSys.reinitGX();

    GXSetArray(GX_VA_POS, mPoints, sizeof(cXyz));
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXLoadPosMtxImm(j3dSys.getViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColor(GX_TEVREG0, mColor);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetZCompLoc(GX_ENABLE);

    if (mClipZ) {
        GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    } else {
        GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
    }

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, g_clearColor);
    GXSetFogRangeAdj(GX_DISABLE, 0, NULL);
    GXSetCullMode(GX_CULL_NONE);
    GXSetDither(GX_ENABLE);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetNumIndStages(0);

    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
    GXPosition1x8(0);
    GXPosition1x8(1);
    GXPosition1x8(2);
    GXEnd();

    J3DShape::resetVcdVatCache();
}

mDoExt_linePacket::mDoExt_linePacket(cXyz& i_start, cXyz& i_end, const GXColor& i_color, u8 i_clipZ, u8 i_width) {
    mStart = i_start;
    mEnd = i_end;
    mColor = i_color;
    mClipZ = i_clipZ;
    mWidth = i_width;
}

void mDoExt_linePacket::draw() {
    j3dSys.reinitGX();

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXLoadPosMtxImm(j3dSys.getViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColor(GX_TEVREG0, mColor);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetZCompLoc(GX_ENABLE);

    if (mClipZ) {
        GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    } else {
        GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
    }

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, g_clearColor);
    GXSetFogRangeAdj(GX_DISABLE, 0, NULL);
    GXSetCullMode(GX_CULL_NONE);
    GXSetDither(GX_ENABLE);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetNumIndStages(0);
    GXSetLineWidth(mWidth, GX_TO_ZERO);

    GXBegin(GX_LINES, GX_VTXFMT0, 2);
    GXPosition3f32(mStart.x, mStart.y, mStart.z);
    GXPosition3f32(mEnd.x, mEnd.y, mEnd.z);
    GXEnd();

    J3DShape::resetVcdVatCache();
}

mDoExt_ArrowPacket::mDoExt_ArrowPacket(cXyz& i_position, cXyz& param_1, const GXColor& i_color, u8 i_clipZ, u8 i_lineWidth) {
    mStart = i_position;
    mEnd = param_1;
    mColor = i_color;
    mClipZ = i_clipZ;
    mLineWidth = i_lineWidth;
}

void mDoExt_ArrowPacket::draw() {
    Mtx sp28;
    cXyz sp18;

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColor(GX_TEVREG0, mColor);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);

    if (mClipZ) {
        GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    } else {
        GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
    }

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetLineWidth(mLineWidth, GX_TO_ZERO);

    sp18 = mEnd - mStart;
    MtxTrans(mStart.x, mStart.y, mStart.z, 0);
    cMtx_YrotM(*calc_mtx, sp18.atan2sX_Z());
    cMtx_XrotM(*calc_mtx, cM_atan2s(JMAFastSqrt(SQUARE(sp18.x) + SQUARE(sp18.z)), sp18.y));
    cMtx_concat(j3dSys.getViewMtx(), *calc_mtx, sp28);

    GXLoadPosMtxImm(sp28, 0);
    GXSetCurrentMtx(0);

    GXBegin(GX_LINES, GX_VTXFMT0, 2);
    GXPosition3f32(0.0f, 0.0f, 0.0f);
    GXPosition3f32(0.0f, sp18.abs(), 0.0f);
    GXEnd();

    f32 var_f29 = sp18.abs();
    f32 var_f31 = var_f29 * 0.1f;
    f32 var_f30 = var_f29 * 0.8f;

    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 6);
    GXPosition3f32(0.0f, var_f29, 0.0f);
    GXPosition3f32(0.0f, var_f30, var_f31);
    GXPosition3f32(var_f31, var_f30, 0.0f);
    GXPosition3f32(0.0f, var_f30, -var_f31);
    GXPosition3f32(-var_f31, var_f30, 0.0f);
    GXPosition3f32(0.0f, var_f30, var_f31);
    GXEnd();
}

mDoExt_pointPacket::mDoExt_pointPacket(cXyz& i_position, const GXColor& i_color, u8 i_clipZ, u8 i_lineWidth) {
    mPosition = i_position;
    mColor = i_color;
    mClipZ = i_clipZ;
    mLineWidth = i_lineWidth;
}

void mDoExt_pointPacket::draw() {
    j3dSys.reinitGX();

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColor(GX_TEVREG0, mColor);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);

    if (mClipZ) {
        GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    } else {
        GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
    }

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetPointSize(mLineWidth, GX_TO_ZERO);

    GXLoadPosMtxImm(j3dSys.getViewMtx(), 0);
    GXSetCurrentMtx(0);

    GXBegin(GX_POINTS, GX_VTXFMT0, 1);
    GXPosition3f32(mPosition.x, mPosition.y, mPosition.z);
    GXEnd();

    j3dSys.reinitGX();
    J3DShape::resetVcdVatCache();
}

mDoExt_circlePacket::mDoExt_circlePacket(cXyz& i_position, f32 i_radius, const GXColor& i_color, u8 i_clipZ, u8 i_lineWidth) {
    mPosition = i_position;
    mRadius = i_radius;
    mColor = i_color;
    mClipZ = i_clipZ;
    mLineWidth = i_lineWidth;
}

void mDoExt_circlePacket::draw() {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColor(GX_TEVREG0, mColor);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);

    if (mClipZ) {
        GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    } else {
        GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
    }

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetLineWidth(mLineWidth, GX_TO_ZERO);
    GXLoadPosMtxImm(j3dSys.getViewMtx(), 0);
    GXSetCurrentMtx(0);

    cXyz sp38;
    cXyz sp44;
    int numEdges = 36;
    sp38.y = sp44.y = mPosition.y;
    
    GXBegin(GX_LINES, GX_VTXFMT0, numEdges * 2);
    for (int i = 0; i < numEdges; i++) {
        sp38.x = cM_fcos((i * 6.2831855f) / numEdges) * mRadius;
        sp38.z = cM_fsin((i * 6.2831855f) / numEdges) * mRadius;

        sp44.x = cM_fcos(((i + 1) * 6.2831855f) / numEdges) * mRadius;
        sp44.z = cM_fsin(((i + 1) * 6.2831855f) / numEdges) * mRadius;

        sp38.x += mPosition.x;
        sp38.z += mPosition.z;
        sp44.x += mPosition.x;
        sp44.z += mPosition.z;
        GXPosition3f32(sp38.x, sp38.y, sp38.z);
        GXPosition3f32(sp44.x, sp44.y, sp44.z);
    }
    GXEnd();
}

mDoExt_spherePacket::mDoExt_spherePacket(cXyz& i_position, f32 i_size, const GXColor& i_color, u8 i_clipZ) {
    mPosition = i_position;
    mSize = i_size;
    mColor = i_color;
    mClipZ = i_clipZ;
}

void mDoExt_spherePacket::draw() {
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColor(GX_TEVREG0, mColor);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);

    if (mClipZ) {
        GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    } else {
        GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
    }

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetCullMode(GX_CULL_BACK);
    GXSetClipMode(GX_CLIP_ENABLE);

    mDoMtx_stack_c::copy(j3dSys.getViewMtx());
    mDoMtx_stack_c::transM(mPosition.x, mPosition.y, mPosition.z);
    mDoMtx_stack_c::scaleM(mSize, mSize, mSize);

    GXLoadPosMtxImm(mDoMtx_stack_c::get(), 0);
    mDoMtx_stack_c::inverseTranspose();

    GXLoadNrmMtxImm(mDoMtx_stack_c::get(), 0);
    GXSetCurrentMtx(0);

    GXDrawSphere(8, 8);
}

mDoExt_cylinderPacket::mDoExt_cylinderPacket(cXyz& i_position, f32 i_radius, f32 i_height, const GXColor& i_color, u8 i_clipZ) {
    mPosition = i_position;
    mRadius = i_radius;
    mHeight = i_height;
    mColor = i_color;
    mClipZ = i_clipZ;
}

void mDoExt_cylinderPacket::draw() {
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, 1, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColor(GX_TEVREG0, mColor);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

    if (mClipZ) {
        GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    } else {
        GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
    }

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetCullMode(GX_CULL_BACK);
    GXSetClipMode(GX_CLIP_ENABLE);

    f32 var_f31 = mHeight * 0.5f;

    mDoMtx_stack_c::copy(j3dSys.getViewMtx());
    mDoMtx_stack_c::transM(mPosition.x, mPosition.y + var_f31, mPosition.z);
    mDoMtx_stack_c::scaleM(mRadius, var_f31, mRadius);
    mDoMtx_stack_c::XrotM(0x4000);

    GXLoadPosMtxImm(mDoMtx_stack_c::get(), 0);
    mDoMtx_stack_c::inverseTranspose();

    GXLoadNrmMtxImm(mDoMtx_stack_c::get(), 0);
    GXSetCurrentMtx(0);
    GXDrawCylinder(8);
}

mDoExt_cylinderMPacket::mDoExt_cylinderMPacket(Mtx i_mtx, const GXColor& i_color, u8 i_clipZ) {
    cMtx_copy(i_mtx, mMatrix);
    mColor = i_color;
    mClipZ = i_clipZ;
}

void mDoExt_cylinderMPacket::draw() {
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColor(GX_TEVREG0, mColor);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);

    if (mClipZ) {
        GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    } else {
        GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
    }

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetCullMode(GX_CULL_BACK);
    GXSetClipMode(GX_CLIP_ENABLE);

    cMtx_concat(j3dSys.getViewMtx(), mMatrix, mMatrix);

    GXLoadPosMtxImm(mMatrix, 0);
    cMtx_inverseTranspose(mMatrix, mMatrix);

    GXLoadNrmMtxImm(mMatrix, 0);
    GXSetCurrentMtx(0);

    GXDrawCylinder(8);
}
#endif

static void mDoExt_initFontCommon(JUTFont** mDoExt_font_p, ResFONT** mDoExt_resfont_p, JKRHeap* param_2,
                                  char const* param_3, JKRArchive* param_4, u8 param_5,
                                  u32 param_6, u32 param_7) {
    JUTFont** mDoExt_font = mDoExt_font_p;
    ResFONT** mDoExt_resfont = mDoExt_resfont_p;
    // these assertion messages are fakematches, not quite sure what's going on here -
    // the assembly indicates the parameters are dereferenced inline into compiler temps,
    // but the messages don't show a dereference
    JUT_ASSERT_MSG(7141, *mDoExt_font == NULL, "mDoExt_font == 0");
    JUT_ASSERT_MSG(7142, *mDoExt_resfont == NULL, "mDoExt_resfont == 0");
    *mDoExt_resfont = (ResFONT*)JKRGetResource('ROOT', param_3, param_4);
    JUT_ASSERT_MSG(7144, *mDoExt_resfont != NULL, "mDoExt_resfont != 0");
    if (param_5 == 0) {
        u32 cacheSize = JUTCacheFont::calcCacheSize(param_7, param_6);
        JUTCacheFont* cacheFont = new (param_2, 0) JUTCacheFont(*mDoExt_resfont, cacheSize, param_2);
        if (cacheFont->isValid()) {
            // "Allocated %d bytes for font cache buffer size"
            OS_REPORT("\nキャッシュフォントバッファサイズとして %d バイト確保しました\n", cacheSize);
            // "The size of the JUTCacheFont class is %d bytes."
            OS_REPORT("JUTCacheFontクラスサイズは %d バイトです。\n", sizeof(JUTCacheFont));
            // "The number of cached pages is %d."
            OS_REPORT("キャッシュページ数は %d です。\n\n", cacheFont->getCachePage());
            // "The maximum sheet size is %d."
            OS_REPORT("シートサイズ（最大）は %d です。\n\n", cacheFont->getMaxSheetSize());
            *mDoExt_font = cacheFont;
            cacheFont->setPagingType(JUTCacheFont::PAGE_TYPE_1);
        }
        JKRRemoveResource(*mDoExt_resfont, NULL);
        *mDoExt_resfont = NULL;
    } else {
        *mDoExt_font = new JUTResFont(*mDoExt_resfont, param_2);
        // "Allocated %d bytes for the font buffer size"
        OS_REPORT("\nフォントバッファサイズとして %d バイト確保しました\n",
                  JKRGetMemBlockSize(param_2, *mDoExt_font));
        // "The size of the JUTResFont class is %d bytes."
        OS_REPORT("JUTResFontクラスサイズは %d バイトです。\n", sizeof(JUTResFont));
    }
    if (*mDoExt_font != NULL && !(*mDoExt_font)->isValid()) {
        // "Failed to create cache font class"
        OSReport_FatalError("\nキャッシュフォントクラス作成に失敗しました\n");
        delete *mDoExt_font;
        *mDoExt_font = NULL;
    }
    JUT_ASSERT_MSG(7183, *mDoExt_font != NULL, "mDoExt_font != 0");
}

static JUTFont* mDoExt_font0;

static int mDoExt_font0_getCount;

static ResFONT* mDoExt_resfont0;

static void mDoExt_initFont0() {
    static char const fontdata[] = "rodan_b_24_22.bfn";
#if REGION_JPN
    mDoExt_initFontCommon(&mDoExt_font0, &mDoExt_resfont0, mDoExt_getZeldaHeap(),
                          fontdata, dComIfGp_getFontArchive(), 0, 200, 512);
#else
    mDoExt_initFontCommon(&mDoExt_font0, &mDoExt_resfont0, mDoExt_getZeldaHeap(),
                          fontdata, dComIfGp_getFontArchive(), 1, 0, 0);
#endif
}

JUTFont* mDoExt_getMesgFont() {
    if (mDoExt_font0 == NULL) {
        mDoExt_initFont0();
    }

    mDoExt_font0_getCount++;
    return mDoExt_font0;
}

void mDoExt_removeMesgFont() {
    JUT_ASSERT(7238, mDoExt_font0_getCount > 0);
    if (mDoExt_font0_getCount > 0) {
        mDoExt_font0_getCount--;
        JUT_ASSERT(7241, mDoExt_font0_getCount > 0);
        if (mDoExt_font0_getCount == 0) {
            delete mDoExt_font0;
            mDoExt_font0 = NULL;
            if (mDoExt_resfont0 != NULL) {
#if REGION_JPN
                JKRFileLoader::removeResource(mDoExt_resfont0, NULL);
#else
                JKRFree(mDoExt_resfont0);
#endif
                mDoExt_resfont0 = NULL;
            }
        }
    }
}

static JUTFont* mDoExt_font1;

static int mDoExt_font1_getCount;

static ResFONT* mDoExt_resfont1;

static void mDoExt_initFont1() {
    static char const fontdata[] = "reishotai_24_22.bfn";
    mDoExt_initFontCommon(&mDoExt_font1, &mDoExt_resfont1, mDoExt_getZeldaHeap(),
                          fontdata, dComIfGp_getRubyArchive(), 1, 1, 0x8000);
}

JUTFont* mDoExt_getRubyFont() {
    if (mDoExt_font1 == NULL) {
        mDoExt_initFont1();
    }

    mDoExt_font1_getCount++;
    return mDoExt_font1;
}

static JUTFont* mDoExt_font2;

static int mDoExt_font2_getCount;

static ResFONT* mDoExt_resfont2;

static void mDoExt_initFont2() {
    static char const fontdata[] = "reishotai_24_22.bfn";
    mDoExt_initFontCommon(&mDoExt_font2, &mDoExt_resfont2, mDoExt_getZeldaHeap(),
                          fontdata, dComIfGp_getRubyArchive(), 1, 1, 0x8000);
}

JUTFont* mDoExt_getSubFont() {
    if (mDoExt_font2 == NULL) {
        mDoExt_initFont2();
    }

    mDoExt_font2_getCount++;
    return mDoExt_font2;
}

void dummy9() {
    JUT_ASSERT(0, mDoExt_font1_getCount > 0);
}

void mDoExt_removeSubFont() {
    JUT_ASSERT(7354, mDoExt_font2_getCount > 0);
    if (mDoExt_font2_getCount > 0) {
        mDoExt_font2_getCount--;
        JUT_ASSERT(7357, mDoExt_font2_getCount > 0);
        if (mDoExt_font2_getCount == 0) {
            delete mDoExt_font2;
            mDoExt_font2 = NULL;
            if (mDoExt_resfont2 != NULL) {
                JKRFree(mDoExt_resfont2);
                mDoExt_resfont2 = NULL;
            }
        }
    }
}

J3DModel* mDoExt_J3DModel__create(J3DModelData* i_modelData, u32 i_modelFlag, u32 i_differedDlistFlag) {
    if (i_modelData != NULL) {
        J3DModel* model = new J3DModel();

        if (model != NULL) {
#if DEBUG
            model->getBaseTRMtx()[0][0] = MAXFLOAT;
#endif
            bool hasSharedDlistObj =
                i_modelData->getMaterialNodePointer(0)->getSharedDisplayListObj() != NULL;
            // Update the modelFlag if the model data passed in has a shared dlist object
            if (hasSharedDlistObj != NULL) {
                if (i_modelData->isLocked()) {
                    i_modelFlag = J3DMdlFlag_UseSharedDL;
                } else if (i_modelFlag == J3DMdlFlag_UseSharedDL) {
                    i_modelFlag |= J3DMdlFlag_UseSingleDL;
                } else {
                    i_modelFlag = J3DMdlFlag_DifferedDLBuffer;
                }
            }

            // Set up the model
            s32 entryModelData = model->entryModelData(i_modelData, i_modelFlag, 1);
            if (entryModelData == kJ3DError_Success) {
                if (i_modelFlag == J3DMdlFlag_DifferedDLBuffer &&
                    model->newDifferedDisplayList(i_differedDlistFlag) != kJ3DError_Success)
                {
                    return NULL;
                }

                model->lock();
                return model;
            }
        }
    }

    return NULL;
}

DummyCheckHeap* dch;

DummyCheckHeap::DummyCheckHeap() {
    mAlloc = NULL;
    mSize = 0;
    mHeap = NULL;
}

DummyCheckHeap::~DummyCheckHeap() {
    if (mHeap != NULL) {
        mHeap->destroy();
    }
}

int DummyCheckHeap::isVirgin() {
    if (mAlloc != NULL && mSize > 0) {
        for (int i = 0; i < mSize; i++) {
            if (((u8*)mAlloc)[i] != 0xdd) {
                u8* off = (u8*)mAlloc + i;
                // "DummyCheckHeap: Heap corruption detected"
                OS_REPORT_ERROR("DummyCheckHeap 領域破壊発見 %08x+%08x=%08x\n", mAlloc, i, off);
                return false;
            }
        }
    }
    return true;
}

JKRHeap* DummyCheckHeap::getHeap() {
    if (mHeap != NULL) {
        JUT_ASSERT(7604, mHeap);
        JUT_ASSERT(7605, mAlloc);
        JKRHeap* heap = mHeap;
        JUT_ASSERT(7607, mSize > 0);
        isVirgin();
        JKRFreeToHeap(mHeap, mAlloc);
        mHeap = NULL;
        mAlloc = NULL;
        return heap;
    } else {
        return NULL;
    }
}

void DummyCheckHeap::setHeap(JKRHeap* heap) {
    JUT_ASSERT(7620, heap != NULL);
    JUT_ASSERT(7621, mHeap == NULL);
    JUT_ASSERT(7622, mAlloc == NULL);
    mHeap = heap;
    mSize = JKRGetFreeSize(mHeap);
    JUT_ASSERT(7625, mSize > 0);
    mAlloc = JKRAllocFromHeap(mHeap, mSize, 0);
    JUT_ASSERT(7627, mAlloc);
    memset(mAlloc, 0xdd, mSize);
}

int DummyCheckHeap_init() {
    JUT_ASSERT(7634, dch == NULL);
    dch = new((JKRHeap*)mDoExt_getZeldaHeap(), 0) DummyCheckHeap();
    if (dch == NULL) {
        return false;
    }

    return true;
}

int DummyCheckHeap_create() {
    if (dch != NULL) {
        if (dch->mHeap == NULL) {
            JKRHeap* rootHeap = JKRGetRootHeap2();
            JKRHeap* heap = JKRCreateExpHeap(0x60000, rootHeap, false);
            if (heap == NULL) {
                // "DummyCheckHeap allocation failure"
                OS_REPORT_ERROR("DummyCheckHeap_create 領域確保失敗\n");
                return 0;
            }
            dch->setHeap(heap);
        }
        return 1;
    }
    return 0;
}

void DummyCheckHeap_destroy() {
    if (dch != NULL && dch->mHeap != NULL) {
        JKRHeap* heap = dch->getHeap();
        if (heap != NULL) {
            heap->destroy();
        }
    }
}

int DummyCheckHeap_isVirgin() {
    int retval;
    if (dch != NULL) {
        retval = dch->isVirgin();
    } else {
        retval = 1;
    }
    return retval;
}

void DummyCheckHeap_check() {
    if (dch != NULL) {
       DummyCheckHeap_isVirgin();
    }
}

u32 aram_cache_size;

u32 mDoExt_getAraCacheSize() {
    return aram_cache_size;
}

void mDoExt_setAraCacheSize(u32 size) {
    aram_cache_size = size;
}

OSThread* mDoExt_GetCurrentRunningThread() {
    OSThread* thread = OSGetCurrentThread();
    if (thread != NULL && thread->state != 2) {
        thread = NULL;
    }

    return thread;
}

// TODO: hack to get these inlines to appear for debug
static void dummy() {
    { J3DZMode temp; temp = temp; J3DZMode temp2(temp); }
    { J3DBlend temp; temp = temp; J3DBlend temp2(temp); }
    { J3DAlphaComp temp; temp = temp; J3DAlphaComp temp2(temp); }
    { J3DIndTexCoordScale temp; temp = temp; J3DIndTexCoordScale temp2(temp); }
    { J3DIndTexMtx temp; temp = temp; J3DIndTexMtx temp2(temp); }
    { J3DIndTevStage temp; temp = temp; J3DIndTevStage temp2(temp); }
    { J3DTevStage temp; temp = temp; J3DTevStage temp2(temp); }
    { J3DTevSwapModeTable temp; temp = temp; J3DTevSwapModeTable temp2(temp); }
    { J3DTevOrder temp; temp = temp; J3DTevOrder temp2(temp); }
    { J3DGXColorS10 temp; temp = temp; J3DGXColorS10 temp2(temp); }
    { J3DTexCoord temp; temp = temp; J3DTexCoord temp2(temp); }
    { J3DColorChan temp; temp = temp; J3DColorChan temp2(temp); }
    { J3DGXColor temp; temp = temp; J3DGXColor temp2(temp); }
    { J3DIndTexOrderInfo temp; temp = temp; J3DIndTexOrderInfo temp2(temp); }
    { J3DFog temp; temp = temp; J3DFog temp2(temp); }
    { J3DTexMtx temp; temp = temp; J3DTexMtx temp2(temp); }
}
