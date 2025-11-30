#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTTexture.h"
#include "JSystem/JUtility/JUTPalette.h"
#include "dolphin/gx.h"

JUTTexture::~JUTTexture() {
    if (getCaptureFlag()) {
        delete[] field_0x3c;
    }
    if (getEmbPaletteDelFlag()) {
        delete mEmbPalette;
    }
}

void JUTTexture::storeTIMG(ResTIMG const* param_0, u8 param_1) {
    _GXTlut tlut;

    if (param_0 && param_1 < 0x10) {
        mTexInfo = param_0;
        mTexData = (void*)((int)mTexInfo + mTexInfo->imageOffset);

        if (mTexInfo->imageOffset == 0) {
            mTexData = (void*)((int)mTexInfo + 0x20);
        }

        field_0x2c = NULL;
        mTlutName = 0;
        mWrapS = mTexInfo->wrapS;
        mWrapT = mTexInfo->wrapT;
        mMinFilter = mTexInfo->minFilter;
        mMagFilter = mTexInfo->magFilter;
        mMinLOD = (s8)mTexInfo->minLOD;
        mMaxLOD = (s8)mTexInfo->maxLOD;
        mLODBias = mTexInfo->LODBias;

        if (mTexInfo->numColors == 0) {
            initTexObj();
        } else {
            if (mTexInfo->numColors > 0x100) {
                tlut = (_GXTlut)((param_1 % 4) + GX_BIGTLUT0);
            } else {
                tlut = (_GXTlut)param_1;
            }

            if (mEmbPalette == NULL || !getEmbPaletteDelFlag()) {
                mEmbPalette = new JUTPalette(tlut, (_GXTlutFmt)mTexInfo->colorFormat,
                                             (JUTTransparency)mTexInfo->alphaEnabled, mTexInfo->numColors,
                                             (void*)(&mTexInfo->format + mTexInfo->paletteOffset));
                mFlags = mFlags & 1 | 2;
            } else {
                mEmbPalette->storeTLUT(tlut, (_GXTlutFmt)mTexInfo->colorFormat,
                                       (JUTTransparency)mTexInfo->alphaEnabled, mTexInfo->numColors,
                                       (void*)(&mTexInfo->format + mTexInfo->paletteOffset));
            }
            attachPalette(mEmbPalette);
        }
    }
}

void JUTTexture::storeTIMG(ResTIMG const* param_0, JUTPalette* param_1) {
    _GXTlut type;

    if (param_1 != NULL) {
        type = (_GXTlut)param_1->getTlutName();
    } else {
        type = GX_TLUT0;
    }
    storeTIMG(param_0, param_1, type);
}

void JUTTexture::storeTIMG(ResTIMG const* param_0, JUTPalette* param_1, _GXTlut param_2) {
    _GXTlut type;

    if (param_0 == NULL) {
        return;
    }
    mTexInfo = param_0;
    mTexData = ((u8*)mTexInfo) + mTexInfo->imageOffset;
    if (mTexInfo->imageOffset == 0) {
        mTexData = ((u8*)mTexInfo) + sizeof(ResTIMG);
    }
    if (getEmbPaletteDelFlag()) {
            delete mEmbPalette;
    }
    mEmbPalette = param_1;
    setEmbPaletteDelFlag(false);
    field_0x2c = NULL;
    if (param_1 != NULL) {
        mTlutName = param_2;
        if (param_2 != param_1->getTlutName()) {
            u8 format = param_1->getFormat();
            u8 transperancy = param_1->getTransparency();
            u16 numColors = param_1->getNumColors();
            ResTLUT* colorTable = param_1->getColorTable();
            param_1->storeTLUT(param_2, (_GXTlutFmt)format,
                            (JUTTransparency)transperancy,
                            numColors, colorTable);
        }
    }

    mWrapS = mTexInfo->wrapS;
    mWrapT = mTexInfo->wrapT;
    mMinFilter = mTexInfo->minFilter;
    mMagFilter = mTexInfo->magFilter;
    mMinLOD = mTexInfo->minLOD;
    mMaxLOD = mTexInfo->maxLOD;
    mLODBias = mTexInfo->LODBias;
    init();
}

void JUTTexture::attachPalette(JUTPalette* param_0) {
    if (mTexInfo->indexTexture) {
        if (param_0 == NULL && mEmbPalette != NULL) {
            field_0x2c = mEmbPalette;
        } else {
            field_0x2c = param_0;
        }
        _GXTlut name = (_GXTlut)field_0x2c->getTlutName();
        initTexObj(name);
    }
}

void JUTTexture::init() {
    if (mTexInfo->numColors == 0) {
        initTexObj();
    } else {
        if (mEmbPalette != NULL) {
            field_0x2c = mEmbPalette;

            _GXTlut name = (_GXTlut)field_0x2c->getTlutName();
            initTexObj(name);
        }
    }
}

void JUTTexture::initTexObj() {
    GXBool mipmapEnabled;
    if (mTexInfo->mipmapEnabled != 0) {
        mipmapEnabled = 1;
    } else {
        mipmapEnabled = 0;
    }
    u8* image = ((u8*)mTexInfo);
    image += (mTexInfo->imageOffset ? mTexInfo->imageOffset : 0x20);
    GXInitTexObj(&mTexObj, image, mTexInfo->width, mTexInfo->height,
                 (GXTexFmt)mTexInfo->format, (GXTexWrapMode)mWrapS,
                 (GXTexWrapMode)mWrapT, mipmapEnabled);
    GXInitTexObjLOD(&mTexObj, (GXTexFilter)mMinFilter, (GXTexFilter)mMagFilter,
                    mMinLOD / 8.0f, mMaxLOD / 8.0f, mLODBias / 100.0f, mTexInfo->biasClamp,
                    mTexInfo->doEdgeLOD, (GXAnisotropy)mTexInfo->maxAnisotropy);
}

void JUTTexture::initTexObj(_GXTlut param_0) {
    GXBool mipmapEnabled;
    if (mTexInfo->mipmapEnabled != 0) {
        mipmapEnabled = 1;
    } else {
        mipmapEnabled = 0;
    }
    mTlutName = param_0;
    u8* image = ((u8*)mTexInfo);
    image += (mTexInfo->imageOffset ? mTexInfo->imageOffset : 0x20);
    GXInitTexObjCI(&mTexObj, image, mTexInfo->width, mTexInfo->height,
                 (GXCITexFmt)mTexInfo->format, (GXTexWrapMode)mWrapS,
                 (GXTexWrapMode)mWrapT, mipmapEnabled, param_0);
    GXInitTexObjLOD(&mTexObj, (GXTexFilter)mMinFilter, (GXTexFilter)mMagFilter,
                    mMinLOD / 8.0f, mMaxLOD / 8.0f, mLODBias / 100.0f, mTexInfo->biasClamp,
                    mTexInfo->doEdgeLOD, (GXAnisotropy)mTexInfo->maxAnisotropy);
}

void JUTTexture::load(_GXTexMapID param_0) {
    if (field_0x2c) {
        field_0x2c->load();
    }
    GXLoadTexObj(&mTexObj, param_0);
}
