/**
 * @file d_a_e_zm.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_zm.h"
#include "f_op/f_op_actor_enemy.h"

enum E_zm_RES_File_ID {
    /* BCK */
    /* 0x4 */ BCK_ZM_BIRI2 = 0x4,
    /* 0x5 */ BCK_ZM_DOWN,
    /* 0x6 */ BCK_ZM_MOUTHOPEN,

    /* BMDR */
    /* 0x9 */ BMDR_ZM = 0x9,
};

enum Joint {
    /* 0x0 */ JNT_CENTER,
    /* 0x1 */ JNT_TONGUE1,
    /* 0x2 */ JNT_TONGUE2,
    /* 0x3 */ JNT_TONGUE3,
    /* 0x4 */ JNT_TONGUE4,
    /* 0x5 */ JNT_TONGUE5,
};

enum Action {
    /* 0x0 */ ACTION_SEARCH_POINT,
    /* 0x1 */ ACTION_WAIT,
    /* 0x2 */ ACTION_MOVE,
    /* 0x3 */ ACTION_ATTACK,
    /* 0x4 */ ACTION_DAMAGE,
    /* 0x5 */ ACTION_DEAD,
    /* 0x6 */ ACTION_BULLET,
};

enum Mode {
    /* 0x0 */ MODE_0,
    /* 0x1 */ MODE_1,
    /* 0x2 */ MODE_2,
    /* 0x3 */ MODE_3,
    /* 0xA */ MODE_10 = 0xA,
    /* 0xB */ MODE_11,
    /* 0xC */ MODE_12,
};

class daE_ZM_HIO_c : public JORReflexible {
public:
    daE_ZM_HIO_c();
    virtual ~daE_ZM_HIO_c() {}

    void genMessage(JORMContext*);

    /* ザントの首 Zant's Head */
    /* 0x04 */ s8 id;
    /* 0x08 */ f32 model_size;                          // モデルサイズ - Model Size
    /* 0x0C */ f32 occurance_range;                     // 出現範囲 - Occurance Range
    /* 0x10 */ f32 bullet_speed;                        // 弾速度 - Bullet Speed
    /* 0x14 */ s16 wait_time_after_attack;              // 攻撃後の待ち時間 - Wait Time After Attack
    /* 0x16 */ s16 wait_time_before_attack;             // 攻撃前の待ち時間 - Wait Time Before Attack
    /* 0x18 */ s16 wait_time_to_appear_after_attack;    // 攻撃終了後次回出現までの待ち時間 - Wait Time To Appear After Attack
    /* 0x1A */ s16 stun_time;                           // 麻痺時間 - Stun Time
};

static u8 l_initHIO;

static daE_ZM_HIO_c l_HIO;

namespace {
    /* 80832DFC-80832E40 000038 0044+00 1/1 0/0 0/0 .data            cc_zm_src__22@unnamed@d_a_e_zm_cpp@ */
    static dCcD_SrcCyl cc_zm_src = {
        {
            {0, {{AT_TYPE_0, 0, 0}, {0xD8FBFDFF, 0x43}, 0x45}},
            {dCcD_SE_13, 0, 0, 0, 0},
            {dCcD_SE_NONE, 0, 0, 0, 0x2504},
            {0},
        },
        {
            {0.0f, 0.0f, 0.0f}, 50.0f, 300.0f,
        },
    };

    /* 80832E40-80832E80 00007C 0040+00 1/1 0/0 0/0 .data            cc_zm_at_src__22@unnamed@d_a_e_zm_cpp@ */
    static dCcD_SrcSph cc_zm_at_src = {
        {
            {0, {{AT_TYPE_CSTATUE_SWING, 2, 0xD}, {0xD8FBFDFF, 0x43}, 0}},
            {dCcD_SE_13, 0, 0, 0, 0},
            {dCcD_SE_NONE, 0, 0, 0, 2},
            {0}
        },
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f},
        },
    };
} // namespace

daE_ZM_HIO_c::daE_ZM_HIO_c() {
    id = -1;
    model_size = 2.0f;
    bullet_speed = 35.0f;
    occurance_range = 300.0f;
    wait_time_before_attack = 60;
    wait_time_after_attack = 60;
    stun_time = 200;
    wait_time_to_appear_after_attack = 100;
}

int daE_ZM_c::draw() {
    if (arg0 == 10) {
        return 1;
    }

    cXyz pos;

    if (arg0 == 20) {
        return 1;
    }
    
    if (mAction != ACTION_DEAD && arg0 < 10 && attention_info.distances[fopAc_attn_BATTLE_e] == 0) {
        return 1;
    }
    
    if (bitSw2 != 0xFF && !fopAcM_isSwitch(this, bitSw2)) {
        return 1;
    }

    J3DModel* model = mpModelMorf->getModel();
    g_env_light.settingTevStruct(2, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);

    J3DModelData* modelData = model->getModelData();
    for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
        J3DMaterial* matNodeP = modelData->getMaterialNodePointer(i);

        if (matNodeP != NULL) {
            matNodeP->getTevColor(0)->r = mColor[0];
            matNodeP->getTevColor(0)->g = mColor[1];
            matNodeP->getTevColor(0)->b = mColor[2];
        }
    }

    mpModelMorf->entryDL();

    if (field_0x6f4.y != l_HIO.model_size) {
        return 1;
    }

    pos.set(current.pos.x, current.pos.y + 10.0f, current.pos.z);
    dComIfGd_setSimpleShadow(&pos, mBgc.GetGroundH(), (BREG_F(18) + 70.0f) * l_HIO.model_size, mBgc.m_gnd, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());

    return 1;
}

static int daE_ZM_Draw(daE_ZM_c* i_this) {
    return i_this->draw();
}

void daE_ZM_c::setBck(int i_anm, u8 i_attr, f32 i_morf, f32 i_rate) {
    mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_ZM", i_anm), i_attr, i_morf, i_rate, 0.0f, -1.0f);
    mAnm = i_anm;
}

void daE_ZM_c::setActionMode(int i_action, int i_actionMode) {
    mAction = i_action;
    mMode = i_actionMode;
}

void daE_ZM_c::damage_check() {
    if (field_0x723 != 0) {
        return;
    }

    if (mAction == ACTION_BULLET) {
        return;
    }

    if (field_0x6f4.y != l_HIO.model_size || health <= 1) {
        return;
    }

    csXyz angle;
    cXyz sp2c, sp38;
    bool bVar1 = false;

    if (parentActorID != 0 && mAction != ACTION_DAMAGE) {
        daE_ZM_c* parent_p;
        if (fopAcM_SearchByID(parentActorID, (fopAc_ac_c**)&parent_p) != 0 && parent_p != NULL && parent_p->mMode >= 10) {
            sp2c = parent_p->current.pos - current.pos;

            if (sp2c.abs() < BREG_F(1) + 300.0f) {
                parent_p->health = 1;
                field_0x714 = 0.0f;
                mCyl.OffTgStopNoConHit();
                mCyl.ClrTgHit();
                setBck(BCK_ZM_BIRI2, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
                setActionMode(ACTION_DAMAGE, MODE_10);
                return;
            }
        }
    }

    cXyz i_effPos;
    i_effPos.set(current.pos);
    i_effPos.y += XREG_F(19) + 200.0f;
    scale.set(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    setMidnaBindEffect(this, &mSound, &i_effPos, &scale);

    if (mCyl.ChkTgHit()) {
        mAtInfo.mpCollider = mCyl.GetTgHitObj();
        field_0x723 = 10;
        sp2c = current.pos - *mCyl.GetTgHitPosP();
        sp38.set(*mCyl.GetTgHitPosP());
        angle.x = 0;
        angle.y = sp2c.atan2sX_Z();
        angle.z = 0;

        if (!mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_NORMAL_SWORD) && !mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_UNK)) {
            if (mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_IRON_BALL) || mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_HOOKSHOT) || mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_ARROW)) {
                def_se_set(&mSound, mAtInfo.mpCollider, 0x28, NULL);
                dComIfGp_setHitMark(2, this, &sp38, &angle, NULL, 0);
            }

            mStts.Move();
            return;
        }

        if (mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_MIDNA_LOCK)) {
            bVar1 = true;
            def_se_set(&mSound, mAtInfo.mpCollider, 31, 0);
            dComIfGp_setHitMark(3, this, &sp38, &angle, NULL, 0);
            health += -0x50;
        } else {
            cc_at_check(this, &mAtInfo);

            if (mAtInfo.mHitStatus == 0) {
                dComIfGp_setHitMark(1, this, &sp38, &angle, NULL, 0);
            } else {
                dComIfGp_setHitMark(3, this, &sp38, &angle, NULL, 0);
            }
        }

        if (health > 1) {
            field_0x72d++;

            if (bVar1 || field_0x72d < 3) {
                if (bVar1 || mCutTypeCheck()) {
                    field_0x723 = 8;
                    mCyl.ClrTgHit();
                    mCyl.OnTgStopNoConHit();
                    setActionMode(ACTION_DAMAGE, MODE_1);
                    return;
                } else {
                    setActionMode(ACTION_DAMAGE, MODE_0);
                }
            } else {
                mTimer = 0;
                setActionMode(ACTION_WAIT, MODE_2);
            }
        } else {
            setActionMode(ACTION_DEAD, MODE_0);
        }
    }

    mCyl.OffTgStopNoConHit();
    mStts.Move();
}

bool daE_ZM_c::mCutTypeCheck() {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    if (player->getCutCount() >= 4) {
        return true;
    }

    if (player->getCutType() == daPy_py_c::CUT_TYPE_TURN_RIGHT || player->getCutType() == daPy_py_c::CUT_TYPE_JUMP || player->getCutType() == daPy_py_c::CUT_TYPE_TWIRL ||
        player->getCutType() == daPy_py_c::CUT_TYPE_TURN_LEFT) {
        return true;
    }

    if (player->getCutType() == daPy_py_c::CUT_TYPE_LARGE_JUMP || player->getCutType() == daPy_py_c::CUT_TYPE_LARGE_JUMP_FINISH ||
        player->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT || player->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT ||
        player->getCutType() == daPy_py_c::CUT_TYPE_MORTAL_DRAW_A || player->getCutType() == daPy_py_c::CUT_TYPE_MORTAL_DRAW_B) {
        return true;
    }

    if (player->getCutType() == daPy_py_c::CUT_TYPE_HEAD_JUMP || player->getCutType() == daPy_py_c::CUT_TYPE_TWIRL) {
        return true;
    }

    return false;
}

static void* s_PointSearch(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_ZM && ((daE_ZM_c*)i_actor)->arg0 == 10 &&
        ((daE_ZM_c*)i_data)->arg2 == ((daE_ZM_c*)i_actor)->arg2) {
        ((daE_ZM_c*)i_data)->field_0x66c[((daE_ZM_c*)i_data)->field_0x6e5] = ((daE_ZM_c*)i_actor)->current.pos;
        ((daE_ZM_c*)i_data)->field_0x6e5++;

        if (((daE_ZM_c*)i_data)->field_0x6e5 >= 10) {
            ((daE_ZM_c*)i_data)->field_0x6e5 = 0;
        }
    }

    return NULL;
}

void daE_ZM_c::executeSearchPoint() {
    if (mTimer == 0) {
        field_0x6e5 = 0;
        fpcM_Search(s_PointSearch, this);
        setActionMode(ACTION_WAIT, MODE_0);
    }
}

void daE_ZM_c::executeWait() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz i_scale(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    cXyz sp44;

    if (mTimer != 0) {
        return;
    }

    switch (mMode) {
        case MODE_0: {
            if (arg0 == 1) {
                if (field_0x6e4 >= field_0x6e5) {
                    field_0x6e4 = 0;
                }

                current.pos.set(field_0x66c[field_0x6e4]);
                field_0x6e4++;
            } else if (arg0 == 2) {
                field_0x6e4 = cM_rndF(field_0x6e5);

                if (field_0x6e4 >= field_0x6e5) {
                    field_0x6e4 = 0;
                }

                current.pos.set(field_0x66c[field_0x6e4]);
            } else {
                sp44 = home.pos - player->current.pos;

                if (sp44.abs() > l_HIO.occurance_range) {
                    return;
                }

                current.pos.x = home.pos.x + cM_rndFX(l_HIO.occurance_range);
                current.pos.z = home.pos.z + cM_rndFX(l_HIO.occurance_range);
            }

            attention_info.distances[fopAc_attn_BATTLE_e] = 3;
            fopAcM_OnStatus(this, 0);
            attention_info.flags |= fopAc_AttnFlag_BATTLE_e;
            mCyl.OnTgSetBit();
            mCyl.OnCoSetBit();
            field_0x6f4.set(0.0f, l_HIO.model_size * 5.0f, 0.0f);
            sp44.set(current.pos);
            sp44.y += field_0x71c;
            mSound.startCreatureSound(Z2SE_EN_ZM_EMERGE, 0, -1);
            dComIfGp_particle_set(dPa_RM(ID_ZI_S_ZM_APP_A), &sp44, &shape_angle, &i_scale);
            
            s16 sVar1 = fopAcM_searchPlayerAngleY(this);
            current.angle.y = sVar1;
            shape_angle.y = sVar1;
            home.pos.set(current.pos);
            home.pos.y += 100.0f;
            setBck(BCK_ZM_MOUTHOPEN, J3DFrameCtrl::EMode_NONE, 3.0f, 0.0f);
            mpModelMorf->setFrame(0.0f);
            mMode = MODE_1;
            // fallthrough
        }
        case MODE_1:
            cLib_addCalc2(&field_0x6f4.x, l_HIO.model_size, 0.7f, 1.0f);
            cLib_addCalc2(&field_0x6f4.y, l_HIO.model_size, 0.8f, 2.0f);
            cLib_addCalc2(&field_0x6f4.z, l_HIO.model_size, 0.7f, 1.0f);

            if (fabsf(field_0x6f4.y - l_HIO.model_size) <= 0.1f) {
                field_0x6f4.set(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
                setActionMode(ACTION_MOVE, MODE_0);
            }
            break;
            
        case MODE_2:
            mStts.Move();
            mCyl.OffTgSetBit();
            mCyl.OffCoSetBit();
            field_0x72d = 0;
            field_0x714 = 0.0f;
            field_0x700.z = 0;
            field_0x700.x = 0;
            sp44.set(current.pos);
            sp44.y += field_0x71c;

            static u16 eff_delete_id[2] = {
                dPa_RM(ID_ZI_S_ZM_DISAPP_A),
                dPa_RM(ID_ZI_S_ZM_DISAPP_B),
            };

            for (int i = 0; i < 2; i++) {
                dComIfGp_particle_set(eff_delete_id[i], &sp44, &shape_angle, &i_scale);
            }

            mSound.startCreatureSound(Z2SE_EN_ZM_DISAPPER, 0, -1);
            mMode++;
            // fallthrough
        case MODE_3:
            cLib_addCalc0(&field_0x6f4.x, 0.7f, 1.0f);
            cLib_addCalc2(&field_0x6f4.y, l_HIO.model_size * 5.0f, 0.8f, 2.0f);
            cLib_addCalc0(&field_0x6f4.z, 0.7f, 1.0f);

            if (fabsf(field_0x6f4.y - l_HIO.model_size * 5.0f) <= 0.1f) {
                mTimer = l_HIO.wait_time_to_appear_after_attack;
                attention_info.distances[fopAc_attn_BATTLE_e] = 0;
                fopAcM_OffStatus(this, 0);
                attention_info.flags &= 0xFFFFFFFB;
                shape_angle.x = 0;
                field_0x6f4.set(0.0f, 0.0f, 0.0f);
                setActionMode(ACTION_WAIT, MODE_0);
            }
            break;
    }
}

void daE_ZM_c::executeMove() {
    switch (mMode) {
        case MODE_0:
            mCyl.SetCoVsGrp(16);
            setBck(BCK_ZM_MOUTHOPEN, J3DFrameCtrl::EMode_NONE, 3.0f, 0.0f);
            mpModelMorf->setFrame(0.0f);
            mMode = 1;
            // fallthrough
        case MODE_1:
            cLib_addCalcAngleS2(&current.angle.y, fopAcM_searchPlayerAngleY(this), 2, 0x600);

            if (field_0x722 == 0) {
                mTimer = l_HIO.wait_time_before_attack;
                setActionMode(ACTION_ATTACK, MODE_0);
            }
    }
}

void daE_ZM_c::executeAttack() {
    cXyz i_scale(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    J3DModel* model = mpModelMorf->getModel();
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp44, sp50;
    csXyz angle;
    cLib_addCalcAngleS2(&current.angle.y, fopAcM_searchPlayerAngleY(this), 2, 0x600);

    if (mTimer == 0) {
        switch (mMode) {
            case MODE_0:
                setBck(BCK_ZM_MOUTHOPEN, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
                mMode = MODE_1;
                break;
            
            case MODE_1:
                if ((int)mpModelMorf->getFrame() == 2) {
                    mSound.startCreatureSound(Z2SE_EN_ZM_PRE_ATK, 0, -1);
                }

                mDoMtx_stack_c::copy(model->getAnmMtx(JNT_CENTER));
                mDoMtx_stack_c::multVecZero(&sp44);

                if (mpModelMorf->isStop()) {
                    angle = shape_angle;
                    sp50.set(player->current.pos);
                    sp50.y += BREG_F(17) + 60.0f;
                    sp50 -= sp44;
                    angle.x = sp50.atan2sY_XZ();

                    parentActorID = fopAcM_createChild(PROC_E_ZM, fopAcM_GetID(this), 20, &sp44, fopAcM_GetRoomNo(this), &angle, NULL, -1, NULL);
                    dComIfGp_particle_set(dPa_RM(ID_ZI_S_ZM_HASSYA_A), &sp44, &shape_angle, &i_scale);
                    mSound.startCreatureSound(Z2SE_EN_ZM_BALL_OUT, 0, -1);
                    setBck(BCK_ZM_MOUTHOPEN, J3DFrameCtrl::EMode_NONE, 3.0f, -1.0f);
                    mMode = MODE_2;
                } else {
                    field_0x730[0] = dComIfGp_particle_set(field_0x730[0], dPa_RM(ID_ZI_S_ZM_TAME_A), &sp44, &shape_angle, &i_scale);
                }
                break;

            case MODE_2:
                if (mpModelMorf->isStop()) {
                    mTimer = l_HIO.wait_time_after_attack;
                    setActionMode(ACTION_WAIT, MODE_2);
                }
                break;
        }
    }
}

void daE_ZM_c::executeDamage() {
    switch (mMode) {
        case MODE_0:
        case MODE_1:
            if (mMode != MODE_1) {
                field_0x714 = 4000.0f;
            } else {
                field_0x714 = 12000.0f;
            }
            
            setBck(BCK_ZM_MOUTHOPEN, J3DFrameCtrl::EMode_NONE, 3.0f, 0.0f);
            // fallthrough
        case MODE_10:
            field_0x72b = 0;
            field_0x72c++;
            field_0x72c &= 1;
            field_0x718 = 500.0f;

            if (field_0x72c == 0) {
                field_0x718 = -500.0f;
            }

            mSound.startCreatureVoice(Z2SE_EN_ZM_V_DMG, -1);

            if (mMode != MODE_10) {
                mMode = MODE_2;
            } else {
                mMode++;
            }
            // fallthrough
        case MODE_11:
        case MODE_2:
            field_0x72b += 2;
            field_0x700.x = field_0x714 * cM_ssin(field_0x72b * field_0x718);
            field_0x700.z = field_0x714 * cM_scos(field_0x72b * field_0x718);
            cLib_addCalc0(&field_0x714, 0.8f, 200.0f);

            if (field_0x714 < 2.0f) {
                field_0x714 = 0.0f;
                field_0x700.z = 0;
                field_0x700.x = 0;

                if (mMode != MODE_11) {
                    mTimer = l_HIO.wait_time_after_attack;
                    setActionMode(ACTION_WAIT, MODE_2);
                } else {
                    mTimer = l_HIO.stun_time;
                    mMode++;
                }
            }
            break;
        
        case MODE_12:
            if (mTimer == 0) {
                setBck(BCK_ZM_MOUTHOPEN, J3DFrameCtrl::EMode_NONE, 3.0f, 0.0f);
                mTimer = l_HIO.wait_time_after_attack;
                setActionMode(ACTION_WAIT, MODE_2);
            }
            break;
    }
}

void daE_ZM_c::executeDead() {
    cXyz sp4c, sp58;
    csXyz angle;

    switch (mMode) {
        case MODE_0:
            setBck(BCK_ZM_MOUTHOPEN, J3DFrameCtrl::EMode_NONE, 3.0f, 0.0f);
            current.angle.y = fopAcM_searchPlayerAngleY(this) + 0x8000;
            speedF = JREG_F(3) + 40.0f;
            attention_info.distances[fopAc_attn_BATTLE_e] = 0;
            fopAcM_OffStatus(this, 0);
            attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
            
            mCyl.OffTgSetBit();
            mCyl.OffCoSetBit();

            mSound.startCreatureVoice(Z2SE_EN_ZM_V_DEAD, -1);
            field_0x72b = 0;
            field_0x722 = JREG_S(2) + 50;
            mMode++;
            break;

        case MODE_1: {
            shape_angle.y += (s16)(field_0x722 * (JREG_S(1) + 100));
            cLib_addCalc0(&speedF, 0.8f, 2.0f);
            field_0x72b += 10;
            field_0x700.x = field_0x714 * cM_ssin(field_0x72b * field_0x718);
            field_0x700.z = field_0x714 * cM_scos(field_0x72b * field_0x718);
            cLib_addCalc0(&field_0x714, 0.8f, 200.0f);
            cLib_addCalc2(&mColor[0], -20.0f, 0.3f, 2.0f);
            f32 fVar1 = mColor[0];
            mColor[2] = fVar1;
            mColor[1] = fVar1;

            if (field_0x722 == 0 && field_0x714 < 1.0f) {
                mMode++;
            }
            break;
        }
        
        case MODE_2: {
            mBgc.SetGroundUpY(BREG_F(2) + 10.0f);
            cLib_addCalc0(&field_0x71c, 0.9f, 30.0f);
            gravity = BREG_F(3) + -6.0f;
            cLib_addCalc2(&mColor[0], -20.0f, 0.3f, 2.0f);

            f32 fVar2 = mColor[0];
            mColor[2] = fVar2;
            mColor[1] = fVar2;

            if (mBgc.ChkGroundHit() || fabsf(current.pos.y - home.pos.y) > 500.0f) {
                gravity = 0.0f;
                speed.y = 0.0f;
                setBck(BCK_ZM_DOWN, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
                mMode++;
            }
            break;
        }

        case MODE_3:
            if ((int)mpModelMorf->getFrame() == 3) {
                mSound.startCreatureSound(Z2SE_EN_ZM_DROP, 0, -1);
            }

            if (mpModelMorf->isStop()) {
                if (bitSw != 0xFF) {
                    fopAcM_onSwitch(this, bitSw);
                }

                fopAcM_createDisappear(this, &current.pos, 8, 1, 20);
                fopAcM_delete(this);
            }
            break;
    }
}

void daE_ZM_c::executeBullet() {
    cXyz i_scale(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    cXyz sp38, sp44;

    switch (mMode) {
        case MODE_11:
            break;

        case MODE_0:
            speedF = l_HIO.bullet_speed;
            // fallthrough
        case MODE_10:
            cMtx_YrotS(*calc_mtx, current.angle.y);
            cMtx_XrotM(*calc_mtx, current.angle.x);
            sp38.x = 0.0f;
            sp38.y = 0.0f;
            sp38.z = speedF;
            MtxPosition(&sp38, &sp44);
            speed.y = sp44.y;
            field_0x722 = 250;
            mSph.OffTgSetBit();
            mSph.OffCoSetBit();
            mAcchCir.SetWall(20.0f, 20.0f);
            mMode++;
            // fallthrough
        case MODE_1:
            if (field_0x722 == 90) {
                mSph.OnTgSetBit();
                mSph.OnCoSetBit();
            }

            if (mSph.ChkAtShieldHit()) {
                daE_ZM_c* parent_p;
                if (fopAcM_SearchByID(parentActorID, (fopAc_ac_c**)&parent_p) != 0 && parent_p != NULL) {
                    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
                    mDoAud_seStart(Z2SE_EN_ZM_BALL_REFLECT, &current.pos, 0, 0);
                    sp44 = parent_p->current.pos;

                    if (player->getCutType() != daPy_py_c::CUT_TYPE_NONE) {
                        sp44.y += BREG_F(4) + 250.0f;
                    } else {
                        sp44.y -= BREG_F(4) + 250.0f;
                    }

                    sp44 -= current.pos;
                    current.angle.y = sp44.atan2sX_Z();

                    if (player->getCutType() == daPy_py_c::CUT_TYPE_NONE) {
                        if (cM_rnd() < 0.5f) {
                            current.angle.y += 0x4000;
                        } else {
                            current.angle.y -= 0x4000;
                        }
                    }

                    current.angle.x = sp44.atan2sY_XZ();
                    speedF = l_HIO.bullet_speed * 1.5f;
                    field_0x722 = 250;
                    mMode = MODE_10;
                }
            }
            break;
    }

    static u16 eff_bullet_id[3] = {
        dPa_RM(ID_ZI_S_ZM_BALL_A), dPa_RM(ID_ZI_S_ZM_BALL_B), dPa_RM(ID_ZI_S_ZM_BALL_C),
    };
    static u16 eff_bullet_delete_id[4] = {
        dPa_RM(ID_ZI_S_ZM_BALLHIT_A), dPa_RM(ID_ZI_S_ZM_BALLHIT_B), dPa_RM(ID_ZI_S_ZM_BALLHIT_C), dPa_RM(ID_ZI_S_ZM_BALLHIT_D),
    };

    if (mBgc.ChkWallHit() || mBgc.ChkGroundHit() || (!mSph.ChkAtShieldHit() && mSph.ChkAtHit()) || field_0x722 == 0 || health <= 1 || mSph.ChkTgHit()) {
        for (int i = 0; i < 4; i++) {
            dComIfGp_particle_set(eff_bullet_delete_id[i], &current.pos, &shape_angle, &i_scale);
        }

        mDoAud_seStart(Z2SE_EN_ZM_BALL_BURST, &current.pos, 0, 0);
        fopAcM_delete(this);
    } else {
        Z2GetAudioMgr()->seStartLevel(Z2SE_EN_ZM_BALL_FLY, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);

        for (int i = 0; i < 3; i++) {
            field_0x730[i] = dComIfGp_particle_set(field_0x730[i], eff_bullet_id[i], &current.pos, &shape_angle, &i_scale);
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(field_0x730[i]);

            if (emitter != NULL) {
                emitter->setGlobalTranslation(current.pos);
            }
        }

        mSph.SetC(current.pos);
        mSph.SetR(l_HIO.model_size * 30.0f);
        dComIfG_Ccsp()->Set(&mSph);
    }
}

void daE_ZM_c::action() {
    damage_check();

    switch (mAction) {
        case ACTION_SEARCH_POINT:
            executeSearchPoint();
            break;

        case ACTION_WAIT:
            executeWait();
            break;

        case ACTION_MOVE:
            executeMove();
            break;

        case ACTION_ATTACK:
            executeAttack();
            break;

        case ACTION_DAMAGE:
            executeDamage();
            break;

        case ACTION_DEAD:
            executeDead();
            break;

        case ACTION_BULLET:
            executeBullet();
            break;
    }

    if (mAnm == BCK_ZM_BIRI2) {
        mSound.startCreatureSoundLevel(Z2SE_EN_ZM_PARA, 0, -1);
    }

    if (mAction == ACTION_DEAD || arg0 >= 10 || attention_info.distances[fopAc_attn_BATTLE_e] != 0) {
        if (mAction != ACTION_DEAD) {
            field_0x720++;
            field_0x71c = (BREG_F(5) + 60.0f) * l_HIO.model_size + (BREG_F(6) + 20.0f) * cM_ssin(field_0x720 * 1000.0f);
            shape_angle.y = current.angle.y;
        }

        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        mBgc.CrrPos(dComIfG_Bgsp());

        if (arg0 != 20) {
            mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        }
    }
}

void daE_ZM_c::mtx_set() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::transM(0.0f, field_0x71c, 0.0f);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::ZXYrotM(field_0x700);
    mDoMtx_stack_c::scaleM(field_0x6f4.x, field_0x6f4.y, field_0x6f4.z);
    mpModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpModelMorf->modelCalc();
}

void daE_ZM_c::cc_set() {
    cXyz sp30, sp3c;
    J3DModel* model = mpModelMorf->getModel();

    if (mAction == ACTION_MOVE || mAction == ACTION_ATTACK) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        mDoMtx_stack_c::copy(model->getAnmMtx(JNT_CENTER));
        sp30.set(0.0f, BREG_F(7) + -60.0f, 0.0f);
        mDoMtx_stack_c::multVec(&sp30, &sp3c);
        sp3c = player->current.pos - sp3c;
        cLib_addCalcAngleS2(&shape_angle.x, sp3c.atan2sY_XZ(), XREG_S(3) + 2, XREG_S(4) + 0x300);
    }

    eyePos.set(current.pos);
    eyePos.y += (BREG_F(8) + 100.0f) * l_HIO.model_size;
    attention_info.position = eyePos;
    attention_info.position.y += (BREG_F(9) + 100.0f) * l_HIO.model_size;

    mDoMtx_stack_c::copy(model->getAnmMtx(JNT_CENTER));
    sp30.set(0.0f, -20.0f, 0.0f);
    mDoMtx_stack_c::multVec(&sp30, &sp3c);
    mCyl.SetC(sp3c);
    mCyl.SetH((BREG_F(10) + 140.0f) * l_HIO.model_size);
    mCyl.SetR((BREG_F(11) + 70.0f) * l_HIO.model_size);
    dComIfG_Ccsp()->Set(&mCyl);
}

int daE_ZM_c::execute() {
    if (arg0 == 10) {
        return 1;
    } 
    
    if (bitSw2 != 0xFF && !fopAcM_isSwitch(this, bitSw2)) {
        return 1;
    }

    if (field_0x722 != 0) {
        field_0x722--;
    }

    if (field_0x723 != 0) {
        field_0x723--;
    }

    if (field_0x724 != 0) {
        field_0x724--;
    }

    if (mTimer != 0) {
        mTimer--;
    }

    action();

    if (mAction != ACTION_DEAD && arg0 < 10 && attention_info.distances[fopAc_attn_BATTLE_e] == 0) {
        return 1;
    }

    if ((int)arg0 != 20) {
        mtx_set();
        cc_set();
    }

    return 1;
}

static int daE_ZM_Execute(daE_ZM_c* i_this) {
    return i_this->execute();
}

static int daE_ZM_IsDelete(daE_ZM_c* i_this) {
    return 1;
}

int daE_ZM_c::_delete() {
    dComIfG_resDelete(&mPhase, "E_ZM");

    if (arg0 == 10 || arg0 == 20) {
        return 1;
    }

    if (field_0xc28) {
        l_initHIO = 0;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (heap != NULL) {
        mSound.deleteObject();
    }

    return 1;
}

static int daE_ZM_Delete(daE_ZM_c* i_this) {
    return i_this->_delete();
}

int daE_ZM_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_ZM", BMDR_ZM);

    JUT_ASSERT(1558, modelData != NULL);

    mpModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_ZM", BCK_ZM_MOUTHOPEN), 2, 1.0f, 0, -1, &mSound, 0, 0x11000084);
    if (mpModelMorf == NULL || mpModelMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    return static_cast<daE_ZM_c*>(a_this)->CreateHeap();
}

cPhs__Step daE_ZM_c::create() {
    fopAcM_ct(this, daE_ZM_c);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhase, "E_ZM");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_ZM PARAM %x\n", fopAcM_GetParam(this));

        arg0 = fopAcM_GetParam(this);
        arg1 = fopAcM_GetParam(this) >> 8;
        arg2 = fopAcM_GetParam(this) >> 16;
        bitSw = fopAcM_GetParam(this) >> 24;
        bitSw2 = current.angle.z;
        shape_angle.z = 0;
        current.angle.z = 0;

        if (arg0 == 0xFF) {
            arg0 = 0;
        }

        if (arg2 == 0xFF) {
            arg2 = 0;
        }

        if (arg0 == 0 || arg0 == 1 || arg0 == 2) {
            if (bitSw != 0xFF && fopAcM_isSwitch(this, bitSw)) {
                return cPhs_ERROR_e;
            }
        } else {
            bitSw = 0xFF;
            bitSw2 = 0xFF;
        }

        OS_REPORT("argument %d\n", argument);
        OS_REPORT("arg0 %d\n", arg0);
        OS_REPORT("arg1 %d\n", arg1);
        OS_REPORT("arg2 %d\n", arg2);
        OS_REPORT("bitSw %x\n", bitSw);
        OS_REPORT("bitSw2 %x\n", bitSw2);

        if (arg0 == 10) {
            fopAcM_OffStatus(this, 0);
            attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
            attention_info.distances[fopAc_attn_BATTLE_e] = 0;
            fopAcM_SetGroup(this, 0);

            if (NREG_S(0) != 0) {
                fopAcM_SetMin(this, -20000.0f, -10000.0f, -10000.0f);
                fopAcM_SetMax(this, 20000.0f, 10000.0f, 10000.0f);
            }

            bitSw = 0xFF;
            bitSw2 = 0xFF;
        } else if (arg0 != 20 && !fopAcM_entrySolidHeap(this, useHeapInit, 0xA90)) {
            return cPhs_ERROR_e;
        } else {
            if (arg0 != 20 && l_initHIO == 0) {
                l_initHIO = 1;
                field_0xc28 = true;
                l_HIO.id = -1;
            }

            mStts.Init(254, 0, this);
            fopAcM_SetMin(this, -400.0f, -400.0f, -400.0f);
            fopAcM_SetMax(this, 400.0f, 400.0f, 400.0f);
            mBgc.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
            mAcchCir.SetWall(60.0f, 100.0f);

            health = 120;
            field_0x560 = 120;
            field_0x6f4.set(0.0f, l_HIO.model_size * 5.0f, 0.0f);

            for (int i = 0; i < 3; i++) {
                mColor[i] = 0.0f;
            }

            attention_info.distances[fopAc_attn_BATTLE_e] = 0;
            fopAcM_OffStatus(this, 0);
            attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;

            switch (arg0) {
                case 20:
                    fopAcM_SetGroup(this, 0);
                    bitSw = 0xFF;
                    bitSw2 = 0xFF;
                    mSph.Set(cc_zm_at_src);
                    mSph.SetStts(&mStts);
                    setActionMode(ACTION_BULLET, MODE_0);
                    break;

                case 1:
                case 2:
                    mTimer = 2;
                    setActionMode(ACTION_SEARCH_POINT, MODE_0);
                    // fallthrough
                default:
                    fopAcM_SetMtx(this, mpModelMorf->getModel()->getBaseTRMtx());
                    mCyl.Set(cc_zm_src);
                    mCyl.SetStts(&mStts);
                    mSound.init(&current.pos, &eyePos, 3, 1);
                    mSound.setEnemyName("E_zm");
                    mAtInfo.mpSound = &mSound;
                    mAtInfo.mPowerType = 2;
                    gravity = 0.0f;
                    mtx_set();

                    if (arg0 != 1 && arg0 != 2) {
                        setActionMode(ACTION_WAIT, MODE_0);
                    }
                    break;
            }
        }
    }

    return phase;
}

static int daE_ZM_Create(daE_ZM_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daE_ZM_Method = {
    (process_method_func)daE_ZM_Create,
    (process_method_func)daE_ZM_Delete,
    (process_method_func)daE_ZM_Execute,
    (process_method_func)daE_ZM_IsDelete,
    (process_method_func)daE_ZM_Draw,
};

extern actor_process_profile_definition g_profile_E_ZM = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_ZM,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daE_ZM_c),       // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  174,                    // mPriority
  &l_daE_ZM_Method,       // sub_method
  0x00040120,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
