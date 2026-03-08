#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DAssert.h"
#include "JSystem/J3DGraphBase/J3DTexture.h"
#include "JSystem/JKernel/JKRHeap.h"

void J3DTexture::loadGX(u16 idx, GXTexMapID texMapID) const {
    J3D_ASSERT_RANGE(29, idx < mNum);

    ResTIMG* timg = getResTIMG(idx);

#if TARGET_PC
    if (timg->width == 0 || timg->height == 0)
        return;
    if (idx >= mNum) {
        OSReport("J3DTexture::loadGX: idx %d out of bounds (mNum=%d)!\n", idx, mNum);
        return;
    }
    if (timg->indexTexture) {
        GXLoadTlut(&mpTlutObj[idx], (GXTlut)texMapID);
        GXInitTexObjTlut(&mpTexObj[idx], (GXTlut)texMapID);
    }
    GXLoadTexObj(&mpTexObj[idx], texMapID);
#else
    GXTexObj texObj;
    GXTlutObj tlutObj;

    if (!timg->indexTexture) {
        GXInitTexObj(&texObj, ((u8*)timg) + timg->imageOffset, timg->width, timg->height,
                     (GXTexFmt)timg->format, (GXTexWrapMode)timg->wrapS, (GXTexWrapMode)timg->wrapT,
                     timg->mipmapEnabled);
    } else {
        GXInitTexObjCI(&texObj, ((u8*)timg) + timg->imageOffset, timg->width, timg->height,
                       (GXCITexFmt)timg->format, (GXTexWrapMode)timg->wrapS,
                       (GXTexWrapMode)timg->wrapT, timg->mipmapEnabled, (u32)texMapID);
        GXInitTlutObj(&tlutObj, ((u8*)timg) + timg->paletteOffset, (GXTlutFmt)timg->colorFormat,
                      timg->numColors);
        GXLoadTlut(&tlutObj, texMapID);
    }

    const f32 kLODClampScale = 1.0f / 8.0f;
    const f32 kLODBiasScale = 1.0f / 100.0f;
    GXInitTexObjLOD(&texObj, (GXTexFilter)timg->minFilter, (GXTexFilter)timg->magFilter,
                    timg->minLOD * kLODClampScale, timg->maxLOD * kLODClampScale,
                    timg->LODBias * kLODBiasScale, timg->biasClamp, timg->doEdgeLOD,
                    (GXAnisotropy)timg->maxAnisotropy);
    GXLoadTexObj(&texObj, texMapID);
#endif
}

#if TARGET_PC
void J3DTexture::loadGXTexObj(u16 idx) {
    J3D_ASSERT_RANGE(29, idx < mNum);
    ResTIMG* timg = getResTIMG(idx);

    GXTlutObj& tlutObj = mpTlutObj[idx];
    GXTexObj& texObj = mpTexObj[idx];

    if (!timg->indexTexture) {
        GXInitTexObj(&texObj, mpImgDataPtr[idx], timg->width, timg->height,
                     (GXTexFmt)timg->format, (GXTexWrapMode)timg->wrapS, (GXTexWrapMode)timg->wrapT,
                     timg->mipmapEnabled);
    } else {
        GXInitTexObjCI(&texObj, mpImgDataPtr[idx], timg->width, timg->height,
                       (GXCITexFmt)timg->format, (GXTexWrapMode)timg->wrapS,
                       (GXTexWrapMode)timg->wrapT, timg->mipmapEnabled, GX_TLUT0);
        GXInitTlutObj(&tlutObj, mpTlutDataPtr[idx], (GXTlutFmt)timg->colorFormat,
                      timg->numColors);
    }

    const f32 kLODClampScale = 1.0f / 8.0f;
    const f32 kLODBiasScale = 1.0f / 100.0f;
    GXInitTexObjLOD(&texObj, (GXTexFilter)timg->minFilter, (GXTexFilter)timg->magFilter,
                    timg->minLOD * kLODClampScale, timg->maxLOD * kLODClampScale,
                    timg->LODBias * kLODBiasScale, timg->biasClamp, timg->doEdgeLOD,
                    (GXAnisotropy)timg->maxAnisotropy);
}
#endif

void J3DTexture::entryNum(u16 num) {
    J3D_ASSERT_NONZEROARG(79, num != 0);

    mNum = num;
    mpRes = JKR_NEW ResTIMG[num];
    J3D_ASSERT_ALLOCMEM(83, mpRes != NULL);
    
    delete[] mpTexObj;
    delete[] mpTlutObj;
    delete[] mpImgDataPtr;
    delete[] mpTlutDataPtr;
    mpTexObj = new GXTexObj[num]();
    mpTlutObj = new GXTlutObj[num]();
    mpImgDataPtr = new u8*[num]();
    mpTlutDataPtr = new u8*[num]();

    for (int i = 0; i < mNum; i++) {
        mpRes[i].paletteOffset = 0;
        mpRes[i].imageOffset = 0;
    }
}

void J3DTexture::addResTIMG(u16 newNum, const ResTIMG* newRes) {
    if (newNum == 0)
        return;

    J3D_ASSERT_NULLPTR(105, newRes != NULL);

    u16 oldNum = mNum;
    ResTIMG* oldRes = mpRes;

    entryNum(mNum + newNum);

    for (u16 i = 0; i < oldNum; i++) {
        setResTIMG(i, oldRes[i]);
    }

    for (u16 i = oldNum; i < mNum; i++) {
        setResTIMG(i, newRes[i]);
    }
}
