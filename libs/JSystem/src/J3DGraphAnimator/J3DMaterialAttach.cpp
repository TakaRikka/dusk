#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphAnimator/J3DMaterialAttach.h"
#include "JSystem/J3DGraphAnimator/J3DMaterialAnm.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "JSystem/JUtility/JUTAssert.h"

void J3DMaterialTable::clear() {
    mMaterialNum = 0;
    mUniqueMatNum = 0;
    mMaterialNodePointer = NULL;
    mMaterialName = NULL;
    field_0x10 = 0;
    mTexture = NULL;
    mTextureName = NULL;
    field_0x1c = 0;
}

J3DMaterialTable::J3DMaterialTable() {
    mMaterialNum = 0;
    mUniqueMatNum = 0;
    mMaterialNodePointer = NULL;
    mMaterialName = NULL;
    field_0x10 = 0;
    mTexture = NULL;
    mTextureName = NULL;
    field_0x1c = 0;
}

J3DMaterialTable::~J3DMaterialTable() {}

int J3DMaterialTable::removeMatColorAnimator(J3DAnmColor* pAnmColor) {
    J3D_ASSERT_NULLPTR(208, pAnmColor != NULL);
    int ret = false;

    u16 updateMatNum = pAnmColor->getUpdateMaterialNum();
    for (u16 i = 0; i < updateMatNum; i++) {
        if (pAnmColor->isValidUpdateMaterialID(i)) {
            u16 matNo = pAnmColor->getUpdateMaterialID(i);
            J3DMaterialAnm* materialAnm = getMaterialNodePointer(matNo)->getMaterialAnm();
            if (materialAnm == NULL) {
                ret = true;
            } else {
                materialAnm->setMatColorAnm(0, NULL);
            }
        }
    }

    return ret;
}

int J3DMaterialTable::removeTexNoAnimator(J3DAnmTexPattern* pAnmTexPattern) {
    J3D_ASSERT_NULLPTR(242, pAnmTexPattern != NULL);
    int ret = 0;

    u16 materialNum = pAnmTexPattern->getUpdateMaterialNum();
    J3DAnmTexPatternFullTable* anm_table = (J3DAnmTexPatternFullTable*)pAnmTexPattern->getAnmTable();

    for (u16 i = 0; i < materialNum; i++) {
        if (pAnmTexPattern->isValidUpdateMaterialID(i)) {
            u16 matNo = pAnmTexPattern->getUpdateMaterialID(i);
            J3DMaterialAnm* pMatAnm = getMaterialNodePointer(matNo)->getMaterialAnm();
            u8 texNo = anm_table[i].mTexNo;
            if (pMatAnm == NULL)
                ret = 1;
            else
                pMatAnm->setTexNoAnm(texNo, NULL);
        }
    }

    return ret;
}

int J3DMaterialTable::removeTexMtxAnimator(J3DAnmTextureSRTKey* pAnmTexSRTKey) {
    J3D_ASSERT_NULLPTR(278, pAnmTexSRTKey != NULL);
    s32 ret = 0;

    u16 materialNum = pAnmTexSRTKey->getUpdateMaterialNum();
    u16 postUpdateMaterialNum[1];
    postUpdateMaterialNum[0] = pAnmTexSRTKey->getPostUpdateMaterialNum();

    for (u16 i = 0; i < materialNum; i++) {
        if (pAnmTexSRTKey->isValidUpdateMaterialID(i)) {
            u16 matNo = pAnmTexSRTKey->getUpdateMaterialID(i);
            J3DMaterial* pMaterial = getMaterialNodePointer(matNo);
            J3DMaterialAnm* pMatAnm = pMaterial->getMaterialAnm();
            u8 texMtxID = pAnmTexSRTKey->getUpdateTexMtxID(i);
            if (pMatAnm == NULL) {
                ret = 1;
            } else if (texMtxID != 0xFF) {
                pMatAnm->setTexMtxAnm(texMtxID, NULL);
            }
        }
    }

    return ret;
}

int J3DMaterialTable::removeTevRegAnimator(J3DAnmTevRegKey* pAnmTevRegKey) {
    J3D_ASSERT_NULLPTR(320, pAnmTevRegKey != NULL);
    s32 ret = 0;

    u16 cRegMaterialNum = pAnmTevRegKey->getCRegUpdateMaterialNum();
    u16 kRegMaterialNum = pAnmTevRegKey->getKRegUpdateMaterialNum();

    for (u16 i = 0; i < cRegMaterialNum; i++) {
        if (pAnmTevRegKey->isValidCRegUpdateMaterialID(i)) {
            const u16 spC = pAnmTevRegKey->getCRegUpdateMaterialID(i);
            J3DMaterialAnm* pMatAnm = getMaterialNodePointer(spC)->getMaterialAnm();
            const J3DAnmCRegKeyTable* table = pAnmTevRegKey->getAnmCRegKeyTable();
            const u8 colorId = table[i].mColorId;
            if (pMatAnm == NULL)
                ret = 1;
            else
                pMatAnm->setTevColorAnm(colorId, NULL);
        }
    }

    for (u16 i = 0; i < kRegMaterialNum; i++) {
        if (pAnmTevRegKey->isValidKRegUpdateMaterialID(i)) {
            const u16 spA = pAnmTevRegKey->getKRegUpdateMaterialID(i);
            J3DMaterialAnm* pMatAnm = getMaterialNodePointer(spA)->getMaterialAnm();
            const J3DAnmKRegKeyTable* table = pAnmTevRegKey->getAnmKRegKeyTable();
            const u8 colorId = table[i].mColorId;
            if (pMatAnm == NULL) {
                ret = 1;
            } else {
                pMatAnm->setTevKColorAnm(colorId, NULL);
            }
        }
    }

    return ret;
}

int J3DMaterialTable::createTexMtxForAnimator(J3DAnmTextureSRTKey* pTexSRTKey) {
    J3D_ASSERT_NULLPTR(382, pTexSRTKey != NULL);
    int rv = 0;

    u16 materialNum = pTexSRTKey->getUpdateMaterialNum();
    if (isLocked()) {
        return 2;
    }

    for (u16 i = 0; i < materialNum; i++) {
        if (pTexSRTKey->isValidUpdateMaterialID(i)) {
            const u16 matNo = pTexSRTKey->getUpdateMaterialID(i);
            J3DMaterial* material = getMaterialNodePointer(matNo);
            u8 texMtx = pTexSRTKey->getUpdateTexMtxID(i);

            J3DMaterialAnm* pMaterialAnm = material->getMaterialAnm();
            if (pMaterialAnm == NULL) {
                rv = 1;
            } else {
                if (texMtx != 0xff && material->getTexMtx(texMtx) == NULL) {
                    J3DTexMtx* mtx = new J3DTexMtx();
                    rv = 4;
                    material->setTexMtx(texMtx, mtx);
                    JUT_WARN(420, "matNo<%d> : texMtx%d nothing !\n", matNo, texMtx);
                }
            }
        }
    }

    return rv;
}

int J3DMaterialTable::entryMatColorAnimator(J3DAnmColor* pAnmColor) {
    J3D_ASSERT_NULLPTR(450, pAnmColor != NULL);
    int rv = 0;

    u16 materialNum = pAnmColor->getUpdateMaterialNum();
    if (isLocked()) {
        return 2;
    }

    for (u16 i = 0; i < materialNum; i++) {
        if (pAnmColor->isValidUpdateMaterialID(i)) {
            const u16 matNo = pAnmColor->getUpdateMaterialID(i);
            J3DMaterialAnm* materialAnm = getMaterialNodePointer(matNo)->getMaterialAnm();
            if (materialAnm == NULL) {
                rv = 1;
            } else {
                J3DMatColorAnm matColorAnm(i, pAnmColor);
                materialAnm->setMatColorAnm(0, &matColorAnm);
            }
        }
    }

    return rv;
}

int J3DMaterialTable::entryTexNoAnimator(J3DAnmTexPattern* pAnmTexPattern) {
    J3D_ASSERT_NULLPTR(489, pAnmTexPattern != NULL);
    int rv = 0;

    u16 materialNum = pAnmTexPattern->getUpdateMaterialNum();
    if (isLocked()) {
        return 2;
    }

    for (u16 i = 0; i < materialNum; i++) {
        if (pAnmTexPattern->isValidUpdateMaterialID(i)) {
            const u16 matNo = pAnmTexPattern->getUpdateMaterialID(i);
            J3DMaterialAnm* materialAnm =  getMaterialNodePointer(matNo)->getMaterialAnm();
            const J3DAnmTexPatternFullTable* table = pAnmTexPattern->getAnmTable();
            const u8 texNo = table[i].mTexNo;
            if (materialAnm == NULL) {
                rv = 1;
            } else {
                J3DTexNoAnm texNoAnm(i, pAnmTexPattern);
                materialAnm->setTexNoAnm(texNo, &texNoAnm);
            }
        }
    }

    return rv;
}

int J3DMaterialTable::entryTexMtxAnimator(J3DAnmTextureSRTKey* pAnmTexSRTKey) {
    J3D_ASSERT_NULLPTR(532, pAnmTexSRTKey != NULL);
    int rv = 0;
    u16 materialNum = pAnmTexSRTKey->getUpdateMaterialNum();

    rv = createTexMtxForAnimator(pAnmTexSRTKey);
    if (rv != 0) {
        return rv;
    }

    if (isLocked()) {
        return 2;
    }

    for (u16 no = 0; no < materialNum; no++) {
        if (pAnmTexSRTKey->isValidUpdateMaterialID(no)) {
            const u16 matNo = pAnmTexSRTKey->getUpdateMaterialID(no);
            J3DMaterial* material = getMaterialNodePointer(matNo);
            J3DMaterialAnm* materialAnm = material->getMaterialAnm();
            u8 texMtxID = pAnmTexSRTKey->getUpdateTexMtxID(no);
            if (materialAnm == NULL) {
                rv = 1;
            } else {
                if (texMtxID != 0xff) {
                    if (material->getTexCoord(texMtxID) != NULL) {
                        material->getTexCoord(texMtxID)->setTexGenMtx((u8)texMtxID * 3 + 30);
                    }
                    J3DTexMtxInfo& tmtxinfo = material->getTexMtx(texMtxID)->getTexMtxInfo();
                    tmtxinfo.mInfo = (tmtxinfo.mInfo & 0x3f) | (pAnmTexSRTKey->getTexMtxCalcType() << 7);
                    tmtxinfo.mCenter.x = pAnmTexSRTKey->getSRTCenter(no)->x;
                    tmtxinfo.mCenter.y = pAnmTexSRTKey->getSRTCenter(no)->y;
                    tmtxinfo.mCenter.z = pAnmTexSRTKey->getSRTCenter(no)->z;
                    J3DTexMtxAnm texMtxAnm(no, pAnmTexSRTKey);
                    materialAnm->setTexMtxAnm(texMtxID, &texMtxAnm);
                }
            }
        }
    }

    return rv;
}

int J3DMaterialTable::entryTevRegAnimator(J3DAnmTevRegKey* pAnmTevRegKey) {
    J3D_ASSERT_NULLPTR(599, pAnmTevRegKey != NULL);
    int rv = 0;

    u16 cRegNum = pAnmTevRegKey->getCRegUpdateMaterialNum();
    u16 kRegNum = pAnmTevRegKey->getKRegUpdateMaterialNum();
    if (isLocked()) {
        return 2;
    }

    for (u16 i = 0; i < cRegNum; i++) {
        if (pAnmTevRegKey->isValidCRegUpdateMaterialID(i)) {
            const u16 matNo = pAnmTevRegKey->getCRegUpdateMaterialID(i);
            J3DMaterialAnm* materialAnm = getMaterialNodePointer(matNo)->getMaterialAnm();
            const J3DAnmCRegKeyTable* table = pAnmTevRegKey->getAnmCRegKeyTable();
            u8 colorId = table[i].mColorId;
            if (materialAnm == NULL) {
                rv = 1;
            } else {
                J3DTevColorAnm tevColorAnm(i, pAnmTevRegKey);
                materialAnm->setTevColorAnm(colorId, &tevColorAnm);
            }
        }
    }

    for (u16 i = 0; i < kRegNum; i++) {
        if (pAnmTevRegKey->isValidKRegUpdateMaterialID(i)) {
            const u16 matNo = pAnmTevRegKey->getKRegUpdateMaterialID(i);
            J3DMaterialAnm* materialAnm = getMaterialNodePointer(matNo)->getMaterialAnm();
            const J3DAnmKRegKeyTable* table = pAnmTevRegKey->getAnmKRegKeyTable();
            const u8 colorId = table[i].mColorId;
            if (materialAnm == NULL) {
                rv = 1;
            } else {
                J3DTevKColorAnm tevKColorAnm(i, pAnmTevRegKey);
                materialAnm->setTevKColorAnm(colorId, &tevKColorAnm);
            }
        }
    }

    return rv;
}
