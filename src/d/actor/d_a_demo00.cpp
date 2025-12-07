/**
 * @file d_a_demo00.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_demo00.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "SSystem/SComponent/c_counter.h"
#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "d/d_kankyo_rain.h"
#include "d/d_camera.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_ext.h"
#include "d/d_demo.h"
#include "Z2AudioLib/Z2Instances.h"
#include "m_Do/m_Do_lib.h"
#include "d/actor/d_a_movie_player.h"
#if DEBUG
#include "d/d_debug_viewer.h"
#endif

struct UnknownLightData {
    u32 field_0x00;
    const char* field_0x04;
    u32 field_0x08;
    u32 field_0x0C;
    u32 field_0x10;
    u32 field_0x14;
}; // Size: 0x18

static UnknownLightData l_lightData = {
    0x01010000,
    "V_24_tri_joint",
    0x00010000,
    0x00000000,
    0x00010000,
    0x00000000,
};

#if !DEBUG
// TODO: what is this?
static const u8 lit_3727[0xC] = {};
#endif

void daDemo00_resID_c::reset() {
    mShapeID = -1;
    field_0x4 = -1;
    field_0x8 = -1;
    field_0xc = -1;
    field_0x10 = -1;
    field_0x14 = -1;
    field_0x18 = -1;
    field_0x1c = -1;
    field_0x20 = -1;
}

void daDemo00_model_c::reset() {
    mID.reset();
    field_0x5d4 = NULL;
    mpBtpAnm = NULL;
    mpBtkAnm = NULL;
    mpBrkAnm = NULL;
    mpBpkAnm = NULL;
    mShadow = NULL;
    mDeformData = NULL;
    mpBlkAnm = NULL;
}

daDemo00_c::~daDemo00_c() {
    if (heap != NULL) {
        if (mModel.mpModelMorf != NULL) {
            mModel.mpModelMorf->stopZelAnime();
        }
    }
}

static void dummy() {
    // Fixes weak function order
    cM3dGPla plane;
    plane.~cM3dGPla();
}

inline int daDemo00_c::create() {
    dKy_tevstr_init(&tevStr, dComIfGp_roomControl_getStayNo(), 0xFF);
    tevStr.field_0x384 = 1;
    mSound.init(&eyePos, NULL, 10, 1);
    setAction(&daDemo00_c::actStandby);
    field_0x588.reset();
    field_0x6a0 = -1;
    return cPhs_COMPLEATE_e;
}

static void get_foward_angle(cXyz* param_1, cXyz* param_2, s16* param_3, s16* param_4) {
    cXyz sp38;
    if (param_1 == NULL || param_2 == NULL || param_3 == NULL || param_4 == NULL) {
        JUT_ASSERT(173, FALSE);
    }

    dKyr_get_vectle_calc(param_1, param_2, &sp38);
    f32 f31 = JMAFastSqrt(sp38.x * sp38.x + sp38.z * sp38.z);
    *param_3 = cM_atan2s(f31, sp38.y);
    *param_4 = sp38.atan2sX_Z();
}

void daDemo00_c::setBaseMtx() {
    s16 sVar1, sVar2;
    cXyz sp38;
    bool bVar1 = FALSE;
    cM3dGPla plane;

    if (tevStr.mInitTimer == 1) {
        field_0x574 = shape_angle;
        #if DEBUG
        debug_field_0x570 = current.pos;
        debug_field_0x57c = shape_angle;
        #endif
    }

    sp38 = current.pos;

    if (field_0x6a2 != 0 || mground2 != 0) {
        cXyz sp44(current.pos.x, current.pos.y + 1000.0f, current.pos.z);
        mModel.mBgc->mGndChk.SetPos(&sp44);
        sp38.y = dComIfG_Bgsp().GroundCross(&mModel.mBgc->mGndChk);
    }

    if (field_0x6a4 != 0 && field_0x568.x != 10000000.0f) {
        if (current.pos != field_0x568 && current.pos.abs(field_0x568) >= 1.5f) {
            get_foward_angle(&field_0x568, &current.pos, &sVar1, &sVar2);
            shape_angle.x = field_0x574.x = sVar1;
            shape_angle.y = field_0x574.y = sVar2;
        } else {
            shape_angle.x = field_0x574.x;
            shape_angle.y = field_0x574.y;
        }
    }

    field_0x568 = current.pos;

    if (mground2 != 0) {
        if (sp38.y != -G_CM3D_F_INF) {
            bVar1 = dComIfG_Bgsp().GetTriPla(mModel.mBgc->mGndChk, &plane);
        }

        if (bVar1 && cBgW_CheckBGround(plane.mNormal.y)) {
            shape_angle.x = fopAcM_getPolygonAngle(&plane, shape_angle.y);
        }
    }

    if (tevStr.mInitTimer != 0 || mground2 == 0) {
        current.angle = shape_angle;
    } else {
        cLib_addCalcAngleS2(&current.angle.x, shape_angle.x, 4, 0x2000);
        cLib_addCalcAngleS2(&current.angle.y, shape_angle.y, 4, 0x2000);
        cLib_addCalcAngleS2(&current.angle.z, shape_angle.z, 4, 0x2000);
    }

    mDoMtx_stack_c::transS(sp38.x, sp38.y, sp38.z);
    mDoMtx_stack_c::XYZrotM(current.angle.x, current.angle.y, current.angle.z);
    mModel.field_0x5d4->setBaseTRMtx(mDoMtx_stack_c::get());
    mModel.field_0x5d4->setBaseScale(scale);
}

void daDemo00_c::setShadowSize() {
    J3DModelData* modelData = mModel.field_0x5d4->getModelData();
    cXyz min(100000000.0f, 100000000.0f, 100000000.0f);
    cXyz max(-100000000.0f, -100000000.0f, -100000000.0f);

    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        J3DJoint* jntNodeP = modelData->getJointNodePointer(i);
        if (jntNodeP->getKind() == 0) {
            cXyz spb0, spbc;
            cMtx_multVec(mModel.field_0x5d4->getAnmMtx(i), jntNodeP->getMin(), &spb0);
            cMtx_multVec(mModel.field_0x5d4->getAnmMtx(i), jntNodeP->getMax(), &spbc);

            min.x = spb0.x < min.x ? spb0.x : min.x;
            min.y = spb0.y < min.y ? spb0.y : min.y;
            min.z = spb0.z < min.z ? spb0.z : min.z;

            max.x = spbc.x > max.x ? spbc.x : max.x;
            max.y = spbc.y > max.y ? spbc.y : max.y;
            max.z = spbc.z > max.z ? spbc.z : max.z;
        }
    }

    mModel.mShadow->field_0x4.x = (max.x + min.x) * 0.5f;
    mModel.mShadow->field_0x4.y = (max.y + min.y) * 0.5f;
    mModel.mShadow->field_0x4.z = (max.z + min.z) * 0.5f;
    cXyz spc8(max - min);
    mModel.mShadow->field_0x1c = spc8.abs() * 3.0f;
    mModel.mShadow->field_0x20 = spc8.absXZ() * 0.25f;
}

static BOOL awaCheck(J3DModel* i_model) {
    J3DModelData* modelData = i_model->getModelData();
    J3DTexture* texture = modelData->getTexture();

    if (texture != NULL) {
        JUTNameTab* nameTab = modelData->getTextureName();
        if (nameTab != NULL) {
            for (u16 i = 0; i < texture->getNum(); i++) {
                const char* name = nameTab->getName(i);
                if (strcmp(name, "B_dummy") == 0 || strcmp(name, "cy_kankyo") == 0) {
                    J3DSkinDeform* skinDeform = new J3DSkinDeform();
                    if (skinDeform == NULL) {
                        return FALSE;
                    }

                    if (i_model->setSkinDeform(skinDeform, 1) != 0) {
                        return FALSE;
                    }

                    if (strcmp(name, "B_dummy") == 0) {
                        texture->setResTIMG(i, *mDoGph_gInf_c::getFrameBufferTimg());
                        mDoExt_modelTexturePatch(modelData);
                    }
                }
            }
        }
    }

    return TRUE;
}

static int createHeapCallBack(fopAc_ac_c* a_this) {
    daDemo00_c* i_this = (daDemo00_c*)a_this;
    return i_this->createHeap();
}

int daDemo00_c::createHeap() {
    if (mModel.mID.mShapeID != -1) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), (u16)mModel.mID.mShapeID);
        mDoExt_bckAnmRemove(modelData);

        if (modelData == NULL) {
            OS_REPORT("\ngetDemoArcName=[%s]", dStage_roomControl_c::getDemoArcName());
            OS_REPORT("\nmModel.mID.mShapeID=[%d]\n", mModel.mID.mShapeID);
        }
        JUT_ASSERT(441, modelData != NULL);

        s32 uVar1 = 0x11000084;
        for (int i = 0; i < modelData->getShapeNum(); i++) {
            J3DShape* shape = modelData->getShapeNodePointer(i);
            if (shape->getTexMtxLoadType() == 0x2000) {
                field_0x6ad = 1;
                break;
            }
        }

        if (mModel.mID.field_0xc != -1) {
            mModel.mpBtpAnm = new mDoExt_btpAnm();
            if (mModel.mpBtpAnm == NULL) {
                return 0;
            }

            J3DAnmTexPattern* i_btk = (J3DAnmTexPattern*)dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), (u16)mModel.mID.field_0xc);
            if (i_btk == NULL) {
                // ESC_WARNING: The specified BTP animation could not be found! (%d)
                OS_REPORT("ESC_WARNING指定btpアニメーションが見つかりません！(%d)\n", mModel.mID.field_0xc);
                return 1;
            }

            if (mModel.mpBtpAnm->init(modelData, i_btk, 1, J3DFrameCtrl::EMode_NULL, 1.0f, 0, -1) == 0) {
                return 0;
            }

            uVar1 |= 0x4020000;
        }

        if (mModel.mID.field_0x10 != -1) {
            mModel.mpBtkAnm = new mDoExt_btkAnm();
            if (mModel.mpBtkAnm == NULL) {
                return 0;
            }

            J3DAnmTextureSRTKey* key = (J3DAnmTextureSRTKey*)dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), (u16)mModel.mID.field_0x10);
            if (key == NULL) {
                // ESC_WARNING: The specified btk animation could not be found! (%d)
                OS_REPORT("ESC_WARNING指定btkアニメーションが見つかりません！(%d)\n", mModel.mID.field_0x10);
                return 1;
            }

            if (mModel.mpBtkAnm->init(modelData, key, 1, J3DFrameCtrl::EMode_NULL, 1.0f, 0, -1) == 0) {
                return 0;
            }

            if ((mModel.mID.field_0x10 & 0x10000000U) != 0) {
                uVar1 |= 0x1200;
            } else {
                uVar1 |= 0x200;
            }
        }

        if (mModel.mID.field_0x14 != -1) {
            mModel.mpBrkAnm = new mDoExt_brkAnm();
            if (mModel.mpBrkAnm == NULL) {
                return 0;
            }

            J3DAnmTevRegKey* regKey = (J3DAnmTevRegKey*)dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), (u16)mModel.mID.field_0x14);
            if (regKey == NULL) {
                // ESC_WARNING: The specified brk animation could not be found! (%d)
                OS_REPORT("ESC_WARNING指定brkアニメーションが見つかりません！(%d)\n", mModel.mID.field_0x14);
                return 1;
            }

            if (mModel.mpBrkAnm->init(modelData, regKey, 1, J3DFrameCtrl::EMode_NULL, 1.0f, 0, -1) == 0) {
                return 0;
            }
        }

        if (mModel.mID.field_0x8 != -1) {
            mModel.mpBpkAnm = new mDoExt_bpkAnm();
            if (mModel.mpBpkAnm == NULL) {
                return 0;
            }

            J3DAnmColor* anm_color = (J3DAnmColor*)dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), (u16)mModel.mID.field_0x8);
            if (anm_color == NULL) {
                // ESC_WARNING: The specified brk animation could not be found! (%d)
                OS_REPORT("ESC_WARNING指定brkアニメーションが見つかりません！(%d)\n", mModel.mID.field_0x8);
                return 1;
            }

            if (mModel.mpBpkAnm->init(modelData, anm_color, 1, J3DFrameCtrl::EMode_NULL, 1.0f, 0, -1) == 0) {
                return 0;
            }

            uVar1 |= 1;
        }

        if (mModel.mID.field_0x4 == -1) {
            mModel.mpModelMorf = NULL;
            if (field_0x6ad == 0) {
                mModel.field_0x5d4 = mDoExt_J3DModel__create(modelData, 0x80000, uVar1);
            } else {
                mModel.field_0x5d4 = mDoExt_J3DModel__create(modelData, 0, uVar1);
            }

            if (mModel.field_0x5d4 == NULL) {
                return 0;
            }
        } else {
            J3DAnmTransform* anm = (J3DAnmTransform*)dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), (u16)mModel.mID.field_0x4);
            JUT_ASSERT(580, anm != NULL);

            if (field_0x6a5 != 0) {
                uVar1 |= 0x20000000;
            }

            if (field_0x6ad == 0) {
                mModel.mpModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, anm, -1, 1.0f, 0, -1, &mSound, 0x80000, uVar1);
            } else {
                mModel.mpModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, anm, -1, 1.0f, 0, -1, &mSound, 0, uVar1);
            }

            if (mModel.mpModelMorf == NULL || mModel.mpModelMorf->getModel() == NULL) {
                return 0;
            }

            mModel.field_0x5d4 = mModel.mpModelMorf->getModel();

            if (!awaCheck(mModel.field_0x5d4)) {
                return 0;
            }
        }

        if (field_0x6b4 != 0) {
            mModel.field_0x5d8 = new mDoExt_invisibleModel();
            if (mModel.field_0x5d8 == NULL) {
                return 0;
            }

            if (mModel.field_0x5d8->create(mModel.field_0x5d4, 1) == 0) {
                return 0;
            }
        } else {
            mModel.field_0x5d8 = NULL;
        }

        mModel.mID.field_0x18 = 1;
        if (mModel.mID.field_0x18 != -1) {
            mModel.mShadow = new daDemo00_shadow_c();
            if (mModel.mShadow == NULL) {
                return 0;
            }

            #if DEBUG
            mModel.field_0x5d4->getBaseTRMtx()[0][0] = 1.0f;
            #endif
            mModel.field_0x5d4->calc();
            setShadowSize();
        }

        mModel.mBgc = new daDemo00_bgc_c();
        if (mModel.mBgc == NULL) {
            return 0;
        }
        mModel.mBgc->mGndChk.OffWall();

        if (mModel.mID.field_0x1c != -1) {
            mModel.mDeformData = (J3DDeformData*)dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), (u16)mModel.mID.field_0x1c);
            JUT_ASSERT(687, mModel.mDeformData != NULL);

            if (mModel.mID.field_0x20 != -1) {
                mModel.mpBlkAnm = new mDoExt_blkAnm();
                if (mModel.mpBlkAnm == NULL) {
                    return 0;
                }

                J3DAnmCluster* anm_cluster = (J3DAnmCluster*)dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), (u16)mModel.mID.field_0x20);
                if (anm_cluster == NULL) {
                    // ESC_WARNING: Specified blk animation not found! (%d)
                    OS_REPORT("ESC_WARNING指定blkアニメーションが見つかりません！(%d)\n", mModel.mID.field_0x20);
                } else {
                    if (mModel.mpBlkAnm->init(mModel.mDeformData, anm_cluster, 1, J3DFrameCtrl::EMode_NULL, 1.0f, 0, -1) == 0) {
                        return 0;
                    }
                }
            }

            s32 ret = mModel.field_0x5d4->setDeformData(mModel.mDeformData, 1);
            if (ret != kJ3DError_Success) {
                return 0;
            }
        }
    }

    if (field_0x6a7 >= 0 && field_0x6a7 <= 5) {
        if (field_0x6bc.init(1, 0x20, (ResTIMG*)dComIfG_getObjectRes("Always", 0x54), 1) == 0) {
            return 0;
        }

        f32* pfVar1 = field_0x6bc.getSize(0);
        for (int i = 0; i < 32; i++, pfVar1++) {
            if (field_0x6a7 >= 2 && field_0x6a7 <= 4) {
                *pfVar1 = 5.0f;
            } else {
                *pfVar1 = 3.0f;
            }
        }
    }

    if (field_0x6a7 == 8 && field_0x2900.init(0x16, 0x10, 1) == 0) {
        return 0;
    }

    return 1;
}

int daDemo00_c::actStandby(dDemo_actor_c* actor) {
    if (field_0x588.mShapeID != -1) {
        mModel.mID = field_0x588;
        u32 heapSize = 0x4c5e0;
        heapSize |= 0x80000000;
        heapSize |= 0x10000000;
        if (fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize)) {
            OS_REPORT("汎用くん確保ヒープサイズ %d\n", heap->getHeapSize());

            if (mModel.field_0x5d4 != NULL) {
                dDemo_setDemoData(this, 42, NULL, NULL, 0, NULL, 0, 0);
                setBaseMtx();
                fopAcM_SetMtx(this, mModel.field_0x5d4->getBaseTRMtx());
                actor->setModel(mModel.field_0x5d4);

                if (mModel.mpModelMorf != NULL) {
                    actor->setAnmFrameMax(mModel.mpModelMorf->getEndFrame());
                }
            }

            setAction(&daDemo00_c::actPerformance);
            action(actor);
        }
    } else {
        if (field_0x6aa == 1 || field_0x6ab >= 0 || field_0x6b8 != 0 || field_0x6ae != 0) {
            dDemo_setDemoData(this, 0x8E, NULL, NULL, 0, NULL, 0, 0);
        }
    }

    return 1;
}

int daDemo00_c::actPerformance(dDemo_actor_c* actor) {
    f32 fVar1;
    if (mModel.mID.mShapeID != field_0x588.mShapeID) {
        mModel.reset();
        setAction(&daDemo00_c::actLeaving);
    } else if (mModel.field_0x5d4 != NULL) {
        if (mModel.mpModelMorf != NULL && mModel.mID.field_0x4 != field_0x588.field_0x4) {
            J3DAnmTransform* anm = (J3DAnmTransform*)dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), (u16)field_0x588.field_0x4);
            if (anm == NULL) {
                // ESC_WARNING: Specified bck animation not found! (%d)
                OS_REPORT("ESC_WARNING指定bckアニメーションが見つかりません！(%d)\n", field_0x588.field_0x4);
                return 1;
            }

            fVar1 = 0.0f;
            if (actor->checkEnable(dDemo_actor_c::ENABLE_ANM_TRANSITION_e)) {
                fVar1 = actor->getAnmTransition();
            }
            mModel.mpModelMorf->setAnm(anm, -1, fVar1, 1.0f, 0.0f, -1.0f);
            mModel.mID.field_0x4 = field_0x588.field_0x4;
        }

        if (mModel.mID.field_0xc != field_0x588.field_0xc) {
            J3DAnmTexPattern* anmTexPattern = (J3DAnmTexPattern*)dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), (u16)field_0x588.field_0xc);
            if (anmTexPattern == NULL) {
                // ESC_WARNING: The specified btp animation could not be found! (%d)
                OS_REPORT("ESC_WARNING指定btpアニメーションが見つかりません！(%d)\n", field_0x588.field_0xc);
                return 1;
            }

            mModel.mpBtpAnm->init(mModel.field_0x5d4->getModelData(), anmTexPattern, 1, J3DFrameCtrl::EMode_NULL, 1.0f, 0, -1);
            mModel.mID.field_0xc = field_0x588.field_0xc;
        }

        if (mModel.mID.field_0x10 != field_0x588.field_0x10) {
            J3DAnmTextureSRTKey* key = (J3DAnmTextureSRTKey*)dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), (u16)field_0x588.field_0x10);
            if (key == NULL) {
                // ESC_WARNING: The specified btk animation could not be found! (%d)
                OS_REPORT("ESC_WARNING指定btkアニメーションが見つかりません！(%d)\n", field_0x588.field_0x10);
                return 1;
            }

            mModel.mpBtkAnm->init(mModel.field_0x5d4->getModelData(), key, 1, J3DFrameCtrl::EMode_NULL, 1.0f, 0, -1);
            mModel.mID.field_0x10 = field_0x588.field_0x10;
        }

        if (mModel.mID.field_0x14 != field_0x588.field_0x14) {
            J3DAnmTevRegKey* anmTev = (J3DAnmTevRegKey*)dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), (u16)field_0x588.field_0x14);
            if (anmTev == NULL) {
                // ESC_WARNING: The specified brk animation could not be found! (%d)
                OS_REPORT("ESC_WARNING指定brkアニメーションが見つかりません！(%d)\n", field_0x588.field_0x14);
                return 1;
            }

            if ((mModel.mID.field_0x14 & 0x10000000) != 0) {
                fVar1 = mModel.mpBrkAnm->getFrame();
            } else {
                fVar1 = 0.0f;
            }

            s16 start = fVar1;

            int attribute;
            if ((mModel.mID.field_0x14 & 0x10000000) != 0) {
                attribute = J3DFrameCtrl::EMode_LOOP;
            } else {
                attribute = J3DFrameCtrl::EMode_NULL;
            }

            mModel.mpBrkAnm->init(mModel.field_0x5d4->getModelData(), anmTev, 1, attribute, 1.0f, start, -1);
            mModel.mID.field_0x14 = field_0x588.field_0x14;
        }

        if (mModel.mID.field_0x8 != field_0x588.field_0x8) {
            J3DAnmColor* anm_color = (J3DAnmColor*)dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), (u16)field_0x588.field_0x8);
            if (anm_color == NULL) {
                // ESC_WARNING: The specified bpk animation could not be found! (%d)
                OS_REPORT("ESC_WARNING指定bpkアニメーションが見つかりません！(%d)\n", field_0x588.field_0x8);
                return 1;
            }

            if ((mModel.mID.field_0x8 & 0x10000000) != 0) {
                fVar1 = mModel.mpBpkAnm->getFrame();
            } else {
                fVar1 = 0.0f;
            }

            s16 start = fVar1;

            int attribute;
            if ((mModel.mID.field_0x8 & 0x10000000) != 0) {
                attribute = J3DFrameCtrl::EMode_LOOP;
            } else {
                attribute = J3DFrameCtrl::EMode_NULL;
            }

            mModel.mpBpkAnm->init(mModel.field_0x5d4->getModelData(), anm_color, 1, attribute, 1.0f, start, -1);
            mModel.mID.field_0x8 = field_0x588.field_0x8;
        }

        if (mModel.mID.field_0x20 != field_0x588.field_0x20) {
            JUT_ASSERT(1049, mModel.mDeformData != NULL && mModel.mpBlkAnm != NULL);

            J3DAnmCluster* anmCluster = (J3DAnmCluster*)dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), (u16)field_0x588.field_0x20);
            if (anmCluster == NULL) {
                // ESC_WARNING: The specified btp animation could not be found! (%d)
                OS_REPORT("ESC_WARNING指定btpアニメーションが見つかりません！(%d)\n", field_0x588.field_0x20);
                return 1;
            }

            mModel.mpBlkAnm->init(mModel.mDeformData, anmCluster, 1, J3DFrameCtrl::EMode_NULL, 1.0f, 0, -1);
            mModel.mID.field_0x20 = field_0x588.field_0x20;
        }

        dDemo_setDemoData(this, 0x2A, 0, 0, 0, 0, 0, 0);
        if (mModel.mBgc != NULL) {
            cXyz sp70(current.pos.x, current.pos.y + 100.0f, current.pos.z);
            mModel.mBgc->mGndChk.SetPos(&sp70);
            mModel.mBgc->field_0x54 = dComIfG_Bgsp().GroundCross(&mModel.mBgc->mGndChk);
            field_0x6a1 = 1;
        }

        setBaseMtx();

        if (actor->checkEnable(dDemo_actor_c::ENABLE_ANM_FRAME_e)) {
            fVar1 = actor->getAnmFrame();
            if (fVar1 > 1.0f) {
                fVar1 -= 1.0f;
                if (mModel.mpModelMorf != NULL) {
                    mModel.mpModelMorf->setFrameF(fVar1);
                    mModel.mpModelMorf->play(
                        mModel.mBgc != NULL && field_0x6a1 != 0 && fabsf(mModel.mBgc->field_0x54 - current.pos.y) < 20.0f ?
                            dComIfG_Bgsp().GetMtrlSndId(mModel.mBgc->mGndChk) :
                            0,
                        dComIfGp_getReverb(dComIfGp_roomControl_getStayNo())
                    );
                }

                if (mModel.mpBtpAnm != NULL) {
                    mModel.mpBtpAnm->setFrame(fVar1);
                    mModel.mpBtpAnm->play();
                }

                if (mModel.mpBtkAnm != NULL) {
                    if ((mModel.mID.field_0x10 & 0x10000000) == 0) {
                        mModel.mpBtkAnm->setFrame(fVar1);
                    }
                    mModel.mpBtkAnm->play();
                }

                if (mModel.mpBrkAnm != NULL) {
                    if ((mModel.mID.field_0x14 & 0x10000000) == 0) {
                        mModel.mpBrkAnm->setFrame(fVar1);
                    }
                    mModel.mpBrkAnm->play();
                }

                if (mModel.mpBpkAnm != NULL) {
                    if ((mModel.mID.field_0x8 & 0x10000000) == 0) {
                        mModel.mpBpkAnm->setFrame(fVar1);
                    }
                    mModel.mpBpkAnm->play();
                }

                if (mModel.mpBlkAnm != NULL) {
                    if ((mModel.mID.field_0x20 & 0x10000000) == 0) {
                        mModel.mpBlkAnm->setFrame(fVar1);
                    }
                    mModel.mpBlkAnm->play();
                }
            } else {
                if (mModel.mpModelMorf != NULL) {
                    mModel.mpModelMorf->setFrameF(fVar1);
                }

                if (mModel.mpBtpAnm != NULL) {
                    mModel.mpBtpAnm->setFrame(fVar1);
                }

                if (mModel.mpBtkAnm != NULL) {
                    if ((mModel.mID.field_0x10 & 0x10000000) == 0) {
                        mModel.mpBtkAnm->setFrame(fVar1);
                    } else {
                        mModel.mpBtkAnm->play();
                    }
                }

                if (mModel.mpBrkAnm != NULL) {
                    if ((mModel.mID.field_0x14 & 0x10000000) == 0) {
                        mModel.mpBrkAnm->setFrame(fVar1);
                    } else {
                        mModel.mpBrkAnm->play();
                    }
                }

                if (mModel.mpBpkAnm != NULL) {
                    if ((mModel.mID.field_0x8 & 0x10000000) == 0) {
                        mModel.mpBpkAnm->setFrame(fVar1);
                    } else {
                        mModel.mpBpkAnm->play();
                    }
                }

                if (mModel.mpBlkAnm != NULL) {
                    if ((mModel.mID.field_0x20 & 0x10000000) == 0) {
                        mModel.mpBlkAnm->setFrame(fVar1);
                    } else {
                        mModel.mpBlkAnm->play();
                    }
                }
            }
        } else if (mModel.mpModelMorf != NULL) {
            mModel.mpModelMorf->play(0, 0);
        } else if (mModel.mpBtpAnm != NULL) {
            mModel.mpBtpAnm->play();
        } else if (mModel.mpBtkAnm != NULL) {
            mModel.mpBtkAnm->play();
        } else if (mModel.mpBrkAnm != NULL) {
            mModel.mpBrkAnm->play();
        } else if (mModel.mpBpkAnm != NULL) {
            mModel.mpBpkAnm->play();
        } else if (mModel.mpBlkAnm != NULL) {
            mModel.mpBlkAnm->play();
        }

        if (actor->checkEnable(dDemo_actor_c::ENABLE_SCALE_e)) {
            scale = actor->getScale();
        }
    }

    return 1;
}

int daDemo00_c::actLeaving(dDemo_actor_c* actor) {
    if (mModel.mpModelMorf != NULL) {
        mModel.mpModelMorf->stopZelAnime();
    }

    fopAcM_DeleteHeap(this);
    setAction(&daDemo00_c::actStandby);
    return 1;
}

inline int daDemo00_c::execute() {
    field_0x6a1 = 0;
    dDemo_actor_c* actor = dDemo_c::getActor(demoActorID);

    if (actor == NULL) {
        fopAcM_delete(this);
        OS_REPORT("汎用くん<dactor%d>削除！！\n", argument);
    } else {
        if (actor->checkEnable(dDemo_actor_c::ENABLE_SHAPE_e)) {
            field_0x588.mShapeID = actor->getShapeId();
        }

        if (actor->checkEnable(dDemo_actor_c::ENABLE_ANM_e)) {
            field_0x588.field_0x4 = actor->getAnmId();
        }

        int sp48, sp44, sp40;
        u16 sp0E;
        u8 sp09;
        if (actor->checkEnable(dDemo_actor_c::ENABLE_UNK_e)) {
            dDemo_prm_c* sp3C = actor->getPrm();
            while (actor->getDemoIDData(&sp48, &sp44, &sp40, &sp0E, &sp09) != 0) {
                if (sp48 == 0) {
                    switch (sp40) {
                        case 0: {
                            u16 sp0C = (sp0E >> 8) & 0xFF;
                            u16 resID8 = sp0E & 0xFF; // r29
                            switch (sp0C) {
                                case 1:
                                    switch (resID8) {
                                        case 1:
                                            field_0x6b4 = 1;
                                            break;

                                        case 2:
                                            field_0x6a5 = 1;
                                            break;

                                        case 3:
                                            field_0x6a5 = 2;
                                            break;

                                        case 4:
                                            field_0x6a6 = 1;
                                            break;

                                        case 5:
                                            field_0x6a6 = 2;
                                            break;

                                        case 6:
                                            field_0x6a6 = 3;
                                            break;

                                        case 0:
                                            break;
                                    }
                                    break;

                                case 2:
                                    if (resID8 == 0) {
                                        field_0x6a2 = 0;
                                        mground2 = 0;
                                    } else if (resID8 == 1) {
                                        field_0x6a2 = 1;
                                    } else {
                                        mground2 = 1;
                                    }

                                    OS_REPORT("\nmground2=[%d]", mground2);
                                    break;

                                case 3: {
                                    static u8 const l_itemNo[4] = {
                                        0x29,
                                        0xFF,
                                        0xFF,
                                        0xFF,
                                    };
                                    JUT_ASSERT(1389, resID8 < (sizeof(l_itemNo)/sizeof(u8)));
                                    u8 itemNo = l_itemNo[resID8];
                                    if (itemNo != 0xFF) {
                                        execItemGet(itemNo);
                                    }
                                    break;
                                }
                                case 4:
                                case 5:
                                case 6:
                                case 7: {
                                    BOOL bVar1 = FALSE;
                                    BOOL bVar2 = FALSE;
                                    int sp30 = resID8;
                                    if (sp0C == 5 || sp0C == 7) {
                                        bVar1 = TRUE;
                                    }

                                    if (sp0C == 6 || sp0C == 7) {
                                        bVar2 = TRUE;
                                    }

                                    if (sp30 == 0xFF) {
                                        sp30 = 0x1A;
                                    }

                                    switch (bVar1) {
                                    case FALSE:
                                        mDoGph_gInf_c::fadeIn(1.0f / sp30, !bVar2 ? g_blackColor : g_saftyWhiteColor);
                                        break;
                                    case TRUE:
                                        mDoGph_gInf_c::fadeOut(1.0f / sp30, !bVar2 ? g_blackColor : g_saftyWhiteColor);
                                        break;
                                    }
                                    break;
                                }
                                case 8: {
                                    u32 sp2C = (resID8 & 0xC0) >> 6;
                                    u32 sp28 = resID8 & 0x3F;

                                    switch (sp2C) {
                                        case 0:
                                            dComIfGp_getVibration().StopQuake(1);
                                            break;

                                        case 1:
                                           dComIfGp_getVibration().StartShock(sp28, 1, cXyz(0.0f, 1.0f, 0.0f));
                                           break;
                                           
                                        case 2:
                                            dComIfGp_getVibration().StartQuake(sp28, 1, cXyz(0.0f, 1.0f, 0.0f));
                                            break;

                                        default:
                                            // Demo General-Kun Vibration Setting Error!!
                                            OS_REPORT("\nデモ汎用君振動設定エラー!!");
                                            break;
                                    }
                                    break;
                                }
                                case 9:
                                    if (resID8 == 0) {
                                        field_0x6a4 = 0;
                                    } else {
                                        field_0x6a4 = 1;
                                    }
                                    break;

                                case 10: {
                                    int sp24 = resID8 & 0xFF;
                                    if (sp24 < 0x40) {
                                        dKy_change_colpat(sp24);
                                        OS_REPORT("\nパレット型[%d]へ変更", sp24);
                                    }
                                    break;
                                }
                                case 11: {
                                    u32 sp20 = resID8;
                                    switch (sp20) {
                                        case 0:
                                            g_env_light.mThunderEff.field_0x2 = 1;
                                            break;

                                        case 1:
                                            field_0x6b2 = 1;
                                            break;

                                        case 2:
                                            field_0x6b2 = 2;
                                            break;

                                        case 3:
                                            field_0x69c = 0xFA;
                                            break;

                                        case 4:
                                            field_0x69c = 0;
                                            break;

                                        case 5:
                                            field_0x69e = 0x32;
                                            break;

                                        case 6:
                                            field_0x69e = 0;
                                            break;
                                    }
                                    break;
                                }
                                case 12: {
                                    u32 sp1C = resID8;
                                    switch (sp1C) {
                                        case 0:
                                            field_0x6a7 = 0;
                                            break;

                                        case 1:
                                            field_0x6a7 = 2;
                                            break;

                                        case 2:
                                            field_0x6a7 = 3;
                                            break;

                                        case 3:
                                            field_0x6a7 = 1;
                                            break;

                                        case 4:
                                            field_0x6a7 = 8;
                                            break;

                                        case 5:
                                            field_0x6a7 = 4;
                                            break;

                                        case 6:
                                            field_0x6b6 = 1;
                                            break;

                                        case 7:
                                            field_0x6b6 = 2;
                                            break;

                                        case 8:
                                            field_0x6a7 = 6;
                                            break;

                                        case 9:
                                            field_0x6a7 = 7;
                                            break;

                                        case 10:
                                            field_0x6a7 = 5;
                                            break;
                                    }
                                    break;
                                }
                                case 13:
                                    field_0x6a9 = resID8;
                                    break;

                                case 14:
                                    field_0x6a8 = resID8;
                                    break;

                                case 15:
                                    if (resID8 == 1) {
                                        field_0x6aa = 1;
                                    } else {
                                        field_0x6aa = 0;
                                    }
                                    break;

                                case 16:
                                    field_0x6ab = resID8;
                                    break;

                                case 17:
                                    switch (resID8) {
                                        case 1:
                                            field_0x6ac = 2;
                                            break;

                                        case 2:
                                            field_0x6ac = 4;
                                            break;

                                        case 3:
                                            field_0x6ac = 7;
                                            break;

                                        case 4:
                                            field_0x6ac = 9;
                                            break;

                                        case 5:
                                            field_0x6ac = 1;
                                            break;

                                        case 6:
                                            field_0x6ac = 16;
                                            break;

                                        case 7:
                                            field_0x6ac = 17;
                                            break;

                                        case 8:
                                            field_0x6ac = 18;
                                            break;

                                        case 9:
                                            field_0x6ac = 19;
                                            break;

                                        case 10:
                                            field_0x6ac = 10;
                                            break;

                                        case 11:
                                            field_0x6ac = 14;
                                            break;

                                        case 12:
                                            field_0x6ac = 3;
                                            break;

                                        case 13:
                                            field_0x6ac = 5;
                                            break;

                                        case 14:
                                            field_0x6ac = 15;
                                            break;
                                    }
                                    break;

                                case 18:
                                    field_0x6ae = 1;
                                    break;

                                case 19:
                                    field_0x6af = resID8;
                                    break;

                                case 20:
                                    field_0x6b0 = resID8;
                                    break;

                                case 21:
                                    field_0x6b1 = 1;
                                    break;

                                case 22:
                                    field_0x6b5 = resID8;
                                    break;

                                case 23:
                                    field_0x6b7 = resID8;
                                    break;

                                case 24:
                                    field_0x6b8 = 1;
                                    break;

                                case 25:
                                    cXyz sp90, sp9c;
                                    sp90.x = 0.0f;
                                    sp90.y = 0.0f;
                                    sp90.z = 0.0f;
                                    sp9c.x = 1.0f;
                                    sp9c.y = 1.0f;
                                    sp9c.z = 1.0f;
                                    field_0x6b9 = 1;
                                    dComIfGp_particle_set((u16)field_0x588.mShapeID, &sp90, NULL, NULL, &sp9c);
                                    field_0x588.reset();
                                    field_0x588.mShapeID = -1;
                                    actor->offEnable(16);
                                    break;
                            }
                            break;
                        }
                        case 1:
                            dComIfGs_onEventBit((u16)dSv_event_flag_c::saveBitLabels[sp0E]);
                            break;

                        case 2: {
                            u16 sp0A = sp0E & 0x3FFF;
                            if ((sp0E & 0xC000) == 0) {
                                fopAcM_create(PROC_MOVIE_PLAYER, sp0A, NULL, fopAcM_GetRoomNo(this), NULL, NULL, 0xFF);
                                mDoGph_gInf_c::fadeOut(1.0f);
                            } else {
                                switch (sp0A) {
                                    case 0:
                                        daMP_c::daMP_c_THPPlayerPlay();
                                        break;

                                    case 1:
                                        daMP_c::daMP_c_THPPlayerPause();
                                        break;
                                }
                            }
                            break;
                        }
                        case 3:
                            dComIfGs_onTmpBit((u16)dSv_event_tmp_flag_c::tempBitLabels[sp0E]);
                            break;
                    }
                } else {
                    switch (sp40) {
                        case 1:
                            field_0x588.reset();
                            field_0x588.mShapeID = sp0E;
                            break;

                        case 2:
                            field_0x588.field_0x4 = sp0E;
                            break;

                        case 3:
                            if (sp09 == 0) {
                                field_0x588.field_0x10 = sp0E;
                            } else {
                                field_0x588.field_0x10 = sp0E | 0x10000000;
                            }
                            break;

                        case 4:
                            if (sp09 == 0) {
                                field_0x588.field_0x14 = sp0E;
                            } else {
                                field_0x588.field_0x14 = sp0E | 0x10000000;
                            }
                            break;

                        case 5:
                            field_0x588.field_0xc = sp0E;
                            break;

                        case 6:
                            field_0x588.field_0x1c = sp0E;
                            break;

                        case 7:
                            field_0x588.field_0x20 = sp0E;
                            break;

                        case 8:
                            field_0x588.field_0x8 = sp0E;
                            break;
                    }
                }
            }

            actor->offEnable(dDemo_actor_c::ENABLE_UNK_e);
        }

        action(actor);

        #if WIDESCREEN_SUPPORT
        if (field_0x6b1 != 0) {
            mDoGph_gInf_c::onWideZoom();
        }
        #endif
    }

    if (field_0x6ae != 0) {
        f32 cutoff = 90.0f;
        cutoff = shape_angle.z / 182.04445f;
        GXColor color;
        color.r = scale.x;
        color.g = scale.y;
        color.b = scale.z;
        color.a = 0xFF;
        f32 f30 = shape_angle.x / 182.04445f;
        f32 f29 = shape_angle.y / 182.04445f;
        dKy_BossSpotLight_set(&current.pos, f30, f29 - 90.0f, 
                              cutoff, &color, gravity, field_0x6b0, field_0x6af);
    }

    if (field_0x6b8 != 0) {
        dComIfGs_setTime(current.pos.x * 15.0f);
    }

    return 1;
}

static void mDad00_changeXluMaterial(J3DMaterial* i_material, int param_2) {
    static J3DBlendInfo l_blendInfoOPA = {
        0,
        1,
        0,
        3,
    };
    static J3DBlendInfo l_blendInfo = {
        1,
        4,
        5,
        3,
    };
    static J3DZModeInfo l_zmodeInfoOPA = {
        1,
        3,
        1,
        0,
    };
    static J3DZModeInfo l_zmodeInfo = {
        1,
        3,
        0,
        0,
    };

    i_material->change();

    if (param_2 == 0) {
        i_material->setMaterialMode(4);
        J3DTevBlock* tevBlock = i_material->getTevBlock();
        J3DPEBlock* peBlock = i_material->getPEBlock();
        u8 tevStageNum = tevBlock->getTevStageNum();
        peBlock->getBlend()->setBlendInfo(l_blendInfo);
        peBlock->getZMode()->setZModeInfo(l_zmodeInfo);
    } else {
        i_material->setMaterialMode(1);
        J3DTevBlock* tevBlock = i_material->getTevBlock();
        J3DPEBlock* peBlock = i_material->getPEBlock();
        u8 tevStageNum = tevBlock->getTevStageNum();
        peBlock->getBlend()->setBlendInfo(l_blendInfoOPA);
        peBlock->getZMode()->setZModeInfo(l_zmodeInfoOPA);
    }
}

static void teduna_calc(cXyz* param_1, cXyz* param_2, cXyz* param_3, s16 param_4, int param_5) {
    cXyz sp70(*param_1 - *param_2);
    f32 f31;
    f32 f30 = 6.0f;
    cXyz sp7c, sp88;

    mDoMtx_stack_c::YrotS(param_4);

    if (param_5 == 1) {
        sp7c.set(0.0f, -5.0f, 130.0f);
        f30 = 1.0f;
    } else if (param_5 == 5) {
        sp7c.set(0.0f, -30.0f, 78.0f);
    } else {
        sp7c.set(0.0f, -30.0f, 60.0f);
    }

    mDoMtx_stack_c::multVec(&sp7c, &sp88);

    for (int i = 0; i < 16; i++, param_3++) {
        *param_3 = *param_1 - (sp70 * (i / 15.0f));
        f31 = cM_ssin((i / 15.0f) * 32768.0f);
        *param_3 += sp88 * f31;
        f32 f29 = f30 * cM_ssin(g_Counter.mCounter0 * 2500 + i * 1600);
        param_3->y += f31 * f29;
    }
}

static cXyz teduna_posL[16];

static cXyz teduna_posR[16];

static cXyz S_ganon_left_hand_pos;

static cXyz S_ganon_right_hand_pos;

static void teduna_draw(J3DModel* i_model, mDoExt_3DlineMat1_c* param_2, dKy_tevstr_c* param_3, int param_4, int param_5, int param_6, int param_7) {
    static GXColor l_color = {
        0x14, 0x0F, 0x00, 0xFF,
    };

    cXyz sp38, sp44, sp50;
    s16 sVar1 = 0;
    if (param_7 == 4) {
        sVar1 = -0x3875;
    } else if (param_7 == 5) {
        sVar1 = -7000;
    }

    MTXCopy(i_model->getAnmMtx(param_6), mDoMtx_stack_c::get());

    if (param_7 == 5) {
        sp38.set(0.0f, 9.0f, 15.0f);
    } else if (param_7 == 2 || param_7 == 3 || param_7 == 4) {
        sp38.set(107.0f, -32.0f, -68.0f);
    } else {
        sp38.set(79.0f, -26.0f, -48.0f);
    }

    mDoMtx_stack_c::multVec(&sp38, &sp44);

    if (param_7 == 5) {
        sp50 = S_ganon_left_hand_pos;
    } else {
        MTXCopy(i_model->getAnmMtx(param_4), mDoMtx_stack_c::get());

        if (param_7 == 1) {
            sp38.set(61.0f, 18.0f, 0.0f);
        } else if (param_7 == 3) {
            sp38.set(191.0f, 28.0f, -80.0f);
        } else {
            sp38.set(0.0f, 0.0f, 0.0f);
        }

        mDoMtx_stack_c::multVec(&sp38, &sp50);
    }

    teduna_calc(&sp44, &sp50, teduna_posL, (sVar1 + 0x6000) + cM_atan2s(sp44.x - sp50.x, sp44.z - sp50.z), param_7);
    MTXCopy(i_model->getAnmMtx(param_6), mDoMtx_stack_c::get());

    if (param_7 == 5) {
        sp38.set(0.0f, 9.0f, -15.0f);
    } else if (param_7 == 2 || param_7 == 3 || param_7 == 4) {
        sp38.set(107.0f, -32.0f, 68.0f);
    } else {
        sp38.set(79.0f, -26.0f, 48.0f);
    }

    mDoMtx_stack_c::multVec(&sp38, &sp44);

    if (param_7 == 5) {
        sp50 = S_ganon_right_hand_pos;
    } else {
        MTXCopy(i_model->getAnmMtx(param_5), mDoMtx_stack_c::get());

        if (param_7 == 1) {
            sp38.set(61.0f, 18.0f, 0.0f);
        } else if (param_7 == 4) {
            sp38.set(40.0f, -12.0f, 40.0f);
        } else {
            sp38.set(0.0f, 0.0f, 0.0f);
        }

        mDoMtx_stack_c::multVec(&sp38, &sp50);
    }

    teduna_calc(&sp44, &sp50, teduna_posR, cM_atan2s(sp44.x - sp50.x, sp44.z - sp50.z) - (sVar1 + 0x6000), param_7);
    cXyz* src = teduna_posL;
    cXyz* dst = param_2->getPos(0);
    for (int i = 0; i < 16; i++, dst++, src++) {
        *dst = *src;
    }
    src = teduna_posR;
    dst = param_2->getPos(0) + 31;
    for (int i = 0; i < 16; i++, dst--, src++) {
        *dst = *src;
    }

    param_2->update(0x20, l_color, param_3);
    dComIfGd_set3DlineMat(param_2);
}

static void teduna_ganon_hand_set(J3DModel* i_model, int param_2, int param_3) {
    cXyz sp20;

    MTXCopy(i_model->getAnmMtx(param_2), mDoMtx_stack_c::get());
    sp20.set(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&sp20, &S_ganon_left_hand_pos);

    MTXCopy(i_model->getAnmMtx(param_3), mDoMtx_stack_c::get());
    sp20.set(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&sp20, &S_ganon_right_hand_pos);
}

static void ke_control(daDemo00_c* i_this, demo_s1_ke_s* param_2, int param_3, f32 param_4) {
    param_3;
    fopAc_ac_c* actor = i_this;
    int i;
    cXyz sp4C, sp40;
    cXyz* pcVar1 = &param_2->field_0x0[1];
    cXyz* pcVar2 = &param_2->field_0xc0[1];

    sp4C.x = 0.0f;
    sp4C.y = 0.0f;
    sp4C.z = param_4;

    cXyz sp34;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    s16 sp14;
    s16 sp12;
    s16 sp10;

    f32 f30;
    f32 f29;
    f32 f28;
    f32 f27;
    f32 f26;
    f32 f25;
    f32 f31;

    sp24 = -12.0f;
    sp10 = cM_rndF2(65536.0f);
    f26 = 3.0f;
    f25 = actor->current.pos.y;
    f31 = 0.8f;


    if (i_this->field_0x6b3 != 0) {
        f31 = 0.0f;
    }

    for (i = 1; i < 16; i++, pcVar1++, pcVar2++) {
        sp20 = f26 * cM_ssin(sp10 + i * 7000);
        sp1C = f26 * cM_ssin(sp10 + 10000 + i * 6000);
        f27 = (16 - i) * 0.1f;

        sp34.x = param_2->field_0x180.x * f27 + sp20 + pcVar2->x;
        sp34.y = pcVar2->y + param_2->field_0x180.y * f27;
        sp34.z = param_2->field_0x180.z * f27 + sp1C + pcVar2->z;
        sp28 = sp34.x + (pcVar1->x - pcVar1[-1].x);
        f29 = sp34.z + (pcVar1->z - pcVar1[-1].z);
        f28 = pcVar1->y + sp34.y + sp24;

        if (f28 < f25) {
            f28 = f25;
        }

        f30 = f28 - pcVar1[-1].y;

        sp14 = (s16)-cM_atan2s(f30, f29);
        sp12 = (s16)cM_atan2s(sp28, JMAFastSqrt(f30 * f30 + f29 * f29));
        cMtx_XrotS(*calc_mtx, sp14);
        cMtx_YrotM(*calc_mtx, sp12);
        MtxPosition(&sp4C, &sp40);
        
        *pcVar2 = *pcVar1;
        pcVar1->x = pcVar1[-1].x + sp40.x;
        pcVar1->y = pcVar1[-1].y + sp40.y;
        pcVar1->z = pcVar1[-1].z + sp40.z;
        pcVar2->x = f31 * (pcVar1->x - pcVar2->x);
        pcVar2->y = f31 * (pcVar1->y - pcVar2->y);
        pcVar2->z = f31 * (pcVar1->z - pcVar2->z);
    }
}

static void ke_move(daDemo00_c* i_this, mDoExt_3DlineMat0_c* param_2, demo_s1_ke_s* param_3, int param_4, f32 param_5) {
    ke_control(i_this, param_3, param_4, param_5);
    cXyz* pcVar1 = param_2->getPos(param_4);
    f32* pfVar1 = param_2->getSize(param_4);

    for (int i = 0; i < 16; i++, pcVar1++, pfVar1++) {
        *pcVar1 = param_3->field_0x0[i];
        if (i == 14) {
            *pfVar1 = 0.6f;
        } else if (i < 5) {
            *pfVar1 = 3.5f;
        } else {
            *pfVar1 = cM_rndF2(1.0f) + 1.8f;
        }
    }
}

static void ke_set(daDemo00_c* i_this) {
    static s16 ke_za[22] = {
        0xFCE0, 0xFE70, 0, 0x0190, 0x0320, 0xFCE0, 0xFE70, 0, 0x0190, 0x0320, 
        0xFCE0, 0xFE70, 0, 0x0190, 0x0320, 0x04B0, 0xFCE0, 0xFE70, 0, 
        0x0190, 0x0320, 0x04B0,
    };

    fopAc_ac_c* actor = i_this;
    cXyz sp58, sp64;
    cM_initRnd2(12, 0x7B, fopAcM_GetID(actor) * 2 + 0x32);
    MTXCopy(i_this->mModel.field_0x5d4->getAnmMtx(4), *calc_mtx);

    f32 fVar1, fVar2, fVar3;
    for (int i = 0; i < 22; i++) {
        MtxPush();
        cMtx_YrotM(*calc_mtx, 0x4000);
        cMtx_XrotM(*calc_mtx, (int)cM_rndF2(2000.0f) + 3000);

        if (i < 10) {
            cMtx_ZrotM(*calc_mtx, ke_za[i] * 5);
            fVar1 = cM_rndF2(2.25f) + 7.5f;
            fVar2 = cM_rndF2(10.0f) - 5.0f;
            fVar3 = cM_rndF2(20.0f) - 10.0f;
        } else {
            cMtx_ZrotM(*calc_mtx, ke_za[i] * 5 + 0x8000);
            fVar1 = cM_rndF2(3.0f) + 8.0f;
            fVar2 = cM_rndF2(20.0f) - 10.0f;
            fVar3 = cM_rndF2(30.0f) - 15.0f;
        }

        sp58.set(fVar2, 15.0f, fVar3);
        MtxPosition(&sp58, &i_this->field_0x6f8[i].field_0x0[0]);
        sp58.set(fVar2, 35.0f, fVar3);
        MtxPosition(&sp58, &i_this->field_0x6f8[i].field_0x180);

        i_this->field_0x6f8[i].field_0x180 -= i_this->field_0x6f8[i].field_0x0[0];
        
        ke_move(i_this, &i_this->field_0x2900, &i_this->field_0x6f8[i], i, fVar1);
        MtxPull();
    }
}

static int daDemo00_Draw(daDemo00_c* i_this) {
    return i_this->draw();
}

int daDemo00_c::draw() {
    f32 f31 = 15.0f;
    if (mModel.field_0x5d4 != NULL) {
        if (field_0x6ac == 7) {
            tevStr.TevColor.a = 0xFF;
        }

        g_env_light.settingTevStruct(field_0x6ac, &eyePos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mModel.field_0x5d4, &tevStr);
        dKy_bg_MAxx_proc(mModel.field_0x5d4);

        if (mModel.mpBtpAnm != NULL) {
            mModel.mpBtpAnm->entry(mModel.field_0x5d4->getModelData());
        }

        if (mModel.mpBtkAnm != NULL) {
            mModel.mpBtkAnm->entry(mModel.field_0x5d4->getModelData());
        }

        if (mModel.mpBrkAnm != NULL){
            mModel.mpBrkAnm->entry(mModel.field_0x5d4->getModelData());
        }

        if (mModel.mpBpkAnm != NULL) {
            mModel.mpBpkAnm->entry(mModel.field_0x5d4->getModelData());
        }

        if (mModel.mpBlkAnm != NULL) {
            mModel.mpBlkAnm->entryFrame();
        }

        if (field_0x6a5 != 0) {
            J3DModelData* modelData = mModel.field_0x5d4->getModelData();
            for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
                J3DMaterial* material = modelData->getMaterialNodePointer(i);

                if (field_0x6a5 == 1) {
                    mDad00_changeXluMaterial(material, 0);
                } else {
                    mDad00_changeXluMaterial(material, 1);
                }
            }
        }

        if (field_0x6a6 == 2) {
            dComIfGd_setListDark();
        } else if (field_0x6a6 == 3) {
            dComIfGd_setListFilter();
        }

        if (mModel.field_0x5d8 != NULL) {
            mModel.mpModelMorf->modelCalc();
            if (field_0x6b5 != 0) {
                mModel.field_0x5d8->entryDL(NULL);
            }
        } else if (field_0x6b5 != 0) {
            if (mModel.mpModelMorf != NULL) {
                mModel.mpModelMorf->updateDL();
            } else {
                mDoExt_modelUpdateDL(mModel.field_0x5d4);
            }
        } else if (mModel.mpModelMorf != NULL) {
            mModel.mpModelMorf->modelCalc();
        }

        if (mModel.field_0x5d4->getSkinDeform() != NULL) {
            dComIfGd_setList();
        }

        if (field_0x6a6 == 2 || field_0x6a6 == 3) {
            dComIfGd_setList();
        }

        if (field_0x6a7 >= 0) {
            if (field_0x6a7 == 0) {
                teduna_draw(mModel.field_0x5d4, &field_0x6bc, &tevStr, 22, 0x1B, 0x27, field_0x6a7);
            } else if (field_0x6a7 == 1) {
                teduna_draw(mModel.field_0x5d4, &field_0x6bc, &tevStr, 0x2B, 0x2B, 0x27, field_0x6a7);
            } else if (field_0x6a7 == 2) {
                teduna_draw(mModel.field_0x5d4, &field_0x6bc, &tevStr, 0x17, 0x1F, 0x3E, field_0x6a7);
            } else if (field_0x6a7 == 3) {
                teduna_draw(mModel.field_0x5d4, &field_0x6bc, &tevStr, 0x1F, 0x1F, 0x3E, field_0x6a7);
            } else if (field_0x6a7 == 4) {
                teduna_draw(mModel.field_0x5d4, &field_0x6bc, &tevStr, 0x17, 0x17, 0x3E, field_0x6a7);
            } else if (field_0x6a7 == 6) {
                teduna_ganon_hand_set(mModel.field_0x5d4, 0x20, 0x21);
            } else if (field_0x6a7 == 7) {
                teduna_ganon_hand_set(mModel.field_0x5d4, 0x16, 0x17);
            } else if (field_0x6a7 == 5) {
                teduna_draw(mModel.field_0x5d4, &field_0x6bc, &tevStr, 0, 0, 0x13, field_0x6a7);
            } else if (field_0x6a7 == 8) {
                if ((current.pos - field_0x291c).abs() > 200.0f) {
                    field_0x6b3 = 5;
                    for (int i = 0; i < 10; i++) {
                        ke_set(this);
                    }
                }

                ke_set(this);
                GXColor color;
                color.r = 0x14;
                color.g = 0x14;
                color.b = 0x14;
                color.a = 0xFF;

                if (field_0x6b5 != 0) {
                    field_0x2900.update(0x10, color, &tevStr);
                    dComIfGd_set3DlineMatDark(&field_0x2900);
                }

                field_0x291c = current.pos;
                if (field_0x6b3 != 0) {
                    field_0x6b3--;
                }
            }
        }

        if (field_0x6b6 != 0) {
            if (field_0x6b6 == 1) {
                if (mModel.field_0x5d4->getModelData() != NULL) {
                    if (mModel.field_0x5d4->getModelData()->getMaterialNodePointer(9) != NULL) {
                        if (mModel.field_0x5d4->getModelData()->getMaterialNodePointer(9)->getShape() != NULL) {
                            mModel.field_0x5d4->getModelData()->getMaterialNodePointer(9)->getShape()->show();
                        }
                    }
                }
            } else if (mModel.field_0x5d4->getModelData() != NULL) {
                if (mModel.field_0x5d4->getModelData()->getMaterialNodePointer(9) != NULL) {
                    if (mModel.field_0x5d4->getModelData()->getMaterialNodePointer(9)->getShape() != NULL) {
                        mModel.field_0x5d4->getModelData()->getMaterialNodePointer(9)->getShape()->hide();
                    }
                }
            }
        }

        if (mModel.mShadow != NULL && field_0x6b7 != 0 && field_0x6b5 != 0 && field_0x6a1 != 0) {
            cXyz sp98, spa4, spb0;

            MTXCopy(mModel.field_0x5d4->getAnmMtx(0), mDoMtx_stack_c::get());
            spb0.set(0.0f, 0.0f, 0.0f);
            mDoMtx_stack_c::multVec(&spb0, &sp98);
            mDoLib_project(&sp98, &spa4);

            if (spa4.x >= -700.0f && spa4.x < 1600.0f && spa4.y >= -200.0f && spa4.y < 600.0f) {
                if (mModel.mID.field_0x18 == 0 || mModel.mID.field_0x18 == 1) {
                    cXyz spbc = sp98 + mModel.mShadow->field_0x4;
                    spbc.y = mModel.mBgc->field_0x54 + f31;
                    mModel.mShadow->field_0x0 = dComIfGd_setShadow(mModel.mShadow->field_0x0, mModel.mID.field_0x18 == 0 ? 0 : 1, mModel.field_0x5d4,
                        &spbc, mModel.mShadow->field_0x1c, mModel.mShadow->field_0x20, sp98.y + f31, sp98.y + 3.0f,
                        mModel.mBgc->mGndChk, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
                } else {
                    cXyz spc8(sp98.x, mModel.mBgc->field_0x54, sp98.z);
                    dComIfGd_setSimpleShadow(&spc8, spc8.y, mModel.mShadow->field_0x20, mModel.mBgc->mGndChk, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
                }
            }
        }

        if (mModel.mpBtpAnm != NULL) {
            mModel.mpBtpAnm->remove(mModel.field_0x5d4->getModelData());
        }

        if (mModel.mpBtkAnm != NULL) {
            mModel.mpBtkAnm->remove(mModel.field_0x5d4->getModelData());
        }

        if (mModel.mpBrkAnm != NULL) {
            mModel.mpBrkAnm->remove(mModel.field_0x5d4->getModelData());
        }

        if (mModel.mpBpkAnm != NULL) {
            mModel.mpBpkAnm->remove(mModel.field_0x5d4->getModelData());
        }

        cXyz spd4, spe0, spec;

        MTXCopy(mModel.field_0x5d4->getAnmMtx(0), mDoMtx_stack_c::get());
        spd4.set(0.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::multVec(&spd4, &spe0);
        eyePos = spe0;
        eyePos.y = spe0.y + 50.0f;

        if (field_0x6a8 > 0) {
            cXyz spf8, sp104;
            dBgS_CamGndChk_Wtr cam_gndchk;
            cXyz sp110;

            if ((g_Counter.mCounter0 & 0x1F) == 0) {
                spf8 = eyePos;
                if (field_0x6a8 == 10) {
                    sp110.x = 2.0f;
                    sp110.y = 2.0f;
                    sp110.z = 2.0f;
                } else if (field_0x6a8 == 6) {
                    cXyz sp11c, sp128, sp134;

                    MTXCopy(mModel.field_0x5d4->getAnmMtx(7), mDoMtx_stack_c::get());
                    sp11c.set(0.0f, 0.0f, 0.0f);
                    mDoMtx_stack_c::multVec(&sp11c, &sp128);
                    spf8 = sp128;
                    sp110.x = 1.0f;
                    sp110.y = 1.0f;
                    sp110.z = 1.0f;
                } else {
                    sp110.x = 1.0f;
                    sp110.y = 1.0f;
                    sp110.z = 1.0f;
                }

                spf8.y += 10000.0f;
                cam_gndchk.SetPos(&spf8);
                spf8.y = dComIfG_Bgsp().GroundCross(&cam_gndchk);
                dComIfGp_particle_setWaterRipple(&field_0x68c, *fopAcM_wt_c::getWaterCheck(), &spf8, 
                                                 0.0f, &tevStr, &sp110, fopAcM_GetRoomNo(this));
            }
        }

        #if DEBUG
        if (field_0x6aa > -1) {
            dDbVw_Report(0x32, 0x91, "DEMOTOOL ERR : Model ga arunoni hisyakai sindo meirei!!!!");
            OSReport_Error("\nDEMOTOOL ERR : Model ga arunoni hisyakai sindo meirei!!!!");
        }

        if (field_0x6ab > -1) {
            dDbVw_Report(0x32, 0x91, "DEMOTOOL ERR : Model ga arunoni monokuro meirei!!!!");
            OSReport_Error("\nDEMOTOOL ERR : Model ga arunoni monokuro meirei!!!!");
        }
        #endif
    }

    if (field_0x6a9 >= 1) {
        dCam_getBody()->StartBlure(0x19, NULL, 0.85f, 1.0f);
    }

    if (field_0x6aa != -1) {
        if (field_0x6aa == 1) {
            g_env_light.mDemoAttentionPoint = current.pos.x;

            if (g_env_light.mDemoAttentionPoint > 1.0f) {
                g_env_light.mDemoAttentionPoint = 1.0f;
            }

            if (g_env_light.mDemoAttentionPoint < -1.0f) {
                g_env_light.mDemoAttentionPoint = -1.0f;
            }

            #if DEBUG
            dDbVw_Report(300, 0x19, "DEMO Depth %f", g_env_light.mDemoAttentionPoint);
            #endif
        } else {
            cLib_addCalc(&g_env_light.mDemoAttentionPoint, 0.0f, 0.5f, 0.1f, 0.0001f);
        }
    }

    if (field_0x6ab >= 0) {
        g_env_light.field_0x12fc = field_0x6ab;
        g_env_light.field_0x1278 = current.pos.y / 100.0f;
        if (g_env_light.field_0x1278 >= 1.0f) {
            g_env_light.field_0x1278 = 1.0f;
        }

        #if DEBUG
        dDbVw_Report(10, 0x17C, "\nbloom pat[%d] ratio[%f]", g_env_light.field_0x1278, field_0x6ab);
        #endif
    }

    switch (field_0x6b2) {
        case 0:
            break;

        case 1:
            if (g_env_light.mHousiCount < 200) {
                g_env_light.mHousiCount += 3;
            }
            break;

        case 2:
            if (g_env_light.mHousiCount > 3) {
                g_env_light.mHousiCount -= 3;
            } else {
                g_env_light.mHousiCount = 0;
            }
            break;
    }

    if (field_0x69c != -1) {
        if (field_0x694 != field_0x69c) {
            if (field_0x694 < field_0x69c) {
                field_0x694++;
            } else {
                field_0x694--;
            }
        }

        dKyw_rain_set(field_0x694);
    }

    if (field_0x69e != -1) {
        if (field_0x698 != field_0x69e) {
            if (field_0x698 < field_0x69e) {
                field_0x698++;
            } else {
                field_0x698--;
            }
        }

        g_env_light.mMoyaMode = 1;
        g_env_light.mMoyaCount = field_0x698;
    }

    return 1;
}

static int daDemo00_Execute(daDemo00_c* i_this) {
    return i_this->execute();
}

static int daDemo00_IsDelete(daDemo00_c* i_this) {
    return 1;
}

static int daDemo00_Delete(daDemo00_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "Demo00");
    i_this->~daDemo00_c();
    return 1;
}

static int daDemo00_Create(fopAc_ac_c* i_this) {
    fopAcM_RegisterCreateID(daDemo00_c, i_this, "Demo00");
    fopAcM_ct(a_this, daDemo00_c);

    a_this->field_0x6a2 = 0;
    a_this->mground2 = 0;
    a_this->field_0x6a4 = 0;
    a_this->field_0x6b2 = 0;
    a_this->field_0x69c = -1;
    a_this->field_0x69e = -1;
    a_this->field_0x694 = 0;
    a_this->field_0x698 = 0;
    a_this->field_0x568.x = 10000000.0f;
    a_this->field_0x568.y = 10000000.0f;
    a_this->field_0x568.z = 10000000.0f;
    a_this->field_0x6a5 = 0;
    a_this->field_0x6a6 = 0;
    a_this->field_0x6a7 = 0xFF;
    a_this->field_0x6a8 = -1;
    a_this->field_0x6a9 = -1;
    a_this->field_0x6aa = 0xFF;
    a_this->field_0x6ab = -1;
    a_this->field_0x6ac = 0;
    a_this->field_0x6ad = 0;
    a_this->field_0x6ae = 0;
    a_this->field_0x6b8 = 0;
    a_this->field_0x6b9 = 0;
    a_this->field_0x6af = 3;
    a_this->field_0x6b0 = 2;
    a_this->field_0x6b4 = 0;
    a_this->field_0x6b1 = 0;
    a_this->field_0x6b5 = 1;
    a_this->field_0x6b6 = 0;
    a_this->field_0x6b7 = 1;
    S_ganon_left_hand_pos.x = 0.0f;
    S_ganon_left_hand_pos.y = 0.0f;
    S_ganon_left_hand_pos.z = 0.0f;
    S_ganon_right_hand_pos.x = 0.0f;
    S_ganon_right_hand_pos.y = 0.0f;
    S_ganon_right_hand_pos.z = 0.0f;

    return a_this->create(); 
}

AUDIO_INSTANCES;
class JAUSectionHeap;
template<>
JAUSectionHeap* JASGlobalInstance<JAUSectionHeap>::sInstance;

static actor_method_class l_daDemo00_Method = {
    (process_method_func)daDemo00_Create,
    (process_method_func)daDemo00_Delete,
    (process_method_func)daDemo00_Execute,
    (process_method_func)daDemo00_IsDelete,
    (process_method_func)daDemo00_Draw,
};

extern actor_process_profile_definition g_profile_DEMO00 = {
  fpcLy_CURRENT_e,       // mLayerID
  7,                     // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_DEMO00,           // mProcName
  &g_fpcLf_Method.base, // sub_method
  sizeof(daDemo00_c),    // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  685,                   // mPriority
  &l_daDemo00_Method,    // sub_method
  0x00044000,            // mStatus
  fopAc_ACTOR_e,         // mActorType
  fopAc_CULLBOX_0_e,     // cullType
};
