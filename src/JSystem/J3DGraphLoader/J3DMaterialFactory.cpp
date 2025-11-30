#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphLoader/J3DMaterialFactory.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JSupport/JSupport.h"
#include "JSystem/JUtility/JUTAssert.h"

J3DMaterialFactory::J3DMaterialFactory(J3DMaterialBlock const& i_block) {
    mMaterialNum = i_block.mMaterialNum;
    mpMaterialInitData = JSUConvertOffsetToPtr<J3DMaterialInitData>(&i_block, i_block.mpMaterialInitData);
    mpMaterialID = JSUConvertOffsetToPtr<u16>(&i_block, i_block.mpMaterialID);
    if (i_block.mpIndInitData != NULL && (uintptr_t)i_block.mpIndInitData - (uintptr_t)i_block.mpNameTable > 4) {
        mpIndInitData = JSUConvertOffsetToPtr<J3DIndInitData>(&i_block, i_block.mpIndInitData);
    } else {
        mpIndInitData = NULL;
    }
    mpCullMode = JSUConvertOffsetToPtr<GXCullMode>(&i_block, i_block.mpCullMode);
    mpMatColor = JSUConvertOffsetToPtr<GXColor>(&i_block, i_block.mpMatColor);
    mpColorChanNum = JSUConvertOffsetToPtr<u8>(&i_block, i_block.mpColorChanNum);
    mpColorChanInfo = JSUConvertOffsetToPtr<J3DColorChanInfo>(&i_block, i_block.mpColorChanInfo);
    mpAmbColor = JSUConvertOffsetToPtr<GXColor>(&i_block, i_block.mpAmbColor);
    mpLightInfo = JSUConvertOffsetToPtr<J3DLightInfo>(&i_block, i_block.mpLightInfo);
    mpTexGenNum = JSUConvertOffsetToPtr<u8>(&i_block, i_block.mpTexGenNum);
    mpTexCoordInfo = JSUConvertOffsetToPtr<J3DTexCoordInfo>(&i_block, i_block.mpTexCoordInfo);
    mpTexCoord2Info = JSUConvertOffsetToPtr<J3DTexCoord2Info>(&i_block, i_block.mpTexCoord2Info);
    mpTexMtxInfo = JSUConvertOffsetToPtr<J3DTexMtxInfo>(&i_block, i_block.mpTexMtxInfo);
    field_0x34 = JSUConvertOffsetToPtr<J3DTexMtxInfo>(&i_block, i_block.field_0x44);
    mpTexNo = JSUConvertOffsetToPtr<u16>(&i_block, i_block.mpTexNo);
    mpTevOrderInfo = JSUConvertOffsetToPtr<J3DTevOrderInfo>(&i_block, i_block.mpTevOrderInfo);
    mpTevColor = JSUConvertOffsetToPtr<GXColorS10>(&i_block, i_block.mpTevColor);
    mpTevKColor = JSUConvertOffsetToPtr<GXColor>(&i_block, i_block.mpTevKColor);
    mpTevStageNum = JSUConvertOffsetToPtr<u8>(&i_block, i_block.mpTevStageNum);
    mpTevStageInfo = JSUConvertOffsetToPtr<J3DTevStageInfo>(&i_block, i_block.mpTevStageInfo);
    mpTevSwapModeInfo = JSUConvertOffsetToPtr<J3DTevSwapModeInfo>(&i_block, i_block.mpTevSwapModeInfo);
    mpTevSwapModeTableInfo = JSUConvertOffsetToPtr<J3DTevSwapModeTableInfo>(&i_block, i_block.mpTevSwapModeTableInfo);
    mpFogInfo = JSUConvertOffsetToPtr<J3DFogInfo>(&i_block, i_block.mpFogInfo);
    mpAlphaCompInfo = JSUConvertOffsetToPtr<J3DAlphaCompInfo>(&i_block, i_block.mpAlphaCompInfo);
    mpBlendInfo = JSUConvertOffsetToPtr<J3DBlendInfo>(&i_block, i_block.mpBlendInfo);
    mpZModeInfo = JSUConvertOffsetToPtr<J3DZModeInfo>(&i_block, i_block.mpZModeInfo);
    mpZCompLoc = JSUConvertOffsetToPtr<u8>(&i_block, i_block.mpZCompLoc);
    mpDither = JSUConvertOffsetToPtr<u8>(&i_block, i_block.mpDither);
    mpNBTScaleInfo = JSUConvertOffsetToPtr<J3DNBTScaleInfo>(&i_block, i_block.mpNBTScaleInfo);
    mpDisplayListInit = NULL;
    mpPatchingInfo = NULL;
    mpCurrentMtxInfo = NULL;
    mpMaterialMode = NULL;
}

J3DMaterialFactory::J3DMaterialFactory(J3DMaterialDLBlock const& i_block) {
    mMaterialNum = i_block.mMaterialNum;
    mpMaterialInitData = NULL;
    mpDisplayListInit = JSUConvertOffsetToPtr<J3DDisplayListInit>(&i_block, i_block.mpDisplayListInit);
    mpPatchingInfo = JSUConvertOffsetToPtr<J3DPatchingInfo>(&i_block, i_block.mpPatchingInfo);
    mpCurrentMtxInfo = JSUConvertOffsetToPtr<J3DCurrentMtxInfo>(&i_block, i_block.mpCurrentMtxInfo);
    mpMaterialMode = JSUConvertOffsetToPtr<u8>(&i_block, i_block.mpMaterialMode);
}

u16 J3DMaterialFactory::countUniqueMaterials() {
    u16 count = 0;
    s32 id = -1;
    for (u16 i = 0; i < mMaterialNum; i++) {
        if (id < mpMaterialID[i]) {
            count++;
            id = mpMaterialID[i];
        }
    }
    return count;
}

u32 J3DMaterialFactory::countTexGens(int i_idx) const {
    u8 tex_gen_num_index = mpMaterialInitData[mpMaterialID[i_idx]].mTexGenNumIdx;
    if (tex_gen_num_index != 0xff) {
        return mpTexGenNum[tex_gen_num_index];
    }
    return 0;
}

u32 J3DMaterialFactory::countStages(int i_idx) const {
    J3DMaterialInitData* init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    u32 count1 = 0;
    u32 count2 = 0;
    u8 tev_stage_num_index = init_data->mTevStageNumIdx;
    if (tev_stage_num_index != 0xff) {
        count2 = mpTevStageNum[tev_stage_num_index];
    }
    for (int i = 0; i < 8; i++) {
        if (init_data->mTexNoIdx[i] != 0xffff) {
            count1++;
        }
    }
    if (count2 != count1 && count1 != 0) {
        if (count2 > count1) {
            return count2;
        } else {
            return count1;
        }
    }
    return count2;
}

J3DMaterial* J3DMaterialFactory::create(J3DMaterial* i_material, MaterialType i_type,
                                        int i_idx, u32 i_flags) const {
    switch (i_type) {
        case MATERIAL_TYPE_NORMAL:
            i_material = createNormalMaterial(i_material, i_idx, i_flags);
            break;
        case MATERIAL_TYPE_LOCKED:
            i_material = createLockedMaterial(i_material, i_idx, i_flags);
            break;
        case MATERIAL_TYPE_PATCHED:
            i_material = createPatchedMaterial(i_material, i_idx, i_flags);
            break;
    }
    return i_material;
}

J3DMaterial* J3DMaterialFactory::createNormalMaterial(J3DMaterial* i_material, int i_idx,
                                                      u32 i_flags) const {
    if (mpDisplayListInit != NULL) {
        return createLockedMaterial(i_material, i_idx, i_flags);
    }

    const u32 stages = countStages(i_idx);
    u32 tev_stage_num = getMdlDataFlag_TevStageNum(i_flags);
    u32 tev_stage_num_max = JMAMax(stages, tev_stage_num);
    u32 tex_num = tev_stage_num_max > 8 ? 8 : tev_stage_num_max;
    u32 texgens = countTexGens(i_idx);
    u32 texgen_flag = texgens > 4 ? getMdlDataFlag_TexGenFlag(0) : getMdlDataFlag_TexGenFlag(i_flags);
    u32 color_flag = getMdlDataFlag_ColorFlag(i_flags);
    u32 pe_flag = getMdlDataFlag_PEFlag(i_flags);
    BOOL ind_flag = (i_flags & 0x1000000) ? TRUE : FALSE;
    if (i_material == NULL) {
        i_material = new J3DMaterial();
        J3D_ASSERT_ALLOCMEM(269, i_material);
    }
    i_material->mColorBlock = J3DMaterial::createColorBlock(color_flag);
    i_material->mTexGenBlock = J3DMaterial::createTexGenBlock(texgen_flag);
    i_material->mTevBlock = J3DMaterial::createTevBlock((u16)tev_stage_num_max);
    i_material->mIndBlock = J3DMaterial::createIndBlock(ind_flag);
    i_material->mPEBlock = J3DMaterial::createPEBlock(pe_flag, getMaterialMode(i_idx));
    i_material->mIndex = i_idx;
    i_material->mMaterialMode = getMaterialMode(i_idx);
    i_material->mColorBlock->setColorChanNum(newColorChanNum(i_idx));
    i_material->mColorBlock->setCullMode(newCullMode(i_idx));
    i_material->mTexGenBlock->setTexGenNum(newTexGenNum(i_idx));
    i_material->mTexGenBlock->setNBTScale(newNBTScale(i_idx));
    i_material->mPEBlock->setFog(newFog(i_idx));
    i_material->mPEBlock->setAlphaComp(newAlphaComp(i_idx));
    i_material->mPEBlock->setBlend(newBlend(i_idx));
    i_material->mPEBlock->setZMode(newZMode(i_idx));
    i_material->mPEBlock->setZCompLoc(newZCompLoc(i_idx));
    i_material->mPEBlock->setDither(newDither(i_idx));
    i_material->mTevBlock->setTevStageNum(newTevStageNum(i_idx));
    for (u8 i = 0; i < tex_num; i++) {
        i_material->mTevBlock->setTexNo(i, newTexNo(i_idx, i));
    }
    for (u8 i = 0; i < tev_stage_num_max; i++) {
        i_material->mTevBlock->setTevOrder(i, newTevOrder(i_idx, i));
    }
    for (u8 i = 0; i < tev_stage_num_max; i++) {
        J3DMaterialInitData* material_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
        i_material->mTevBlock->setTevStage(i, newTevStage(i_idx, i));
        if (material_init_data->mTevSwapModeIdx[i] != 0xffff) {
            i_material->mTevBlock->getTevStage(i)->setTexSel(
                mpTevSwapModeInfo[material_init_data->mTevSwapModeIdx[i]].mTexSel);
            i_material->mTevBlock->getTevStage(i)->setRasSel(
                mpTevSwapModeInfo[material_init_data->mTevSwapModeIdx[i]].mRasSel);
        }
    }
    for (u8 i = 0; i < 4; i++) {
        i_material->mTevBlock->setTevKColor(i, newTevKColor(i_idx, i));
    }
    for (u8 i = 0; i < 4; i++) {
        i_material->mTevBlock->setTevColor(i, newTevColor(i_idx, i));
    }
    for (u8 i = 0; i < 4; i++) {
        i_material->mTevBlock->setTevSwapModeTable(i, newTevSwapModeTable(i_idx, i));
    }
    for (u8 i = 0; i < 2; i++) {
        i_material->mColorBlock->setAmbColor(i, newAmbColor(i_idx, i));
    }
    for (u8 i = 0; i < 2; i++) {
        i_material->mColorBlock->setMatColor(i, newMatColor(i_idx, i));
    }
    for (u8 i = 0; i < 4; i++) {
        J3DColorChan color_chan = newColorChan(i_idx, i);
        i_material->mColorBlock->setColorChan(i, color_chan);
    }
    for (u8 i = 0; i < texgens; i++) {
        J3DTexCoord tex_coord = newTexCoord(i_idx, i);
        i_material->mTexGenBlock->setTexCoord(i, &tex_coord);
    }
    for (u8 i = 0; i < 8; i++) {
        i_material->mTexGenBlock->setTexMtx(i, newTexMtx(i_idx, i));
    }
    J3DMaterialInitData* material_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    for (u8 i = 0; i < tev_stage_num_max; i++) {
        if (material_init_data->mTevKColorSel[i] != 0xff) {
            i_material->mTevBlock->setTevKColorSel(i, material_init_data->mTevKColorSel[i]);
        } else {
            i_material->mTevBlock->setTevKColorSel(i, 0xc);
        }
    }
    for (u8 i = 0; i < tev_stage_num_max; i++) {
        if (material_init_data->mTevKAlphaSel[i] != 0xff) {
            i_material->mTevBlock->setTevKAlphaSel(i, material_init_data->mTevKAlphaSel[i]);
        } else {
            i_material->mTevBlock->setTevKAlphaSel(i, 0x1c);
        }
    }
    if (mpIndInitData != NULL) {
        u8 ind_tex_stage_num = newIndTexStageNum(i_idx);
        i_material->mIndBlock->setIndTexStageNum(newIndTexStageNum(i_idx));
        for (u8 i = 0; i < ind_tex_stage_num; i++) {
            i_material->mIndBlock->setIndTexMtx(i, newIndTexMtx(i_idx, i));
        }
        for (u8 i = 0; i < ind_tex_stage_num; i++) {
            i_material->mIndBlock->setIndTexOrder(i, newIndTexOrder(i_idx, i));
        }
        for (u8 i = 0; i < ind_tex_stage_num; i++) {
            i_material->mIndBlock->setIndTexCoordScale(i, newIndTexCoordScale(i_idx, i));
        }
        for (u8 i = 0; i < tev_stage_num_max; i++) {
            i_material->mTevBlock->setIndTevStage(i, newIndTevStage(i_idx, i));
        }
    }
    return i_material;
}

J3DMaterial* J3DMaterialFactory::createPatchedMaterial(J3DMaterial* i_material, int i_idx,
                                                       u32 i_flags) const {
    if (i_material == NULL) {
        i_material = new J3DPatchedMaterial();
        J3D_ASSERT_ALLOCMEM(430, i_material);
    }
    bool bVar1 = i_flags & 0x3000000 ? true : false;
    i_material->mColorBlock = J3DMaterial::createColorBlock(0x40000000);
    i_material->mTexGenBlock = new J3DTexGenBlockPatched();
    J3D_ASSERT_ALLOCMEM(440, i_material->mTexGenBlock);
    i_material->mTevBlock = new J3DTevBlockPatched();
    J3D_ASSERT_ALLOCMEM(442, i_material->mTevBlock);
    i_material->mIndBlock = J3DMaterial::createIndBlock(bVar1);
    i_material->mPEBlock = J3DMaterial::createPEBlock(0x10000000, getMaterialMode(i_idx));
    i_material->mIndex = i_idx;
    i_material->mMaterialMode = getMaterialMode(i_idx);
    i_material->mTevBlock->setTevStageNum(newTevStageNum(i_idx));
    i_material->mColorBlock->setColorChanNum(newColorChanNum(i_idx));
    i_material->mColorBlock->setCullMode(newCullMode(i_idx));
    i_material->mPEBlock->setFog(newFog(i_idx));
    i_material->mPEBlock->setAlphaComp(newAlphaComp(i_idx));
    i_material->mPEBlock->setBlend(newBlend(i_idx));
    i_material->mPEBlock->setZMode(newZMode(i_idx));
    i_material->mPEBlock->setZCompLoc(newZCompLoc(i_idx));
    i_material->mPEBlock->setDither(newDither(i_idx));
    u8 tev_stage_num = i_material->mTevBlock->getTevStageNum();
    for (u8 i = 0; i < 8; i++) {
        i_material->mTevBlock->setTexNo(i, newTexNo(i_idx, i));
    }
    for (u8 i = 0; i < tev_stage_num; i++) {
        i_material->mTevBlock->setTevOrder(i, newTevOrder(i_idx, i));
    }
    for (u8 i = 0; i < 4; i++) {
        i_material->mTevBlock->setTevKColor(i, newTevKColor(i_idx, i));
    }
    for (u8 i = 0; i < 4; i++) {
        i_material->mTevBlock->setTevColor(i, newTevColor(i_idx, i));
    }
    for (u8 i = 0; i < tev_stage_num; i++) {
        J3DMaterialInitData* material_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
        i_material->mTevBlock->setTevStage(i, newTevStage(i_idx, i));
        if (material_init_data->mTevSwapModeIdx[i] != 0xffff) {
            i_material->mTevBlock->getTevStage(i)->setTexSel(
                mpTevSwapModeInfo[material_init_data->mTevSwapModeIdx[i]].mTexSel);
            i_material->mTevBlock->getTevStage(i)->setRasSel(
                mpTevSwapModeInfo[material_init_data->mTevSwapModeIdx[i]].mRasSel);
        }
    }
    J3DMaterialInitData* init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    for (u8 i = 0; i < tev_stage_num; i++) {
        if (init_data->mTevKColorSel[i] != 0xff) {
            i_material->mTevBlock->setTevKColorSel(i, init_data->mTevKColorSel[i]);
        } else {
            i_material->mTevBlock->setTevKColorSel(i, 0xC);
        }
    }
    for (u8 i = 0; i < 2; i++) {
        i_material->mColorBlock->setMatColor(i, newMatColor(i_idx, i));
    }
    for (u8 i = 0; i < 4; i++) {
        J3DColorChan color_chan = newColorChan(i_idx, i);
        i_material->mColorBlock->setColorChan(i, color_chan);
    }
    u32 texgens = countTexGens(i_idx);
    i_material->mTexGenBlock->setTexGenNum(newTexGenNum(i_idx));
    for (u8 i = 0; i < 8; i++) {
        i_material->mTexGenBlock->setTexMtx(i, newTexMtx(i_idx, i));
    }
    for (u8 i = 0; i < texgens; i++) {
        J3DTexCoord tex_coord = newTexCoord(i_idx, i);
        i_material->mTexGenBlock->setTexCoord(i, &tex_coord);
    }
    if (bVar1 && mpIndInitData != NULL) {
        u8 ind_tex_stage_num = newIndTexStageNum(i_idx);
        i_material->mIndBlock->setIndTexStageNum(newIndTexStageNum(i_idx));
        for (u8 i = 0; i < ind_tex_stage_num; i++) {
            i_material->mIndBlock->setIndTexMtx(i, newIndTexMtx(i_idx, i));
        }
        for (u8 i = 0; i < ind_tex_stage_num; i++) {
            i_material->mIndBlock->setIndTexOrder(i, newIndTexOrder(i_idx, i));
        }
        for (u8 i = 0; i < ind_tex_stage_num; i++) {
            i_material->mIndBlock->setIndTexCoordScale(i, newIndTexCoordScale(i_idx, i));
        }
        for (u8 i = 0; i < tev_stage_num; i++) {
            i_material->mTevBlock->setIndTevStage(i, newIndTevStage(i_idx, i));
        }
    }
    return i_material;
}

void J3DMaterialFactory::modifyPatchedCurrentMtx(J3DMaterial* i_material, int i_idx) const {
    J3DTexCoord coord[8];
    u32 tex_gens = countTexGens(i_idx);
    for (u8 i = 0; i < tex_gens; i++) {
        coord[i] = newTexCoord(i_idx, i);
    }
    J3DCurrentMtx currentMtx;
    currentMtx.setCurrentTexMtx(
        coord[0].getTexGenMtx(),
        coord[1].getTexGenMtx(),
        coord[2].getTexGenMtx(),
        coord[3].getTexGenMtx(),
        coord[4].getTexGenMtx(),
        coord[5].getTexGenMtx(),
        coord[6].getTexGenMtx(),
        coord[7].getTexGenMtx()
    );
    i_material->mCurrentMtx = currentMtx;
}

J3DMaterial* J3DMaterialFactory::createLockedMaterial(J3DMaterial* i_material, int i_idx,
                                                      u32 i_flags) const {
    if (i_material == NULL) {
        i_material = new J3DLockedMaterial();
        i_material->mColorBlock = new J3DColorBlockNull();
        i_material->mTexGenBlock = new J3DTexGenBlockNull();
        i_material->mTevBlock = new J3DTevBlockNull();
        i_material->mIndBlock = new J3DIndBlockNull();
        i_material->mPEBlock = new J3DPEBlockNull();
        i_material->mIndex = i_idx;
        i_material->mMaterialMode = mpMaterialMode[i_idx];
    }

    i_material->mCurrentMtx = mpCurrentMtxInfo[i_idx];
    i_material->mColorBlock->setMatColorOffset(mpPatchingInfo[i_idx].mMatColorOffset);
    i_material->mColorBlock->setColorChanOffset(mpPatchingInfo[i_idx].mColorChanOffset);
    i_material->mTexGenBlock->setTexMtxOffset(mpPatchingInfo[i_idx].mTexMtxOffset);
    i_material->mTevBlock->setTexNoOffset(mpPatchingInfo[i_idx].mTexNoOffset);
    i_material->mTevBlock->setTevRegOffset(mpPatchingInfo[i_idx].mTevRegOffset);
    i_material->mPEBlock->setFogOffset(mpPatchingInfo[i_idx].mFogOffset);
    if (i_material->mSharedDLObj == NULL) {
        i_material->mSharedDLObj = new J3DDisplayListObj();
        i_material->mSharedDLObj->setSingleDisplayList((void*)(
            mpDisplayListInit[i_idx].mOffset + (uintptr_t)&mpDisplayListInit[i_idx]),
            mpDisplayListInit[i_idx].field_0x4
        );
    }
    return i_material;
}

u32 J3DMaterialFactory::calcSize(J3DMaterial* i_material, J3DMaterialFactory::MaterialType i_type,
                                  int i_idx, u32 i_flags) const {
    u32 size = 0;
    switch (i_type) {
        case MATERIAL_TYPE_NORMAL:
            size = calcSizeNormalMaterial(i_material, i_idx, i_flags);
            break;
        case MATERIAL_TYPE_LOCKED:
            size = calcSizeLockedMaterial(i_material, i_idx, i_flags);
            break;
        case MATERIAL_TYPE_PATCHED:
            size = calcSizePatchedMaterial(i_material, i_idx, i_flags);
            break;
    }
    return size;
}

u32 J3DMaterialFactory::calcSizeNormalMaterial(J3DMaterial* i_material, int i_idx,
                                               u32 i_flags) const {
    u32 size = 0;
    if (mpDisplayListInit != NULL) {
        return calcSizeLockedMaterial(i_material, i_idx, i_flags);
    }

    const u32 stages = countStages(i_idx);
    u32 tev_stage_num = getMdlDataFlag_TevStageNum(i_flags);
    u32 tev_stage_num_max = JMAMax(stages, tev_stage_num);
    u32 tex_num = tev_stage_num_max > 8 ? 8 : tev_stage_num_max;
    u32 tex_gens = countTexGens(i_idx);
    u32 tex_gen_flag = tex_gens > 4 ? getMdlDataFlag_TexGenFlag(0) : getMdlDataFlag_TexGenFlag(i_flags);
    u32 color_block_flag = getMdlDataFlag_ColorFlag(i_flags);
    u32 pe_flag = getMdlDataFlag_PEFlag(i_flags);
    BOOL ind_flag = (i_flags & 0x1000000) ? TRUE : FALSE;
    if (i_material == NULL) {
        size += sizeof(J3DMaterial);
    }
    size += J3DMaterial::calcSizeColorBlock(color_block_flag);
    size += J3DMaterial::calcSizeTexGenBlock(tex_gen_flag);
    size += J3DMaterial::calcSizeTevBlock((u16)tev_stage_num_max);
    size += J3DMaterial::calcSizeIndBlock(ind_flag);
    size += J3DMaterial::calcSizePEBlock(pe_flag, getMaterialMode(i_idx));
    J3DMaterialInitData* init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    for (u32 i = 0; i < 8; i++) {
        if (init_data->mTexMtxIdx[i] != 0xffff) {
            size += sizeof(J3DTexMtx);
        }
    }
    return size;
}

u32 J3DMaterialFactory::calcSizePatchedMaterial(J3DMaterial* i_material, int i_idx,
                                                u32 i_flags) const {
    u32 size = 0;
    if (i_material == NULL) {
        size = sizeof(J3DPatchedMaterial);
    }
    u8 ind_flag = (i_flags & 0x3000000) != 0 ? 1 : 0;
    size += J3DMaterial::calcSizeColorBlock(0x40000000);
    size += 0x134;  // TODO what is this
    size += J3DMaterial::calcSizeIndBlock(ind_flag);
    size += J3DMaterial::calcSizePEBlock(0x10000000, getMaterialMode(i_idx));
    J3DMaterialInitData* init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    for (int i = 0; i < 8; i++) {
        if (init_data->mTexMtxIdx[i] != 0xffff) {
            size += sizeof(J3DTexMtx);
        }
    }
    return size;
}

u32 J3DMaterialFactory::calcSizeLockedMaterial(J3DMaterial* i_material, int i_idx,
                                               u32 i_flags) const {
    u32 size = 0;
    if (i_material == NULL) {
        size = sizeof(J3DLockedMaterial) + sizeof(J3DColorBlockNull) + sizeof(J3DTexGenBlockNull)
            + sizeof(J3DTevBlockNull) + sizeof(J3DIndBlockNull) + sizeof(J3DPEBlockNull);
    }
    return size + sizeof(J3DDisplayListObj);
}

static GXColor const defaultMatColor = {0xff, 0xff, 0xff, 0xff};

J3DGXColor J3DMaterialFactory::newMatColor(int i_idx, int i_no) const {
    J3DGXColor dflt = (J3DGXColor)defaultMatColor;
    u16 mat_color_index = mpMaterialInitData[mpMaterialID[i_idx]].mMatColorIdx[i_no];
    if (mat_color_index != 0xffff) {
        return mpMatColor[mat_color_index];
    } else {
        return dflt;
    }
}

u8 J3DMaterialFactory::newColorChanNum(int i_idx) const {
    u8 color_chan_num_index = mpMaterialInitData[mpMaterialID[i_idx]].mColorChanNumIdx;
    if (color_chan_num_index != 0xff) {
        return mpColorChanNum[color_chan_num_index];
    } else {
        return 0;
    }
}

J3DColorChan J3DMaterialFactory::newColorChan(int i_idx, int i_no) const {
    u32 r29 = 0;
    J3DMaterialInitData* init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (init_data->mColorChanIdx[i_no] != 0xffff) {
        return J3DColorChan(mpColorChanInfo[init_data->mColorChanIdx[i_no]]);
    } else {
        return J3DColorChan();
    }
}

static GXColor const defaultAmbColor = {0x32, 0x32, 0x32, 0x32};

J3DGXColor J3DMaterialFactory::newAmbColor(int i_idx, int i_no) const {
    J3DGXColor dflt = (J3DGXColor)defaultAmbColor;
    u16 amb_color_index = mpMaterialInitData[mpMaterialID[i_idx]].mAmbColorIdx[i_no];
    if (amb_color_index != 0xffff) {
        return mpAmbColor[amb_color_index];
    } else {
        return dflt;
    }
}

u32 J3DMaterialFactory::newTexGenNum(int i_idx) const {
    u8 tex_gen_num_index = mpMaterialInitData[mpMaterialID[i_idx]].mTexGenNumIdx;
    if (tex_gen_num_index != 0xff) {
        return mpTexGenNum[tex_gen_num_index];
    } else {
        return 0;
    }
}

J3DTexCoord J3DMaterialFactory::newTexCoord(int i_idx, int i_no) const {
    u16 tex_coord_index = mpMaterialInitData[mpMaterialID[i_idx]].mTexCoordIdx[i_no];
    if (tex_coord_index != 0xffff) {
        return J3DTexCoord(mpTexCoordInfo[tex_coord_index]);
    } else {
        return J3DTexCoord();
    }
}

J3DTexMtx* J3DMaterialFactory::newTexMtx(int i_idx, int i_no) const {
    J3DTexMtx* tex_mtx = NULL;
    J3DMaterialInitData* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mTexMtxIdx[i_no] != 0xffff) {
        tex_mtx = new J3DTexMtx(mpTexMtxInfo[mtl_init_data->mTexMtxIdx[i_no]]);
    }
    return tex_mtx;
}

u8 J3DMaterialFactory::newCullMode(int i_idx) const {
    u8 cull_mode_index = mpMaterialInitData[mpMaterialID[i_idx]].mCullModeIdx;
    if (cull_mode_index != 0xff) {
        return mpCullMode[cull_mode_index];
    } else {
        return 0xff;
    }
}

u16 J3DMaterialFactory::newTexNo(int i_idx, int i_no) const {
    u16 tex_no_index = mpMaterialInitData[mpMaterialID[i_idx]].mTexNoIdx[i_no];
    if (tex_no_index != 0xffff) {
        return mpTexNo[tex_no_index];
    } else {
        return 0xffff;
    }
}

J3DTevOrder J3DMaterialFactory::newTevOrder(int i_idx, int i_no) const {
    J3DMaterialInitData* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mTevOrderIdx[i_no] != 0xffff) {
        return J3DTevOrder(mpTevOrderInfo[mtl_init_data->mTevOrderIdx[i_no]]);
    } else {
        return J3DTevOrder();
    }
}

J3DGXColorS10 J3DMaterialFactory::newTevColor(int i_idx, int i_no) const {
    J3DGXColorS10 dflt = (GXColorS10){0, 0, 0, 0};
    J3DMaterialInitData* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mTevColorIdx[i_no] != 0xffff) {
        return mpTevColor[mtl_init_data->mTevColorIdx[i_no]];
    } else {
        return dflt;
    }
}

static GXColor const defaultTevKColor = {0xff, 0xff, 0xff, 0xff};

J3DGXColor J3DMaterialFactory::newTevKColor(int i_idx, int i_no) const {
    J3DGXColor dflt = (J3DGXColor)defaultTevKColor;
    u16 tev_kcolor_index = mpMaterialInitData[mpMaterialID[i_idx]].mTevKColorIdx[i_no];
    if (tev_kcolor_index != 0xffff) {
        return mpTevKColor[tev_kcolor_index];
    } else {
        return dflt;
    }
}

u8 J3DMaterialFactory::newTevStageNum(int i_idx) const {
    u8 tev_stage_num_index = mpMaterialInitData[mpMaterialID[i_idx]].mTevStageNumIdx;
    if (tev_stage_num_index != 0xff) {
        return mpTevStageNum[tev_stage_num_index];
    } else {
        return 0xff;
    }
}

J3DTevStage J3DMaterialFactory::newTevStage(int i_idx, int i_no) const {
    u16 tev_stage_index = mpMaterialInitData[mpMaterialID[i_idx]].mTevStageIdx[i_no];
    if (tev_stage_index != 0xffff) {
        return J3DTevStage(mpTevStageInfo[tev_stage_index]);
    } else {
        return J3DTevStage();
    }
}

J3DTevSwapModeTable J3DMaterialFactory::newTevSwapModeTable(int i_idx, int i_no) const {
    u16 tev_swap_mode_table_index = mpMaterialInitData[mpMaterialID[i_idx]].mTevSwapModeTableIdx[i_no];
    if (tev_swap_mode_table_index != 0xffff) {
        return J3DTevSwapModeTable(mpTevSwapModeTableInfo[tev_swap_mode_table_index]);
    } else {
        return J3DTevSwapModeTable(j3dDefaultTevSwapModeTable);
    }
}

u8 J3DMaterialFactory::newIndTexStageNum(int i_idx) const {
    if (mpIndInitData[i_idx].mEnabled == true) {
        return mpIndInitData[i_idx].mIndTexStageNum;
    } else {
        return 0;
    }
}

J3DIndTexOrder J3DMaterialFactory::newIndTexOrder(int i_idx, int i_no) const {
    J3DIndTexOrder dflt;
    if (mpIndInitData[i_idx].mEnabled == true) {
        return J3DIndTexOrder(mpIndInitData[i_idx].mIndTexOrderInfo[i_no]);
    } else {
        return dflt;
    }
}

J3DIndTexMtx J3DMaterialFactory::newIndTexMtx(int i_idx, int i_no) const {
    J3DIndTexMtx dflt;
    if (mpIndInitData[i_idx].mEnabled == true) {
        return J3DIndTexMtx(mpIndInitData[i_idx].mIndTexMtxInfo[i_no]);
    } else {
        return dflt;
    }
}

J3DIndTevStage J3DMaterialFactory::newIndTevStage(int i_idx, int i_no) const {
    J3DIndTevStage dflt;
    if (mpIndInitData[i_idx].mEnabled == true) {
        return J3DIndTevStage(mpIndInitData[i_idx].mIndTevStageInfo[i_no]);
    } else {
        return dflt;
    }
}

J3DIndTexCoordScale J3DMaterialFactory::newIndTexCoordScale(int i_idx, int i_no) const {
    J3DIndTexCoordScale dflt;
    if (mpIndInitData[i_idx].mEnabled == true) {
        return J3DIndTexCoordScale(mpIndInitData[i_idx].mIndTexCoordScaleInfo[i_no]);
    } else {
        return dflt;
    }
}

J3DFog J3DMaterialFactory::newFog(int i_idx) const {
    J3DFog fog;
    u16 fog_index = mpMaterialInitData[mpMaterialID[i_idx]].mFogIdx;
    if (fog_index != 0xffff) {
        fog.setFogInfo(mpFogInfo[fog_index]);
    }
    return fog;
}

J3DAlphaComp J3DMaterialFactory::newAlphaComp(int i_idx) const {
    J3DMaterialInitData* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mAlphaCompIdx != 0xffff) {
        return J3DAlphaComp(mpAlphaCompInfo[mtl_init_data->mAlphaCompIdx]);
    } else {
        return J3DAlphaComp(0xffff);
    }
}

J3DBlend J3DMaterialFactory::newBlend(int i_idx) const {
    J3DMaterialInitData* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mBlendIdx != 0xffff) {
        return J3DBlend(mpBlendInfo[mtl_init_data->mBlendIdx]);
    } else {
        return J3DBlend(j3dDefaultBlendInfo);
    }
}

J3DZMode J3DMaterialFactory::newZMode(int i_idx) const {
    J3DMaterialInitData* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mZModeIdx != 0xff) {
        return J3DZMode(mpZModeInfo[mtl_init_data->mZModeIdx]);
    } else {
        return J3DZMode();
    }
}

u8 J3DMaterialFactory::newZCompLoc(int i_idx) const {
    J3DMaterialInitData* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mZCompLocIdx != 0xff){
        return mpZCompLoc[mtl_init_data->mZCompLocIdx];
    } else {
        return 0;
    }
}

u8 J3DMaterialFactory::newDither(int i_idx) const {
    J3DMaterialInitData* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mDitherIdx != 0xff){
        return mpDither[mtl_init_data->mDitherIdx];
    } else {
        return 1;
    }
}

J3DNBTScale J3DMaterialFactory::newNBTScale(int i_idx) const {
    J3DNBTScale dflt(j3dDefaultNBTScaleInfo);
    J3DMaterialInitData* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mNBTScaleIdx != 0xffff) {
        return J3DNBTScale(mpNBTScaleInfo[mtl_init_data->mNBTScaleIdx]);
    } else {
        return dflt;
    }
}
