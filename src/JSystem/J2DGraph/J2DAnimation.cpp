#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DAnimation.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/J3DGraphBase/J3DTexture.h"
#include "JSystem/JUtility/JUTPalette.h"
#include "JSystem/JUtility/JUTResource.h"
#include "dolphin/os.h"

f32 J2DGetKeyFrameInterpolationf(f32 param_0, J3DAnmKeyTableBase* param_1, f32* param_2);
f32 J2DGetKeyFrameInterpolations(f32 param_0, J3DAnmKeyTableBase* param_1, s16* param_2);

void J2DAnmTransformFull::getTransform(u16 param_0, J3DTransformInfo* transformInfo) const {
    u16 idx = (param_0 * 3);
    J3DAnmTransformFullTable* xPart = &mTableInfo[idx];
    J3DAnmTransformFullTable* yPart = &mTableInfo[idx + 1];
    J3DAnmTransformFullTable* zPart = &mTableInfo[idx + 2];
    u16 xMaxFrame = xPart->mScaleMaxFrame;
    if (getFrame() < 0) {
        transformInfo->mScale.x = mScaleValues[xPart->mScaleOffset];
    } else if (getFrame() >= xMaxFrame) {
        transformInfo->mScale.x = mScaleValues[(xPart->mScaleOffset + (xMaxFrame - 1))];
    } else {
        transformInfo->mScale.x = mScaleValues[(xPart->mScaleOffset + (int)getFrame())];
    }
    u16 yMaxFrame = yPart->mScaleMaxFrame;
    if (getFrame() < 0) {
        transformInfo->mScale.y = mScaleValues[yPart->mScaleOffset];
    } else if (getFrame() >= yMaxFrame) {
        transformInfo->mScale.y = mScaleValues[(yPart->mScaleOffset + (yMaxFrame - 1))];
    } else {
        transformInfo->mScale.y = mScaleValues[(yPart->mScaleOffset + (int)getFrame())];
    }
    u16 zMaxFrame = zPart->mScaleMaxFrame;
    if (getFrame() < 0) {
        transformInfo->mScale.z = mScaleValues[zPart->mScaleOffset];
    } else if (getFrame() >= zMaxFrame) {
        transformInfo->mScale.z = mScaleValues[(zPart->mScaleOffset + (zMaxFrame - 1))];
    } else {
        transformInfo->mScale.z = mScaleValues[(zPart->mScaleOffset + (int)getFrame())];
    }
    xMaxFrame = xPart->mRotationMaxFrame;
    if (getFrame() < 0) {
        transformInfo->mRotation.x = mRotationValues[xPart->mRotationOffset];
    } else if (getFrame() >= xMaxFrame) {
        transformInfo->mRotation.x = mRotationValues[(xPart->mRotationOffset + (xMaxFrame - 1))];
    } else {
        transformInfo->mRotation.x = mRotationValues[(xPart->mRotationOffset + (int)getFrame())];
    }
    yMaxFrame = yPart->mRotationMaxFrame;
    if (getFrame() < 0) {
        transformInfo->mRotation.y = mRotationValues[yPart->mRotationOffset];
    } else if (getFrame() >= yMaxFrame) {
        transformInfo->mRotation.y = mRotationValues[(yPart->mRotationOffset + (yMaxFrame - 1))];
    } else {
        transformInfo->mRotation.y = mRotationValues[(yPart->mRotationOffset + (int)getFrame())];
    }
    zMaxFrame = zPart->mRotationMaxFrame;
    if (getFrame() < 0) {
        transformInfo->mRotation.z = mRotationValues[zPart->mRotationOffset];
    } else if (getFrame() >= zMaxFrame) {
        transformInfo->mRotation.z = mRotationValues[(zPart->mRotationOffset + (zMaxFrame - 1))];
    } else {
        transformInfo->mRotation.z = mRotationValues[(zPart->mRotationOffset + (int)getFrame())];
    }
    xMaxFrame = xPart->mTranslateMaxFrame;
    if (getFrame() < 0) {
        transformInfo->mTranslate.x = mTranslateValues[xPart->mTranslateOffset];
    } else if (getFrame() >= xMaxFrame) {
        transformInfo->mTranslate.x = mTranslateValues[(xPart->mTranslateOffset + (xMaxFrame - 1))];
    } else {
        transformInfo->mTranslate.x = mTranslateValues[(xPart->mTranslateOffset + (int)getFrame())];
    }
    yMaxFrame = yPart->mTranslateMaxFrame;
    if (getFrame() < 0) {
        transformInfo->mTranslate.y = mTranslateValues[yPart->mTranslateOffset];
    } else if (getFrame() >= yMaxFrame) {
        transformInfo->mTranslate.y = mTranslateValues[(yPart->mTranslateOffset + (yMaxFrame - 1))];
    } else {
        transformInfo->mTranslate.y = mTranslateValues[(yPart->mTranslateOffset + (int)getFrame())];
    }
    zMaxFrame = zPart->mTranslateMaxFrame;
    if (getFrame() < 0) {
        transformInfo->mTranslate.z = mTranslateValues[zPart->mTranslateOffset];
    } else if (getFrame() >= zMaxFrame) {
        transformInfo->mTranslate.z = mTranslateValues[(zPart->mTranslateOffset + (zMaxFrame - 1))];
    } else {
        transformInfo->mTranslate.z = mTranslateValues[(zPart->mTranslateOffset + (int)getFrame())];
    }
}

void J2DAnmTransformKey::calcTransform(f32 param_0, u16 param_1,
                                       J3DTransformInfo* transformInfo) const {
    u16 idx = param_1 * 3;
    J3DAnmTransformKeyTable* xInf = &mInfoTable[idx];
    J3DAnmTransformKeyTable* yInf = &mInfoTable[idx + 1];
    J3DAnmTransformKeyTable* zInf = &mInfoTable[idx + 2];
    switch (xInf->mScaleInfo.mMaxFrame) {
    case 0:
        transformInfo->mScale.x = 1;
        break;
    case 1:
        transformInfo->mScale.x = mScaleValues[xInf->mScaleInfo.mOffset];
        break;
    default:
        transformInfo->mScale.x = J2DGetKeyFrameInterpolationf(
            param_0, &xInf->mScaleInfo, &mScaleValues[xInf->mScaleInfo.mOffset]);
    }
    switch (yInf->mScaleInfo.mMaxFrame) {
    case 0:
        transformInfo->mScale.y = 1;
        break;
    case 1:
        transformInfo->mScale.y = mScaleValues[yInf->mScaleInfo.mOffset];
        break;
    default:
        transformInfo->mScale.y = J2DGetKeyFrameInterpolationf(
            param_0, &yInf->mScaleInfo, &mScaleValues[yInf->mScaleInfo.mOffset]);
    }
    switch (zInf->mScaleInfo.mMaxFrame) {
    case 0:
        transformInfo->mScale.z = 1;
        break;
    case 1:
        transformInfo->mScale.z = mScaleValues[zInf->mScaleInfo.mOffset];
        break;
    default:
        transformInfo->mScale.z = J2DGetKeyFrameInterpolationf(
            param_0, &zInf->mScaleInfo, &mScaleValues[zInf->mScaleInfo.mOffset]);
    }
    switch (xInf->mRotationInfo.mMaxFrame) {
    case 0:
        transformInfo->mRotation.x = 0;
        break;
    case 1:
        transformInfo->mRotation.x = mRotationValues[xInf->mRotationInfo.mOffset] << field_0x24;
        break;
    default:
        transformInfo->mRotation.x =
            static_cast<s32>(J2DGetKeyFrameInterpolations(
                param_0, &xInf->mRotationInfo, &mRotationValues[xInf->mRotationInfo.mOffset]))
            << field_0x24;
    }
    switch (yInf->mRotationInfo.mMaxFrame) {
    case 0:
        transformInfo->mRotation.y = 0;
        break;
    case 1:
        transformInfo->mRotation.y = mRotationValues[yInf->mRotationInfo.mOffset] << field_0x24;
        break;
    default:
        transformInfo->mRotation.y =
            static_cast<s32>(J2DGetKeyFrameInterpolations(
                param_0, &yInf->mRotationInfo, &mRotationValues[yInf->mRotationInfo.mOffset]))
            << field_0x24;
    }
    switch (zInf->mRotationInfo.mMaxFrame) {
    case 0:
        transformInfo->mRotation.z = 0;
        break;
    case 1:
        transformInfo->mRotation.z = mRotationValues[zInf->mRotationInfo.mOffset] << field_0x24;
        break;
    default:
        transformInfo->mRotation.z =
            static_cast<s32>(J2DGetKeyFrameInterpolations(
                param_0, &zInf->mRotationInfo, &mRotationValues[zInf->mRotationInfo.mOffset]))
            << field_0x24;
    }
    switch (xInf->mTranslateInfo.mMaxFrame) {
    case 0:
        transformInfo->mTranslate.x = 0;
        break;
    case 1:
        transformInfo->mTranslate.x = mTranslateValues[xInf->mTranslateInfo.mOffset];
        break;
    default:
        transformInfo->mTranslate.x = J2DGetKeyFrameInterpolationf(
            param_0, &xInf->mTranslateInfo, &mTranslateValues[xInf->mTranslateInfo.mOffset]);
    }
    switch (yInf->mTranslateInfo.mMaxFrame) {
    case 0:
        transformInfo->mTranslate.y = 0;
        break;
    case 1:
        transformInfo->mTranslate.y = mTranslateValues[yInf->mTranslateInfo.mOffset];
        break;
    default:
        transformInfo->mTranslate.y = J2DGetKeyFrameInterpolationf(
            param_0, &yInf->mTranslateInfo, &mTranslateValues[yInf->mTranslateInfo.mOffset]);
    }
    switch (zInf->mTranslateInfo.mMaxFrame) {
    case 0:
        transformInfo->mTranslate.z = 0;
        break;
    case 1:
        transformInfo->mTranslate.z = mTranslateValues[zInf->mTranslateInfo.mOffset];
        break;
    default:
        transformInfo->mTranslate.z = J2DGetKeyFrameInterpolationf(
            param_0, &zInf->mTranslateInfo, &mTranslateValues[zInf->mTranslateInfo.mOffset]);
    }
}

void J2DAnmColor::searchUpdateMaterialID(J2DScreen* pScreen) {
    if (pScreen != NULL && pScreen->mNameTable != NULL) {
        for (u16 entry = 0; entry < this->getUpdateMaterialNum(); entry++) {
            s32 idx = pScreen->mNameTable->getIndex(this->field_0x20.getName(entry));
            if (idx != -1) {
                mUpdateMaterialID[entry] = idx;
            } else {
                mUpdateMaterialID[entry] = 0xFFFF;
            }
        }
    }
}

void J2DAnmColorFull::getColor(u16 param_0, _GXColor* pColor) const {
    J3DAnmColorFullTable* info = &mInfoTable[param_0];
    u16 maxFrame = info->mRMaxFrame;
    if (getFrame() < 0) {
        pColor->r = mRValues[info->mROffset];
    } else if (getFrame() >= maxFrame) {
        pColor->r = mRValues[info->mROffset + (maxFrame - 1)];
    } else {
        pColor->r = mRValues[info->mROffset + (int)getFrame()];
    }
    maxFrame = info->mGMaxFrame;
    if (getFrame() < 0) {
        pColor->g = mGValues[info->mGOffset];
    } else if (getFrame() >= maxFrame) {
        pColor->g = mGValues[info->mGOffset + (maxFrame - 1)];
    } else {
        pColor->g = mGValues[info->mGOffset + (int)getFrame()];
    }
    maxFrame = info->mBMaxFrame;
    if (getFrame() < 0) {
        pColor->b = mBValues[info->mBOffset];
    } else if (getFrame() >= maxFrame) {
        pColor->b = mBValues[info->mBOffset + (maxFrame - 1)];
    } else {
        pColor->b = mBValues[info->mBOffset + (int)getFrame()];
    }
    maxFrame = info->mAMaxFrame;
    if (getFrame() < 0) {
        pColor->a = mAValues[info->mAOffset];
    } else if (getFrame() >= maxFrame) {
        pColor->a = mAValues[info->mAOffset + (maxFrame - 1)];
    } else {
        pColor->a = mAValues[info->mAOffset + (int)getFrame()];
    }
}

void J2DAnmColorKey::getColor(u16 param_0, _GXColor* pColor) const {
    J3DAnmColorKeyTable* info = &mInfoTable[param_0];
    f32 val;
    switch (info->mRInfo.mMaxFrame) {
    case 0:
        pColor->r = 0;
        break;
    case 1:
        pColor->r = mRValues[info->mRInfo.mOffset];
        break;
    default:
        val = J2DGetKeyFrameInterpolations(getFrame(), &info->mRInfo,
                                           &mRValues[info->mRInfo.mOffset]);
        if (val < 0) {
            pColor->r = 0;
        } else if (val > 255) {
            pColor->r = 255;
        } else {
            OSf32tou8(&val, &pColor->r);
        }
    }
    switch (info->mGInfo.mMaxFrame) {
    case 0:
        pColor->g = 0;
        break;
    case 1:
        pColor->g = mGValues[info->mGInfo.mOffset];
        break;
    default:
        val = J2DGetKeyFrameInterpolations(getFrame(), &info->mGInfo,
                                           &mGValues[info->mGInfo.mOffset]);
        if (val < 0) {
            pColor->g = 0;
        } else if (val > 255) {
            pColor->g = 255;
        } else {
            OSf32tou8(&val, &pColor->g);
        }
    }
    switch (info->mBInfo.mMaxFrame) {
    case 0:
        pColor->b = 0;
        break;
    case 1:
        pColor->b = mBValues[info->mBInfo.mOffset];
        break;
    default:
        val = J2DGetKeyFrameInterpolations(getFrame(), &info->mBInfo,
                                           &mBValues[info->mBInfo.mOffset]);
        if (val < 0) {
            pColor->b = 0;
        } else if (val > 255) {
            pColor->b = 255;
        } else {
            OSf32tou8(&val, &pColor->b);
        }
    }
    switch (info->mAInfo.mMaxFrame) {
    case 0:
        pColor->a = 0;
        break;
    case 1:
        pColor->a = mAValues[info->mAInfo.mOffset];
        break;
    default:
        val = J2DGetKeyFrameInterpolations(getFrame(), &info->mAInfo,
                                           &mAValues[info->mAInfo.mOffset]);
        if (val < 0) {
            pColor->a = 0;
        } else if (val > 255) {
            pColor->a = 255;
        } else {
            OSf32tou8(&val, &pColor->a);
        }
    }
}

void J2DAnmVtxColorFull::getColor(u8 param_0, u16 param_1, _GXColor* pColor) const {
    J3DAnmColorFullTable* info = &mInfoTable[param_0][param_1];
    u16 maxFrame = info->mRMaxFrame;
    if (getFrame() < 0) {
        pColor->r = mRValues[info->mROffset];
    } else if (getFrame() >= maxFrame) {
        pColor->r = mRValues[info->mROffset + (maxFrame - 1)];
    } else {
        pColor->r = mRValues[info->mROffset + (int)getFrame()];
    }
    maxFrame = info->mGMaxFrame;
    if (getFrame() < 0) {
        pColor->g = mGValues[info->mGOffset];
    } else if (getFrame() >= maxFrame) {
        pColor->g = mGValues[info->mGOffset + (maxFrame - 1)];
    } else {
        pColor->g = mGValues[info->mGOffset + (int)getFrame()];
    }
    maxFrame = info->mBMaxFrame;
    if (getFrame() < 0) {
        pColor->b = mBValues[info->mBOffset];
    } else if (getFrame() >= maxFrame) {
        pColor->b = mBValues[info->mBOffset + (maxFrame - 1)];
    } else {
        pColor->b = mBValues[info->mBOffset + (int)getFrame()];
    }
    maxFrame = info->mAMaxFrame;
    if (getFrame() < 0) {
        pColor->a = mAValues[info->mAOffset];
    } else if (getFrame() >= maxFrame) {
        pColor->a = mAValues[info->mAOffset + (maxFrame - 1)];
    } else {
        pColor->a = mAValues[info->mAOffset + (int)getFrame()];
    }
}

void J2DAnmVtxColorKey::getColor(u8 param_0, u16 param_1, _GXColor* pColor) const {
    J3DAnmColorKeyTable* info = &mInfoTable[param_0][param_1];
    f32 val;
    switch (info->mRInfo.mMaxFrame) {
    case 0:
        pColor->r = 0;
        break;
    case 1:
        pColor->r = mRValues[info->mRInfo.mOffset];
        break;
    default:
        val = J2DGetKeyFrameInterpolations(getFrame(), &info->mRInfo,
                                           &mRValues[info->mRInfo.mOffset]);
        if ((val <= 0)) {
            pColor->r = 0;
        } else if (val <= 255) {
            OSf32tou8(&val, &pColor->r);
        } else {
            pColor->r = 255;
        }
    }
    switch (info->mGInfo.mMaxFrame) {
    case 0:
        pColor->g = 0;
        break;
    case 1:
        pColor->g = mGValues[info->mGInfo.mOffset];
        break;
    default:
        val = J2DGetKeyFrameInterpolations(getFrame(), &info->mGInfo,
                                           &mGValues[info->mGInfo.mOffset]);
        if (val <= 0) {
            pColor->g = 0;
        } else if (val <= 255) {
            OSf32tou8(&val, &pColor->g);
        } else {
            pColor->g = 255;
        }
    }
    switch (info->mBInfo.mMaxFrame) {
    case 0:
        pColor->b = 0;
        break;
    case 1:
        pColor->b = mBValues[info->mBInfo.mOffset];
        break;
    default:
        val = J2DGetKeyFrameInterpolations(getFrame(), &info->mBInfo,
                                           &mBValues[info->mBInfo.mOffset]);
        if (val <= 0) {
            pColor->b = 0;
        } else if (val <= 255) {
            OSf32tou8(&val, &pColor->b);
        } else {
            pColor->b = 255;
        }
    }
    switch (info->mAInfo.mMaxFrame) {
    case 0:
        pColor->a = 0;
        break;
    case 1:
        pColor->a = mAValues[info->mAInfo.mOffset];
        break;
    default:
        val = J2DGetKeyFrameInterpolations(getFrame(), &info->mAInfo,
                                           &mAValues[info->mAInfo.mOffset]);
        if (val <= 0) {
            pColor->a = 0;
        } else if (val <= 255) {
            OSf32tou8(&val, &pColor->a);
        } else {
            pColor->a = 255;
        }
    }
}

void J2DAnmTextureSRTKey::calcTransform(f32 param_0, u16 param_1, J3DTextureSRTInfo* pInfo) const {
    u16 idx = param_1 * 3;
    J3DAnmTransformKeyTable* xInf = &mInfoTable[idx];
    J3DAnmTransformKeyTable* yInf = &mInfoTable[idx + 1];
    J3DAnmTransformKeyTable* zInf = &mInfoTable[idx + 2];
    switch (xInf->mScaleInfo.mMaxFrame) {
    case 0:
        pInfo->mScaleX = 1;
        break;
    case 1:
        pInfo->mScaleX = mScaleValues[xInf->mScaleInfo.mOffset];
        break;
    default:
        pInfo->mScaleX = J2DGetKeyFrameInterpolationf(param_0, &xInf->mScaleInfo,
                                                      &mScaleValues[xInf->mScaleInfo.mOffset]);
    }
    switch (yInf->mScaleInfo.mMaxFrame) {
    case 0:
        pInfo->mScaleY = 1;
        break;
    case 1:
        pInfo->mScaleY = mScaleValues[yInf->mScaleInfo.mOffset];
        break;
    default:
        pInfo->mScaleY = J2DGetKeyFrameInterpolationf(param_0, &yInf->mScaleInfo,
                                                      &mScaleValues[yInf->mScaleInfo.mOffset]);
    }
    switch (zInf->mRotationInfo.mMaxFrame) {
    case 0:
        pInfo->mRotation = 0;
        break;
    case 1:
        pInfo->mRotation = mRotationValues[zInf->mRotationInfo.mOffset] << field_0x10;
        break;
    default:
        pInfo->mRotation =
            static_cast<s32>(J2DGetKeyFrameInterpolations(
                param_0, &zInf->mRotationInfo, &mRotationValues[zInf->mRotationInfo.mOffset]))
            << field_0x10;
    }
    switch (xInf->mTranslateInfo.mMaxFrame) {
    case 0:
        pInfo->mTranslationX = 0;
        break;
    case 1:
        pInfo->mTranslationX = mTranslationValues[xInf->mTranslateInfo.mOffset];
        break;
    default:
        pInfo->mTranslationX = J2DGetKeyFrameInterpolationf(
            param_0, &xInf->mTranslateInfo, &mTranslationValues[xInf->mTranslateInfo.mOffset]);
    }
    switch (yInf->mTranslateInfo.mMaxFrame) {
    case 0:
        pInfo->mTranslationY = 0;
        break;
    case 1:
        pInfo->mTranslationY = mTranslationValues[yInf->mTranslateInfo.mOffset];
        break;
    default:
        pInfo->mTranslationY = J2DGetKeyFrameInterpolationf(
            param_0, &yInf->mTranslateInfo, &mTranslationValues[yInf->mTranslateInfo.mOffset]);
    }
}

void J2DAnmTextureSRTKey::searchUpdateMaterialID(J2DScreen* pScreen) {
    if (pScreen != NULL && pScreen->mNameTable != NULL) {
        for (u16 entry = 0; entry < this->getUpdateMaterialNum(); entry++) {
            s32 idx = pScreen->mNameTable->getIndex(field_0x34.getName(entry));
            if (idx != -1) {
                mUpdateMaterialID[entry] = idx;
            } else {
                mUpdateMaterialID[entry] = 0xFFFF;
            }
        }
    }
}

void J2DAnmTexPattern::searchUpdateMaterialID(J2DScreen* pScreen) {
    if (pScreen != NULL && pScreen->mNameTable != NULL && pScreen->mTexRes != NULL) {
        for (u16 entry = 0; entry < this->getUpdateMaterialNum(); entry++) {
            s32 idx = pScreen->mNameTable->getIndex(field_0x20.getName(entry));
            if (idx != -1) {
                mUpdateMaterialID[entry] = idx;
            } else {
                mUpdateMaterialID[entry] = 0xFFFF;
            }
        }
        delete[] mTIMGPtrArray;
        mTIMGPtrArray = new J2DAnmTexPatternTIMGPointer[pScreen->mTexRes->mCount];
        if (mTIMGPtrArray != NULL) {
            JUTResReference resRef;
            for (u16 i = 0; i < pScreen->mTexRes->mCount; i++) {
                s8* var1 = pScreen->mTexRes->getResReference(i);
                ResTIMG* var2 = NULL;
                if (var1 != NULL) {
                    var2 = (ResTIMG*)resRef.getResource(var1, 'TIMG', NULL);
                    if (var2 == NULL && J2DScreen::getDataManage() != NULL) {
                        var2 =
                            (ResTIMG*)J2DScreen::getDataManage()->get(pScreen->mTexRes->getName(i));
                    }
                }
                mTIMGPtrArray[i].mRes = var2;
                if (var2 != NULL && var2->indexTexture) {
                    JUTPalette* palette =
                        new JUTPalette(GX_TLUT0, (_GXTlutFmt)var2->colorFormat,
                                       (JUTTransparency)var2->alphaEnabled, var2->numColors,
                                       ((u8*)var2) + var2->paletteOffset);
                    mTIMGPtrArray[i].mPalette = palette;
                }
            }
        }
    }
}

void J2DAnmTexPattern::getTexNo(u16 param_0, u16* pTexNo) const {
    u16 maxFrame = mAnmTable[param_0].mMaxFrame;
    f32 frame = getFrame();
    if (frame < 0) {
        *pTexNo = mValues[mAnmTable[param_0].mOffset];
    } else if (frame >= maxFrame) {
        *pTexNo = mValues[mAnmTable[param_0].mOffset + (maxFrame - 1)];
    } else {
        *pTexNo = mValues[mAnmTable[param_0].mOffset + (int)frame];
    }
}

ResTIMG* J2DAnmTexPattern::getResTIMG(u16 param_0) const {
    if (mTIMGPtrArray == NULL) {
        return NULL;
    } else {
        u16 texNo;
        this->getTexNo(param_0, &texNo);
        return mTIMGPtrArray[texNo].mRes;
    }
}

JUTPalette* J2DAnmTexPattern::getPalette(u16 param_0) const {
    if (mTIMGPtrArray == NULL) {
        return NULL;
    } else {
        u16 texNo;
        this->getTexNo(param_0, &texNo);
        return mTIMGPtrArray[texNo].mPalette;
    }
}

void J2DAnmVisibilityFull::getVisibility(u16 param_0, u8* pOut) const {
    u16 maxFrame = mTable[param_0]._0;
    if (getFrame() < 0) {
        *pOut = mValues[mTable[param_0]._2];
    } else if (getFrame() >= maxFrame) {
        *pOut = mValues[mTable[param_0]._2 + (maxFrame - 1)];
    } else {
        int frame = getFrame();
        *pOut = mValues[mTable[param_0]._2 + frame];
    }
}

void J2DAnmTevRegKey::getTevColorReg(u16 param_0, _GXColorS10* pColor) const {
    J3DAnmCRegKeyTable* info = &mAnmCRegKeyTable[param_0];
    f32 val;
    switch (info->mRTable.mMaxFrame) {
    case 0:
        pColor->r = 0;
        break;
    case 1:
        pColor->r = mCRValues[info->mRTable.mOffset];
        break;
    default:
        val = J2DGetKeyFrameInterpolations(getFrame(), &info->mRTable,
                                           &mCRValues[info->mRTable.mOffset]);
        if (val < -0x400) {
            pColor->r = -0x400;
        } else if (val > 0x3FF) {
            pColor->r = 0x3FF;
        } else {
            OSf32tos16(&val, &pColor->r);
        }
    }
    switch (info->mGTable.mMaxFrame) {
    case 0:
        pColor->g = 0;
        break;
    case 1:
        pColor->g = mCGValues[info->mGTable.mOffset];
        break;
    default:
        val = J2DGetKeyFrameInterpolations(getFrame(), &info->mGTable,
                                           &mCGValues[info->mGTable.mOffset]);
        if (val < -0x400) {
            pColor->g = -0x400;
        } else if (val > 0x3FF) {
            pColor->g = 0x3FF;
        } else {
            OSf32tos16(&val, &pColor->g);
        }
    }
    switch (info->mBTable.mMaxFrame) {
    case 0:
        pColor->b = 0;
        break;
    case 1:
        pColor->b = mCBValues[info->mBTable.mOffset];
        break;
    default:
        val = J2DGetKeyFrameInterpolations(getFrame(), &info->mBTable,
                                           &mCBValues[info->mBTable.mOffset]);
        if (val < -0x400) {
            pColor->b = -0x400;
        } else if (val > 0x3FF) {
            pColor->b = 0x3FF;
        } else {
            OSf32tos16(&val, &pColor->b);
        }
    }
    switch (info->mATable.mMaxFrame) {
    case 0:
        pColor->a = 0;
        break;
    case 1:
        pColor->a = mCAValues[info->mATable.mOffset];
        break;
    default:
        val = J2DGetKeyFrameInterpolations(getFrame(), &info->mATable,
                                           &mCAValues[info->mATable.mOffset]);
        if (val < -0x400) {
            pColor->a = -0x400;
        } else if (val > 0x3FF) {
            pColor->a = 0x3FF;
        } else {
            OSf32tos16(&val, &pColor->a);
        }
    }
}

void J2DAnmTevRegKey::getTevKonstReg(u16 param_0, _GXColor* pColor) const {
    J3DAnmKRegKeyTable* info = &mAnmKRegKeyTable[param_0];
    f32 val;
    switch (info->mRTable.mMaxFrame) {
    case 0:
        pColor->r = 0;
        break;
    case 1:
        pColor->r = mKRValues[info->mRTable.mOffset];
        break;
    default:
        val = J2DGetKeyFrameInterpolations(getFrame(), &info->mRTable,
                                           &mKRValues[info->mRTable.mOffset]);
        if (val < 0) {
            pColor->r = 0;
        } else if (val > 255) {
            pColor->r = 255;
        } else {
            OSf32tou8(&val, &pColor->r);
        }
    }
    switch (info->mGTable.mMaxFrame) {
    case 0:
        pColor->g = 0;
        break;
    case 1:
        pColor->g = mKGValues[info->mGTable.mOffset];
        break;
    default:
        val = J2DGetKeyFrameInterpolations(getFrame(), &info->mGTable,
                                           &mKGValues[info->mGTable.mOffset]);
        if (val < 0) {
            pColor->g = 0;
        } else if (val > 255) {
            pColor->g = 255;
        } else {
            OSf32tou8(&val, &pColor->g);
        }
    }
    switch (info->mBTable.mMaxFrame) {
    case 0:
        pColor->b = 0;
        break;
    case 1:
        pColor->b = mKBValues[info->mBTable.mOffset];
        break;
    default:
        val = J2DGetKeyFrameInterpolations(getFrame(), &info->mBTable,
                                           &mKBValues[info->mBTable.mOffset]);
        if (val < 0) {
            pColor->b = 0;
        } else if (val > 255) {
            pColor->b = 255;
        } else {
            OSf32tou8(&val, &pColor->b);
        }
    }
    switch (info->mATable.mMaxFrame) {
    case 0:
        pColor->a = 0;
        break;
    case 1:
        pColor->a = mKAValues[info->mATable.mOffset];
        break;
    default:
        val = J2DGetKeyFrameInterpolations(getFrame(), &info->mATable,
                                           &mKAValues[info->mATable.mOffset]);
        if (val < 0) {
            pColor->a = 0;
        } else if (val > 255) {
            pColor->a = 255;
        } else {
            OSf32tou8(&val, &pColor->a);
        }
    }
}

void J2DAnmTevRegKey::searchUpdateMaterialID(J2DScreen* pScreen) {
    if (pScreen != NULL && pScreen->mNameTable != NULL) {
        for (u16 i = 0; i < mCRegUpdateMaterialNum; i++) {
            s32 idx = pScreen->mNameTable->getIndex(mCRegNameTab.getName(i));
            if (idx != -1) {
                mCRegUpdateMaterialID[i] = idx;
            } else {
                mCRegUpdateMaterialID[i] = 0xFFFF;
            }
        }
        for (u16 i = 0; i < mKRegUpdateMaterialNum; i++) {
            s32 idx = pScreen->mNameTable->getIndex(mKRegNameTab.getName(i));
            if (idx != -1) {
                mKRegUpdateMaterialID[i] = idx;
            } else {
                mKRegUpdateMaterialID[i] = 0xFFFF;
            }
        }
    }
}

// should be J2DGetKeyFrameInterpolation<s16>
f32 J2DGetKeyFrameInterpolations(f32 param_0, J3DAnmKeyTableBase* param_1, s16* param_2) {
    if (param_0 < param_2[0]) {
        return param_2[1];
    } else {
        if (param_1->mType == 0) {
            if (param_2[(param_1->mMaxFrame - 1) * 3] <= param_0) {
                return param_2[(param_1->mMaxFrame - 1) * 3 + 1];
            } else {
                u32 tmp = param_1->mMaxFrame;
                while (tmp > 1) {
                    u32 halfTmp = tmp / 2;
                    u32 upIdx = halfTmp * 3;
                    if (param_0 >= param_2[upIdx]) {
                        param_2 = param_2 + upIdx;
                        tmp -= halfTmp;
                    } else {
                        tmp = halfTmp;
                    }
                }
                return J2DHermiteInterpolation<s16>(param_0, &param_2[0], &param_2[1], &param_2[2],
                                                    &param_2[3], &param_2[4], &param_2[5]);
            }
        } else {
            if (param_2[(param_1->mMaxFrame - 1) * 4] <= param_0) {
                return param_2[(param_1->mMaxFrame - 1) * 4 + 1];
            } else {
                u32 tmp = param_1->mMaxFrame;
                while (tmp > 1) {
                    u32 halfTmp = tmp / 2;
                    u32 upIdx = halfTmp * 4;
                    if (param_0 >= param_2[upIdx]) {
                        param_2 = param_2 + upIdx;
                        tmp -= halfTmp;
                    } else {
                        tmp = halfTmp;
                    }
                }
                return J2DHermiteInterpolation<s16>(param_0, &param_2[0], &param_2[1], &param_2[3],
                                                    &param_2[4], &param_2[5], &param_2[6]);
            }
        }
    }
}

// should be J2DGetKeyFrameInterpolation<s16>
f32 J2DGetKeyFrameInterpolationf(f32 param_0, J3DAnmKeyTableBase* param_1, f32* param_2) {
    if (param_0 < param_2[0]) {
        return param_2[1];
    } else {
        if (param_1->mType == 0) {
            if (param_2[(param_1->mMaxFrame - 1) * 3] <= param_0) {
                return param_2[(param_1->mMaxFrame - 1) * 3 + 1];
            } else {
                u32 tmp = param_1->mMaxFrame;
                while (tmp > 1) {
                    u32 halfTmp = tmp / 2;
                    u32 upIdx = halfTmp * 3;
                    if (param_0 >= param_2[upIdx]) {
                        param_2 = param_2 + upIdx;
                        tmp -= halfTmp;
                    } else {
                        tmp = halfTmp;
                    }
                }
                return J2DHermiteInterpolation<f32>(param_0, &param_2[0], &param_2[1], &param_2[2],
                                                    &param_2[3], &param_2[4], &param_2[5]);
            }
        } else {
            if (param_2[(param_1->mMaxFrame - 1) * 4] <= param_0) {
                return param_2[(param_1->mMaxFrame - 1) * 4 + 1];
            } else {
                u32 tmp = param_1->mMaxFrame;
                while (tmp > 1) {
                    u32 halfTmp = tmp / 2;
                    u32 upIdx = halfTmp * 4;
                    if (param_0 >= param_2[upIdx]) {
                        param_2 = param_2 + upIdx;
                        tmp -= halfTmp;
                    } else {
                        tmp = halfTmp;
                    }
                }
                return J2DHermiteInterpolation<f32>(param_0, &param_2[0], &param_2[1], &param_2[3],
                                                    &param_2[4], &param_2[5], &param_2[6]);
            }
        }
    }
}

J2DAnmTexPattern::J2DAnmTexPatternTIMGPointer::J2DAnmTexPatternTIMGPointer() {
    mRes = NULL;
    mPalette = NULL;
}

J2DAnmTexPattern::J2DAnmTexPatternTIMGPointer::~J2DAnmTexPatternTIMGPointer() {
    delete mPalette;
}

static void dummyDelete(J2DAnmTexPattern* pattern) {
    delete pattern;
}
