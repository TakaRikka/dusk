/**
 * @file d_a_e_po.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_po.h"
#include "d/actor/d_a_obj_poFire.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_enemy.h"
#include "f_op/f_op_camera_mng.h"
#include "Z2AudioLib/Z2Instances.h"


static s16 mAttackNo = 3;

static f32 mHaba = 400.0f;

static s16 mRollHp = 90;

class daE_PO_HIO_c {
public:
    daE_PO_HIO_c();
    virtual ~daE_PO_HIO_c() {}

    /* 0x04 */ s8 field_0x04[4];
    /* 0x08 */ f32 mBaseSize;
    /* 0x0c */ f32 mSearchDist;
    /* 0x10 */ f32 mAttackDist;
    /* 0x14 */ f32 mMovementSpeed;
    /* 0x18 */ f32 mType0AtRange;
    /* 0x1c */ s16 mKanteraColor1R;
    /* 0x1e */ s16 mKanteraColor1G;
    /* 0x20 */ s16 mKanteraColor1B;
    /* 0x22 */ s16 mKanteraColor2R;
    /* 0x24 */ s16 mKanteraColor2G;
    /* 0x26 */ s16 mKanteraColor2B;
    /* 0x28 */ s16 mKanteraColor1A;
    /* 0x2a */ s16 mReviveTime1;
    /* 0x2c */ s16 mReviveTime2;
    /* 0x2e */ s16 mReviveTime3;
    /* 0x30 */ s16 mReviveTime4;
    /* 0x32 */ s16 mFallChanceTime;
    /* 0x34 */ s16 mAttackChanceTime;
    /* 0x36 */ s16 field_0x36;
    /* 0x38 */ f32 mModelSize;
};

daE_PO_HIO_c::daE_PO_HIO_c() {
    field_0x04[0] = -1;
    mBaseSize = 1.2f;
    mSearchDist = 1000.0f;
    mAttackDist = 300.0f;
    mMovementSpeed = 15.0f;
    mType0AtRange = 700.0f;
    mKanteraColor1R = 72;
    mKanteraColor1G = 133;
    mKanteraColor1B = 255;
    mKanteraColor1A = 255;
    mKanteraColor2R = 0;
    mKanteraColor2G = 0;
    mKanteraColor2B = 255;
    mReviveTime1 = 150;
    mReviveTime2 = 75;
    mReviveTime3 = 75;
    mReviveTime4 = 150;
    mFallChanceTime = 50;
    mAttackChanceTime = 50;
    mModelSize = 1.0f;
}

static u8 hio_set;
static daE_PO_HIO_c l_HIO;

static s16 mRollCount;
static s16 mRollPlus;

static s16 mRollDemoFlag;
static s16 mHollDemoFlag;

static int mArg0Check(e_po_class* i_this, s16 param_1) {
    if (param_1 != 0xFF) {
        return (param_1 == i_this->mArg0 ? 1 : 0);
    }
    if (i_this->mArg0 >= 3 && i_this->mArg0 <= 6) {
        return 2;
    }
    if (i_this->mArg0 >= 7 && i_this->mArg0 <= 10) {
        return 3;
    }
    if (i_this->mArg0 >= 11 && i_this->mArg0 <= 14) {
        return 4;
    }
    return 0;
}


static void anm_init(e_po_class* i_this, int i_index, f32 i_morf, u8 i_attr, f32 i_rate) {
    J3DAnmTransform* anm = (J3DAnmTransform*)dComIfG_getObjectRes("E_PO", i_index);
    i_this->mpMorf->setAnm(anm, i_attr, i_morf, i_rate, 0.0f, -1.0f);
    i_this->mAnmID = i_index;
}

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model_p = j3dSys.getModel();
        e_po_class* i_this = (e_po_class*)model_p->getUserArea();

        if (i_this != NULL) {
            MTXCopy(model_p->getAnmMtx(jnt_no), *calc_mtx);
            if (jnt_no >= 17 && jnt_no <= 22) {
                if (jnt_no >= 21) {
                    cMtx_YrotM(*calc_mtx, i_this->field_0x7E6[jnt_no - 19].y);
                    cMtx_ZrotM(*calc_mtx, i_this->field_0x7E6[jnt_no - 19].x);
                } else {
                    cMtx_YrotM(*calc_mtx, i_this->field_0x7E6[jnt_no - 17].y);
                    cMtx_ZrotM(*calc_mtx, i_this->field_0x7E6[jnt_no - 17].x);
                }
                model_p->setAnmMtx(jnt_no, *calc_mtx);
            } else {
                if (jnt_no == 2) {
                    cMtx_XrotM(*calc_mtx, i_this->field_0x810);
                }
            }
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }
    return 1;
}


static int daE_PO_Draw(e_po_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;

    if (mArg0Check(i_this, 0xFF) == 3 && mHollDemoFlag == 0) {
        return 1;
    }

    if (i_this->mType == 1000) {
        return 1;
    }

    g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
    if (i_this->field_0x758 == 0) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpModel, &a_this->tevStr);
        mDoExt_modelUpdateDL(i_this->mpModel);
    }

    if (i_this->field_0x800) {
        J3DMaterial* material_p =
            i_this->mpMorf2->getModel()->getModelData()->getMaterialNodePointer(0);
        if (material_p != NULL) {
            material_p->getTevColor(1)->r = l_HIO.mKanteraColor1R;
            material_p->getTevColor(1)->g = l_HIO.mKanteraColor1G;
            material_p->getTevColor(1)->b = l_HIO.mKanteraColor1B;
            material_p->getTevColor(1)->a = i_this->field_0x800;
        }
        i_this->field_0x7E4 += 1;
        if (material_p != NULL) {
            material_p->getTevColor(2)->r = l_HIO.mKanteraColor2R;
            material_p->getTevColor(2)->g = l_HIO.mKanteraColor2G;
            material_p->getTevColor(2)->b = l_HIO.mKanteraColor2B;
        }
        i_this->mpMorf2->entryDL();
    }

    if (i_this->field_0x75A != 0) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpModel2, &a_this->tevStr);
        i_this->mpMorf3->entryDL();
        return 1;
    }

    if (a_this->health == 0 || (i_this->field_0x5F4 > 1.0f &&
                                (!dComIfGp_event_runCheck() ||
                                 (fopAcM_getTalkEventPartner(daPy_getLinkPlayerActorClass()) !=
                                  (fopAc_ac_c*)daPy_py_c::getMidnaActor()))))
    {
        J3DModel* model_p = i_this->mpMorf->getModel();
        g_env_light.setLightTevColorType_MAJI(model_p, &a_this->tevStr);
        J3DModelData* model_data_p = model_p->getModelData();
        u8 temp_u8 = i_this->field_0x5F4;
        for (u16 i = 0; i < model_data_p->getMaterialNum(); i++) {
            J3DMaterial* material_p = model_data_p->getMaterialNodePointer(i);
            material_p->getTevColor(2)->a = temp_u8;
            if (i_this->mActionID == ACT_ATTACK &&
                (mArg0Check(i_this, 0xFF) != 2 || mArg0Check(i_this, mAttackNo) != 0))
            {
                material_p->getTevColor(0)->r = 17;
                material_p->getTevColor(0)->g = 3;
                material_p->getTevColor(0)->b = 37;
                material_p->getTevColor(0)->a = 128;
            }
        }
        i_this->mInvModel.entryDL(NULL);
    } else {
        i_this->field_0x5F4 = 0.0f;
    }

    if (i_this->mActionID == ACT_DEAD && daPy_py_c::checkNowWolf()) {
        if (i_this->mType == 2) {
            cLib_addCalc2(&i_this->field_0x7D0, 45.0f, 0.7f, 4.0f);
        }

        if (!((daPy_py_c*)dComIfGp_getPlayer(0))->checkWolfDownAttackPullOut() &&
            i_this->mAnmID != ANM_DOWN_DEAD && i_this->mType < 30)
        {
            i_this->enemy.drawBallModel(&a_this->tevStr);
        }
    }

    return 1;
}


static void e_po_opening(e_po_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;

    fopAc_ac_c* player_p = dComIfGp_getPlayer(0);
    camera_class* camera1 =
        static_cast<camera_class*>(dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0)));
    camera_class* camera2 = static_cast<camera_class*>(dComIfGp_getCamera(0));
    cXyz temp_vec;

    switch (i_this->mType) {
    case 0:
        if (!i_this->field_0x5C0) {
            if (!daPy_py_c::checkNowWolfEyeUp() ||
                dComIfGp_getAttention()->GetLockonList(0) == NULL ||
                !dComIfGp_getAttention()->LockonTruth() ||
                dComIfGp_getAttention()->GetLockonList(0)->getActor() != a_this)
            {
                break;
            }
        }
        i_this->field_0x5C0 = true;
        i_this->mSound1.startCreatureVoice(Z2SE_EN_PO_V_FIND, -1);
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, -1, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            i_this->field_0x838 = *fopCamM_GetEye_p(camera2);
            i_this->field_0x844 = *fopCamM_GetCenter_p(camera2);
            dComIfGs_onSaveDunSwitch(35);
            anm_init(i_this, ANM_RUN_AFTER2, 10.0f, 2, 1.0f);
            return;
        }
        camera1->mCamera.Stop();
        camera1->mCamera.SetTrimSize(3);
        a_this->current.angle.y = fopAcM_searchPlayerAngleY(a_this) + 0x8000;
        a_this->shape_angle.y = a_this->current.angle.y;
        i_this->field_0x74A[0] = 10;
        i_this->mType = 1;
        break;

    case 1:
        if (i_this->field_0x74A[0] != 0) {
            if (i_this->field_0x74A[0] != 1) {
                return;
            }
            dCam_getBody()->StartBlure(20, a_this, 0.5f, 1.5f);
            return;
        }
        cLib_addCalcAngleS2(&a_this->current.angle.y, fopAcM_searchPlayerAngleY(a_this), 8, 0x2000);
        a_this->shape_angle.y = a_this->current.angle.y;
        if (abs((int)(s16)(a_this->shape_angle.y -
                           fopAcM_searchPlayerAngleY(a_this))) <= 0x100)
        {
            anm_init(i_this, ANM_AWAKE, 10.0f, 0, 1.0f);
            i_this->mSound1.startCreatureVoice(Z2SE_EN_PO_V_AWAKE, -1);
            i_this->mType = 2;
        }
        break;

    case 2:
        if ((int)i_this->mpMorf->getFrame() == 0x41) {
            dCam_getBody()->StartBlure(50, a_this, 0.5f, 1.5f);
        }
        if (i_this->mpMorf->isStop()) {
            i_this->field_0x74A[0] = 2;
            i_this->mType = 3;
        }
        break;

    case 3:
        if (i_this->field_0x74A[0] != 0) {
            return;
        }
        camera1->mCamera.Start();
        camera1->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();
        daPy_getPlayerActorClass()->cancelOriginalDemo();
        fopAcM_OffStatus(a_this, 0x4000);
        i_this->field_0x5C1 = 0;
        anm_init(i_this, ANM_RUN_AFTER2, 10.0f, 2, 1.0f);
        i_this->mType = 2;
        i_this->mActionID = ACT_SEARCH;
        i_this->field_0x757 = 1;

    default:
        break;
    }

    if (i_this->mType != 0) {
        cXyz temp_pos = a_this->current.pos;
        temp_pos.y += 250.0f;
        cMtx_YrotS(*calc_mtx, fopAcM_searchPlayerAngleY(a_this));

        cXyz temp_pos2;
        cXyz temp_pos3;
        temp_pos2.x = 0.0f;
        temp_pos2.y = 200.0f;
        temp_pos2.z = 200.0f;
        MtxPosition(&temp_pos2, &temp_pos3);
        temp_vec = a_this->current.pos;
        temp_vec += temp_pos3;
        camera1->mCamera.Set(temp_pos, temp_vec);
    }
}


static void e_po_wait(e_po_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;

    switch (i_this->mType) {
    case 0:
        anm_init(i_this, ANM_WAIT, 3.0f, 2, 1.0f);
        i_this->field_0x74A[4] = (u8)(50.0f + cM_rndF(50.0f));
        i_this->mType = 1;
        return;

    case 1:
        if (!i_this->field_0x5C1) {
            if (daPy_py_c::checkNowWolfPowerUp() &&
                dComIfGp_getAttention()->GetLockonList(0) != NULL &&
                dComIfGp_getAttention()->LockonTruth() &&
                dComIfGp_getAttention()->GetLockonList(0)->getActor() == a_this)
            {
                i_this->mType = 0;
                i_this->mSound1.startCreatureVoice(Z2SE_EN_PO_V_FIND, -1);
                i_this->field_0x757 = 1;
                i_this->mActionID = ACT_SEARCH;
                return;
            }
            i_this->enemy.current.angle.y = fopAcM_searchPlayerAngleY(a_this) + 0x8000;
            return;
        }
        if (mArg0Check(i_this, 3) == 0) {
            i_this->mType = 0;
            i_this->mActionID = ACT_OPENING;
            i_this->enemy.current.pos = i_this->enemy.home.pos;
            e_po_opening(i_this);
            return;
        }
        i_this->mActionID = ACT_LIMBERING;
        i_this->enemy.current.pos = i_this->enemy.home.pos;
        i_this->mType = 10;

    default:
        return;
    }
}


static void e_po_avoid(e_po_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;

    switch (i_this->mType) {
    case 0:
        anm_init(i_this, ANM_SWAY_BACK, 0.0f, 0, 1.0f);
        i_this->mSound1.startCreatureVoice(Z2SE_EN_PO_V_LAUGH, -1);
        i_this->enemy.current.angle.y = fopAcM_searchPlayerAngleY(a_this);
        i_this->enemy.attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
        if (cM_rndF(1.0f) < 0.5f) {
            i_this->enemy.current.angle.y += 0x4000;
        } else {
            i_this->enemy.current.angle.y += -0x4000;
        }
        i_this->mType = 1;
        break;

    case 1:
        if (i_this->mpMorf->isStop()) {
            i_this->mSound1.startCreatureVoice(Z2SE_EN_PO_V_FIND, -1);
            i_this->enemy.current.angle.y = i_this->enemy.shape_angle.y;
            i_this->enemy.attention_info.flags |= fopAc_AttnFlag_BATTLE_e;
            i_this->mActionID = ACT_SEARCH;
            anm_init(i_this, ANM_RUN_AFTER2, 10.0f, 2, 1.0f);
            i_this->mType = 2;
        }

    default:
        break;
    }

    cLib_addCalc2(&i_this->enemy.speedF, 8.0f, 1.0f, 1.0f);
    cLib_addCalcAngleS2(&i_this->enemy.shape_angle.y,
                        fopAcM_searchPlayerAngleY(a_this), 2, 0x2000);
    i_this->field_0x754 = 20;
}


static void e_po_search(e_po_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;

    fopAc_ac_c* player_p = static_cast<fopAc_ac_c*>(dComIfGp_getPlayer(0));
    cXyz distance_from_home;
    cXyz distance_from_player;
    f32 temp_float = 0.0f;
    f32 temp_float2 = i_this->field_0x744 * 10.0f;
    if (temp_float2 > 7000.0f) {
        temp_float2 = 7000.0f;
    }
    s16 angle_tan;
    s16 sVar3 = temp_float2 * cM_ssin(i_this->field_0x740 * 500);

    switch (i_this->mType) {
    case 0:
        anm_init(i_this, ANM_AWAKE, 3.0f, 0, 1.0f);
        i_this->mSound1.startCreatureVoice(Z2SE_EN_PO_V_AWAKE, -1);
        i_this->mType = 1;
        break;

    case 1:
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, ANM_RUN_AFTER2, 10.0f, 2, 1.0f);
            i_this->mType = 2;
        }
        break;

    case 2:
        temp_float = l_HIO.mMovementSpeed;
        if (i_this->field_0x744 < l_HIO.mAttackDist + 50.0f) {
            i_this->mType = 3;
            anm_init(i_this, ANM_WAIT02, 10.0f, 2, 1.0f);
            i_this->field_0x74A[0] = (s16)(cM_rndF(30.0f) + 20.0f);
            if (mArg0Check(i_this, 0) != 0) {
                i_this->field_0x74A[0] = i_this->field_0x74A[0] >> 1;
            }
        }
        break;

    case 3:
        temp_float = 5.0f;
        if (i_this->field_0x744 < l_HIO.mAttackDist) {
            temp_float = -5.0f;
            if (i_this->field_0x74A[0] == 0) {
                i_this->mActionID = ACT_ATTACK;
                i_this->mType = 0;
            }
        } else {
            if (i_this->field_0x744 > l_HIO.mAttackDist + 100.0f) {
                anm_init(i_this, ANM_RUN_AFTER2, 10.0f, 2, 1.0f);
                i_this->mType = 2;
            }
        }
        break;

    case 4:
        temp_float = 10.0f;
        distance_from_home = a_this->home.pos - a_this->current.pos;
        angle_tan = (s16)distance_from_home.atan2sX_Z();
        if (distance_from_home.abs() < 200.0f) {
            temp_float = 0.0f;
            if (i_this->mAnmID != ANM_WAIT02) {
                anm_init(i_this, ANM_WAIT02, 10.0f, 2, 1.0f);
            }
            angle_tan = sVar3 + fopAcM_searchPlayerAngleY(a_this);

            distance_from_player = a_this->home.pos - player_p->current.pos;
            if (distance_from_player.abs() < l_HIO.mType0AtRange) {
                anm_init(i_this, ANM_RUN_AFTER2, 10.0f, 2, 1.0f);
                i_this->mType = 2;
            }
        }
        cLib_addCalcAngleS2(&a_this->current.angle.y, angle_tan, 5, 0x800);
        break;
    }

    cLib_addCalc2(&a_this->speedF, temp_float, 1.0f, 1.0f);
    if (i_this->mType != 4) {
        cLib_addCalc2(&a_this->current.pos.y, player_p->current.pos.y, 0.05f, l_HIO.mMovementSpeed);
    } else {
        cLib_addCalc2(&a_this->current.pos.y, a_this->home.pos.y, 0.05f, l_HIO.mMovementSpeed);
    }
    cLib_addCalc2(&i_this->field_0x7D4, l_HIO.mMovementSpeed, 1.0f, 0.5f);
    if (mArg0Check(i_this, 0) != 0) {
        distance_from_home = a_this->home.pos - a_this->current.pos;
        if (distance_from_home.abs() > l_HIO.mType0AtRange) {
            if (i_this->mType == 4) {
                return;
            }
            i_this->mType = 4;
            anm_init(i_this, ANM_RUN_AFTER2, 10.0f, 2, 1.0f);
            return;
        }
    }
    if (i_this->mType != 4) {
        cLib_addCalcAngleS2(&a_this->current.angle.y, sVar3 + fopAcM_searchPlayerAngleY(a_this),
                            0x10, 0x800);
    }
}


static void e_po_attack(e_po_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player_p = (daPy_py_c*)dComIfGp_getPlayer(0);
    s32 frame = i_this->mpMorf->getFrame();
    f32 temp_float = 0.0f;
    if (mArg0Check(i_this, 0) != 0 && i_this->field_0x5C1) {
        i_this->mActionID = ACT_OPENING;
        i_this->mType = 0;
        a_this->current.pos.set(a_this->home.pos);
        e_po_opening(i_this);
        return;
    }
    switch (i_this->mType) {
    case 10:
        anm_init(i_this, ANM_WAIT02, 10.0f, 2, 1.0f);
        i_this->field_0x74A[0] = l_HIO.mAttackChanceTime;
        i_this->mType += 1;
        break;

    case 11:
        if (i_this->field_0x74A[0] != 0) {
            break;
        }

    case 0:
        anm_init(i_this, ANM_ATTACK, 3.0f, 0, 1.0f);
        i_this->mType = 1;
        break;

    case 1:
        if (frame == 6) {
            i_this->mSound1.startCreatureVoice(Z2SE_EN_PO_V_ATTACK, -1);
        }
        if (mArg0Check(i_this, 0xFF) == 2) {
            temp_float = 15.0f;
        }
        if (frame >= 23 && frame <= 33) {
            if (frame == 23) {
                i_this->field_0xA68 = 2;
                i_this->mSound1.startCreatureSound(Z2SE_EN_PO_ATTACK, 0, -1);
            } else {
                i_this->field_0xA68 = 3;
            }
        } else if (frame >= 13 && frame <= 43) {
            i_this->field_0xA68 = 1;
        }

        if (i_this->mpMorf->isStop()) {
            i_this->mType = 2;
        }
        break;

    case 2:
        anm_init(i_this, ANM_RUN_AFTER2, 10.0f, 2, 1.0f);
        if (mArg0Check(i_this, 0xFF) != 2) {
            if (mArg0Check(i_this, 0) != 0 && i_this->field_0x5C1) {
                i_this->mActionID = ACT_WAIT;
                i_this->mType = 0;
                return;
            }
            i_this->mActionID = ACT_SEARCH;
            i_this->field_0x74A[0] = 20;
            i_this->mType = 2;
            i_this->field_0x740 = cM_rndF(65536.0f);
            return;
        }
        i_this->field_0x74A[2] = 0;
        a_this->speedF = 0.0f;
        anm_init(i_this, ANM_WAIT02, 10.0f, 2, 1.0f);
        i_this->mActionID = ACT_ROLL_MOVE;
        i_this->mType = 1;
        if (i_this->BitSW3 != 0xFF) {
            fopAcM_onSwitch(a_this, i_this->BitSW3);
        }
        return;
    }

    if (mArg0Check(i_this, 0xFF) == 2 && mAttackNo == 0) {
        i_this->field_0x758 = 1;
        fopAcM_delete(a_this);
    } else {
        if (mArg0Check(i_this, 0) != 0) {
            cXyz distance_from_home = a_this->home.pos - a_this->current.pos;
            if (distance_from_home.abs() > l_HIO.mType0AtRange || i_this->field_0x5C1) {
                temp_float = 0.0f;
            }
        }

        cLib_addCalc2(&a_this->speedF, temp_float, 0.8f, 3.0f);
        cLib_addCalcAngleS2(&a_this->current.angle.y, fopAcM_searchPlayerAngleY(a_this), 0x10,
                            0x1000);
        cLib_addCalc2(&a_this->current.pos.y, player_p->current.pos.y, 0.05f, i_this->field_0x7D4);
        cLib_addCalc2(&i_this->field_0x7D4, l_HIO.mMovementSpeed, 1.0f, 0.5f);
    }
}


static void damage_check(e_po_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player_p = (daPy_py_c*)dComIfGp_getPlayer(0);

    if (i_this->field_0x754 != 0 || i_this->field_0x757 == 0) {
        return;
    }
    if (i_this->mActionID == ACT_DAMAGE || i_this->mActionID == ACT_AVOID ||
        i_this->mActionID == ACT_OPENING || i_this->mActionID == ACT_HOLL_DEMO ||
        a_this->health <= 1)
    {
        return;
    }

    cXyz position;
    PSMTXCopy(i_this->mpMorf->getModel()->getAnmMtx(1), mDoMtx_stack_c::get());
    mDoMtx_stack_c::multVecZero(&position);
    i_this->enemy.scale.set(l_HIO.mBaseSize, l_HIO.mBaseSize, l_HIO.mBaseSize);
    setMidnaBindEffect(&i_this->enemy, &i_this->mSound1, &position, &i_this->enemy.scale);
    i_this->field_0x7DC = TRUE;
    if (daPy_py_c::checkNowWolf() &&
        ((i_this->mActionID == ACT_ATTACK && i_this->field_0x5F4 > 200.0f) ||
         (i_this->mActionID == ACT_ROLL_MOVE && i_this->field_0x5F4 > 200.0f)))
    {
        i_this->field_0x7DC = FALSE;
    }
    i_this->mColliderStts.Move();
    if (i_this->mCyl.ChkTgHit() != 0) {
        if (i_this->field_0x7DC == TRUE) {
            i_this->enemy.speedF = 0.0f;
            i_this->mCyl.ClrTgHit();
            i_this->mActionID = ACT_AVOID;
            i_this->mType = 0;
            i_this->field_0x754 = 20;
            return;
        }

        i_this->enemy.current.angle.y = i_this->enemy.shape_angle.y;
        i_this->mAtInfo.mpCollider = i_this->mCyl.GetTgHitObj();
        i_this->mAtInfo.field_0x18 = 31;
        cc_at_check(a_this, &i_this->mAtInfo);
        if (mArg0Check(i_this, 0xFF) == 2) {
            mRollHp = i_this->enemy.health;
        }

        i_this->field_0x754 = 20;
        cXyz temp_vec;
        temp_vec.set(*i_this->mCyl.GetTgHitPosP());
        if (player_p->getCutType() != daPy_py_c::CUT_TYPE_WOLF_B_LEFT &&
            player_p->getCutType() != daPy_py_c::CUT_TYPE_WOLF_B_RIGHT &&
            i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_WOLF_ATTACK) &&
            player_p->onWolfEnemyHangBite(&i_this->enemy))
        {
            dScnPly_c::setPauseTimer(0);
            dComIfGp_setHitMark(3, &i_this->enemy, &i_this->enemy.getDownPos(),
                                &i_this->enemy.shape_angle, NULL, 0);
            i_this->mActionID = ACT_WOLF_BITE;
            i_this->mType = 0;
            i_this->enemy.speedF = 0.0f;
            return;
        }

        dComIfGp_setHitMark(3, &i_this->enemy, &temp_vec, &i_this->enemy.shape_angle, NULL, 0);
        i_this->mType = 0;
        if (a_this->health <= 1) {
            a_this->health = 0;
            if (mArg0Check(i_this, 0xFF) == 2) {
                mRollHp = a_this->health;
            }
            i_this->mActionID = ACT_DEAD;
            i_this->mCyl.SetTgHitMark(CcG_Tg_UNK_MARK_3);
            dScnPly_c::setPauseTimer(5);
            i_this->enemy.speedF = 20.0f;
            return;
        }

        i_this->mActionID = ACT_DAMAGE;
        i_this->mSound1.startCreatureVoice(Z2SE_EN_PO_V_DAMAGE, -1);
        i_this->enemy.speedF = -20.0f;
        i_this->enemy.current.angle.y = i_this->mAtInfo.mHitDirection.y;
    }
}


static void e_po_damage(e_po_class* i_this) {
    i_this->field_0x754 = 10;
    switch (i_this->mType) {
    case 0:
        if (mArg0Check(i_this, 0xFF) == 2) {
            mAttackNo = 0;
            if (i_this->BitSW3 != 0xFF) {
                fopAcM_offSwitch((fopAc_ac_c*)i_this, i_this->BitSW3);
            }
            i_this->mArg0 = 3;
        }
        anm_init(i_this, ANM_DAMAGE, 2.0f, 0, 1.0f);
        i_this->mType += 1;
        break;

    case 1:
        if (i_this->mpMorf->isStop()) {
            if (mArg0Check(i_this, 0xFF) != 2) {
                i_this->mActionID = ACT_SEARCH;
                anm_init(i_this, ANM_RUN_AFTER2, 10.0f, 2, 1.0f);
                i_this->mType = 2;
            } else {
                if (mArg0Check(i_this, 0xFF) == 2) {
                    mAttackNo = (u8)(3.0f + cM_rndF(4.0f));
                    if (mAttackNo > 6) {
                        mAttackNo = 3;
                    }
                }
                i_this->mType = 0;
                i_this->field_0x74A[2] = 0;
                i_this->mActionID = ACT_LIMBERING;
            }
        }
        break;
    }
    cLib_addCalc0(&i_this->enemy.speedF, 1.0f, 2.0f);
}

static void e_po_wolfbite(e_po_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;
    daPy_py_c* player_p = (daPy_py_c*)dComIfGp_getPlayer(0);
    i_this->field_0x754 = 10;
    a_this->speedF = 0.0f;
    switch (i_this->mType) {
    case 0:
        if (mArg0Check(i_this, 0xFF) == 2) {
            mAttackNo = 0;
            i_this->mArg0 = 3;
        }
        anm_init(i_this, ANM_HANGED, 2.0f, 0, 1.0f);
        i_this->mSound1.startCreatureVoice(Z2SE_EN_PO_V_DAMAGE, -1);
        i_this->mType += 1;
        i_this->field_0x7DE = 0;
        return;

    case 1:
        if (i_this->mAnmID == ANM_HANGED_DAMAGE || i_this->mAnmID == ANM_HANGED) {
            if (i_this->mpMorf->isStop()) {
                anm_init(i_this, ANM_HANGED_WAIT, 3.0f, 2, 1.0f);
            }
        }
        if (a_this->health <= 0 || a_this->checkWolfBiteDamage()) {
            a_this->offWolfBiteDamage();
            a_this->health -= 4;
            if (mArg0Check(i_this, 0xFF) == 2) {
                mRollHp = a_this->health;
            }
            if (a_this->health <= 1) {
                player_p->offWolfEnemyHangBite();
                i_this->mActionID = ACT_DEAD;
                i_this->mType = 0;
                a_this->health = 0;
                if (mArg0Check(i_this, 0xFF) == 2) {
                    mRollHp = a_this->health;
                }
                i_this->mCyl.SetTgHitMark(CcG_Tg_UNK_MARK_3);
                dScnPly_c::setPauseTimer(5);
                i_this->mSound1.startCollisionSE(Z2SE_HIT_WOLFBITE, 0x1F);
                return;
            }
            i_this->mSound1.startCollisionSE(Z2SE_HIT_WOLFBITE, 0x1E);
            i_this->mSound1.startCreatureVoice(Z2SE_EN_PO_V_DAMAGE, -1);
            anm_init(i_this, ANM_HANGED_DAMAGE, 2.0f, 0, 1.0f);
            i_this->field_0x7DE += 1;
        }
        if (i_this->field_0x7DE > 3 || !player_p->checkWolfEnemyBiteAllOwn(a_this) ||
            i_this->field_0x5F4 < 200.0f)
        {
            anm_init(i_this, ANM_HANGED_BRUSH, 2.0f, 0, 1.0f);
            player_p->offWolfEnemyHangBite();
            i_this->mType = 2;
            return;
        }
        break;

    case 2:
        if (i_this->mpMorf->isStop()) {
            i_this->mType = 0;
            if (mArg0Check(i_this, 0xFF) != 2) {
                i_this->mActionID = ACT_SEARCH;
                i_this->field_0x74A[0] = 20;
                i_this->mType = 2;
                i_this->field_0x740 = cM_rndF(65536.0f);
                return;
            }
            if (mArg0Check(i_this, 0xFF) == 2) {
                mAttackNo = (u8)(3.0f + cM_rndF(4.0f));
                if (mAttackNo > 6) {
                    mAttackNo = 3;
                }
            }
            i_this->field_0x74A[2] = 0;
            i_this->mActionID = ACT_LIMBERING;
        }
        return;
    }
}


static void e_po_dead(e_po_class* i_this) {
    static u16 particleNmaeDt[7] = {
        dPa_RM(ID_ZF_S_POFIREA01_XZSMK),
        dPa_RM(ID_ZF_S_POFIREA02_FIRE),
        dPa_RM(ID_ZF_S_POFIREA03_HAHEN),
        dPa_RM(ID_ZF_S_POFIREA04_SMK),
        dPa_RM(ID_ZF_S_POFIREA05_FIRE),
        dPa_RM(ID_ZF_S_POFIREA06_EXP),
        dPa_RM(ID_ZF_S_POFIREA07_FIRE),
    };
    static u16 dead_eff_Dt1[4] = {
        dPa_RM(ID_ZF_S_PODEATH00SMK),
        dPa_RM(ID_ZF_S_PODEATH01SMK),
        dPa_RM(ID_ZF_S_PODEATH02SP),
        dPa_RM(ID_ZF_S_PODEATH03JET),
    };
    static u16 dead_eff_Dt2[2] = {
        dPa_RM(ID_ZF_S_PODEATHHEAD00SMK),
        dPa_RM(ID_ZF_S_PODEATHHEAD01SP),
    };
    static u16 dead_eff_Dt3[2] = {
        dPa_RM(ID_ZF_S_PODEATHSKIRTR0200SMK),
        dPa_RM(ID_ZF_S_PODEATHSKIRTR0201SP),
    };
    static u16 dead_eff_Dt4[2] = {
        dPa_RM(ID_ZF_S_POFIREB01_FIRE),
        dPa_RM(ID_ZF_S_POFIREB02_FIRE),
    };

    cXyz local_100;
    cXyz cStack_10c;

    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;

    daPy_py_c* player_p = (daPy_py_c*)dComIfGp_getPlayer(0);
    camera_class* camera_player =
        static_cast<camera_class*>(dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0)));
    camera_class* camera = static_cast<camera_class*>(dComIfGp_getCamera(0));
    dBgS_LinChk lin_chk;
    cXyz scale(1.0f, 1.0f, 1.0f);
    csXyz local_1a4;  // Angle for particles
    dBgS_GndChk gnd_chk;
    i_this->field_0x754 = 10;
    daPy_py_c* player_actor = daPy_getPlayerActorClass();
    player_actor->setWolfEnemyHangBiteAngle(fopAcM_searchPlayerAngleY(a_this) + 0x8000);

    int i;
    switch (i_this->mType) {
    case 30:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, -1, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            i_this->field_0x838 = *fopCamM_GetEye_p(camera);
            i_this->field_0x844 = *fopCamM_GetCenter_p(camera);
        } else {
            camera_player->mCamera.Stop();
            camera_player->mCamera.SetTrimSize(3);
            a_this->attention_info.flags &= ~fopAc_AttnFlag_UNK_0x400000;
            i_this->mType = 3;
        }

    default:
        break;

    case 0:
        if (mArg0Check(i_this, 0xFF) == 2) {
            mAttackNo = 0;
            i_this->mArg0 = 3;
            if (i_this->BitSW3 != 0xFF) {
                fopAcM_offSwitch(a_this, i_this->BitSW3);
            }
        }
        if (daPy_getPlayerActorClass()->checkWolfLock(a_this)) {
            daPy_getPlayerActorClass()->cancelWolfLock(a_this);
        }
        a_this->onWolfNoLock();
        i_this->field_0x759 = 1;
        a_this->speedF = 0.0f;
        anm_init(i_this, ANM_DOWN, 3.0f, 0, 1.0f);
        i_this->mSound1.startCreatureVoice(Z2SE_EN_PO_V_DOWN, -1);
        cMtx_YrotS(*calc_mtx, a_this->current.angle.y + 0x8000);
        local_100.x = 0.0f;
        local_100.y = 100.0f;
        local_100.z = 200.0f;
        MtxPosition(&local_100, &cStack_10c);
        local_100.set(a_this->current.pos);
        cStack_10c += a_this->current.pos;
        lin_chk.Set(&local_100, &cStack_10c, a_this);
        i_this->field_0x7DE = 0;
        i_this->field_0x74A[2] = 0;
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            a_this->current.angle.y += 0x8000;
        }
        i_this->field_0x7DD = FALSE;
        i_this->mType = 1;
        break;

    case 1:
        if (i_this->field_0x7DD == FALSE && (int)i_this->mpMorf->getFrame() == 27) {
            Z2GetAudioMgr()->seStart(Z2SE_EN_PO_LAMP_LAND, &i_this->field_0x77C, 0, 0, 1.0f, 1.0f,
                                     -1.0f, -1.0f, 0);
            i_this->field_0x7DD = TRUE;
        }
        if (i_this->mpMorf->isStop()) {
            switch (i_this->mArg0) {
            case 0:
                i_this->field_0x74A[0] = l_HIO.mReviveTime1;
                break;
            case 1:
                i_this->field_0x74A[0] = l_HIO.mReviveTime2;
                break;
            case 2:
                i_this->field_0x74A[0] = l_HIO.mReviveTime3;
                break;
            default:
                i_this->field_0x74A[0] = l_HIO.mReviveTime4;
                break;
            }
            i_this->field_0x74A[1] = 7;
            anm_init(i_this, ANM_DOWN_WAIT, 1.0f, 2, 1.0f);
            i_this->field_0x7D0 = 0.0f;
            i_this->mType = 2;
        }
        break;

    case 2:
        if (i_this->field_0x74A[1] == 0) {
            a_this->onDownFlg();
        }
        if (i_this->field_0x74A[2] == 0 &&
            (i_this->field_0x74A[0] == 0 || !daPy_py_c::checkNowWolf()))
        {
            i_this->mType = 20;
            i_this->field_0x759 = 0;
            a_this->offDownFlg();
            anm_init(i_this, ANM_DOWN, 20.0f, 0, -1.0f);
            i_this->mSound1.startCreatureVoice(Z2SE_EN_PO_V_FIND, -1);
            i_this->field_0x74A[0] = 60;
            return;
        }
        if (a_this->checkWolfDownPullFlg()) {
            a_this->offDownFlg();
            i_this->field_0x74A[0] = 200;
            i_this->field_0x74A[2] = 31;
        }
        if (i_this->field_0x74A[2] == 1) {
            i_this->mSound1.startCollisionSE(0x40003, 0x20);
            i_this->mSound1.startCreatureSound(Z2SE_EN_PO_SOUL_PULLOUT, 0, -1);
            anm_init(i_this, ANM_DOWN_DEAD, 10.0f, 0, 1.0f);
            fopAcM_OffStatus(a_this, 0);
            a_this->attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
            i_this->mCyl.SetTgHitMark(CcG_Tg_UNK_MARK_3);
            dScnPly_c::setPauseTimer(5);
            fopAcM_OnStatus(a_this, 0x4000);
            i_this->mType = 50;
        }
        break;

    case 3:
        if (i_this->mpMorf->isStop()) {
            i_this->field_0x75A = 1;
            fopAcM_OffStatus(a_this, 0);
            a_this->attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
            a_this->attention_info.distances[fopAc_attn_BATTLE_e] = 0;
            fopAcM_SetGroup(a_this, 0);
            i_this->mpMorf3->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_PO", 0x17), 0, 0.0f,
                                    1.0f, 0.0f, -1.0f);
            i_this->mSound1.startCreatureVoice(Z2SE_EN_PO_V_DEATH, -1);
            i_this->mType = 4;
        }
        break;

    case 4: {
        J3DModel* model_p = i_this->mpMorf3->getModel();
        for (i = 0; i < 4; i++) {
            i_this->mParticleKeys[i] =
                dComIfGp_particle_set(i_this->mParticleKeys[i], dead_eff_Dt1[i],
                                      &a_this->current.pos, &a_this->shape_angle, &scale);
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mParticleKeys[i]);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(model_p->getAnmMtx(0x10));
            }

            i_this->mParticleKeys[i + 4] =
                dComIfGp_particle_set(i_this->mParticleKeys[i + 4], dead_eff_Dt1[i],
                                      &a_this->current.pos, &a_this->shape_angle, &scale);
            emitter = dComIfGp_particle_getEmitter(i_this->mParticleKeys[i + 4]);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(model_p->getAnmMtx(0x15));
            }
        }

        for (i = 0; i < 2; i++) {
            i_this->mParticleKeys[i + 8] =
                dComIfGp_particle_set(i_this->mParticleKeys[i + 8], dead_eff_Dt2[i],
                                      &a_this->current.pos, &a_this->shape_angle, &scale);
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mParticleKeys[i + 8]);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(model_p->getAnmMtx(0xA));
            }
            i_this->mParticleKeys[i + 10] =
                dComIfGp_particle_set(i_this->mParticleKeys[i + 10], dead_eff_Dt3[i],
                                      &a_this->current.pos, &a_this->shape_angle, &scale);
            emitter = dComIfGp_particle_getEmitter(i_this->mParticleKeys[i + 10]);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(model_p->getAnmMtx(0x7));
            }
        }

        if (!i_this->mpMorf3->isStop()) {
            i_this->field_0x74A[0] = 20;
            break;
        } else {
            if (i_this->mAcch.GetGroundH() != -G_CM3D_F_INF) {
                i_this->field_0x788.y = i_this->mAcch.GetGroundH();
            }
        }
    }
    case 6:
        if (i_this->field_0x74A[0] == 0) {
            local_100 = a_this->current.pos - i_this->field_0x788;
            cMtx_YrotS(*calc_mtx, local_100.atan2sX_Z() + 0x8000);
            local_100.x = 0.0f;
            local_100.y = 0.0f;
            local_100.z = 250.0f;
            MtxPosition(&local_100, &cStack_10c);
            cStack_10c += a_this->current.pos;
            i_this->field_0x844.set(cStack_10c);
            local_100.x = 0.0f;
            local_100.y = 200.0f;
            local_100.z = 0.0f;
            MtxPosition(&local_100, &cStack_10c);
            cStack_10c += a_this->current.pos;
            i_this->field_0x838.set(cStack_10c);
            camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
            if (i_this->BitSW != 0xFF) {
                fopAcM_onSwitch(a_this, i_this->BitSW);
            }
            i_this->field_0x74A[1] = 50;
            i_this->mType = 5;
        }
        break;

    case 5:
        cLib_addCalc0(&i_this->field_0x800, 0.8f, 10.0f);
        if (i_this->field_0x74A[1] != 0) {
            if (i_this->field_0x74A[1] == 1) {
                if (!i_this->field_0x788.x && !i_this->field_0x788.z) {
                    J3DModel* model = i_this->mpMorf3->getModel();
                    PSMTXCopy(model->getAnmMtx(0x15), *calc_mtx);
                    local_100.set(50.0f, 0.0f, 0.0f);
                    MtxPosition(&local_100, &i_this->field_0x788);
                    if (i_this->mAcch.GetGroundH() != -G_CM3D_F_INF) {
                        i_this->field_0x788.y = i_this->mAcch.GetGroundH();
                    }
                }
                local_1a4 = i_this->field_0x764;
                local_1a4.x = local_1a4.z = 0;
                local_100.set(i_this->field_0x788);
                for (i = 0; i < 7; i++) {
                    dComIfGp_particle_set(particleNmaeDt[i], &local_100, &local_1a4, &scale);
                }
                i_this->field_0x74A[2] = 50;
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_FIRE_EMERGE, &i_this->field_0x788, 0, 0, 1.0f,
                                         1.0f, -1.0f, -1.0f, 0);
                if (i_this->mAcch.GetGroundH() != -G_CM3D_F_INF) {
                    i_this->field_0x7C4 =
                        ((70.0f + i_this->mAcch.GetGroundH()) - i_this->field_0x788.y) / 60.0f;
                    Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_FIRE_FLY, &a_this->current.pos, 0, 0,
                                                  1.0f, 1.0f, -1.0f, -1.0f, 0);
                }
            }
        } else if (i_this->field_0x74A[2] != 0) {
            Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_FIRE_FLY, &i_this->field_0x788, 0, 0, 1.0f, 1.0f,
                                          -1.0f, -1.0f, 0);
        } else {
            local_1a4.x = local_1a4.y = local_1a4.z = 0;
            if (i_this->field_0x758 == 0) {
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_KNTR_BREAK, &i_this->field_0x788, 0, 0, 1.0f,
                                         1.0f, -1.0f, -1.0f, 0);
                i_this->field_0x758 = 1;
            }
            Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_FIRE_FLY, &i_this->field_0x788, 0, 0, 1.0f, 1.0f,
                                          -1.0f, -1.0f, 0);
            i_this->field_0x788.y += i_this->field_0x7C4;
            i_this->field_0x814.x = i_this->field_0x788.x - i_this->field_0x788.x;
            i_this->field_0x814.y =
                i_this->field_0x788.y - (i_this->field_0x788.y + i_this->field_0x7C4);
            i_this->field_0x814.z = i_this->field_0x788.z - i_this->field_0x788.z;
            i_this->field_0x814 *= 0.8f;
            for (i = 0; i < 2; i++) {
                i_this->mParticleKey5[i] =
                    dComIfGp_particle_set(i_this->mParticleKey5[i], dead_eff_Dt4[i],
                                          &i_this->field_0x788, &local_1a4, &scale);
                JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mParticleKey5[i]);
                if (emitter != NULL) {
                    emitter->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
                    emitter->setUserWork((uintptr_t)&i_this->field_0x814);
                }
            }
            bool var_r0 = FALSE;
            if (i_this->mAcch.GetGroundH() != -G_CM3D_F_INF) {
                if ((70.0f + i_this->mAcch.GetGroundH()) <= i_this->field_0x788.y) {
                    var_r0 = TRUE;
                }
            } else {
                var_r0 = TRUE;
            }
            if (var_r0) {
                u32 param = 0xFFFFF000;
                param |= i_this->mArg0;
                i_this->field_0x5B8 = fopAcM_create(PROC_Obj_poFire, param, &i_this->field_0x788,
                                                    fopAcM_GetRoomNo(a_this), NULL, NULL, -1);
                i_this->mType = 7;
                i_this->field_0x74A[1] = 50;
            }
        }
        break;

    case 7:
        fopAc_ac_c* local_1b0_actor;

        if (i_this->field_0x74A[1] == 0) {
            i_this->mType = 9;
            camera_player->mCamera.Reset(i_this->field_0x844, i_this->field_0x838);
            camera_player->mCamera.Start();
            camera_player->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            dComIfGs_addPohSpiritNum();
#if !PLATFORM_SHIELD
            if (dComIfGs_getPohSpiritNum() == 0x14) {
                /* dSv_event_flag_c::F_0457 - Castle Town - Revived cat */
                dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[457]);
            }
#endif
            daPy_getPlayerActorClass()->cancelOriginalDemo();
        } else if (mArg0Check(i_this, 0) != 0) {
            if (!fopAcM_isSwitch(a_this, 0x22)) {
                if (fopAcM_SearchByID(i_this->field_0x5B8, &local_1b0_actor) != 0 &&
                    local_1b0_actor != NULL)
                {
                    daPoFire_c* local_1b0 = static_cast<daPoFire_c*>(local_1b0_actor);
                    i_this->field_0x74A[1] = 50;
                    i_this->field_0x838.x = -490.0f;
                    i_this->field_0x838.y = 1526.0f;
                    i_this->field_0x838.z = -1590.0f;
                    i_this->field_0x844.set(local_1b0_actor->current.pos);
                    Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_FIRE_FLY, &local_1b0_actor->current.pos,
                                                  0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                    camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838, 20.0f, 0);
                    i_this->field_0x80C = 0.0f;
                    if (local_1b0->getMoveEnd() == 1) {
                        fopAcM_onSwitch(a_this, 0x22);
                    }
                }
            } else {
                dKy_change_colpat(3);
            }
        } else {
            if (fopAcM_SearchByID(i_this->field_0x5B8, &local_1b0_actor) != 0 &&
                local_1b0_actor != NULL)
            {
                daPoFire_c* local_1b0 = static_cast<daPoFire_c*>(local_1b0_actor);
                Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_FIRE_FLY, &local_1b0->current.pos, 0, 0,
                                              1.0f, 1.0f, -1.0f, -1.0f, 0);
            }
            i_this->field_0x844.set(a_this->current.pos);
            cMtx_YrotS(*calc_mtx, player_p->shape_angle.y);
            local_100.x = 0.0f;
            local_100.y = 300.0f;
            local_100.z = 0.0f;
            MtxPosition(&local_100, &cStack_10c);
            cStack_10c += player_p->current.pos;
            i_this->field_0x838.set(cStack_10c);
            camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
        }
        break;

    case 9:
        if (i_this->field_0x74A[0] != 0) {
            return;
        }
        scale.x = 6.0f;
        fopAcM_create(PROC_KYTAG03, 0x10000B2, &i_this->field_0x7B8, fopAcM_GetRoomNo(a_this),
                      &a_this->shape_angle, &scale, -1);
        i_this->mType = 10;

    case 10:
        i_this->mSound1.setEnemyName(NULL);
        fopAcM_SetGroup(a_this, 0);
        fopAcM_OffStatus(a_this, 0);
        a_this->attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
        break;

    case 20:
        if ((int)i_this->mpMorf->getFrame() < 1) {
            a_this->health = 90;
            if (mArg0Check(i_this, 0xFF) == 2) {
                mRollHp = a_this->health;
                mAttackNo = (u8)(3.0f + cM_rndF(4.0f));
                if (mAttackNo > 6) {
                    mAttackNo = 3;
                }
            }
            a_this->offWolfNoLock();
            if (mArg0Check(i_this, 0xFF) != 2) {
                i_this->mActionID = ACT_SEARCH;
                i_this->mType = 0;
            } else {
                if (mArg0Check(i_this, 0xFF) == 2) {
                    mAttackNo = (u8)(3.0f + cM_rndF(4.0f));
                    if (mAttackNo > 6) {
                        mAttackNo = 3;
                    }
                }
                i_this->mType = 0;
                i_this->field_0x74A[2] = 0;
                i_this->mActionID = ACT_LIMBERING;
            }
        }
        break;

    case 50:
        i_this->field_0x75C = -1;
        fopAcM_createDisappear(a_this, &a_this->current.pos, 8, 3, 0xFF);
        i_this->mType += 1;

    case 51:
        if (dComIfGp_event_runCheck()) {
            if (a_this->eventInfo.checkCommandDemoAccrpt()) {
                if (dComIfGp_getEventManager().endCheck(i_this->field_0x762) != 0) {
                    dComIfGp_event_reset();
                } else {
                    if (strcmp(dComIfGp_getEventManager().getRunEventName(), "DEFAULT_GETITEM") ==
                            0 &&
                        i_this->field_0x75C != -1)
                    {
                        dComIfGp_event_setTalkPartner(NULL);
                        dComIfGp_event_setItemPartnerId(i_this->field_0x75C);
                        i_this->field_0x75C = -1;
                        i_this->field_0x75B = 1;
                    }
                }
            }
        } else if (i_this->field_0x75B != 0) {
            if (dComIfGp_getEvent()->isOrderOK()) {
                if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                    fopAcM_orderPotentialEvent(a_this, 2, -1, 0);
                    a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                    i_this->field_0x838 = *fopCamM_GetEye_p(camera);
                    i_this->field_0x844 = *fopCamM_GetCenter_p(camera);
                    i_this->mType = 30;
                } else {
                    camera_player->mCamera.Stop();
                    camera_player->mCamera.SetTrimSize(3);
                    a_this->attention_info.flags &= ~fopAc_AttnFlag_UNK_0x400000;
                    i_this->mType = 3;
                }
            }
        } else {
            if (i_this->field_0x75C == -1) {
                i_this->field_0x75C = fopAcM_createItemForPresentDemo(&a_this->current.pos, 0xE0, 0,
                                                                      -1, -1, NULL, NULL);
            }
            if (fopAcM_IsExecuting(i_this->field_0x75C)) {
                i_this->field_0x762 =
                    dComIfGp_getEventManager().getEventIdx(a_this, "DEFAULT_GETITEM", 0xFF);
                fopAcM_orderOtherEventId(a_this, i_this->field_0x762, 0xFF, 0xFFFF, 6, 1);
            }
        }
        break;
    }
    if (i_this->mType == 3 || i_this->mType == 4) {
        i_this->field_0x844.set(a_this->current.pos);
        cMtx_YrotS(*calc_mtx, player_p->shape_angle.y);
        local_100.x = 0.0f;
        local_100.y = 800.0f;
        local_100.z = 0.0f;
        MtxPosition(&local_100, &cStack_10c);
        cStack_10c += player_p->current.pos;
        i_this->field_0x838.set(cStack_10c);
        camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
    }
    if (i_this->mAcch.GetGroundH() != -G_CM3D_F_INF) {
        cLib_addCalc2(&a_this->current.pos.y, i_this->mAcch.GetGroundH(), 0.05f, 15.0f);
    }
    return;
}


static f32 e_rollingMove(e_po_class* i_this, s16 param_1, f32 param_2, f32 param_3) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;
    fopAc_ac_c* player_p = dComIfGp_getPlayer(0);
    cXyz local_38;
    cXyz cStack_44;

    cMtx_YrotS(*calc_mtx, param_1);
    local_38.x = 0.0f;
    local_38.y = 0.0f;
    local_38.z = param_2;
    MtxPosition(&local_38, &cStack_44);
    local_38.set(player_p->current.pos);
    cLib_addCalc2(&i_this->field_0x820.x, local_38.x, 0.7f, param_3);
    cLib_addCalc2(&i_this->field_0x820.z, local_38.z, 0.7f, param_3);
    if (i_this->mAcch.GetGroundH() != -G_CM3D_F_INF) {
        i_this->field_0x820.y = i_this->mAcch.GetGroundH();
    }
    cStack_44 += i_this->field_0x820;
    i_this->field_0x82C.set(cStack_44);
    local_38 = cStack_44 - a_this->current.pos;
    a_this->current.angle.y = local_38.atan2sX_Z();
    return cLib_addCalcPos(&a_this->current.pos, i_this->field_0x82C, 0.7f, i_this->field_0x7D4,
                           20.0f);
}


static void e_po_limbering(e_po_class* i_this) {
    cXyz local_48;
    cXyz local_54;

    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;
    fopAc_ac_c* player_p = dComIfGp_getPlayer(0);
    camera_class* camera_player =
        static_cast<camera_class*>(dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0)));
    camera_class* camera = static_cast<camera_class*>(dComIfGp_getCamera(0));

    switch (i_this->mType) {
    case 10:
        a_this->current.angle.y = fopAcM_searchActorAngleY(a_this, player_p) + 0x8000;
        if (i_this->field_0x5C0 ||
            (daPy_py_c::checkNowWolfPowerUp() &&
             dComIfGp_getAttention()->GetLockonList(0) != NULL &&
             dComIfGp_getAttention()->LockonTruth() &&
             dComIfGp_getAttention()->GetLockonList(0)->getActor() == (fopAc_ac_c*)i_this))
        {
            i_this->mSound1.startCreatureVoice(Z2SE_EN_PO_V_FIND, -1);
            i_this->field_0x5C0 = true;
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                i_this->field_0x838 = *fopCamM_GetEye_p(camera);
                i_this->field_0x844 = *fopCamM_GetCenter_p(camera);
                return;
            }
            camera_player->mCamera.Stop();
            camera_player->mCamera.SetTrimSize(3);
            local_48.x = -4400.0f;
            local_48.y = player_p->current.pos.y;
            local_48.z = 4000.0f;
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(
                &local_48, fopAcM_searchPlayerAngleY(a_this) + 0x8000, 0);
            anm_init(i_this, ANM_RUN_AFTER2, 10.0f, 2, 1.0f);
            cMtx_YrotS(*calc_mtx, fopAcM_searchPlayerAngleY(a_this) + 0x8000);
            local_48.x = 0.0f;
            local_48.y = 0.0f;
            local_48.z = 250.0f;
            MtxPosition(&local_48, &local_54);
            a_this->current.pos.x = local_54.x + player_p->current.pos.x;
            a_this->current.pos.z = local_54.z + player_p->current.pos.z;
            a_this->current.angle.y = fopAcM_searchPlayerAngleY(a_this);
            a_this->shape_angle.y = a_this->current.angle.y + 0x8000;
            i_this->field_0x820 = player_p->current.pos;
            if (i_this->mAcch.GetGroundH() != -G_CM3D_F_INF) {
                i_this->field_0x820.y = i_this->mAcch.GetGroundH();
            }
            a_this->attention_info.distances[fopAc_attn_BATTLE_e] = 25;
            i_this->mType = 11;
            i_this->field_0x5C1 = FALSE;
            break;
        }
        break;

    case 11:
        i_this->field_0x844 = a_this->current.pos;
        i_this->field_0x844.y += 250.0f;
        cMtx_YrotS(*calc_mtx, fopAcM_searchPlayerAngleY(a_this));
        local_48.x = 0.0f;
        local_48.y = 200.0f;
        local_48.z = 200.0f;
        MtxPosition(&local_48, &local_54);
        i_this->field_0x838 = a_this->current.pos;
        i_this->field_0x838 += local_54;
        camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
        cLib_addCalcAngleS2(&a_this->current.angle.y, fopAcM_searchPlayerAngleY(a_this), 8, 0x800);
        a_this->shape_angle.y = a_this->current.angle.y;
        if (abs((int)(s16)(a_this->shape_angle.y - fopAcM_searchPlayerAngleY(a_this))) <= 0x100) {
            anm_init(i_this, ANM_WAIT02, 10.0f, 2, 1.0f);
            i_this->mSph2.OffCoSetBit();
            i_this->field_0x74A[0] = 40;
            i_this->mType = 12;
        }
        break;

    case 12:
        if (i_this->field_0x74A[0] != 0) {
            cLib_addCalc2(&a_this->current.pos.y, player_p->current.pos.y, 0.7f, 5.0f);
            if (i_this->field_0x74A[0] == 1) {
                anm_init(i_this, ANM_RUN_AFTER2, 10.0f, 2, 1.0f);
            }
        } else {
            mRollCount = (fopAcM_searchPlayerAngleY(a_this) + 0x8000) / 256 & 0xFE;
            i_this->field_0x7E0 = mRollCount;
            i_this->field_0x7D4 = 100.0f;
            i_this->field_0x7E2 = 0;
            i_this->field_0x7DE = 0;
            i_this->field_0x74A[0] = 10;
            i_this->field_0x804 = mHaba;
            i_this->mType += 1;
        }
        break;

    case 0:
        a_this->speedF = 0.0f;
        anm_init(i_this, ANM_RUN_AFTER2, 10.0f, 2, 1.0f);
        i_this->field_0x804 = mHaba;
        i_this->mType += 1;
        break;

    case 1:
        i_this->field_0x820.set(player_p->current.pos);
        if (i_this->mAcch.GetGroundH() != -G_CM3D_F_INF) {
            i_this->field_0x820.y = i_this->mAcch.GetGroundH();
        }
        local_48 = a_this->current.pos - i_this->field_0x82C;
        i_this->field_0x7D4 = 50.0f;
        if (local_48.abs() < 4.0f) {
            i_this->field_0x7E0 = mRollCount;
            i_this->field_0x7D4 = 100.0f;
            i_this->field_0x7E2 = 0;
            i_this->field_0x7DE = 0;
            i_this->mArg0 = 3;
            i_this->mType = 3;
            return;
        }
        mRollCount = ((fopAcM_searchPlayerAngleY(a_this) + 0x8000) / 256) & 0xFE;
        break;

    case 13:
        if (i_this->field_0x74A[0] != 0) {
            cLib_addCalc2(&a_this->current.pos.y, player_p->current.pos.y, 0.7f, 5.0f);
            break;
        }

    case 2:
        mRollCount += 2;
        mRollCount = (u8)mRollCount;
        i_this->field_0x7DE += 2;
        if (mRollCount == i_this->field_0x7E0) {
            i_this->field_0x7E2 = mRollCount;
            mAttackNo = (u8)(3.0f + cM_rndF(4.0f));
            if (mAttackNo > 6) {
                mAttackNo = 3;
            }
            if (i_this->mType == 13) {
                i_this->field_0x74A[0] = 30;
                anm_init(i_this, ANM_WAIT02, 10.0f, 2, 1.0f);
                i_this->mType += 1;
            } else {
                i_this->mActionID = ACT_ROLL_MOVE;
                i_this->mType = 0;
                i_this->field_0x74A[2] = 0;
            }
        } else if (!(i_this->field_0x7DE & 0x3F) && i_this->field_0x7E2 < 3) {
            i_this->field_0x7DE = 0;
            u32 param = i_this->field_0x7E2 + 4;
            param |= i_this->BitSW << 8;
            param |= mRollCount << 16;
            param |= i_this->BitSW3 << 24;
            fopAcM_create(PROC_E_PO, param, &a_this->current.pos, fopAcM_GetRoomNo(a_this),
                          &a_this->shape_angle, NULL, -1);
            i_this->field_0x7E2 += 1;
        }
        break;

    case 3:
        for (int i = 0; i < 3; i++) {
            u32 param = i + 4;
            param |= i_this->BitSW << 8;
            param |= (mRollCount + (i + 1) * 0x3F) << 16;
            param |= i_this->BitSW3 << 24;
            fopAcM_create(PROC_E_PO, param, &a_this->current.pos, fopAcM_GetRoomNo(a_this),
                          &a_this->shape_angle, NULL, -1);
        }
        mRollPlus = 0;
        i_this->field_0x74A[2] = 0;
        i_this->field_0x7E2 = mRollCount;
        i_this->mActionID = ACT_ROLL_MOVE;
        i_this->mType = 0;
        break;

    case 14:
        a_this->current.angle.y = fopAcM_searchPlayerAngleY(a_this);
        if (i_this->field_0x74A[0] == 0) {
            anm_init(i_this, ANM_AWAKE, 10.0f, 0, 1.0f);
            i_this->mSound1.startCreatureVoice(Z2SE_EN_PO_V_AWAKE, -1);
            mRollDemoFlag = 1;
            i_this->mType += 1;
        }
        break;

    case 15:
        if ((int)i_this->mpMorf->getFrame() == 65) {
            dCam_getBody()->StartBlure(50, a_this, 0.5f, 1.5f);
        }
        if (i_this->mpMorf->isStop()) {
            camera_player->mCamera.Start();
            camera_player->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            daPy_getPlayerActorClass()->cancelOriginalDemo();
            mRollDemoFlag = 2;
            fopAcM_OffStatus(a_this, fopAcM_STATUS_UNK_0x4000);
            i_this->field_0x74A[2] = 0;
            i_this->mActionID = ACT_ROLL_MOVE;
            i_this->mType = 0;
        }
        break;

    default:
        break;
    }
    if (i_this->mType < 10 || (i_this->mType == 13 && i_this->field_0x74A[0] == 0)) {
        f32 var_f1 = 250.0f;
        if (i_this->mType < 10) {
            var_f1 = 400.0f;
        }
        e_rollingMove(i_this, mRollCount << 8, var_f1, 50.0f);
    }
    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 4, 0x800);
    if (i_this->mType >= 12 && i_this->mType <= 14) {
        local_54.set(a_this->current.pos);
        local_54.y += 250.0f;
        cLib_addCalcPos(&i_this->field_0x844, local_54, 1.0f, 10.0f, 1.0f);
        cMtx_YrotS(*calc_mtx, fopAcM_searchPlayerAngleY(a_this));
        local_48.x = 0.0f;
        local_48.y = 200.0f;
        local_48.z = 600.0f;
        MtxPosition(&local_48, &local_54);
        local_54 += a_this->current.pos;
        cLib_addCalcPos(&i_this->field_0x838, local_54, 1.0f, 10.0f, 1.0f);
        camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
        local_48.x = -4400.0f;
        local_48.y = player_p->current.pos.y;
        local_48.z = 4000.0f;
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(
            &local_48, fopAcM_searchPlayerAngleY(a_this) + 0x8000, 0);
    }
}


static void e_po_roll_move(e_po_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;
    fopAc_ac_c* player_p = static_cast<fopAc_ac_c*>(dComIfGp_getPlayer(0));

    if (mArg0Check(i_this, 0xFF) == 2 && mAttackNo == 0) {
        i_this->field_0x758 = 1;
        fopAcM_delete(a_this);
        return;
    }
    if (mArg0Check(i_this, 3) != 0) {
        fopAcM_onSwitch(a_this, i_this->BitSW3);
    }
    switch (i_this->mType) {
    case 0:
        if (mRollDemoFlag != 0) {
            anm_init(i_this, ANM_WAIT02, 10.0f, 2, 1.0f);
            i_this->field_0x74A[1] = 2;
            fopAcM_OffStatus(a_this, fopAcM_STATUS_UNK_0x4000);
        } else {
            anm_init(i_this, ANM_RUN_AFTER2, 10.0f, 2, 1.0f);
            i_this->field_0x74A[1] = 20;
        }
        i_this->field_0x7E0 = i_this->field_0x7E2;
        i_this->field_0x820.set(player_p->current.pos);
        if (i_this->mAcch.GetGroundH() != -G_CM3D_F_INF) {
            i_this->field_0x820.y = i_this->mAcch.GetGroundH();
        }
        i_this->mSph2.OffCoSetBit();
        i_this->mType = 1;
        break;

    case 1:
        if (i_this->field_0x74A[1] != 0) {
            if (i_this->field_0x74A[1] == 1 && i_this->mAnmID == ANM_RUN_AFTER2) {
                anm_init(i_this, ANM_WAIT02, 10.0f, 2, 1.0f);
            }
        } else {
            if (mArg0Check(i_this, 3) != 0 && mRollPlus == 0) {
                mRollPlus = 1;
            }
            i_this->field_0x757 = 0;
            if (mRollPlus == 0 && mRollDemoFlag == 1 && i_this->mAnmID != ANM_AWAKE) {
                anm_init(i_this, ANM_AWAKE, 10.0f, 0, 1.0f);
                i_this->mSound1.startCreatureVoice(Z2SE_EN_PO_V_AWAKE, -1);
            }
            if (mRollPlus != 0) {
                if (i_this->mAnmID == ANM_AWAKE) {
                    anm_init(i_this, ANM_WAIT02, 10.0f, 2, 1.0f);
                    fopAcM_OffStatus(a_this, fopAcM_STATUS_UNK_0x4000);
                }
                i_this->field_0x7D4 = 0.0f;
                i_this->mType = 2;
            }
        }
        break;

    case 2:
        f32 fVar1;

        if (i_this->field_0x74A[2] == 1) {
            if (mRollHp < a_this->health) {
                a_this->health = mRollHp;
            }
            a_this->current.angle.y = a_this->shape_angle.y;
            i_this->field_0x757 = 1;
            i_this->mActionID = ACT_ATTACK;
            i_this->mType = 10;
            return;
        }
        if (mArg0Check(i_this, 3) != 0 && mRollPlus == 0) {
            mRollPlus = 1;
        }
        if (!(mRollPlus & 0x80)) {
            if (i_this->mAnmID != ANM_WAIT02) {
                anm_init(i_this, ANM_WAIT02, 10.0f, 2, 1.0f);
                i_this->field_0x74A[2] = l_HIO.mFallChanceTime;
                if (mArg0Check(i_this, mAttackNo) != 0) {
                    i_this->field_0x757 = 1;
                }
            }
            cLib_addCalc2(&i_this->field_0x7D4, 50.0f + WREG_F(4), 0.5f, 10.0f + WREG_F(6));
            fVar1 = 5.0f + WREG_F(11);
        } else {
            if (i_this->mAnmID != ANM_RUN_AFTER2) {
                anm_init(i_this, ANM_RUN_AFTER2, 10.0f, 2, 1.0f);
                i_this->field_0x804 = 600.0f + ZREG_F(8);
            }
            i_this->field_0x820.set(player_p->current.pos);
            if (i_this->mAcch.GetGroundH() != -G_CM3D_F_INF) {
                i_this->field_0x820.y = i_this->mAcch.GetGroundH();
            }
            cLib_addCalc2(&i_this->field_0x7D4, 100.0f, 0.5f, 10.0f);
            fVar1 = 50.0f + WREG_F(12);
        }
        if (i_this->field_0x74A[2] == 0) {
            i_this->field_0x7E0 += mRollPlus;
            i_this->field_0x7E0 = (u8)i_this->field_0x7E0;
        }
        if (mArg0Check(i_this, 3) != 0) {
            if (!(mRollCount & 0x3F) && !(mRollPlus & 0x80)) {
                i_this->field_0x804 = 250.0f + 10.0f * cM_rndF(10.0f);
            }
            if (i_this->field_0x74A[2] == 0 && mRollCount == 0) {
                if (!(mRollPlus & 0x80)) {
                    mRollPlus ^= 0xFFFF;
                } else {
                    if ((mHaba - i_this->field_0x804) <= 2.0f && cM_rndF(1.0f) < 0.8f) {
                        mRollPlus ^= 0xFFFF;
                    }
                }
            }
        }
        s16 temp = i_this->field_0x7E0 << 8;
        s16 temp2 = temp;
        e_rollingMove(i_this, temp, mHaba, fVar1);
        if (mArg0Check(i_this, 3) != 0) {
            f32 var_f2 = 0.5f;
            f32 var_f3 = 5.0f;
            if (i_this->field_0x74A[2] == 0) {
                mRollCount += mRollPlus;
                mRollCount &= 0xff;
            } else {
                var_f2 = 0.7f;
                var_f3 = 20.0f;
            }
            cLib_addCalc2(&mHaba, i_this->field_0x804, var_f2, var_f3);
        }
    }
    s16 var_r4 = fopAcM_searchPlayerAngleY(a_this);
    if (i_this->mAnmID == ANM_RUN_AFTER2) {
        var_r4 = a_this->current.angle.y;
    }
    cLib_addCalcAngleS2(&a_this->shape_angle.y, var_r4, 4, 0x800);
}


static void e_po_holl_demo(e_po_class* i_this) {
    static u16 holl_demo_eff_Dt[2] = {
        dPa_RM(ID_ZF_S_POFIRED00_GLOW),
        dPa_RM(ID_ZF_S_POFIRED01_FIRE),
    };
    static s16 mKAngInit_dt[4] = {
        0x0000,
        -0x8000,
        -0x8000,
        0x0000,
    };

    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;
    daPy_py_c* player_p = (daPy_py_c*)dComIfGp_getPlayer(0);
    camera_class* camera_player = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera = dComIfGp_getCamera(0);

    cXyz scale(1.0f, 1.0f, 1.0f);
    u32 param = 7;
    cXyz local_84;   // Distance to center
    cXyz cStack_90;  // Distance to Eye
    static cXyz mKcenterInit_dt[] = {
        cXyz(-13.0f, 143.0f, 344.0f),    cXyz(48.0f, 631.0f, -2300.0f),
        cXyz(-697.0f, 515.0f, -1640.0f), cXyz(-645.0f, 640.0f, -1900.0f),
        cXyz(778.0f, 496.0f, -1619.0f),  cXyz(-35.0f, 241.0f, -1223.0f),
        cXyz(50.0f, 635.0f, -2360.0f),   cXyz(-32.0f, 177.0f, -846.0f),
        cXyz(40.0f, 740.0f, -2850.0f),   cXyz(0.0f, 39.0f, -577.0f),
        cXyz(-45.0f, 197.0f, 1620.0f),   cXyz(1630.0f, 40.0f, 450.0f),
        cXyz(1575.0f, 160.0f, -420.0f),  cXyz(-715.0f, 1350.0f, 100.0f),
        cXyz(90.0f, 50.0f, -480.0f),     cXyz(-30.0f, 1970.0f, -1090.0f),
        cXyz(-74.0f, 684.0f, -1095.0f),  cXyz(-31.0f, 846.0f, -2029.0f),
    };
    static cXyz mKeyeInit_dt[] = {
        cXyz(-10.0f, 165.0f, 690.0f),    cXyz(53.0f, 563.0f, -1971.0f),
        cXyz(-942.0f, 433.0f, -1403.0f), cXyz(-590.0f, 715.0f, -1560.0f),
        cXyz(1030.0f, 410.0f, -1400.0f), cXyz(-45.0f, 84.0f, -851.0f),
        cXyz(160.0f, 520.0f, -1990.0f),  cXyz(-155.0f, 105.0f, -262.0f),
        cXyz(45.0f, 670.0f, -2520.0f),   cXyz(-17.0f, 256.0f, -236.0f),
        cXyz(-255.0f, 58.0f, 1382.0f),   cXyz(1365.0f, 100.0f, 665.0f),
        cXyz(1440.0f, 175.0f, -95.0f),   cXyz(-475.0f, 1415.0f, 350.0f),
        cXyz(60.0f, 100.0f, -750.0f),    cXyz(-30.0f, 2260.0f, -890.0f),
        cXyz(-94.0f, 693.0f, -739.0f),   cXyz(-41.0f, 800.0f, -1676.0f),
    };
    static cXyz mKPosInit_dt[] = {
        cXyz(110.0f, 740.0f, -3350.0f),  cXyz(50.0f, 550.0f, -2165.0f),
        cXyz(-726.0f, 540.0f, -2089.0f), cXyz(653.0f, 770.0f, -2231.0f),
        cXyz(725.0f, 540.0f, -1629.0f),  cXyz(-609.0f, 770.0f, -2634.0f),
        cXyz(-531.0f, 740.0f, -1834.0f), cXyz(458.0f, 970.0f, -2446.0f),
        cXyz(530.0f, 740.0f, -1844.0f),  cXyz(-469.0f, 970.0f, -2409.0f),
        cXyz(-20.0f, 250.0f, 1500.0f),   cXyz(1645.0f, 50.0f, 430.0f),
        cXyz(1580.0f, 150.0f, -610.0f),  cXyz(-1550.0f, 1200.0f, -605.0f),
        cXyz(80.0f, 100.0f, 2100.0f),    cXyz(1645.0f, -200.0f, 430.0f),
        cXyz(1810.0f, 150.0f, -610.0f),  cXyz(-1800.0f, 1200.0f, -590.0f),
        cXyz(-626.0f, 660.0f, -1839.0f), cXyz(553.0f, 870.0f, -2426.0f),
        cXyz(625.0f, 640.0f, -1849.0f),  cXyz(-549.0f, 870.0f, -2424.0f),
    };

    switch (i_this->mType) {
    case 0:
        if (player_p->current.pos.z > 800.0f) {
            break;
        }

    case 100:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            a_this->current.angle.y = fopAcM_searchPlayerAngleY(a_this);
            a_this->shape_angle.y = a_this->current.angle.y;
            i_this->field_0x844 = dCam_getBody()->Center();
            i_this->field_0x838 = dCam_getBody()->Eye();
            i_this->field_0x7D8 = dCam_getBody()->Fovy();
        } else {
            camera_player->mCamera.Stop();
            camera_player->mCamera.SetTrimSize(1);
            daPy_getPlayerActorClass()->changeOriginalDemo();
            if (i_this->mType == 0) {
                daPy_getPlayerActorClass()->changeDemoMode(4, 2, 0, 0);
            }
            i_this->field_0x7DE = 0;
            mHollDemoFlag = 0;
            i_this->mType += 1;
            i_this->field_0x74A[2] = 2;
        }

    case 1000:
    default:
        break;

    case 1:
        if (i_this->field_0x74A[2] != 0) {
            local_84.set(0.0f, player_p->current.pos.y, 555.0f);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&local_84, -0x8000, 0);
            daPy_getPlayerActorClass()->changeDemoMode(2, 0, 0, 0);
        } else {
            i_this->field_0x844.set(mKcenterInit_dt[0]);
            i_this->field_0x838.set(mKeyeInit_dt[0]);
            camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
            i_this->mType = 2;
            i_this->field_0x74A[2] = 50;
            local_84.set(0.0f, player_p->current.pos.y, 555.0f);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&local_84, -0x8000, 0);
            daPy_getPlayerActorClass()->changeDemoMode(2, 0, 0, 0);
        }
        break;

    case 2:
        local_84.set(0.0f, player_p->current.pos.y, 200.0f);
        player_p->changeDemoPos0(&local_84);
        if (i_this->field_0x74A[2] != 0) {
            break;
        }
        i_this->field_0x74A[2] = 20;
        i_this->mType = 3;
        // Fallthrough

    case 3:
        if (i_this->field_0x74A[2] != 0) {
            break;
        }
        cLib_addCalcPos(&i_this->field_0x844, mKcenterInit_dt[8], 0.5f, 40.0f, 20.0f);
        cLib_addCalcPos(&i_this->field_0x838, mKeyeInit_dt[8], 0.5f, 40.0f, 20.0f);
        camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
        local_84 = i_this->field_0x844 - mKcenterInit_dt[8];
        cStack_90 = i_this->field_0x838 - mKeyeInit_dt[8];
        if (local_84.abs() > 2.0f || cStack_90.abs() > 2.0f) {
            break;
        }
        i_this->mType = 4;
        i_this->field_0x74A[2] = 50;
        // Fallthrough

    case 4:
        if (i_this->field_0x74A[2] != 0) {
            break;
        }
        a_this->current.pos.set(mKPosInit_dt[0]);
        for (int i = 1; i < 4; i++) {
            param = i - 0xFFF9;
            fopAcM_create(PROC_E_PO, param, &a_this->current.pos, fopAcM_GetRoomNo(a_this),
                          &a_this->shape_angle, NULL, -1);
        }
        a_this->health = 2;
        mHollDemoFlag = 7;
        i_this->mType = 10;
        // Fallthrough

    case 10:
        i_this->field_0x759 = 0;
        a_this->current.pos.set(mKPosInit_dt[0]);
        if (mArg0Check(i_this, 0xA) != 0) {
            i_this->field_0x7DE = -13;
            i_this->field_0x7E2 = -2;
        } else {
            i_this->field_0x7DE = -15;
            i_this->field_0x7E2 = 0;
        }
        i_this->field_0x74A[2] = 100.0f;
        local_84.set(mKPosInit_dt[1]);
        cStack_90 = local_84 - a_this->current.pos;
        a_this->current.angle.y = cStack_90.atan2sX_Z();
        a_this->current.angle.x = cStack_90.atan2sY_XZ();
        i_this->mType = 11;
        // Fallthrough

    case 11:
        if (mArg0Check(i_this, 7) != 0) {
            cLib_addCalcPos(&i_this->field_0x844, mKcenterInit_dt[1], 0.5f, 10.0f, 2.0f);
            cLib_addCalcPos(&i_this->field_0x838, mKeyeInit_dt[1], 0.5f, 10.0f, 2.0f);
            camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
        }
        cLib_addCalc2(&a_this->speedF, 20.0f, 0.1f, 0.3f);
        i_this->field_0x808 = 0.2f + (a_this->speedF / 20.0f);
        i_this->field_0x7DE += 1;
        i_this->field_0x7E2 += 1;
        if (mArg0Check(i_this, 7) != 0) {
            a_this->current.angle.x += (s16)(400.0f * cM_ssin(1000.0f * i_this->field_0x7DE));
            a_this->current.angle.y += (s16)(500.0f * cM_scos(1000.0f * i_this->field_0x7E2));
            a_this->current.angle.z += (s16)(500.0f * -cM_scos(1000.0f * i_this->field_0x7E2));
        } else if (mArg0Check(i_this, 8) != 0) {
            a_this->current.angle.x += (s16)(-400.0f * cM_ssin(1000.0f * i_this->field_0x7DE));
            a_this->current.angle.y += (s16)(500.0f * cM_scos(1000.0f * i_this->field_0x7E2));
            a_this->current.angle.z += (s16)(500.0f * -cM_scos(1000.0f * i_this->field_0x7E2));
        } else if (mArg0Check(i_this, 9) != 0) {
            a_this->current.angle.x -= (s16)(200.0f * cM_ssin(1000.0f * i_this->field_0x7DE));
            a_this->current.angle.y -= (s16)(1000.0f * cM_scos(1000.0f * i_this->field_0x7E2));
            a_this->current.angle.z -= (s16)(500.0f * -cM_scos(1000.0f * i_this->field_0x7E2));
        } else {
            a_this->current.angle.x -= (s16)(100.0f * cM_ssin(1000.0f * i_this->field_0x7DE));
            a_this->current.angle.y -= (s16)(100.0f * cM_scos(1000.0f * i_this->field_0x7E2));
            a_this->current.angle.z -= (s16)(500.0f * -cM_scos(1000.0f * i_this->field_0x7E2));
        }
        if (i_this->field_0x74A[2] != 0) {
            break;
        }
        if (mArg0Check(i_this, 7) != 0) {
            i_this->field_0x844.set(mKcenterInit_dt[2]);
            i_this->field_0x838.set(mKeyeInit_dt[2]);
            camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
        }
        i_this->field_0x74A[2] = 50;
        a_this->current.pos.set(mKPosInit_dt[1]);
        i_this->field_0x7E2 = cM_rndF(65536.0f);
        i_this->field_0x7B8.set(mKPosInit_dt[i_this->mArg0 - 5]);
        a_this->speedF = 0.0f;
        i_this->mType = 12;
        // Fallthrough

    case 12:
        if (i_this->field_0x74A[2] != 0) {
            local_84.set(mKPosInit_dt[i_this->mArg0 - 5]);
            a_this->shape_angle.y = mKAngInit_dt[i_this->mArg0 - 7];
            cLib_addCalcPos(&a_this->current.pos, local_84, 0.5f, 20.0f, 10.0f);
            break;
        } else if (mHollDemoFlag != i_this->mArg0) {
            break;
        }
        i_this->field_0x74A[2] = 20;
        if (mArg0Check(i_this, 7) == 0) {
            i_this->mType = 14;
            break;
        } else {
            i_this->field_0x74A[2] = 50;
            i_this->mType = 13;
        }
        // Fallthrough

    case 13:
        cLib_addCalcPos(&i_this->field_0x844, mKcenterInit_dt[3], 0.5f, 10.0f, 2.0f);
        cLib_addCalcPos(&i_this->field_0x838, mKeyeInit_dt[3], 0.5f, 10.0f, 2.0f);
        camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
        local_84 = mKcenterInit_dt[3] - i_this->field_0x844;
        cStack_90 = mKeyeInit_dt[3] - i_this->field_0x838;
        if (local_84.abs() > 2.0f || cStack_90.abs() > 2.0f) {
            break;
        }
        i_this->field_0x7E2 = 0;
        i_this->field_0x74A[2] = 2;
        i_this->mType = 14;
        // Fallthrough

    case 14:
        if (i_this->field_0x74A[2] == 0) {
            anm_init(i_this, ANM_FIRE_CATCH, 10.0f, 0, 1.0f);
            i_this->mType = 15;
        }
        break;

    case 15:
        if (!i_this->mpMorf->isStop()) {
            if (mArg0Check(i_this, 8) != 0 && (int)i_this->mpMorf->getFrame() == 10) {
                mHollDemoFlag += 1;
            }
            if (mArg0Check(i_this, 9) != 0 && (int)i_this->mpMorf->getFrame() == 1) {
                mHollDemoFlag += 1;
            }
            if ((int)i_this->mpMorf->getFrame() == 40) {
                fopAcM_offSwitch(a_this, i_this->mArg0 + 27);
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_FIRE_TAKEN, &i_this->field_0x794, 0, 0, 1.0f,
                                         1.0f, -1.0f, -1.0f, 0);
            }
            if ((int)i_this->mpMorf->getFrame() == 43) {
                i_this->field_0x800 = l_HIO.mKanteraColor1A;
                i_this->field_0x756 = 1;
            }
        } else {
            if (mArg0Check(i_this, 7) != 0) {
                i_this->field_0x844.set(mKcenterInit_dt[4]);
                i_this->field_0x838.set(mKeyeInit_dt[4]);
                camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
            }
            if (mArg0Check(i_this, 7) != 0 || mArg0Check(i_this, 0xA) != 0) {
                mHollDemoFlag += 1;
            }
            i_this->mType = 16;
        }
        break;

    case 16:
        if (mHollDemoFlag >= 11) {
            if (mArg0Check(i_this, 7) != 0) {
                i_this->field_0x844.set(mKcenterInit_dt[5]);
                i_this->field_0x838.set(mKeyeInit_dt[5]);
                camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
            }
            local_84.set(mKPosInit_dt[i_this->mArg0 - 1]);
            cLib_addCalcPos(&a_this->current.pos, local_84, 0.5f, 8.0f, 2.0f);
            cStack_90 = local_84 - a_this->current.pos;
            if (cStack_90.abs() < 1.0f) {
                a_this->current.pos.set(local_84);
                i_this->field_0x7B8.set(a_this->current.pos);
                i_this->mType = 30;
                i_this->field_0x7E2 = 0;
                i_this->field_0x74A[1] = 31;
                mHollDemoFlag += 1;
            }
        }
        break;

    case 30:
        if (mHollDemoFlag < 15) {
            i_this->field_0x74A[1] = 30;
        } else if (i_this->field_0x74A[1] != 0) {
            i_this->field_0x74A[2] = 50;
        } else if (i_this->field_0x74A[2] != 0) {
            if (mArg0Check(i_this, 7) != 0) {
                Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_POU_GATE_QUAKE, NULL, 0, 0, 1.0f, 1.0f,
                                              -1.0f, -1.0f, 0);
                if (i_this->field_0x74A[2] == 20) {
                    dComIfGp_getVibration().StartQuake(VIBMODE_Q_POWER2, 0x1F,
                                                       cXyz(0.0f, 1.0f, 0.0f));
                }
            }
        } else if (mArg0Check(i_this, 7) != 0) {
            i_this->field_0x80C = 0.0f;
            i_this->field_0x74A[2] = 20;
            i_this->mType = 17;
        } else {
            i_this->field_0x74A[2] = 80;
            i_this->mType = 21;
        }
        break;

    case 17:
        Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_POU_GATE_QUAKE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                      0);
        if (i_this->field_0x74A[2] != 0) {
            break;
        }
        i_this->field_0x74A[2] = 50;
        i_this->mType = 18;
        // Fallthrough

    case 18:
        Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_POU_GATE_QUAKE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                      0);
        if (i_this->field_0x74A[2] != 0) {
            break;
        }
        a_this->current.angle.z = 0;
        a_this->current.pos.set(i_this->field_0x7B8);
        dKy_change_colpat(4);
        i_this->field_0x844.set(mKcenterInit_dt[6]);
        i_this->field_0x838.set(mKeyeInit_dt[6]);
        camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
        i_this->field_0x74A[2] = 50;
        i_this->mType = 19;
        // Fallthrough

    case 19:
        Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_POU_GATE_QUAKE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                      0);
        if (i_this->field_0x74A[2] != 0) {
            break;
        }
        fopAcM_onSwitch(a_this, 0x45);
        i_this->field_0x74A[2] = 70;
        i_this->mType = 20;
        // Fallthrough

    case 20:
        Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_POU_GATE_QUAKE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                      0);
        if (i_this->field_0x74A[2] != 0) {
            break;
        }
        i_this->mType = 21;
        dComIfGp_getVibration().StopQuake(0x1F);
        mHollDemoFlag = 20;
        anm_init(i_this, ANM_WAIT02, 10.0f, 2, 1.0f);
        local_84.set(0.0f, player_p->current.pos.y, -600.0f);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&local_84, -0x8000, 0);
        break;

    case 21:
        if (i_this->field_0x74A[2] != 0) {
            if (i_this->field_0x74A[2] == 1) {
                anm_init(i_this, ANM_WAIT02, 10.0f, 2, 1.0f);
            }
            break;
        }
        a_this->current.angle.z = 0;
        a_this->current.pos.set(i_this->field_0x7B8);
        if (mHollDemoFlag != 20) {
            break;
        }
        i_this->field_0x74A[2] = 30;
        i_this->mType = 22;
        // Fallthrough

    case 22: {
        if (mArg0Check(i_this, 7) != 0) {
            cLib_addCalcPos(&i_this->field_0x844, mKcenterInit_dt[9], 0.5f, 20.0f, 10.0f);
            cLib_addCalcPos(&i_this->field_0x838, mKeyeInit_dt[9], 0.5f, 20.0f, 10.0f);
            camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
        }
        local_84 = i_this->field_0x838 - mKeyeInit_dt[9];
        if (local_84.abs() < 100.0f && mArg0Check(i_this, 7) != 0) {
            daPy_getPlayerActorClass()->changeDemoMode(0x17, 2, 2, 0);
        }
        if (i_this->field_0x74A[2] != 0) {
            break;
        }
        i_this->field_0x7DE = (i_this->mArg0 - 7) << 0xE;
        cMtx_YrotS(*calc_mtx, i_this->field_0x7DE);
        local_84.x = 0.0f;
        local_84.y = 50.0f;
        local_84.z = 150.0f;
        MtxPosition(&local_84, &cStack_90);
        cStack_90 += player_p->current.pos;
        local_84 = cStack_90 - a_this->current.pos;
        f32 var_f2 = local_84.abs() / 30.0f;
        if (var_f2 < 20.0f) {
            var_f2 = 20.0f;
        }
        cLib_addCalcPos(&a_this->current.pos, cStack_90, 0.8f, var_f2, 10.0f);
        local_84 = cStack_90 - a_this->current.pos;
        cLib_addCalcAngleS2(&a_this->shape_angle.y, fopAcM_searchPlayerAngleY(a_this), 1, 0x500);
        if (local_84.abs() > 2.0f) {
            break;
        }
        a_this->current.pos.set(cStack_90);
        i_this->field_0x7B8.set(a_this->current.pos);
        i_this->field_0x74A[2] = 10;
        mHollDemoFlag += 1;
        i_this->field_0x7E2 = 0;
        i_this->mType = 23;
        // Fallthrough
    }
    case 23:
        if (mHollDemoFlag < 24) {
            i_this->field_0x74A[2] = 10;
        } else if (i_this->field_0x74A[2] == 0) {
            if (mArg0Check(i_this, 7) != 0) {
                dKy_change_colpat(1);
            }
            cMtx_YrotS(*calc_mtx, i_this->field_0x7DE);
            local_84.x = 0.0f;
            local_84.y = 50.0f;
            local_84.z = 150.0f;
            MtxPosition(&local_84, &cStack_90);
            i_this->field_0x82C = cStack_90 + player_p->current.pos;
            cStack_90 = i_this->field_0x82C - a_this->current.pos;
            a_this->current.angle.y = cStack_90.atan2sX_Z();
            cLib_addCalcPos(&a_this->current.pos, i_this->field_0x82C, 0.8f, 20.0f, 5.0f);
            a_this->shape_angle.y = fopAcM_searchPlayerAngleY(a_this);
            if (cStack_90.abs() < 40.0f) {
                i_this->field_0x7DE += 0x1000;
                if (mArg0Check(i_this, 7) != 0 && i_this->field_0x7DE == -0x7000) {
                    i_this->field_0x7E2 += 1;
                    if (i_this->field_0x7E2 > 1) {
                        mHollDemoFlag = 100;
                    }
                }
            }
            if (mHollDemoFlag == 100) {
                i_this->field_0x7DE = 0;
                i_this->field_0x7E2 = 0;
                a_this->current.angle.x = 0;
                a_this->current.angle.z = 0;
                i_this->field_0x74A[2] = 50;
                i_this->mType = 24;
            }
        }
        break;

    case 24:
        if (mArg0Check(i_this, 7) != 0) {
            local_84.x = -5.0f;
            local_84.y = 105.0f;
            local_84.z = -470.0f;
            cLib_addCalcPos(&i_this->field_0x844, local_84, 0.5f, 20.0f, 10.0f);
            local_84.x = -20.0f;
            local_84.y = 320.0f;
            local_84.z = -128.0f;
            cLib_addCalcPos(&i_this->field_0x838, local_84, 0.5f, 20.0f, 10.0f);
            camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
        } else {
            a_this->speedF = 40.0f;
            a_this->current.angle.y = fopAcM_searchPlayerAngleY(a_this) + 0x8000;
        }
        cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 1, 0x1000);
        if (i_this->field_0x74A[2] == 0) {
            if (mArg0Check(i_this, 8) != 0 || mArg0Check(i_this, 9) != 0 ||
                mArg0Check(i_this, 0xA) != 0)
            {
                i_this->field_0x758 = 1;
                fopAcM_delete(a_this);
                return;
            }
            if (mArg0Check(i_this, 7) != 0) {
                local_84.set(0.0f, 55.0f, -601.0f);
                cStack_90.set(0.0f, 136.0f, -354.0f);
                i_this->field_0x759 = 1;
                camera_player->mCamera.Reset(local_84, cStack_90);
                camera_player->mCamera.Start();
                camera_player->mCamera.SetTrimSize(0);
                dComIfGp_event_reset();
                daPy_getPlayerActorClass()->cancelOriginalDemo();
                fopAcM_onSwitch(a_this, 0x43);
                i_this->mArg0 = 0;
                i_this->BitSW = 0x1E;
                i_this->BitSW2 = 0x26;
                i_this->BitSW3 = 0xFF;
                i_this->field_0x756 = 0;
                i_this->field_0x759 = 0;
                a_this->health = 90;
                a_this->field_0x560 = 90;
                i_this->mSph2.OnCoSetBit();
                i_this->field_0x5C1 = 1;
                a_this->attention_info.flags = fopAc_AttnFlag_UNK_0x400000;
                a_this->attention_info.distances[fopAc_attn_BATTLE_e] = 3;
                fopAcM_SetGroup(a_this, 2);
                a_this->home.pos.set(a_this->current.pos);
                i_this->field_0x740 = cM_rndF(65536.0f);
                i_this->mType = 0;
                i_this->mActionID = ACT_WAIT;
            }
            break;
        }
        break;

    case 101:
        if (i_this->field_0x74A[2] == 0 && i_this->field_0x7DE == mHollDemoFlag) {
            if ((s16)i_this->field_0x7E2 == 0) {
                mHollDemoFlag = 0;
                i_this->mType = 102;
            } else {
                for (int i = 0, local_100 = 1; i < 4; i++) {
                    if (i_this->field_0x7E2 & local_100) {
                        param = -0x100;
                        param |= i + 0xB;
                        i_this->field_0x844.set(mKcenterInit_dt[i + 10]);
                        i_this->field_0x838.set(mKeyeInit_dt[i + 10]);
                        i_this->field_0x7E2 ^= local_100;
                        i_this->field_0x7DE += 1;
                        fopAcM_createChild(PROC_E_PO, fopAcM_GetID(a_this), param,
                                           &mKPosInit_dt[i + 14], fopAcM_GetRoomNo(a_this),
                                           &a_this->shape_angle, NULL, -1, NULL);
                        if (i == 0) {
                            i_this->mType = 102;
                        }
                        break;
                    }
                    local_100 *= 2;
                }
                camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
                i_this->field_0x74A[2] = 70;
            }
        }
        break;

    case 102:
        if (i_this->field_0x74A[2] == 0) {
            if (fopAcM_isSwitch(a_this, 0x1E) && !fopAcM_isSwitch(a_this, 0x22)) {
                i_this->field_0x838.x = -490.0f;
                i_this->field_0x838.y = 826.0f;
                i_this->field_0x838.z = -1590.0f;
                i_this->field_0x7DE = 0;
            } else {
                if (i_this->field_0x838.x == -490.0f && i_this->field_0x838.y == 826.0f &&
                    i_this->field_0x838.z == -1590.0f)
                {
                    i_this->field_0x7DE += 1;
                    if (i_this->field_0x7DE > 70) {
                        camera_player->mCamera.Start();
                        camera_player->mCamera.SetTrimSize(0);
                        dComIfGp_event_reset();
                        daPy_getPlayerActorClass()->cancelOriginalDemo();
                        i_this->mType = 1000;
                        return;
                    }
                } else {
                    if (i_this->field_0x7DE == mHollDemoFlag) {
                        if (fopAcM_isSwitch(a_this, 0x23) && fopAcM_isSwitch(a_this, 0x24) &&
                            fopAcM_isSwitch(a_this, 0x25))
                        {
                            i_this->field_0x74A[2] = 140;
                            i_this->mType = 103;
                            i_this->field_0x844.set(mKcenterInit_dt[16]);
                            i_this->field_0x838.set(mKeyeInit_dt[16]);
                            camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
                        } else {
                            camera_player->mCamera.Start();
                            camera_player->mCamera.SetTrimSize(0);
                            dComIfGp_event_reset();
                            daPy_getPlayerActorClass()->cancelOriginalDemo();
                            i_this->mType = 1000;
                        }
                        return;
                    }
                    i_this->field_0x844.set(mKcenterInit_dt[15]);
                    i_this->field_0x838.set(mKeyeInit_dt[15]);
                }
            }
            camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
            break;
        }
        break;

    case 103:
        cLib_addCalcPos(&i_this->field_0x844, mKcenterInit_dt[17], 0.5f, 10.0f, 5.0f);
        cLib_addCalcPos(&i_this->field_0x838, mKeyeInit_dt[17], 0.5f, 10.0f, 5.0f);
        camera_player->mCamera.Set(i_this->field_0x844, i_this->field_0x838);
        if (i_this->field_0x74A[2] == 0) {
            dComIfGp_getVibration().StartQuake(VIBMODE_Q_POWER2, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            i_this->field_0x74A[2] = 140;
            Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_POU_GATE_QUAKE, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                          -1.0f, 0);
            fopAcM_offSwitch(a_this, 0x45);
            i_this->mType = 104;
        }
        break;

    case 104:
        if (i_this->field_0x74A[2] != 0) {
            if (i_this->field_0x74A[2] > 0x14) {
                Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_POU_GATE_QUAKE, NULL, 0, 0, 1.0f, 1.0f,
                                              -1.0f, -1.0f, 0);
            }
            if (i_this->field_0x74A[2] == 0x14) {
                dComIfGp_getVibration().StopQuake(0x1F);
            }
        } else {
            camera_player->mCamera.Start();
            camera_player->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            daPy_getPlayerActorClass()->cancelOriginalDemo();
            fopAcM_onSwitch(a_this, 0x26);
            i_this->field_0x758 = 1;
            fopAcM_delete(a_this);
        }
        break;

    case 200:
        fopAc_ac_c* actor_temp;
        if (mArg0Check(i_this, 0xB) != 0) {
            if (a_this->parentActorID != 0 &&
                fopAcM_SearchByID(a_this->parentActorID, &actor_temp) != 0 && actor_temp != NULL)
            {
                actor_temp->eyePos.set(a_this->current.pos);
            }
        }
        local_84.set(mKPosInit_dt[i_this->mArg0 - 1]);
        Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_FIRE_FLY, &a_this->current.pos, 0, 0, 1.0f, 1.0f,
                                      -1.0f, -1.0f, 0);
        i_this->field_0x82C.set(a_this->current.pos);
        cLib_addCalcPos(&a_this->current.pos, local_84, 0.5f, 10.0f, 2.0f);
        cStack_90 = a_this->current.pos - mKPosInit_dt[i_this->mArg0 - 1];
        if (cStack_90.abs() > 2.0f) {
            break;
        }
        mHollDemoFlag += 1;
        i_this->mType = 201;
        if (mArg0Check(i_this, 0xB) == 0) {
            i_this->field_0x74A[2] = 70;
        }
        // Fallthrough

    case 201:
        if (i_this->field_0x74A[2] == 0) {
            if (mArg0Check(i_this, 0xB) != 0) {
                local_84.set(mKPosInit_dt[18]);
                cLib_addCalcPos(&a_this->current.pos, local_84, 0.5f, 20.0f, 10.0f);

                fopAc_ac_c* po_temp;
                if (a_this->parentActorID != 0 &&
                    fopAcM_SearchByID(a_this->parentActorID, &po_temp) != 0)
                {
                    e_po_class* var_r26 = (e_po_class*)po_temp;
                    if (var_r26 != NULL) {
                        po_temp->eyePos.set(a_this->current.pos);
                        var_r26->field_0x844.set(a_this->current.pos);
                    }
                }

                cStack_90 = a_this->current.pos - local_84;
                if (!(cStack_90.abs() > 2.0f)) {
                    i_this->field_0x74A[2] = 70;
                    i_this->mType = 202;
                    fopAcM_onSwitch(a_this, 0x22);
                }
            } else {
                local_84.set(mKPosInit_dt[i_this->mArg0 + 7]);
                cLib_addCalcPos(&a_this->current.pos, local_84, 0.5f, 20.0f, 10.0f);
                Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_FIRE_FLY, &a_this->current.pos, 0, 0, 1.0f,
                                              1.0f, -1.0f, -1.0f, 0);
                cStack_90 = a_this->current.pos - local_84;
                if (!(cStack_90.abs() > 2.0f)) {
                    i_this->field_0x74A[2] = 50;
                    i_this->mType = 203;
                    fopAcM_onSwitch(a_this, i_this->mArg0 + 0x17);
                }
            }
        }
        break;

    case 202:
        fopAc_ac_c* po_temp;
        if (a_this->parentActorID != 0 && fopAcM_SearchByID(a_this->parentActorID, &po_temp) != 0) {
            e_po_class* var_r26 = (e_po_class*)po_temp;
            if (var_r26 != NULL) {
                po_temp->eyePos.set(a_this->current.pos);
                var_r26->field_0x844.set(a_this->current.pos);
            }
        }
        if (i_this->field_0x74A[2] == 0) {
            i_this->field_0x758 = 1;
            fopAcM_delete(a_this);
        }
        break;

    case 203:
        if (i_this->field_0x74A[2] == 0) {
            mHollDemoFlag += 1;
            i_this->field_0x758 = 1;
            fopAcM_delete(a_this);
        }
        break;
    }

    if (i_this->mType == 0xC8 || i_this->mType == 0xC9) {
        scale.z = 3.0f;
        scale.y = 3.0f;
        scale.x = 3.0f;
        for (int i = 0; i < 2; i++) {
            i_this->mParticleKey5[i] =
                dComIfGp_particle_set(i_this->mParticleKey5[i], holl_demo_eff_Dt[i],
                                      &a_this->current.pos, &a_this->current.angle, &scale);
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mParticleKey5[i]);
        }
    }

    if ((i_this->mType == 12 && i_this->field_0x74A[2] == 0) || i_this->mType == 13 ||
        i_this->mType == 14 || (i_this->mType == 16 && mHollDemoFlag < 11))
    {
        i_this->field_0x7E2 += 1;
        i_this->field_0x7E2 = i_this->field_0x7E2;
        i_this->field_0x7E2 = (u8)i_this->field_0x7E2;
        cStack_90.y = (s16)(100.0f * cM_ssin(i_this->field_0x7E2 * 2000));
        local_84.y = i_this->field_0x7B8.y + cStack_90.y;
        cLib_addCalc2(&a_this->current.pos.y, local_84.y, 0.3f, 1.0f);
    }

    if ((i_this->mType == 30 && i_this->field_0x74A[1] == 0) || i_this->mType == 17 ||
        i_this->mType == 18 || (i_this->mType == 21 && i_this->field_0x74A[2] != 0))
    {
        i_this->field_0x7E2 += 4;
        i_this->field_0x7E2 = (u8)i_this->field_0x7E2;
        a_this->current.angle.z = 500.0f * i_this->field_0x7E2;
        cMtx_ZrotS(*calc_mtx, a_this->current.angle.z);
        local_84.x = 0.0f;
        local_84.y = 100.0f;
        local_84.z = 0.0f;
        MtxPosition(&local_84, &cStack_90);
        local_84 = i_this->field_0x7B8 + cStack_90;
        cLib_addCalcPos(&a_this->current.pos, local_84, 1.0f, 20.0f, 10.0f);
    }
}


static void action(e_po_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;
    daPy_py_c* player_p = (daPy_py_c*)dComIfGp_getPlayer(0);

    cXyz local_3c;
    cXyz local_48;
    i_this->field_0x744 = fopAcM_searchPlayerDistance(a_this);

    f32 var_f31 = 0.0f;
    f32 var_f30 = 30.0f;
    if (mArg0Check(i_this, 2) != 0) {
        if (fopAcM_otherBgCheck(a_this, dComIfGp_getPlayer(0))) {
            i_this->field_0x74A[3] = 5;
        }
    }
    if (i_this->field_0x75A == 0 && i_this->mActionID != ACT_HOLL_DEMO) {
        if (daPy_py_c::checkNowWolfPowerUp() != 0 ||
            (i_this->mActionID == ACT_DEAD && i_this->mType == 2))
        {
            if (i_this->field_0x74A[3] == 0) {
                var_f31 = 230.0f;
                var_f30 = 60.0f;
                fopAcM_OnStatus(a_this, 0);
                a_this->attention_info.flags |= fopAc_AttnFlag_BATTLE_e;
                if (i_this->field_0x5C1) {
                    a_this->attention_info.flags = fopAc_AttnFlag_UNK_0x400000 | fopAc_AttnFlag_BATTLE_e;
                }
                if (i_this->mActionID == ACT_OPENING || i_this->mActionID == ACT_DAMAGE ||
                    i_this->mActionID == ACT_WOLF_BITE || i_this->mActionID == ACT_DEAD ||
                    i_this->mActionID == ACT_ATTACK ||
                    (mArg0Check(i_this, mAttackNo) != 0 && i_this->field_0x74A[2] != 0))
                {
                    var_f31 = 255.0f;
                    var_f30 = 60.0f;
                }
                if (i_this->mActionID == ACT_ATTACK) {
                    var_f31 = 255.0f;
                    var_f30 = 30.0f;
                    if (mArg0Check(i_this, 0xFF) == 2 && mArg0Check(i_this, mAttackNo) == 0) {
                        var_f31 = 70.0f;
                        var_f30 = 60.0f;
                        i_this->field_0x754 = 10;
                    }
                }
            }
        } else {
            i_this->field_0x74A[3] = 5;
            i_this->field_0x754 = 10;
            fopAcM_OffStatus(a_this, 0);
            a_this->attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
            if (i_this->field_0x5C1) {
                a_this->attention_info.flags = fopAc_AttnFlag_UNK_0x400000;
            }
        }
    }
    if (i_this->field_0x759 == 0 && a_this->health > 1) {
        if (i_this->mAnmID == ANM_RUN_AFTER2) {
            i_this->mSound1.startCreatureSoundLevel(Z2SE_EN_PO_MOVE, 0, -1);
        } else {
            i_this->mSound1.startCreatureSoundLevel(Z2SE_EN_PO_VISIBLE, 0, -1);
        }
    }
    if (i_this->mActionID == ACT_DEAD && (i_this->mType == 3 || i_this->field_0x74A[2] != 0)) {
        var_f31 = 255.0f;
        var_f30 = 60.0f;
    }
    cLib_addCalc2(&i_this->field_0x5F4, var_f31, 1.0f, var_f30);
    int var_r27 = 0;
    damage_check(i_this);
    switch (i_this->mActionID) {
    case ACT_WAIT:
        e_po_wait(i_this);
        break;
    case ACT_AVOID:
        e_po_avoid(i_this);
        break;
    case ACT_OPENING:
        e_po_opening(i_this);
        break;
    case ACT_SEARCH:
        e_po_search(i_this);
        var_r27 = 1;
        i_this->field_0x812 = 1;
        break;
    case ACT_ATTACK:
        e_po_attack(i_this);
        var_r27 = 1;
        i_this->field_0x812 = 1;
        break;
    case ACT_DAMAGE:
        e_po_damage(i_this);
        break;
    case ACT_WOLF_BITE:
        e_po_wolfbite(i_this);
        break;
    case ACT_DEAD:
        e_po_dead(i_this);
        break;
    case ACT_LIMBERING:
        e_po_limbering(i_this);
        break;
    case ACT_ROLL_MOVE:
        e_po_roll_move(i_this);
        break;
    case ACT_HOLL_DEMO:
        e_po_holl_demo(i_this);
        break;
    }
    if (i_this->mActionID != ACT_DEAD && i_this->mType != 10) {
        i_this->mSound1.setLinkSearch(var_r27);
    }
    if (i_this->field_0x75A == 0) {
        if (i_this->mActionID != ACT_AVOID &&
            (i_this->mActionID != ACT_OPENING || !i_this->field_0x5C0) &&
            i_this->mActionID != ACT_LIMBERING && i_this->mActionID != ACT_ROLL_MOVE &&
            i_this->mActionID != ACT_HOLL_DEMO)
        {
            s16 var_r27_2 = a_this->current.angle.y;
            if (i_this->field_0x5C1 &&
                (daPy_py_c::checkNowWolfPowerUp() != 0 || i_this->field_0x5F4 > 50.0f))
            {
                var_r27_2 = fopAcM_searchPlayerAngleY(a_this) + 0x8000;
            }
            cLib_addCalcAngleS2(&a_this->shape_angle.y, var_r27_2, 2, 0x2000);
        }
    }
    if (i_this->mActionID != ACT_HOLL_DEMO) {
        cLib_addCalcAngleS2(&a_this->shape_angle.x, a_this->current.angle.x, 2, 0x2000);
        cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 4, 0xC8);
    }
    if (i_this->mActionID != ACT_DEAD && i_this->mActionID != ACT_LIMBERING) {
        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        cMtx_XrotM(*calc_mtx, a_this->current.angle.x);
        cMtx_ZrotM(*calc_mtx, a_this->current.angle.z);
        local_3c.x = 0.0f;
        local_3c.y = 0.0f;
        local_3c.z = a_this->speedF;
        MtxPosition(&local_3c, &local_48);
        a_this->speed.x = local_48.x;
        a_this->speed.y = local_48.y;
        a_this->speed.z = local_48.z;
        a_this->current.pos += a_this->speed * l_HIO.mBaseSize;
    }
    s16 var_r4 = 0;
    if (i_this->field_0x812 != 0) {
        local_3c = player_p->eyePos - a_this->current.pos;
        var_r4 = local_3c.atan2sX_Z() - a_this->shape_angle.y;
        if (var_r4 > 6000 || var_r4 < -6000) {
            var_r4 = 0;
        }
        i_this->field_0x812 = 0;
    }
    cLib_addCalcAngleS2(&i_this->field_0x810, var_r4, 2, 0x1000);
    if (i_this->mActionID != ACT_DEAD) {
        for (int i = 0; i < 4; i++) {
            cLib_addCalcAngleS2(&i_this->field_0x7E6[i].x, 0, 4, 100);
        }
    }
    if (i_this->mActionID == ACT_DEAD) {
        i_this->mAcch.CrrPos(dComIfG_Bgsp());
    }
}


static int daE_PO_Execute(e_po_class* i_this) {
    static u8 po_bottleIn_dt[4] = {
        0xEF,
        0xF0,
        0xF1,
        0xF2,
    };

    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->enemy;
    fopEn_enemy_c* e_this = (fopEn_enemy_c*)a_this;

    cXyz local_4c;
    cXyz cStack_58;
    cXyz cStack_64;

    if (i_this->mType == 1000) {
        return 1;
    }
    i_this->field_0x740++;
    for (int i = 0; i < 5; i++) {
        if ((int)i_this->field_0x74A[i] != 0) {
            i_this->field_0x74A[i]--;
        }
    }
    if ((int)i_this->field_0x754 != 0) {
        i_this->field_0x754--;
    }
    action(i_this);
    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
    mDoMtx_stack_c::YrotM((s16)a_this->shape_angle.y);
    mDoMtx_stack_c::XrotM((s16)a_this->shape_angle.x);
    mDoMtx_stack_c::scaleM(i_this->field_0x808, i_this->field_0x808, i_this->field_0x808);
    if (i_this->field_0x75A != 0) {
        J3DModel* model_p = i_this->mpMorf3->getModel();
        model_p->setBaseTRMtx(mDoMtx_stack_c::get());
        i_this->mpMorf3->modelCalc();
        i_this->mpMorf3->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
        MTXCopy(model_p->getAnmMtx(10), *calc_mtx);
        local_4c.set(-22.0f, 0.0f, 0.0f);
        MtxPosition(&local_4c, &i_this->field_0x7B8);
    } else {
        if (e_this->checkBallModelDraw()) {
            i_this->mSound2.startCreatureSoundLevel(Z2SE_EN_PO_SOUL, 0, -1);
        }
        J3DModel* model_p = i_this->mpMorf->getModel();
        model_p->setBaseTRMtx(mDoMtx_stack_c::get());
        i_this->mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
        i_this->mpMorf->modelCalc();
        cStack_58 = a_this->current.pos;
        i_this->mCyl.SetC(cStack_58);
        dComIfG_Ccsp()->Set(&i_this->mCyl);
        cMtx_copy(model_p->getAnmMtx(2), *calc_mtx);
        local_4c.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&local_4c, &a_this->eyePos);
        a_this->attention_info.position = a_this->eyePos;
        a_this->attention_info.position.y += 50.0f;
        cMtx_copy(model_p->getAnmMtx(0x10), *calc_mtx);
        local_4c.set(24.0f + TREG_F(0), TREG_F(1), -8.4f + TREG_F(2));
        i_this->field_0x770 = i_this->field_0x77C;
        MtxPosition(&local_4c, &i_this->field_0x77C);
        mDoMtx_stack_c::transS(i_this->field_0x77C.x, i_this->field_0x77C.y, i_this->field_0x77C.z);
        mDoMtx_stack_c::YrotM(i_this->field_0x764.y);
        mDoMtx_stack_c::XrotM(i_this->field_0x764.x);
        mDoMtx_stack_c::ZrotM(i_this->field_0x764.z);
        mDoMtx_stack_c::scaleM(i_this->field_0x808, i_this->field_0x808, i_this->field_0x808);
        i_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
        mDoMtx_stack_c::transM(-25.0f + TREG_F(6), -45.0f + TREG_F(7), 10.0f + TREG_F(8));
        MTXCopy(mDoMtx_stack_c::get(), *calc_mtx);
        MtxPosition(&local_4c, &cStack_58);
        i_this->field_0x7AC = i_this->field_0x7A0;
        i_this->field_0x7A0 = i_this->field_0x794;
        i_this->field_0x794.set(cStack_58);
        if (i_this->field_0x760 != 0) {
            i_this->field_0x7AC = i_this->field_0x794;
            i_this->field_0x7A0 = i_this->field_0x794;
            i_this->field_0x760 = 0;
        }
        i_this->field_0x788.set(i_this->field_0x794);
        if (i_this->field_0xA68 >= 2) {
            i_this->mSph2.OnAtSetBit();
            if (i_this->field_0xA68 == 2) {
                i_this->mSph2.StartCAt(cStack_58);
            } else {
                i_this->mSph2.MoveCAt(cStack_58);
            }
        } else {
            i_this->mSph2.OffAtSetBit();
            i_this->mSph2.SetC(cStack_58);
        }
        i_this->mSph2.SetR(55.0f + TREG_F(5));
        dComIfG_Ccsp()->Set(&i_this->mSph2);
        local_4c = i_this->field_0x770 - i_this->field_0x77C;
        f32 var_f1_2 = 1000.0f + (local_4c.abs() * 400.0f);
        if (var_f1_2 > 6000.0f) {
            var_f1_2 = 6000.0f;
        }
        cLib_addCalc2(&i_this->field_0x76C, var_f1_2, 0.1f, 0.1f * var_f1_2);
        if (i_this->mAnmID == ANM_ATTACK && (int)i_this->mpMorf->getFrame() > 14 &&
            (int)i_this->mpMorf->getFrame() < 50)
        {
            local_4c = i_this->field_0x77C - a_this->current.pos;
            i_this->field_0x764.y = local_4c.atan2sX_Z();
            s16 var_r4;
            s16 var_r6;
            if (i_this->field_0xA68 == 1) {
                var_r4 = -0x2000;
                var_r6 = 0x400;
            } else {
                var_r4 = -0x4000;
                var_r6 = 0x1000;
            }
            cLib_addCalcAngleS2(&i_this->field_0x764.x, var_r4, 2, var_r6);
        } else {
            cLib_addCalcAngleS2(&i_this->field_0x764.y, a_this->shape_angle.y + 0x4000, 2, 0x1000);
            cLib_addCalcAngleS2(&i_this->field_0x764.x,
                                i_this->field_0x76C * cM_ssin(i_this->field_0x740 * 2000), 2,
                                0x800);
            i_this->field_0x764.z = i_this->field_0x76C * cM_ssin(i_this->field_0x740 * 2500);
            if (a_this->health > 1) {
                i_this->mSound1.startCreatureSoundLevel(Z2SE_EN_PO_LAMP, 0, -1);
            }
        }
    }

    if (i_this->field_0x800) {
        MTXTrans(mDoMtx_stack_c::now, i_this->field_0x794.x, i_this->field_0x794.y,
                 i_this->field_0x794.z);
        local_4c = dComIfGp_getCamera(0)->lookat.eye - i_this->field_0x794;
        mDoMtx_stack_c::YrotM((int)local_4c.atan2sX_Z());
        mDoMtx_stack_c::XrotM((int)local_4c.atan2sY_XZ());
        mDoMtx_stack_c::scaleM(l_HIO.mModelSize, l_HIO.mModelSize, l_HIO.mModelSize);
        i_this->mpMorf2->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
        i_this->mpMorf2->play(NULL, 0, 0);
        i_this->mpMorf2->modelCalc();
    }

    if (i_this->field_0x758 == 0 && (i_this->mActionID == ACT_ATTACK || i_this->field_0x756 != 0)) {
        cXyz scale(1.0f, 1.0f, 1.0f);
        i_this->mParticleKey6 = dComIfGp_particle_set(i_this->mParticleKey6, dPa_RM(ID_ZF_S_POKANFIRE00),
                                                      &i_this->field_0x794, &i_this->field_0x764, &scale);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mParticleKey6);
        if (emitter != NULL) {
            i_this->field_0xEB4.setup(emitter, &i_this->field_0x794, &i_this->field_0x764,
                                      fopAcM_GetRoomNo(a_this));
            emitter->setEmitterCallBackPtr(&i_this->field_0xEB4);
        }
    }

    i_this->field_0xA68 = 0;
    if (a_this->health <= 1) {
        i_this->field_0x764.x = 0;
        i_this->field_0x764.y = a_this->shape_angle.y - 22000;
        i_this->field_0x764.z = 20000;
    }
    i_this->field_0xEB4.setOldPosP(&i_this->field_0x7A0, &i_this->field_0x7AC);
    if (i_this->field_0x75A != 0) {
        return 1;
    }

    if (fopAcM_isSwitch(a_this, 0x43) && i_this->mArg0 <= 3) {
        if (daPy_py_c::checkNowWolfPowerUp() != 0) {
            dKy_change_colpat(2);
        } else if (mArg0Check(i_this, 0) != 0) {
            dKy_change_colpat(1);
        } else {
            dKy_change_colpat(2);
        }
    }

    fopAc_ac_c* player_p = dComIfGp_getPlayer(0);
    MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(2), mDoMtx_stack_c::get());
    mDoMtx_stack_c::multVecZero(&cStack_58);
    s16 var_r4_2;
    if (i_this->mActionID == ACT_DEAD) {
        var_r4_2 = a_this->shape_angle.y;
    } else {
        local_4c = player_p->current.pos - cStack_58;
        var_r4_2 = local_4c.atan2sX_Z();
    }
    cMtx_YrotS(*calc_mtx, var_r4_2);
    if (i_this->mActionID == ACT_DEAD) {
        local_4c.x = -15.0f;
        local_4c.y = i_this->field_0x7D0;
        local_4c.z = 45.0f;
    } else {
        local_4c.x = 0.0f;
        local_4c.y = 0.0f;
        local_4c.z = 70.0f;
    }
    MtxPosition(&local_4c, &cStack_64);
    cStack_64 += cStack_58;
    if (i_this->mActionID == ACT_DEAD) {
        a_this->eyePos.set(cStack_64);
        a_this->attention_info.position = a_this->eyePos;
        a_this->attention_info.position.y += 50.0f;
    }
    ((fopEn_enemy_c*)a_this)->setDownPos(&cStack_64);
    ((fopEn_enemy_c*)a_this)->setBallModelEffect(&a_this->tevStr);
    return 1;
}

static void mPo_tgHitCallback(fopAc_ac_c* param_0, dCcD_GObjInf* param_1, fopAc_ac_c* param_2,
                              dCcD_GObjInf* param_3) {
    /* empty function */
}

static int daE_PO_IsDelete(e_po_class* param_0) {
    return 1;
}

static int daE_PO_Delete(e_po_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;

    dComIfG_resDelete(&i_this->mPhase, "E_PO");
    if (i_this->field_0xECC != 0) {
        hio_set = FALSE;
    }
    if (a_this->heap != NULL) {
        i_this->mSound1.deleteObject();
        i_this->mSound2.deleteObject();
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_po_class* a_this = (e_po_class*)i_this;

    a_this->mpMorf =
        new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_PO", 0x1B), NULL, NULL,
                             (J3DAnmTransform*)dComIfG_getObjectRes("E_PO", 0x15), 2, 1.0f, 0, -1,
                             &a_this->mSound1, 0x80000, 0x11000084);
    if (a_this->mpMorf == NULL || a_this->mpMorf->getModel() == NULL) {
        return 0;
    }
    if (a_this->mInvModel.create(a_this->mpMorf->getModel(), 1) == 0) {
        return 0;
    }

    a_this->mpMorf->getModel()->setUserArea((uintptr_t)i_this);
    for (u16 i = 0; i < a_this->mpMorf->getModel()->getModelData()->getJointNum(); i++) {
        if (i == 2 || (i >= 17 && i <= 22)) {
            a_this->mpMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(
                nodeCallBack);
        }
    }

    a_this->mpModel = mDoExt_J3DModel__create((J3DModelData*)dComIfG_getObjectRes("E_PO", 0x1C),
                                              0x80000, 0x11000084);
    if (a_this->mpModel == NULL) {
        return 0;
    }

    a_this->mpMorf3 =
        new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_PO", 0x1D), NULL, NULL,
                             (J3DAnmTransform*)dComIfG_getObjectRes("E_PO", 0x17), 0, 1.0f, 0, -1,
                             &a_this->mSound1, 0x80000, 0x11000084);
    if (a_this->mpMorf3 == NULL || a_this->mpMorf3->getModel() == NULL) {
        return 0;
    }
    a_this->mpModel2 = mDoExt_J3DModel__create((J3DModelData*)dComIfG_getObjectRes("E_PO", 0x1D),
                                               0x80000, 0x11000084);
    if (a_this->mpModel2 == NULL) {
        return 0;
    }

    a_this->mpMorf2 = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("E_PO", 0x1A), NULL,
                                         NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_PO", 0x4),
                                         2, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11000084);
    if (a_this->mpMorf2 == NULL || a_this->mpMorf2->getModel() == NULL) {
        return 0;
    }

    if (static_cast<fopEn_enemy_c*>(i_this)->initBallModel()) {
        return 1;
    }
    return 0;
}


static int daE_PO_Create(fopAc_ac_c* i_act_this) {
    e_po_class* i_this = (e_po_class*)i_act_this;
    fopAcM_ct(i_act_this, e_po_class);

    int phase_state = dComIfG_resLoad(&i_this->mPhase, "E_PO");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (fopAcM_isSwitch(i_act_this, 0x26)) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("E_PO PARAM %x\n", fopAcM_GetParam(i_act_this));

        i_this->mArg0 = fopAcM_GetParam(i_act_this);
        if (i_this->mArg0 == 0xFF) {
            i_this->mArg0 = 1;
        }
        if (i_this->mArg0 == 0) {
            i_this->field_0x5C1 = TRUE;
        }
        i_this->field_0x808 = l_HIO.mBaseSize;
        i_this->BitSW = fopAcM_GetParam(i_act_this) >> 8;
        i_this->BitSW2 = fopAcM_GetParam(i_act_this) >> 0x10;
        i_this->BitSW3 = fopAcM_GetParam(i_act_this) >> 0x18;
        if (ZREG_S(0) != 0) {
            i_this->BitSW = ZREG_S(0);
        }
        if (mArg0Check(i_this, 3) == 0 && mArg0Check(i_this, 0xFF) == 2) {
            i_this->BitSW2 = 0xFF;
        }

        OS_REPORT("\n\n\n");
        OS_REPORT("E_PO：i_this->arg0 %d\n", i_this->mArg0);
        OS_REPORT("E_PO：i_this->bitsw %d\n", i_this->BitSW);
        OS_REPORT("E_PO：i_this->bitsw2 %d\n", i_this->BitSW2);
        OS_REPORT("E_PO：i_this->bitsw3 %d\n", i_this->BitSW3);
        OS_REPORT("\n\n\n");
        OS_REPORT("E_PO//////////////E_PO SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_act_this, useHeapInit, 0x6830)) {
            OS_REPORT("//////////////E_PO SET NON !!\n");
            return cPhs_ERROR_e;
        }

        if (hio_set == FALSE) {
            i_this->field_0xECC = 1;
            hio_set = TRUE;
            l_HIO.field_0x04[0] = mDoHIO_CREATE_CHILD("ポウ", (JORReflexible*)&l_HIO);
        }

        i_this->mAcch.Set(fopAcM_GetPosition_p(i_act_this), fopAcM_GetOldPosition_p(i_act_this), i_act_this, 1,
                          &i_this->mAcchCir, fopAcM_GetSpeed_p(i_act_this), NULL, NULL);
        i_this->mAcchCir.SetWall(80.0f, 100.0f);
        i_this->mColliderStts.Init(150, 0, i_act_this);

        static dCcD_SrcCyl cc_cyl_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8000000, 0x3}, 0x0}},  // mObj
                {dCcD_SE_13, 0x0, 0x0, 0x0, 0x0},                  // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6},                // mGObjTg
                {0x0},                                             // mGObjCo
            },                                                     // mObjInf
            {
                {
                    {0.0f, 0.0f, 0.0f},  // mCenter
                    50.0f,               // mRadius
                    300.0f               // mHeight
                }  // mCyl
            }
        };

        i_this->mCyl.Set(cc_cyl_src);
        i_this->mCyl.SetStts(&i_this->mColliderStts);
        i_this->mCyl.SetTgHitCallback(mPo_tgHitCallback);

        static dCcD_SrcSph at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0xd8fbfdff, 0x43}, 0x15}},  // mObj
                {dCcD_SE_METAL, 0x0, 0x1, 0x0, 0x0},                                   // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6},                                    // mGObjTg
                {0x0},                                                                 // mGObjCo
            },                                                                         // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 30.0f}  // mSph
            }  // mSphAttr
        };

        i_this->mSph2.Set(at_sph_src);
        i_this->mSph2.SetStts(&i_this->mColliderStts);
        i_act_this->health = 90;
        i_act_this->field_0x560 = 90;
        fopAcM_SetMin(i_act_this, -500.0f, -500.0f, -500.0f);
        fopAcM_SetMax(i_act_this, 500.0f, 500.0f, 500.0f);
        if (mArg0Check(i_this, 0xFF) == 2 && mRollHp < i_act_this->health) {
            i_act_this->health = mRollHp;
        }
        i_this->field_0x794.set(i_act_this->current.pos);
        if (BREG_S(4) != 0) {
            i_act_this->field_0x560 = i_act_this->health = 2;
        }
        i_this->field_0x800 = l_HIO.mKanteraColor1A;
        i_this->mSound1.init(&i_act_this->current.pos, &i_act_this->eyePos, 3, 1);
        i_this->mSound2.init(&i_act_this->current.pos, &i_act_this->eyePos, 3, 1);
        i_this->mSound1.setEnemyName("E_po");

        i_this->mAtInfo.mpSound = &i_this->mSound1;
        i_this->field_0x740 = cM_rndF(65536.0f);
        i_this->mActionID = ACT_WAIT;
        i_this->field_0x7FE = g_env_light.wether_pat1;
        if (mArg0Check(i_this, 3) == 0 && mArg0Check(i_this, 0xFF) == 2) {
            i_this->mAcch.CrrPos(dComIfG_Bgsp());
            i_this->field_0x7E2 = (u8)(fopAcM_GetParam(i_act_this) >> 16);
            i_this->mActionID = ACT_ROLL_MOVE;
            i_act_this->attention_info.distances[fopAc_attn_BATTLE_e] = 25;
        }
        fopAcM_OffStatus(i_act_this, 0);
        i_act_this->attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;

        if (mArg0Check(i_this, 0xFF) == 3 || mArg0Check(i_this, 0xFF) == 4) {
            i_this->BitSW = 0xFF;
            i_this->mActionID = ACT_HOLL_DEMO;
            i_this->mSph2.OffCoSetBit();
            i_this->field_0x800 = 0.0f;
            i_this->field_0x808 = 0.0f;
            i_act_this->attention_info.distances[fopAc_attn_BATTLE_e] = 0;
            fopAcM_SetGroup(i_act_this, 0);
            if (mArg0Check(i_this, 7) != 0) {
                s32 unused = 0;
                i_this->field_0x759 = 1;
                i_act_this->health = 0;

                if (BREG_S(2) != 0) {
                    i_this->mType = 0x64;
                    i_this->field_0x758 = 1;
                    i_this->field_0x800 = 0;
                    i_this->field_0x5F4 = 0;
                    i_this->field_0x759 = 1;
                    for (s32 i = 0; i < 4; i++) {
                        fopAcM_onSwitch(i_act_this, i + 30);
                    }
                    if (!fopAcM_isSwitch(i_act_this, 69)) {
                        fopAcM_onSwitch(i_act_this, 69);
                        for (s32 i = 0; i < 4; i++) {
                            fopAcM_offSwitch(i_act_this, i + 34);
                        }
                    }
                    fopAcM_onSwitch(i_act_this, 67);
                    fopAcM_onSwitch(i_act_this, 34);
                }

                s32 i;
                s32 local_48;
                for (i = 0, local_48 = 0; i < 4; i++) {
                    if (fopAcM_isSwitch(i_act_this, i + 30)) {
                        local_48++;
                    }
                }
                i_this->mType = 0;
                if (local_48 == 0) {
                    if (!fopAcM_isSwitch(i_act_this, 67)) {
                        for (i = 0; i < 4; i++) {
                            fopAcM_onSwitch(i_act_this, i + 34);
                        }
                        fopAcM_offSwitch(i_act_this, 0x45);
                    } else {
                        i_this->mType = 1000;
                    }
                } else {
                    for (i = 0; i < 4; i++) {
                        if (fopAcM_isSwitch(i_act_this, i + 30) && !fopAcM_isSwitch(i_act_this, i + 34)) {
                            if (i == 0) {
                                i_this->field_0x7E2 |= 1;
                            } else {
                                i_this->field_0x7E2 |= 1 << i;
                            }
                            i_this->field_0x758 = 1;
                            i_this->field_0x800 = 0.0f;
                            i_this->field_0x5F4 = 0.0f;
                            i_this->field_0x759 = 1;
                            i_this->mType = 100;
                            fopAcM_OnStatus(i_act_this, 0x4000);
                        }
                    }
                    if (i_this->mType == 0) {
                        return cPhs_ERROR_e;
                    }
                }
            } else {
                if (mArg0Check(i_this, 0xFF) == 3) {
                    i_this->mType = 10;
                } else if (mArg0Check(i_this, 0xFF) == 4) {
                    i_this->field_0x758 = 1;
                    i_this->field_0x800 = 0.0f;
                    i_this->field_0x5F4 = 0.0f;
                    i_this->field_0x759 = 1;
                    i_act_this->health = 0;
                    i_this->mType = 200;
                }
            }
            anm_init(i_this, ANM_WAIT, 0.0f, 0, 1.0f);
            i_this->mpMorf->setStartFrame(150.0f);
        }
        i_this->field_0xEB4.setOldPosP(&i_act_this->current.pos, &i_act_this->current.pos);
        if ((i_this->mArg0 < 4 || i_this->mArg0 > 6) && mArg0Check(i_this, 0xFF) != 3 &&
            mArg0Check(i_this, 0xFF) != 4 && i_this->BitSW != 0xFF &&
            fopAcM_isSwitch(i_act_this, i_this->BitSW))
        {
            cXyz local_40(1.0f, 1.0f, 1.0f);
            local_40.x = 6.0f;
            i_this->mActionID = ACT_DEAD;
            i_this->mType = 9;
            fopAcM_SetMtx(i_act_this, i_this->mpMorf3->getModel()->getBaseTRMtx());
            if (i_this->mAcch.GetGroundH() != -G_CM3D_F_INF) {
                i_act_this->current.pos.y = i_this->mAcch.GetGroundH();
            }
            i_this->mpMorf3->setStartFrame(95.0f);
            i_this->field_0x74A[0] = 2;
            i_this->field_0x800 = 0.0f;
            if (mArg0Check(i_this, 0) != 0) {
                i_act_this->current.pos.set(0.0f, -20.0f, -600.0f);
                i_act_this->shape_angle.y = i_act_this->current.angle.y = 0;
                dKy_change_colpat(3);
            } else if (mArg0Check(i_this, 1) != 0) {
                i_act_this->current.pos.set(4170.0f, 150.0f, -5180.0f);
                i_act_this->shape_angle.y = i_act_this->current.angle.y = 0;
            } else if (mArg0Check(i_this, 2) != 0) {
                i_act_this->current.pos.set(5630.0f, 1050.0f, 4560.0f);
                i_act_this->shape_angle.y = i_act_this->current.angle.y = 0x4000;
            } else if (mArg0Check(i_this, 3) != 0) {
                i_act_this->current.pos.set(-4550.0f, 1055.0f, 4000.0f);
                i_act_this->shape_angle.y = i_act_this->current.angle.y = -0x8000;
            }
            i_this->field_0x758 = 1;
            i_this->field_0x75A = 1;
            i_this->field_0x757 = 0;
            i_this->field_0x5F4 = 0.0f;
            i_act_this->health = 0;
        } else {
            if (i_this->mArg0 >= 1 && i_this->mArg0 <= 2) {
                fopAcM_OffStatus(i_act_this, 0x4000);
            }
            fopAcM_SetMtx(i_act_this, i_this->mpMorf->getModel()->getBaseTRMtx());
            if (mArg0Check(i_this, 3) != 0) {
                i_this->mAcch.CrrPos(dComIfG_Bgsp());
                i_this->field_0x5C1 = TRUE;
            }
            if (i_this->field_0x5C1) {
                i_act_this->attention_info.flags = fopAc_AttnFlag_UNK_0x400000;
            }
            if (mArg0Check(i_this, 0) != 0) {
                if (!fopAcM_isSwitch(i_act_this, 0x43)) {
                    return cPhs_ERROR_e;
                }
                dKy_change_colpat(1);
            }
        }
        daE_PO_Execute(i_this);
    }
    return phase_state;
}

e_po_class::e_po_class() {
    // empty method
}

static actor_method_class l_daE_PO_Method = {
    (process_method_func)daE_PO_Create,  (process_method_func)daE_PO_Delete,
    (process_method_func)daE_PO_Execute, (process_method_func)daE_PO_IsDelete,
    (process_method_func)daE_PO_Draw,
};

actor_process_profile_definition g_profile_E_PO = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_E_PO,               // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(e_po_class),      // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    162,                     // mPriority
    &l_daE_PO_Method,        // sub_method
    0x00044100,              // mStatus
    fopAc_ENEMY_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES;
