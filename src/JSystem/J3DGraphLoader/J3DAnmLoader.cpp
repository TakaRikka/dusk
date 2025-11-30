//
// J3DAnmLoader
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphLoader/J3DAnmLoader.h"
#include "JSystem/JSupport/JSupport.h"
#include "dolphin/os.h"

J3DAnmBase* J3DAnmLoaderDataBase::load(const void* i_data, J3DAnmLoaderDataBaseFlag flag) {
    if (!i_data) {
        return NULL;
    }
    const JUTDataFileHeader* header = (const JUTDataFileHeader*)i_data;
    if (header->mMagic == 'J3D1') {
        switch (header->mType) {
        case 'bck1': {
            J3DAnmKeyLoader_v15 loader;
            loader.mAnm = new J3DAnmTransformKey();
            return loader.load(i_data);
        }
        case 'bpk1': {
            J3DAnmKeyLoader_v15 loader;
            loader.mAnm = new J3DAnmColorKey();
            return loader.load(i_data);
        }
        case 'blk1': {
            J3DAnmKeyLoader_v15 loader;
            loader.mAnm = new J3DAnmClusterKey();
            return loader.load(i_data);
        }
        case 'btk1': {
            J3DAnmKeyLoader_v15 loader;
            loader.mAnm = new J3DAnmTextureSRTKey();
            return loader.load(i_data);
        }
        case 'brk1': {
            J3DAnmKeyLoader_v15 loader;
            loader.mAnm = new J3DAnmTevRegKey();
            return loader.load(i_data);
        }
        case 'bxk1': {
            J3DAnmKeyLoader_v15 loader;
            loader.mAnm = new J3DAnmVtxColorKey();
            return loader.load(i_data);
        }
        case 'bca1': {
            J3DAnmFullLoader_v15 loader;

            if (flag & J3DLOADER_UNK_FLAG1) {
                loader.mAnm = new J3DAnmTransformFullWithLerp();
            } else {
                loader.mAnm = new J3DAnmTransformFull();
            }

            return loader.load(i_data);
        }
        case 'bpa1': {
            J3DAnmFullLoader_v15 loader;
            loader.mAnm = new J3DAnmColorFull();
            return loader.load(i_data);
        }
        case 'btp1': {
            J3DAnmFullLoader_v15 loader;
            loader.mAnm = new J3DAnmTexPattern();
            return loader.load(i_data);
        }
        case 'bva1': {
            J3DAnmFullLoader_v15 loader;
            loader.mAnm = new J3DAnmVisibilityFull();
            return loader.load(i_data);
        }
        case 'bla1': {
            J3DAnmFullLoader_v15 loader;
            loader.mAnm = new J3DAnmClusterFull();
            return loader.load(i_data);
        }
        case 'bxa1': {
            J3DAnmFullLoader_v15 loader;
            loader.mAnm = new J3DAnmVtxColorFull();
            return loader.load(i_data);
        }
        default:
            OSReport("J3DAnmLoader: this is not a J3D Binary.\n");
            return NULL;
        }
    }
    return NULL;
}

void J3DAnmLoaderDataBase::setResource(J3DAnmBase* param_1, const void* param_2) {
    const JUTDataFileHeader* header = (const JUTDataFileHeader*)param_2;
    if (!header) {
        return;
    }
    if (header->mMagic != 'J3D1') {
        return;
    }
    switch (header->mType) {
    case 'bck1': {
        J3DAnmKeyLoader_v15 loader;
        loader.setResource(param_1, param_2);
        break;
    }
    case 'bpk1': {
        J3DAnmKeyLoader_v15 loader;
        loader.setResource(param_1, param_2);
        break;
    }
    case 'blk1': {
        J3DAnmKeyLoader_v15 loader;
        loader.setResource(param_1, param_2);
        break;
    }
    case 'btk1': {
        J3DAnmKeyLoader_v15 loader;
        loader.setResource(param_1, param_2);
        break;
    }
    case 'brk1': {
        J3DAnmKeyLoader_v15 loader;
        if (param_1->getKind() == 5) {
            loader.setResource(param_1, param_2);
        }
        break;
    }
    case 'bxk1': {
        J3DAnmKeyLoader_v15 loader;
        loader.setResource(param_1, param_2);
        break;
    }
    case 'bca1': {
        J3DAnmFullLoader_v15 loader;
        loader.setResource(param_1, param_2);
        break;
    }
    case 'bpa1': {
        J3DAnmFullLoader_v15 loader;
        loader.setResource(param_1, param_2);
        break;
    }
    case 'btp1': {
        J3DAnmFullLoader_v15 loader;
        loader.setResource(param_1, param_2);
        break;
    }
    case 'bla1': {
        J3DAnmFullLoader_v15 loader;
        loader.setResource(param_1, param_2);
        break;
    }
    case 'bxa1': {
        J3DAnmFullLoader_v15 loader;
        loader.setResource(param_1, param_2);
        break;
    }
    case 'bva1': {
        J3DAnmFullLoader_v15 loader;
        loader.setResource(param_1, param_2);
        break;
    }
    default: {
        OSReport("J3DAnmLoader: this is not a J3D Binary.\n");
        break;
    }
    }
}

J3DAnmFullLoader_v15::J3DAnmFullLoader_v15() {}

J3DAnmFullLoader_v15::~J3DAnmFullLoader_v15() {}

J3DAnmKeyLoader_v15::J3DAnmKeyLoader_v15() {}

J3DAnmKeyLoader_v15::~J3DAnmKeyLoader_v15() {}

J3DAnmBase* J3DAnmFullLoader_v15::load(const void* param_1) {
    const JUTDataFileHeader* header = (const JUTDataFileHeader*)param_1;
    const JUTDataBlockHeader* block = &header->mFirstBlock;
    for (int i = 0; i < header->mBlockNum; i++) {
        switch (block->mType) {
        case 'ANF1':
            readAnmTransform((J3DAnmTransformFullData*)block);
            break;
        case 'PAF1':
            readAnmColor((J3DAnmColorFullData*)block);
            break;
        case 'TPT1':
            readAnmTexPattern((J3DAnmTexPatternFullData*)block);
            break;
        case 'CLF1':
            readAnmCluster((J3DAnmClusterFullData*)block);
            break;
        case 'VAF1':
            readAnmVisibility((J3DAnmVisibilityFullData*)block);
            break;
        case 'VCF1':
            readAnmVtxColor((J3DAnmVtxColorFullData*)block);
            break;
        default:
            OSReport("Unknown data block\n");
            break;
        }
        block = block->getNext();
    }
    return mAnm;
}

void J3DAnmFullLoader_v15::setResource(J3DAnmBase* param_1, const void* param_2) {
    const JUTDataFileHeader* header = (const JUTDataFileHeader*)param_2;
    const JUTDataBlockHeader* block = &header->mFirstBlock;
    for (int i = 0; i < header->mBlockNum; i++) {
        switch (block->mType) {
        case 'ANF1':
            setAnmTransform((J3DAnmTransformFull*)param_1, (const J3DAnmTransformFullData*)block);
            break;
        case 'PAF1':
            setAnmColor((J3DAnmColorFull*)param_1, (const J3DAnmColorFullData*)block);
            break;
        case 'TPT1':
            setAnmTexPattern((J3DAnmTexPattern*)param_1, (const J3DAnmTexPatternFullData*)block);
            break;
        case 'CLF1':
            setAnmCluster((J3DAnmClusterFull*)param_1, (const J3DAnmClusterFullData*)block);
            break;
        case 'VAF1':
            setAnmVisibility((J3DAnmVisibilityFull*)param_1,
                             (const J3DAnmVisibilityFullData*)block);
            break;
        case 'VCF1':
            setAnmVtxColor((J3DAnmVtxColorFull*)param_1, (const J3DAnmVtxColorFullData*)block);
            break;
        default:
            OSReport("Unknown data block\n");
            break;
        }
        block = block->getNext();
    }
}

void J3DAnmFullLoader_v15::readAnmTransform(const J3DAnmTransformFullData* param_1) {
    setAnmTransform((J3DAnmTransformFull*)mAnm, param_1);
}

void J3DAnmFullLoader_v15::setAnmTransform(J3DAnmTransformFull* param_1,
                                           const J3DAnmTransformFullData* param_2) {
    param_1->field_0x1e = param_2->field_0xc;
    param_1->mFrameMax = param_2->mFrameMax;
    param_1->mAttribute = param_2->field_0x8;
    param_1->mFrame = 0.0f;
    param_1->mAnmTable =
        JSUConvertOffsetToPtr<J3DAnmTransformFullTable>(param_2, param_2->mTableOffset);
    param_1->mScaleData = JSUConvertOffsetToPtr<f32>(param_2, param_2->mScaleValOffset);
    param_1->mRotData = JSUConvertOffsetToPtr<s16>(param_2, param_2->mRotValOffset);
    param_1->mTransData = JSUConvertOffsetToPtr<f32>(param_2, param_2->mTransValOffset);
}


void J3DAnmFullLoader_v15::readAnmColor(const J3DAnmColorFullData* param_1) {
    setAnmColor((J3DAnmColorFull*)mAnm, param_1);
}

void J3DAnmFullLoader_v15::setAnmColor(J3DAnmColorFull* param_1,
                                       const J3DAnmColorFullData* param_2) {
    param_1->mFrameMax = param_2->mFrameMax;
    param_1->mAttribute = param_2->field_0x8;
    param_1->mFrame = 0.0f;
    param_1->mUpdateMaterialNum = param_2->mUpdateMaterialNum;
    param_1->mAnmTable =
        JSUConvertOffsetToPtr<J3DAnmColorFullTable>(param_2, param_2->mTableOffset);
    param_1->mColorR = JSUConvertOffsetToPtr<u8>(param_2, param_2->mRValuesOffset);
    param_1->mColorG = JSUConvertOffsetToPtr<u8>(param_2, param_2->mGValuesOffset);
    param_1->mColorB = JSUConvertOffsetToPtr<u8>(param_2, param_2->mBValuesOffset);
    param_1->mColorA = JSUConvertOffsetToPtr<u8>(param_2, param_2->mAValuesOffset);
    param_1->mUpdateMaterialID =
        JSUConvertOffsetToPtr<u16>(param_2, param_2->mUpdateMaterialIDOffset);
    param_1->mUpdateMaterialName.setResource(
        JSUConvertOffsetToPtr<ResNTAB>(param_2, param_2->mNameTabOffset));
}

void J3DAnmFullLoader_v15::readAnmTexPattern(const J3DAnmTexPatternFullData* param_1) {
    setAnmTexPattern((J3DAnmTexPattern*)mAnm, param_1);
}

void J3DAnmFullLoader_v15::setAnmTexPattern(J3DAnmTexPattern* param_1,
                                            const J3DAnmTexPatternFullData* param_2) {
    param_1->mFrameMax = param_2->mFrameMax;
    param_1->mAttribute = param_2->field_0x8;
    param_1->mFrame = 0.0f;
    param_1->mUpdateMaterialNum = param_2->field_0xc;
    param_1->field_0x14 = param_2->field_0xe;
    param_1->mAnmTable =
        JSUConvertOffsetToPtr<J3DAnmTexPatternFullTable>(param_2, param_2->mTableOffset);
    param_1->mTextureIndex = JSUConvertOffsetToPtr<u16>(param_2, param_2->mValuesOffset);
    param_1->mUpdateMaterialID =
        JSUConvertOffsetToPtr<u16>(param_2, param_2->mUpdateMaterialIDOffset);
    param_1->mUpdateMaterialName.setResource(
        JSUConvertOffsetToPtr<ResNTAB>(param_2, param_2->mNameTabOffset));
}

void J3DAnmFullLoader_v15::readAnmVisibility(const J3DAnmVisibilityFullData* param_1) {
    setAnmVisibility((J3DAnmVisibilityFull*)mAnm, param_1);
}

void J3DAnmFullLoader_v15::setAnmVisibility(J3DAnmVisibilityFull* param_1,
                                            const J3DAnmVisibilityFullData* param_2) {
    param_1->mFrameMax = param_2->mFrameMax;
    param_1->mAttribute = param_2->field_0x8;
    param_1->mFrame = 0.0f;
    param_1->mUpdateMaterialNum = param_2->field_0xc;
    param_1->field_0xe = param_2->field_0xe;
    param_1->mAnmTable =
        JSUConvertOffsetToPtr<J3DAnmVisibilityFullTable>(param_2, param_2->mTableOffset);
    param_1->mVisibility = JSUConvertOffsetToPtr<u8>(param_2, param_2->mValuesOffset);
}

void J3DAnmFullLoader_v15::readAnmCluster(const J3DAnmClusterFullData* param_1) {
    setAnmCluster((J3DAnmClusterFull*)mAnm, param_1);
}

void J3DAnmFullLoader_v15::setAnmCluster(J3DAnmClusterFull* param_1,
                                         const J3DAnmClusterFullData* param_2) {
    param_1->mFrameMax = param_2->mFrameMax;
    param_1->mAttribute = param_2->field_0x8;
    param_1->mFrame = 0.0f;
    param_1->mAnmTable =
        JSUConvertOffsetToPtr<J3DAnmClusterFullTable>(param_2, param_2->mTableOffset);
    param_1->mWeight = JSUConvertOffsetToPtr<f32>(param_2, param_2->mWeightOffset);
}

void J3DAnmFullLoader_v15::readAnmVtxColor(const J3DAnmVtxColorFullData* param_1) {
    setAnmVtxColor((J3DAnmVtxColorFull*)mAnm, param_1);
}

void J3DAnmFullLoader_v15::setAnmVtxColor(J3DAnmVtxColorFull* dst,
                                          const J3DAnmVtxColorFullData* data) {
    dst->mFrameMax = data->mFrameMax;
    dst->mAttribute = data->field_0x8;
    dst->mFrame = 0.0f;
    dst->mAnmTableNum[0] = data->mAnmTableNum[0];
    dst->mAnmTableNum[1] = data->mAnmTableNum[1];
    dst->mpTable[0] =
        JSUConvertOffsetToPtr<J3DAnmColorFullTable>(data, data->mTableOffsets[0]);
    dst->mpTable[1] =
        JSUConvertOffsetToPtr<J3DAnmColorFullTable>(data, data->mTableOffsets[1]);
    dst->mAnmVtxColorIndexData[0] = JSUConvertOffsetToPtr<J3DAnmVtxColorIndexData>(
        data, data->mVtxColorIndexDataOffsets[0]);
    dst->mAnmVtxColorIndexData[1] = JSUConvertOffsetToPtr<J3DAnmVtxColorIndexData>(
        data, data->mVtxColorIndexDataOffsets[1]);
    void* indexPtr0 =
        JSUConvertOffsetToPtr<u16>(data, data->mVtxColorIndexPointerOffsets[0]);
    void* indexPtr1 =
        JSUConvertOffsetToPtr<u16>(data, data->mVtxColorIndexPointerOffsets[1]);

    for (s32 i = 0; i < dst->mAnmTableNum[0]; i++)
        dst->mAnmVtxColorIndexData[0][i].mpData =
            (void*)((s32)indexPtr0 + (s32)dst->mAnmVtxColorIndexData[0][i].mpData * 2);
    for (s32 i = 0; i < dst->mAnmTableNum[1]; i++)
        dst->mAnmVtxColorIndexData[1][i].mpData =
            (void*)((s32)indexPtr1 + (s32)dst->mAnmVtxColorIndexData[1][i].mpData * 2);

    dst->mColorR = JSUConvertOffsetToPtr<u8>(data, data->mRValuesOffset);
    dst->mColorG = JSUConvertOffsetToPtr<u8>(data, data->mGValuesOffset);
    dst->mColorB = JSUConvertOffsetToPtr<u8>(data, data->mBValuesOffset);
    dst->mColorA = JSUConvertOffsetToPtr<u8>(data, data->mAValuesOffset);
}

J3DAnmBase* J3DAnmKeyLoader_v15::load(const void* param_1) {
    const JUTDataFileHeader* header = (const JUTDataFileHeader*)param_1;
    const JUTDataBlockHeader* block = &header->mFirstBlock;
    for (int i = 0; i < header->mBlockNum; i++) {
        switch (block->mType) {
        case 'ANK1':
            readAnmTransform((J3DAnmTransformKeyData*)block);
            break;
        case 'PAK1':
            readAnmColor((J3DAnmColorKeyData*)block);
            break;
        case 'CLK1':
            readAnmCluster((J3DAnmClusterKeyData*)block);
            break;
        case 'TTK1':
            readAnmTextureSRT((J3DAnmTextureSRTKeyData*)block);
            break;
        case 'TRK1':
            readAnmTevReg((J3DAnmTevRegKeyData*)block);
            break;
        case 'VCK1':
            readAnmVtxColor((J3DAnmVtxColorKeyData*)block);
            break;
        default:
            OSReport("Unknown data block\n");
            break;
        }
        block = block->getNext();
    }
    return mAnm;
}

void J3DAnmKeyLoader_v15::setResource(J3DAnmBase* param_1, const void* param_2) {
    const JUTDataFileHeader* header = (const JUTDataFileHeader*)param_2;
    const JUTDataBlockHeader* block = &header->mFirstBlock;
    for (int i = 0; i < header->mBlockNum; i++) {
        switch (block->mType) {
        case 'ANK1':
            setAnmTransform((J3DAnmTransformKey*)param_1, (const J3DAnmTransformKeyData*)block);
            break;
        case 'PAK1':
            setAnmColor((J3DAnmColorKey*)param_1, (const J3DAnmColorKeyData*)block);
            break;
        case 'CLK1':
            setAnmCluster((J3DAnmClusterKey*)param_1, (const J3DAnmClusterKeyData*)block);
            break;
        case 'TTK1':
            setAnmTextureSRT((J3DAnmTextureSRTKey*)param_1, (const J3DAnmTextureSRTKeyData*)block);
            break;
        case 'TRK1':
            setAnmTevReg((J3DAnmTevRegKey*)param_1, (const J3DAnmTevRegKeyData*)block);
            break;
        case 'VCK1':
            setAnmVtxColor((J3DAnmVtxColorKey*)param_1, (const J3DAnmVtxColorKeyData*)block);
            break;
        default:
            OSReport("Unknown data block\n");
            break;
        }
        block = block->getNext();
    }
}

void J3DAnmKeyLoader_v15::readAnmTransform(const J3DAnmTransformKeyData* param_1) {
    setAnmTransform((J3DAnmTransformKey*)mAnm, param_1);
}

void J3DAnmKeyLoader_v15::setAnmTransform(J3DAnmTransformKey* param_1,
                                          const J3DAnmTransformKeyData* param_2) {
    param_1->field_0x1e = param_2->field_0xc;
    param_1->mFrameMax = param_2->mFrameMax;
    param_1->mAttribute = param_2->field_0x8;
    param_1->mDecShift = param_2->field_0x9;
    param_1->mFrame = 0.0f;
    param_1->mAnmTable =
        JSUConvertOffsetToPtr<J3DAnmTransformKeyTable>(param_2, param_2->mTableOffset);
    param_1->mScaleData = JSUConvertOffsetToPtr<f32>(param_2, param_2->field_0x18);
    param_1->mRotData = JSUConvertOffsetToPtr<s16>(param_2, param_2->field_0x1c);
    param_1->mTransData = JSUConvertOffsetToPtr<f32>(param_2, param_2->field_0x20);
}


void J3DAnmKeyLoader_v15::readAnmTextureSRT(const J3DAnmTextureSRTKeyData* param_1) {
    setAnmTextureSRT((J3DAnmTextureSRTKey*)mAnm, param_1);
}

void J3DAnmKeyLoader_v15::setAnmTextureSRT(J3DAnmTextureSRTKey* param_1,
                                           const J3DAnmTextureSRTKeyData* param_2) {
    param_1->mTrackNum = param_2->field_0xc;
    param_1->mFrameMax = param_2->field_0xa;
    param_1->mAttribute = param_2->field_0x8;
    param_1->mDecShift = param_2->field_0x9;
    param_1->mFrame = 0.0f;
    param_1->mTrackNum = param_2->field_0xc;
    param_1->mScaleNum = param_2->field_0xe;
    param_1->mRotNum = param_2->field_0x10;
    param_1->mTransNum = param_2->field_0x12;
    param_1->mAnmTable =
        JSUConvertOffsetToPtr<J3DAnmTransformKeyTable>(param_2, param_2->mTableOffset);
    param_1->mUpdateMaterialID =
        JSUConvertOffsetToPtr<u16>(param_2, param_2->mUpdateMatIDOffset);
    param_1->mUpdateMaterialName.setResource(
        JSUConvertOffsetToPtr<ResNTAB>(param_2, param_2->mNameTab1Offset));
    param_1->mUpdateTexMtxID =
        JSUConvertOffsetToPtr<u8>(param_2, param_2->mUpdateTexMtxIDOffset);
    param_1->mSRTCenter = JSUConvertOffsetToPtr<Vec>(param_2, param_2->unkOffset);
    param_1->mScaleData = JSUConvertOffsetToPtr<f32>(param_2, param_2->mScaleValOffset);
    param_1->mRotData = JSUConvertOffsetToPtr<s16>(param_2, param_2->mRotValOffset);
    param_1->mTransData = JSUConvertOffsetToPtr<f32>(param_2, param_2->mTransValOffset);
    if (param_2->mNameTab2Offset) {
        param_1->mPostUpdateMaterialName.setResource(
            JSUConvertOffsetToPtr<ResNTAB>(param_2, param_2->mNameTab2Offset));
    }
    param_1->field_0x4a = param_2->field_0x34;
    param_1->field_0x44 = param_2->field_0x36;
    param_1->field_0x46 = param_2->field_0x38;
    param_1->field_0x48 = param_2->field_0x3a;
    param_1->field_0x58 =
        JSUConvertOffsetToPtr<J3DAnmTransformKeyTable>(param_2, param_2->mInfoTable2Offset);
    param_1->mPostUpdateMaterialID = JSUConvertOffsetToPtr<u16>(param_2, param_2->field_0x40);
    param_1->mPostUpdateTexMtxID = JSUConvertOffsetToPtr<u8>(param_2, param_2->field_0x48);
    param_1->mPostSRTCenter = JSUConvertOffsetToPtr<Vec>(param_2, param_2->field_0x4c);
    param_1->field_0x4c = JSUConvertOffsetToPtr<f32>(param_2, param_2->field_0x50);
    param_1->field_0x50 = JSUConvertOffsetToPtr<s16>(param_2, param_2->field_0x54);
    param_1->field_0x54 = JSUConvertOffsetToPtr<f32>(param_2, param_2->field_0x58);
    switch (param_2->field_0x5c) {
    case 0:
    case 1:
        param_1->mTexMtxCalcType = param_2->field_0x5c;
        break;
    default:
        param_1->mTexMtxCalcType = 0;
        break;
    }
}


void J3DAnmKeyLoader_v15::readAnmColor(const J3DAnmColorKeyData* param_1) {
    setAnmColor((J3DAnmColorKey*)mAnm, param_1);
}

void J3DAnmKeyLoader_v15::setAnmColor(J3DAnmColorKey* param_1, const J3DAnmColorKeyData* param_2) {
    param_1->mFrameMax = param_2->mFrameMax;
    param_1->mAttribute = param_2->field_0x8;
    param_1->mFrame = 0.0f;
    param_1->mUpdateMaterialNum = param_2->mUpdateMaterialNum;
    param_1->field_0xc = param_2->field_0x10;
    param_1->field_0xe = param_2->field_0x12;
    param_1->field_0x10 = param_2->field_0x14;
    param_1->field_0x12 = param_2->field_0x16;
    param_1->mAnmTable =
        JSUConvertOffsetToPtr<J3DAnmColorKeyTable>(param_2, param_2->mTableOffset);
    param_1->mColorR = JSUConvertOffsetToPtr<s16>(param_2, param_2->mRValOffset);
    param_1->mColorG = JSUConvertOffsetToPtr<s16>(param_2, param_2->mGValOffset);
    param_1->mColorB = JSUConvertOffsetToPtr<s16>(param_2, param_2->mBValOffset);
    param_1->mColorA = JSUConvertOffsetToPtr<s16>(param_2, param_2->mAValOffset);
    param_1->mUpdateMaterialID =
        JSUConvertOffsetToPtr<u16>(param_2, param_2->mUpdateMaterialIDOffset);
    param_1->mUpdateMaterialName.setResource(
        JSUConvertOffsetToPtr<ResNTAB>(param_2, param_2->mNameTabOffset));
}


void J3DAnmKeyLoader_v15::readAnmCluster(const J3DAnmClusterKeyData* param_1) {
    setAnmCluster((J3DAnmClusterKey*)mAnm, param_1);
}

void J3DAnmKeyLoader_v15::setAnmCluster(J3DAnmClusterKey* param_1,
                                        const J3DAnmClusterKeyData* param_2) {
    param_1->mFrameMax = param_2->mFrameMax;
    param_1->mAttribute = param_2->field_0x8;
    param_1->mFrame = 0.0f;
    param_1->mAnmTable =
        JSUConvertOffsetToPtr<J3DAnmClusterKeyTable>(param_2, param_2->mTableOffset);
    param_1->mWeight = JSUConvertOffsetToPtr<f32>(param_2, param_2->mWeightOffset);
}


void J3DAnmKeyLoader_v15::readAnmTevReg(const J3DAnmTevRegKeyData* param_1) {
    setAnmTevReg((J3DAnmTevRegKey*)mAnm, param_1);
}

void J3DAnmKeyLoader_v15::setAnmTevReg(J3DAnmTevRegKey* param_1,
                                       const J3DAnmTevRegKeyData* param_2) {
    param_1->mFrameMax = param_2->mFrameMax;
    param_1->mAttribute = param_2->field_0x8;
    param_1->mFrame = 0.0f;
    param_1->mCRegUpdateMaterialNum = param_2->mCRegUpdateMaterialNum;
    param_1->mAnmCRegKeyTable =
        JSUConvertOffsetToPtr<J3DAnmCRegKeyTable>(param_2, param_2->mCRegTableOffset);
    param_1->mCRegUpdateMaterialID =
        JSUConvertOffsetToPtr<u16>(param_2, param_2->mCRegUpdateMaterialIDOffset);
    param_1->mCRegUpdateMaterialName.setResource(
        JSUConvertOffsetToPtr<ResNTAB>(param_2, param_2->mCRegNameTabOffset));
    param_1->mKRegUpdateMaterialNum = param_2->mKRegUpdateMaterialNum;
    param_1->mAnmKRegKeyTable =
        JSUConvertOffsetToPtr<J3DAnmKRegKeyTable>(param_2, param_2->mKRegTableOffset);
    param_1->mKRegUpdateMaterialID =
        JSUConvertOffsetToPtr<u16>(param_2, param_2->mKRegUpdateMaterialIDOffset);
    param_1->mKRegUpdateMaterialName.setResource(
        JSUConvertOffsetToPtr<ResNTAB>(param_2, param_2->mKRegNameTabOffset));
    param_1->mCRegDataCountR = param_2->field_0x10;
    param_1->mCRegDataCountG = param_2->field_0x12;
    param_1->mCRegDataCountB = param_2->field_0x14;
    param_1->mCRegDataCountA = param_2->field_0x16;
    param_1->mAnmCRegDataR = JSUConvertOffsetToPtr<s16>(param_2, param_2->mCRValuesOffset);
    param_1->mAnmCRegDataG = JSUConvertOffsetToPtr<s16>(param_2, param_2->mCGValuesOffset);
    param_1->mAnmCRegDataB = JSUConvertOffsetToPtr<s16>(param_2, param_2->mCBValuesOffset);
    param_1->mAnmCRegDataA = JSUConvertOffsetToPtr<s16>(param_2, param_2->mCAValuesOffset);
    param_1->mKRegDataCountR = param_2->field_0x18;
    param_1->mKRegDataCountG = param_2->field_0x1a;
    param_1->mKRegDataCountB = param_2->field_0x1c;
    param_1->mKRegDataCountA = param_2->field_0x1e;
    param_1->mAnmKRegDataR = JSUConvertOffsetToPtr<s16>(param_2, param_2->mKRValuesOffset);
    param_1->mAnmKRegDataG = JSUConvertOffsetToPtr<s16>(param_2, param_2->mKGValuesOffset);
    param_1->mAnmKRegDataB = JSUConvertOffsetToPtr<s16>(param_2, param_2->mKBValuesOffset);
    param_1->mAnmKRegDataA = JSUConvertOffsetToPtr<s16>(param_2, param_2->mKAValuesOffset);
}


void J3DAnmKeyLoader_v15::readAnmVtxColor(const J3DAnmVtxColorKeyData* param_1) {
    setAnmVtxColor((J3DAnmVtxColorKey*)mAnm, param_1);
}

void J3DAnmKeyLoader_v15::setAnmVtxColor(J3DAnmVtxColorKey* dst,
                                         const J3DAnmVtxColorKeyData* data) {
    dst->mFrameMax = data->mFrameMax;
    dst->mAttribute = data->field_0x8;
    dst->mFrame = 0.0f;
    dst->mAnmTableNum[0] = data->mAnmTableNum[0];
    dst->mAnmTableNum[1] = data->mAnmTableNum[1];
    dst->mpTable[0] =
        JSUConvertOffsetToPtr<J3DAnmColorKeyTable>(data, data->mTableOffsets[0]);
    dst->mpTable[1] =
        JSUConvertOffsetToPtr<J3DAnmColorKeyTable>(data, data->mTableOffsets[1]);
    dst->mAnmVtxColorIndexData[0] = JSUConvertOffsetToPtr<J3DAnmVtxColorIndexData>(
        data, data->mVtxColoIndexDataOffset[0]);
    dst->mAnmVtxColorIndexData[1] = JSUConvertOffsetToPtr<J3DAnmVtxColorIndexData>(
        data, data->mVtxColoIndexDataOffset[1]);
    void* indexPtr0 = JSUConvertOffsetToPtr<u16>(data, data->mVtxColoIndexPointerOffset[0]);
    void* indexPtr1 = JSUConvertOffsetToPtr<u16>(data, data->mVtxColoIndexPointerOffset[1]);

    for (s32 i = 0; i < dst->mAnmTableNum[0]; i++)
        dst->mAnmVtxColorIndexData[0][i].mpData =
            (void*)((s32)indexPtr0 + (s32)dst->mAnmVtxColorIndexData[0][i].mpData * 2);
    for (s32 i = 0; i < dst->mAnmTableNum[1]; i++)
        dst->mAnmVtxColorIndexData[1][i].mpData =
            (void*)((s32)indexPtr1 + (s32)dst->mAnmVtxColorIndexData[1][i].mpData * 2);

    dst->mColorR = JSUConvertOffsetToPtr<s16>(data, data->mRValOffset);
    dst->mColorG = JSUConvertOffsetToPtr<s16>(data, data->mGValOffset);
    dst->mColorB = JSUConvertOffsetToPtr<s16>(data, data->mBValOffset);
    dst->mColorA = JSUConvertOffsetToPtr<s16>(data, data->mAValOffset);
}
