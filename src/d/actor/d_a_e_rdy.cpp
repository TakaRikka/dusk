/**
 * @file d_a_e_rdy.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_rdy.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_e_arrow.h"
#include "d/actor/d_a_e_yc.h"
#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_kago.h"
#include "d/d_bomb.h"
#include "d/d_cc_d.h"
#include "d/d_msg_object.h"
#include "d/d_camera.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_kankyo_mng.h"
#include "f_op/f_op_actor_enemy.h"
#include "m_Do/m_Do_graphic.h"

class daE_RDY_HIO_c : public JORReflexible {
public:
    daE_RDY_HIO_c();
    virtual ~daE_RDY_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mScale;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 mWalkSpeed;
    /* 0x14 */ f32 mRunSpeed;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ f32 mAttackAnmSpeed;
    /* 0x24 */ s16 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ u8 field_0x38;
    /* 0x39 */ u8 field_0x39;
    /* 0x3A */ u8 mDrawEyeModel;
    /* 0x3B */ u8 field_0x3b;
    /* 0x3C */ f32 field_0x3c;
    /* 0x40 */ f32 field_0x40;
    /* 0x44 */ f32 field_0x44;
    /* 0x48 */ f32 field_0x48;
    /* 0x4C */ f32 field_0x4c;
};

STATIC_ASSERT(sizeof(daE_RDY_HIO_c) == 0x50);

enum Action {
    /* 0x00 */ ACT_NORMAL,
    /* 0x03 */ ACT_FIGHT_RUN = 3,
    /* 0x04 */ ACT_FIGHT,
    /* 0x05 */ ACT_BOW_RUN,
    /* 0x06 */ ACT_BOW,
    /* 0x07 */ ACT_BOW2,
    /* 0x09 */ ACT_BOW_IKKI2 = 9,
    /* 0x0A */ ACT_AVOID,
    /* 0x13 */ ACT_BOMB_ACTION = 0x13,
    /* 0x14 */ ACT_S_DAMAGE,
    /* 0x15 */ ACT_DAMAGE,
    /* 0x16 */ ACT_DROP,
    /* 0x17 */ ACT_A_DAMAGE,
    /* 0x18 */ ACT_STAND,
    /* 0x19 */ ACT_COMMANDER,
    /* 0x1A */ ACT_BOW3,
    /* 0x1B */ ACT_EXCITE,
    /* 0x1D */ ACT_WATER = 0x1D,
    /* 0x1E */ ACT_TKUSA,
    /* 0x20 */ ACT_YC_RIDE = 0x20,
    /* 0x33 */ ACT_JYUNKAI = 0x33,
};

enum Animation {
    /* 0x04 */ ANM_ARMAMENT = 4,
    /* 0x05 */ ANM_ATTACK01,
    /* 0x06 */ ANM_AVOID,
    /* 0x07 */ ANM_BLOW,
    /* 0x08 */ ANM_BOW_READY,
    /* 0x09 */ ANM_BOW_RREADY,
    /* 0x0A */ ANM_BOW_RSHOOT,
    /* 0x0B */ ANM_BOW_SHOOT,
    /* 0x0C */ ANM_DAMAGE_L,
    /* 0x0D */ ANM_DAMAGE_R,
    /* 0x0E */ ANM_DAMAGE_S,
    /* 0x0F */ ANM_DAMAGE_W,
    /* 0x10 */ ANM_DEAD_ARROW,
    /* 0x11 */ ANM_DIEA,
    /* 0x12 */ ANM_DIEA_UP,
    /* 0x13 */ ANM_DIEB,
    /* 0x14 */ ANM_DIEB_UP,
    /* 0x15 */ ANM_DROWNEDA,
    /* 0x16 */ ANM_DROWNEDB,
    /* 0x17 */ ANM_FURA2,
    /* 0x18 */ ANM_GOSIGN,
    /* 0x19 */ ANM_JUMP_A,
    /* 0x1A */ ANM_JUMP_B,
    /* 0x1B */ ANM_JUMP_C,
    /* 0x1C */ ANM_KAMAE,
    /* 0x1D */ ANM_KICK,
    /* 0x1E */ ANM_KYORO2,
    /* 0x1F */ ANM_LOOKBACK,
    /* 0x20 */ ANM_PICKUP,
    /* 0x21 */ ANM_RATTACK01,
    /* 0x22 */ ANM_RATTACK01_WAIT,
    /* 0x23 */ ANM_RATTACK02,
    /* 0x24 */ ANM_RATTACK02_WAIT,
    /* 0x25 */ ANM_RATTACK03,
    /* 0x26 */ ANM_RATTACK04,
    /* 0x27 */ ANM_RDAMAGE,
    /* 0x28 */ ANM_RDAMAGE_BACK,
    /* 0x29 */ ANM_RFLY,
    /* 0x2A */ ANM_RFLY_GLIDE,
    /* 0x2B */ ANM_RHANGED,
    /* 0x2C */ ANM_RHANGED_BRASH,
    /* 0x2D */ ANM_RHANGED_BRASH2,
    /* 0x2E */ ANM_RHANGED_DAMAGE,
    /* 0x2F */ ANM_RHANGED_WAIT,
    /* 0x30 */ ANM_RHOVERING,
    /* 0x31 */ ANM_RJUMP_C,
    /* 0x32 */ ANM_RRUN,
    /* 0x33 */ ANM_RRUN02_BACK,
    /* 0x34 */ ANM_RRUN_BACK,
    /* 0x35 */ ANM_RSHOOT,
    /* 0x36 */ ANM_RSHOOT_READY,
    /* 0x37 */ ANM_RSHOOT_WAIT,
    /* 0x38 */ ANM_RSHOOT_WAIT02,
    /* 0x39 */ ANM_RSTEP_BACK,
    /* 0x3A */ ANM_RSTEP_L,
    /* 0x3B */ ANM_RSTEP_LBACK,
    /* 0x3C */ ANM_RSTEP_R,
    /* 0x3D */ ANM_RSTEP_RBACK,
    /* 0x3E */ ANM_RUN,
    /* 0x3F */ ANM_RUN02,
    /* 0x40 */ ANM_RWAIT,
    /* 0x41 */ ANM_RWAIT_BACK,
    /* 0x42 */ ANM_RWALK,
    /* 0x43 */ ANM_RWALK_BACK,
    /* 0x44 */ ANM_SHOOT,
    /* 0x45 */ ANM_SHOOT_READY,
    /* 0x46 */ ANM_SHOOT_WAIT,
    /* 0x47 */ ANM_SHOUT,
    /* 0x48 */ ANM_SIGN,
    /* 0x49 */ ANM_SLEEP_LOOKAROUND,
    /* 0x4A */ ANM_SLEEP_STANDUP,
    /* 0x4B */ ANM_SLEEP_WAIT,
    /* 0x4C */ ANM_WAIT01,
    /* 0x4D */ ANM_WALK,
};
  
enum Joint {
    /* 0x00 */ JNT_KOSI,
    /* 0x01 */ JNT_HIP1,
    /* 0x02 */ JNT_KOKA_L,
    /* 0x03 */ JNT_MOMO_L,
    /* 0x04 */ JNT_SUNE_L1,
    /* 0x05 */ JNT_ASI_L,
    /* 0x06 */ JNT_KOKA_R,
    /* 0x07 */ JNT_MOMOR_R,
    /* 0x08 */ JNT_SUNE_R1,
    /* 0x09 */ JNT_ASI_R,
    /* 0x0A */ JNT_MUNE1,
    /* 0x0B */ JNT_MUNE2,
    /* 0x0C */ JNT_KUBI,
    /* 0x0D */ JNT_HEAD,
    /* 0x0E */ JNT_HIMO_L,
    /* 0x0F */ JNT_HIMO_R,
    /* 0x10 */ JNT_SHOULDER_L,
    /* 0x11 */ JNT_ARM_L1,
    /* 0x12 */ JNT_ARM_L2,
    /* 0x13 */ JNT_HAND_L,
    /* 0x14 */ JNT_YUBI_L,
    /* 0x15 */ JNT_SHOULDER_R,
    /* 0x16 */ JNT_ARM_R1,
    /* 0x17 */ JNT_ARM_R2,
    /* 0x18 */ JNT_HAND_R,
    /* 0x19 */ JNT_YUBI_R,
 };

 enum WeaponType {
    /* 0 */ WEAPON_NONE,
    /* 1 */ WEAPON_CLUB,
    /* 2 */ WEAPON_BOW,
    /* 3 */ WEAPON_BOW_FIRE,
    /* 4 */ WEAPON_BOW_BOMB,
 };

static e_rdy_class* boss;

static bool hioInit;

static daE_RDY_HIO_c l_HIO;

static s16 S_find;

static cXyz S_find_pos;

static fopAc_ac_c* target_info[10];

static int target_info_count;

daE_RDY_HIO_c::daE_RDY_HIO_c() {
    field_0x4 = -1;
    mScale = 1.2f;
    field_0xc = 1.2f;
    mWalkSpeed = 3.5f;
    mRunSpeed = 14.0f;
    field_0x18 = 250.0f;
    field_0x1c = 300.0f;
    mAttackAnmSpeed = 1.1f;
    field_0x24 = 60;
    field_0x28 = 1300.0f;
    field_0x2c = 2000.0f;
    field_0x30 = 3.0f;
    field_0x34 = 3.0f;
    field_0x38 = 1;
    field_0x39 = 0;
    mDrawEyeModel = false;
    field_0x3b = 0;
    field_0x3c = 75.0f;
    field_0x48 = 5.0f;
    field_0x44 = 10.0f;
    field_0x4c = 33.0f;
    field_0x40 = 5.0f;
}

void daE_RDY_HIO_c::genMessage(JORMContext* context) {
    // DEBUG NONMATCHING
}

static void anm_init(e_rdy_class* i_this, int i_anm, f32 i_morf, u8 i_attr, f32 i_rate) {
    J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes(i_this->mpArcName, i_anm);
    i_this->mpMorf->setAnm(bck, i_attr, i_morf, i_rate, 0.0f, -1.0f);
    i_this->mAnm = i_anm;

    if (i_anm == ANM_KYORO2) {
        i_this->mpMorf->setFrame(cM_rndF(30.0f));
    }
}

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DJoint* dank_joint = i_joint;
        int jnt_no = dank_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_rdy_class* _this = (e_rdy_class*)model->getUserArea();
        fopAc_ac_c* a_this = &_this->actor;
        (void) a_this;
        if (_this != NULL) {
            MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);

            if (jnt_no == JNT_KOSI) {
                if (_this->field_0xa98 == 3) {
                    cMtx_YrotM(*calc_mtx, (s16)_this->field_0xa9a);
                }
            } else if (jnt_no == JNT_HIP1) {
                if (_this->field_0xa98 == 3) {
                    cMtx_XrotM(*calc_mtx, _this->field_0xa9a);
                }
                cMtx_YrotM(*calc_mtx, (s16)_this->field_0xbae.y);
                cMtx_ZrotM(*calc_mtx, _this->field_0xb8c[2]
                                        + (-_this->field_0xb02[2].x + _this->field_0xbae.x));
            } else if (jnt_no == JNT_KOKA_L) {
                cMtx_YrotM(*calc_mtx, _this->field_0xb8c[7]);
            } else if (jnt_no == JNT_SUNE_L1) {
                cMtx_ZrotM(*calc_mtx, _this->field_0xb8c[8]);
            } else if (jnt_no == JNT_KOKA_R) {
                cMtx_YrotM(*calc_mtx, _this->field_0xb8c[9]);
            } else if (jnt_no == JNT_SUNE_R1) {
                cMtx_ZrotM(*calc_mtx, _this->field_0xb8c[10]);
            } else if (jnt_no == JNT_MUNE2) {
                cMtx_YrotM(*calc_mtx, _this->field_0xb02[1].y);
                if (_this->field_0xa98 == 3) {
                    cMtx_XrotM(*calc_mtx, _this->field_0xa9a);
                }
                cMtx_ZrotM(*calc_mtx, _this->field_0xb02[1].x + _this->field_0xb8c[1]);
                if (_this->field_0xa98 == 4) {
                    f32 my_f32 = _this->field_0xa9c * -1.0f;
                    cMtx_YrotM(*calc_mtx, my_f32);
                    my_f32 = _this->field_0xa9c * 0.3f;
                    cMtx_XrotM(*calc_mtx, my_f32);
                }
                if (_this->field_0xa98 == 3) {
                    cMtx_ZrotM(*calc_mtx, _this->field_0xa9c);
                }
            } else if (jnt_no == JNT_HEAD) {
                if (_this->field_0xa98 != 3 && _this->field_0xa98 != 4) {
                    cMtx_YrotM(*calc_mtx, (_this->field_0xa9e - _this->field_0xa9a) / 2);
                    cMtx_ZrotM(*calc_mtx, (_this->field_0xb8c[0]
                                            + (_this->field_0xb02[0].x - _this->field_0xa9c)) / 2);
                }
            } else if (jnt_no == JNT_KUBI) {
                cMtx_YrotM(*calc_mtx, _this->field_0xb02[0].y);
                if (_this->field_0xa98 != 3 && _this->field_0xa98 != 4) {
                    cMtx_YrotM(*calc_mtx, (_this->field_0xa9e - _this->field_0xa9a) / 2);
                    cMtx_ZrotM(*calc_mtx, (_this->field_0xb8c[0]
                                            + (_this->field_0xb02[0].x - _this->field_0xa9c)) / 2);
                } else {
                    cMtx_ZrotM(*calc_mtx, _this->field_0xb02[0].x + _this->field_0xb8c[0]);
                }
            } else if (jnt_no == JNT_ARM_L1) {
                cMtx_YrotM(*calc_mtx, _this->field_0xb02[3].y + _this->field_0xb8c[3]);
            } else if (jnt_no == JNT_ARM_L2) {
                cMtx_YrotM(*calc_mtx, _this->field_0xb02[4].y + _this->field_0xb8c[4]);
            } else if (jnt_no == JNT_ARM_R1) {
                cMtx_YrotM(*calc_mtx, _this->field_0xb02[5].y + _this->field_0xb8c[5]);
            } else if (jnt_no == JNT_ARM_R2) {
                cMtx_YrotM(*calc_mtx, _this->field_0xb02[6].y + _this->field_0xb8c[6]);
            }

            if (_this->field_0xbaa != 0) {
                if (jnt_no & 1) {
                    cMtx_YrotM(*calc_mtx, _this->field_0xbaa);
                } else if (jnt_no & 2) {
                    cMtx_XrotM(*calc_mtx, _this->field_0xbaa);
                } else if (jnt_no & 4) {
                    cMtx_ZrotM(*calc_mtx, _this->field_0xbaa);
                }
            }

            model->setAnmMtx(jnt_no, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static int nodeCallBack_bow(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_rdy_class* _this = (e_rdy_class*)model->getUserArea();
        if (_this != NULL && _this->mBowRotation != 0) {
            MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
            mDoMtx_ZrotM(*calc_mtx, _this->mBowRotation);
            model->setAnmMtx(jnt_no, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static int daE_RDY_Draw(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    if (i_this->field_0x5b8 == 12
            /* dSv_event_flag_c::T_0239 - Lake Hylia - Spoke with Fyer (start dark carge) */
        && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[239])) {
        return 1;
    }
    
    if (i_this->mKargarokDeleteTimer != 0) {
        return 1;
    }

    J3DModel* model = i_this->mpMorf->getModel();
    g_env_light.settingTevStruct(2, &a_this->current.pos, &a_this->tevStr);
    dComIfGd_setListDark();
    g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);

    if (i_this->mIsDying) {
        J3DModelData* model_data = model->getModelData();
        for (u16 i = 0; i < model_data->getMaterialNum(); i++) {
            J3DMaterial* material = model_data->getMaterialNodePointer(i);
            material->getTevColor(0)->r = i_this->mDyingTevColor;
            material->getTevColor(0)->g = i_this->mDyingTevColor;
            material->getTevColor(0)->b = i_this->mDyingTevColor;
        }
    }

    i_this->mpMorf->entryDL();

    if (i_this->mIsDying) {
        J3DModelData* model_data = model->getModelData();
        for (u16 i = 0; i < model_data->getMaterialNum(); i++) {
            J3DMaterial* material = model_data->getMaterialNodePointer(i);
            material->getTevColor(0)->r = 0;
            material->getTevColor(0)->g = 0;
            material->getTevColor(0)->b = 0;
        }
    }

    if (i_this->mWeaponType == WEAPON_CLUB) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpWeaponModel, &a_this->tevStr);
        mDoExt_modelUpdateDL(i_this->mpWeaponModel);
    } else if (i_this->mWeaponType >= WEAPON_BOW) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpBowMorf->getModel(), &a_this->tevStr);
        i_this->mpBowMorf->entryDL();
        if (i_this->mHasArrow) {
            g_env_light.setLightTevColorType_MAJI(i_this->mpWeaponModel, &a_this->tevStr);
            mDoExt_modelUpdateDL(i_this->mpWeaponModel);
        }
    }

    if (i_this->mDrawEyeModel) {
        for (int i = 0; i < 2; i++) {
            g_env_light.setLightTevColorType_MAJI(i_this->mpEyeModel[i], &a_this->tevStr);
            J3DModelData* model_data = i_this->mpEyeModel[i]->getModelData();
            for (u16 j = 0; j < model_data->getMaterialNum(); j++) {
                J3DMaterial* material = model_data->getMaterialNodePointer(j);
                material->getTevColor(1)->r = 0xff;
                material->getTevColor(1)->g = 0x8a;
                material->getTevColor(1)->b = 0x18;
            }
            mDoExt_modelUpdateDL(i_this->mpEyeModel[i]);
        }
    }

    dComIfGd_setList();

    if (i_this->mRideState != 2) {
        cXyz pos;
        pos.set(a_this->current.pos.x, a_this->current.pos.y + 100.0f, a_this->current.pos.z);
        i_this->mShadowKey = dComIfGd_setShadow(i_this->mShadowKey, 1, model, &pos, 1200.0f, 0.0f,
                                                a_this->current.pos.y, i_this->mAcch.GetGroundH(),
                                                i_this->mAcch.m_gnd, &a_this->tevStr, 0, 1.0f,
                                                dDlst_shadowControl_c::getSimpleTex());
        if (i_this->mWeaponType == WEAPON_CLUB) {
            dComIfGd_addRealShadow(i_this->mShadowKey, i_this->mpWeaponModel);
        }
    }

    if (i_this->mIsHoldingGrass) {
        g_env_light.settingTevStruct(0x10, &a_this->current.pos, &i_this->mHawkGrassTevStr);
        g_env_light.setLightTevColorType_MAJI(i_this->mpHawkGrassModel, &i_this->mHawkGrassTevStr);
        mDoExt_modelUpdateDL(i_this->mpHawkGrassModel);
    }

    return 1;
}

static BOOL other_bg_check(e_rdy_class* i_this, fopAc_ac_c* i_actor) {
    fopAc_ac_c* _this = (fopAc_ac_c*)i_this;
    dBgS_LinChk lin_chk;
    cXyz start, end;
    end = i_actor->current.pos;
    end.y += 130.0f;
    start = _this->current.pos;
    start.y = _this->eyePos.y;
    lin_chk.Set(&start, &end, _this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static void* s_b_sub(void* i_proc, void* i_this) {
    if (fopAc_IsActor(i_proc)) {
        fopAc_ac_c* actor = (fopAc_ac_c*)i_proc;
        if (dBomb_c::checkBombActor(actor) && !((dBomb_c*)actor)->checkStateExplode()
            && target_info_count < 10)
        {
            target_info[target_info_count] = actor;
            target_info_count++;
        }
    }

    return NULL;
}

// The bit is sort of a fakematch. This gets GCN to match, but this function doesn't inline in Debug:
inline u16 absoluteValue(s16 num) {
    if (num < 0) {
        num = -1*num;
    }
    return num;
}

static dBomb_c* search_bomb(e_rdy_class* i_this, BOOL param_1) {
    if ((i_this->field_0xaf0 & 0x10000000) == 0) {
        return NULL;
    }

    fopAc_ac_c* a_this = &i_this->actor;
    target_info_count = 0;
    for (int i = 0; i < 10; i++) {
        target_info[i] = NULL;
    }
    fpcM_Search(s_b_sub, i_this);

    f32 threshold = 50.0f;
    if (target_info_count != 0) {
        cXyz vec1, vec2, vec3;
        for (int i = 0; i < target_info_count;) {
            dBomb_c* bomb = (dBomb_c*)target_info[i];
            vec1.x = bomb->current.pos.x - a_this->eyePos.x;
            vec1.y = 50.0f + bomb->current.pos.y - a_this->eyePos.y;
            vec1.z = bomb->current.pos.z - a_this->eyePos.z;
            vec2.x = bomb->current.pos.x - a_this->current.pos.x;
            vec2.z = bomb->current.pos.z - a_this->current.pos.z;
            f32 dist1 = JMAFastSqrt(vec1.x * vec1.x + vec1.z * vec1.z);
            f32 dist2 = JMAFastSqrt(vec2.x * vec2.x + vec2.z * vec2.z);
            
            if (dist1 < threshold && !(dist2 > i_this->mPlayerDist + 30.0f)
                && (!other_bg_check(i_this, bomb) || !param_1))
            {
                if (param_1) {
                    f32 abs_res = fabsf(50.0f + bomb->current.pos.y - a_this->eyePos.y);
                    if (abs_res <= 300.0f) {
                        s16 ang_y = a_this->shape_angle.y - cM_atan2s(vec1.x, vec1.z);
                        if (absoluteValue(ang_y) < 20000) {
                            return bomb;
                        }

                        cMtx_YrotS(*calc_mtx, -a_this->shape_angle.y);
                        MtxPosition(&vec1, &vec3);
                        if (fabsf(vec3.x) < 500.0f && fabsf(vec3.y) < 300.0f
                            && vec3.z > -125.0f && vec3.z < 500.0f)
                        {
                            return bomb;
                        }
                    }
                } else {
                    return bomb;
                }
            }

            i++;
            if (i == target_info_count) {
                i = 0;
                threshold += 50.0f;
                if (threshold > 1500.0f) {
                    return NULL;
                }
            }
        }
    } else {
        return NULL;
    }

    return NULL;
}

static BOOL way_bg_check(e_rdy_class* i_this, f32 i_dist) {
    fopAc_ac_c* _this = (fopAc_ac_c*)i_this;
    dBgS_LinChk lin_chk;
    cXyz vec, start, end;
    start = _this->current.pos;
    start.y += 50.0f;
    mDoMtx_YrotS(*calc_mtx, _this->shape_angle.y);
    vec.x = 0.0f;
    vec.y = 50.0f;
    vec.z = i_dist;
    MtxPosition(&vec, &end);
    end += _this->current.pos;
    lin_chk.Set(&start, &end, _this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static void ride_off(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    e_yc_class* kargarok = (e_yc_class*)fopAcM_SearchByID(i_this->mKargarokID);
    if (kargarok != NULL) {
        kargarok->field_0x6b0 = 0;
        i_this->field_0xa8e = 0;
        a_this->current.angle.x = 0;
        i_this->mKargarokID = fpcM_ERROR_PROCESS_ID_e;
    }

    i_this->mRideState = 0;
    if (i_this->mpBowMorf != NULL) {
        J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes(i_this->mpArcName, 11);
        i_this->mpBowMorf->setAnm(bck, J3DFrameCtrl::EMode_NONE, 1.0f, 1.0f, 0.0f, -1.0f);
        i_this->mpBowMorf->setFrame(10.0f);
    }
    a_this->home.pos = a_this->current.pos;
}

static BOOL pl_check(e_rdy_class* i_this, f32 i_dist, s16 i_angle) {
    fopAc_ac_c* a_this = &i_this->actor;
    if (dComIfGp_event_runCheck()) {
        return FALSE;
    }

    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (S_find != 0) {
        i_dist = 10000.0f;
    }

    if (i_this->mPlayerDist < i_dist) {
        s16 angle = a_this->shape_angle.y - i_this->mPlayerAngle;
        if (angle < i_angle && angle > (s16)-i_angle && !other_bg_check(i_this, player)) {
            return TRUE;
        }

        for (int i = 0; i <= 2; i++) {
            if (i_this->mCcSph[i].ChkCoHit()) {
                fopAc_ac_c* hit_actor = dCc_GetAc(i_this->mCcSph[i].GetCoHitObj()->GetAc());
                if (daPy_getPlayerActorClass() == hit_actor) {
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

static dBomb_c* bomb_view_check(e_rdy_class* i_this) {
    return search_bomb(i_this, TRUE);
}

static dBomb_c* bomb_check(e_rdy_class* i_this) {
    return search_bomb(i_this, FALSE);
}

static BOOL move_gake_check(e_rdy_class* i_this, f32 i_dist) {
    fopAc_ac_c* a_this = &i_this->actor;
    dBgS_GndChk gnd_chk;
    cXyz vec, pos;
    mDoMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
    vec.x = 0.0f;
    vec.y = 100.0f;
    vec.z = i_dist;
    MtxPosition(&vec, &pos);
    pos += a_this->current.pos;
    gnd_chk.SetPos(&pos);
    if (a_this->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk) > 300.0f) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static BOOL way_check(e_rdy_class* i_this) {
    fopAc_ac_c* _this = (fopAc_ac_c*)i_this;
    cXyz start, end, vec;
    s16 angle = cM_rndF(65535.0f);
    f32 dist = 1000.0f;
    start = _this->current.pos;
    start.y += 50.0f;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 0x10; j++) {
            mDoMtx_YrotS(*calc_mtx, angle);
            vec.x = 0.0f;
            vec.y = 0.0f;
            vec.z = dist;
            MtxPosition(&vec, &end);
            end += start;
            dBgS_LinChk lin_chk;
            lin_chk.Set(&start, &end, _this);
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                angle += 0x1000;
            } else {
                i_this->mTargetAngleY = angle;
                return TRUE;
            }
        }
        dist -= 150.0f;
    }
    return FALSE;
}

static BOOL path_check(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    static bool check_index[0xff];
    dBgS_LinChk lin_chk;
    cXyz start, end;
    start = a_this->current.pos;
    start.y += 10.0f;
    dPnt* point = i_this->mpPath->m_points;
    for (int i = 0; i < i_this->mpPath->m_num; i++, point++) {
        if (i < 0xff) {
            end.x = point->m_position.x;
            end.y = point->m_position.y + 10.0f;
            end.z = point->m_position.z;
            lin_chk.Set(&start, &end, a_this);
            if (!dComIfG_Bgsp().LineCross(&lin_chk)) {
                check_index[i] = true;
            } else {
                check_index[i] = false;
            }
        }
    }

    f32 dist, delta_x, delta_y, delta_z;
    f32 threshold = 100.0f;
    bool check = false;
    for (int i = 0; i < 100; threshold += 50.0f, i++) {
        point = i_this->mpPath->m_points;
        for (int i = 0; i < i_this->mpPath->m_num; i++, point++) {
            if (i < 0xff && check_index[i]) {
                delta_x = a_this->current.pos.x - point->m_position.x;
                delta_y = a_this->current.pos.y - point->m_position.y;
                delta_z = a_this->current.pos.z - point->m_position.z;
                dist = JMAFastSqrt(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);
                if (dist < threshold) {
                    i_this->mPathIndex = i;
                    check = true;
                    break;
                }
            }
        }
        if (check) break;
    }

    if (!check) {
        return FALSE;
    } else {
        if (cM_rndF(1.0f) < 0.5f) {
            i_this->mPathDirection = 1;
        } else {
            i_this->mPathDirection = -1;
        }
        return TRUE;
    }
}

static void e_rdy_normal(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    f32 target_speed = 0.0f;
    s16 check_angle = 0x4000;

    switch (i_this->mMode) {
    case 0:
        if (i_this->field_0x5bb) {
            anm_init(i_this, ANM_ARMAMENT, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mMode = 1;
        } else {
            i_this->mMode = 2;
        }
        break;

    case 1:
        if ((int)i_this->mpMorf->getFrame() == 11) {
            i_this->field_0x5bb = false;
        }
        if (i_this->mpMorf->isStop()) {
            i_this->mMode = 2;
        }
        break;

    case 2:
        if (i_this->mTimer[0] == 0) {
            if (i_this->mpPath != NULL && path_check(i_this)) {
                i_this->mAction = ACT_JYUNKAI;
                i_this->mMode = 0;
                return;
            } else if (way_check(i_this)) {
                anm_init(i_this, ANM_WALK, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mMode = 3;
                i_this->mTimer[0] = cM_rndF(100.0f) + 100.0f;
            }
        } else {
            check_angle = 0x6000;
        }
        break;

    case 3:
        target_speed = l_HIO.mWalkSpeed;
        if (a_this->speedF >= 3.0f && move_gake_check(i_this, 100.0f)) {
            i_this->mMode = 4;
            i_this->mTimer[0] = cM_rndF(100.0f) + 50.0f;
            anm_init(i_this, ANM_KYORO2, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        } else if (i_this->mTimer[0] == 0
            || (a_this->speedF >= 3.0f && way_bg_check(i_this, 200.0f)))
        {
            i_this->mMode = 2;
            i_this->mTimer[0] = cM_rndF(100.0f) + 50.0f;
            anm_init(i_this, ANM_KYORO2, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
        break;

    case 4:
        i_this->field_0xa98 = -1;
        if (i_this->mTimer[0] == 0) {
            i_this->mTargetAngleY = a_this->current.angle.y + 0x8000 + (int)cM_rndFX(4000.0f);
            anm_init(i_this, ANM_WALK, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 3;
            i_this->mTimer[0] = cM_rndF(100.0f) + 100.0f;
        }
        break;
    }

    if (target_speed) {
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mTargetAngleY, 8, 0x400);
        s16 angle_diff = a_this->current.angle.y - i_this->mTargetAngleY;
        if (angle_diff > 0x400 || angle_diff < -0x400) {
            target_speed = 0.0f;
        }
    }
    cLib_addCalc2(&a_this->speedF, target_speed, 1.0f, 3.0f);

    f32 check_dist = i_this->mPlayerCheckDist;
    BOOL bVar1 = FALSE;
    if (i_this->field_0xa71 == 1 || S_find != 0) {
        if (i_this->field_0xa71 == 1) {
            bVar1 = TRUE;
        }
        check_dist = 10000.0f;
        check_angle = 0x7fff;
    }

    if (((i_this->mCounter & 7) == 0 || bVar1) && i_this->mMode >= 2
        && ((i_this->mCounter & 8) == 0 || bVar1))
    {
        if (pl_check(i_this, check_dist, check_angle)) {
            if (i_this->mWeaponType == WEAPON_CLUB) {
                i_this->mAction = ACT_FIGHT_RUN;
                i_this->mMode = -10;
                i_this->mTimer[0] = 60;
            } else if (i_this->mWeaponType >= WEAPON_BOW) {
                i_this->mAction = ACT_BOW_RUN;
                i_this->mMode = -10;
            }
        }
        if (bomb_view_check(i_this)) {
            i_this->mAction = ACT_BOMB_ACTION;
            i_this->mMode = 0;
        }
    }
}

static void e_rdy_fight_run(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz unused_xyz_0;
    cXyz unused_xyz_1;
    f32 target_speed = 0.0f;
    s8 bVar1 = true;
    if (!pl_check(i_this, i_this->mPlayerCheckDist + 50.0f, 0x7fff) && i_this->mTimer[0] == 0) {
        if (i_this->field_0xa6c == 0) {
            bVar1 = false;
        }
    } else {
        i_this->field_0xa6c = 20;
    }
    f32 run_speed = l_HIO.mRunSpeed;

    if (i_this->mWeaponType != WEAPON_CLUB) {
        if (i_this->mWeaponType >= WEAPON_BOW) {
            i_this->mAction = ACT_BOW_RUN;
        } else {
            i_this->mAction = ACT_NORMAL;
            i_this->mTimer[0] = cM_rndF(50.0f) + 50.0f;
            anm_init(i_this, ANM_KYORO2, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
        i_this->mMode = 0;
        return;
    }

    switch (i_this->mMode) {
    case -20:
        if ((int)i_this->mpMorf->getFrame() == 11) {
            i_this->field_0x5bb = false;
        }
        if (i_this->mpMorf->isStop()) {
            i_this->mMode = 0;
        }
        break;

    case -10:
        anm_init(i_this, ANM_WAIT01, 7.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mTimer[1] = cM_rndF(10.0f) + 15.0f;
        i_this->mMode = -9;
        break;

    case -9:
        if (i_this->mTimer[1] == 0) {
            i_this->mMode = 0;
        }
        break;

    case 0:
        if (i_this->field_0x5bb) {
            anm_init(i_this, ANM_ARMAMENT, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mMode = -20;
        } else if (i_this->mTimer[1] == 0) {
            anm_init(i_this, ANM_RUN, 5.0f, J3DFrameCtrl::EMode_LOOP, cM_rndFX(0.05f) + 1.0f);
            i_this->mMode = 1;
        }
        break;

    case 1:
        target_speed = run_speed;
        if (i_this->mPlayerDist < l_HIO.field_0x18) {
            anm_init(i_this, ANM_KAMAE, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 2;
        }
        break;

    case 2:
        target_speed = l_HIO.mWalkSpeed;
        if (i_this->mPlayerDist > l_HIO.field_0x18 + 50.0f) {
            i_this->mMode = 0;
        } else if (i_this->mPlayerDist < l_HIO.field_0x18 - 50.0f) {
            i_this->mMode = 3;
        }
        break;

    case 3:
        target_speed = -l_HIO.mWalkSpeed;
        if (i_this->mPlayerDist > l_HIO.field_0x18) {
            i_this->mMode = 2;
        }
        break;

    case 5:
        if (i_this->mTimer[3] != 0) {
            i_this->field_0xa98 = -1;
        }
        target_speed = 0.0f;
        a_this->speedF = 0.0f;
        if (!move_gake_check(i_this, 100.0f)) {
            i_this->mMode = 2;
        }
    }

    if (i_this->mMode < 5 && move_gake_check(i_this, 100.0f)) {
        i_this->mMode = 5;
        i_this->mTimer[3] = cM_rndF(15.0f) + 30.0f;
        anm_init(i_this, ANM_WAIT01, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
    }

    if (i_this->mMode >= 0) {
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngle, 4, 0x800);
        s16 angle_diff = a_this->current.angle.y - i_this->mPlayerAngle;
        if (angle_diff > 0x400 || angle_diff < -0x400) {
            target_speed = 0.0f;
        }
    }
    cLib_addCalc2(&a_this->speedF, target_speed, 1.0f, 5.0f);

    if (!bVar1) {
        i_this->mAction = ACT_NORMAL;
        if (i_this->mMode == 5) {
            i_this->mTargetAngleY = a_this->current.angle.y + 0x8000 + (int)cM_rndFX(4000.0f);
            anm_init(i_this, ANM_WALK, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 3;
            i_this->mTimer[0] = cM_rndF(100.0f) + 100.0f;
            i_this->mTimer[1] = 40;
        } else {
            i_this->mMode = 0;
            i_this->mTimer[0] = cM_rndF(50.0f) + 50.0f;
            anm_init(i_this, ANM_KYORO2, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
    } else {
        if (i_this->mPlayerDist < l_HIO.field_0x18 && (i_this->field_0xaf0 & cc_pl_cut_bit_get())
            && i_this->mKnockback < 0.1f)
        {
            i_this->mAction = ACT_AVOID;
            i_this->mMode = 0;
            i_this->mIFrameTimer = 20;
        } else if (!i_this->field_0x5bb && i_this->mPlayerDist < l_HIO.field_0x1c
            && i_this->mTimer[2] == 0)
        {
            i_this->mTimer[2] = cM_rndF(20.0f) + 20.0f;
            f32 chk_val = 0.5f;
            if (cM_rndF(1.0f) < chk_val) {
                i_this->mAction = ACT_FIGHT;
                i_this->mMode = 0;
            }
        }
    }

    if ((i_this->mCounter & 0xf) == 0 && bomb_view_check(i_this)) {
        i_this->mAction = ACT_BOMB_ACTION;
        i_this->mMode = 0;
    }

    if (i_this->mAction == ACT_FIGHT_RUN && i_this->field_0xa98 >= 0) {
        i_this->field_0xa98 = 1;
    }
}

static fopAc_ac_c* at_hit_check(e_rdy_class* i_this) {
    if (i_this->mMode >= 10) {
        return NULL;
    }

    if (i_this->mAtSph.ChkAtHit()) {
        return dCc_GetAc(i_this->mAtSph.GetAtHitObj()->GetAc());
    } else {
        return NULL;
    }
}

static void e_rdy_fight(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    int frame = (int)i_this->mpMorf->getFrame();
    int last_frame;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_ATTACK01, 6.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        if (l_HIO.field_0x38 == 0) {
            i_this->mTimer[0] = 25;
        } else if (l_HIO.field_0x38 == 1) {
            i_this->mTimer[0] = 15;
        } else if (l_HIO.field_0x38 == 2) {
            i_this->mTimer[0] = 10;
        }
        i_this->mMode = 1;
        i_this->mTimer[1] = 5;
        i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_WAND_ATTACK, -1);
        break;

    case 1:
        if (i_this->mTimer[1] != 0) {
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngle, 4, 0x800);
        }
        if (frame == 30) {
            i_this->mpMorf->setFrame(0.0f);
        } else if (i_this->mTimer[0] == 0) {
            anm_init(i_this, ANM_ATTACK01, 2.0f, J3DFrameCtrl::EMode_NONE, l_HIO.mAttackAnmSpeed);
            i_this->mpMorf->setFrame(30.0f);
            i_this->mMode = 2;
        }
        break;

    case 2:
        if (frame >= 44 && frame <= 47) {
            i_this->field_0xa7b = 1;
        }
        if (i_this->mpMorf->checkFrame(44.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_RD_SWING_CLUB, 0, -1);
        }
        if (i_this->mpMorf->checkFrame(50.0f)) {
            i_this->mpMorf->setPlaySpeed(1.0f);
        }
        if (l_HIO.field_0x38 == 0) {
            last_frame = 64;
        } else if (l_HIO.field_0x38 == 1) {
            last_frame = 54;
        } else if (l_HIO.field_0x38 == 2) {
            last_frame = 44;
        }
        if (frame >= last_frame) {
            i_this->mMode = 3;
            if (l_HIO.field_0x38 == 0) {
                i_this->mTimer[0] = 20;
            } else if (l_HIO.field_0x38 == 1) {
                i_this->mTimer[0] = 15;
            } else if (l_HIO.field_0x38 == 2) {
                i_this->mTimer[0] = 10;
            }
            anm_init(i_this, ANM_KAMAE, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
        break;

    case 3:
    case 10:
        if (i_this->mTimer[0] == 0) {
            i_this->mAction = ACT_FIGHT_RUN;
            i_this->mMode = 0;
        }
        break;
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 3.0f);

    if (i_this->field_0xa7b != 0) {
        fopAc_ac_c* hit_actor = at_hit_check(i_this);
        if (hit_actor != NULL && fopAcM_GetName(hit_actor) == PROC_ALINK
            && daPy_getPlayerActorClass()->checkPlayerGuard())
        {
            i_this->mpMorf->setPlaySpeed(-1.0f);
            dComIfGp_getVibration().StartShock(4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            dKy_Sound_set(a_this->current.pos, 100, fopAcM_GetID(i_this), 5);
            i_this->mMode = 10;
            i_this->mTimer[0] = 10;
        }
    }
}

static void e_rdy_bow_run(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    f32 target_speed = 0.0f;
    f32 run_speed = l_HIO.mRunSpeed;
    s16 target_angle = fopAcM_searchPlayerAngleY(a_this);
    s8 bVar2 = false;

    switch (i_this->mMode) {
    case -20:
        if ((int)i_this->mpMorf->getFrame() == 11) {
            i_this->field_0x5bb = false;
        }
        if (i_this->mpMorf->isStop()) {
            i_this->mMode = 0;
        }
        break;

    case -10:
        anm_init(i_this, ANM_WAIT01, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mTimer[0] = cM_rndF(10.0f) + 15.0f;
        i_this->mMode = -9;
        break;

    case -9:
        i_this->field_0xa98 = 1;
        if (i_this->mTimer[0] == 0) {
            i_this->mMode = 0;
        }
        break;

    case 0:
        if (i_this->field_0x5bb) {
            anm_init(i_this, ANM_ARMAMENT, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mMode = -20;
        } else {
            anm_init(i_this, ANM_RUN, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimer[0] = cM_rndF(50.0f) + 20.0f;
            if (i_this->mPlayerDist < l_HIO.field_0x28 - 100.0f) {
                i_this->mMode = 1;
            } else if (i_this->mPlayerDist > l_HIO.field_0x28 + 100.0f) {
                i_this->mMode = 2;
            } else {
                bVar2 = true;
            }
        }
        break;

    case 1:
        target_speed = run_speed;
        target_angle += 0x8000;
        if (i_this->mPlayerDist > l_HIO.field_0x28 || i_this->mTimer[0] == 0
            || i_this->mAcch.ChkWallHit())
        {
            bVar2 = true;
        }
        i_this->mTimer[2] = 20;
        break;

    case 2:
        target_speed = run_speed;
        if (i_this->mPlayerDist < l_HIO.field_0x28) {
            bVar2 = true;
        }
        break;
    }

    if (i_this->mMode >= 0) {
        cLib_addCalcAngleS2(&a_this->current.angle.y, target_angle, 2, 0x1000);
        s16 angle_diff = a_this->current.angle.y - target_angle;
        if (angle_diff > 0x800 || angle_diff < -0x800) {
            target_speed = 0.0f;
        }
    }
    cLib_addCalc2(&a_this->speedF, target_speed, 1.0f, 5.0f);

    if (move_gake_check(i_this, 200.0f)) {
        bVar2 = true;
    }

    if (!i_this->field_0x5bb && bVar2) {
        if (i_this->field_0x5b8 == 3) {
            i_this->mAction = ACT_BOW2;
        } else {
            i_this->mAction = ACT_BOW;
        }
        i_this->mMode = 0;
        i_this->mTimer[1] = cM_rndF(150.0f);
    } else if (i_this->mTimer[2] == 0 && !pl_check(i_this, i_this->mPlayerCheckDist + 50.0f, 0x7fff)) {
        i_this->mAction = ACT_NORMAL;
        i_this->mMode = 0;
        i_this->mTimer[0] = cM_rndF(50.0f) + 50.0f;
        anm_init(i_this, ANM_KYORO2, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
    } else if ((i_this->mCounter & 0xf) == 0 && bomb_view_check(i_this)) {
        i_this->mAction = ACT_BOMB_ACTION;
        i_this->mMode = 0;
    }

    if (i_this->mAction != ACT_BOW_RUN) {
        i_this->field_0xa98 = 0;
    }
}

static void e_rdy_bow(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    J3DAnmTransform* bck;
    int frame = (int)i_this->mpMorf->getFrame();

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_SHOOT_READY, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        bck = (J3DAnmTransform*)dComIfG_getObjectRes(i_this->mpArcName, 8);
        i_this->mpBowMorf->setAnm(bck, J3DFrameCtrl::EMode_NONE, 5.0f, 0.9f, 0.0f, -1.0f);
        i_this->mMode = 1;
        a_this->speedF = 0.0f;
        break;

    case 1:
        i_this->field_0xa98 = 4;
        if (frame >= 10) {
            i_this->mHasArrow = true;
        }
        if (frame == 20) {
            i_this->mSound.startCreatureSound(Z2SE_OBJ_ARROW_DRAW_NORMAL, 0, -1);
        }
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, ANM_SHOOT_WAIT, 4.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimer[0] = cM_rndF(10.0f) + 10.0f;
            i_this->mMode = 2;
        }
        break;

    case 2:
        i_this->field_0xa98 = 4;
        if (i_this->mTimer[0] == 0) {
            if (!i_this->field_0xa74 && !dMsgObject_isTalkNowCheck()) {
                anm_init(i_this, ANM_SHOOT, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                bck = (J3DAnmTransform*)dComIfG_getObjectRes(i_this->mpArcName, 11);
                i_this->mpBowMorf->setAnm(bck, J3DFrameCtrl::EMode_NONE, 1.0f, 1.0f, 0.0f, -1.0f);
                i_this->mMode = 3;
            } else {
                i_this->mAction = ACT_BOW_RUN;
                i_this->mMode = 0;
            }
        }
        i_this->mHasArrow = true;
        break;

    case 3:
        if (frame <= 3) {
            i_this->mHasArrow = true;
            i_this->field_0xa98 = 4;
        }
        if (frame == 2) {
            i_this->mArrowFired = true;
        }
        if (i_this->mpMorf->isStop()) {
            if (i_this->mTimer[1] == 0 && (i_this->mPlayerDist < l_HIO.field_0x28 - 200.0f
                                            || i_this->mPlayerDist > l_HIO.field_0x28)) {
                if (!(i_this->mPlayerDist < l_HIO.field_0x28 - 200.0f
                                            && way_bg_check(i_this, -100.0f))) {
                    i_this->mAction = ACT_BOW_RUN;
                }
                i_this->mMode = 0;
            } else {
                i_this->mMode = 0;
            }
        }
        break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngle, 4, 0x1000);
}

static void* s_command3_sub(void* i_proc, void* i_this) {
    if (fopAc_IsActor(i_proc) && fopAcM_GetName(i_proc) == PROC_E_RDY) {
        e_rdy_class* other = (e_rdy_class*)i_proc;
        if (other != i_this && other->mAction == ACT_COMMANDER) {
            return other;
        }
    }
    return NULL;
}

static s8 e_rdy_bow2(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*) dComIfGp_getPlayer(0);
    cXyz vec1, vec2;
    int frame = (int)i_this->mpMorf->getFrame();

    s8 ret = false;
    if (i_this->mMode <= 1) {
        if (i_this->mPlayerDist < i_this->mPlayerCheckDist && !other_bg_check(i_this, player)) {
            i_this->mMode = 2;
        }
        ret = true;
    }

    if (i_this->mpPath != NULL) {
        dPnt* point = i_this->mpPath->m_points;
        i_this->mArrowTarget.x = point->m_position.x;
        i_this->mArrowTarget.y = point->m_position.y;
        i_this->mArrowTarget.z = point->m_position.z;
        vec1 = i_this->mArrowTarget - a_this->current.pos;
        i_this->mPlayerAngle = cM_atan2s(vec1.x, vec1.z);
    }

    int sw_bit = (fopAcM_GetParam(a_this) & 0xff000000) >> 0x18;
    BOOL bVar2 = FALSE;
    if (sw_bit != 0xff && dComIfGs_isSwitch(sw_bit, fopAcM_GetRoomNo(a_this))) {
        bVar2 = TRUE;
    }

    switch (i_this->mMode) {
    case 0:
        if (i_this->mTimer[0] == 0) {
            anm_init(i_this, ANM_KYORO2, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 1;
            i_this->mTimer[0] = cM_rndF(40.0f) + 60.0f;
        }
        break;

    case 1:
        if (i_this->mTimer[0] == 0) {
            anm_init(i_this, ANM_WAIT01, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimer[0] = cM_rndF(100.0f) + 100.0f;
            i_this->mMode = 0;
        }
        break;

    case 2: {
        anm_init(i_this, ANM_SHOOT_READY, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mpBowMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mpArcName, 8),
                                   J3DFrameCtrl::EMode_NONE, 5.0f, TREG_F(3) + 0.9f, 0.0f, -1.0f);
        i_this->mMode = 3;
        a_this->speedF = 0.0f;
        break;
    }

    case 3:
        i_this->field_0xa98 = 4;
        if (frame >= 10) {
            i_this->mHasArrow = true;
        }
        if (frame == 20) {
            i_this->mSound.startCreatureSound(Z2SE_OBJ_ARROW_DRAW_NORMAL, 0, -1);
        }
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, ANM_SHOOT_WAIT, 4.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimer[0] = cM_rndF(10.0f);
            i_this->mMode = 4;
        }
        break;

    case 4:
        i_this->field_0xa98 = 4;
        if (i_this->mTimer[0] == 0) {
            if (i_this->mpPath == NULL && sw_bit == 0xff
                && (i_this->mPlayerDist > i_this->mPlayerCheckDist || other_bg_check(i_this, player)))
            {
                i_this->mMode = 0;
                return ret;
            }
            if ((sw_bit == 0xff || (sw_bit != 0xff && bVar2)) && !dMsgObject_isTalkNowCheck()
                && !i_this->field_0xa74)
            {
                anm_init(i_this, ANM_SHOOT, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mpBowMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mpArcName, 11),
                                           J3DFrameCtrl::EMode_NONE, 1.0f, 1.0f, 0.0f, -1.0f);
                i_this->mMode = 5;
            }
        }
        i_this->mHasArrow = true;
        break;

    case 5:
        if (frame <= 3 + XREG_S(5)) {
            i_this->mHasArrow = true;
            i_this->field_0xa98 = 4;
        }
        if (frame == 2 + XREG_S(7)) {
            i_this->mArrowFired = true;
        }
        if (i_this->mpMorf->isStop()) {
            if (i_this->mTimer[1] == 0 && i_this->mPlayerDist < l_HIO.field_0x28 - 200.0f) {
                if (!way_bg_check(i_this, -100.0f) && !move_gake_check(i_this, -200.0f)) {
                    i_this->mAction = ACT_BOW_RUN;
                    i_this->field_0x1365 = false;
                }
                i_this->mMode = 0;
            } else {
                i_this->mMode = 0;
            }
        }
        break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngle, 4, 0x1000);
    if (i_this->field_0xa98 != 0 && i_this->mpPath != NULL) {
        i_this->field_0xa98 = 14;
    }
    return ret;
}

static void e_rdy_bow_ikki2(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    J3DAnmTransform* bck;
    int frame = (int)i_this->mpMorf->getFrame();

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_WAIT01, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode = 1;
        a_this->current.pos.x = -92938.0f;
        a_this->current.pos.z = 49992.0f;
        break;

    case 1:
        if (player->current.pos.z > 35000.0f) {
            i_this->mDemoMode = 20;
            i_this->mDemoTimer = 0;
            i_this->mMode = 2;
        }
        break;

    case 2:
        i_this->mCamDist = 0.0f;
        break;

    case 3:
        cLib_addCalc2(&a_this->current.pos.x, -93658.0f, 1.0f, 50.0f);
        cLib_addCalc2(&a_this->current.pos.z, 49894.0f, 1.0f, 50.0f);
        break;

    case 4:
        anm_init(i_this, ANM_SHOOT_READY, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        bck = (J3DAnmTransform*)dComIfG_getObjectRes(i_this->mpArcName, 8);
        i_this->mpBowMorf->setAnm(bck, J3DFrameCtrl::EMode_NONE, 5.0f, 0.9f, 0.0f, -1.0f);
        i_this->mMode = 5;
        break;

    case 5:
        if (frame >= 10) {
            i_this->mHasArrow = true;
        }
        if (frame == 20) {
            i_this->mSound.startCreatureSound(Z2SE_OBJ_ARROW_DRAW_NORMAL, 0, -1);
        }
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, ANM_SHOOT_WAIT, 4.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 6;
            i_this->mTimer[0] = 40;
        }
        break;

    case 6:
        if (i_this->mTimer[0] == 0 && !i_this->field_0xa74) {
            anm_init(i_this, ANM_SHOOT, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            bck = (J3DAnmTransform*)dComIfG_getObjectRes(i_this->mpArcName, 11);
            i_this->mpBowMorf->setAnm(bck, J3DFrameCtrl::EMode_NONE, 1.0f, 1.0f, 0.0f, -1.0f);
            i_this->mMode = 7;
        }
        i_this->mHasArrow = true;
        break;

    case 7:
        if (frame <= 3) {
            i_this->mHasArrow = true;
        }
        if (frame == 2) {
            i_this->mArrowFired = true;
        }
        if (i_this->mpMorf->isStop()) {
            i_this->mMode = 2;
            anm_init(i_this, ANM_WAIT01, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
        break;
    }

    if (i_this->mMode >= 6) {
        i_this->field_0xa98 = 14;
        i_this->mArrowTarget.set(-93666.0f, player->current.pos.y + i_this->mCamDist,
                                player->current.pos.z);
        f32 fVar2;
        if (i_this->mDemoMode > 25) {
            fVar2 = -1000.0f;
        } else {
            fVar2 = 5400.0f;
        }
        cLib_addCalc2(&i_this->mCamDist, fVar2, 0.1f, 200.0f);
    }
}

static void e_rdy_avoid(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    int frame = (int)i_this->mpMorf->getFrame();

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_AVOID, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode = 1;
        a_this->speedF = -30.0f;
        a_this->speed.y = 20.0f;
        i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_DODGE, -1);
        break;

    case 1:
        if (frame == 6) {
            i_this->mpMorf->setPlaySpeed(0.0f);
            i_this->mMode = 2;
        }
        break;

    case 2:
        if (i_this->mAcch.ChkGroundHit()) {
            i_this->mpMorf->setPlaySpeed(1.0f);
            i_this->mMode = 3;
        }
        break;

    case 3:
        cLib_addCalc0(&a_this->speedF, 1.0f, 10.0f);
        if (i_this->mpMorf->isStop()) {
            i_this->mAction = ACT_FIGHT_RUN;
            i_this->mMode = 0;
            i_this->mTimer[0] = 30;
        }
        break;
    }
}

static void* s_kusa_sub(void* i_proc, void* i_this) {
    if (fopAc_IsActor(i_proc) && fopAcM_GetName(i_proc) == PROC_Obj_Yobikusa) {
        fopAc_ac_c* grass = (fopAc_ac_c*)i_proc;
        fopAc_ac_c* _this = (fopAc_ac_c*)i_this;
        cXyz delta = grass->current.pos - _this->current.pos;
        if (delta.abs() < 1000.0f) {
            return i_proc;
        }
    }
    return NULL;
}

static void e_rdy_tkusa(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz vec;
    cXyz unused_vec;
    i_this->mIFrameTimer = 20;

    if (i_this->mKargarokID == fpcM_ERROR_PROCESS_ID_e) {
        i_this->mKargarokID = fopAcM_GetID(fopAcM_SearchByName(PROC_E_YC));
    }

    fopAc_ac_c* a_karg = (fopAc_ac_c*) fopAcM_SearchByID(i_this->mKargarokID);
    e_yc_class* kargarok = (e_yc_class*) a_karg;
    if (kargarok != NULL) {
        kargarok->mRiderID = fopAcM_GetID(i_this);
    }

    int frame = (int)i_this->mpMorf->getFrame();
    f32 target_speed = 0.0f;

    fopAc_ac_c* grass = (fopAc_ac_c*)fpcM_Search(s_kusa_sub, i_this);
    if (grass == NULL) {
        return;
    }

    if (i_this->mMode <= 1 && i_this->mPlayerDist < 1300.0f + TREG_F(7)) {
        i_this->mDemoMode = 10;
        i_this->mDemoTimer = 0;
        i_this->mMode = 2;
        anm_init(i_this, ANM_WAIT01, 7.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
    }

    switch (i_this->mMode) {
    case 0:
        if (i_this->mTimer[0] == 0) {
            anm_init(i_this, ANM_WALK, 7.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimer[0] = cM_rndF(60.0f) + 60.0f;
            i_this->mMode = 1;
            vec.x = a_this->home.pos.x + cM_rndFX(200.0f) - a_this->current.pos.x;
            vec.z = a_this->home.pos.z + cM_rndFX(200.0f) - a_this->current.pos.z;
            i_this->mTargetAngleY = cM_atan2s(vec.x, vec.z);
        }
        break;

    case 1:
        target_speed = l_HIO.mWalkSpeed;
        if (i_this->mTimer[0] == 0) {
            anm_init(i_this, ANM_KYORO2, 7.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 0;
            i_this->mTimer[0] = cM_rndF(30.0f) + 60.0f;
        }
        break;

    case 2:
        if (i_this->mDemoMode == 11) {
            if (i_this->mDemoTimer > 70) {
                vec = grass->current.pos - a_this->current.pos;
                i_this->mTargetAngleY = cM_atan2s(vec.x, vec.z) + -1000 + NREG_S(7);
            } else if (i_this->mDemoTimer > 30) {
                i_this->mTargetAngleY = i_this->mPlayerAngle;
                if (i_this->mDemoTimer == 31) {
                    i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_NOTICE, -1);
                }
            }
        } else if (i_this->mDemoTimer == 110) {
            anm_init(i_this, ANM_RUN, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 3;
            i_this->mDemoMode++;
            i_this->mDemoTimer = 0;
        }
        break;

    case 3:
        target_speed = l_HIO.mRunSpeed;
        vec = grass->current.pos - a_this->current.pos;
        i_this->mTargetAngleY = cM_atan2s(vec.x, vec.z);
        if (vec.abs() < 160.0f + TREG_F(8)) {
            anm_init(i_this, ANM_WAIT01, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimer[0] = 20;
            i_this->mMode = 4;
        }
        break;

    case 4:
        if (i_this->mTimer[0] == 0) {
            anm_init(i_this, ANM_PICKUP, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mMode = 5;
        }
        break;

    case 5:
        if (frame == 15 + TREG_S(7)) {
            fopAcM_setCarryNow(grass, FALSE);
            i_this->mIsHoldingGrass = true;
            i_this->mSound.startCreatureSound(Z2SE_AL_PICKUP_GRASS, 0, -1);
            i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_CALLING, -1);
        }
        if (frame == 30 + TREG_S(8)) {
            i_this->mDemoMode++;
        }
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, ANM_BLOW, 0.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 6;
            i_this->mTimer[0] = 220;
        }
        break;

    case 6:
        if (i_this->mTimer[0] == 215) {
            mDoAud_seStart(Z2SE_PLAY_REED_TOBIKUSA, NULL, 0, 0);
            i_this->mTimer[2] = 100;
        }
        if (i_this->mTimer[0] == 140) {
            i_this->mDemoMode++;
            i_this->mDemoTimer = 0;
        }
        if (i_this->mTimer[0] == 100) {
            mDoAud_seStart(Z2SE_PLAY_REED_TOBIKUSA, NULL, 0, 0);
            i_this->mTimer[2] = 100;
        }
        if (i_this->mTimer[0] == 50) {
            i_this->mMode = 7;
            i_this->mTimer[0] = 3;
            kargarok->mMode = 2;
            a_karg->current = a_karg->home;
            a_karg->shape_angle = a_karg->current.angle;
            kargarok->mNoDrawFlag = true;
            kargarok->mAngleSpeedRatio = 0.0f;
        }
        break;

    case 7:
        if (i_this->mTimer[0] == 1) {
            i_this->mDemoMode = 16;
            i_this->mDemoTimer = 0;
            anm_init(i_this, ANM_WAIT01, 7.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            Z2GetAudioMgr()->setBattleBgmOff(false);
        }
        if (i_this->mDemoTimer == 309 + XREG_S(2)) {
            anm_init(i_this, ANM_LOOKBACK, 10.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mIsHoldingGrass = false;
            kargarok->mMode = 4;
            a_karg->speedF = 30.0f;
            cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
            vec.x = 1100.0f + XREG_F(10);
            vec.y = 1050.0f + XREG_F(11);
            vec.z = -1100.0f + XREG_F(12);
            MtxPosition(&vec, &a_karg->current.pos);
            a_karg->current.pos += a_this->current.pos;
            vec = a_this->current.pos - a_karg->current.pos;
            a_karg->current.angle.y = cM_atan2s(vec.x, vec.z);
            a_karg->shape_angle.y = a_karg->current.angle.y;
            a_karg->current.angle.z = a_karg->shape_angle.z = NREG_S(6) + 0x2000;
            i_this->mDemoMode = 17;
            i_this->mDemoTimer = 0;
            i_this->mMode = 8;
        }
        break;

    case 8:
        if (i_this->mDemoTimer == 95) {
            anm_init(i_this, ANM_RUN, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            vec = a_karg->current.pos - a_this->current.pos;
            i_this->mTargetAngleY = cM_atan2s(vec.x, vec.z);
            i_this->mDemoMode = 18;
            i_this->mDemoTimer = 0;
            i_this->mMode = 9;
            kargarok->mMode = 5;
        }
        break;

    case 9:
        if (i_this->mDemoTimer == 10) {
            anm_init(i_this, ANM_JUMP_A, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_JUMP_RIDING, -1);
            i_this->mMode = 10;
            a_this->home = a_this->current;
        }
        break;

    case 10:
        if (i_this->mpMorf->isStop()) {
            i_this->mMode = 11;
            anm_init(i_this, ANM_JUMP_B, 10.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mRideState = 1;
            i_this->mJumpSpeed = 50.0f + TREG_F(4);
            i_this->mTimer[3] = 30 + JREG_S(6);
        }
        break;

    case 11:
        if (i_this->mJumpHeight <= 0.0f) {
            anm_init(i_this, ANM_RJUMP_C, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mMode = 12;
            kargarok->field_0x698 = -27.0f + JREG_F(17);
        }
        break;

    case 12:
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, ANM_RHOVERING, 15.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mpMorf->setFrame(kargarok->mpMorf->getFrame());
            i_this->mMode = 13;
            kargarok->mMode = 6;
            i_this->mDemoMode = 19;
            i_this->mDemoTimer = 0;
        }
        break;

    case 13:
        break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mTargetAngleY, 2, 0x1000);
    s16 angle_diff = a_this->current.angle.y - i_this->mTargetAngleY;
    if (angle_diff > 0x400 || angle_diff < -0x400) {
        target_speed = 0.0f;
    }
    cLib_addCalc2(&a_this->speedF, target_speed, 1.0f, 5.0f);
}

static void e_rdy_yc_ride(e_rdy_class* i_this) {
    int frame = (int)i_this->mpMorf->getFrame();
    e_yc_class* kargarok = (e_yc_class*)fopAcM_SearchByID(i_this->mKargarokID);
    i_this->mIFrameTimer = 20;

    if (kargarok == NULL) {
        kargarok = (e_yc_class*)fopAcM_SearchByName(PROC_E_YC);
        i_this->mKargarokID = fopAcM_GetID(kargarok);
        return;
    }
    
    if (i_this->field_0xa6e != 0) {
        return;
    }

    int kargarok_frame = (int)kargarok->mpMorf->getFrame();
    if (i_this->mAnm != ANM_RHANGED && i_this->mAnm != ANM_RHANGED_BRASH
        && i_this->mAnm != ANM_RHANGED_BRASH2 && i_this->mAnm != ANM_RHANGED_DAMAGE)
    {
        i_this->field_0xa98 = 4;
    } else {
        i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_DAMAGE, -1);
    }

    switch (i_this->mMode) {
    case 0:
        if (kargarok->mAnm == e_yc_class::ANM_HOVERING && i_this->mAnm != ANM_RHOVERING) {
            anm_init(i_this, ANM_RHOVERING, 15.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mpMorf->setFrame(kargarok->mpMorf->getFrame());
        } else if (kargarok->mAnm == e_yc_class::ANM_FLY && i_this->mAnm != ANM_RFLY) {
            anm_init(i_this, ANM_RFLY, 15.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mpMorf->setFrame(kargarok->mpMorf->getFrame());
        } else if (kargarok->mAnm == e_yc_class::ANM_FLY_GLIDE && i_this->mAnm != ANM_RFLY_GLIDE) {
            anm_init(i_this, ANM_RFLY_GLIDE, 15.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mpMorf->setFrame(kargarok->mpMorf->getFrame());
        } else if (kargarok->mAnm == e_yc_class::ANM_HANGED && kargarok_frame == 1) {
            anm_init(i_this, ANM_RHANGED, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        } else if (kargarok->mAnm == e_yc_class::ANM_HANGED_WAIT
                                                            && i_this->mAnm != ANM_RHANGED_WAIT) {
            anm_init(i_this, ANM_RHANGED_WAIT, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mpMorf->setFrame(kargarok->mpMorf->getFrame());
        } else if (kargarok->mAnm == e_yc_class::ANM_HANGED_DAMAGE && kargarok_frame == 1) {
            anm_init(i_this, ANM_RHANGED_DAMAGE, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        } else if (kargarok->mAnm == e_yc_class::ANM_HANGED_BRUSH && kargarok_frame == 1) {
            anm_init(i_this, ANM_RHANGED_BRASH, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        } else if (kargarok->mAnm == e_yc_class::ANM_HANGED_BRUSH2 && kargarok_frame == 1) {
            anm_init(i_this, ANM_RHANGED_BRASH2, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        }
        if (i_this->mAnm == ANM_RHOVERING) {
            i_this->mMode = 1;
            i_this->mTimer[0] = cM_rndF(30.0f) + 30.0f;
        }
        break;

    case 1:
        if (i_this->mTimer[0] == 0) {
            anm_init(i_this, ANM_RSHOOT_READY, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes(i_this->mpArcName, 9);
            i_this->mpBowMorf->setAnm(bck, J3DFrameCtrl::EMode_NONE, 5.0f, 1.0f, 0.0f, -1.0f);
            i_this->mMode = 2;
        }
        break;

    case 2:
        if (frame == 14) {
            i_this->mSound.startCreatureSound(Z2SE_OBJ_ARROW_DRAW_NORMAL, 0, -1);
        }
        if (i_this->mpMorf->isStop()) {
            i_this->mMode = 3;
            anm_init(i_this, ANM_RSHOOT_WAIT, 4.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
        i_this->mHasArrow = true;
        break;

    case 3:
        if (i_this->mTimer[1] == 0 && !dComIfGp_event_runCheck()) {
            anm_init(i_this, ANM_RSHOOT, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes(i_this->mpArcName, 11);
            i_this->mpBowMorf->setAnm(bck, J3DFrameCtrl::EMode_NONE, 1.0f, 1.0f, 0.0f, -1.0f);
            i_this->mMode = 4;
        }
        i_this->mHasArrow = true;
        break;

    case 4:
        if (frame <= 3) {
            i_this->mHasArrow = true;
        }
        if (frame == 2) {
            i_this->mArrowFired = true;
        }
        if (i_this->mpMorf->isStop()) {
            if (kargarok->mAnm == e_yc_class::ANM_HOVERING) {
                anm_init(i_this, ANM_RSHOOT_READY, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes(i_this->mpArcName, 9);
                i_this->mpBowMorf->setAnm(bck, J3DFrameCtrl::EMode_NONE, 5.0f, 1.0f, 0.0f, -1.0f);
                i_this->mMode = 2;
                i_this->mTimer[1] = cM_rndF(30.0f) + 20.0f;
            } else {
                i_this->mMode = 0;
            }
        }
        break;
    }

    i_this->mRideState = 2;
    i_this->field_0xa8e = 1;
    kargarok->field_0x6b0 = 1;
    if (i_this->mAnm == ANM_RFLY || i_this->mAnm == ANM_RFLY_GLIDE) {
        i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_RD_V_RIDING_YELL, -1);
    }
}

static void e_rdy_bomb_action(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz vec1, vec2;
    dBomb_c* bomb = bomb_check(i_this);
    if (bomb == NULL) {
        i_this->mAction = ACT_FIGHT_RUN;
        i_this->mMode = 0;
        return;
    }

    vec1.x = bomb->current.pos.x - a_this->current.pos.x;
    vec1.z = bomb->current.pos.z - a_this->current.pos.z;
    s16 target_angle = cM_atan2s(vec1.x, vec1.z);
    f32 target_speed = 0.0f;

    switch (i_this->mMode) {
    case 0:
        i_this->mMode = 1;
        anm_init(i_this, ANM_WAIT01, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mTimer[1] = cM_rndF(10.0f) + 10.0f;
        // fallthrough

    case 1:
        if (i_this->mTimer[1] == 0 && bomb->speedF < 0.1f) {
            i_this->mMode = 2;
            anm_init(i_this, ANM_RUN, 5.0f, J3DFrameCtrl::EMode_LOOP, cM_rndFX(0.05f) + 1.0f);
        }
        break;

    case 2:
        if (bomb->speedF < 0.1f && !bomb->checkStateCarry()
            && JMAFastSqrt(vec1.x * vec1.x + vec1.z * vec1.z) < 250.0f)
        {
            s16 angle = i_this->mPlayerAngle - a_this->shape_angle.y;
            if (angle < 0x4000 && angle > -0x4000) {
                i_this->mMode = 5;
                break;
            }
        }
        target_angle += 0x8000;
        target_speed = l_HIO.mRunSpeed;
        if (JMAFastSqrt(vec1.x * vec1.x + vec1.z * vec1.z) > 600.0f) {
            i_this->mMode = 3;
            anm_init(i_this, ANM_WAIT01, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
        break;

    case 3:
        target_angle = i_this->mPlayerAngle;
        if (JMAFastSqrt(vec1.x * vec1.x + vec1.z * vec1.z) < 500.0f) {
            i_this->mMode = 0;
        }
        break;

    case 5:
        target_speed = l_HIO.mRunSpeed;
        i_this->mCollisionEnabled = false;
        if (bomb->speedF > 0.1f) {
            i_this->mMode = 0;
        } else if (JMAFastSqrt(vec1.x * vec1.x + vec1.z * vec1.z) < 100.0f) {
            i_this->mTimer[0] = 20;
            i_this->mMode = 6;
            anm_init(i_this, ANM_KICK, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        }
        break;

    case 6:
        i_this->mCollisionEnabled = false;
        target_angle = i_this->mPlayerAngle;
        if ((int)i_this->mpMorf->getFrame() <= 10) {
            mDoMtx_YrotS(*calc_mtx, target_angle);
            vec1.x = -30.0f;
            vec1.y = 0.0f;
            vec1.z = 60.0f;
            MtxPosition(&vec1, &vec2);
            vec2 += a_this->current.pos;
            cLib_addCalc2(&bomb->current.pos.x, vec2.x, 1.0f, 15.0f);
            cLib_addCalc2(&bomb->current.pos.z, vec2.z, 1.0f, 15.0f);
            if ((int)i_this->mpMorf->getFrame() == 10) {
                bomb->speedF = 30.0f;
                bomb->speed.y = 30.0f;
                bomb->current.angle.y = a_this->shape_angle.y;
            }
        }
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, ANM_WAIT01, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 3;
        }
        break;
    }

    cLib_addCalc2(&a_this->speedF, target_speed, 1.0f, 5.0f);
    cLib_addCalcAngleS2(&a_this->current.angle.y, target_angle, 4, 0x1000);
    i_this->field_0xa98 = 11;
    i_this->mArrowTarget = bomb->current.pos;
}

static void e_rdy_s_damage(e_rdy_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->actor;
    switch (i_this->mMode) {
    case 0:
        i_this->mMode = 1;
        i_this->mTimer[0] = 10;
        if (i_this->field_0xa8e != 0) {
            e_yc_class* kargarok = (e_yc_class*)fopAcM_SearchByID(i_this->mKargarokID);
            if (kargarok != NULL) {
                kargarok->field_0x6b0 &= ~i_this->field_0xa8e;
            }
            i_this->field_0xa8e = 0;
            i_this->mKargarokID = fpcM_ERROR_PROCESS_ID_e;
        }
        break;

    case 1:
        if (i_this->mTimer[1] != 0) {
            a_this->onHeadLockFlg();
        }
        if (i_this->mpMorf->isStop()) {
            if (i_this->mPrevAction == ACT_BOW2) {
                i_this->mAction = ACT_BOW2;
            } else if (i_this->mPrevAction == ACT_BOW3) {
                i_this->mAction = ACT_BOW3;
            } else {
                i_this->mAction = ACT_FIGHT_RUN;
                i_this->mTimer[0] = 40;
            }
            i_this->mMode = 0;
        }
    }
    
    cLib_addCalc0(&a_this->speedF, 1.0f, 3.0f);
}

static int kado_check(e_rdy_class* i_this) {
    fopAc_ac_c* _this = (fopAc_ac_c*)i_this;
    dBgS_LinChk lin_chk;
    cXyz vec1, vec2, vec3;
    int ret = 0;
    mDoMtx_YrotS(*calc_mtx, i_this->field_0xadc.y);
    
    static int kado_bit[2] = {1, 2};
    static f32 kado_check_x[2] = {80.0f, -80.0f};

    for (int i = 0; i < 2; i++) {
        vec2 = _this->current.pos;
        vec2.y += 100.0f;
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = 70.0f;
        MtxPosition(&vec1, &vec3);
        vec2 += vec3;

        vec1.y = 50.0f;
        vec1.z = -150.0f;
        vec1.x = kado_check_x[i];
        MtxPosition(&vec1, &vec3);
        vec3 += vec2;
        
        lin_chk.Set(&vec2, &vec3, _this);
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            ret |= kado_bit[i];
        }
    }

    if (ret == 3) {
        ret = 0;
    }
    return ret;
}

static void rd_disappear(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz vec1, vec2;
    MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(JNT_MUNE2), *calc_mtx);
    vec1.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&vec1, &vec2);
    fopAcM_createDisappear(a_this, &vec2, 10, 1, 12);

    if (i_this->field_0x5b8 == 12) {
        i_this->mKargarokDeleteTimer = 50;
        Z2GetAudioMgr()->setBattleBgmOff(true);
    } else {
        fopAcM_delete(a_this);
    }

    int sw_bit = fopAcM_GetParam(i_this) >> 0x18;
    if (sw_bit != 0xff) {
        dComIfGs_onSwitch(sw_bit, fopAcM_GetRoomNo(a_this));
    }
}

static BOOL body_gake(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz vec1, vec2;
    MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(JNT_MUNE2), *calc_mtx);
    vec1.set(40.0f, 0.0f, 0.0f);
    MtxPosition(&vec1, &vec2);
    dBgS_ObjGndChk gnd_chk;
    vec2.y += 100.0f;
    gnd_chk.SetPos(&vec2);
    vec2.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (a_this->current.pos.y - vec2.y > 300.0f) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static void e_rdy_damage(e_rdy_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->actor;
    cXyz vec1, vec2;

    if (a_this->health <= 0) {
        fopAcM_OffStatus(a_this, 0);
        a_this->attention_info.flags = 0;
    }

    i_this->mTargetEyeScale = 0.0f;

    if (!daPy_py_c::checkNowWolf() && a_this->checkCutDownHitFlg()) {
        a_this->offCutDownHitFlg();
        i_this->mMode = 3;
        i_this->mTimer[0] = 100;
        i_this->mTimer[1] = 45;
        a_this->health = 0;
        i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_DEATH, -1);
        i_this->mIsDying = true;
        daPy_getPlayerActorClass()->onEnemyDead();
        i_this->field_0xbac = 15;
        a_this->offDownFlg();
    }

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_DIEB, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode = 1;
        a_this->speedF = 0.0f;
        ride_off(i_this);
        i_this->field_0xb88 = 8000.0f;
        if (a_this->health <= 0) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_DEATH, -1);
            i_this->mIsDying = true;
            daPy_getPlayerActorClass()->onEnemyDead();
        } else {
            i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_DAMAGE, -1);
        }
        i_this->mTimer[0] = 0;
        break;

    case 1:
        if (i_this->mAcch.ChkWallHit() && i_this->mTimer[0] == 0) {
            int check = kado_check(i_this);
            if (check != 0) {
                if (check == 2) {
                    i_this->field_0xac6 = 0x1000;
                    i_this->field_0xadc.y += -7000;
                } else {
                    i_this->field_0xac6 = -0x1000;
                    i_this->field_0xadc.y += 7000;
                }
                i_this->field_0xb88 = 8000.0f;
                i_this->field_0xaf4 = 100.0f;
                i_this->field_0xafc = 100.0f;
                i_this->mTimer[0] = 60;
            } else {
                i_this->mMode = 10;
                a_this->speed.y = 0.0f;
                i_this->field_0xabc *= 0.2f;
                i_this->field_0xadc.y += 0x8000;
                i_this->field_0xbc0 = 5;
                i_this->field_0xaf4 = 100.0f;
                i_this->field_0xafc = 100.0f;
                i_this->field_0xac6 = 0;
            }
        } else {
            cLib_addCalcAngleS2(&i_this->field_0xadc.x, -0x4000, 1, 0x300);
            a_this->current.angle.y += i_this->field_0xac6;
            a_this->shape_angle.y = a_this->current.angle.y;
            if (i_this->field_0xadc.x <= -0x3000 && i_this->mAcch.ChkGroundHit()) {
                i_this->field_0xaee = 10;
                if (i_this->mInWater) {
                    i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_ASASE_M, 0, -1);
                } else {
                    i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_M, 0, -1);
                }
                i_this->mMode = 2;
                s16 angle_y;
                if (!i_this->mIsUpsideDown) {
                    anm_init(i_this, ANM_DIEA, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                    angle_y = i_this->field_0xadc.y;
                } else {
                    angle_y = i_this->field_0xadc.y + 0x8000;
                }
                a_this->current.angle.y = angle_y;
                if (i_this->field_0xac8) {
                    i_this->field_0xabc = 0.0f;
                    i_this->field_0xb88 = 2000.0f;
                } else {
                    if (!i_this->mIsUpsideDown) {
                        i_this->field_0xaf4 = -100.0f;
                        i_this->field_0xafc = -100.0f;
                    } else {
                        i_this->field_0xaf4 = 100.0f;
                        i_this->field_0xafc = 100.0f;
                    }
                    i_this->field_0xaf8 = -0x5000;
                    i_this->field_0xb00 = -0x4000;
                    i_this->field_0xabc *= 0.5f;
                    a_this->speed.y = 20.0f;
                    i_this->field_0xac6 /= 2;
                    i_this->field_0xb88 = 8000.0f;
                    i_this->mGroundHit = true;
                }
            }
        }
        break;

    case 2:
        a_this->current.angle.y += i_this->field_0xac6;
        a_this->shape_angle.y = a_this->current.angle.y;
        cLib_addCalcAngleS2(&i_this->field_0xadc.x, -0x4000, 1, 0x300);
        if (i_this->mAcch.ChkGroundHit()) {
            if (a_this->health > 0 && !daPy_py_c::checkNowWolf()) {
                a_this->onDownFlg();
            }
            dKy_Sound_set(a_this->current.pos, 100, fopAcM_GetID(i_this), 5);
            i_this->field_0xaee = 10;
            if (!i_this->mIsUpsideDown) {
                anm_init(i_this, ANM_DIEA, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            } else {
                anm_init(i_this, ANM_DIEB, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            }
            i_this->field_0xabc = 0.0f;
            i_this->field_0xadc.x = -0x4000;
            s16 angle_y;
            if (!i_this->mIsUpsideDown) {
                angle_y = i_this->field_0xadc.y;
                for (int i = 0; i < 4; i++) {
                    i_this->field_0xba2[i] = cM_rndF(10.0f);
                }
            } else {
                angle_y = i_this->field_0xadc.y + 0x8000;
            }
            a_this->current.angle.y = angle_y;
            if (daPy_py_c::checkNowWolf()) {
                i_this->mTimer[0] = 80;
                i_this->mTimer[1] = 55;
            } else {
                i_this->mTimer[0] = cM_rndF(10.0f) + 60.0f;
                i_this->mTimer[1] = 35;
            }
            i_this->mMode = 3;
            i_this->field_0xb88 = 1500.0f;
            i_this->mGroundHit = true;
        }
        break;

    case 3:
        if (body_gake(i_this)) {
            i_this->field_0xabc = -20.0f;
            i_this->field_0xadc.x += -0x300;
        }
        if (a_this->health <= 0 && i_this->mTimer[1] == 0) {
            rd_disappear(i_this);
            return;
        }
        if (daPy_getPlayerActorClass()->getCutType() != daPy_py_c::CUT_TYPE_DOWN
            && i_this->mTimer[0] == 0)
        {
            i_this->field_0xabc = 0.0f;
            i_this->field_0xadc.x = -0x4000;
            a_this->offDownFlg();
            i_this->mMode = 20;
            if (!i_this->mIsUpsideDown) {
                anm_init(i_this, ANM_DIEA_UP, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            } else {
                anm_init(i_this, ANM_DIEB_UP, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            }
            if (i_this->field_0x5b8 == 12) {
                i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_CALLING, -1);
            }
        }
        break;

    case 10:
        if (i_this->field_0xbc0 != 0) {
            cLib_addCalcAngleS2(&i_this->field_0xadc.x, 0, 1, 0x300);
        } else {
            cLib_addCalcAngleS2(&i_this->field_0xadc.x, -0x4000, 1, 0x800);
            if (i_this->mAcch.ChkGroundHit()) {
                i_this->mMode = 2;
                if (!i_this->mIsUpsideDown) {
                    anm_init(i_this, ANM_DIEA, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                    i_this->field_0xaf4 = -100.0f;
                    i_this->field_0xafc = -100.0f;
                } else {
                    i_this->field_0xaf4 = 100.0f;
                    i_this->field_0xafc = 100.0f;
                }
                i_this->field_0xaf8 = -0x5000;
                i_this->field_0xb00 = -0x4000;
                i_this->field_0xabc *= 0.5f;
                a_this->speed.y = 20.0f;
                s16 angle_y;
                if (i_this->mIsUpsideDown == 0) {
                    angle_y = i_this->field_0xadc.y;
                } else {
                    angle_y = i_this->field_0xadc.y + 0x8000;
                }
                a_this->current.angle.y = angle_y;
                i_this->field_0xb88 = 8000.0f;
            }
        }
        break;

    case 20:
        if (i_this->mpMorf->getFrame() >= 35.0f) {
            i_this->field_0xae8.z = 0;
            i_this->field_0xae8.x = 0;
        }
        if (i_this->mpMorf->isStop()) {
            i_this->field_0xadc.y = 0;
            i_this->field_0xadc.x = 0;
            anm_init(i_this, ANM_WAIT01, 0.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimer[0] = 5;
            i_this->mMode = 21;
            a_this->offDownFlg();
        }
        break;

    case 21:
        if (i_this->mTimer[0] == 0) {
            i_this->mTimer[0] = cM_rndF(20.0f) + 20.0f;
            anm_init(i_this, ANM_KYORO2, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 22;
        }
        break;

    case 22:
        i_this->mIFrameTimer = 0;
        if (i_this->mTimer[0] == 0) {
            i_this->mAction = ACT_NORMAL;
            i_this->mMode = 0;
            i_this->mTimer[0] = cM_rndF(50.0f) + 50.0f;
            anm_init(i_this, ANM_KYORO2, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
        break;

    case 30:
        if (!a_this->checkWolfDownPullFlg()) {
            i_this->mTimer[1] = 0;
        }
        if (i_this->mTimer[0] == 0) {
            i_this->mMode = 31;
            if (!i_this->mIsUpsideDown) {
                anm_init(i_this, ANM_DIEA_UP, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            } else {
                anm_init(i_this, ANM_DIEB_UP, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            }
            i_this->mTimer[0] = 10;
        } else if (i_this->mTimer[1] == 1) {
            a_this->onWolfDownPullEndFlg();
            a_this->offCutDownHitFlg();
            a_this->health = 0;
            i_this->mTimer[0] = 1000;
            i_this->mTimer[1] = 35;
            i_this->mMode = 3;
        }
        break;

    case 31:
        if (i_this->mTimer[0] == 0) {
            a_this->offDownFlg();
            i_this->mMode = 20;
            i_this->field_0xbca = 0;
        }
        break;
    }

    if (a_this->checkDownFlg()) {
        MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(JNT_MUNE2), *calc_mtx);
        vec1.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&vec1, &vec2);
        a_this->setDownPos(&vec2);
        i_this->mCollisionEnabled = false;
    }

    if (i_this->field_0xbac != 0) {
        i_this->field_0xbac--;
        i_this->field_0xbaa = i_this->field_0xbac * cM_ssin(i_this->field_0xbac * 0x3800) * 100.0f;
    }
}

static s16 gake_check(e_rdy_class* i_this, f32 i_dist) {
    fopAc_ac_c* _this = (fopAc_ac_c*)i_this;
    dBgS_GndChk gnd_chk;
    dBgS_LinChk lin_chk;
    cXyz vec1, vec2;
    s16 angle_y = _this->shape_angle.y;
    for (int i = 0; i < 0x10; i++, angle_y += 0x1000) {
        mDoMtx_YrotS(*calc_mtx, angle_y);
        vec1.x = 0.0f;
        vec1.y = 100.0f;
        vec1.z = i_dist;
        MtxPosition(&vec1, &vec2);
        vec2 += _this->current.pos;
        gnd_chk.SetPos(&vec2);
        if (_this->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk) > 300.0f) {
            vec1 = _this->current.pos;
            vec1.y += 100.0f;
            lin_chk.Set(&vec1, &vec2, _this);
            if (!dComIfG_Bgsp().LineCross(&lin_chk)) {
                return angle_y;
            }
        }
    }
    return _this->shape_angle.y;
}

static void* s_bikkuri_sub(void* i_proc, void* i_this) {
    if (fopAcM_IsActor(i_proc) && fopAcM_GetName(i_proc) == PROC_E_RDY) {
        if (i_proc != i_this && ((e_rdy_class*)i_proc)->mAction == ACT_BOW3 && ((e_rdy_class*)i_proc)->mMode <= 2) {
            cXyz delta = ((fopAc_ac_c*)i_proc)->current.pos - ((fopAc_ac_c*)i_this)->current.pos;
            if (delta.abs() < 350.0f + KREG_F(11)) {
                anm_init((e_rdy_class*)i_proc, ANM_KYORO2, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.3f);
                ((e_rdy_class*)i_proc)->mMode = 3;
                ((e_rdy_class*)i_proc)->mTimer[0] = cM_rndF(20.0f) + 40.0f;
            }
        }
    }
    return NULL;
}

static void e_rdy_drop(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    f32 target_speed = 0.0f;

    switch (i_this->mMode) {
    case 0:
        i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_DAMAGE, -1);
        anm_init(i_this, ANM_DAMAGE_S, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode = 1;
        break;

    case 1:
        i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_DAMAGE, -1);
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, ANM_FURA2, 0.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 2;
            i_this->mTargetAngleY = gake_check(i_this, 200.0f);
            fpcM_Search(s_bikkuri_sub, i_this);
            i_this->mTimer[0] = 60;
        }
        break;

    case 2:
        i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_DAMAGE, -1);
        target_speed = 4.0f;
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mTargetAngleY, 8, 0x400);
        if (i_this->mTimer[0] == 0) {
            i_this->mAction = ACT_A_DAMAGE;
            i_this->mMode = 0;
            return;
        }
        if (move_gake_check(i_this, 10.0f)) {
            i_this->mMode = 3;
            i_this->field_0xadc.y = a_this->shape_angle.y + 0x8000;
            i_this->field_0xadc.x = 0;
            i_this->mpMorf->setPlaySpeed(0.3f);
            i_this->field_0xbc8 = 0;
            i_this->mTimer[0] = 20;
        }
        break;

    case 3:
        target_speed = 3.0f;
        if (i_this->field_0xadc.x > -0x4000) {
            a_this->speed.y = 0.0f;
            i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_DAMAGE, -1);
        } else if (i_this->field_0xadc.x > -0x6800) {
            a_this->speed.y = -fabsf(cM_scos(i_this->field_0xadc.x) * 70.0f);
            i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_FALLING, -1);
        } else {
            i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_FALLING, -1);
        }
        cLib_addCalcAngleS2(&i_this->field_0xadc.x, -0x7000, 2, i_this->field_0xbc8);
        cLib_addCalcAngleS2(&i_this->field_0xbc8, 0x800, 1, 0x50);
        i_this->field_0xa7b = 2;
        if (i_this->mTimer[0] == 0 && a_this->current.pos.y <= i_this->mAcch.GetGroundH() + 100.0f)
        {
            a_this->current.pos.y = i_this->mAcch.GetGroundH() + 100.0f;
            i_this->field_0xadc.x = -0x4000;
            i_this->mAction = ACT_DAMAGE;
            i_this->mMode = 0;
            i_this->field_0xabc = 0.0f;
            a_this->health = 0;
        }
        break;
    }

    cLib_addCalc2(&a_this->speedF, target_speed, 1.0f, 3.0f);
}

static void e_rdy_a_damage(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    i_this->mIFrameTimer = 3;

    switch (i_this->mMode) {
    case 0:
        i_this->mMode = 1;
        anm_init(i_this, ANM_DEAD_ARROW, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mTimer[0] = 80;
        dKy_Sound_set(a_this->current.pos, 100, fopAcM_GetID(i_this), 5);
        break;

    case 1:
        if (i_this->mTimer[0] == 0) {
            rd_disappear(i_this);
        }
        break;
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 3.0f);
}

static void e_rdy_stand(e_rdy_class* i_this) {
    switch (i_this->mMode) {
    case 0:
        if (i_this->mTimer[0] == 0) {
            anm_init(i_this, ANM_KYORO2, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 1;
            i_this->mTimer[0] = cM_rndF(40.0f) + 60.0f;
        }
        break;

    case 1:
        if (i_this->mTimer[0] == 0) {
            anm_init(i_this, ANM_WAIT01, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimer[0] = cM_rndF(100.0f) + 100.0f;
            i_this->mMode = 0;
        }
        break;
    }

    f32 dist = i_this->mPlayerCheckDist;
    s16 angle = 0x4000;
    BOOL bVar1 = FALSE;
    if (i_this->field_0xa71 == 1 || S_find != 0) {
        if (i_this->field_0xa71 == 1) {
            bVar1 = TRUE;
        }
        dist = 10000.0f;
        angle = 0x7fff;
    }
    if (((i_this->mCounter & 7) == 0 || bVar1) && pl_check(i_this, dist, angle)
        && i_this->mWeaponType == WEAPON_CLUB)
    {
        i_this->mAction = ACT_FIGHT_RUN;
        i_this->mMode = -10;
        i_this->mTimer[0] = 60;
    }
}

static s8 e_rdy_bow3(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    if (i_this->mIFrameTimer == 0 && i_this->mMode != 5
        && fpcM_Search(s_command3_sub, i_this) == NULL)
    {
        i_this->mMode = 5;
        i_this->mTimer[1] = cM_rndF(20.0f) + 40.0f;
        i_this->mTimer[0] = cM_rndF(20.0f) + 80.0f;
    }

    int frame = (int)i_this->mpMorf->getFrame();
    s8 turn_to_player = true;

    switch (i_this->mMode) {
    case 0:
        turn_to_player = false;
        anm_init(i_this, ANM_WAIT01, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode = 1;
        break;

    case 1:
        turn_to_player = false;
        break;

    case 3:
        if (i_this->mTimer[0] == 0) {
            i_this->mMode = 0;
        }
        break;

    case 5:
        if (i_this->mTimer[1] == 1) {
            anm_init(i_this, ANM_KYORO2, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.3f);
        }
        if (i_this->mTimer[0] == 10) {
            anm_init(i_this, ANM_WAIT01, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
        if (i_this->mTimer[0] == 0) {
            i_this->mAction = ACT_BOW2;
            i_this->mMode = 0;
        }
        break;

    case 10:
        if (i_this->mTimer[0] == 0) {
            anm_init(i_this, ANM_SHOOT_READY, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes(i_this->mpArcName, 8);
            i_this->mpBowMorf->setAnm(bck, J3DFrameCtrl::EMode_NONE, 5.0f, 0.9f, 0.0f, -1.0f);
            i_this->mMode = 11;
            a_this->speedF = 0.0f;
        }
        break;

    case 11:
        i_this->field_0xa98 = 4;
        if (frame >= 10) {
            i_this->mHasArrow = true;
        }
        if (frame == 20) {
            i_this->mSound.startCreatureSound(Z2SE_OBJ_ARROW_DRAW_NORMAL, 0, -1);
        }
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, ANM_SHOOT_WAIT, 4.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimer[0] = cM_rndF(10.0f) + 10.0f;
            i_this->mMode = 12;
        }
        break;

    case 12:
        i_this->field_0xa98 = 4;
        if (i_this->mTimer[0] == 0 && !i_this->field_0xa74 && !dMsgObject_isTalkNowCheck()) {
            anm_init(i_this, ANM_SHOOT, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes(i_this->mpArcName, 11);
            i_this->mpBowMorf->setAnm(bck, J3DFrameCtrl::EMode_NONE, 1.0f, 1.0f, 0.0f, -1.0f);
            i_this->mMode = 13;
        }
        i_this->mHasArrow = true;
        break;

    case 13:
        if (frame <= 3) {
            i_this->mHasArrow = true;
            i_this->field_0xa98 = 4;
        }
        if (frame == 2) {
            i_this->mArrowFired = true;
        }
        if (i_this->mpMorf->isStop()) {
            i_this->mMode = 0;
        }
        break;
    }

    if (turn_to_player) {
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngle, 4, 0x1000);
    }

    if (i_this->mPlayerDist < 500.0f) {
        i_this->mAction = ACT_BOW_RUN;
        i_this->mMode = 0;
        i_this->field_0x1365 = false;
    }

    return false;
}

static void* s_command2_sub(void* i_proc, void* i_this) {
    if (fopAc_IsActor(i_proc) && fopAcM_GetName(i_proc) == PROC_E_RDY) {
        e_rdy_class* other = (e_rdy_class*)i_proc;
        if (other->mAction == ACT_BOW3 && other->mMode < 10) {
            other->mTimer[0] = cM_rndF(10.0f) + 10.0f;
            other->mMode = 10;
        }
    }
    return NULL;
}

static void* s_command4_sub(void* i_proc, void* i_this) {
    if (fopAc_IsActor(i_proc) && fopAcM_GetName(i_proc) == PROC_E_RDY) {
        e_rdy_class* other = (e_rdy_class*)i_proc;
        if (other != i_this && other->mAction == ACT_BOW3) {
            return other;
        }
    }
    return NULL;
}

static void e_rdy_commander(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    s8 bVar1 = false;
    s8 turn_to_player = false;
    boss = i_this;

    if (i_this->mTimer[2] == 0 && fpcM_Search(s_command4_sub, i_this) == NULL) {
        i_this->mAction = ACT_BOW2;
        i_this->mMode = 0;
        return;
    }

    switch (i_this->mMode) {
    case 0:
        i_this->mMode = 1;
        break;

    case 1:
        bVar1 = true;
        if (i_this->mTimer[0] == 0) {
            anm_init(i_this, ANM_KYORO2, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 2;
            i_this->mTimer[0] = cM_rndF(40.0f) + 60.0f;
        }
        break;

    case 2:
        bVar1 = true;
        if (i_this->mTimer[0] == 0) {
            anm_init(i_this, ANM_WAIT01, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimer[0] = cM_rndF(100.0f) + 100.0f;
            i_this->mMode = 1;
        }
        break;

    case 5:
        anm_init(i_this, ANM_WAIT01, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mTimer[0] = 30;
        i_this->mMode = 6;
        break;

    case 6:
        turn_to_player = true;
        if (i_this->mTimer[0] == 0) {
            anm_init(i_this, ANM_GOSIGN, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            mDoAud_seStart(Z2SE_EN_RD_V_CALLING, NULL, 0, 0);
            i_this->mMode = 7;
            if (dComIfGp_event_runCheck()) {
                i_this->mTimer[0] = l_HIO.field_0x24 + 2;
            } else {
                i_this->mTimer[0] = 10;
            }
        }
        break;

    case 7:
        turn_to_player = true;
        if (i_this->mTimer[0] == 1) {
            fpcM_Search(s_command2_sub, i_this);
        }
        if (i_this->mTimer[0] == 0 && i_this->mpMorf->isStop()) {
            anm_init(i_this, ANM_WAIT01, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            if (!dComIfGp_event_runCheck() && !pl_check(i_this, i_this->mPlayerCheckDist, 0x4000)) {
                i_this->mMode = 10;
                i_this->mTimer[1] = 350;
            } else {
                if (dComIfGp_event_runCheck()) {
                    i_this->mTimer[0] = 80;
                } else {
                    i_this->mTimer[0] = 0;
                }
                i_this->mMode = 6;
            }
        }
        break;

    case 10:
        anm_init(i_this, ANM_GOSIGN, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        mDoAud_seStart(Z2SE_EN_RD_V_CALLING, NULL, 0, 0);
        fpcM_Search(s_command2_sub, i_this);
        i_this->mMode = 11;
        break;

    case 11:
        if (i_this->mpMorf->isStop()) {
            if (i_this->mTimer[1] == 0) {
                anm_init(i_this, ANM_WAIT01, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mTimer[0] = cM_rndF(30.0f) + 30.0f;
                i_this->mMode = 1;
            } else {
                i_this->mMode = 10;
            }
        }
        break;
    }

    if (bVar1 && pl_check(i_this, i_this->mPlayerCheckDist, 0x4000)) {
        i_this->mMode = 5;
        if (i_this->mSwBit2 != 0) {
            dComIfGs_onSwitch(i_this->mSwBit2, fopAcM_GetRoomNo(a_this));
        }
    }

    if (turn_to_player) {
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngle, 2, 0x800);
    }
}

static void e_rdy_excite(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz vec;
    f32 target_speed = 0.0f;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_WAIT01, 4.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode = 1;
        i_this->mTimer[0] = cM_rndF(20.0f) + 20.0f;
        i_this->mTargetAngleY = a_this->current.angle.y;
        i_this->field_0x5bc = S_find_pos;
        break;

    case 1:
        i_this->field_0xa98 = 11;
        i_this->mArrowTarget = S_find_pos;
        if (i_this->mTimer[0] == 0) {
            if (i_this->field_0x1365) {
                i_this->mMode = 3;
                anm_init(i_this, ANM_KYORO2, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.3f);
                i_this->mTimer[0] = cM_rndF(20.0f) + 40.0f;
            } else {
                i_this->mMode = 2;
                anm_init(i_this, ANM_RUN, 2.0f, J3DFrameCtrl::EMode_LOOP,
                         cM_rndFX(0.05f) + 1.0f);
                i_this->mTimer[0] = cM_rndF(15.0f) + 30.0f;
            }
            vec = i_this->field_0x5bc - a_this->current.pos;
            i_this->mTargetAngleY = cM_atan2s(vec.x, vec.z);
        }
        break;

    case 2:
        i_this->field_0xa98 = 11;
        i_this->mArrowTarget = i_this->field_0x5bc;
        target_speed = l_HIO.mRunSpeed;
        vec = i_this->field_0x5bc - a_this->current.pos;
        if (i_this->mTimer[0] == 0 || move_gake_check(i_this, 100.0f) || vec.abs() < 100.0f) {
            i_this->mMode = 3;
            anm_init(i_this, ANM_KYORO2, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.3f);
            i_this->mTimer[0] = cM_rndF(20.0f) + 40.0f;
        }
        break;

    case 3:
        if (i_this->mTimer[0] == 0) {
            i_this->mAction = i_this->mPrevAction;
            if (i_this->mAction == ACT_STAND) {
                i_this->mAction = ACT_NORMAL;
            }
            i_this->mMode = 0;
        }
        break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mTargetAngleY, 2, 0x800);
    s16 angle_diff = a_this->current.angle.y - i_this->mTargetAngleY;
    if (angle_diff > 0x400 || angle_diff < -0x400) {
        target_speed = 0.0f;
    }
    cLib_addCalc2(&a_this->speedF, target_speed, 1.0f, 5.0f);
}

static void e_rdy_water(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    i_this->mIFrameTimer = 10;

    switch (i_this->mMode) {
    case 0: {
        a_this->speed.y = 0.0f;
        if (!i_this->mIsUpsideDown) {
            anm_init(i_this, ANM_DROWNEDA, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        } else {
            anm_init(i_this, ANM_DROWNEDB, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
        i_this->mTimer[0] = 60;
        i_this->mMode = 1;
        a_this->speedF = 0.0f;
        i_this->field_0xabc = 0.0f;
        i_this->mKnockback = 0.0f;
        cXyz pos = a_this->current.pos;
        pos.y = i_this->mWaterHeight;
        fopKyM_createWpillar(&pos, 1.5f, 0);
        break;
    }

    case 1:
        a_this->speed.y = 0.0f;
        cLib_addCalc2(&a_this->current.pos.y, i_this->mWaterHeight, 0.5f, 5.0f);
        cLib_addCalcAngleS2(&i_this->field_0xadc.x, -0x4000, 4, 0x400);
        if (i_this->mTimer[0] == 0) {
            rd_disappear(i_this);
        }
    }
}

static void e_rdy_jyunkai(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz vec;
    f32 target_speed = 0.0f;
    s16 check_angle = 0x4000;

    switch (i_this->mMode) {
    case 0: {
        anm_init(i_this, ANM_WALK, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode = 1;
        dPnt* point = &i_this->mpPath->m_points[i_this->mPathIndex];
        i_this->field_0x5bc = point->m_position;
        // fallthrough
    }

    case 1:
        target_speed = l_HIO.mWalkSpeed;
        vec = i_this->field_0x5bc - a_this->current.pos;
        i_this->mTargetAngleY = cM_atan2s(vec.x, vec.z);
        if (vec.abs() < 50.0f) {
            i_this->mPathIndex += i_this->mPathDirection;
            if (i_this->mPathIndex >= (i_this->mpPath->m_num & 0xff)) {
                if (dPath_ChkClose(i_this->mpPath)) {
                    i_this->mPathIndex = 0;
                } else {
                    i_this->mPathDirection = -1;
                    i_this->mPathIndex = i_this->mpPath->m_num - 2;
                }
            } else if (i_this->mPathIndex < 0) {
                i_this->mPathDirection = 1;
                i_this->mPathIndex = 1;
            }
            i_this->mMode = 2;
            i_this->mTimer[0] = cM_rndF(100.0f) + 100.0f;
            anm_init(i_this, ANM_KYORO2, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
        break;

    case 2:
        check_angle = 0x6000;
        if (i_this->mTimer[0] == 0) {
            i_this->mMode = 0;
        }
        break;

    case 10:
        anm_init(i_this, ANM_SHOUT, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode = 11;
        dKy_Sound_set(a_this->current.pos, 100, fopAcM_GetID(i_this), 3);
        S_find = 20;
        break;

    case 11:
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngle, 2, 0x800);
        if (i_this->mpMorf->isStop()) {
            i_this->mAction = ACT_FIGHT_RUN;
            i_this->mMode = -10;
            i_this->mTimer[0] = 60;
            return;
        }
        break;
    }

    if (target_speed) {
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mTargetAngleY, 8, 0x400);
        s16 angle_diff = a_this->current.angle.y - i_this->mTargetAngleY;
        if (angle_diff > 0x400 || angle_diff < -0x400) {
            target_speed = 0.0f;
        }
    }
    cLib_addCalc2(&a_this->speedF, target_speed, 1.0f, 3.0f);

    f32 check_dist = i_this->mPlayerCheckDist;
    BOOL bVar1 = FALSE;
    if (i_this->field_0xa71 == 1 || S_find != 0) {
        if (i_this->field_0xa71 == 1) {
            bVar1 = TRUE;
        }
        check_dist = 10000.0f;
        check_angle = 0x7fff;
    }

    if (i_this->mMode < 10) {
        if (bVar1) {
            i_this->mMode = 2;
            i_this->mTimer[0] = cM_rndF(100.0f) + 100.0f;
            anm_init(i_this, ANM_KYORO2, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        } else if ((i_this->mCounter & 7) == 0 && pl_check(i_this, check_dist, check_angle)) {
            if (S_find == 0) {
                i_this->mMode = 10;
            } else {
                i_this->mAction = 3;
                i_this->mMode = -10;
                i_this->mTimer[0] = 60;
            }
        }
    }
}

static void wolfkick_damage(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    i_this->mAction = ACT_DAMAGE;
    i_this->mMode = 0;
    i_this->field_0xadc.y = player->shape_angle.y + 0x8000;
    a_this->speed.y = l_HIO.field_0x30;
    i_this->field_0xabc = -l_HIO.field_0x34;
    i_this->mSound.startCollisionSE(Z2SE_HIT_WOOD_WEAPON, 0x1f);
    i_this->field_0xac8 = true;
}

static void big_damage(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    i_this->mAction = ACT_DAMAGE;
    i_this->mMode = 0;
    a_this->speed.y = l_HIO.field_0x4c + 2.0f;

    if (i_this->mRideState == 2) {
        fopAc_ac_c* kargarok = fopAcM_SearchByID(i_this->mKargarokID);
        if (kargarok != NULL && kargarok->speedF >= 20.0f) {
            i_this->field_0xabc = kargarok->speedF;
            if (i_this->field_0xabc > 40.0f) {
                i_this->field_0xabc = 40.0f;
            }
        } else {
            i_this->field_0xabc = -l_HIO.field_0x48;
        }
        i_this->field_0xadc.y = a_this->shape_angle.y + (s16)cM_rndFX(3000.0f);
    } else {
        i_this->field_0xadc.y = i_this->mAtInfo.mHitDirection.y;
        i_this->field_0xabc = -l_HIO.field_0x44;
    }

    if (i_this->mAtInfo.mHitBit & 0x80) {
        i_this->field_0xac6 = cM_rndFX(2000.0f) + 4000.0f;
    } else if (i_this->mAtInfo.mHitBit & 0x800) {
        i_this->field_0xac6 = -(cM_rndFX(2000.0f) + 4000.0f);
    } else if (i_this->mRideState != 0) {
        i_this->field_0xac6 = cM_rndFX(3000.0f);
    } else if (fopAcM_GetName(i_this->mAtInfo.mpActor) == PROC_E_WB) {
        i_this->field_0xac6 = cM_rndFX(8000.0f);
        a_this->speed.y = cM_rndF(15.0f) + 35.0f;
        i_this->field_0xabc = -(i_this->mAtInfo.mpActor->speedF * 0.5f);
    } else {
        i_this->field_0xac6 = cM_rndFX(1000.0f);
    }

    i_this->field_0xac8 = false;
}

static void small_damage(e_rdy_class* i_this, int i_collider) {
    fopAc_ac_c* a_this = &i_this->actor;
    if (i_this->mAction != ACT_S_DAMAGE) {
        i_this->mPrevAction = i_this->mAction;
    }
    i_this->mAction = ACT_S_DAMAGE;
    i_this->mMode = 0;
    i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_DAMAGE, -1);

    if (i_collider == 0) {
        s16 angle_diff = a_this->shape_angle.y - i_this->mPlayerAngle;
        if (angle_diff < -0x4000 || angle_diff > 0x4000) {
            anm_init(i_this, ANM_DAMAGE_W, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        } else if (angle_diff < 0) {
            anm_init(i_this, ANM_DAMAGE_L, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        } else {
            anm_init(i_this, ANM_DAMAGE_R, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        }
    } else {
        anm_init(i_this, ANM_DAMAGE_W, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
    }

    i_this->mKnockback = 20.0f;
    i_this->mHitDirection = i_this->mAtInfo.mHitDirection.y;
}

static void damage_check(e_rdy_class* i_this) {
    fopAc_ac_c* _this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    if (i_this->field_0xa8f != 0) {
        i_this->field_0xa8f = 0;
        big_damage(i_this);
        i_this->mIFrameTimer = 1000;
        _this->current.angle.x = 0;
        _this->current.angle.z = 0;
        _this->shape_angle.x = _this->current.angle.x;
        _this->shape_angle.z = _this->current.angle.z;
    } else {
        i_this->mCcStts.Move();
        if (i_this->mIFrameTimer == 0) {
            if (l_HIO.field_0x39) {
                _this->health = 100;
            }

            for (int i = 0; i <= 2; i++) {
                if (i_this->mCcSph[i].ChkTgHit()) {
                    i_this->mIFrameTimer = 6;
                    i_this->mParticleTimer = 3;
                    i_this->mAtInfo.mpCollider = i_this->mCcSph[i].GetTgHitObj();

                    if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_10000000)) {
                        wolfkick_damage(i_this);
                        i_this->mIFrameTimer = 1000;
                        break;

                    } else {
                        at_power_check(&i_this->mAtInfo);
                        if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
                            i_this->mTimer[1] = 30;
                        } else {
                            i_this->mTimer[1] = 1;
                        }

                        if (i_this->mAtInfo.mHitType != HIT_TYPE_LINK_NORMAL_ATTACK
                            && i_this->mAtInfo.mHitType != HIT_TYPE_STUN
                            && i_this->mAtInfo.mHitType != HIT_TYPE_BOMB
                            && i_this->field_0x1365)
                        {
                            i_this->mIFrameTimer = 1000;
                            i_this->mAction = ACT_DROP;
                            i_this->mMode = 0;
                            _this->health = 0;
                            cc_at_check(_this, &i_this->mAtInfo);
                            return;
                        }

                        if (i_this->mRideState == 0 && i_this->mAtInfo.mHitType == HIT_TYPE_ARROW)
                        {
                            i_this->mAction = ACT_A_DAMAGE;
                            i_this->mMode = 0;
                            i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_DAMAGE, -1);
                            _this->health = 0;
                            cc_at_check(_this, &i_this->mAtInfo);
                            return;
                        }

                        cc_at_check(_this, &i_this->mAtInfo);

                        if (daPy_getPlayerActorClass()->getCutType()
                                                        == daPy_py_c::CUT_TYPE_HEAD_JUMP) {
                            _this->health = 0;
                        }

                        if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                            i_this->mIFrameTimer = 20;
                        } else {
                            i_this->mIFrameTimer = 10;
                        }
                        if (i_this->mAtInfo.mAttackPower <= 1) {
                            i_this->mIFrameTimer = 10;
                        }

                        i_this->field_0xaf0 |= i_this->mAtInfo.mHitBit;

                        if (daPy_getPlayerActorClass()->checkHorseRide()
                            && dComIfGp_getHorseActor()->speedF >= 20.0f
                            && i_this->mAtInfo.mHitType == HIT_TYPE_LINK_NORMAL_ATTACK)
                        {
                            i_this->mAtInfo.mAttackPower = 20;
                        }

                        if (_this->health <= 0 || i_this->mAtInfo.mAttackPower >= 20
                            || i_this->mAtInfo.mHitStatus != 0 || i_this->mRideState != 0)
                        {
                            if (player->getCutType() == daPy_py_c::CUT_TYPE_JUMP
                                && player->checkCutJumpCancelTurn())
                            {
                                small_damage(i_this, i);
                                i_this->mIFrameTimer = 3;
                            } else {
                                big_damage(i_this);
                                i_this->mIFrameTimer = 1000;
                            }
                        } else {
                            int collider = i;
                            if (i_this->mAtInfo.mHitType == HIT_TYPE_STUN) {
                                collider = 0;
                            }
                            small_damage(i_this, collider);
                        }

                        _this->speedF = 0.0f;

                        if (i_this->mpBowMorf != NULL) {
                            J3DAnmTransform* bck =
                                (J3DAnmTransform*)dComIfG_getObjectRes(i_this->mpArcName, 11);
                            i_this->mpBowMorf->setAnm(bck, J3DFrameCtrl::EMode_NONE,
                                                      1.0f, 1.0f, 0.0f, -1.0f);
                            break;
                        } else {
                            break;
                        }
                    }
                }
            }

            for (int i = 0; i <= 2; i++) {
                if (_this->health <= 1) {
                    _this->health = 0;
                    i_this->mCcSph[i].SetTgHitMark(CcG_Tg_UNK_MARK_3);
                }
            }
        }
    }
}

static void action(e_rdy_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz vec1, vec2;

    i_this->field_0xa98 = 0;
    i_this->mPlayerDist = fopAcM_searchPlayerDistance(a_this);
    if (daPy_getPlayerActorClass()->checkHorseRide()) {
        i_this->mPlayerDist -= 100.0f + BREG_F(17);
    }
    i_this->mPlayerAngle = fopAcM_searchPlayerAngleY(a_this);

    if (i_this->mRideState == 0) {
        fopAcM_OnStatus(a_this, 0);
        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(a_this, 0);
        a_this->attention_info.flags = 0;
    }

    damage_check(i_this);

    s8 link_search = false;
    s8 water_check = true;
    s8 search_sound = false;
    a_this->offHeadLockFlg();

    switch (i_this->mAction) {
    case ACT_NORMAL:
        e_rdy_normal(i_this);
        search_sound = true;
        break;
    case ACT_FIGHT_RUN:
        e_rdy_fight_run(i_this);
        link_search = true;
        break;
    case ACT_FIGHT:
        e_rdy_fight(i_this);
        link_search = true;
        break;
    case ACT_BOW_RUN:
        e_rdy_bow_run(i_this);
        link_search = true;
        break;
    case ACT_BOW:
        e_rdy_bow(i_this);
        link_search = true;
        break;
    case ACT_BOW2:
        search_sound = e_rdy_bow2(i_this);
        link_search = true;
        break;
    case ACT_BOW_IKKI2:
        e_rdy_bow_ikki2(i_this);
        water_check = false;
        break;
    case ACT_AVOID:
        e_rdy_avoid(i_this);
        break;
    case ACT_BOMB_ACTION:
        e_rdy_bomb_action(i_this);
        break;
    case ACT_S_DAMAGE:
        e_rdy_s_damage(i_this);
        break;
    case ACT_DAMAGE:
        e_rdy_damage(i_this);
        break;
    case ACT_DROP:
        e_rdy_drop(i_this);
        break;
    case ACT_A_DAMAGE:
        e_rdy_a_damage(i_this);
        break;
    case ACT_STAND:
        e_rdy_stand(i_this);
        search_sound = true;
        break;
    case ACT_COMMANDER:
        e_rdy_commander(i_this);
        link_search = true;
        break;
    case ACT_BOW3:
        search_sound = e_rdy_bow3(i_this);
        link_search = true;
        break;
    case ACT_EXCITE:
        e_rdy_excite(i_this);
        break;
    case ACT_WATER:
        e_rdy_water(i_this);
        water_check = false;
        break;
    case ACT_TKUSA:
        e_rdy_tkusa(i_this);
        break;
    case ACT_YC_RIDE:
        e_rdy_yc_ride(i_this);
        break;
    case ACT_JYUNKAI:
        e_rdy_jyunkai(i_this);
        break;
    }

    if (search_sound) {
        SND_INFLUENCE* sound = dKy_Sound_get();
        if (sound->actor_id != fpcM_ERROR_PROCESS_ID_e && sound->actor_id != fopAcM_GetID(i_this)) {
            vec1 = sound->position - a_this->current.pos;
            if (vec1.abs() < sound->field_0xc * 120) {
                i_this->mPrevAction = i_this->mAction;
                i_this->mAction = ACT_EXCITE;
                i_this->mMode = 0;
                S_find_pos = sound->position;
            }
        }
    }

    if (water_check) {
        dBgS_ObjGndChk_Spl gnd_chk;
        vec1 = a_this->current.pos;
        vec1.y += 150.0f;
        gnd_chk.SetPos(&vec1);
        i_this->mWaterHeight = dComIfG_Bgsp().GroundCross(&gnd_chk);
        if (a_this->current.pos.y < i_this->mWaterHeight) {
            i_this->mAction = ACT_WATER;
            i_this->mMode = 0;
            a_this->current.pos.y = i_this->mWaterHeight;
        }
    }

    if (link_search) {
        i_this->mSound.setLinkSearch(true);
    } else {
        i_this->mSound.setLinkSearch(false);
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 2, 0x1000);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, a_this->current.angle.x, 2, 0x1000);
    cLib_addCalcAngleS2(&a_this->shape_angle.z, a_this->current.angle.z, 2, 0x1000);

    if (i_this->mRideState == 0) {
        a_this->attention_info.distances[fopAc_attn_BATTLE_e] = 3;
        if (i_this->field_0xabc) {
            mDoMtx_YrotS(*calc_mtx, i_this->field_0xadc.y);
            if (i_this->field_0xbc0 == 0) {
                vec1.z = i_this->field_0xabc;
            } else {
                vec1.z = 0.0f;
                a_this->speed.y = 0.0f;
            }
            a_this->gravity = -(l_HIO.field_0x40 - 1.0f);
        } else {
            mDoMtx_YrotS(*calc_mtx, a_this->current.angle.y);
            vec1.z = a_this->speedF;
            a_this->gravity = -5.0f;
        }
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        MtxPosition(&vec1, &vec2);
        a_this->speed.x = vec2.x;
        a_this->speed.z = vec2.z;
        a_this->current.pos += a_this->speed;
        a_this->speed.y += a_this->gravity;
        if (a_this->speed.y < -100.0f) {
            a_this->speed.y = -100.0f;
        }
        if (i_this->mCollisionEnabled) {
            cXyz* move_p = i_this->mCcStts.GetCCMoveP();
            if (move_p != NULL) {
                a_this->current.pos.x += move_p->x;
                a_this->current.pos.z += move_p->z;
            }
        }
        cLib_addCalc0(&i_this->mJumpHeight, 1.0f, 3.0f);
        if (i_this->mKnockback > 0.1f) {
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = -i_this->mKnockback;
            mDoMtx_YrotS(*calc_mtx, i_this->mHitDirection);
            MtxPosition(&vec1, &vec2);
            a_this->current.pos += vec2;
            cLib_addCalc0(&i_this->mKnockback, 1.0f, TREG_F(12) + 7.0f);
        }

    } else {
        fopAc_ac_c* a_karg = fopAcM_SearchByID(i_this->mKargarokID);
        if (a_karg == NULL) {
            i_this->mAction = ACT_NORMAL;
            i_this->mMode = 0;
            return;
        }

        e_yc_class* kargorok = (e_yc_class*) a_karg;
        a_this->speedF = a_karg->speedF;
        a_this->speed.set(0.0f, 0.0f, 0.0f);
        MTXCopy(kargorok->mpMorf->getModel()->getAnmMtx(9), *calc_mtx);
        vec1.set(0.0f, -60.0f + BREG_F(4), 15.0f + BREG_F(5));
        MtxPosition(&vec1, &i_this->mKargarokPos);
        if (i_this->mRideState == 2) {
            a_this->current.pos = i_this->mKargarokPos;
            a_this->shape_angle = a_this->current.angle = a_karg->shape_angle;
            kargorok->mRiderID = fopAcM_GetID(i_this);
        } else {
            vec1 = i_this->mKargarokPos - a_this->current.pos;
            cMtx_YrotS(*calc_mtx, cM_atan2s(vec1.x, vec1.z));
            cMtx_XrotM(*calc_mtx, -cM_atan2s(vec1.y, JMAFastSqrt(vec1.x * vec1.x + vec1.z * vec1.z)));
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = 20.0f + TREG_F(3);
            MtxPosition(&vec1, &vec2);
            a_this->current.pos += vec2;
            vec1 = i_this->mKargarokPos - a_this->current.pos;
            if (vec1.abs() < 30.0f || i_this->mTimer[3] == 0) {
                i_this->mRideState = 2;
            }
            cLib_addCalcAngleS2(&a_this->current.angle.y, a_karg->shape_angle.y, 4, 0x800);
        }
        i_this->mJumpHeight += i_this->mJumpSpeed;
        i_this->mJumpSpeed -= 4.5f + TREG_F(5);
        if (i_this->mJumpHeight <= 0.0f) {
            i_this->mJumpHeight = 0.0f;
            i_this->mJumpSpeed = 0.0f;
        }
    }

    s16 ang_diff_y = 0;
    s16 sVar10 = 0;
    s16 ang_adj = 16000;
    i_this->field_0xa74 = false;
    cXyz vec3;
    s16 sVar15 = 0x1000;

    if (i_this->field_0xa98 == -1 || i_this->field_0xa98 == -2) {
        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        vec1.x = 0.0f;
        if (i_this->field_0xa98 == -2) {
            vec1.y = 110.0f + KREG_F(5);
        } else {
            vec1.y = 70.0f + KREG_F(8);
        }
        vec1.z = 50.0f;
        MtxPosition(&vec1, &vec3);
        vec3 += a_this->current.pos;
        i_this->field_0xa98 = 2;
        sVar15 = 0x800 + KREG_S(7);
    } else if (i_this->field_0xa98 >= 11) {
        i_this->field_0xa98 -= 10;
        vec3 = i_this->mArrowTarget;
    } else {
        vec3 = player->eyePos;
    }

    if (i_this->mRideState == 2) {
        sVar15 = 0x1000;
    }

    if (i_this->field_0xa98 != 0) {
        vec1 = vec3 - a_this->current.pos;
        vec1.y += -150.0f + TREG_F(2);
        ang_diff_y = cM_atan2s(vec1.x, vec1.z) - a_this->shape_angle.y;
        sVar10 = a_this->shape_angle.x + cM_atan2s(vec1.y, JMAFastSqrt(vec1.x * vec1.x + vec1.z * vec1.z));
        if (i_this->field_0xa98 <= 2) {
            if (ang_diff_y > ang_adj) {
                ang_diff_y = ang_adj;
            } else if ((s16)ang_diff_y < (s16)-ang_adj) {
                ang_diff_y = -ang_adj;
            }
            if (sVar10 > ang_adj) {
                sVar10 = ang_adj;
            } else if ((s16)sVar10 < (s16)-ang_adj) {
                sVar10 = -ang_adj;
            }
            cLib_addCalcAngleS2(&i_this->field_0xa9a, ang_diff_y, 2, sVar15);
        } else if (i_this->field_0xa98 == 5) {
            if ((i_this->mCounter & 0xf) == 0 && cM_rndF2(1.0f) < 0.3f) {
                i_this->field_0xaa0 = cM_rndFX2(2000.0f);
            }
        } else {
            ang_adj = 30000 + XREG_S(5);
            s16 other_adj = 25000 + XREG_S(6);
            if (ang_diff_y > ang_adj) {
                ang_diff_y = ang_adj;
                i_this->field_0xa74 = true;
            } else if (ang_diff_y < (s16)-other_adj) {
                ang_diff_y = -other_adj;
                i_this->field_0xa74 = true;
            }
            cLib_addCalcAngleS2(&i_this->field_0xa9a, ang_diff_y / 2, 2, 0x1000);
            sVar10 = -cM_atan2s(vec1.y, JMAFastSqrt(vec1.x * vec1.x + vec1.z * vec1.z));
            if (sVar10 > 11000) {
                sVar10 = 11000;
                i_this->field_0xa74 = true;
            } else if (sVar10 < -8000) {
                sVar10 = -8000;
                i_this->field_0xa74 = true;
            }
        }
    } else {
        cLib_addCalcAngleS2(&i_this->field_0xa9a, 0, 2, 0x1000);
        i_this->field_0xaa0 = 0;
    }

    cLib_addCalcAngleS2(&i_this->field_0xa9c, sVar10, 2, sVar15);
    cLib_addCalcAngleS2(&i_this->field_0xa9e, i_this->field_0xaa0, 4, 0x500);

    if (fabsf(i_this->field_0xafc) > 1.0f) {
        i_this->field_0xb44[1].x = i_this->field_0xafc * cM_scos(i_this->field_0xb00) * (70.0f + BREG_F(0));
        i_this->field_0xb44[2].x += i_this->field_0xb44[1].x;
        cLib_addCalc0(&i_this->field_0xafc, 1.0f, 3.0f + BREG_F(1));
        i_this->field_0xb00 += (s16)(4000 + BREG_S(0));
    }

    if (fabsf(i_this->field_0xaf4) > 1.0f) {
        i_this->field_0xb44[0].x = i_this->field_0xaf4 * cM_scos(i_this->field_0xaf8) * (70.0f + BREG_F(2));
        cLib_addCalc0(&i_this->field_0xaf4, 1.0f, 3.0f + BREG_F(3));
        i_this->field_0xaf8 += (s16)(4000 + BREG_S(1));
    }

    if (a_this->speed.y < 0.0f && i_this->mAcch.ChkGroundHit()) {
        cLib_addCalc0(&i_this->field_0xafc, 1.0f, 30.0f);
        cLib_addCalc0(&i_this->field_0xaf4, 1.0f, 30.0f);
    }

    if (i_this->field_0xabc) {
        ang_diff_y = i_this->field_0xadc.y - a_this->shape_angle.y;
        i_this->field_0xb44[3].y = -ang_diff_y + 0x4000;
        if (i_this->field_0xb44[3].y > 6000) {
            i_this->field_0xb44[3].y = 6000;
        } else if (i_this->field_0xb44[3].y < -12000) {
            i_this->field_0xb44[3].y = -12000;
        }
        i_this->field_0xb44[4].y = -ang_diff_y + 0x4000;
        if (i_this->field_0xb44[4].y > 15000) {
            i_this->field_0xb44[4].y = 15000;
        } else if (i_this->field_0xb44[4].y < 0) {
            i_this->field_0xb44[4].y = 0;
        }
        i_this->field_0xb44[5].y = ang_diff_y + 0x4000;
        if (i_this->field_0xb44[5].y > 6000) {
            i_this->field_0xb44[5].y = 6000;
        } else if (i_this->field_0xb44[5].y < -12000) {
            i_this->field_0xb44[5].y = -12000;
        }
        i_this->field_0xb44[6].y = ang_diff_y + 0x4000;
        if (i_this->field_0xb44[6].y > 15000) {
            i_this->field_0xb44[6].y = 15000;
        } else if (i_this->field_0xb44[6].y < 0) {
            i_this->field_0xb44[6].y = 0;
        }

        cMtx_YrotS(*calc_mtx, ang_diff_y);
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = 12000.0f;
        MtxPosition(&vec1, &vec2);
        i_this->field_0xbb4.y = -vec2.x;
        i_this->field_0xbb4.x = -vec2.z * 1.3f;
        if (i_this->field_0xbb4.x > 10000) {
            i_this->field_0xbb4.x = 10000;
        }
    }

    static s16 j_spd[11] = {
        0x4000, 0x4000, 0x4000, 0x1000, 0x1000, 0x1000, 0x1000, 0x4000, 0x4000, 0x4000, 0x4000,
    };
    for (int i = 0; i < 11; i++) {
        cLib_addCalcAngleS2(&i_this->field_0xb02[i].x, i_this->field_0xb44[i].x, 2, j_spd[i]);
        cLib_addCalcAngleS2(&i_this->field_0xb02[i].y, i_this->field_0xb44[i].y, 2, j_spd[i]);
        cLib_addCalcAngleS2(&i_this->field_0xb02[i].z, i_this->field_0xb44[i].z, 2, j_spd[i]);
        i_this->field_0xb44[i].x = i_this->field_0xb44[i].z = 0;
        if (i >= 3 && i <= 6) {
            if (i_this->field_0xba2[i - 3] == 0) {
                s16 spd;
                if (i == 4 || i == 6) {
                    spd = 0x600;
                } else {
                    spd = 0x800;
                }
                cLib_addCalcAngleS2(&i_this->field_0xb44[i].y, 0, 1, spd);
            } else {
                i_this->field_0xba2[i - 3]--;
            }
        } else {
            i_this->field_0xb44[i].y = 0;
        }
        if (i >= 3) {
            i_this->field_0xb8c[i] =
                i_this->field_0xb88 * cM_scos(i_this->mCounter * (BREG_S(0) + 4000) + i * (BREG_S(1) + 20000));
        }
    }
    cLib_addCalc0(&i_this->field_0xb88, 1.0f, 100.0f + BREG_F(11));

    cLib_addCalcAngleS2(&i_this->field_0xbae.y, i_this->field_0xbb4.y, 2, 0x800);
    cLib_addCalcAngleS2(&i_this->field_0xbae.x, i_this->field_0xbb4.x, 2, 0x800);
    i_this->field_0xbb4.y = i_this->field_0xbb4.x = 0;

    s16 sVar11 = 0;
    s16 sVar12 = 0;
    if (i_this->field_0xaee != 0) {
        i_this->field_0xaee--;
        if (i_this->field_0xadc.x != 0) {
            Vec gnd_pos2;
            cXyz gnd_pos;
            cXyz unused_vec;
            dBgS_GndChk gnd_chk;
            f32 z_adj = 75.0f;
            MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(JNT_HIP1), *calc_mtx);
            vec1.set(0.0f, 0.0f, 0.0f);
            MtxPosition(&vec1, &gnd_pos);
            gnd_pos.y += 100.0f;
            gnd_pos.y += 100.0f;
            gnd_chk.SetPos(&gnd_pos);
            gnd_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
            f32 y_diff;
            f32 ox_diff;

            if (gnd_pos.y != -1.0e9f) {
                gnd_pos2.x = gnd_pos.x;
                gnd_pos2.y = gnd_pos.y + 100.0f;
                gnd_pos2.z = gnd_pos.z + z_adj;
                gnd_chk.SetPos(&gnd_pos2);
                gnd_pos2.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
                if (gnd_pos2.y != -1.0e9f) {
                    y_diff = gnd_pos2.y - gnd_pos.y;
                    f32 z_diff = gnd_pos2.z - gnd_pos.z;
                    sVar12 = -cM_atan2s(y_diff, z_diff);
                    if (sVar12 > 0x3000 || sVar12 < -0x3000) {
                        sVar12 = 0;
                    }
                }

                gnd_pos2.x = gnd_pos.x + z_adj;
                gnd_pos2.y = gnd_pos.y + 100.0f;
                gnd_pos2.z = gnd_pos.z;
                gnd_chk.SetPos(&gnd_pos2);
                gnd_pos2.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
                if (gnd_pos2.y != -1.0e9f) {
                    y_diff = gnd_pos2.y - gnd_pos.y;
                    ox_diff = gnd_pos2.x - gnd_pos.x;
                    sVar11 = (s16)cM_atan2s(y_diff, ox_diff);
                    if (sVar11 > 0x3000 || sVar11 < -0x3000) {
                        sVar11 = 0;
                    }
                }
            }

            dBgS_LinChk lin_chk;
            cXyz vec4, lin_start, lin_end;
            lin_start = a_this->current.pos;
            lin_start.y += 30.0f + JREG_F(5);
            J3DModel* model = i_this->mpMorf->getModel();
            vec4.set(0.0f, 0.0f, 0.0f);
            MTXCopy(model->getAnmMtx(JNT_HEAD + BREG_S(8)), *calc_mtx);
            MtxPosition(&vec4, &lin_end);
            lin_end.y += 30.0f + JREG_F(6);
            lin_chk.Set(&lin_start, &lin_end, a_this);
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                vec4 = lin_start - lin_end;
                cMtx_YrotS(*calc_mtx, cM_atan2s(vec4.x, vec4.z));
                vec4.x = 0.0f;
                vec4.y = 0.0f;
                vec4.z = 50.0f + TREG_F(11);
                MtxPosition(&vec4, &vec2);
                a_this->current.pos += vec2;
            }

            i_this->field_0xae8.x = sVar12;
            i_this->field_0xae8.z = sVar11;
        }
    }
    cLib_addCalcAngleS2(&i_this->field_0xae2.x, i_this->field_0xae8.x, 1, 0x400);
    cLib_addCalcAngleS2(&i_this->field_0xae2.z, i_this->field_0xae8.z, 1, 0x400);

    if (i_this->mParticleTimer != 0) {
        i_this->mParticleTimer--;
        if (i_this->mParticleTimer == 0) {
            dComIfGp_particle_set(0x15e, &a_this->eyePos, &a_this->shape_angle, NULL);
        }
    }

    if (i_this->mGroundHit) {
        J3DModel* model = i_this->mpMorf->getModel();
        MTXCopy(model->getAnmMtx(JNT_MUNE2), *calc_mtx);
        vec1.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&vec1, &vec2);
        dBgS_ObjGndChk_Spl wtr_chk;
        vec1 = vec2;
        vec1.y += 100.0f;
        wtr_chk.SetPos(&vec1);
        if (fabsf(dComIfG_Bgsp().GroundCross(&wtr_chk) - vec2.y) < 50.0f) {
            f32 tmp_val = 1.0f;
            cXyz scl(tmp_val, tmp_val, tmp_val);
            csXyz rot(0, 0, 0);
            static u16 w_eff_name[3] = {0x8258, 0x8259, 0x825a};
            for (int i = 0; i < 3; i++) {
                i_this->mWaterParticleKey[i] =
                    dComIfGp_particle_set(i_this->mWaterParticleKey[i], w_eff_name[i], &vec2,
                                          &a_this->tevStr, &rot, &scl, 0xff,
                                          NULL, -1, NULL, NULL, NULL);
            }
        } else {
            fopAcM_effSmokeSet1(&i_this->mSmokeKey1, &i_this->mSmokeKey2, &vec2,
                                NULL, 1.3f, &a_this->tevStr, 1);
        }
        i_this->mGroundHit = false;
    }

    cXyz bind_scl(1.3f, 1.3f, 1.3f);
    setMidnaBindEffect((fopEn_enemy_c *)a_this, &i_this->mSound, &a_this->eyePos, &bind_scl);
}

static void fire_eff_set(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    int eff_num;
    u16 eff[2];
    cXyz vec1, vec2;

    if (i_this->mWeaponType == WEAPON_BOW_BOMB) {
        eff_num = 2;
        eff[0] = 0x86a5;
        eff[1] = 0x86a6;
        i_this->mSound.startCreatureSoundLevel(Z2SE_OBJ_BOMB_IGNITION, 0, -1);
    } else {
        eff_num = 1;
        eff[0] = 0x8113;
        i_this->mSound.startCreatureSoundLevel(Z2SE_OBJ_ARROW_FIRE_READY, 0, -1);
    }

    vec1.x = 90.0f;
    vec1.y = 10.0f;
    vec1.z = -60.0f;
    MtxPosition(&vec1, &vec2);
    i_this->field_0x1358 = vec2 - i_this->field_0x134c;
    i_this->field_0x1358 *= 0.9f;
    i_this->field_0x134c = vec2;

    for (int i = 0; i < eff_num; i++) {
        i_this->mFireParticleKey[i] = dComIfGp_particle_set(i_this->mFireParticleKey[i], eff[i],
                                                            &vec2, &a_this->shape_angle, NULL);
        if (i == 0) {
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mFireParticleKey[i]);
            if (emitter != NULL) {
                emitter->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
                emitter->setUserWork((uintptr_t)&i_this->field_0x1358);
            }
        }
    }
}

static void cam_3d_morf(e_rdy_class* i_this, f32 i_scale) {
    cLib_addCalc2(&i_this->mCamCenter.x, i_this->mCamCenterTarget.x, i_scale,
                  i_this->mCamCenterSpeed.x * i_this->mCamSpeed);
    cLib_addCalc2(&i_this->mCamCenter.y, i_this->mCamCenterTarget.y, i_scale,
                  i_this->mCamCenterSpeed.y * i_this->mCamSpeed);
    cLib_addCalc2(&i_this->mCamCenter.z, i_this->mCamCenterTarget.z, i_scale,
                  i_this->mCamCenterSpeed.z * i_this->mCamSpeed);
    cLib_addCalc2(&i_this->mCamEye.x, i_this->mCamEyeTarget.x, i_scale,
                  i_this->mCamEyeSpeed.x * i_this->mCamSpeed);
    cLib_addCalc2(&i_this->mCamEye.y, i_this->mCamEyeTarget.y, i_scale,
                  i_this->mCamEyeSpeed.y * i_this->mCamSpeed);
    cLib_addCalc2(&i_this->mCamEye.z, i_this->mCamEyeTarget.z, i_scale,
                  i_this->mCamEyeSpeed.z * i_this->mCamSpeed);
}

static void* s_adel_sub(void* i_proc, void* i_this) {
    if (fopAc_IsActor(i_proc) && fopAcM_GetName(i_proc) == PROC_E_ARROW) {
        fopAcM_delete((fopAc_ac_c*)i_proc);
    }
    return NULL;
}

static void demo_camera(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    camera_class* player_camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera = dComIfGp_getCamera(0);
    cXyz vec1, vec2, vec3, vec4, vec5;
    u8 unused_u8 = 1;
    (void) unused_u8;
    s8 cVar13 = 0;
    s8 cVar12 = 0;
    fopAc_ac_c* a_karg = fopAcM_SearchByID(i_this->mKargarokID);
    e_yc_class* kargarok = (e_yc_class*) a_karg;

    switch (i_this->mDemoMode) {
    case 1: {
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffef, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        player_camera->mCamera.Stop();
        i_this->mDemoMode = 2;
        i_this->mDemoTimer = 0;
        i_this->mCamFovy = 55.0f;
        player_camera->mCamera.SetTrimSize(3);
        daPy_getPlayerActorClass()->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(0x38, 0, 0, 0);
        i_this->mCamEye = camera->lookat.eye;
        i_this->mCamCenter = camera->lookat.center;
        s16 sang_y_diff = a_karg->shape_angle.y - player->shape_angle.y;
        if (sang_y_diff > 0) {
            i_this->field_0x13d4 = -0x4000;
        } else {
            i_this->field_0x13d4 = 0x4000;
        }
        player->speedF = 0.0f;
        // fallthrough
    }
    case 2:
    case 3:
    case 4:
        cLib_addCalc2(&i_this->mCamCenter.x, player->current.pos.x, 0.2f, 100.0f);
        cLib_addCalc2(&i_this->mCamCenter.y, player->current.pos.y + 100.0f, 0.2f, 100.0f);
        cLib_addCalc2(&i_this->mCamCenter.z, player->current.pos.z, 0.2f, 100.0f);
        cLib_addCalc2(&i_this->mCamEye.y, player->current.pos.y + 100.0f, 0.1f, 5.0f);
        if (i_this->mDemoMode == 3) {
            i_this->mDemoMode = 4;
            i_this->mDemoTimer = 0;
            daPy_getPlayerActorClass()->setThrowDamage(a_this->shape_angle.y, 40.0f, KREG_F(14), 2, 1, 0);
            daPy_getPlayerActorClass()->changeDemoMode(1, 0, 0, 0);
        }
        if (i_this->mDemoMode == 4 && i_this->mDemoTimer == 70 + JREG_S(7)) {
            cVar13 = 1;
        }
        break;

    case 5:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        player_camera->mCamera.Stop();
        i_this->mDemoMode = 6;
        i_this->mDemoTimer = 0;
        i_this->mCamFovy = 55.0f;
        player_camera->mCamera.SetTrimSize(3);
        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        vec1.x = 200.0f + AREG_F(13);
        vec1.y = 100.0f + AREG_F(14);
        vec1.z = -300.0f + AREG_F(15);
        MtxPosition(&vec1, &i_this->mCamEye);
        i_this->mCamEye += player->current.pos;
        // fallthrough

    case 6:
        vec5 = a_this->current.pos;
        vec5.y += 100.0f + BREG_F(3);
        if (i_this->mDemoTimer == 0) {
            i_this->mCamCenter = vec5;
        } else {
            cLib_addCalc2(&i_this->mCamCenter.x, vec5.x, 0.2f, 50.0f);
            cLib_addCalc2(&i_this->mCamCenter.y, vec5.y, 0.2f, 50.0f);
            cLib_addCalc2(&i_this->mCamCenter.z, vec5.z, 0.2f, 50.0f);
        }
        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        vec1.x = 200.0f + AREG_F(13);
        vec1.y = 100.0f + AREG_F(14);
        vec1.z = -300.0f + AREG_F(15);
        MtxPosition(&vec1, &vec4);
        vec4 += player->current.pos;
        cLib_addCalc2(&i_this->mCamEye.x, vec4.x, 0.1f, 50.0f);
        cLib_addCalc2(&i_this->mCamEye.y, vec4.y, 0.1f, 50.0f);
        cLib_addCalc2(&i_this->mCamEye.z, vec4.z, 0.1f, 50.0f);
        if (i_this->mDemoTimer == 170) {
            cVar13 = 1;
            player_camera->mCamera.Reset(i_this->mCamCenter, i_this->mCamEye);
            dComIfGs_offOneZoneSwitch(1, -1);
        }
        break;

    case 10:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        player_camera->mCamera.Stop();
        i_this->mDemoMode = 11;
        i_this->mDemoTimer = 0;
        i_this->mCamFovy = 55.0f;
        player_camera->mCamera.SetTrimSize(3);
        daPy_getPlayerActorClass()->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(0x17, 0, 0, 0);
        i_this->mCamDist = 300.0f;
        dComIfGp_getEvent().startCheckSkipEdge(a_this);
        i_this->mTargetAngleY = i_this->mPlayerAngle;
        a_this->current.angle.y = -0x3384;
        a_this->shape_angle.y = a_this->current.angle.y;
        a_this->current.pos = a_this->home.pos;
        // fallthrough

    case 11:
        vec2.set(-102687.0f, -23441.5f, 38942.0f);
        player->setPlayerPosAndAngle(&vec2, -0x7384, 0);
        if (i_this->mDemoTimer >= 1) {
            i_this->mCamCenter.set(-102677.0f, -23375.0f, 38866.0f);
            i_this->mCamEye.set(-102392.0f, -23393.0f, 39031.0f);
        }
        if (i_this->mDemoTimer != 85) {
            break;
        }
        i_this->mDemoMode = 12;
        i_this->mDemoTimer = 0;
        // fallthrough

    case 12:
        vec2.set(-102520.0f, -23416.0f, 38021.0f);
        player->setPlayerPosAndAngle(&vec2, -0x3b5d, 0);
        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y + BREG_S(0));
        vec1.x = BREG_F(0) - 50.0f;
        vec1.y = 30.0f + BREG_F(1) + 50.0f;
        vec1.z = i_this->mCamDist;
        MtxPosition(&vec1, &i_this->mCamEye);
        i_this->mCamEye += a_this->current.pos;
        // fallthrough

    case 13:
        vec5 = a_this->current.pos;
        vec5.y += 100.0f + BREG_F(3);
        if (i_this->mDemoTimer == 0) {
            i_this->mCamCenter = vec5;
        } else {
            cLib_addCalc2(&i_this->mCamCenter.x, vec5.x, 0.2f, 50.0f);
            cLib_addCalc2(&i_this->mCamCenter.y, vec5.y, 0.2f, 50.0f);
            cLib_addCalc2(&i_this->mCamCenter.z, vec5.z, 0.2f, 50.0f);
        }
        if (i_this->mDemoTimer >= 20) {
            cLib_addCalc2(&i_this->mCamDist, 420.0f, 0.2f, i_this->mCamSpeed);
            cLib_addCalc2(&i_this->mCamSpeed, 5.0f, 1.0f, 0.1f);
        }
        break;

    case 14:
        i_this->mCamCenter = a_this->current.pos;
        i_this->mCamCenter.y += 100.0f + BREG_F(3);
        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y + BREG_S(0));
        vec1.x = 20.0f + BREG_F(0) + 150.0f;
        vec1.y = 30.0f + BREG_F(0) + 50.0f + 30.0f + 50.0f;
        vec1.z = 300.0f + BREG_F(0);
        MtxPosition(&vec1, &i_this->mCamEye);
        i_this->mCamEye += a_this->current.pos;
        break;

    case 15:
        cLib_addCalc2(&i_this->mCamFovy, 80.0f, 0.03f, 0.3f);
        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y + BREG_S(0));
        vec1.x = 20.0f + BREG_F(0) - 50.0f;
        vec1.y = 30.0f + BREG_F(1) + 50.0f + 30.0f + 100.0f;
        vec1.z = 200.0f + BREG_F(2);
        MtxPosition(&vec1, &i_this->mCamEye);
        i_this->mCamEye += a_this->current.pos;
        i_this->mCamSpeed = 0.0f;
        break;

    case 16:
        i_this->mCamFovy = 70.0f + DREG_F(3);
        cMtx_YrotS(*calc_mtx, a_karg->current.angle.y);
        cMtx_XrotM(*calc_mtx, a_karg->current.angle.x);
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = 500.0f;
        MtxPosition(&vec1, &i_this->mCamCenter);
        i_this->mCamCenter += a_karg->current.pos;
        i_this->mCamEye = a_karg->current.pos;
        if (i_this->mDemoTimer == 80) {
            kargarok->mMode = 3;
            i_this->mCamBankPhase = 80;
        }
        if (i_this->mDemoTimer >= 80) {
            cLib_addCalc2(&i_this->mCamBank, 3000.0f + AREG_F(5), 1.0f, 20.0f);
        }
        break;

    case 17:
        i_this->mCamFovy = 55.0f + JREG_F(18);
        if (i_this->mDemoTimer >= 1) {
            kargarok->mNoDrawFlag = false;
            i_this->mCamBank = 0.0f;
            i_this->mCamCenter = a_this->current.pos
                                + (a_karg->current.pos - a_this->current.pos) * 0.5f;
            i_this->mCamCenter.y += 50.0f + BREG_F(3);
            cMtx_YrotS(*calc_mtx, a_this->shape_angle.y + BREG_S(0));
            vec1.x = 20.0f + BREG_F(0) + 150.0f;
            vec1.y = 30.0f + BREG_F(1) + 50.0f + 30.0f + 50.0f;
            vec1.z = 400.0f + BREG_F(2);
            MtxPosition(&vec1, &i_this->mCamEye);
            i_this->mCamEye += a_this->current.pos;
        }
        break;

    case 18:
        i_this->mCamCenter = a_this->current.pos
                            + (a_karg->current.pos - a_this->current.pos) * 0.5f;
        i_this->mCamCenter.y += 50.0f + BREG_F(3);
        i_this->mCamSpeed = 0.0f;
        break;

    case 19:
        i_this->mCamCenter = a_this->current.pos
                            + (a_karg->current.pos - a_this->current.pos) * 0.5f;
        i_this->mCamCenter.y += 50.0f + BREG_F(3);
        if (i_this->mDemoTimer > 30) {
            cMtx_YrotS(*calc_mtx, a_karg->shape_angle.y);
            vec1.x = 200.0f + BREG_F(13);
            vec1.y = BREG_F(14) - 500.0f + 100.0f + 50.0f;
            vec1.z = 1000.0f + BREG_F(15) + 50.0f;
            MtxPosition(&vec1, &vec4);
            vec4 += a_karg->current.pos;
            cLib_addCalc2(&i_this->mCamEye.x, vec4.x, 0.1f, i_this->mCamSpeed * 10.0f);
            cLib_addCalc2(&i_this->mCamEye.y, vec4.y, 0.1f, i_this->mCamSpeed * 10.0f);
            cLib_addCalc2(&i_this->mCamEye.z, vec4.z, 0.1f, i_this->mCamSpeed * 10.0f);
            cLib_addCalc2(&i_this->mCamSpeed, 1.0f, 1.0f, 0.02f);
        }
        if (i_this->mDemoTimer == 150) {
            i_this->mAction = ACT_YC_RIDE;
            i_this->mMode = 0;
            kargarok->mAction = e_yc_class::ACT_HOVERING;
            kargarok->mMode = 0;
            a_karg->home.pos = a_karg->current.pos;
            kargarok->mFlyAnmMode = 1;
            cVar13 = 1;
            player_camera->mCamera.Reset(i_this->mCamCenter, i_this->mCamEye);
            dComIfGs_onOneZoneSwitch(1, -1);
        }
        break;

    case 20:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        player_camera->mCamera.Stop();
        i_this->mDemoMode = 21;
        i_this->mDemoTimer = 0;
        i_this->mCamFovy = 55.0f;
        player_camera->mCamera.SetTrimSize(3);
        i_this->mCamCenter.set(-93806.0f, -5894.0f, 42791.0f);
        i_this->mCamEye.set(-93532.0f, -5824.0f, 43075.0f);
        daPy_getPlayerActorClass()->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(4, 0, 0, 0);
        Z2GetAudioMgr()->bgmStop(70, 0);
        dComIfGp_getEvent().offFlag2(8);
        dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[11] & 0xffff);
        // fallthrough

    case 21:
        vec2.set(-93705.0f, -5951.0f, 42867.0f);
        player->setPlayerPosAndAngle(&vec2, 0, 0);
        if (i_this->mDemoTimer >= 30) {
            if (i_this->mDemoTimer == 30) {
                i_this->mMsgFlow.init(a_this, 0x7d1, 0, NULL);
            }
            if (i_this->mDemoTimer == 50) {
                dComIfGp_getEvent().startCheckSkipEdge(a_this);
            }
            if (i_this->mDemoTimer == 55) {
                daPy_getPlayerActorClass()->changeDemoMode(1, 4, 0, 0);
            }
            if (i_this->mDemoTimer == 130) {
                daPy_getPlayerActorClass()->changeDemoMode(0x35, 0, 0, 0);
            }
            if (i_this->mMsgFlow.doFlow(a_this, NULL, 0)) {
                if (i_this->mDemoTimer < 130) {
                    daPy_getPlayerActorClass()->changeDemoMode(0x35, 0, 0, 0);
                }
                i_this->mDemoMode = 22;
                i_this->mDemoTimer = 0;
                Z2GetAudioMgr()->bgmStart(Z2BGM_BRIDGE_ON_FIRE, 0, 0);
            }
        }
        break;

    case 22:
        if (i_this->mDemoTimer == 50) {
            daPy_getPlayerActorClass()->changeDemoMode(0x17, 0, 0, 0);
        }
        if (i_this->mDemoTimer >= 70) {
            if (i_this->mDemoTimer == 70) {
                i_this->mCamCenter.set(-93860.0f, -5786.0f, 42701.0f);
                i_this->mCamEye.set(-93644.0f, -5908.0f, 43016.0f);
            }
            if (i_this->mDemoTimer >= 80) {
                cLib_addCalc2(&i_this->mCamFovy, 40.0f, 0.5f, 5.0f);
            }
        }
        if (i_this->mDemoTimer == 110) {
            i_this->mCamCenter.set(-93614.0f, -5879.0f, 43003.0f);
            i_this->mCamEye.set(-93593.0f, -5871.0f, 42603.0f);
            i_this->mCamFovy = 55.0f;
            i_this->mDemoMode = 23;
            i_this->mDemoTimer = 0;
        }
        break;

    case 23:
        if (i_this->mDemoTimer == 10) {
            i_this->mMode = 3;
            a_this->speed.y = 35.0f + KREG_F(17);
        }
        if (i_this->mDemoTimer == 40) {
            i_this->mCamCenterTarget.set(-93660.0f, -5873.0f, 49831.0f);
            i_this->mCamEyeTarget.set(-93551.0f, -5926.0f, 49449.0f);
            i_this->mCamEyeSpeed.x = fabsf(i_this->mCamEyeTarget.x - i_this->mCamEye.x);
            i_this->mCamEyeSpeed.y = fabsf(i_this->mCamEyeTarget.y - i_this->mCamEye.y);
            i_this->mCamEyeSpeed.z = fabsf(i_this->mCamEyeTarget.z - i_this->mCamEye.z);
            i_this->mCamCenterSpeed.x = fabsf(i_this->mCamCenterTarget.x - i_this->mCamCenter.x);
            i_this->mCamCenterSpeed.y = fabsf(i_this->mCamCenterTarget.y - i_this->mCamCenter.y);
            i_this->mCamCenterSpeed.z = fabsf(i_this->mCamCenterTarget.z - i_this->mCamCenter.z);
            i_this->mCamSpeed = 0.0f;
            i_this->mDemoMode = 24;
            i_this->mDemoTimer = 0;
        }
        break;

    case 24:
        i_this->mCamSpeed = 0.05f;
        cam_3d_morf(i_this, 0.5f);
        if (i_this->mDemoTimer == 50) {
            i_this->mMode = 4;
        }
        if (i_this->mDemoTimer != 135) {
            break;
        }
        i_this->mDemoMode = 25;
        i_this->mDemoTimer = 0;
        // fallthrough

    case 25:
        i_this->mCamEye.set(-92666.0f, -5540.0f, 43878.0f);
        cVar12 = 1;
        if (i_this->mDemoTimer != 100) {
            break;
        }
        i_this->mDemoMode = 26;
        i_this->mDemoTimer = 0;
        vec2.set(-93666.0f, -5951.0f, 39000.0f);
        player->setPlayerPosAndAngle(&vec2, 0, 0);
        // fallthrough

    case 26:
        i_this->mCamEye.set(-93539.0f, -5857.0f, 30480.0f);
        if (i_this->mDemoTimer >= 115) {
            i_this->mDemoMode = 27;
            i_this->mDemoTimer = 0;
            i_this->mCamEyeSpeed.x = i_this->mCamEyeSpeed.y = 0.0f;
            i_this->mCamSpeed = 0.0f;
        } else {
            cVar12 = 2;
        }
        break;

    case 27: {
        cVar12 = 3;
        cLib_addCalc2(&i_this->mCamEye.y, -4469.0f, 0.05f, i_this->mCamEyeSpeed.y);
        cLib_addCalc2(&i_this->mCamEyeSpeed.y, 50.0f, 1.0f, 0.5f);
        cLib_addCalc2(&i_this->mCamEye.x, -92006.0f, 0.05f, i_this->mCamEyeSpeed.x);
        cLib_addCalc2(&i_this->mCamEyeSpeed.x, 50.0f, 1.0f, 0.5f);
        if (i_this->mDemoTimer != 115) {
            break;
        }
        i_this->mDemoMode = 28;
        i_this->mDemoTimer = 0;
        i_this->mMode = 4;
        i_this->mCamDist = 0.0f;
        e_arrow_class* arrow = (e_arrow_class*)fopAcM_SearchByName(PROC_E_ARROW);
        arrow->field_0xa00.z = 30000.0f + NREG_F(0);
        cVar12 = 0;
    }
    // fallthrough
    case 28:
        i_this->mCamCenter.set(-93660.0f, -5873.0f, 49831.0f);
        i_this->mCamEye.set(-93551.0f, -5926.0f, 49449.0f);
        if (i_this->mDemoTimer == 80) {
            i_this->mDemoMode = 29;
            i_this->mDemoTimer = 0;
        }
        break;

    case 29:
        i_this->mCamCenter.set(-92787.0f, -5256.0f, 46550.0f);
        i_this->mCamEye.set(-92609.0f, -5139.0f, 46210.0f);
        if (i_this->mDemoTimer == 115) {
            i_this->mDemoMode = 30;
            i_this->mDemoTimer = 0;
            i_this->field_0x13d4 = NREG_S(1) - 0x2000;
        }
        break;

    case 30:
        cMtx_YrotS(*calc_mtx, i_this->field_0x13d4);
        vec1.x = 0.0f;
        vec1.y = 150.0f + NREG_F(8);
        vec1.z = 400.0f + NREG_F(9);
        MtxPosition(&vec1, &i_this->mCamEye);
        i_this->mCamEye += player->current.pos;
        i_this->mCamCenter = player->current.pos;
        i_this->mCamCenter.y += 100.0f + NREG_F(10);
        i_this->field_0x13d4 += (s16)(230 + NREG_S(2));
        if (i_this->mDemoTimer >= 30) {
            if (i_this->mDemoTimer == 30) {
                i_this->mMsgFlow.init(a_this, 0x7d1, 0, NULL);
            }
            if (i_this->mMsgFlow.doFlow(a_this, NULL, 0)) {
                cVar13 = 1;
            }
            if (i_this->mDemoTimer >= 40 && !dMsgObject_isTalkNowCheck()) {
                cVar13 = 1;
            }
        }
        break;

    case 100:
        cVar13 = 1;
    }

    if (cVar12 != 0) {
        fopAc_ac_c* a_arr = (fopAc_ac_c*) fopAcM_SearchByName(PROC_E_ARROW);
        if (a_arr != NULL) {
            e_arrow_class* arrow = (e_arrow_class*) a_arr;
            if (cVar12 == 3) {
                vec5 = arrow->field_0xa00;
            } else {
                vec5 = a_arr->current.pos;
            }
            f32 scale;
            if (cVar12 == 1) {
                vec5.y -= 300.0f + JREG_F(6);
                scale = 0.2f;
            } else {
                scale = 0.5f;
            }
            if (i_this->mDemoTimer == 0) {
                i_this->mCamCenter = vec5;
            } else {
                cLib_addCalc2(&i_this->mCamCenter.x, vec5.x, scale, 200.0f);
                cLib_addCalc2(&i_this->mCamCenter.y, vec5.y, scale, 200.0f);
                cLib_addCalc2(&i_this->mCamCenter.z, vec5.z, scale, 200.0f);
            }
        }
    }

    if (i_this->mDemoMode >= 11 && i_this->mDemoMode < 20) {
        if (dComIfGp_getEvent().checkSkipEdge()) {
            a_this->current.pos.set(-103242.0f, -22894.0f, 38097.0f);
            a_this->current.angle.y = 0x44a3;
            a_karg->current.pos.set(-103242.0f, -22894.0f, 38097.0f);
            a_karg->current.angle.y = 0x44a3;
            i_this->mAction = ACT_YC_RIDE;
            i_this->mMode = 0;
            kargarok->mAction = e_yc_class::ACT_HOVERING;
            kargarok->mMode = 0;
            kargarok->mNoDrawFlag = false;
            a_karg->speedF = 0.0f;
            cVar13 = 1;
            dComIfGs_onOneZoneSwitch(1, -1);
            Z2GetAudioMgr()->setBattleBgmOff(false);
        }
    } else if (i_this->mDemoMode >= 20 && i_this->mDemoMode < 29) {
        if (dComIfGp_getEvent().checkSkipEdge()) {
            cVar13 = 2;
            vec2.set(-93666.0f, -5951.0f, 39000.0f);
            player->setPlayerPosAndAngle(&vec2, 0, 0);
            fpcM_Search(s_adel_sub, i_this);
#if VERSION != VERSION_SHIELD_DEBUG
            i_this->mMode = 2;
            anm_init(i_this, ANM_WAIT01, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
#endif
            u32 child_params = 0x80000011;
            csXyz angle(0, 0x4000, 0);
            vec2.set(-93652.0f, -5875.0f, 46674.0f);
            fopAcM_createChild(PROC_E_ARROW, fopAcM_GetID(a_this), child_params, &vec2,
                               fopAcM_GetRoomNo(a_this), &angle, NULL, -1, NULL);
            vec2.set(-93659.0f, -5875.0f, 32500.0f);
            fopAcM_createChild(PROC_E_ARROW, fopAcM_GetID(a_this), child_params, &vec2,
                               fopAcM_GetRoomNo(a_this), &angle, NULL, -1, NULL);
            Z2GetAudioMgr()->bgmStop(30, 0);
            mDoGph_gInf_c::fadeOut(1.0f, g_blackColor);
            i_this->mFadeTimer = 30;
        }
    }

    if (cVar13 != 0) {
        if (cVar13 == 2) {
            cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            vec1.x = 0.0f;
            vec1.y = 200.0f + JREG_F(1);
            vec1.z = -250.0f + JREG_F(2);
            MtxPosition(&vec1, &i_this->mCamEye);
            i_this->mCamEye += player->current.pos;
            i_this->mCamCenter = player->current.pos;
            i_this->mCamCenter.y += 120.0f;
            player_camera->mCamera.Reset(i_this->mCamCenter, i_this->mCamEye, i_this->mCamFovy, 0);
        }
        player_camera->mCamera.Start();
        player_camera->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();
        daPy_getPlayerActorClass()->cancelOriginalDemo();
        i_this->mDemoMode = 0;
    }

    if (i_this->mDemoMode > 0) {
        s16 bank = -i_this->mCamBank * cM_scos(i_this->mCamBankPhase * (600 + AREG_S(5)));
        player_camera->mCamera.Set(i_this->mCamCenter, i_this->mCamEye, bank, i_this->mCamFovy);
        i_this->mDemoTimer++;
        i_this->mCamBankPhase++;
    }
}

static BOOL c_start;

static int daE_RDY_Execute(e_rdy_class* i_this) {
    f32 scale;
    fopEn_enemy_c* a_this = &i_this->actor;
    if (!c_start && dComIfGp_event_runCheck()) {
        if (fopAcM_getTalkEventPartner(daPy_getLinkPlayerActorClass())
                                                        == daPy_py_c::getMidnaActor()
            || daPy_getPlayerActorClass()->eventInfo.checkCommandDoor())
        {
            return 1;
        }
    }

    if (i_this->field_0x5b8 == 12 && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[239] & 0xffff)) {
        fopAcM_OffStatus(a_this, 0);
        a_this->attention_info.flags = 0;
        return 1;
    }

    if (i_this->mKargarokDeleteTimer != 0) {
        i_this->mKargarokDeleteTimer--;
        if (i_this->mKargarokDeleteTimer == 0) {
            daKago_c* kago = (daKago_c*)fopAcM_SearchByName(PROC_KAGO);
            kago->setEvent2();
            fopAcM_delete(a_this);
            fopAcM_delete(fopAcM_SearchByID(i_this->mKargarokID));
        }
        return 1;
    }

    if (i_this->mFadeTimer != 0) {
        --i_this->mFadeTimer;
        if (i_this->mFadeTimer == 20) {
            mDoGph_gInf_c::fadeIn(0.05f, g_blackColor);
        }
        if (i_this->mFadeTimer == 0) {
            Z2GetAudioMgr()->bgmStart(Z2BGM_EVENT02, 0, 0);
        }
    }

    cXyz vec1, vec2, vec3;

    if (S_find != 0) {
        S_find--;
    }
    i_this->mCounter++;
    i_this->mDrawEyeModel = l_HIO.mDrawEyeModel;
    for (int i = 0; i < 4; i++) {
        if (i_this->mTimer[i] != 0) {
            i_this->mTimer[i]--;
        }
    }
    if (i_this->mIFrameTimer != 0) {
        i_this->mIFrameTimer--;
    }
    if (i_this->field_0xbc0 != 0) {
        i_this->field_0xbc0--;
    }
    if (i_this->field_0xa6c != 0) {
        i_this->field_0xa6c--;
    }
    if (i_this->field_0xa6e != 0) {
        i_this->field_0xa6e--;
    }

    if (fopAcM_otoCheck(a_this, 1000.0f) || daPy_getPlayerActorClass()->checkWolfBark()) {
        i_this->field_0xa71 = cM_rndF(10.0f) + 10.0f;
    }
    if (i_this->field_0xa71 != 0) {
        i_this->field_0xa71--;
    }

    i_this->mHasArrow = false;
    i_this->mCollisionEnabled = true;

    action(i_this);

    if (i_this->mRideState != 0) {
        i_this->mCollisionEnabled = false;
    }

    if (i_this->mRideState != 2 && i_this->field_0xbc0 == 0) {
        vec2.set(0.0f, 0.0f, 0.0f);
        if (i_this->field_0xabc) {
            cMtx_YrotS(*calc_mtx, i_this->field_0xadc.y);
            vec1.x = 0.0f;
            vec1.y = 0.0f + TREG_F(8);
            vec1.z = i_this->field_0xbbc;
            MtxPosition(&vec1, &vec2);
            cLib_addCalc2(&i_this->field_0xbbc, i_this->field_0xabc * (2.0f + TREG_F(9)), 1.0f, 5.0f + TREG_F(17));
        } else {
            cLib_addCalc0(&i_this->field_0xbbc, 1.0f, 5.0f + TREG_F(17));
        }
        if (i_this->mAnm == ANM_DIEB && i_this->field_0xadc.x != 0 && !i_this->mIsUpsideDown) {
            vec2.y += -20.0f + KREG_F(7);
        }
        a_this->current.pos += vec2;
        a_this->old.pos += vec2;
        i_this->mAcch.CrrPos(dComIfG_Bgsp());
        a_this->current.pos -= vec2;
        a_this->old.pos -= vec2;
        i_this->mInWater = false;
        if (i_this->mAcch.GetGroundH() != -1.0e9f && i_this->mAcch.ChkWaterHit()
            && i_this->mAcch.m_wtr.GetHeight() > a_this->current.pos.y)
        {
            i_this->mInWater = true;
        }
    }

    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y + i_this->mJumpHeight,
                           a_this->current.pos.z);
    mDoMtx_stack_c::XrotM((s16)i_this->field_0xae2.x);
    mDoMtx_stack_c::ZrotM((s16)i_this->field_0xae2.z);
    mDoMtx_stack_c::YrotM((s16)i_this->field_0xadc.y);
    mDoMtx_stack_c::XrotM((s16)i_this->field_0xadc.x);
    mDoMtx_stack_c::YrotM(-i_this->field_0xadc.y);
    mDoMtx_stack_c::YrotM((s16)a_this->shape_angle.y);
    mDoMtx_stack_c::XrotM((s16)a_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);
    scale = l_HIO.mScale * a_this->scale.x;
    mDoMtx_stack_c::scaleM(scale, scale, scale);
    J3DModel* model = i_this->mpMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    int frame = i_this->mpMorf->getFrame();
    if (i_this->mAnm == ANM_RUN && (frame == 1 || frame == 10)) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_RUNNING_BREATH, -1);
    }
    if (i_this->mAnm == ANM_KYORO2 && (frame == 5 || frame == 26)) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_RUNNING_BREATH, -1);
    }
    if ((i_this->mAnm == ANM_SHOOT_READY && frame == 10)
        || (i_this->mAnm == ANM_RSHOOT_READY && frame == 5)
        || (i_this->mAnm == ANM_ARMAMENT && frame == 10))
    {
        i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_READY_WEAPON, -1);
    }
    if ((i_this->mAnm == ANM_WALK
            && (i_this->mpMorf->checkFrame(1.0f) || i_this->mpMorf->checkFrame(15.0f)))
        || ((i_this->mAnm == ANM_RUN || i_this->mAnm == ANM_RUN02)
            && (i_this->mpMorf->checkFrame(1.0f) || i_this->mpMorf->checkFrame(7.0f))))
    {
        if (i_this->mInWater) {
            i_this->mSound.startCreatureSound(Z2SE_RIDER_FOOTNOTE_WATER, 0, -1);
        } else {
            i_this->mSound.startCreatureSound(Z2SE_RIDER_FOOTNOTE, 0, -1);
        }
    }

    i_this->mpMorf->modelCalc();

    if (dComIfGp_checkPlayerStatus0(0, 0x200000)) {
        i_this->mDrawEyeModel = false;
    }

    static f32 time_scale[25] = {
        1.2f, 1.2f, 1.1f, 1.0f, 0.8f, 0.6f, 0.4f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.2f, 0.5f, 0.8f, 1.2f, 1.2f, 1.2f, 1.2f,
    };

    if (i_this->mDrawEyeModel) {
        J3DModel* model = i_this->mpMorf->getModel();
        cLib_addCalc2(&i_this->mEyeScale, i_this->mTargetEyeScale, 1.0f, 0.02f);
        i_this->mTargetEyeScale = 1.0f;
        MTXCopy(model->getAnmMtx(JNT_HEAD), *calc_mtx);
        camera_class* camera = (camera_class*) dComIfGp_getCamera(0);
        s16 ang_x, ang_y;
        for (int i = 0; i < 2; i++) {
            MtxPush();
            if (i == 0) {
                vec1.set(38.0f, 0.0f, 0.0f);
                MtxPosition(&vec1, &vec2);
                vec1 = camera->lookat.eye - vec2;
                ang_y = cM_atan2s(vec1.x, vec1.z);
                ang_x = -cM_atan2s(vec1.y, JMAFastSqrt(vec1.x * vec1.x + vec1.z * vec1.z));
                scale = vec1.abs() * (0.001f + JREG_F(8));
                if (scale > 2.0f + JREG_F(17)) {
                    scale = 2.0f + JREG_F(17);
                }

                dScnKy_env_light_c* light = dKy_getEnvlight();
                int div_res = light->daytime / 15.0f;
                scale *= i_this->mEyeScale * time_scale[div_res];
                vec1.set(38.0f, 0.0f, 6.0f);
            } else {
                vec1.set(38.0f, 0.0f, -6.0f);
            }
            MtxPosition(&vec1, &vec2);
            MtxTrans(vec2.x, vec2.y, vec2.z, 0);
            cMtx_YrotM(*calc_mtx, ang_y);
            cMtx_XrotM(*calc_mtx, ang_x);
            MtxScale(scale, scale, scale, 1);
            i_this->mpEyeModel[i]->setBaseTRMtx(*calc_mtx);
            MtxPull();
        }
    }

    MTXCopy(model->getAnmMtx(JNT_MUNE2), *calc_mtx);
    vec1.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&vec1, &vec2);
    vec1.set(YREG_F(1), 100.0f + YREG_F(2), YREG_F(3));
    MtxPosition(&vec1, &vec3);
    if (vec2.y < vec3.y) {
        i_this->mIsUpsideDown = false;
    } else {
        i_this->mIsUpsideDown = true;
    }

    cXyz cc_offset(0.0f, 0.0f, 0.0f);
    if (i_this->mIFrameTimer != 0) {
        cc_offset.set(-20000.0f, 200000.0f, 30000.0f);
    }

    MTXCopy(model->getAnmMtx(JNT_HEAD), *calc_mtx);
    vec1.set(20.0f, 0.0f, 0.0f);
    MtxPosition(&vec1, &a_this->eyePos);

    if (daPy_py_c::checkNowWolf()) {
        cc_offset.y += 30.0f;
        i_this->mCcSph[0].SetC(a_this->eyePos + cc_offset);
        i_this->mCcSph[0].SetR(50.0f);
    } else {
        i_this->mCcSph[0].SetC(a_this->eyePos + cc_offset);
        i_this->mCcSph[0].SetR(35.0f);
    }

    a_this->attention_info.position = a_this->eyePos;
    a_this->attention_info.position.y += 30.0f;

    vec1.set(BREG_F(14), BREG_F(15), BREG_F(16));
    MTXCopy(model->getAnmMtx(JNT_KUBI), *calc_mtx);
    MtxPosition(&vec1, &vec2);
    i_this->mCcSph[2].SetC(vec2 + cc_offset);
    i_this->mCcSph[2].SetR(35.0f);

    vec1.set(-10.0f + BREG_F(11), BREG_F(12), BREG_F(13));
    MTXCopy(model->getAnmMtx(JNT_MUNE1), *calc_mtx);
    MtxPosition(&vec1, &vec2);
    i_this->mCcSph[1].SetC(vec2 + cc_offset);
    i_this->mCcSph[1].SetR(35.0f);

    for (int i = 0; i <= 2; i++) {
        if (i_this->mCollisionEnabled) {
            i_this->mCcSph[i].OnCoSetBit();
        } else {
            i_this->mCcSph[i].OffCoSetBit();
        }
        dComIfG_Ccsp()->Set(&i_this->mCcSph[i]);
        i_this->mCcSph[i].OffTgNoHitMark();
    }

    if (i_this->field_0xa7b == 2) {
        MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(JNT_MUNE2), *calc_mtx);
        vec1.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&vec1, &vec2);
        i_this->mAtSph.SetR(l_HIO.mScale * 50.0f);
        i_this->mAtSph.SetC(vec2);
        dComIfG_Ccsp()->Set(&i_this->mAtSph);
        i_this->field_0xa7b = 0;
    }

    if (i_this->mWeaponType == WEAPON_CLUB) {
        if (!i_this->field_0x5bb) {
            MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(JNT_HAND_R), *calc_mtx);
        } else {
            MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(JNT_MUNE2), *calc_mtx);
            cMtx_YrotM(*calc_mtx, 6000);
            cMtx_XrotM(*calc_mtx, 10000);
            cMtx_ZrotM(*calc_mtx, 18000);
            MtxTrans(-30.0f, -50.0f, 20.0f, 1);
        }
        i_this->mpWeaponModel->setBaseTRMtx(*calc_mtx);

        if (i_this->field_0xa7b != 0) {
            vec1.set(0.0f, 30.0f, 0.0f);
            MtxPosition(&vec1, &vec2);
            i_this->mAtSph.SetR(l_HIO.mScale * 50.0f);
            if (!i_this->field_0xa7c) {
                i_this->field_0xa7c = true;
                i_this->mAtSph.StartCAt(vec2);
            } else {
                i_this->mAtSph.MoveCAt(vec2);
            }
            i_this->field_0xa7b = 0;
        } else {
            cXyz at_center(-200000.0f, -200000.0f, -200000.0f);
            i_this->mAtSph.SetC(at_center);
            i_this->mAtSph.SetR(-200.0f);
            i_this->field_0xa7c = false;
        }
        dComIfG_Ccsp()->Set(&i_this->mAtSph);
    }

    if (i_this->mWeaponType >= WEAPON_BOW) {
        J3DModel* model_ptr = i_this->mpMorf->getModel();
        if (i_this->mBowRotationTimer != 0) {
            i_this->mBowRotationTimer--;
        }
        i_this->mBowRotation = i_this->mBowRotationTimer
                                * cM_ssin(i_this->mBowRotationTimer * (TREG_S(9) + 0x7800)) * (TREG_F(5) + 100.0f);

        model_ptr = i_this->mpBowMorf->getModel();
        if (!i_this->field_0x5bb) {
            model_ptr->setBaseTRMtx(i_this->mpMorf->getModel()->getAnmMtx(JNT_HAND_L));
        } else {
            MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(JNT_MUNE2), *calc_mtx);
            cMtx_YrotM(*calc_mtx, 6000);
            cMtx_XrotM(*calc_mtx, 10000);
            cMtx_ZrotM(*calc_mtx, 18000);
            MtxTrans(-20.0f, -20.0f, 30.0f, 1);
            model_ptr->setBaseTRMtx(*calc_mtx);
        }
        i_this->mpBowMorf->play(0, 0);
        i_this->mpBowMorf->modelCalc();

        if (i_this->mHasArrow) {
            MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(JNT_HAND_R), *calc_mtx);
            i_this->mpWeaponModel->setBaseTRMtx(*calc_mtx);
            if (i_this->mWeaponType >= WEAPON_BOW_FIRE) {
                if (i_this->mRideState != 2 || i_this->mTimer[1] <= 18) {
                    fire_eff_set(i_this);
                }
            } else {
                i_this->field_0x134c.set(-200000.0f, -200000.0f, -200000.0f);
            }
        }

        if (i_this->mArrowFired) {
            if (i_this->mWeaponType == WEAPON_BOW_FIRE) {
                i_this->mSound.startCreatureSound(Z2SE_OBJ_ARROW_SHOT_FIRE, 0, -1);
            } else {
                i_this->mSound.startCreatureSound(Z2SE_OBJ_ARROW_SHOT_NORMAL, 0, -1);
            }
            i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_SHOT_ARROW, -1);
            vec1.set(0.0f, 0.0f, 0.0f);
            MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(JNT_HAND_R), *calc_mtx);
            MtxPosition(&vec1, &vec2);
            csXyz arrow_angle;
            if (i_this->mpPath != NULL || i_this->mAction == ACT_BOW_IKKI2) {
                vec1 = i_this->mArrowTarget - vec2;
                arrow_angle.y = cM_atan2s(vec1.x, vec1.z);
                arrow_angle.x = -cM_atan2s(vec1.y, JMAFastSqrt(vec1.x * vec1.x + vec1.z * vec1.z));
            } else {
                fopAc_ac_c* player = dComIfGp_getPlayer(0);
                vec1 = player->eyePos;
                if (i_this->field_0x1366) {
                    f32 mult_val = 15.0f + TREG_F(7);
                    vec1.x += player->speed.x * mult_val;
                    vec1.z += player->speed.z * mult_val;
                }
                vec1 -= vec2;
                vec1.y -= 30.0f;
                arrow_angle.y = cM_atan2s(vec1.x, vec1.z);
                arrow_angle.x = -cM_atan2s(vec1.y, JMAFastSqrt(vec1.x * vec1.x + vec1.z * vec1.z));
                if (strcmp(dComIfGp_getStartStageName(), "F_SP121") && !i_this->field_0x1366) {
                    // Not Hyrule Field
                    arrow_angle.x += (s16)(cM_rndFX(200.0f) + -500.0f);
                    arrow_angle.y += (s16)cM_rndFX(100.0f);
                }
            }
            arrow_angle.z = 0;
            u32 params = 0x80000000;
            if (i_this->mWeaponType == WEAPON_BOW_FIRE) {
                params |= 1;
            } else if (i_this->mWeaponType == WEAPON_BOW_BOMB) {
                params |= 2;
            }
            if (i_this->field_0x5b8 == 11) {
                params |= 0x10;
            }
            fopAcM_createChild(PROC_E_ARROW, fopAcM_GetID(a_this), params, &vec2,
                               fopAcM_GetRoomNo(a_this), &arrow_angle, NULL, -1, NULL);
            i_this->mArrowFired = false;
            i_this->mBowRotationTimer = 15 + TREG_S(8);
        }
    }

    if (i_this->mIsHoldingGrass) {
        s16 ang_y = 0;
        if (i_this->mTimer[2] != 0) {
            int tmp = i_this->mTimer[2] * 10;
            if (tmp > 400) {
                tmp = 400;
            }
            ang_y = (i_this->mCounter & 1) * tmp * 2 - tmp;
        }
        MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(JNT_HAND_R), *calc_mtx);
        MtxTrans(20.0f + NREG_F(0), 5.0f + NREG_F(1), -10.0f + NREG_F(2), 1);
        cMtx_XrotM(*calc_mtx, NREG_S(0) + 0x8000);
        cMtx_YrotM(*calc_mtx, NREG_S(1) + ang_y + -0x15cd);
        cMtx_ZrotM(*calc_mtx, NREG_S(2) + 0x28cd);
        i_this->mpHawkGrassModel->setBaseTRMtx(*calc_mtx);
    }

    if (i_this->mIsDying) {
        cLib_addCalc2(&i_this->mDyingTevColor, -20.0f, 1.0f, 0.4f);
    }

    if (i_this->mRideState == 0) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(JNT_MUNE2), mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&vec2);
        vec1 = player->current.pos - vec2;
        s16 tan_res = cM_atan2s(vec1.x, vec1.z);
        cMtx_YrotS(*calc_mtx, tan_res);
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = 40.0f + BREG_F(12) - 30.0f;
        MtxPosition(&vec1, &vec3);
        vec3 += vec2;
        a_this->setDownPos(&vec3);
    }

    cXyz lock_pos = a_this->eyePos;
    lock_pos.y += 130.0f + NREG_F(7);
    a_this->setHeadLockPos(&lock_pos);

    demo_camera(i_this);
    return 1;
}

static int daE_RDY_IsDelete(e_rdy_class* i_this) {
    return 1;
}

static int daE_RDY_Delete(e_rdy_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    dComIfG_resDelete(&i_this->mPhase, i_this->mpArcName);

    if (i_this->mHIOInit) {
        hioInit = false;
    }

    if (a_this->heap != NULL) {
        i_this->mpMorf->stopZelAnime();
    }

    return 1;
}

static Vec jv_offset = {0.0f, 0.0f, 0.0f};
static s8 data_8077aa20;

static dJntColData_c jc_data[11] = {
    {0, 1, JNT_HIP1, 30.0f, &jv_offset},
    {0, 1, JNT_MUNE2, 25.0f, &jv_offset},
    {0, 1, JNT_KUBI, 20.0f, &jv_offset},
    {0, 1, JNT_ARM_L1, 15.0f, &jv_offset},
    {0, 1, JNT_ARM_L2, 10.0f, &jv_offset},
    {0, 1, JNT_ARM_R1, 15.0f, &jv_offset},
    {0, 1, JNT_ARM_R2, 10.0f, &jv_offset},
    {0, 1, JNT_MOMO_L, 10.0f, &jv_offset},
    {0, 1, JNT_SUNE_L1, 5.0f, &jv_offset},
    {0, 1, JNT_SUNE_R1, 10.0f, &jv_offset},
    {0, 1, JNT_ASI_R, 5.0f, &jv_offset},
};

static int useHeapInit(fopAc_ac_c* i_this) {
    e_rdy_class* _this = (e_rdy_class*)i_this;
    
    _this->mpMorf = new mDoExt_McaMorfSO(
        (J3DModelData*)dComIfG_getObjectRes(_this->mpArcName, 81), NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectRes(_this->mpArcName, 76),
        J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, &_this->mSound, 0x80000, 0x11000084);
    if (_this->mpMorf == NULL || _this->mpMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = _this->mpMorf->getModel();
    model->setUserArea((uintptr_t)_this);
    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }
    if (!_this->mJntCol.init(i_this, jc_data, model, 11)) {
        return 0;
    }

    if (_this->mWeaponType == WEAPON_CLUB) {
        J3DModelData* weapon_model_data = (J3DModelData*)dComIfG_getObjectRes(_this->mpArcName, 85);
        _this->mpWeaponModel = mDoExt_J3DModel__create(weapon_model_data, 0x80000, 0x11000084);
        if (_this->mpWeaponModel == NULL) {
            return 0;
        }
    } else if (_this->mWeaponType >= WEAPON_BOW) {
        _this->mpBowMorf = new mDoExt_McaMorfSO(
            (J3DModelData*)dComIfG_getObjectRes(_this->mpArcName, 84), NULL, NULL,
            (J3DAnmTransform*)dComIfG_getObjectRes(_this->mpArcName, 11),
            J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, NULL, 0x80000, 0x11000084);
        if (_this->mpBowMorf == NULL || _this->mpBowMorf->getModel() == NULL) {
            return 0;
        }

        J3DModel* bow_model = _this->mpBowMorf->getModel();
        bow_model->setUserArea((uintptr_t)_this);
        for (u16 i = 0; i < bow_model->getModelData()->getJointNum(); i++) {
            if (i == 2 || i == 3) {
                bow_model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_bow);
            }
        }

        J3DModelData* weapon_model_data;
        if (_this->mWeaponType == WEAPON_BOW_BOMB) {
            weapon_model_data = (J3DModelData*)dComIfG_getObjectRes(_this->mpArcName, 83);
        } else {
            weapon_model_data = (J3DModelData*)dComIfG_getObjectRes(_this->mpArcName, 82);
        }
        _this->mpWeaponModel = mDoExt_J3DModel__create(weapon_model_data, 0x80000, 0x11000084);
        if (_this->mpWeaponModel == NULL) {
            return 0;
        }
    }

    J3DModelData* eye_model_data = (J3DModelData*)dComIfG_getObjectRes(_this->mpArcName, 80);
    for (u16 i = 0; i < 2; i++) {
        _this->mpEyeModel[i] = mDoExt_J3DModel__create(eye_model_data, 0x80000, 0x11000084);
        if (_this->mpEyeModel[i] == NULL) {
            return 0;
        }
    }

    if (_this->field_0x5b8 == 12) {
        J3DModelData* grass_model_data = (J3DModelData*)dComIfG_getObjectRes("J_Tobi", 3);
        _this->mpHawkGrassModel = mDoExt_J3DModel__create(grass_model_data, 0x80000, 0x11000084);
        if (_this->mpHawkGrassModel == NULL) {
            return 0;
        }
    }

    return 1;
}

static cPhs__Step daE_RDY_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, e_rdy_class);
    e_rdy_class* _this = (e_rdy_class*)i_this;

    _this->field_0x5b6 = fopAcM_GetParam(_this) & 0xff;
    _this->field_0x5b7 = _this->field_0x5b6;
    _this->field_0x5b8 = (fopAcM_GetParam(_this) >> 0xc) & 0xf;
    if (_this->field_0x5b8 == 0xf) {
        _this->field_0x5b8 = 0;
    }

    _this->mpArcName = "E_rdy";
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&_this->mPhase, _this->mpArcName);

    if (step == cPhs_COMPLEATE_e) {
        if (_this->field_0x5b8 != 3) {
            int swbit = fopAcM_GetParam(_this) >> 0x18;
            if (swbit != 0xff && dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(i_this))) {
                return cPhs_ERROR_e;
            }
        }

        _this->mWeaponType = (fopAcM_GetParam(_this) >> 8) & 0xf;
        if (_this->mWeaponType == 0xff) {
            _this->mWeaponType = WEAPON_NONE;
        }
        _this->mSwBit = (fopAcM_GetParam(_this) >> 0x18) & 0xff;

        if (_this->field_0x5b8 == 3 || _this->field_0x5b8 == 4 || _this->field_0x5b8 == 5
            || _this->field_0x5b8 == 6 || _this->field_0x5b8 == 7)
        {
            if (_this->field_0x5b8 != 5) {
                _this->field_0x1365 = true;
            }
            if (_this->field_0x5b8 == 4) {
                _this->field_0x1366 = true;
            }
            if (_this->field_0x5b8 == 6) {
                _this->field_0x1368 = true;
                _this->mAction = ACT_COMMANDER;
                _this->mMode = 0;
                _this->mTimer[2] = 20;
                _this->field_0x1366 = true;
            } else if (_this->field_0x5b8 == 7) {
                _this->mAction = ACT_BOW3;
                _this->field_0x1366 = true;
                _this->mIFrameTimer = 20;
            } else {
                _this->mAction = ACT_BOW2;
            }
            fopAcM_OffStatus(i_this, fopAcStts_CULL_e);
            _this->field_0x5b8 = 3;
        } else if (_this->field_0x5b8 == 11) {
            _this->mAction = ACT_BOW_IKKI2;
        } else if (_this->field_0x5b8 == 12) {
                /* dSv_event_flag_c::M_051 - Main Event - Shadow Kargorok (?) (Large) event complete 
                                             (Horse grass appears in various places) */
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[84])) {
                return cPhs_ERROR_e;
            }
            cXyz pos = i_this->current.pos;
            _this->mAction = ACT_TKUSA;
            pos.y += 5000.0f;
            _this->mKargarokID = fpcM_ERROR_PROCESS_ID_e;
            fopAcM_create(PROC_E_YC, 0xffffff00, &pos, fopAcM_GetRoomNo(i_this),
                          &i_this->shape_angle, NULL, 0xff);
        }

        int tmp1 = i_this->home.angle.z & 0xff;
        if (tmp1 == 0 || tmp1 == 0xff) {
            _this->mPlayerCheckDist = 100000.0f;
        } else {
            _this->mPlayerCheckDist = tmp1 * 100.0f;
        }

        _this->mSwBit2 = (i_this->home.angle.z >> 8) & 0xff;
        if (_this->mSwBit2 == 0xff) {
            _this->mSwBit2 = 0;
        }

        fopAcM_OnStatus(i_this, fopAcStts_CULL_e);
        i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        u8 path_id = i_this->home.angle.x & 0xff;
        if (path_id != 0xff) {
            _this->mpPath = dPath_GetRoomPath(path_id, fopAcM_GetRoomNo(i_this));
            if (_this->mpPath == NULL) {
                return cPhs_ERROR_e;
            }
        }

        if (_this->field_0x5b6 == 7) {
            _this->mAction = ACT_NORMAL;
        }
        if (_this->mWeaponType == WEAPON_CLUB && _this->field_0x5b6 == 0) {
            _this->mAction = ACT_STAND;
            _this->mMode = 0;
        }
        if (_this->field_0x5b6 > 2) {
            _this->field_0x5b6 = 0;
        }

        i_this->shape_angle.x = 0;
        i_this->current.angle.x = 0;
        i_this->home.angle.x = 0;
        i_this->shape_angle.z = 0;
        i_this->current.angle.z = 0;
        i_this->home.angle.z = 0;

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x57D0)) {
            return cPhs_ERROR_e;
        }

        if (!hioInit) {
            _this->mHIOInit = true;
            hioInit = true;
            l_HIO.field_0x4 = -1;
        }

        fopAcM_SetMtx(i_this, _this->mpMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(i_this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(i_this, 200.0f, 200.0f, 200.0f);
        _this->mAcch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1,
                         &_this->mAcchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        _this->mAcchCir.SetWall(50.0f, 50.0f);

        i_this->health = 60;
        i_this->field_0x560 = 60;

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };

        static dCcD_SrcSph at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0x1d}, {0x0, 0x0}, 0x0}}, // mObj
                {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 35.0f} // mSph
            } // mSphAttr
        };

        _this->mCcStts.Init(200, 0, i_this);
        for (int i = 0; i <= 2; i++) {
            _this->mCcSph[i].Set(cc_sph_src);
            _this->mCcSph[i].SetStts(&_this->mCcStts);
            if (_this->field_0x1369) {
                _this->mCcSph[i].SetTgType(0xd8fbfdbf);
            }
        }
        _this->mAtSph.Set(at_sph_src);
        _this->mAtSph.SetStts(&_this->mCcStts);

        _this->mCounter = data_8077aa20 | ((u16)cM_rndF(0xFFFF) & 0xFF00);
        data_8077aa20++;

        _this->mSound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        _this->mSound.setEnemyName("E_rdy");
        _this->mAtInfo.mpSound = &_this->mSound;

        _this->field_0xa70 = 5;

        if (!strcmp(dComIfGp_getStartStageName(), "F_SP115")) {  // Lake Hylia
            fopAcM_OffStatus(i_this, 0x4000);
        }

        c_start = TRUE;
        daE_RDY_Execute(_this);
        c_start = FALSE;
    }

    return step;
}

e_rdy_class::e_rdy_class() {
    /* empty function */
}

AUDIO_INSTANCES

static actor_method_class l_daE_RDY_Method = {
    (process_method_func)daE_RDY_Create,
    (process_method_func)daE_RDY_Delete,
    (process_method_func)daE_RDY_Execute,
    (process_method_func)daE_RDY_IsDelete,
    (process_method_func)daE_RDY_Draw,
};

extern actor_process_profile_definition g_profile_E_RDY = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_RDY,             // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(e_rdy_class),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  157,                    // mPriority
  &l_daE_RDY_Method,      // sub_method
  0x00044000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
