/**
 * @file d_a_obj_bhashi.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_bhashi.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_b_mgn.h"
#include "d/d_s_play.h"

void daObjBHASHI_c::initCcCylinder() {
    const static dCcD_SrcCyl ccCylSrc = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0x100000, 0x1f}, 0x0}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {
                {0.0f, 0.0f, 0.0f}, // mCenter
                300.0f, // mRadius
                2200.0f // mHeight
            } // mCyl
        }
    };

    mCcStts.Init(0xFF, 0xFF, this);
    mCcCyl.Set(ccCylSrc);
    mCcCyl.SetStts(&mCcStts);
    mCcCyl.SetC(current.pos);
    mCcCyl.OnTgShield();
}

void daObjBHASHI_c::setCcCylinder() {
    cXyz pos;
    pos = current.pos;
    pos.y += -1100.0f;
    mCcCyl.SetC(pos);
    dComIfG_Ccsp()->Set(&mCcCyl);
}

static int daObjBHASHI_Create(fopAc_ac_c* i_this) {
    return ((daObjBHASHI_c*)i_this)->create();
}

static int daObjBHASHI_Delete(daObjBHASHI_c* i_this) {
    i_this->MoveBGDelete();
    return 1;
}

void daObjBHASHI_c::Set_Hahen() {
    csXyz sp8(0, 0, 0);

    if (mMode == 0) {
        cXyz sp10(current.pos.x, current.pos.y - 1100.0f, current.pos.z);
        sp8.y = field_0x11dc;
        Set_Speed(0, 60, 50.0f, 50.0f);
        mMode = 2;
        field_0x11e0 = 1000;

        if (mpBgW != NULL && mpBgW->ChkUsed() && dComIfG_Bgsp().Release(mpBgW)) {
            OS_REPORT("Release Error\n");
        }
    }
}

void daObjBHASHI_c::Set_Speed(s16 param_0, s16 i_hahenNum, f32 param_2, f32 param_3) {
    csXyz sp24;
    f32 var_f31;
    f32 var_f30;
    s16 rot_y;

    for (int i = param_0; i < i_hahenNum - 40; i++) {
        mHahen[i].pos.set(current.pos.x + cM_rndFX(80.0f),
                          350.0f + (current.pos.y - 1100.0f) + cM_rndFX(350.0f),
                          current.pos.z + cM_rndFX(80.0f));

        mHahen[i].scale.set((1.0f + XREG_F(0)) * (2.0f + cM_rndF(5.5f)),
                            (1.0f + XREG_F(0)) * (3.1f + cM_rndF(4.5f)),
                            (1.0f + XREG_F(0)) * (2.5f + cM_rndF(5.5f)));

        var_f30 = nREG_F(1) + (60.0f + (param_2 + cM_rndFX(param_3)));
        var_f31 = 30.0f + cM_rndFX(30.0f + nREG_F(2));
        rot_y = nREG_S(0) + (field_0x11dc + (s16)cM_rndFX(8192.0f));

        mHahen[i].speed.set(0.0f, 0.0f, var_f31);
        mDoMtx_stack_c::YrotS(rot_y);
        mDoMtx_stack_c::multVec(&mHahen[i].speed, &mHahen[i].speed);
        mHahen[i].speed.y = var_f31;

        mHahen[i].rot_speed.x = cM_rndFX(15000.0f);
        mHahen[i].rot_speed.y = cM_rndFX(15000.0f);
        mHahen[i].rotation.x = cM_rndFX(65536.0f);
        mHahen[i].rotation.y = cM_rndFX(65536.0f);
    }

    for (int i = i_hahenNum - 40; i < i_hahenNum - 20; i++) {
        mHahen[i].pos.set(current.pos.x + cM_rndFX(80.0f),
                          1050.0f + (current.pos.y - 1100.0f) + cM_rndFX(350.0f),
                          current.pos.z + cM_rndFX(80.0f));

        mHahen[i].scale.set((1.0f + XREG_F(0)) * (2.0f + cM_rndF(5.5f)),
                            (1.0f + XREG_F(0)) * (3.1f + cM_rndF(4.5f)),
                            (1.0f + XREG_F(0)) * (2.5f + cM_rndF(5.5f)));

        var_f30 = nREG_F(3) + (50.0f + cM_rndFX(60.0f));
        var_f31 = nREG_F(4) + (10.0f + cM_rndFX(20.0f));
        rot_y = nREG_S(0) + (field_0x11dc + (s16)cM_rndFX(8192.0f));

        mHahen[i].speed.set(0.0f, 0.0f, var_f31);
        mDoMtx_stack_c::YrotS(rot_y);
        mDoMtx_stack_c::multVec(&mHahen[i].speed, &mHahen[i].speed);
        mHahen[i].speed.y = var_f31;

        mHahen[i].rot_speed.x = cM_rndFX(15000.0f);
        mHahen[i].rot_speed.y = cM_rndFX(15000.0f);
        mHahen[i].rotation.x = cM_rndFX(65536.0f);
        mHahen[i].rotation.y = cM_rndFX(65536.0f);
    }

    for (int i = i_hahenNum - 20; i < i_hahenNum; i++) {
        mHahen[i].pos.set(current.pos.x + cM_rndFX(80.0f),
                          1750.0f + (current.pos.y - 1100.0f) + cM_rndFX(350.0f),
                          current.pos.z + cM_rndFX(80.0f));

        mHahen[i].scale.set(2.0f + cM_rndF(5.5f),
                            3.1f + cM_rndF(4.5f),
                            2.5f + cM_rndF(5.5f));

        var_f30 = nREG_F(3) + (20.0f + cM_rndFX(20.0f));
        var_f31 = nREG_F(4) + (10.0f + cM_rndFX(20.0f));
        rot_y = (s16)cM_rndFX(32000.0f);

        f32 var_f29 = var_f30 * cM_ssin(rot_y);
        f32 var_f28 = var_f31 + gravity;
        f32 var_f27 = var_f30 * cM_scos(rot_y);

        mHahen[i].speed.set(var_f29, var_f28, var_f27);

        mHahen[i].rot_speed.x = cM_rndFX(15000.0f);
        mHahen[i].rot_speed.y = cM_rndFX(15000.0f);
        mHahen[i].rotation.x = cM_rndFX(65536.0f);
        mHahen[i].rotation.y = cM_rndFX(65536.0f);
    }
}

void daObjBHASHI_c::Obj_Damage() {
    if (mDamageTimer == 0) {
        mCcStts.Move();

        if (mCcCyl.ChkTgHit()) {
            cXyz& player_pos = fopAcM_GetPosition(daPy_getPlayerActorClass());
            mDamageTimer = 10;

            fopAc_ac_c* tg_hit_obj = dCc_GetAc(mCcCyl.GetTgHitObj()->GetAc());
            cXyz* tg_hit_pos = mCcCyl.GetTgHitPosP();
            s16 rot_y = cLib_targetAngleY(&current.pos, tg_hit_pos);
            if (fopAcM_GetName(tg_hit_obj) == PROC_B_MGN) {
                field_0x5a8++;
                
                fpc_ProcID tg_hit_id = fopAcM_GetID(tg_hit_obj);
                fopAc_ac_c* mgn_p;
                if (fopAcM_SearchByName(PROC_B_MGN, &mgn_p) && mgn_p != NULL) {
                    field_0x11dc = cLib_targetAngleY(&fopAcM_GetPosition(mgn_p), &current.pos);
                }

                Z2GetAudioMgr()->seStart(Z2SE_OBJ_MGN_BREAK_PILLAR, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                Set_Hahen();
                dComIfGp_getVibration().StartShock(7, 1, cXyz(0.0f, 1.0f, 0.0f));

                if (mSwbit != 0xFF && !dComIfGs_isSwitch(mSwbit, fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(mSwbit, fopAcM_GetRoomNo(this));
                }

                csXyz eff_rot(0, rot_y, 0);
                cXyz eff_pos(current.pos.x, current.pos.y - 1100.0f, current.pos.z);
                dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV9PILLAR_BREAK_A), &eff_pos, &tevStr, &eff_rot, NULL);
                dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV9PILLAR_BREAK_B), &eff_pos, &tevStr, &eff_rot, NULL);
                dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV9PILLAR_BREAK_C), &eff_pos, &tevStr, &eff_rot, NULL);
                dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV9PILLAR_BREAK_D), &eff_pos, &tevStr, &eff_rot, NULL);
            }

            mCcCyl.ResetTgHit();
            fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x100);
        }
    }
}

void daObjBHASHI_c::Obj_Damage(cXyz param_0) {
    field_0x11dc = cLib_targetAngleY(&param_0, &current.pos);

    Z2GetAudioMgr()->seStart(Z2SE_OBJ_MGN_BREAK_PILLAR, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    Set_Hahen();
    dComIfGp_getVibration().StartShock(7, 1, cXyz(0.0f, 1.0f, 0.0f));

    if (mSwbit != 0xFF && !dComIfGs_isSwitch(mSwbit, fopAcM_GetRoomNo(this))) {
        dComIfGs_onSwitch(mSwbit, fopAcM_GetRoomNo(this));
    }

    csXyz eff_rot(0, field_0x11dc, 0);
    cXyz eff_pos(current.pos.x, current.pos.y - 1100.0f, current.pos.z);
    dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV9PILLAR_BREAK_A), &eff_pos, &tevStr, &eff_rot, NULL);
    dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV9PILLAR_BREAK_B), &eff_pos, &tevStr, &eff_rot, NULL);
    dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV9PILLAR_BREAK_C), &eff_pos, &tevStr, &eff_rot, NULL);
    dComIfGp_particle_set(dPa_RM(ID_ZI_S_LV9PILLAR_BREAK_D), &eff_pos, &tevStr, &eff_rot, NULL);

    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x100);
}

Hahen_c::Hahen_c() {
    field_0x31 = 0;
    status = 0;
}

void Hahen_c::Wall_Check() {
    dBgS_LinChk line_chk;
    cXyz end_pos(pos.x + speed.x, pos.y, pos.z + speed.z);
    cXyz start_pos(pos.x, pos.y, pos.z);

    line_chk.Set(&start_pos, &end_pos, NULL);
    if (dComIfG_Bgsp().LineCross(&line_chk) == true) {
        speed.x = -(0.05f + nREG_F(18)) * speed.x;
        speed.z = -(0.05f + nREG_F(18)) * speed.z;
    }
}

void Hahen_c::HahenMotion() {
    if (status == 0) {
        f32 temp_f31 = 5.0f + nREG_F(16);
        Wall_Check();

        if (pos.y >= 10.0f + temp_f31) {
            speed.y += -9.0f;

            pos.x += speed.x;
            pos.y += speed.y;
            pos.z += speed.z;

            rotation.x += rot_speed.x;
            rotation.y += rot_speed.y;
        }

        if (pos.y < 10.0f + temp_f31) {
            f32 temp_f30 = speed.y;
            pos.y = 10.0f + temp_f31;

            speed.y = temp_f30 * -(1.0f - (scale.abs() / (15.0f + XREG_F(10))));
            speed.x *= 0.7f + cM_rndF(0.3f);
            speed.z *= 0.7f + cM_rndF(0.3f);

            if (speed.y < 4.0f) {
                status = 1;
            }

            rot_speed.x = cM_rndFX(5000.0f);
            rot_speed.y = cM_rndFX(5000.0f);
        }
    } else if (status == 1) {
        if (field_0x31 != 0) {
            status = 2;
        }

        if (CheckCull()) {
            status = 2;

            if (scale.abs() > 8.0f + XREG_F(11)) {
                scale *= 0.6f + XREG_F(9);
            }

            field_0x31 = 1;
        }
    }
}

bool Hahen_c::CheckCull() {
    if (pos.absXZ(fopAcM_GetPosition(daPy_getPlayerActorClass())) > 1000.0f && !checkViewArea()) {
        return false;
    }

    return true;
}

bool Hahen_c::checkViewArea() {
    Vec proj;
    mDoLib_project(&pos, &proj);
    return (proj.x >= -50.0f && proj.x <= 658.0f) && (proj.y >= -50.0f && proj.y <= 498.0f);
}

void daObjBHASHI_c::Rolling(cXyz* i_pos, f32 i_range, f32 param_2) {
    fopAcM_GetPosition(daPy_getPlayerActorClass());
    if (mMode == 2) {
        for (s16 i = 0; i < 60; i++) {
            if (i_pos->abs(mHahen[i].pos) < i_range) {
                mHahen[i].Roll_Set(i_pos, param_2, i);
                mHahen[i].status = 0;
            }
        }
    }
}

void Hahen_c::Roll_Set(cXyz* i_pos, f32 param_1, s16 i_no) {
    f32 temp_f30 = param_1 * (2.0f + nREG_F(10) + cM_rndFX(0.2f));
    s16 angle = cLib_targetAngleY(&pos, i_pos);

    f32 speed_x = (-1.0f + nREG_F(12)) * param_1 * cM_ssin(angle);
    f32 temp_f31 = scale.abs();
    temp_f31 = 3.0f / temp_f31;
    
    f32 speed_y = temp_f30 - 9.0f;
    f32 speed_z = (-1.0f + nREG_F(13)) * param_1 * cM_scos(angle);

    speed.set(temp_f31 * speed_x, temp_f31 * speed_y, temp_f31 * speed_z);

    rot_speed.x = cM_rndFX(5000.0f + nREG_F(6));
    rot_speed.y = cM_rndFX(5000.0f + nREG_F(6));
    rotation.x += (s16)0x1000;
    rotation.y += (s16)0x1000;
}

void daObjBHASHI_c::setBaseMtx() {
    if (mMode == 2) {
        for (s16 i = 0; i < 60; i++) {
            mDoMtx_stack_c::transS(mHahen[i].pos);
            mDoMtx_stack_c::ZXYrotM(mHahen[i].rotation);
            mDoMtx_stack_c::scaleM(mHahen[i].scale);
            mpHahenMdls[i]->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    } else {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mpPillarMdl->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    mDoMtx_stack_c::transM(0.0f, -1100.0f + yREG_F(10), 0.0f);
    cMtx_copy(mDoMtx_stack_c::get(), mBgMtx);
}

static int daObjBHASHI_Draw(daObjBHASHI_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daObjBHASHI_Execute(daObjBHASHI_c* i_this) {
    return i_this->MoveBGExecute();
}

static char* l_arcName = "B_Hashi";

int daObjBHASHI_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "Lv9_R07_pillar.bmd");
    JUT_ASSERT(98, modelData != NULL);

    J3DModelData* modelData2 = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "Lv9_R07_pillar_hahen.bmd");
    JUT_ASSERT(102, modelData2 != NULL);

    for (int i = 0; i < 60; i++) {
        mpHahenMdls[i] = mDoExt_J3DModel__create(modelData2, J3DMdlFlag_UseSharedDL, BMD_DEFAULT_DIFF_FLAGS);
        if (mpHahenMdls[i] == NULL) {
            return 0;
        }
    }

    mpPillarMdl = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, BMD_DEFAULT_DIFF_FLAGS);
    if (mpPillarMdl == NULL) {
        return 0;
    }

    return 1;
}

int daObjBHASHI_c::create() {
    fopAcM_ct(this, daObjBHASHI_c);

    int phase_state = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        gravity = -9.0f + nREG_F(0);

        int dzb_id = dComIfG_getObjctResName2Index(l_arcName, "Lv9_R07_pillar.dzb");
        if (dzb_id == -1) {
            // "dzb data not found!<%s>"
            OS_REPORT("dzbデータが見つかりませんでした!<%s>\n\n", l_arcName);
        }

        JUT_ASSERT(1192, dzb_id != -1);
        phase_state = MoveBGCreate(l_arcName, dzb_id, dBgS_MoveBGProc_TypicalRotY, 0x8DA0, NULL);
        if (phase_state == cPhs_ERROR_e) {
            return phase_state;
        }

        initCcCylinder();
        mMode = 0;
        fopAcM_SetMtx(this, mpPillarMdl->getBaseTRMtx());

        cXyz sp8(current.pos.x, 100.0f + current.pos.y, current.pos.z);
        if (fopAcM_gc_c::gndCheck(&sp8)) {
            current.pos.y = fopAcM_gc_c::getGroundY();
            current.pos.y += 1100.0f;
        }

        fopAcM_setCullSizeBox(this, -500.0f, 0.0f, -500.0f, 500.0f, 3000.0f, 500.0f);

        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "Lv9_R07_pillar_hahen.bmd");
        JUT_ASSERT(1219, modelData != NULL);

        if (dComIfGp_addSimpleModel(modelData, fopAcM_GetRoomNo(this), 0) == -1) {
            // "Fire Demon Room Pillar Fragment: simple model entry failed."
            OS_REPORT("\x1B[43;30m火魔神部屋の柱の破片：シンプルモデル登録失敗しました。\n\x1B[m");
        }

        mIsHahenSimpleEntry = TRUE;
        daObjBHASHI_Execute(this);
    }

    return phase_state;
}

static int daObjBHASHI_IsDelete(daObjBHASHI_c* i_this) {
    return 1;
}

int daObjBHASHI_c::Create() {
    fopAcM_setCullSizeBox(this, -1000.0f, -500.0f, -1000.0f, 1000.0f, 500.0f, 1000.0f);
    return cPhs_COMPLEATE_e;
}

int daObjBHASHI_c::Execute(Mtx** param_0) {
    if (mMode != 2) {
        Obj_Damage();
    }

    if (mMode == 2) {
        fopAcM_GetPosition(daPy_getPlayerActorClass());

        field_0x11e0--;
        if (field_0x11e0 <= 0) {
            field_0x11e0 = 0;
        }

        for (s16 i = 0; i < 60; i++) {
            mHahen[i].HahenMotion();
        }

        fopAc_ac_c* mgn_p;
        if (fopAcM_SearchByName(PROC_B_MGN, &mgn_p) && mgn_p != NULL && ((daB_MGN_c*)mgn_p)->isAppear()) {
            Rolling(&fopAcM_GetPosition(mgn_p), 400.0f, 90.0f + yREG_F(0));
        }
    }

    *param_0 = &mBgMtx;
    setBaseMtx();

    if (mMode != 2) {
        setCcCylinder();
    }

    if (mDamageTimer != 0) {
        mDamageTimer--;
    }

    return 1;
}

int daObjBHASHI_c::Draw() {
    g_env_light.settingTevStruct(8, &current.pos, &tevStr);

    if (mMode == 0) {
        g_env_light.setLightTevColorType_MAJI(mpPillarMdl, &tevStr);
    }

    dComIfGd_setListBG();

    if (mMode == 0) {
        mDoExt_modelUpdateDL(mpPillarMdl);
    } else {
        int roomNo = fopAcM_GetRoomNo(this);
        for (int i = 0; i < 60; i++) {
            dComIfGp_entrySimpleModel(mpHahenMdls[i], roomNo);
        }
    }

    dComIfGd_setList();
    return 1;
}

int daObjBHASHI_c::Delete() {
    if (mIsHahenSimpleEntry) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "Lv9_R07_pillar_hahen.bmd");
        dComIfGp_removeSimpleModel(modelData, fopAcM_GetRoomNo(this));
    }

    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static actor_method_class l_daObjBHASHI_Method = {
    (process_method_func)daObjBHASHI_Create,
    (process_method_func)daObjBHASHI_Delete,
    (process_method_func)daObjBHASHI_Execute,
    (process_method_func)daObjBHASHI_IsDelete,
    (process_method_func)daObjBHASHI_Draw,
};

actor_process_profile_definition g_profile_Obj_BHASHI = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_BHASHI,        // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjBHASHI_c),  // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  463,                    // mPriority
  &l_daObjBHASHI_Method,  // sub_method
  0x00044000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
