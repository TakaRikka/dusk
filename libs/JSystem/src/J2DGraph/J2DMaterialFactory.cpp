#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DMaterialFactory.h"
#include "JSystem/J2DGraph/J2DMaterial.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/JSupport/JSupport.h"
#include "JSystem/JUtility/JUTResource.h"
#include <cstring>
#include <types.h>

J2DMaterialFactory::J2DMaterialFactory(J2DMaterialBlock const& param_0) {
    mMaterialNum = param_0.field_0x8;
    mpMaterialInitData = JSUConvertOffsetToPtr<J2DMaterialInitData>(&param_0, param_0.field_0xc);
    mpMaterialID = JSUConvertOffsetToPtr<BE(u16)>(&param_0, param_0.field_0x10);
    if (param_0.field_0x18 && param_0.field_0x18 - param_0.field_0x14 > 4) {
        mpIndInitData = JSUConvertOffsetToPtr<J2DIndInitData>(&param_0, param_0.field_0x18);
    }
    else {
        mpIndInitData = NULL;
    }
    mpCullMode = JSUConvertOffsetToPtr<BE(GXCullMode)>(&param_0, param_0.field_0x1c);
    mpMatColor = JSUConvertOffsetToPtr<GXColor>(&param_0, param_0.field_0x20);
    mpColorChanNum = JSUConvertOffsetToPtr<u8>(&param_0, param_0.field_0x24);
    mpColorChanInfo = JSUConvertOffsetToPtr<J2DColorChanInfo>(&param_0, param_0.field_0x28);
    mpTexGenNum = JSUConvertOffsetToPtr<u8>(&param_0, param_0.field_0x2c);
    mpTexCoordInfo = JSUConvertOffsetToPtr<J2DTexCoordInfo>(&param_0, param_0.field_0x30);
    mpTexMtxInfo = JSUConvertOffsetToPtr<J2DTexMtxInfo>(&param_0, param_0.field_0x34);
    mpTexNo = JSUConvertOffsetToPtr<BE(u16)>(&param_0, param_0.field_0x38);
    mpFontNo = JSUConvertOffsetToPtr<BE(u16)>(&param_0, param_0.field_0x3c);
    mpTevOrderInfo = JSUConvertOffsetToPtr<J2DTevOrderInfo>(&param_0, param_0.field_0x40);
    mpTevColor = JSUConvertOffsetToPtr<BE(GXColorS10)>(&param_0, param_0.field_0x44);
    mpTevKColor = JSUConvertOffsetToPtr<GXColor>(&param_0, param_0.field_0x48);
    mpTevStageNum = JSUConvertOffsetToPtr<u8>(&param_0, param_0.field_0x4c);
    mpTevStageInfo = JSUConvertOffsetToPtr<J2DTevStageInfo>(&param_0, param_0.field_0x50);
    mpTevSwapModeInfo = JSUConvertOffsetToPtr<J2DTevSwapModeInfo>(&param_0, param_0.field_0x54);
    mpTevSwapModeTableInfo = JSUConvertOffsetToPtr<J2DTevSwapModeTableInfo>(&param_0, param_0.field_0x58);
    mpAlphaCompInfo = JSUConvertOffsetToPtr<J2DAlphaCompInfo>(&param_0, param_0.field_0x5c);
    mpBlendInfo = JSUConvertOffsetToPtr<J2DBlendInfo>(&param_0, param_0.field_0x60);
    mpDither = JSUConvertOffsetToPtr<u8>(&param_0, param_0.field_0x64);
}

u32 J2DMaterialFactory::countStages(int param_0) const {
    J2DMaterialInitData* iVar5 = &mpMaterialInitData[mpMaterialID[param_0]];
    u32 uVar4 = 0;
    u32 uVar3 = 0;
    if (iVar5->field_0x4 != 0xff) {
        uVar3 = mpTevStageNum[iVar5->field_0x4];
    }
    for (int i = 0; i < 8; i++) {
        if (iVar5->field_0x38[i] != 0xffff) {
            uVar4++;
        }
    }
    if ((uVar3 != uVar4 && uVar4 != 0)) {
        return uVar3 > uVar4 ? uVar3 : uVar4;
    }
    return uVar3;
}

J2DMaterial* J2DMaterialFactory::create(J2DMaterial* param_0, int index, u32 param_2,
                                    J2DResReference* param_3, J2DResReference* param_4,
                                    JKRArchive* param_5) const {
    u32 stages = countStages(index);
    u32 local_36c = ((param_2 & 0x1f0000) >> 16);
    u32 local_370 = stages > local_36c ? stages : local_36c;
    u32 local_374 = local_370 > 8 ? 8 : local_370;
    s32 local_378 = (param_2 & 0x1000000) ? 1 : 0;
    local_378 = (param_2 & 0x1f0000) ? local_378 : 0;
    bool local_403 = (param_2 & 0x1f0000);
    param_0->mTevBlock = J2DMaterial::createTevBlock((u16)local_370, local_403);
    param_0->mIndBlock = J2DMaterial::createIndBlock(local_378, local_403);
    param_0->mIndex = index;
    param_0->field_0x8 = getMaterialMode(index);
    param_0->getColorBlock()->setColorChanNum(newColorChanNum(index));
    param_0->getColorBlock()->setCullMode(newCullMode(index));
    param_0->getTexGenBlock()->setTexGenNum(newTexGenNum(index));
    param_0->getPEBlock()->setAlphaComp(newAlphaComp(index));
    param_0->getPEBlock()->setBlend(newBlend(index));
    param_0->getPEBlock()->setDither(newDither(index));
    param_0->getTevBlock()->setTevStageNum(newTevStageNum(index));
    param_0->mMaterialAlphaCalc = getMaterialAlphaCalc(index);

    JUTResReference aJStack_12c;
    for (u8 i = 0; i < local_374; i++) {
        u16 texNo = newTexNo(index, i);
        char* local_37c = param_3->getResReference(texNo);
        void* local_380 = NULL;
        if (local_37c != NULL) {
            local_380 = aJStack_12c.getResource(local_37c, 'TIMG', param_5);
            if (local_380 == NULL && param_5 != NULL) {
                local_380 = aJStack_12c.getResource(local_37c, 'TIMG', NULL);
            }
            if (local_380 == NULL && J2DScreen::getDataManage() != NULL) {
                char acStack_230[257];
                strcpy(acStack_230, param_3->getName(texNo));
                local_380 = J2DScreen::getDataManage()->get(acStack_230);
            }
        }
        param_0->getTevBlock()->insertTexture(i, (ResTIMG*)local_380);
        param_0->getTevBlock()->setTexNo(i, texNo);
    }
    u16 fontNo = newFontNo(index);
    param_0->getTevBlock()->setFontNo(fontNo);
    
    char* local_384 = param_4->getResReference(param_0->getTevBlock()->getFontNo());
    void* local_388 = NULL;
    if (local_384 != NULL) {
        local_388 = aJStack_12c.getResource(local_384, 'FONT', param_5);
        if (local_388 == NULL && param_5 != NULL) {
            local_388 = aJStack_12c.getResource(local_384, 'FONT', NULL);
        }
        if (local_388 == NULL && J2DScreen::getDataManage() != NULL) {
            char acStack_334[257];
            strcpy(acStack_334, param_4->getName(param_0->getTevBlock()->getFontNo()));
            local_388 = J2DScreen::getDataManage()->get(acStack_334);
        }
    }

    param_0->getTevBlock()->setFont((ResFONT*)local_388);
    for (u8 i = 0; i < local_370; i++) {
        param_0->getTevBlock()->setTevOrder(i, newTevOrder(index, i));
    }
    for (u8 i = 0; i < local_370; i++) {
        J2DMaterialInitData* local_38c = &mpMaterialInitData[mpMaterialID[index]];
        param_0->getTevBlock()->setTevStage(i, newTevStage(index, i));
        if (local_38c->field_0xba[i] != 0xffff) {
            param_0->getTevBlock()->getTevStage(i)->setTexSel(mpTevSwapModeInfo[local_38c->field_0xba[i]].mTexSel);
            param_0->getTevBlock()->getTevStage(i)->setRasSel(mpTevSwapModeInfo[local_38c->field_0xba[i]].mRasSel);
        }
    }
    for (u8 i = 0; i < 4; i++) {
        param_0->getTevBlock()->setTevKColor(i, newTevKColor(index, i));
    }
    for (u8 i = 0; i < 4; i++) {
        param_0->getTevBlock()->setTevColor(i, newTevColor(index, i));
    }
    for (u8 i = 0; i < 4; i++) {
        param_0->getTevBlock()->setTevSwapModeTable(i, newTevSwapModeTable(index, i));
    }
    for (u8 i = 0; i < 2; i++) {
        param_0->getColorBlock()->setMatColor(i, newMatColor(index, i));
    }
    for (u8 i = 0; i < 4; i++) {
        J2DColorChan colorChan = newColorChan(index, i);
        param_0->getColorBlock()->setColorChan(i, colorChan);
    }
    for (u8 i = 0; i < 8; i++) {
        J2DTexCoord texCoord = newTexCoord(index, i);
        param_0->getTexGenBlock()->setTexCoord(i, &texCoord);
    }
    for (u8 i = 0; i < 8; i++) {
        param_0->getTexGenBlock()->setTexMtx(i, newTexMtx(index, i));
    }
    J2DMaterialInitData* local_394 = &mpMaterialInitData[mpMaterialID[index]];
    for (u8 i = 0; i < local_370; i++) {
        param_0->getTevBlock()->setTevKColorSel(i, local_394->field_0x52[i]);
    }
    for (u8 i = 0; i < local_370; i++) {
        param_0->getTevBlock()->setTevKAlphaSel(i, local_394->field_0x62[i]);
    }
    if (mpIndInitData != NULL || local_378 != 0) {
        u8 local_410 = newIndTexStageNum(index);
        param_0->mIndBlock->setIndTexStageNum(local_410);
        for (u8 i = 0; i < local_410; i++) {
            param_0->getIndBlock()->setIndTexMtx(i, newIndTexMtx(index, i));
        }
        for (u8 i = 0; i < local_410; i++) {
            param_0->getIndBlock()->setIndTexOrder(i, newIndTexOrder(index, i));
        }
        for (u8 i = 0; i < local_410; i++) {
            param_0->getIndBlock()->setIndTexCoordScale(i, newIndTexCoordScale(index, i));
        }
        for (u8 i = 0; i < local_370; i++) {
            param_0->getTevBlock()->setIndTevStage(i, newIndTevStage(index, i));
        }
    }
    return param_0;
}

JUtility::TColor J2DMaterialFactory::newMatColor(int param_0, int param_1) const {
#ifdef __MWERKS__
    JUtility::TColor local_20 = (GXColor){0xff,0xff,0xff,0xff};
#else
    JUtility::TColor local_20 = GXColor{0xff,0xff,0xff,0xff};
#endif
    J2DMaterialInitData* iVar2 = &mpMaterialInitData[mpMaterialID[param_0]];
    if (iVar2->field_0x8[param_1] != 0xffff) {
        return mpMatColor[iVar2->field_0x8[param_1]];
    }
    return local_20;
}

u8 J2DMaterialFactory::newColorChanNum(int param_0) const {
    J2DMaterialInitData* iVar2 = &mpMaterialInitData[mpMaterialID[param_0]];
    if (iVar2->field_0x2 != 0xff) {
        return mpColorChanNum[iVar2->field_0x2];
    }
    return 0;
}

J2DColorChan J2DMaterialFactory::newColorChan(int param_0, int param_1) const {
    int r29 = 0;
    J2DMaterialInitData* iVar2 = &mpMaterialInitData[mpMaterialID[param_0]];
    if (iVar2->field_0xc[param_1] != 0xffff) {
        return J2DColorChan(mpColorChanInfo[iVar2->field_0xc[param_1]]);
    }
    return J2DColorChan();
}

u32 J2DMaterialFactory::newTexGenNum(int param_0) const {
    int r30 = 0;
    J2DMaterialInitData* iVar2 = &mpMaterialInitData[mpMaterialID[param_0]];
    if (iVar2->field_0x3 != 0xff) {
        return mpTexGenNum[iVar2->field_0x3];
    }
    return 0;
}

J2DTexCoord J2DMaterialFactory::newTexCoord(int param_0, int param_1) const {
    J2DMaterialInitData* iVar2 = &mpMaterialInitData[mpMaterialID[param_0]];
    if (iVar2->field_0x14[param_1] != 0xffff) {
        return J2DTexCoord(mpTexCoordInfo[iVar2->field_0x14[param_1]]);
    }
    return J2DTexCoord();
}

J2DTexMtx* J2DMaterialFactory::newTexMtx(int param_0, int param_1) const {
    J2DTexMtx* rv = NULL;
    J2DMaterialInitData* iVar2 = &mpMaterialInitData[mpMaterialID[param_0]];
    if (iVar2->field_0x24[param_1] != 0xffff) {
        rv = JKR_NEW J2DTexMtx(mpTexMtxInfo[iVar2->field_0x24[param_1]]);
        rv->calc();
    }
    return rv;
}

u8 J2DMaterialFactory::newCullMode(int param_0) const {
    int r30 = 0;
    J2DMaterialInitData* iVar2 = &mpMaterialInitData[mpMaterialID[param_0]];
    if (iVar2->field_0x1 != 0xff) {
        return mpCullMode[iVar2->field_0x1];
    }
    return 0xff;
}

u16 J2DMaterialFactory::newTexNo(int param_0, int param_1) const {
    J2DMaterialInitData* iVar2 = &mpMaterialInitData[mpMaterialID[param_0]];
    if (iVar2->field_0x38[param_1] != 0xffff) {
        return mpTexNo[iVar2->field_0x38[param_1]];
    }
    return 0xFFFF;
}

u16 J2DMaterialFactory::newFontNo(int param_0) const {
    J2DMaterialInitData* iVar2 = &mpMaterialInitData[mpMaterialID[param_0]];
    if (iVar2->field_0x48 != 0xffff) {
        return mpFontNo[iVar2->field_0x48];
    }
    return 0xFFFF;
}

J2DTevOrder J2DMaterialFactory::newTevOrder(int param_0, int param_1) const {
    J2DMaterialInitData* iVar2 = &mpMaterialInitData[mpMaterialID[param_0]];
    if (iVar2->field_0x72[param_1] != 0xffff) {
        return J2DTevOrder(mpTevOrderInfo[iVar2->field_0x72[param_1]]);
    }
    return J2DTevOrder();
}

J2DGXColorS10 J2DMaterialFactory::newTevColor(int param_0, int param_1) const {
    GXColorS10 color = {0, 0, 0, 0};
    J2DGXColorS10 rv = color;
    J2DMaterialInitData* iVar2 = &mpMaterialInitData[mpMaterialID[param_0]];

    if (iVar2->field_0x92[param_1] != 0xffff) {
        return (GXColorS10) mpTevColor[iVar2->field_0x92[param_1]];
    }
    
    return rv;
}

JUtility::TColor J2DMaterialFactory::newTevKColor(int param_0, int param_1) const {
    JUtility::TColor local_20 = COMPOUND_LITERAL(GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    J2DMaterialInitData* iVar2 = &mpMaterialInitData[mpMaterialID[param_0]];
    if (iVar2->field_0x4a[param_1] != 0xffff) {
        return mpTevKColor[iVar2->field_0x4a[param_1]];
    }
    return local_20;
}

u8 J2DMaterialFactory::newTevStageNum(int param_0) const {
    J2DMaterialInitData* iVar2 = &mpMaterialInitData[mpMaterialID[param_0]];
    if (iVar2->field_0x4 != 0xff) {
        return mpTevStageNum[iVar2->field_0x4];
    }
    return 0xFF;
}

J2DTevStage J2DMaterialFactory::newTevStage(int param_0, int param_1) const {
    J2DMaterialInitData* iVar2 = &mpMaterialInitData[mpMaterialID[param_0]];
    if (iVar2->field_0x9a[param_1] != 0xffff) {
        return J2DTevStage(mpTevStageInfo[iVar2->field_0x9a[param_1]]);
    }
    return J2DTevStage();
}

J2DTevSwapModeTable J2DMaterialFactory::newTevSwapModeTable(int param_0, int param_1) const {
    J2DMaterialInitData* iVar2 = &mpMaterialInitData[mpMaterialID[param_0]];
    if (iVar2->field_0xda[param_1] != 0xffff) {
        return J2DTevSwapModeTable(mpTevSwapModeTableInfo[iVar2->field_0xda[param_1]]);
    }
    return J2DTevSwapModeTable(j2dDefaultTevSwapModeTable);
}

u8 J2DMaterialFactory::newIndTexStageNum(int param_0) const {
    u8 r31 = 0;
    if (mpIndInitData != NULL) {
        if (mpIndInitData[param_0].field_0x0 == 1) {
            return mpIndInitData[param_0].field_0x1;
        }
    }
    return r31;
}

J2DIndTexOrder J2DMaterialFactory::newIndTexOrder(int param_0, int param_1) const {
    J2DIndTexOrder rv;
    if (mpIndInitData != NULL) {
        if (mpIndInitData[param_0].field_0x0 == 1) {
            return J2DIndTexOrder(mpIndInitData[param_0].field_0x4[param_1]);
        }
    }
    return rv;
}

J2DIndTexMtx J2DMaterialFactory::newIndTexMtx(int param_0, int param_1) const {
    J2DIndTexMtx rv;
    if (mpIndInitData != NULL) {
        if (mpIndInitData[param_0].field_0x0 == 1) {
            return J2DIndTexMtx(mpIndInitData[param_0].field_0xc[param_1]);
        }
    }
    return rv;
}

J2DIndTevStage J2DMaterialFactory::newIndTevStage(int param_0, int param_1) const {
    J2DIndTevStage rv;
    if (mpIndInitData != NULL) {
        if (mpIndInitData[param_0].field_0x0 == 1) {
            return J2DIndTevStage(mpIndInitData[param_0].field_0x68[param_1]);
        }
    }
    return rv;
}

J2DIndTexCoordScale J2DMaterialFactory::newIndTexCoordScale(int param_0, int param_1) const {
    J2DIndTexCoordScale rv;
    if (mpIndInitData != NULL) {
        if (mpIndInitData[param_0].field_0x0 == 1) {
            return J2DIndTexCoordScale(mpIndInitData[param_0].field_0x60[param_1]);
        }
    }
    return rv;
}

J2DAlphaComp J2DMaterialFactory::newAlphaComp(int param_0) const {
    J2DMaterialInitData* iVar2 = &mpMaterialInitData[mpMaterialID[param_0]];
    if (iVar2->field_0xe2 != 0xffff) {
        return J2DAlphaComp(mpAlphaCompInfo[iVar2->field_0xe2]);
    }
    return J2DAlphaComp();
}

J2DBlend J2DMaterialFactory::newBlend(int param_0) const {
    J2DMaterialInitData* iVar2 = &mpMaterialInitData[mpMaterialID[param_0]];
    if (iVar2->field_0xe4 != 0xffff) {
        return J2DBlend(mpBlendInfo[iVar2->field_0xe4]);
    }
    return J2DBlend(j2dDefaultBlendInfo);
}

u8 J2DMaterialFactory::newDither(int param_0) const {
    J2DMaterialInitData* iVar2 = &mpMaterialInitData[mpMaterialID[param_0]];
    if (iVar2->field_0x5 != 0xff) {
        return mpDither[iVar2->field_0x5];
    }
    return 0;
}
