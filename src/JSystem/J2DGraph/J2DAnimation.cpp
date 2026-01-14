#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DAnimation.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/J3DGraphBase/J3DTexture.h"
#include "JSystem/JUtility/JUTPalette.h"
#include "JSystem/JUtility/JUTResource.h"
#include "dolphin/os.h"

template<typename T>
f32 J2DGetKeyFrameInterpolation(f32 param_0, J3DAnmKeyTableBase* param_1, T* param_2) {
    J3D_PANIC(101, param_2, "Error : null pointer.");
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
                f32 result = J2DHermiteInterpolation(param_0, &param_2[0], &param_2[1], &param_2[2], &param_2[3], &param_2[4], &param_2[5]);
                return result;
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
                f32 result = J2DHermiteInterpolation(param_0, &param_2[0], &param_2[1], &param_2[3], &param_2[4], &param_2[5], &param_2[6]);
                return result;
            }
        }
    }
}

void J2DAnmTransformFull::getTransform(u16 param_0, J3DTransformInfo* transformInfo) const {
    u32 local_3c = mFrame;
    J3D_PANIC(169, param_0 < field_0x22, "Error : range over.");
    J3D_PANIC(170, transformInfo, "Error : null pointer.");
    u16 idx = (param_0 * 3);
    J3DAnmTransformFullTable* xPart = &mTableInfo[idx];
    J3DAnmTransformFullTable* yPart = &mTableInfo[idx + 1];
    J3DAnmTransformFullTable* zPart = &mTableInfo[idx + 2];
    u32 maxFrame = xPart->mScaleMaxFrame;
    if (mFrame < 0) {
        transformInfo->mScale.x = mScaleValues[xPart->mScaleOffset];
    } else if (mFrame >= maxFrame) {
        transformInfo->mScale.x = mScaleValues[(xPart->mScaleOffset + (maxFrame - 1))];
    } else {
        transformInfo->mScale.x = mScaleValues[(xPart->mScaleOffset + (int)mFrame)];
    }
    maxFrame = yPart->mScaleMaxFrame;
    if (mFrame < 0) {
        transformInfo->mScale.y = mScaleValues[yPart->mScaleOffset];
    } else if (mFrame >= maxFrame) {
        transformInfo->mScale.y = mScaleValues[(yPart->mScaleOffset + (maxFrame - 1))];
    } else {
        transformInfo->mScale.y = mScaleValues[(yPart->mScaleOffset + (int)mFrame)];
    }
    maxFrame = zPart->mScaleMaxFrame;
    if (mFrame < 0) {
        transformInfo->mScale.z = mScaleValues[zPart->mScaleOffset];
    } else if (mFrame >= maxFrame) {
        transformInfo->mScale.z = mScaleValues[(zPart->mScaleOffset + (maxFrame - 1))];
    } else {
        transformInfo->mScale.z = mScaleValues[(zPart->mScaleOffset + (int)mFrame)];
    }
    maxFrame = xPart->mRotationMaxFrame;
    if (mFrame < 0) {
        transformInfo->mRotation.x = mRotationValues[xPart->mRotationOffset];
    } else if (mFrame >= maxFrame) {
        transformInfo->mRotation.x = mRotationValues[(xPart->mRotationOffset + (maxFrame - 1))];
    } else {
        transformInfo->mRotation.x = mRotationValues[(xPart->mRotationOffset + (int)mFrame)];
    }
    maxFrame = yPart->mRotationMaxFrame;
    if (mFrame < 0) {
        transformInfo->mRotation.y = mRotationValues[yPart->mRotationOffset];
    } else if (mFrame >= maxFrame) {
        transformInfo->mRotation.y = mRotationValues[(yPart->mRotationOffset + (maxFrame - 1))];
    } else {
        transformInfo->mRotation.y = mRotationValues[(yPart->mRotationOffset + (int)mFrame)];
    }
    maxFrame = zPart->mRotationMaxFrame;
    if (mFrame < 0) {
        transformInfo->mRotation.z = mRotationValues[zPart->mRotationOffset];
    } else if (mFrame >= maxFrame) {
        transformInfo->mRotation.z = mRotationValues[(zPart->mRotationOffset + (maxFrame - 1))];
    } else {
        transformInfo->mRotation.z = mRotationValues[(zPart->mRotationOffset + (int)mFrame)];
    }
    maxFrame = xPart->mTranslateMaxFrame;
    if (mFrame < 0) {
        transformInfo->mTranslate.x = mTranslateValues[xPart->mTranslateOffset];
    } else if (mFrame >= maxFrame) {
        transformInfo->mTranslate.x = mTranslateValues[(xPart->mTranslateOffset + (maxFrame - 1))];
    } else {
        transformInfo->mTranslate.x = mTranslateValues[(xPart->mTranslateOffset + (int)mFrame)];
    }
    maxFrame = yPart->mTranslateMaxFrame;
    if (mFrame < 0) {
        transformInfo->mTranslate.y = mTranslateValues[yPart->mTranslateOffset];
    } else if (mFrame >= maxFrame) {
        transformInfo->mTranslate.y = mTranslateValues[(yPart->mTranslateOffset + (maxFrame - 1))];
    } else {
        transformInfo->mTranslate.y = mTranslateValues[(yPart->mTranslateOffset + (int)mFrame)];
    }
    maxFrame = zPart->mTranslateMaxFrame;
    if (mFrame < 0) {
        transformInfo->mTranslate.z = mTranslateValues[zPart->mTranslateOffset];
    } else if (mFrame >= maxFrame) {
        transformInfo->mTranslate.z = mTranslateValues[(zPart->mTranslateOffset + (maxFrame - 1))];
    } else {
        transformInfo->mTranslate.z = mTranslateValues[(zPart->mTranslateOffset + (int)mFrame)];
    }
}

void J2DAnmTransformKey::calcTransform(f32 param_0, u16 param_1,
                                       J3DTransformInfo* transformInfo) const {
    J3D_PANIC(268, param_1 < field_0x22, "Error : range over.");
    J3D_PANIC(269, transformInfo, "Error : null pointer.");
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
        transformInfo->mScale.x = J2DGetKeyFrameInterpolation<f32>(
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
        transformInfo->mScale.y = J2DGetKeyFrameInterpolation<f32>(
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
        transformInfo->mScale.z = J2DGetKeyFrameInterpolation<f32>(
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
            static_cast<s32>(J2DGetKeyFrameInterpolation<s16>(
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
            static_cast<s32>(J2DGetKeyFrameInterpolation<s16>(
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
            static_cast<s32>(J2DGetKeyFrameInterpolation<s16>(
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
        transformInfo->mTranslate.x = J2DGetKeyFrameInterpolation<f32>(
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
        transformInfo->mTranslate.y = J2DGetKeyFrameInterpolation<f32>(
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
        transformInfo->mTranslate.z = J2DGetKeyFrameInterpolation<f32>(
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
    J3D_PANIC(432, param_0 < mUpdateMaterialNum, "Error : range over.");
    J3D_PANIC(433, pColor, "Error : null pointer.");
    J3DAnmColorFullTable* info = &mInfoTable[param_0];
    u16 maxFrame = info->mRMaxFrame;
    if (mFrame < 0) {
        pColor->r = mRValues[info->mROffset];
    } else if (mFrame >= maxFrame) {
        pColor->r = mRValues[info->mROffset + (maxFrame - 1)];
    } else {
        pColor->r = mRValues[info->mROffset + (int)mFrame];
    }
    maxFrame = info->mGMaxFrame;
    if (mFrame < 0) {
        pColor->g = mGValues[info->mGOffset];
    } else if (mFrame >= maxFrame) {
        pColor->g = mGValues[info->mGOffset + (maxFrame - 1)];
    } else {
        pColor->g = mGValues[info->mGOffset + (int)mFrame];
    }
    maxFrame = info->mBMaxFrame;
    if (mFrame < 0) {
        pColor->b = mBValues[info->mBOffset];
    } else if (mFrame >= maxFrame) {
        pColor->b = mBValues[info->mBOffset + (maxFrame - 1)];
    } else {
        pColor->b = mBValues[info->mBOffset + (int)mFrame];
    }
    maxFrame = info->mAMaxFrame;
    if (mFrame < 0) {
        pColor->a = mAValues[info->mAOffset];
    } else if (mFrame >= maxFrame) {
        pColor->a = mAValues[info->mAOffset + (maxFrame - 1)];
    } else {
        pColor->a = mAValues[info->mAOffset + (int)mFrame];
    }
}

void J2DAnmColorKey::getColor(u16 param_0, _GXColor* pColor) const {
    J3D_PANIC(490, param_0 < mUpdateMaterialNum, "Error : range over.");
    J3D_PANIC(491, pColor, "Error : null pointer.");
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
        val = J2DGetKeyFrameInterpolation<s16>(mFrame, &info->mRInfo,
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
        val = J2DGetKeyFrameInterpolation<s16>(mFrame, &info->mGInfo,
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
        val = J2DGetKeyFrameInterpolation<s16>(mFrame, &info->mBInfo,
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
        val = J2DGetKeyFrameInterpolation<s16>(mFrame, &info->mAInfo,
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
    J3D_PANIC(597, pColor, "Error : null pointer.");
    J3DAnmColorFullTable* info = &mInfoTable[param_0][param_1];
    u16 maxFrame = info->mRMaxFrame;
    if (mFrame < 0) {
        pColor->r = mRValues[info->mROffset];
    } else if (mFrame >= maxFrame) {
        pColor->r = mRValues[info->mROffset + (maxFrame - 1)];
    } else {
        pColor->r = mRValues[info->mROffset + (int)mFrame];
    }
    maxFrame = info->mGMaxFrame;
    if (mFrame < 0) {
        pColor->g = mGValues[info->mGOffset];
    } else if (mFrame >= maxFrame) {
        pColor->g = mGValues[info->mGOffset + (maxFrame - 1)];
    } else {
        pColor->g = mGValues[info->mGOffset + (int)mFrame];
    }
    maxFrame = info->mBMaxFrame;
    if (mFrame < 0) {
        pColor->b = mBValues[info->mBOffset];
    } else if (mFrame >= maxFrame) {
        pColor->b = mBValues[info->mBOffset + (maxFrame - 1)];
    } else {
        pColor->b = mBValues[info->mBOffset + (int)mFrame];
    }
    maxFrame = info->mAMaxFrame;
    if (mFrame < 0) {
        pColor->a = mAValues[info->mAOffset];
    } else if (mFrame >= maxFrame) {
        pColor->a = mAValues[info->mAOffset + (maxFrame - 1)];
    } else {
        pColor->a = mAValues[info->mAOffset + (int)mFrame];
    }
}

void J2DAnmVtxColorKey::getColor(u8 param_0, u16 param_1, _GXColor* pColor) const {
    J3D_PANIC(658, pColor, "Error : null pointer.");
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
        val = J2DGetKeyFrameInterpolation<s16>(mFrame, &info->mRInfo,
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
        val = J2DGetKeyFrameInterpolation<s16>(mFrame, &info->mGInfo,
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
        val = J2DGetKeyFrameInterpolation<s16>(mFrame, &info->mBInfo,
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
        val = J2DGetKeyFrameInterpolation<s16>(mFrame, &info->mAInfo,
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
    J3D_PANIC(766, param_1 < mUpdateMaterialNum, "Error : range over.");
    J3D_PANIC(767, pInfo, "Error : null pointer.");
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
        pInfo->mScaleX = J2DGetKeyFrameInterpolation<f32>(param_0, &xInf->mScaleInfo,
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
        pInfo->mScaleY = J2DGetKeyFrameInterpolation<f32>(param_0, &yInf->mScaleInfo,
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
            static_cast<s32>(J2DGetKeyFrameInterpolation<s16>(
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
        pInfo->mTranslationX = J2DGetKeyFrameInterpolation<f32>(
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
        pInfo->mTranslationY = J2DGetKeyFrameInterpolation<f32>(
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
                char* var1 = pScreen->mTexRes->getResReference(i);
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
                    mTIMGPtrArray[i].mPalette = new JUTPalette(
                        GX_TLUT0,
                        GXTlutFmt(var2->colorFormat),
                        JUTTransparency(var2->alphaEnabled),
                        var2->numColors,
                        ((u8*)var2) + var2->paletteOffset
                    );
                }
            }
        }
    }
}

void J2DAnmTexPattern::getTexNo(u16 param_0, u16* pTexNo) const {
    u16 maxFrame = mAnmTable[param_0].mMaxFrame;
    J3D_PANIC(1024, param_0 < mUpdateMaterialNum, "Error : range over.");
    J3D_PANIC(1025, pTexNo, "Error : null pointer.");
    if (mFrame < 0) {
        *pTexNo = mValues[mAnmTable[param_0].mOffset];
    } else if (mFrame >= maxFrame) {
        *pTexNo = mValues[mAnmTable[param_0].mOffset + (maxFrame - 1)];
    } else {
        *pTexNo = mValues[mAnmTable[param_0].mOffset + (int)mFrame];
    }
}

ResTIMG* J2DAnmTexPattern::getResTIMG(u16 param_0) const {
    if (mTIMGPtrArray == NULL) {
        return NULL;
    }
    u16 maxFrame = mAnmTable[param_0].mMaxFrame;
    J3D_PANIC(1053, param_0 < mUpdateMaterialNum, "Error : range over.");

    u16 texNo;
    this->getTexNo(param_0, &texNo);
    return mTIMGPtrArray[texNo].mRes;
}

JUTPalette* J2DAnmTexPattern::getPalette(u16 param_0) const {
    if (mTIMGPtrArray == NULL) {
        return NULL;
    }
    u16 maxFrame = mAnmTable[param_0].mMaxFrame;
    J3D_PANIC(1076, param_0 < mUpdateMaterialNum, "Error : range over.");
    u16 texNo;
    this->getTexNo(param_0, &texNo);
    return mTIMGPtrArray[texNo].mPalette;
}

void J2DAnmVisibilityFull::getVisibility(u16 param_0, u8* pOut) const {
    u16 maxFrame = mTable[param_0]._0;
    J3D_PANIC(1094, param_0 < field_0x10, "Error : range over.");
    J3D_PANIC(1095, pOut, "Error : null pointer.");
    if (mFrame < 0) {
        *pOut = mValues[mTable[param_0]._2];
    } else if (mFrame >= maxFrame) {
        *pOut = mValues[mTable[param_0]._2 + (maxFrame - 1)];
    } else {
        *pOut = mValues[mTable[param_0]._2 + (int)mFrame];
    }
}

void J2DAnmTevRegKey::getTevColorReg(u16 param_0, GXColorS10* pColor) const {
    J3D_PANIC(1116, param_0 < mCRegUpdateMaterialNum, "Error : range over.");
    J3D_PANIC(1117, pColor, "Error : null pointer.");
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
        val = J2DGetKeyFrameInterpolation<s16>(mFrame, &info->mRTable,
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
        val = J2DGetKeyFrameInterpolation<s16>(mFrame, &info->mGTable,
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
        val = J2DGetKeyFrameInterpolation<s16>(mFrame, &info->mBTable,
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
        val = J2DGetKeyFrameInterpolation<s16>(mFrame, &info->mATable,
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

void J2DAnmTevRegKey::getTevKonstReg(u16 param_0, GXColor* pColor) const {
    J3D_PANIC(1222, param_0 < mKRegUpdateMaterialNum, "Error : range over.");
    J3D_PANIC(1223, pColor, "Error : null pointer.");
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
        val = J2DGetKeyFrameInterpolation<s16>(mFrame, &info->mRTable,
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
        val = J2DGetKeyFrameInterpolation<s16>(mFrame, &info->mGTable,
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
        val = J2DGetKeyFrameInterpolation<s16>(mFrame, &info->mBTable,
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
        val = J2DGetKeyFrameInterpolation<s16>(mFrame, &info->mATable,
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
        for (u16 i = 0; i < getCRegUpdateMaterialNum(); i++) {
            s32 idx = pScreen->mNameTable->getIndex(mCRegNameTab.getName(i));
            if (idx != -1) {
                mCRegUpdateMaterialID[i] = idx;
            } else {
                mCRegUpdateMaterialID[i] = 0xFFFF;
            }
        }
        for (u16 i = 0; i < getKRegUpdateMaterialNum(); i++) {
            s32 idx = pScreen->mNameTable->getIndex(mKRegNameTab.getName(i));
            if (idx != -1) {
                mKRegUpdateMaterialID[i] = idx;
            } else {
                mKRegUpdateMaterialID[i] = 0xFFFF;
            }
        }
    }
}
