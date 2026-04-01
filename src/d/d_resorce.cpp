#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_resorce.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "JSystem/J3DGraphAnimator/J3DMaterialAnm.h"
#include "JSystem/J3DGraphLoader/J3DAnmLoader.h"
#include "JSystem/J3DGraphLoader/J3DClusterLoader.h"
#include "JSystem/J3DGraphLoader/J3DModelLoader.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "d/d_bg_w_kcol.h"
#include "d/d_com_inf_game.h"
#include "f_ap/f_ap_game.h"
#include "f_op/f_op_camera_mng.h"
#include "m_Do/m_Do_graphic.h"
#include "res/Object/Always.h"
#include <cstdio>
#include <cstring>

#include "dusk/logging.h"

#ifndef __MWERKS__
#include "dusk/extras.h"
#endif

dRes_info_c::dRes_info_c() {
    mCount = 0;
    mDMCommand = NULL;
    mArchive = NULL;
    heap = NULL;
    mDataHeap = NULL;
    mRes = NULL;
}

dRes_info_c::~dRes_info_c() {
    if (mDMCommand != NULL) {
        mDMCommand->destroy();
        mDMCommand = NULL;
    } else if (mArchive != NULL) {
        deleteArchiveRes();
        if (mDataHeap != NULL) {
            mDoExt_destroySolidHeap(mDataHeap);
            mDataHeap = NULL;
            mArchive->unmount();
        }
        mRes = NULL;
        mArchive = NULL;
    }
}

int dRes_info_c::set(char const* i_arcName, char const* i_path, u8 i_mountDirection, JKRHeap* i_heap) {
#ifdef __MWERKS__
    JUT_ASSERT(120, strlen(i_arcName) <= NAME_MAX);
#endif

    if (*i_path != '\0') {
        char path[40];
        snprintf(path, sizeof(path), "%s%s.arc", i_path, i_arcName);
        mDMCommand = mDoDvdThd_mountArchive_c::create(path, i_mountDirection, i_heap);

        if (mDMCommand == NULL) {
            return false;
        }
    }

    strncpy(mArchiveName, i_arcName, sizeof(mArchiveName) - 1);
    return true;
}

static void setAlpha(J3DMaterialTable* i_matTable) {
    for (u16 i = 0; i < i_matTable->getMaterialNum(); i++) {
        J3DTevBlock* tevBlock = i_matTable->getMaterialNodePointer(i)->getTevBlock();

        if (tevBlock != NULL) {
            GXColorS10* tevColor = tevBlock->getTevColor(3);
            if (tevColor != NULL) {
                tevColor->a = tevBlock->getTevStageNum();
            }
        }
    }
}

static void setIndirectTex(J3DModelData* i_modelData) {
    const char* textureName;
    J3DMaterialTable& materialTable = i_modelData->getMaterialTable();
    J3DTexture* texture = materialTable.getTexture();
    if (texture == NULL)
        return;

    JUTNameTab* nameTab = materialTable.getTextureName();
    if (nameTab == NULL)
        return;

    for (u16 i = 0; i < texture->getNum(); i++) {
        textureName = nameTab->getName(i);
        if (memcmp(textureName, "fbtex_dummy", 0xc) == 0) {
            texture->setResTIMG(i, *mDoGph_gInf_c::getFrameBufferTimg());
        }
        if (memcmp(textureName, "dummy", 6) == 0) {
            texture->setResTIMG(i, *mDoGph_gInf_c::getFrameBufferTimg());
        }
#if !TARGET_PC
        if (memcmp(textureName, "Zbuffer", 8) == 0) {
            texture->setResTIMG(i, *mDoGph_gInf_c::getZbufferTimg());
        }
#endif
    }
}

static void setAlpha(J3DModelData* i_modelData) {
    setAlpha(&i_modelData->getMaterialTable());
}

static const J3DTexMtxInfo l_texMtxInfo = {
    0x00,
    0x08, 0x00, 0x00,
    {0.5f, 0.5f, 0.0f},
    {0.1f, 0.1f, 0, 0.0f, 0.0f},
    {
        {0.5f, 0.0f, 0.0f, 0.5f},
        {0.0f, 0.5f, 0.0f, 0.5f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    },
};

static void addWarpMaterial(J3DModelData* i_modelData) {
    static J3DTexCoordInfo l_texCoordInfo = {0x00, 0x00, 0x27};
    static J3DTevOrderInfo l_tevOrderInfo = {0x00, 0x03, 0xFF, 0x00};
    static J3DTevStageInfo const l_tevStageInfo = {
        0x05, 0x0F, 0x08, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x01, 0x00,
        0x07, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x01, 0x00,
    };
    static J3DAlphaCompInfo const l_alphaCompInfo = {0x04, 0x80, 0x00, 0x03, 0xFF};

    ResTIMG* resTimg = (ResTIMG*)dComIfG_getObjectRes("Always", dRes_ID_ALWAYS_BTI_WARP_TEX_e);
    JUT_ASSERT(279, resTimg != NULL);

    J3DTexture* texture = i_modelData->getTexture();
    u16 textureNum = texture->getNum();
    texture->addResTIMG(1, resTimg - textureNum);

    J3DTexMtx* newTexMtx = JKR_NEW J3DTexMtx(l_texMtxInfo);
    JUT_ASSERT(285, newTexMtx != NULL);

    for (u16 i = 0; i < i_modelData->getMaterialNum(); i++) {
        J3DMaterial* material = i_modelData->getMaterialNodePointer(i);
        J3DTexGenBlock* texGenBlock = material->getTexGenBlock();
        u32 texGenNum = texGenBlock->getTexGenNum();
        JUT_ASSERT(290, texGenNum < 4);

        J3DTexCoord* coord = texGenBlock->getTexCoord(texGenNum);
        l_texCoordInfo.mTexGenMtx = texGenNum * 3 + GX_TEXMTX0;
        coord->setTexCoordInfo(l_texCoordInfo);
        coord->resetTexMtxReg();

        texGenBlock->setTexGenNum(texGenNum + 1);
        texGenBlock->setTexMtx(texGenNum, newTexMtx);
        J3DTevBlock* tevBlock = material->getTevBlock();
        u8 tevStageNum = tevBlock->getTevStageNum();
        JUT_ASSERT(299, tevStageNum < 4);
        l_tevOrderInfo.mTexCoord = texGenNum;
        JUT_ASSERT(301, tevBlock->getTexNo(3) == 0xffff);
        tevBlock->setTexNo(3, textureNum);
        tevBlock->setTevOrder(tevStageNum, J3DTevOrder(l_tevOrderInfo));
        tevBlock->setTevStage(tevStageNum, J3DTevStage(l_tevStageInfo));
        tevBlock->setTevStageNum(tevStageNum + 1);

        J3DShape* shape = material->getShape();
        GXAttr attr = (GXAttr)(texGenNum + GX_VA_TEX0MTXIDX);
        shape->addTexMtxIndexInDL(attr, 0);
        shape->addTexMtxIndexInVcd(attr);

        J3DPEBlock* peBlock = material->getPEBlock();
        J3DAlphaComp* alphaComp = peBlock->getAlphaComp();
        alphaComp->setAlphaCompInfo(l_alphaCompInfo);
        peBlock->setZCompLoc((u8)0);
    }
}

void dRes_info_c::onWarpMaterial(J3DModelData* i_modelData) {
    for (u16 i = 0; i < i_modelData->getMaterialNum(); i++) {
        J3DMaterial* material = i_modelData->getMaterialNodePointer(i);
        J3DTevBlock* tevBlock = material->getTevBlock();
        u8 tevStageNum = tevBlock->getTevStageNum();
        J3DTevOrder* tevorder = tevBlock->getTevOrder(tevStageNum - 1);
        if (tevorder->getTexMap() == 3) {
            break;
        }

        tevBlock->setTevStageNum(tevStageNum + 1);
        J3DTexGenBlock* texGenBlock = material->getTexGenBlock();
        texGenBlock->setTexGenNum(texGenBlock->getTexGenNum() + 1);
    }
}

void dRes_info_c::offWarpMaterial(J3DModelData* i_modelData) {
    for (u16 i = 0; i < i_modelData->getMaterialNum(); i++) {
        J3DMaterial* material = i_modelData->getMaterialNodePointer(i);
        J3DTevBlock* tevBlock = material->getTevBlock();
        u8 tevStageNum = tevBlock->getTevStageNum();
        J3DTevOrder* tevorder = tevBlock->getTevOrder(tevStageNum - 1);
        if (tevorder->getTexMap() != 3) {
            break;
        }
        tevBlock->setTevStageNum(tevStageNum - 1);
        J3DTexGenBlock* texGenBlock = material->getTexGenBlock();
        texGenBlock->setTexGenNum(texGenBlock->getTexGenNum() - 1);
    }
}

void dRes_info_c::setWarpSRT(J3DModelData* i_modelData, const cXyz& i_pos, f32 i_transX,
                             f32 i_transY) {
    J3DMaterial* material = i_modelData->getMaterialNodePointer(0);
    J3DTexGenBlock* texGenBlock = material->getTexGenBlock();
    u32 texGenNum = texGenBlock->getTexGenNum();
    J3DTexMtxInfo& texMtxInfo = texGenBlock->getTexMtx(texGenNum - 1)->getTexMtxInfo();
    texMtxInfo.mSRT.mTranslationX = i_transX;
    texMtxInfo.mSRT.mTranslationY = i_transY;

    mDoMtx_stack_c::transS(-i_pos.x, -i_pos.y, -i_pos.z);
    camera_process_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    mDoMtx_stack_c::YrotM(fopCamM_GetAngleY(camera));
    cMtx_concat(l_texMtxInfo.mEffectMtx, mDoMtx_stack_c::get(), texMtxInfo.mEffectMtx);
}

J3DModelData* dRes_info_c::loaderBasicBmd(u32 i_tag, void* i_data) {
    u32 flags = 0x59020010;
    u16 i;
    J3DMaterialAnm* materialAnm;

    if (i_tag == 'BMDE' || i_tag == 'BMDV') {
        flags |= 0x20;
    } else if (i_tag == 'BMWR' || i_tag == 'BMWE') {
        flags ^= 0x60020;
    }

    i_data = J3DModelLoaderDataBase::load(i_data, flags);
    if (i_data == NULL) {
        return NULL;
    }

    J3DModelData* modelData = (J3DModelData*)i_data;

    if (i_tag == 'BMDE' || i_tag == 'BMDV' || i_tag == 'BMWE') {
        for (i = 0; i < modelData->getShapeNum(); i++) {
            J3DShape* shape = modelData->getShapeNodePointer(i);
            shape->setTexMtxLoadType(0x2000);
        }
    }

    for (i = 0; i < modelData->getMaterialNum(); i++) {
        J3DMaterial* material = modelData->getMaterialNodePointer(i);
        u8 lightMask = material->getColorChan(0)->getLightMask();
        switch (g_env_light.light_mask_type) {
        case 1:
            lightMask &= 0x4;
            break;
        case 2:
            lightMask &= 0xC;
            break;
        case 3:
            lightMask &= 0xD;
            break;
        case 4:
            lightMask &= 0xF;
            break;
        case 5:
            lightMask &= 0x1F;
            break;
        case 6:
            lightMask &= 0x3F;
            break;
        case 7:
            lightMask &= 0x7F;
        }

        material->getColorChan(0)->setLightMask(lightMask);
        material->change();

        materialAnm = JKR_NEW J3DMaterialAnm();
        if (materialAnm == NULL) {
            return NULL;
        }

        material->setMaterialAnm(materialAnm);
    }

    setIndirectTex(modelData);

    if (i_tag == 'BMWR' || i_tag == 'BMWE') {
        addWarpMaterial(modelData);
    }

    if (i_tag == 'BMDR' || i_tag == 'BMWR') {
        s32 result = modelData->newSharedDisplayList(J3DMdlFlag_UseSingleDL);
        if (result != kJ3DError_Success) {
            return NULL;
        } else {
            modelData->simpleCalcMaterial(const_cast<MtxP>(j3dDefaultMtx));
            modelData->makeSharedDL();
        }
    }

    return (J3DModelData*)i_data;
}

int dRes_info_c::loadResource() {
    JUT_ASSERT(709, mRes == NULL);

    s32 countFile = mArchive->countFile();
    mRes = JKR_NEW_ARRAY(void*, countFile);
    if (mRes == NULL) {
        OSReport_Error("<%s.arc> setRes: res pointer buffer nothing !!\n", mArchiveName);
        return -1;
    }

    for (int i = 0; i < countFile; i++) {
        mRes[i] = NULL;
    }
    JKRArchive::SDIDirEntry* node = mArchive->mNodes;

    for (int i = 0; i < mArchive->countDirectory(); i++) {
        u32 nodeType = node->type;
        u32 fileIndex = node->first_file_index;

        for (int j = 0; j < node->num_entries; j++) {
            if (mArchive->isFileEntry(fileIndex)) {
#if DEBUG
                const char* tmp = mArchive->mStringTable + (mArchive->findIdxResource(fileIndex)->type_flags_and_name_offset & 0xFFFFFF);
#endif
                void* res = mArchive->getIdxResource(fileIndex);
#if TARGET_PC
                u32 size = mArchive->findIdxResource(fileIndex)->data_size;
                std::string fileName = mArchive->mStringTable +
                        (mArchive->findIdxResource(fileIndex)->type_flags_and_name_offset & 0xFFFFFF);
                DuskLog.debug("Loading Resource: {} (Size: {})", fileName, size);
#endif

                if (res == NULL) {
                    OSReport_Error("<%s> res == NULL !!\n",
                        mArchive->mStringTable +
                        (mArchive->findIdxResource(fileIndex)->type_flags_and_name_offset & 0xFFFFFF));
                } else if (nodeType == 'ARC ') {
                    JKRArchive::SDIFileEntry* entry = mArchive->findIdxResource(fileIndex);
                    JUT_ASSERT(765, entry != NULL);

                    const char* name_p = mArchive->mStringTable + (entry->type_flags_and_name_offset & 0xFFFFFF);
                    size_t resNameLen = strlen(name_p) - 4;
#ifdef __MWERKS__
                    JUT_ASSERT(769, resNameLen <= NAME_MAX);
#endif

                    char arcName[10];
                    strncpy(arcName, name_p, resNameLen);
                    arcName[resNameLen] = '\0';

                    JKRExpHeap* parentHeap = (JKRExpHeap*)JKRHeap::findFromRoot(JKRHeap::getCurrentHeap());
                    JUT_ASSERT(776, parentHeap != NULL && (parentHeap == mDoExt_getGameHeap() || parentHeap == mDoExt_getArchiveHeap()));

                    // ">>>>>>>>>>>>>>>>>> Pack Archive<%s> <%s>\n"
                    OS_REPORT(">>>>>>>>>>>>>>>>>> パックアーカイブ<%s> <%s>\n", arcName, parentHeap == mDoExt_getGameHeap() ? "GameHeap" : "ArchiveHeap");

                    if (parentHeap == (JKRExpHeap*)mDoExt_getGameHeap()) {
                        parentHeap = NULL;
                    }

                    int rt = dComIfG_setObjectRes(arcName, res, entry->data_size, parentHeap);
                    JUT_ASSERT(788, rt);
                } else if (nodeType == 'BMDP') {
#if DEBUG
                    g_kankyoHIO.navy.field_0x22a |= u16(0x100);
#endif
                    res = (J3DModelData*)J3DModelLoaderDataBase::load(res, 0x59020030);
                    if (res == NULL) {
                        return -1;
                    }

                    J3DModelData* modelData = (J3DModelData*)res;
                    for (u16 k = 0; k < modelData->getMaterialNum(); k++) {
                        J3DMaterial* material_p = modelData->getMaterialNodePointer(k);
                        material_p->change();

                        J3DMaterialAnm* materialAnm = JKR_NEW J3DMaterialAnm();
                        if (materialAnm == NULL) {
                            return -1;
                        }

                        material_p->setMaterialAnm(materialAnm);
                    }

                    setAlpha((J3DModelData*)res);
                    s32 result = modelData->newSharedDisplayList(J3DMdlFlag_UseSingleDL);
                    if (result != kJ3DError_Success) {
                        return -1;
                    }

                    modelData->simpleCalcMaterial(const_cast<MtxP>(j3dDefaultMtx));
                    modelData->makeSharedDL();
                } else if (nodeType == 'BMDR' || nodeType == 'BMDV' || nodeType == 'BMDE' ||
                           nodeType == 'BMWR' || nodeType == 'BMWE')
                {
                    res = loaderBasicBmd(nodeType, res);
                    if (res == NULL) {
                        return -1;
                    }
                } else if (nodeType == 'BMDG') {
                    res = (J3DModelData*)J3DModelLoaderDataBase::load(res, 0x59020010);
                    if (res == NULL) {
                        return -1;
                    }

                    J3DModelData* modelData = (J3DModelData*)res;
#if DEBUG
                    J3DMaterial* materialp = modelData->getMaterialNodePointer(0);
                    if (!materialp->isDrawModeOpaTexEdge()) {
                        // "BMDG:Translucent model can't be drawn!!\n"
                        OSReport_Error("BMDG:半透明モデルは描画できません！！\n");
                        return -1;
                    }
#endif
                    s32 result = modelData->newSharedDisplayList(J3DMdlFlag_UseSingleDL);
                    if (result != kJ3DError_Success) {
                        return -1;
                    }

                    modelData->simpleCalcMaterial(const_cast<MtxP>(j3dDefaultMtx));
                    modelData->makeSharedDL();
                } else if (nodeType == 'BMDA') {
#if DEBUG
                    g_kankyoHIO.navy.field_0x22a |= u16(0x800);
#endif
                    res = (J3DModelData*)J3DModelLoaderDataBase::load(res, 0x59020010);
                    if (res == NULL) {
                        return -1;
                    }

                    J3DModelData* modelData = (J3DModelData*)res;

                    s32 result = modelData->newSharedDisplayList(J3DMdlFlag_UseSingleDL);
                    if (result != kJ3DError_Success) {
                        return -1;
                    }

                    modelData->simpleCalcMaterial(const_cast<MtxP>(j3dDefaultMtx));
                    modelData->makeSharedDL();
#if DEBUG
                } else if (nodeType == 'BMDL') {
                    J3DModelFileData* fileData = (J3DModelFileData*)res;
                    if (fileData->mMagic2 == 'bmd3') {
                        res = J3DModelLoaderDataBase::load(res, 0x29020030);
                        if (res) {
                            J3DModelData* modelData = (J3DModelData*)res;
                            int local_8c = modelData->newSharedDisplayList(fileData->field_0x1c & 0x80000000 ? 0 : 0x40000);
                            if (local_8c) {
                                return -1;
                            }
                            modelData->simpleCalcMaterial(const_cast<MtxP>(j3dDefaultMtx));
                            modelData->makeSharedDL();
                            modelData->makeSharedDL();
                        }
                    } else {
                        res = J3DModelLoaderDataBase::loadBinaryDisplayList(res, 0x1010);
                        if (res) {
                            J3DModelData* modelData = (J3DModelData*)res;
                            for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
                                J3DMaterial* material = modelData->getMaterialNodePointer(i);
                                material->onInvalid();
                            }
                        }
                    }
                    if (!res) {
                        return -1;
                    }
#endif
                } else if (nodeType == 'BLS ') {
                    res = J3DClusterLoaderDataBase::load(res);
                    if (res == NULL) {
                        return -1;
                    }
                } else if (nodeType == 'BCKS' || nodeType == 'BCK ') {
                    struct J3DUnkChunk {
                        u8 unk_data[0x1C];
                        BE(u32) some_data_offset;
                    };
                    J3DUnkChunk* chunk = (J3DUnkChunk*)res;
                    void* bas;

                    if (chunk->some_data_offset != 0xFFFFFFFF) {
                        bas = (void*)(chunk->some_data_offset + (uintptr_t)res);
                    } else {
                        bas = NULL;
                    }

                    mDoExt_transAnmBas* transAnmBas = JKR_NEW mDoExt_transAnmBas(bas);
                    if (transAnmBas == NULL) {
                        return -1;
                    }

                    J3DAnmLoaderDataBase::setResource(transAnmBas, res);
                    res = transAnmBas;
                } else if (nodeType == 'BTP ' || nodeType == 'BTK ' || nodeType == 'BPK ' ||
                           nodeType == 'BRK ' || nodeType == 'BLK ' || nodeType == 'BVA ' ||
                           nodeType == 'BXA ')
                {
                    res = J3DAnmLoaderDataBase::load(res);
                    if (res == NULL) {
                        return -1;
                    }
                } else if (nodeType == 'DZB ') {
                    res = cBgS::ConvDzb(res);
                } else if (nodeType == 'KCL ') {
                    res = dBgWKCol::initKCollision(res);
                }

                JUT_ASSERT(1092, fileIndex < countFile);
                mRes[fileIndex] = res;
            }
            fileIndex++;
        }
        node++;
    }

    return 0;
}

void dRes_info_c::deleteArchiveRes() {
    JUT_ASSERT(1118, mArchive != NULL);

    JKRArchive::SDIDirEntry* nodes = mArchive->mNodes;
    for (int i = 0; i < mArchive->countDirectory(); i++) {
        u32 type = nodes->type;
        if (type == 'ARC ') {
            u32 fileIndex = nodes->first_file_index;
            for (int j = 0; j < nodes->num_entries; j++) {
                if (mArchive->isFileEntry(fileIndex)) {
                    const char* fileName = mArchive->mStringTable + (mArchive->findIdxResource(fileIndex)->type_flags_and_name_offset & 0xFFFFFF);
                    size_t resNameLen = strlen(fileName) - 4;
#ifdef __MWERKS__
                    JUT_ASSERT(1132, resNameLen <= NAME_MAX);
#endif

                    char nameBuffer[12];
                    strncpy(nameBuffer, fileName, resNameLen);
                    nameBuffer[resNameLen] = '\0';

                    int rt = dComIfG_deleteObjectResMain(nameBuffer);
                    JUT_ASSERT(1136, rt);
                }
                fileIndex++;
            }
        }
        nodes++;
    }
}

static SArcHeader* getArcHeader(JKRArchive* i_archive) {
    if (i_archive != NULL) {
        switch (i_archive->mMountMode) {
        case JKRArchive::MOUNT_MEM:
            JKRMemArchive* memArchive = (JKRMemArchive*)i_archive;
            return memArchive->mArcHeader;
        }
    }

    return NULL;
}

int dRes_info_c::setRes(JKRArchive* i_archive, JKRHeap* i_heap) {
    JUT_ASSERT(1197, mArchive == NULL);
    mArchive = i_archive;
    heap = i_heap;
    mDataHeap = NULL;

    int rt = loadResource();
    JUT_ASSERT(1204, rt >= 0);
    return rt >> 0x1F;
}

bool data_8074C6C0_debug;

int dRes_info_c::setRes() {
    if (mArchive == NULL) {
        if (mDMCommand == NULL) {
            return -1;
        }
        if (mDMCommand->sync() == 0) {
            return 1;
        }

        mArchive = mDMCommand->getArchive();
        heap = mDMCommand->getHeap();

        mDMCommand->destroy();
        mDMCommand = NULL;

        if (mArchive == NULL) {
            OSReport_Error("<%s.arc> setRes: archive mount error !!\n", mArchiveName);
            return -1;
        }

        u32 r28;

        if (heap != NULL) {
            heap->lock();
            mDataHeap = mDoExt_createSolidHeapToCurrent(0, heap, 0x20);
            JUT_ASSERT(1260, mDataHeap != NULL);
            JKRHEAP_NAMEF(mDataHeap, "Resource (%s)", mArchiveName);

            int rt = loadResource();
            mDoExt_restoreCurrentHeap();
            r28 = mDoExt_adjustSolidHeap(mDataHeap);
            heap->unlock();
#if DEBUG
            JUT_ASSERT(1270, rt >= 0);
#else
            if (rt < 0) {
                return -1;
            }
#endif
        } else {
            mDataHeap = mDoExt_createSolidHeapFromGameToCurrent(0, 0);
            if (mDataHeap == NULL) {
                OSReport_Error("<%s.arc> mDMCommandsetRes: can't alloc memory\n", mArchiveName);
                return -1;
            }
            JKRHEAP_NAMEF(mDataHeap, "Resource (%s)", mArchiveName);
            int rt = loadResource();
            mDoExt_restoreCurrentHeap();
            r28 = mDoExt_adjustSolidHeap(mDataHeap);

#if DEBUG
            JUT_ASSERT(1289, rt >= 0);
#else
            if (rt < 0) {
                return -1;
            }
#endif
        }

#if DEBUG
        mSize = JKRGetRootHeap()->getSize(((JKRMemArchive*)mArchive)->mArcHeader) + JKRGetMemBlockSize(NULL, mDataHeap);
        if (data_8074C6C0_debug) {
            JKRExpHeap* zeldaHeap = mDoExt_getZeldaHeap();
            OSReport("\e[33mdRes_info_c::setRes <使用=%08x(work:%08x) 連続空き=%08x 残り空き=%08x (%3d) %s.arc\n\e[m", mSize, r28, zeldaHeap->getFreeSize(), zeldaHeap->getTotalFreeSize(), getResNum(), this);
            OSReport("\e[33mSolid=%08x-%08x StartAddr=%08x EndAddr=%08x HeapSize=%08x \n\e[m", mDataHeap, uintptr_t(mDataHeap) + mDataHeap->getHeapSize(), mDataHeap->getStartAddr(), mDataHeap->getEndAddr(), mDataHeap->getHeapSize());
        }
#endif

        u32 heapSize = mDataHeap->getHeapSize();
        DCStoreRangeNoSync(mDataHeap->getStartAddr(), heapSize);

#if TARGET_PC
        char buf[32];
        snprintf(buf, sizeof(buf), "Arc %s", mArchiveName);
        JKRHEAP_NAME(mDataHeap, buf);
#endif
    }

    return 0;
}

static s32 myGetMemBlockSize(void* i_data) {
    JKRHeap* heap = JKRFindHeap(i_data);
    u32 heapType = heap->getHeapType();
    s32 size = heapType == 'EXPH' ? JKRGetMemBlockSize(heap, i_data) : -1;
    return size;
}

static s32 myGetMemBlockSize0(void* i_data) {
    s32 size = myGetMemBlockSize(i_data);
    if (size < 0) {
        size = 0;
    }
    return size;
}

// Fixes string data and float literal order
f32 dummy(int x) {
    DEAD_STRING("%5.1f %5x %5.1f %5x %3d %s\n");
    return x;
}

void dRes_info_c::dump_long(dRes_info_c* i_resInfo, int i_infoNum) {
    JUTReportConsole_f("dRes_info_c::dump_long %08x %d\n", i_resInfo, i_infoNum);
    JUTReportConsole_f("No Command Archive  ArcHeader(size) SolidHeap(size) Resource Cnt ArchiveName\n");

    for (int i = 0; i < i_infoNum; i++) {
        if (i_resInfo->getCount() != 0) {
            JKRArchive* archive = i_resInfo->getArchive();
            void* header = NULL;
            int blockSize1 = 0;

            if (archive != NULL) {
                header = getArcHeader(archive);
                blockSize1 = myGetMemBlockSize0(header);
            }

            JKRSolidHeap* dataHeap = i_resInfo->mDataHeap;
            int blockSize2 = 0;
            if (dataHeap != NULL) {
                blockSize2 = myGetMemBlockSize0((void*)dataHeap);
            }

            JUTReportConsole_f("%2d %08x %08x %08x(%6x) %08x(%5x) %08x %3d %s\n",
                               i,
                               i_resInfo->getDMCommand(),
                               archive,
                               header,
                               blockSize1,
                               dataHeap,
                               blockSize2,
                               i_resInfo->mRes,
                               i_resInfo->getCount(),
                               i_resInfo->getArchiveName());
        }
        i_resInfo++;
    }
}

void dRes_info_c::dump(dRes_info_c* i_resInfo, int i_infoNum) {
    int totalArcHeaderSize;
    int totalHeapSize;
    int arcHeaderSize;
    int heapSize;
    JUTReportConsole_f("dRes_info_c::dump %08x %d\n", i_resInfo, i_infoNum);
    JUTReportConsole_f("No ArchiveSize(KB) SolidHeapSize(KB) Cnt ArchiveName\n");

    totalArcHeaderSize = 0;
    totalHeapSize = 0;

    for (int i = 0; i < i_infoNum; i++) {
        if (i_resInfo->getCount() != 0) {
            arcHeaderSize = JKRGetMemBlockSize(NULL, getArcHeader(i_resInfo->getArchive()));
            heapSize = JKRGetMemBlockSize(NULL, i_resInfo->mDataHeap);
            JUTReportConsole_f("%2d %6.1f %6x %6.1f %6x %3d %s\n",
                               i,
                               arcHeaderSize / 1024.0f,
                               arcHeaderSize,
                               heapSize / 1024.0f,
                               heapSize,
                               i_resInfo->getCount(),
                               i_resInfo->getArchiveName());
            totalArcHeaderSize += arcHeaderSize;
            totalHeapSize += heapSize;
        }
        i_resInfo++;
    }

    JUTReportConsole_f("----------------------------------------------\n   %6.1f %6x %6.1f %6x   Total\n\n",
                       totalArcHeaderSize / 1024.0f,
                       totalArcHeaderSize,
                       totalHeapSize / 1024.0f,
                       totalHeapSize);
}

#if DEBUG
void dRes_info_c::dumpTag(dRes_info_c* info, int param_2, int param_3, int param_4) {
    for (int i = 0; i < param_2; i++) {
        if (info->getCount()) {
            fapGm_dataMem::printfTag(1, param_3, 0, info->getArchiveName(), getArcHeader(info->getArchive()), 0, NULL, NULL);
            fapGm_dataMem::printfTag(1, param_4, 0, info->getArchiveName(), info->mDataHeap, 0, NULL, NULL);
        }
        info++;
    }
}

void dRes_info_c::dump(char* param_1, dRes_info_c* info, int param_3) {
    for (int i = 0; i < param_3; i++) {
        if (info->getCount()) {
            char* r28 = param_1 + strlen(param_1);
            sprintf(r28, ",%s,%d,\n", info->getArchiveName(), JKRGetMemBlockSize(NULL, getArcHeader(info->getArchive())) + JKRGetMemBlockSize(NULL, info->mDataHeap));
        }
        info++;
    }
}
#endif

dRes_control_c::~dRes_control_c() {
    for (int i = 0; i < ARRAY_SIZE(mObjectInfo); i++) {
        mObjectInfo[i].~dRes_info_c();
    }

    for (int i = 0; i < ARRAY_SIZE(mStageInfo); i++) {
        mStageInfo[i].~dRes_info_c();
    }
}

int dRes_control_c::setRes(char const* i_arcName, dRes_info_c* i_resInfo, int i_infoNum,
                           char const* i_path, u8 i_mountDirection, JKRHeap* i_heap) {
    dRes_info_c* resInfo = getResInfo(i_arcName, i_resInfo, i_infoNum);

    if (resInfo == NULL) {
        resInfo = newResInfo(i_resInfo, i_infoNum);

        if (resInfo == NULL) {
            // "<%s.arc> dRes_control_c::setRes: There isn't a free Resource Info pointer\n"
            OSReport_Error("<%s.arc> dRes_control_c::setRes: 空きリソース情報ポインタがありません\n", i_arcName);
            resInfo->~dRes_info_c();
            return 0;
        }

        if (resInfo->set(i_arcName, i_path, i_mountDirection, i_heap) == 0) {
            OSReport_Error("<%s.arc> dRes_control_c::setRes: res info set error !!\n", i_arcName);
            resInfo->~dRes_info_c();
            return 0;
        }
    }

    resInfo->incCount();
    return 1;
}

int dRes_control_c::syncRes(char const* i_arcName, dRes_info_c* i_resInfo, int i_infoNum) {
    dRes_info_c* resInfo = getResInfo(i_arcName, i_resInfo, i_infoNum);

    if (resInfo == NULL) {
#if DEBUG
        if (i_arcName[0] == 'R' ||
            (i_arcName[0] == 'S' && i_arcName[1] == 't' && i_arcName[2] == 'g' && i_arcName[3] == '_' && i_arcName[4] == '0' && i_arcName[5] == '0') ||
            strncmp(i_arcName, "Pack", 4) == 0)
        {
            // "<%s.arc> syncRes: Resource not registered (No Error)\n"
            OS_REPORT("\e[34m<%s.arc> syncRes: リソース未登録(問題無し)\n\e[m", i_arcName);
        } else {
            // "<%s.arc> syncRes: Resource not registered!!\n"
            OS_REPORT_ERROR("<%s.arc> syncRes: リソース未登録!!\n", i_arcName);
        }
#endif
        return -1;
    } else {
        return resInfo->setRes();
    }
}

int dRes_control_c::deleteRes(char const* i_arcName, dRes_info_c* i_resInfo, int i_infoNum) {
    dRes_info_c* resInfo = getResInfo(i_arcName, i_resInfo, i_infoNum);

    if (resInfo == NULL) {
#if DEBUG
    if (strcmp(i_arcName, "Xtg_00")) {
        // "<%s.arc> deleteRes: res nothing !!\n(Detected deleting an unregistered resource! Please fix.)\n"
        OS_REPORT_ERROR("<%s.arc> deleteRes: res nothing !!\n(未登録のリソースを削除してるのを発見しました！修正してください。)\n", i_arcName);
    }
#endif
        return 0;
    }

    if (resInfo->decCount() == 0) {
        resInfo->~dRes_info_c();
    }
    return 1;
}

dRes_info_c* dRes_control_c::getResInfo(char const* i_arcName, dRes_info_c* i_resInfo, int i_infoNum) {
    for (int i = 0; i < i_infoNum; i++) {
        if (i_resInfo->getCount() != 0) {
            if (!stricmp(i_arcName, i_resInfo->getArchiveName())) {
                return i_resInfo;
            }
        }
        i_resInfo++;
    }

    return NULL;
}

dRes_info_c* dRes_control_c::newResInfo(dRes_info_c* i_resInfo, int i_infoNum) {
    for (int i = 0; i < i_infoNum; i++) {
        if (i_resInfo->getCount() == 0) {
#if TARGET_PC
            // The original code reuses explicitly-destructed dRes_info_c slots without
            // placement-new. The destructor's cleanup writes (mArchive = NULL, etc.) are
            // dead stores from the compiler's perspective and may be optimized away,
            // leaving stale pointers. Reconstruct the slot to ensure a clean state.
            new (i_resInfo) dRes_info_c();
#endif
            return i_resInfo;
        }
        i_resInfo++;
    }

    return NULL;
}

dRes_info_c* dRes_control_c::getResInfoLoaded(char const* i_arcName, dRes_info_c* i_resInfo,
                                              int i_infoNum) {
    dRes_info_c* resInfo = getResInfo(i_arcName, i_resInfo, i_infoNum);

    if (resInfo == NULL) {
#if DEBUG
    if (stricmp(i_arcName, "Xtg_00")) {
        OS_REPORT("\e[35m<%s.arc> getRes: res nothing !!\n\e[m", i_arcName);
    }
#endif
        return NULL;
    } else if (resInfo->getArchive() == NULL) {
        OSReport_Warning("<%s.arc> getRes: res during reading !!\n", i_arcName);
        return NULL;
    }

    return resInfo;
}

void* dRes_control_c::getRes(char const* i_arcName, s32 i_index, dRes_info_c* i_resInfo, int i_infoNum) {
    dRes_info_c* resInfo = getResInfoLoaded(i_arcName, i_resInfo, i_infoNum);
    if (resInfo == NULL) {
        return resInfo;
    }

    JKRArchive* archive = resInfo->getArchive();
    int count = archive->countFile();

    if (i_index >= count) {
        OSReport_Error("<%s.arc> getRes: res index over !! index=%d count=%d\n", i_arcName, i_index,
                       count);
        return NULL;
    }

    return resInfo->getRes(i_index);
}

void* dRes_control_c::getRes(char const* i_arcName, char const* i_resName, dRes_info_c* i_resInfo,
                             int i_infoNum) {
    dRes_info_c* resInfo = getResInfoLoaded(i_arcName, i_resInfo, i_infoNum);
    if (resInfo == NULL) {
        return resInfo;
    }

    JKRArchive* archive = resInfo->getArchive();
    JKRArchive::SDIFileEntry* entry = archive->findNameResource(i_resName);

    if (entry != NULL) {
        return resInfo->getRes(entry - archive->mFiles);
    } else {
        OS_REPORT("\e[34m%s not found in %s.arc\n\e[m", i_resName, i_arcName);
        return NULL;
    }
}

void* dRes_control_c::getIDRes(char const* i_arcName, u16 i_resID, dRes_info_c* i_resInfo, int i_infoNum) {
    dRes_info_c* resInfo = getResInfoLoaded(i_arcName, i_resInfo, i_infoNum);
    if (resInfo == NULL) {
        return resInfo;
    }

    JKRArchive* archive = resInfo->getArchive();
    int index = mDoExt_resIDToIndex(archive, i_resID);
    if (index < 0) {
        return 0;
    }

    return resInfo->getRes(index);
}

int dRes_control_c::syncAllRes(dRes_info_c* i_resInfo, int i_infoNum) {
    for (int i = 0; i < i_infoNum; i++) {
        if (i_resInfo->getDMCommand() != NULL && i_resInfo->setRes() > 0) {
            return 1;
        }
        i_resInfo++;
    }

    return 0;
}

#if DEBUG
int dRes_control_c::getSize(const char* i_arcName, dRes_info_c* i_resInfo, int i_infoNum) {
    dRes_info_c* info = getResInfoLoaded(i_arcName, i_resInfo, i_infoNum);
    if (!info) {
        return 0;
    }
    return info->getSize();
}

int dRes_control_c::getStageAllSize() {
    int size = 0;
    dRes_info_c* info = mStageInfo;

    for (int i = 0; i < ARRAY_SIZE(mStageInfo); i++) {
        if (info->getCount()) {
            size += info->getSize();
        }
        info++;
    }

    return size;
}

int dRes_control_c::getObjectAllSize() {
    int size = 0;
    dRes_info_c* info = mObjectInfo;

    for (int i = 0; i < ARRAY_SIZE(mObjectInfo); i++) {
        if (info->getCount()) {
            size += info->getSize();
        }
        info++;
    }

    return size;
}
#endif

int dRes_control_c::setObjectRes(char const* i_arcName, void* i_archiveRes, u32 i_bufferSize,
                                 JKRHeap* i_heap) {
    JUT_ASSERT(1955, i_archiveRes != NULL);

#if DEBUG
    dRes_info_c* nowInfo = getResInfo(i_arcName, mObjectInfo, ARRAY_SIZEU(mObjectInfo));
    JUT_ASSERT(1958, nowInfo == NULL);
#endif

    int r26 = setRes(i_arcName, mObjectInfo, ARRAY_SIZEU(mObjectInfo), "", mDoDvd_MOUNT_DIRECTION_HEAD, NULL);
    if (!r26) {
        return 0;
    }

    JKRMemArchive* memArchive = JKR_NEW JKRMemArchive(i_archiveRes, i_bufferSize, JKRMEMBREAK_FLAG_UNKNOWN0);
    if (memArchive == NULL || !memArchive->isMounted()) {
        return 0;
    }

    dRes_info_c* info = getResInfo(i_arcName, mObjectInfo, ARRAY_SIZEU(mObjectInfo));
    JUT_ASSERT(1975, info != NULL);

    if (info->setRes(memArchive, i_heap)) {
        return 0;
    }

    return 1;
}

int dRes_control_c::setStageRes(char const* i_arcName, JKRHeap* i_heap) {
    char path[20];

    snprintf(path, sizeof(path), "/res/Stage/%s/", dComIfGp_getStartStageName());
    return setRes(i_arcName, mStageInfo, ARRAY_SIZEU(mStageInfo), path, mDoDvd_MOUNT_DIRECTION_TAIL, i_heap);
}

void dRes_control_c::dump() {
    JUTReportConsole_f("\ndRes_control_c::dump mObjectInfo\n");
    dRes_info_c::dump(mObjectInfo, ARRAY_SIZEU(mObjectInfo));
    dRes_info_c::dump_long(mObjectInfo, ARRAY_SIZEU(mObjectInfo));

    JUTReportConsole_f("\ndRes_control_c::dump mStageInfo\n");
    dRes_info_c::dump(mStageInfo, ARRAY_SIZEU(mStageInfo));
    dRes_info_c::dump_long(mStageInfo, ARRAY_SIZEU(mStageInfo));
}

#if DEBUG
void dRes_control_c::dumpTag() {
    dRes_info_c::dumpTag(mObjectInfo, ARRAY_SIZE(mObjectInfo), 7, 8);
    dRes_info_c::dumpTag(mStageInfo, ARRAY_SIZE(mStageInfo), 9, 10);
}

void dRes_control_c::dump(char* param_1) {
    sprintf(param_1 + strlen(param_1), ",アーカイブ名,サイズ,\n");
    dRes_info_c::dump(param_1, mObjectInfo, ARRAY_SIZE(mObjectInfo));
    dRes_info_c::dump(param_1, mStageInfo, ARRAY_SIZE(mStageInfo));
    sprintf(param_1 + strlen(param_1), ",パーティクルリソース（シーン依存）,%d,\n\n", JKRGetRootHeap()->getSize(g_dComIfG_gameInfo.play.getParticle()->getSceneRes()));
}
#endif

int dRes_control_c::getObjectResName2Index(char const* i_arcName, char const* i_resName) {
    dRes_info_c* info = getResInfoLoaded(i_arcName, mObjectInfo, ARRAY_SIZEU(mObjectInfo));

    if (info == NULL) {
        return -1;
    } else if (i_resName == NULL) {
        return -1;
    }

    JKRArchive* archive = info->getArchive();
    JKRArchive::SDIFileEntry* entry = archive->findNameResource(i_resName);
    if (entry != NULL) {
        return entry->file_id;
    }

    return -1;
}
