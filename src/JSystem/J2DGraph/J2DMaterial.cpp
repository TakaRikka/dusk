#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DMaterial.h"
#include "JSystem/J3DGraphBase/J3DStruct.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTPalette.h"
#include "JSystem/JUtility/JUTTexture.h"

J2DMaterial::J2DMaterial() {
    field_0x4 = 0;
    mIndex = -1;
    mTevBlock = NULL;
    mIndBlock = NULL;
    mAnmPointer = NULL;
    field_0x8 = 1;
    mVisible = true;
}

J2DMaterial::~J2DMaterial() {
    delete mTevBlock;
    delete mIndBlock;
    delete mAnmPointer;
}

void J2DMaterial::setGX() {
    if (mVisible) {
        getColorBlock()->setGX();
        getTexGenBlock()->setGX();

        if (getTevBlock() != NULL) {
            getTevBlock()->setGX();
        }

        if (getIndBlock() != NULL) {
            getIndBlock()->setGX();
        }

        getPEBlock()->setGX();
    }
}

J2DTevBlock* J2DMaterial::createTevBlock(int block_type, bool noAlign) {
    J2DTevBlock* block;

    if (noAlign) {
        if (block_type <= 1) {
            block = new J2DTevBlock1();
        } else if (block_type == 2) {
            block = new J2DTevBlock2();
        } else if (block_type <= 4) {
            block = new J2DTevBlock4();
        } else if (block_type <= 8) {
            block = new J2DTevBlock8();
        } else {
            block = new J2DTevBlock16();
        }
    } else {
        if (block_type <= 1) {
            block = new (-4) J2DTevBlock1();
        } else if (block_type == 2) {
            block = new (-4) J2DTevBlock2();
        } else if (block_type <= 4) {
            block = new (-4) J2DTevBlock4();
        } else if (block_type <= 8) {
            block = new (-4) J2DTevBlock8();
        } else {
            block = new (-4) J2DTevBlock16();
        }
    }

    return block;
}

J2DIndBlock* J2DMaterial::createIndBlock(int block_type, bool noAlign) {
    J2DIndBlock* block;

    if (noAlign) {
        if (block_type != 0) {
            block = new J2DIndBlockFull();
        } else {
            block = new J2DIndBlockNull();
        }
    } else {
        if (block_type != 0) {
            block = new (-4) J2DIndBlockFull();
        } else {
            block = new (-4) J2DIndBlockNull();
        }
    }

    return block;
}

J2DMaterial::J2DMaterialAnmPointer::J2DMaterialAnmPointer() {
    mColorAnm = NULL;
    mColorIds = -1;
    mSRTAnm = NULL;

    for (int i = 0; i < 8; i++) {
        mSRTIds[i] = -1;
    }
    mPatternAnm = NULL;

    for (int i = 0; i < 8; i++) {
        mPatternIds[i] = -1;
    }
    mTevAnm = NULL;

    for (int i = 0; i < 4; i++) {
        mTevCRegIds[i] = -1;
    }

    for (int i = 0; i < 4; i++) {
        mTevKRegIds[i] = -1;
    }
}

void J2DMaterial::makeAnmPointer() {
    if (mAnmPointer == NULL) {
        mAnmPointer = new J2DMaterialAnmPointer();
    }
}

void J2DMaterial::setAnimation(J2DAnmColor* anm) {
    if (anm != NULL || mAnmPointer != NULL) {
        makeAnmPointer();
        mAnmPointer->mColorAnm = anm;
        mAnmPointer->mColorIds = -1;

        if (anm != NULL) {
            u16 matNum = anm->getUpdateMaterialNum();
            u16 index = getIndex();

            for (u16 i = 0; i < matNum; i++) {
                u16 matID = anm->getUpdateMaterialID(i);

                if (index == matID) {
                    mAnmPointer->mColorIds = i;
                    return;
                }
            }
        }
    }
}

void J2DMaterial::setAnimation(J2DAnmTextureSRTKey* anm) {
    if (anm != NULL || mAnmPointer != NULL) {
        makeAnmPointer();
        mAnmPointer->mSRTAnm = anm;

        for (int i = 0; i < 8; i++) {
            mAnmPointer->mSRTIds[i] = -1;
        }

        if (anm != NULL) {
            u16 matNum = anm->getUpdateMaterialNum();
            u16 index = getIndex();

            for (u16 i = 0; i < matNum; i++) {
                u16 matID = anm->getUpdateMaterialID(i);

                if (index == matID) {
                    u8 mtxID = anm->getUpdateTexMtxID(i);
                    mAnmPointer->mSRTIds[mtxID] = i;
                }
            }
        }
    }
}

void J2DMaterial::setAnimation(J2DAnmTexPattern* anm) {
    if (anm != NULL || mAnmPointer != NULL) {
        makeAnmPointer();
        mAnmPointer->mPatternAnm = anm;

        for (int i = 0; i < 8; i++) {
            mAnmPointer->mPatternIds[i] = -1;
        }

        if (anm != NULL) {
            u16 matNum = anm->getUpdateMaterialNum();
            u16 index = getIndex();
            J3DAnmTexPatternFullTable* anmTbl = anm->getAnmTable();

            for (u16 i = 0; i < matNum; i++) {
                u16 matID = anm->getUpdateMaterialID(i);

                if (index == matID) {
                    mAnmPointer->mPatternIds[anmTbl[i].mTexNo] = i;
                }
            }
        }
    }
}

void J2DMaterial::setAnimation(J2DAnmTevRegKey* anm) {
    if (anm != NULL || mAnmPointer != NULL) {
        makeAnmPointer();
        mAnmPointer->mTevAnm = anm;

        for (int i = 0; i < 4; i++) {
            mAnmPointer->mTevCRegIds[i] = -1;
        }

        if (anm != NULL) {
            u16 matNum = anm->getCRegUpdateMaterialNum();
            u16 index = getIndex();

            for (u16 i = 0; i < matNum; i++) {
                u16 matID = anm->getCRegUpdateMaterialID(i);

                if (index == matID) {
                    J3DAnmCRegKeyTable* anmTbl = anm->getAnmCRegKeyTable();
                    mAnmPointer->mTevCRegIds[anmTbl[i].mColorId] = i;
                }
            }
        }

        for (int i = 0; i < 4; i++) {
            mAnmPointer->mTevKRegIds[i] = -1;
        }

        if (anm != NULL) {
            u16 matNum = anm->getKRegUpdateMaterialNum();
            u16 index = getIndex();

            for (u16 i = 0; i < matNum; i++) {
                u16 matID = anm->getKRegUpdateMaterialID(i);

                if (index == matID) {
                    J3DAnmKRegKeyTable* anmTbl = anm->getAnmKRegKeyTable();
                    mAnmPointer->mTevKRegIds[anmTbl[i].mColorId] = i;
                }
            }
        }
    }
}

void J2DMaterial::animation() {
    if (!mAnmPointer) {
		return;
	}

	if (mAnmPointer->mColorAnm && mAnmPointer->mColorIds != 0xFFFF) {
		GXColor color;
		mAnmPointer->mColorAnm->getColor(mAnmPointer->mColorIds, &color);
		getColorBlock()->setMatColor(0, color);
	}
    
	if (mAnmPointer->mSRTAnm) {
		for (u8 i = 0; i < 8; i++) {
			if (mAnmPointer->mSRTIds[i] != 0xFFFF) {
				J3DTextureSRTInfo info3D;
				mAnmPointer->mSRTAnm->getTransform(mAnmPointer->mSRTIds[i], &info3D);
				J2DTextureSRTInfo info2D;
				info2D.mScaleX       = info3D.mScaleX;
				info2D.mScaleY       = info3D.mScaleY;
				info2D.mRotationDeg  = (360.0f * f32((u16)info3D.mRotation)) / 65535.0f;
				info2D.mTranslationX = info3D.mTranslationX;
				info2D.mTranslationY = info3D.mTranslationY;

				J2DTexMtx texMtx;
				getTexGenBlock()->getTexMtx(i, texMtx);

				texMtx.getTexMtxInfo().mTexSRTInfo = info2D;
				getTexGenBlock()->setTexMtx(i, texMtx);
				getTexGenBlock()->getTexCoord(i).setTexGenMtx(i * 3 + 30);
			}
		}
	}

	if (getTevBlock() && mAnmPointer->mPatternAnm) {
		for (u8 i = 0; i < 8; i++) {
			if (mAnmPointer->mPatternIds[i] != 0xFFFF) {
				JUTTexture* texture = getTevBlock()->getTexture(i);
				if (!texture) {
					continue;
				}

				u16 idx = mAnmPointer->mPatternIds[i];
				u16 texNo;
				mAnmPointer->mPatternAnm->getTexNo(idx, &texNo);
				getTevBlock()->setTexNo(i, texNo);

                ResTIMG* img = mAnmPointer->mPatternAnm->getResTIMG(idx);
                if (texture->getTexInfo() != img) {
                    JUTPalette* palette = NULL;
                    GXTlut tlut = GX_TLUT0;
                    if (img->indexTexture != 0) {
                        palette = mAnmPointer->mPatternAnm->getPalette(idx);
						if (palette->getNumColors() > 256) {
							tlut = (GXTlut)(i % 4 + 16);
						} else {
							tlut = (GXTlut)i;
						}
                    }

                    texture->storeTIMG(img, palette, tlut);
                }
            }
		}
	}

	if (getTevBlock() && mAnmPointer->mTevAnm) {
		for (u8 i = 0; i < 4; i++) {
			if (mAnmPointer->mTevCRegIds[i] != 0xFFFF) {
				J2DGXColorS10 color;
				mAnmPointer->mTevAnm->getTevColorReg(mAnmPointer->mTevCRegIds[i], &color);
				getTevBlock()->setTevColor(i, color);
			}
		}

		for (u8 i = 0; i < 4; i++) {
			u16 idx = mAnmPointer->mTevKRegIds[i];
			if (idx != 0xFFFF) {
				JUtility::TColor konstColor;
				mAnmPointer->mTevAnm->getTevKonstReg(idx, &konstColor);
				getTevBlock()->setTevKColor(i, konstColor);
			}
		}
	}
}
