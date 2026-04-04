#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "JSystem/J3DGraphLoader/J3DMaterialFactory.h"
#include "JSystem/J3DGraphLoader/J3DMaterialFactory_v21.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JSupport/JSupport.h"

J3DMaterialFactory_v21::J3DMaterialFactory_v21(J3DMaterialBlock_v21 const& i_block) {
    mMaterialNum = i_block.mMaterialNum;
    mpMaterialInitData = JSUConvertOffsetToPtr<J3DMaterialInitData_v21>(&i_block, i_block.mpMaterialInitData);
    mpMaterialID = JSUConvertOffsetToPtr<BE(u16)>(&i_block, i_block.mpMaterialID);
    mpCullMode = JSUConvertOffsetToPtr<BE(GXCullMode)>(&i_block, i_block.mpCullMode);
    mpMatColor = JSUConvertOffsetToPtr<GXColor>(&i_block, i_block.mpMatColor);
    mpColorChanNum = JSUConvertOffsetToPtr<u8>(&i_block, i_block.mpColorChanNum);
    mpColorChanInfo = JSUConvertOffsetToPtr<J3DColorChanInfo>(&i_block, i_block.mpColorChanInfo);
    mpTexGenNum = JSUConvertOffsetToPtr<u8>(&i_block, i_block.mpTexGenNum);
    mpTexCoordInfo = JSUConvertOffsetToPtr<J3DTexCoordInfo>(&i_block, i_block.mpTexCoordInfo);
    mpTexCoord2Info = JSUConvertOffsetToPtr<J3DTexCoord2Info>(&i_block, i_block.mpTexCoord2Info);
    mpTexMtxInfo = JSUConvertOffsetToPtr<J3DTexMtxInfo>(&i_block, i_block.mpTexMtxInfo);
    field_0x28 = JSUConvertOffsetToPtr<J3DTexMtxInfo>(&i_block, i_block.field_0x38);
    mpTexNo = JSUConvertOffsetToPtr<BE(u16)>(&i_block, i_block.mpTexNo);
    mpTevOrderInfo = JSUConvertOffsetToPtr<J3DTevOrderInfo>(&i_block, i_block.mpTevOrderInfo);
    mpTevColor = JSUConvertOffsetToPtr<BE(GXColorS10)>(&i_block, i_block.mpTevColor);
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
}

u16 J3DMaterialFactory_v21::countUniqueMaterials() {
    u16 i;
    for (i = 0; i < mMaterialNum; i++) {
        if (mpMaterialID[i] - i == 0) {
            // No code, maybe commented out.
        }
    }
    return i;
}

u32 J3DMaterialFactory_v21::countTexGens(int i_idx) const {
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mTexGenNumIdx != 0xff) {
        return mpTexGenNum[mtl_init_data->mTexGenNumIdx];
    }
    return 0;
}

u32 J3DMaterialFactory_v21::countStages(int i_idx) const {
    J3DMaterialInitData_v21* init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    u32 count1 = 0;
    u32 count2 = 0;
    if (init_data->mTevStageNumIdx != 0xff) {
        count2 = mpTevStageNum[init_data->mTevStageNumIdx];
    }
    for (int i = 0; i < 8; i++) {
        if (init_data->mTexNoIdx[i] != 0xffff) {
            count1++;
        }
    }
    if (count2 != count1 && count1 != 0) {
        u32 count3;
        if (count2 > count1) {
            count3 = count2;
        } else {
            count3 = count1;
        }
        return count3;
    }
    return count2;
}

J3DMaterial* J3DMaterialFactory_v21::create(J3DMaterial* i_material, int i_idx, u32 i_flags) const {
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
        i_material = JKR_NEW J3DMaterial();
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
        J3DMaterialInitData_v21* material_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
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
    J3DMaterialInitData_v21* material_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
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
    return i_material;
}

J3DGXColor J3DMaterialFactory_v21::newMatColor(int i_idx, int i_no) const {
    #ifdef __MWERKS__
    J3DGXColor defaultColor = (GXColor){0xff, 0xff, 0xff, 0xff};
    #else
    J3DGXColor defaultColor = GXColor{0xff, 0xff, 0xff, 0xff};
    #endif
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mMatColorIdx[i_no] != 0xffff) {
        return J3DGXColor(mpMatColor[mtl_init_data->mMatColorIdx[i_no]]);
    } 
    return J3DGXColor(defaultColor);
}

const u8 J3DMaterialFactory_v21::newColorChanNum(int i_idx) const {
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mColorChanNumIdx != 0xff) {
        return mpColorChanNum[mtl_init_data->mColorChanNumIdx];
    } else {
        return 0;
    }
}

J3DColorChan J3DMaterialFactory_v21::newColorChan(int i_idx, int i_no) const {
    u8 r29 = 0;
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mColorChanIdx[i_no] != 0xffff) {
        return J3DColorChan(mpColorChanInfo[mtl_init_data->mColorChanIdx[i_no]]);
    } else {
        return J3DColorChan();
    }
}

u32 J3DMaterialFactory_v21::newTexGenNum(int i_idx) const {
    u8 r30 = 0;
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mTexGenNumIdx != 0xff) {
        return mpTexGenNum[mtl_init_data->mTexGenNumIdx];
    } else {
        return 0;
    }
}

J3DTexCoord J3DMaterialFactory_v21::newTexCoord(int i_idx, int i_no) const {
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mTexCoordIdx[i_no] != 0xffff) {
        return J3DTexCoord(mpTexCoordInfo[mtl_init_data->mTexCoordIdx[i_no]]);
    } else {
        return J3DTexCoord();
    }
}

J3DTexMtx* J3DMaterialFactory_v21::newTexMtx(int i_idx, int i_no) const {
    J3DTexMtx* tex_mtx = NULL;
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mTexMtxIdx[i_no] != 0xffff) {
        tex_mtx = JKR_NEW J3DTexMtx(mpTexMtxInfo[mtl_init_data->mTexMtxIdx[i_no]]);
    }
    return tex_mtx;
}

u8 J3DMaterialFactory_v21::newCullMode(int i_idx) const {
    u8 r30 = 0;
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mCullModeIdx != 0xff) {
        return mpCullMode[mtl_init_data->mCullModeIdx];
    } else {
        return 0xff;
    }
}

u16 J3DMaterialFactory_v21::newTexNo(int i_idx, int i_no) const {
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mTexNoIdx[i_no] != 0xffff) {
        return mpTexNo[mtl_init_data->mTexNoIdx[i_no]];
    } else {
        return 0xffff;
    }
}

J3DTevOrder J3DMaterialFactory_v21::newTevOrder(int i_idx, int i_no) const {
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mTevOrderIdx[i_no] != 0xffff) {
        return J3DTevOrder(mpTevOrderInfo[mtl_init_data->mTevOrderIdx[i_no]]);
    } else {
        return J3DTevOrder();
    }
}

J3DGXColorS10 J3DMaterialFactory_v21::newTevColor(int i_idx, int i_no) const {
    GXColorS10 defaultTevColor = {0,0,0,0};
    J3DGXColorS10 dflt = defaultTevColor;
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mTevColorIdx[i_no] != 0xffff) {
        return mpTevColor[mtl_init_data->mTevColorIdx[i_no]];
    } else {
        return dflt;
    }
}

J3DGXColor J3DMaterialFactory_v21::newTevKColor(int i_idx, int param_1) const {
    #ifdef __MWERKS__ 
    J3DGXColor defaultColor = (GXColor){0xff, 0xff, 0xff, 0xff};
    #else
    J3DGXColor defaultColor = GXColor{0xff, 0xff, 0xff, 0xff};
    #endif
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mTevKColorIdx[param_1] != 0xffff) {
        return J3DGXColor(mpTevKColor[mtl_init_data->mTevKColorIdx[param_1]]);
    } 
    return J3DGXColor(defaultColor);
}

const u8 J3DMaterialFactory_v21::newTevStageNum(int i_idx) const {
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mTevStageNumIdx != 0xff) {
        return mpTevStageNum[mtl_init_data->mTevStageNumIdx];
    } else {
        return 0xff;
    }
}

J3DTevStage J3DMaterialFactory_v21::newTevStage(int i_idx, int i_no) const {
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mTevStageIdx[i_no] != 0xffff) {
        return J3DTevStage(mpTevStageInfo[mtl_init_data->mTevStageIdx[i_no]]);
    } else {
        return J3DTevStage();
    }
}

J3DTevSwapModeTable J3DMaterialFactory_v21::newTevSwapModeTable(int i_idx, int param_1) const {
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mTevSwapModeTableIdx[param_1] != 0xffff) {
        return J3DTevSwapModeTable(mpTevSwapModeTableInfo[mtl_init_data->mTevSwapModeTableIdx[param_1]]);
    } 
    return J3DTevSwapModeTable(j3dDefaultTevSwapModeTable);
}

J3DFog J3DMaterialFactory_v21::newFog(int i_idx) const {
    J3DFog fog;
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mFogIdx != 0xffff) {
        fog.setFogInfo(mpFogInfo[mtl_init_data->mFogIdx]);
    }
    return fog;
}

J3DAlphaComp J3DMaterialFactory_v21::newAlphaComp(int i_idx) const {
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mAlphaCompIdx != 0xffff) {
        return J3DAlphaComp(mpAlphaCompInfo[mtl_init_data->mAlphaCompIdx]);
    } 
    return J3DAlphaComp(0xffff);
}

J3DBlend J3DMaterialFactory_v21::newBlend(int i_idx) const {
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mBlendIdx != 0xffff) {
        return J3DBlend(mpBlendInfo[mtl_init_data->mBlendIdx]);
    } else {
        return J3DBlend(j3dDefaultBlendInfo);
    }
}

const J3DZMode J3DMaterialFactory_v21::newZMode(int i_idx) const {
    u8 r29 = 0;
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mZModeIdx != 0xff) {
        return J3DZMode(mpZModeInfo[mtl_init_data->mZModeIdx]);
    } else {
        return J3DZMode();
    }
}

const u8 J3DMaterialFactory_v21::newZCompLoc(int i_idx) const {
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mZCompLocIdx != 0xff){
        return mpZCompLoc[mtl_init_data->mZCompLocIdx];
    } else {
        return 0;
    }
}

const u8 J3DMaterialFactory_v21::newDither(int i_idx) const {
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mDitherIdx != 0xff){
        return mpDither[mtl_init_data->mDitherIdx];
    } else {
        return 1;
    }
}

J3DNBTScale J3DMaterialFactory_v21::newNBTScale(int i_idx) const {
    J3DNBTScale dflt(j3dDefaultNBTScaleInfo);
    J3DMaterialInitData_v21* mtl_init_data = &mpMaterialInitData[mpMaterialID[i_idx]];
    if (mtl_init_data->mNBTScaleIdx != 0xffff) {
        return J3DNBTScale(mpNBTScaleInfo[mtl_init_data->mNBTScaleIdx]);
    } else {
        return dflt;
    }
}
