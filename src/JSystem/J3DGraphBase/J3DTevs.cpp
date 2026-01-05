//
// J3DTevs
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphBase/J3DTevs.h"
#include "JSystem/J3DGraphBase/J3DGD.h"
#include "JSystem/J3DGraphBase/J3DMatBlock.h"
#include "JSystem/J3DGraphBase/J3DSys.h"
#include "JSystem/J3DGraphBase/J3DTransform.h"
#include "global.h"

static void J3DGDLoadTexMtxImm(f32 (*)[4], u32, GXTexMtxType);
static void J3DGDLoadPostTexMtxImm(f32 (*)[4], u32);

void J3DLightObj::load(u32 lightIdx) const {
    GDOverflowCheck(0x48);
    J3DGDSetLightPos(GXLightID(1 << lightIdx), mInfo.mLightPosition.x, mInfo.mLightPosition.y, mInfo.mLightPosition.z);
    J3DGDSetLightAttn(GXLightID(1 << lightIdx), mInfo.mCosAtten.x, mInfo.mCosAtten.y, mInfo.mCosAtten.z, mInfo.mDistAtten.x, mInfo.mDistAtten.y, mInfo.mDistAtten.z);
    J3DGDSetLightColor(GXLightID(1 << lightIdx), mInfo.mColor);
    J3DGDSetLightDir(GXLightID(1 << lightIdx), mInfo.mLightDirection.x, mInfo.mLightDirection.y, mInfo.mLightDirection.z);
}

void loadTexCoordGens(u32 texGenNum, J3DTexCoord* texCoords) {
    u32 var_r28;
    GDOverflowCheck(texGenNum * 4 * 2 + 10);
    J3DGDWriteXFCmdHdr(GX_XF_REG_TEX0, texGenNum);

    for (int i = 0; i < texGenNum; i++) {
        J3DGDSetTexCoordGen(
            GXTexGenType(texCoords[i].getTexGenType()),
            GXTexGenSrc(texCoords[i].getTexGenSrc())
        );
    }

    var_r28 = 61;
    J3DGDWriteXFCmdHdr(GX_XF_REG_DUALTEX0, texGenNum);

    if (j3dSys.checkFlag(0x40000000)) {
        for (int i = 0; i < texGenNum; i++) {
            if (texCoords[i].getTexGenMtx() != 60) {
                var_r28 = i * 3;
            } else {
                var_r28 = 61;
            }
            J3DGDWrite_u32(var_r28);
        }
    } else {
        for (int i = 0; i < texGenNum; i++) {
            J3DGDWrite_u32(var_r28);
        }
    }
}

void J3DTexMtx::load(u32 mtxIdx) const {
    if (j3dSys.checkFlag(J3DSysFlag_PostTexMtx)) {
        loadPostTexMtx(mtxIdx);
    } else {
        loadTexMtx(mtxIdx);
    }
}

void J3DTexMtx::calc(f32 const (*param_0)[4]) {
    calcTexMtx(param_0);
}

void J3DTexMtx::calcTexMtx(const Mtx param_0) {
    Mtx44 mtx1;
    Mtx44 mtx2;

    static Mtx qMtx = {
        0.5f, 0.0f, 0.5f, 0.0f,
        0.0f, -0.5f, 0.5f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
    };
    static Mtx qMtx2 = {
        0.5f, 0.0f, 0.0f, 0.5f,
        0.0f, -0.5f, 0.0f, 0.5f,
        0.0f, 0.0f, 1.0f, 0.0f,
    };

    u32 r28 = mTexMtxInfo.mInfo & 0x3f;
    u32 r30 = (mTexMtxInfo.mInfo >> 7) & 1;
    switch (r28) {
    case 8:
    case 9:
    case 11:
        if (r30 == 0) {
            J3DGetTextureMtx(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx2);
        } else if (r30 == 1) {
            J3DGetTextureMtxMaya(mTexMtxInfo.mSRT, mtx2);
        }
        MTXConcat(mtx2, qMtx, mtx2);
        J3DMtxProjConcat(mtx2, mTexMtxInfo.mEffectMtx, mtx1);
        MTXConcat(mtx1, param_0, mMtx);
        break;
    case 7:
        if (r30 == 0) {
            J3DGetTextureMtx(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx1);
        } else if (r30 == 1) {
            J3DGetTextureMtxMaya(mTexMtxInfo.mSRT, mtx1);
        }
        MTXConcat(mtx1, qMtx, mtx1);
        MTXConcat(mtx1, param_0, mMtx);
        break;
    case 10:
        if (r30 == 0) {
            J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx2);
        } else if (r30 == 1) {
            J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mtx2);
        }
        MTXConcat(mtx2, qMtx2, mtx2);
        J3DMtxProjConcat(mtx2, mTexMtxInfo.mEffectMtx, mtx1);
        MTXConcat(mtx1, param_0, mMtx);
        break;
    case 6:
        if (r30 == 0) {
            J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx1);
        } else if (r30 == 1) {
            J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mtx1);
        }
        MTXConcat(mtx1, qMtx2, mtx1);
        MTXConcat(mtx1, param_0, mMtx);
        break;
    case 1:
        if (r30 == 0) {
            J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx1);
        } else if (r30 == 1) {
            J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mtx1);
        }
        MTXConcat(mtx1, param_0, mMtx);
        break;
    case 2:
    case 3:
    case 5:
        if (r30 == 0) {
            J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx2);
        } else if (r30 == 1) {
            J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mtx2);
        }
        J3DMtxProjConcat(mtx2, mTexMtxInfo.mEffectMtx, mtx1);
        MTXConcat(mtx1, param_0, mMtx);
        break;
    case 4:
        if (r30 == 0) {
            J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx2);
        } else if (r30 == 1) {
            J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mtx2);
        }
        J3DMtxProjConcat(mtx2, mTexMtxInfo.mEffectMtx, mMtx);
        break;
    default:
        if (r30 == 0) {
            J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mMtx);
        } else if (r30 == 1) {
            J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mMtx);
        }
        break;
    }
}

void J3DTexMtx::calcPostTexMtx(const Mtx param_0) {
    Mtx44 mtx1;
    Mtx44 mtx2;

    static Mtx qMtx = {
        0.5f, 0.0f, 0.5f, 0.0f,
        0.0f, -0.5f, 0.5f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
    };
    static Mtx qMtx2 = {
        0.5f, 0.0f, 0.0f, 0.5f,
        0.0f, -0.5f, 0.0f, 0.5f,
        0.0f, 0.0f, 1.0f, 0.0f,
    };

    u32 r29 = mTexMtxInfo.mInfo & 0x3f;
    u32 r30 = (mTexMtxInfo.mInfo >> 7) & 1;
    switch (r29) {
    case 8:
    case 11:
        if (r30 == 0) {
            J3DGetTextureMtx(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx2);
        } else if (r30 == 1) {
            J3DGetTextureMtxMaya(mTexMtxInfo.mSRT, mtx2);
        }
        MTXConcat(mtx2, qMtx, mtx2);
        J3DMtxProjConcat(mtx2, mTexMtxInfo.mEffectMtx, mtx1);
        MTXConcat(mtx1, param_0, mMtx);
        break;
    case 9:
        if (r30 == 0) {
            J3DGetTextureMtx(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx2);
        } else if (r30 == 1) {
            J3DGetTextureMtxMaya(mTexMtxInfo.mSRT, mtx2);
        }
        MTXConcat(mtx2, qMtx, mtx2);
        J3DMtxProjConcat(mtx2, mTexMtxInfo.mEffectMtx, mMtx);
        break;
    case 7:
        if (r30 == 0) {
            J3DGetTextureMtx(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx1);
        } else if (r30 == 1) {
            J3DGetTextureMtxMaya(mTexMtxInfo.mSRT, mtx1);
        }
        MTXConcat(mtx1, qMtx, mMtx);
        break;
    case 10:
        if (r30 == 0) {
            J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx2);
        } else if (r30 == 1) {
            J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mtx2);
        }
        MTXConcat(mtx2, qMtx2, mtx2);
        J3DMtxProjConcat(mtx2, mTexMtxInfo.mEffectMtx, mtx1);
        MTXConcat(mtx1, param_0, mMtx);
        break;
    case 6:
        if (r30 == 0) {
            J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx1);
        } else if (r30 == 1) {
            J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mtx1);
        }
        MTXConcat(mtx1, qMtx2, mMtx);
        break;
    case 1:
        if (r30 == 0) {
            J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mMtx);
        } else if (r30 == 1) {
            J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mMtx);
        }
        break;
    case 2:
    case 5:
        if (r30 == 0) {
            J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx2);
        } else if (r30 == 1) {
            J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mtx2);
        }
        J3DMtxProjConcat(mtx2, mTexMtxInfo.mEffectMtx, mtx1);
        MTXConcat(mtx1, param_0, mMtx);
        break;
    case 3:
        if (r30 == 0) {
            J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx2);
        } else if (r30 == 1) {
            J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mtx2);
        }
        J3DMtxProjConcat(mtx2, mTexMtxInfo.mEffectMtx, mMtx);
        break;
    case 4:
        if (r30 == 0) {
            J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx2);
        } else if (r30 == 1) {
            J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mtx2);
        }
        J3DMtxProjConcat(mtx2, mTexMtxInfo.mEffectMtx, mMtx);
        break;
    default:
        if (r30 == 0) {
            J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mMtx);
        } else if (r30 == 1) {
            J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mMtx);
        }
        break;
    }
}

bool isTexNoReg(void* pDL) {
    u8 r31 = ((u8*)pDL)[1];
    if (r31 >= 0x80 && r31 <= 0xbb) {
        return true;
    }
    return false;
}

u16 getTexNoReg(void* pDL) {
    u32 var_r31 = *(u32*)((u8*)pDL + 1);
    return var_r31 & 0xFFFFFF;
}

void loadTexNo(u32 param_0, const u16& texNo) {
    ResTIMG* resTIMG = j3dSys.getTexture()->getResTIMG(texNo);
    J3D_ASSERT_NULLPTR(462, resTIMG != NULL);

    J3DSys::sTexCoordScaleTable[param_0].field_0x00 = (u16)resTIMG->width;
    J3DSys::sTexCoordScaleTable[param_0].field_0x02 = (u16)resTIMG->height;

    GDOverflowCheck(0x14);
    J3DGDSetTexImgPtr(GXTexMapID(param_0), (u8*)resTIMG + resTIMG->imageOffset);
    J3DGDSetTexImgAttr(GXTexMapID(param_0), resTIMG->width, resTIMG->height, GXTexFmt(resTIMG->format & 0x0f));
    J3DGDSetTexLookupMode(GXTexMapID(param_0), GXTexWrapMode(resTIMG->wrapS), GXTexWrapMode(resTIMG->wrapT), GXTexFilter(resTIMG->minFilter), GXTexFilter(resTIMG->magFilter), resTIMG->minLOD * 0.125f, resTIMG->maxLOD * 0.125f, resTIMG->LODBias * 0.01f, resTIMG->biasClamp, resTIMG->doEdgeLOD, GXAnisotropy(resTIMG->maxAnisotropy));

    if (resTIMG->indexTexture == true) {
        GXTlutSize tlutSize = resTIMG->numColors > 16 ? GX_TLUT_256 : GX_TLUT_16;
        GDOverflowCheck(0x14);
        J3DGDLoadTlut((u8*)resTIMG + resTIMG->paletteOffset, (param_0 << 13) + 0xf0000, tlutSize);
        J3DGDSetTexTlut(GXTexMapID(param_0), (param_0 << 13) + 0xf0000, GXTlutFmt(resTIMG->colorFormat));
    }
}

void patchTexNo_PtrToIdx(u32 texID, const u16& idx) {
    ResTIMG* timg = j3dSys.getTexture()->getResTIMG(idx);
    J3D_ASSERT_NULLPTR(523, timg != NULL);

    J3DGDSetTexImgPtrRaw(GXTexMapID(texID), idx);
}

void loadNBTScale(J3DNBTScale& NBTScale) {
    if (NBTScale.mbHasScale == true) {
        j3dSys.setNBTScale(&NBTScale.mScale);
    } else {
        j3dSys.setNBTScale(NULL);
    }
}

const J3DLightInfo j3dDefaultLightInfo = {
    0.0f, 0.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0xff, 0xff, 0xff, 0xff,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
};

J3DTexCoordInfo const j3dDefaultTexCoordInfo[8] = {
    {GX_MTX2x4, GX_TG_TEX0, GX_IDENTITY, 0}, {GX_MTX2x4, GX_TG_TEX1, GX_IDENTITY, 0},
    {GX_MTX2x4, GX_TG_TEX2, GX_IDENTITY, 0}, {GX_MTX2x4, GX_TG_TEX3, GX_IDENTITY, 0},
    {GX_MTX2x4, GX_TG_TEX4, GX_IDENTITY, 0}, {GX_MTX2x4, GX_TG_TEX5, GX_IDENTITY, 0},
    {GX_MTX2x4, GX_TG_TEX6, GX_IDENTITY, 0}, {GX_MTX2x4, GX_TG_TEX7, GX_IDENTITY, 0},
};

J3DTexMtxInfo const j3dDefaultTexMtxInfo = {
    0x01,
    0x00,
    0xFF,
    0xFF,
    {0.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     1.0f},
};

J3DIndTexMtxInfo const j3dDefaultIndTexMtxInfo = {
    0.5f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.0f,
    1
};

J3DTevStageInfo const j3dDefaultTevStageInfo = {
    0x04, 0x0A, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x05, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
};

J3DIndTevStageInfo const j3dDefaultIndTevStageInfo = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

J3DFogInfo const j3dDefaultFogInfo = {
    0x00, 0x00, 0x0140, 0.0f, 0.0f, 0.1f, 10000.0f, 0xFF, 0xFF, 0xFF, 0x00,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

J3DNBTScaleInfo const j3dDefaultNBTScaleInfo = {
    0x00, 1.0f, 1.0f, 1.0f,
};

static u8 j3dTexCoordTable[7623];

void makeTexCoordTable() {
    u8 texMtx[] = {
        GX_TEXMTX0,
        GX_TEXMTX1,
        GX_TEXMTX2,
        GX_TEXMTX3,
        GX_TEXMTX4,
        GX_TEXMTX5,
        GX_TEXMTX6,
        GX_TEXMTX7,
        GX_TEXMTX8,
        GX_TEXMTX9,
        GX_IDENTITY,
    };

    u8* table = j3dTexCoordTable;
    for (u32 i = 0; i < 11; i++) {
        for (u32 j = 0; j < 21; j++) {
            for (int k = 0; k < ARRAY_SIZE(texMtx); k++) {
                u32 idx = j * 11 + i * 0xe7 + k;
                table[idx * 3 + 0] = i;
                table[idx * 3 + 1] = j;
                table[idx * 3 + 2] = texMtx[k];
            }
        }
    }
}

u8 j3dTevSwapTableTable[1024];

u8 j3dAlphaCmpTable[768];

void makeAlphaCmpTable() {
    u8* table = j3dAlphaCmpTable;
    for (u32 i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            for (u32 k = 0; k < 8; k++) {
                u32 idx = i * 32 + j * 8 + k;
                table[idx * 3] = i;
                table[idx * 3 + 1] = j;
                table[idx * 3 + 2] = k;
            }
        }
    }
}

u8 j3dZModeTable[96];

void makeZModeTable() {
    u8* table = j3dZModeTable;
    for (int i = 0; i < 2; i++) {
        for (u32 j = 0; j < 8; j++) {
            for (int k = 0; k < 2; k++) {
                u32 idx = j * 2 + i * 16 + k;
                table[idx * 3 + 0] = i;
                table[idx * 3 + 1] = j;
                table[idx * 3 + 2] = k;
            }
        }
    }
}

void makeTevSwapTable() {
    u8* table = j3dTevSwapTableTable;
    int i = 0;
    do {
        table[0] = i >> 6;
        table[1] = (i >> 4) & 3;
        table[2] = (i >> 2) & 3;
        table[3] = i & 3;
        i++;
        table += 4;
    } while (i < 256);
}

void J3DTexMtx::loadTexMtx(u32 param_0) const {
    GDOverflowCheck(0x35);
	J3DGDLoadTexMtxImm((MtxP)mMtx, param_0 * 3 + 30, (_GXTexMtxType)mTexMtxInfo.mProjection);
}

void J3DTexMtx::loadPostTexMtx(u32 param_0) const {
    GDOverflowCheck(0x35);
    J3DGDLoadPostTexMtxImm((MtxP)mMtx, param_0 * 3 + 0x40);
}

static void J3DGDLoadTexMtxImm(f32 (*param_1)[4], u32 param_2, _GXTexMtxType param_3) {
    u16 addr = param_2 << 2;
    u8 len = param_3 == GX_MTX2x4 ? 8 : 12;
    J3DGDWriteXFCmdHdr(addr, len);
    J3DGDWrite_f32(param_1[0][0]);
    J3DGDWrite_f32(param_1[0][1]);
    J3DGDWrite_f32(param_1[0][2]);
    J3DGDWrite_f32(param_1[0][3]);
    J3DGDWrite_f32(param_1[1][0]);
    J3DGDWrite_f32(param_1[1][1]);
    J3DGDWrite_f32(param_1[1][2]);
    J3DGDWrite_f32(param_1[1][3]);
    if (param_3 == GX_MTX3x4) {
        J3DGDWrite_f32(param_1[2][0]);
        J3DGDWrite_f32(param_1[2][1]);
        J3DGDWrite_f32(param_1[2][2]);
        J3DGDWrite_f32(param_1[2][3]);
    }
}

static void J3DGDLoadPostTexMtxImm(f32 (*param_1)[4], u32 param_2) {
    u16 addr = (param_2 - 0x40) * 4 + 0x500;
    int stride = 12;

    J3DGDWriteXFCmdHdr(addr, stride);
    J3DGDWrite_f32(param_1[0][0]);
    J3DGDWrite_f32(param_1[0][1]);
    J3DGDWrite_f32(param_1[0][2]);
    J3DGDWrite_f32(param_1[0][3]);
    J3DGDWrite_f32(param_1[1][0]);
    J3DGDWrite_f32(param_1[1][1]);
    J3DGDWrite_f32(param_1[1][2]);
    J3DGDWrite_f32(param_1[1][3]);
    J3DGDWrite_f32(param_1[2][0]);
    J3DGDWrite_f32(param_1[2][1]);
    J3DGDWrite_f32(param_1[2][2]);
    J3DGDWrite_f32(param_1[2][3]);
}

const GXColor j3dDefaultColInfo = {0xFF, 0xFF, 0xFF, 0xFF};

const GXColor j3dDefaultAmbInfo = {0x32, 0x32, 0x32, 0x32};

const u8 j3dDefaultNumChans = 1;

const J3DTevOrderInfo j3dDefaultTevOrderInfoNull = {0xFF, 0xFF, 0xFF, 0x00};

const J3DIndTexOrderInfo j3dDefaultIndTexOrderNull = {0xFF, 0xFF, 0x00, 0x00};

const GXColorS10 j3dDefaultTevColor = {0xFF, 0xFF, 0xFF, 0xFF};

const J3DIndTexCoordScaleInfo j3dDefaultIndTexCoordScaleInfo = {
    0x00,
    0x00,
    0x00,
    0x00,
};

const GXColor j3dDefaultTevKColor = {0xFF, 0xFF, 0xFF, 0xFF};

J3DTevSwapModeInfo const j3dDefaultTevSwapMode = {
    0x00,
    0x00,
    0x00,
    0x00,
};

const J3DTevSwapModeTableInfo j3dDefaultTevSwapModeTable = {0x00, 0x01, 0x02, 0x03};

const J3DBlendInfo j3dDefaultBlendInfo = {GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP};

const J3DColorChanInfo j3dDefaultColorChanInfo = {
    0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0xFF, 0xFF,
};

const u8 j3dDefaultTevSwapTableID = 0x1B;

const u16 j3dDefaultAlphaCmpID = 0x00E7;

const u16 j3dDefaultZModeID = 0x0017;
