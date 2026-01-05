#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphBase/J3DShapeMtx.h"
#include "JSystem/J3DGraphAnimator/J3DModel.h"
#include "JSystem/J3DGraphBase/J3DFifo.h"
#include "JSystem/J3DGraphBase/J3DMatBlock.h"
#include "JSystem/J3DGraphBase/J3DTexture.h"

u16 J3DShapeMtx::sMtxLoadCache[10];

void J3DShapeMtx::resetMtxLoadCache() {
    sMtxLoadCache[0] =
    sMtxLoadCache[1] =
    sMtxLoadCache[2] =
    sMtxLoadCache[3] =
    sMtxLoadCache[4] =
    sMtxLoadCache[5] =
    sMtxLoadCache[6] =
    sMtxLoadCache[7] =
    sMtxLoadCache[8] =
    sMtxLoadCache[9] = 0xFFFF;
}

void J3DShapeMtx::loadMtxIndx_PNGP(int slot, u16 indx) const {
    // J3DFifoLoadPosMtxIndx(indx, slot * 3);  // matches debug, but not retail 
    J3DFifoLoadIndx(GX_LOAD_INDX_A, indx, 0xB000 | ((u16)(slot * 0x0C)));
    J3DFifoLoadNrmMtxIndx3x3(indx, slot * 3);
}

void J3DShapeMtx::loadMtxIndx_PCPU(int slot, u16 indx) const {
    J3DFifoLoadPosMtxImm(*j3dSys.getShapePacket()->getBaseMtxPtr(), slot * 3);
    j3dSys.loadNrmMtxIndx(slot, indx);
}

void J3DShapeMtx::loadMtxIndx_NCPU(int slot, u16 indx) const {
    j3dSys.loadPosMtxIndx(slot, indx);
    J3DFifoLoadNrmMtxImm(*j3dSys.getShapePacket()->getBaseMtxPtr(), slot * 3);
}

void J3DShapeMtx::loadMtxIndx_PNCPU(int slot, u16 indx) const {
    J3DFifoLoadPosMtxImm(*j3dSys.getShapePacket()->getBaseMtxPtr(), slot * 3);
    J3DFifoLoadNrmMtxImm(*j3dSys.getShapePacket()->getBaseMtxPtr(), slot * 3);
}

J3DShapeMtx_LoadFunc J3DShapeMtx::sMtxLoadPipeline[4] = {
    &J3DShapeMtx::loadMtxIndx_PNGP,
    &J3DShapeMtx::loadMtxIndx_PCPU,
    &J3DShapeMtx::loadMtxIndx_NCPU,
    &J3DShapeMtx::loadMtxIndx_PNCPU,
};

J3DShapeMtxConcatView_LoadFunc J3DShapeMtxConcatView::sMtxLoadPipeline[4] = {
    &J3DShapeMtxConcatView::loadMtxConcatView_PNGP,
    &J3DShapeMtxConcatView::loadMtxConcatView_PCPU,
    &J3DShapeMtxConcatView::loadMtxConcatView_NCPU,
    &J3DShapeMtxConcatView::loadMtxConcatView_PNCPU,
};

J3DShapeMtxConcatView_LoadFunc J3DShapeMtxConcatView::sMtxLoadLODPipeline[4] = {
    &J3DShapeMtxConcatView::loadMtxConcatView_PNGP_LOD,
    &J3DShapeMtxConcatView::loadMtxConcatView_PCPU,
    &J3DShapeMtxConcatView::loadMtxConcatView_NCPU,
    &J3DShapeMtxConcatView::loadMtxConcatView_PNCPU,
};

u32 J3DShapeMtx::sCurrentPipeline;

u8* J3DShapeMtx::sCurrentScaleFlag;

bool J3DShapeMtx::sNBTFlag;

bool J3DShapeMtx::sLODFlag;

u32 J3DShapeMtx::sTexMtxLoadType;

MtxP J3DShapeMtxConcatView::sMtxPtrTbl[2];

J3DTexGenBlock* J3DDifferedTexMtx::sTexGenBlock;

J3DTexMtxObj* J3DDifferedTexMtx::sTexMtxObj;

void J3DDifferedTexMtx::loadExecute(f32 const (*param_0)[4]) {
    static Mtx qMtx = {
        {0.5f, 0.0f, 0.5f, 0.0f},
        {0.0f, -0.5f, 0.5f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
    };

    static Mtx qMtx2 = {
        {0.5f, 0.0f, 0.0f, 0.5f},
        {0.0f, -0.5f, 0.0f, 0.5f},
        {0.0f, 0.0f, 1.0f, 0.0f}
    };

    Mtx* mtx; // sp_64
    Mtx sp_e8;
    Mtx44 sp_a8, sp_68;

    J3DTexGenBlock* tex_gen_block = sTexGenBlock; // sp_60
    J3D_ASSERT_NULLPTR(195, tex_gen_block != NULL);

    J3DTexMtxObj* tex_mtx_obj = sTexMtxObj; // sp_5c
    J3D_ASSERT_NULLPTR(198, tex_mtx_obj != NULL);

    J3DTexMtxInfo* tex_mtx_info_1; // sp_58
    int tex_gen_type; // sp_54
    J3DTexMtx* tex_mtx_2; // sp_50

    u16 tex_mtx_num = tex_mtx_obj->getNumTexMtx();

    if (j3dSys.checkFlag(J3DSysFlag_PostTexMtx)) {
        for (u16 i = 0; i < tex_mtx_num; i++) {
            tex_gen_type = tex_gen_block->getTexCoord(i)->getTexGenType();
            if (tex_gen_type == 1 || tex_gen_type == 0) {
                tex_mtx_2 = tex_gen_block->getTexMtx(i);
                J3D_ASSERT_NULLPTR(215, tex_mtx_2 != NULL);
                tex_mtx_info_1 = &tex_mtx_2->getTexMtxInfo();

                u32 sp_4c = tex_mtx_info_1->mInfo & 0x3f;
                switch (sp_4c) {
                case 3:
                case 9: {
                    mtx = &tex_mtx_obj->getMtx(i);
                    break;
                }
                case 1:
                case 6:
                case 7: {
                    mtx = &tex_mtx_obj->getMtx(i);
                    break;
                }
                case 2:
                case 8: {
                    MTXInverse(j3dSys.getViewMtx(), sp_e8);
                    MTXConcat(tex_mtx_obj->getMtx(i), sp_e8, sp_e8);
                    mtx = &sp_e8;
                    break;
                }
                case 5: {
                    J3DTexMtxInfo* tex_mtx_info_2 = &tex_mtx_2->getTexMtxInfo(); // sp_48
                    u32 sp_44 = (u32)(tex_mtx_info_2->mInfo & 0x80) >> 7;
                    if (sp_44 == 0) {
                        J3DGetTextureMtxOld(tex_mtx_info_2->mSRT, tex_mtx_info_2->mCenter, sp_68);
                    } else if (sp_44 == 1) {
                        J3DGetTextureMtxMayaOld(tex_mtx_info_2->mSRT, sp_68);
                    }
                    J3DMtxProjConcat(sp_68, tex_mtx_obj->getEffectMtx(i), sp_e8);
                    MTXInverse(j3dSys.getViewMtx(), sp_a8);
                    MTXConcat(sp_e8, sp_a8, sp_e8);
                    sp_e8[0][3] = sp_e8[1][3] = sp_e8[2][3] = 0.0f;
                    mtx = &sp_e8;
                    break;
                }
                case 11: {
                    J3DTexMtxInfo* tex_mtx_info_2 = &tex_mtx_2->getTexMtxInfo(); // sp_40
                    u32 sp_3c = (u32)(tex_mtx_info_2->mInfo & 0x80) >> 7;
                    if (sp_3c == 0) {
                        J3DGetTextureMtx(tex_mtx_info_2->mSRT, tex_mtx_info_2->mCenter, sp_68);
                    } else if (sp_3c == 1) {
                        J3DGetTextureMtxMaya(tex_mtx_info_2->mSRT, sp_68);
                    }

                    MTXConcat(sp_68, qMtx, sp_68);
                    J3DMtxProjConcat(sp_68, tex_mtx_obj->getEffectMtx(i), sp_e8);
                    MTXInverse(j3dSys.getViewMtx(), sp_a8);
                    MTXConcat(sp_e8, sp_a8, sp_e8);
                    sp_e8[0][3] = sp_e8[1][3] = sp_e8[2][3] = 0.0f;
                    mtx = &sp_e8;
                    break;
                }
                case 10: {
                    J3DTexMtxInfo* tex_mtx_info_2 = &tex_mtx_2->getTexMtxInfo(); // sp_38
                    u32 sp_34 = (u32)(tex_mtx_info_2->mInfo & 0x80) >> 7;
                    if (sp_34 == 0) {
                        J3DGetTextureMtx(tex_mtx_info_2->mSRT, tex_mtx_info_2->mCenter, sp_68);
                    } else if (sp_34 == 1) {
                        J3DGetTextureMtxMaya(tex_mtx_info_2->mSRT, sp_68);
                    }

                    MTXConcat(sp_68, qMtx2, sp_68);
                    J3DMtxProjConcat(sp_68, tex_mtx_obj->getEffectMtx(i), sp_e8);
                    MTXInverse(j3dSys.getViewMtx(), sp_a8);
                    MTXConcat(sp_e8, sp_a8, sp_e8);
                    sp_e8[0][3] = sp_e8[1][3] = sp_e8[2][3] = 0.0f;
                    mtx = &sp_e8;
                    break;
                }
                case 0:
                case 4:
                default: {
                    mtx = &tex_mtx_obj->getMtx(i);
                }
                }
                GXLoadTexMtxImm(*mtx, i * 3 + 0x40, GX_MTX3x4);
            }
        }
    } else {
        for (u16 i = 0; i < tex_mtx_num; i++) {
            int tex_gen_type = tex_gen_block->getTexCoord(i)->getTexGenType();
            if (tex_gen_type == 1 || tex_gen_type == 0) {
                J3DTexMtx* tex_mtx = tex_gen_block->getTexMtx(i); // sp_2c
                J3D_ASSERT_NULLPTR(325, tex_mtx != NULL);
                tex_mtx_info_1 = &tex_mtx->getTexMtxInfo();

                u32 tex_gen_src = tex_mtx_info_1->mInfo & 0x3f; // sp_28
                switch (tex_gen_src) {
                case 3:
                case 9:
                    MTXConcat(tex_mtx_obj->getMtx(i), param_0, sp_e8);
                    mtx = &sp_e8;
                    break;
                case 1:
                case 6:
                case 7:
                    MTXCopy(param_0, sp_a8);
                    sp_a8[0][3] = sp_a8[1][3] = sp_a8[2][3] = 0.0f;
                    MTXConcat(tex_mtx_obj->getMtx(i), sp_a8, sp_e8);
                    mtx = &sp_e8;
                    break;
                case 2:
                case 8:
                    mtx = &tex_mtx_obj->getMtx(i);
                    break;
                case 5: {
                    J3DTexMtxInfo* tex_mtx_info_2 = &tex_mtx->getTexMtxInfo(); // sp_24
                    u32 sp_24 = (u32)(tex_mtx_info_2->mInfo & 0x80) >> 7;
                    if (sp_24 == 0) {
                        J3DGetTextureMtxOld(tex_mtx_info_2->mSRT, tex_mtx_info_2->mCenter, sp_68);
                    } else if (sp_24 == 1) {
                        J3DGetTextureMtxMayaOld(tex_mtx_info_2->mSRT, sp_68);
                    }

                    J3DMtxProjConcat(sp_68, tex_mtx_obj->getEffectMtx(i), sp_e8);
                    MTXInverse(j3dSys.getViewMtx(), sp_a8);
                    MTXConcat(sp_e8, sp_a8, sp_e8);
                    MTXConcat(sp_e8, param_0, sp_e8);
                    sp_e8[0][3] = sp_e8[1][3] = sp_e8[2][3] = 0.0f;
                    mtx = &sp_e8;
                    break;
                }
                case 11: {
                    J3DTexMtxInfo* tex_mtx_info_2 = &tex_mtx->getTexMtxInfo(); // sp_1c
                    u32 sp_18 = (u32)(tex_mtx_info_2->mInfo & 0x80) >> 7;
                    if (sp_18 == 0) {
                        J3DGetTextureMtx(tex_mtx_info_2->mSRT, tex_mtx_info_2->mCenter, sp_68);
                    } else if (sp_18 == 1) {
                        J3DGetTextureMtxMaya(tex_mtx_info_2->mSRT, sp_68);
                    }

                    MTXConcat(sp_68, qMtx, sp_68);
                    J3DMtxProjConcat(sp_68, tex_mtx_obj->getEffectMtx(i), sp_e8);
                    MTXInverse(j3dSys.getViewMtx(), sp_a8);
                    MTXConcat(sp_e8, sp_a8, sp_e8);
                    MTXConcat(sp_e8, param_0, sp_e8);
                    sp_e8[0][3] = sp_e8[1][3] = sp_e8[2][3] = 0.0f;
                    mtx = &sp_e8;
                    break;
                }
                case 10: {
                    J3DTexMtxInfo* tex_mtx_info_2 = &tex_mtx->getTexMtxInfo(); // sp_14
                    u32 sp_10 = (u32)(tex_mtx_info_2->mInfo & 0x80) >> 7;
                    if (sp_10 == 0) {
                        J3DGetTextureMtx(tex_mtx_info_2->mSRT, tex_mtx_info_2->mCenter, sp_68);
                    } else if (sp_10 == 1) {
                        J3DGetTextureMtxMaya(tex_mtx_info_2->mSRT, sp_68);
                    }

                    MTXConcat(sp_68, qMtx2, sp_68);
                    J3DMtxProjConcat(sp_68, tex_mtx_obj->getEffectMtx(i), sp_e8);
                    MTXInverse(j3dSys.getViewMtx(), sp_a8);
                    MTXConcat(sp_e8, sp_a8, sp_e8);
                    MTXConcat(sp_e8, param_0, sp_e8);
                    sp_e8[2][3] = 0.0f;
                    sp_e8[1][3] = 0.0f;
                    sp_e8[0][3] = 0.0f;
                    mtx = &sp_e8;
                    break;
                }
                case 0:
                case 4:
                default: {
                    mtx = &tex_mtx_obj->getMtx(i);
                    break;
                }
                }
                GXLoadTexMtxImm(*mtx, i * 3 + 30, (GXTexMtxType)tex_mtx_info_1->mProjection);
            }
        }
    }
}

void J3DShapeMtxConcatView::loadMtxConcatView_PNGP(int slot, u16 drw) const {
    Mtx m;
    MTXConcat(*j3dSys.getShapePacket()->getBaseMtxPtr(), j3dSys.getModelDrawMtx(drw), m);
    J3DDifferedTexMtx::load(m);
    J3DFifoLoadPosMtxImm(m, slot * 3);
    loadNrmMtx(slot, drw, m);
}

void J3DShapeMtxConcatView::loadMtxConcatView_PCPU(int slot, u16 drw) const {
    Mtx m;
    MTXConcat(*j3dSys.getShapePacket()->getBaseMtxPtr(), j3dSys.getModelDrawMtx(drw), m);
    J3DDifferedTexMtx::load(m);
    J3DFifoLoadPosMtxImm(*j3dSys.getShapePacket()->getBaseMtxPtr(), slot * 3);
    loadNrmMtx(slot, drw, m);
}

void J3DShapeMtxConcatView::loadMtxConcatView_NCPU(int slot, u16 drw) const {
    Mtx m;
    MTXConcat(*j3dSys.getShapePacket()->getBaseMtxPtr(), j3dSys.getModelDrawMtx(drw), m);
    J3DDifferedTexMtx::load(m);
    J3DFifoLoadPosMtxImm(m, slot * 3);
    J3DFifoLoadNrmMtxImm(*j3dSys.getShapePacket()->getBaseMtxPtr(), slot * 3);

    if (J3DShapeMtx::sTexMtxLoadType == 0x2000)
        J3DFifoLoadNrmMtxToTexMtx(*j3dSys.getShapePacket()->getBaseMtxPtr(), slot * 3 + GX_TEXMTX0);
}

void J3DShapeMtxConcatView::loadMtxConcatView_PNCPU(int slot, u16 drw) const {
    if (J3DDifferedTexMtx::sTexGenBlock != NULL) {
        Mtx m;
        MTXConcat(*j3dSys.getShapePacket()->getBaseMtxPtr(), j3dSys.getModelDrawMtx(drw), m);
        J3DDifferedTexMtx::loadExecute(m);
    }

    J3DFifoLoadPosMtxImm(*j3dSys.getShapePacket()->getBaseMtxPtr(), slot * 3);
    J3DFifoLoadNrmMtxImm(*j3dSys.getShapePacket()->getBaseMtxPtr(), slot * 3);

    if (J3DShapeMtx::sTexMtxLoadType == 0x2000)
        J3DFifoLoadNrmMtxToTexMtx(*j3dSys.getShapePacket()->getBaseMtxPtr(), slot * 3 + GX_TEXMTX0);
}

void J3DShapeMtxConcatView::loadMtxConcatView_PNGP_LOD(int slot, u16 drw) const {
    Mtx m;
    MTXConcat(*j3dSys.getShapePacket()->getBaseMtxPtr(), j3dSys.getModelDrawMtx(drw), m);
    MTXConcat(m, j3dSys.getModel()->getModelData()->getInvJointMtx(drw), m);
    J3DDifferedTexMtx::load(m);
    J3DFifoLoadPosMtxImm(m, slot * 3);
    loadNrmMtx(slot, drw, m);
}

void J3DShapeMtx::load() const {
    J3DShapeMtx_LoadFunc func = sMtxLoadPipeline[sCurrentPipeline];
    (this->*func)(0, mUseMtxIndex);
}

void J3DShapeMtx::calcNBTScale(Vec const& param_0, Mtx33* param_1, Mtx33* param_2) {
    J3DPSMtx33Copy(param_1[mUseMtxIndex], param_2[mUseMtxIndex]);
    J3DScaleNrmMtx33(param_2[mUseMtxIndex], param_0);
}

void J3DShapeMtxConcatView::load() const {
    sMtxPtrTbl[0] = j3dSys.getModel()->getMtxBuffer()->getUserAnmMtx(0);
    sMtxPtrTbl[1] = j3dSys.getModel()->getWeightAnmMtx(0);
    J3DShapeMtxConcatView_LoadFunc func = sMtxLoadPipeline[sCurrentPipeline];

    j3dSys.setModelDrawMtx((Mtx*)sMtxPtrTbl[j3dSys.getModel()->getModelData()->getDrawMtxFlag(mUseMtxIndex)]);

    u16 draw_mtx_index = j3dSys.getModel()->getModelData()->getDrawMtxIndex(mUseMtxIndex);
    (this->*func)(0, draw_mtx_index);
}

void J3DShapeMtxConcatView::loadNrmMtx(int param_0, u16 param_1, MtxP param_2) const {
    if (sCurrentScaleFlag[param_1] == 1) {
        if (sTexMtxLoadType == 0x2000) {
            J3DFifoLoadNrmMtxToTexMtx(param_2, 0x1e);
        }

        if (!sNBTFlag) {
            J3DFifoLoadNrmMtxImm(param_2, 0);
        } else {
            Mtx33 mtx;
            Vec* scale = j3dSys.getNBTScale();
            J3DPSMtx33CopyFrom34(param_2, mtx);
            J3DScaleNrmMtx33(mtx, *scale);
            J3DFifoLoadNrmMtxImm3x3(mtx, 0);
        }
    } else {
        Mtx33 mtx;
        J3DPSCalcInverseTranspose(param_2, mtx);
        if (sTexMtxLoadType == 0x2000) {
            J3DFifoLoadNrmMtxToTexMtx3x3(mtx, 0x1e);
        }

        if (!sNBTFlag) {
            J3DFifoLoadNrmMtxImm3x3(mtx, 0);
        } else {
            Vec* scale = j3dSys.getNBTScale();
            J3DScaleNrmMtx33(mtx, *scale);
            J3DFifoLoadNrmMtxImm3x3(mtx, 0);
        }
    }
}

void J3DShapeMtxMulti::load() const {
    J3DShapeMtx_LoadFunc func = sMtxLoadPipeline[sCurrentPipeline];

    int use_mtx_num = mUseMtxNum;
    for (int i = 0; i < use_mtx_num; i++) {
        if (mUseMtxIndexTable[i] != 0xffff) {
            (this->*func)(i, mUseMtxIndexTable[i]);
        }
    }
}

void J3DShapeMtxMulti::calcNBTScale(Vec const& param_0, Mtx33* param_1, Mtx33* param_2) {
    int use_mtx_num = mUseMtxNum;
    for (int i = 0; i < use_mtx_num; i++) {
        if (mUseMtxIndexTable[i] != 0xffff) {
            J3DPSMtx33Copy(param_1[mUseMtxIndexTable[i]], param_2[mUseMtxIndexTable[i]]);
            J3DScaleNrmMtx33(param_2[mUseMtxIndexTable[i]], param_0);
        }
    }
}

void J3DShapeMtxMultiConcatView::load() const {
    sMtxPtrTbl[0] = j3dSys.getModel()->getMtxBuffer()->getUserAnmMtx(0);
    sMtxPtrTbl[1] = j3dSys.getModel()->getWeightAnmMtx(0);

    if (!sLODFlag) {
        J3DShapeMtxConcatView_LoadFunc func = sMtxLoadPipeline[sCurrentPipeline];
        int use_mtx_num = mUseMtxNum;
        for (int i = 0; i < use_mtx_num; i++) {
            if (mUseMtxIndexTable[i] != 0xffff) {
                u16 draw_mtx_index = j3dSys.getModel()->getModelData()->getDrawMtxIndex(mUseMtxIndexTable[i]);
                j3dSys.setModelDrawMtx((Mtx*)sMtxPtrTbl[j3dSys.getModel()->getModelData()->getDrawMtxFlag(mUseMtxIndexTable[i])]);
                (this->*func)(i, draw_mtx_index);
            }
        }
    } else {
        u16* important_mtx_indices = j3dSys.getModel()->getModelData()->getWEvlpImportantMtxIndex();
        j3dSys.setModelDrawMtx((Mtx*)sMtxPtrTbl[0]);

        int use_mtx_num = mUseMtxNum;
        for (int i = 0; i < use_mtx_num; i++) {
            u32 current_pipeline = sCurrentPipeline;
            J3DShapeMtxConcatView_LoadFunc func = sMtxLoadLODPipeline[current_pipeline];

            if (mUseMtxIndexTable[i] != 0xffff) {
                u16 important_mtx_index = important_mtx_indices[mUseMtxIndexTable[i]];
                if (important_mtx_index != sMtxLoadCache[i]) {
                    if (j3dSys.getModel()->getModelData()->getDrawMtxFlag(mUseMtxIndexTable[i]) == 0) {
                        func = sMtxLoadPipeline[sCurrentPipeline];
                    }

                    (this->*func)(i, important_mtx_index);
                    sMtxLoadCache[i] = important_mtx_index;
                }
            }
        }
    }
}

void J3DShapeMtxMultiConcatView::loadNrmMtx(int param_0, u16 param_1, MtxP param_2) const {
    Mtx33 mtx1, mtx2;
    if (sCurrentScaleFlag[param_1] == 1) {
        if (sTexMtxLoadType == 0x2000) {
            J3DFifoLoadNrmMtxToTexMtx(param_2, param_0 * 3 + 0x1e);
        }
        if (!sNBTFlag) {
            J3DFifoLoadNrmMtxImm(param_2, param_0 * 3);
        } else {
            Vec* nbt_scale = j3dSys.getNBTScale();
            J3DPSMtx33CopyFrom34(param_2, mtx1);
            J3DScaleNrmMtx33(mtx1, *nbt_scale);
            J3DFifoLoadNrmMtxImm3x3(mtx1, 0);
        }
    } else {
        J3DPSCalcInverseTranspose(param_2, mtx2);
        if (sTexMtxLoadType == 0x2000) {
            J3DFifoLoadNrmMtxToTexMtx3x3(mtx2, param_0 * 3 + 0x1e);
        }
        if (!sNBTFlag) {
            J3DFifoLoadNrmMtxImm3x3(mtx2, param_0 * 3);
        } else {
            Vec* nbt_scale = j3dSys.getNBTScale();
            J3DScaleNrmMtx33(mtx2, *nbt_scale);
            J3DFifoLoadNrmMtxImm3x3(mtx2, param_0 * 3);
        }
    }
}

void J3DShapeMtxBBoardConcatView::load() const {
    Mtx mtx;
    u16 draw_mtx_index = j3dSys.getModel()->getModelData()->getDrawMtxIndex(mUseMtxIndex);

    if (j3dSys.getModel()->getModelData()->getDrawMtxFlag(mUseMtxIndex) == 0) {
        MTXConcat(j3dSys.getViewMtx(), j3dSys.getModel()->getMtxBuffer()->getUserAnmMtx(draw_mtx_index), mtx);
    } else {
        MTXConcat(j3dSys.getViewMtx(), j3dSys.getModel()->getWeightAnmMtx(draw_mtx_index), mtx);
    }

    J3DCalcBBoardMtx(mtx);
    J3DFifoLoadPosMtxImm(mtx, 0);

    mtx[0][0] = 1.0f / mtx[0][0];
    mtx[1][1] = 1.0f / mtx[1][1];
    mtx[2][2] = 1.0f / mtx[2][2];
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;

    
    if (!sNBTFlag) {
        J3DFifoLoadNrmMtxImm(mtx, 0);
    } else {
        Vec* nbt_scale = j3dSys.getNBTScale();
        J3DScaleNrmMtx(mtx, *nbt_scale);
        J3DFifoLoadNrmMtxImm(mtx, 0);
    }
}

void J3DShapeMtxYBBoardConcatView::load() const {
    Mtx mtx1;
    Mtx33 mtx2;
    u16 draw_mtx_index = j3dSys.getModel()->getModelData()->getDrawMtxIndex(mUseMtxIndex);

    if (j3dSys.getModel()->getModelData()->getDrawMtxFlag(mUseMtxIndex) == 0) {
        MTXConcat(j3dSys.getViewMtx(), j3dSys.getModel()->getMtxBuffer()->getUserAnmMtx(draw_mtx_index), mtx1);
    } else {
        MTXConcat(j3dSys.getViewMtx(), j3dSys.getModel()->getWeightAnmMtx(draw_mtx_index), mtx1);
    }

    J3DCalcYBBoardMtx(mtx1);
    J3DFifoLoadPosMtxImm(mtx1, 0);

    if (sCurrentScaleFlag[mUseMtxIndex] == 1) {
        if (!sNBTFlag) {
            J3DFifoLoadNrmMtxImm(mtx1, 0);
        } else {
            Vec* nbt_scale = j3dSys.getNBTScale();
            J3DScaleNrmMtx(mtx1, *nbt_scale);
            J3DFifoLoadNrmMtxImm(mtx1, 0);
        }
    } else {
        J3DPSCalcInverseTranspose(mtx1, mtx2);
        if (!sNBTFlag) {
            J3DFifoLoadNrmMtxImm3x3(mtx2, 0);
        } else {
            Vec* nbt_scale = j3dSys.getNBTScale();
            J3DScaleNrmMtx33(mtx2, *nbt_scale);
            J3DFifoLoadNrmMtxImm3x3(mtx2, 0);
        }
    }
}

static void dummy() {
    static u8 mtxCache[20] = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    };
}
