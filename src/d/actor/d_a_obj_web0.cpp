/**
 * d_a_obj_web0.cpp
 * Spider Web (Wall)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_web0.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_bg_w.h"
#include "d/d_s_play.h"
#include "f_op/f_op_actor_mng.h"
#include "global.h"

daObj_Web0_HIO_c::daObj_Web0_HIO_c() {
    field_0x4 = -1;
}

static int daObj_Web0_Draw(obj_web0_class* i_this) {
    g_env_light.settingTevStruct(16, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mpModel, &i_this->tevStr);
    i_this->mpBrk->entry(i_this->mpModel->getModelData());

    mDoExt_modelUpdateDL(i_this->mpModel);
    return 1;
}

static void damage_check(obj_web0_class* i_this) {
    i_this->mStts.Move();

    if (i_this->mHitTimer != 0) {
        return;
    }

    if (i_this->mSphCc.ChkTgHit()) {
        i_this->mHitTimer = 6;

        if (i_this->mSphCc.GetTgHitObj()->ChkAtType(AT_TYPE_IRON_BALL)) {
            i_this->mDeleteTimer = 41;
            return;
        }

        if (i_this->mSphCc.GetTgHitObj()->ChkAtType(AT_TYPE_LANTERN_SWING) &&
            i_this->mSphCc.GetTgHitGObj()->GetAtMtrl() != dCcD_MTRL_FIRE) {
            return;
        }

        if (i_this->mSphCc.GetTgHitObj()->ChkAtType(AT_TYPE_BOMB) ||
            i_this->mSphCc.GetTgHitGObj()->GetAtMtrl() == dCcD_MTRL_FIRE) {
            i_this->mDeleteTimer = 1;
            return;
        }

        i_this->mReboundTimer = 20;
        fopAcM_seStart(i_this, Z2SE_OBJ_WEB_BOUND_L, 0);
    }

    if (i_this->mSphCc.ChkCoHit()) {
        i_this->mReboundTimer = 10;
    }

    if (daPy_getPlayerActorClass()->checkFrontRollCrash() &&
        fopAcM_searchPlayerDistanceXZ(i_this) < i_this->scale.x * 260.0f) {
        i_this->mReboundTimer = 20;
        fopAcM_seStart(i_this, Z2SE_OBJ_WEB_BOUND_L, 0);
    }
}

static int daObj_Web0_Execute(obj_web0_class* i_this) {
    fopAc_ac_c* base_p = i_this;
    fopAc_ac_c* player = (fopAc_ac_c*) dComIfGp_getPlayer(0);

    i_this->field_0x57c++;

    for (int i = 0; i < 2; i++) {
        if (i_this->field_0x584[i] != 0) {
            i_this->field_0x584[i]--;
        }
    }

    if (i_this->mHitTimer != 0) {
        i_this->mHitTimer--;
    }

    if (i_this->mDeleteTimer != 0) {
        if (i_this->mDeleteTimer == 1) {
            cXyz sp20(base_p->scale);
            sp20.z = 1.0f;

            dComIfGp_particle_set(0x840C, &base_p->current.pos, &base_p->shape_angle, &sp20);
            i_this->mpBrk->setPlaySpeed(1.0f);
        } else if (i_this->mDeleteTimer == 41) {
            i_this->mpBrk->setPlaySpeed(1.0f);
        }

        fopAcM_seStartLevel(base_p, Z2SE_OBJ_WEB_BURN, 0);

        if (i_this->mDeleteTimer == 40 || i_this->mDeleteTimer == 80) {
            u32 sp0C = (fopAcM_GetParam(base_p) & 0xff000000) >> 24;
            dComIfGs_onSwitch(sp0C, fopAcM_GetRoomNo(base_p));
            fopAcM_delete(base_p);
        }

        i_this->mDeleteTimer++;
    } else {
        damage_check(i_this);
    }

    mDoMtx_stack_c::transS(base_p->current.pos.x, base_p->current.pos.y, base_p->current.pos.z);
    mDoMtx_stack_c::YrotM((s16)base_p->shape_angle.y);
    mDoMtx_stack_c::ZrotM((s16)base_p->shape_angle.z);
    mDoMtx_stack_c::scaleM(base_p->scale.x, base_p->scale.y, base_p->scale.z);

    i_this->mpBrk->play();
    i_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    if (i_this->mReboundTimer != 0) {
        i_this->mReboundTimer--;
    }

    base_p->scale.z =
        i_this->mReboundTimer * cM_ssin(i_this->mReboundTimer * 0x1900) * (0.075f + TREG_F(0));

    s16 var_r28 = fopAcM_searchPlayerAngleY(base_p);
    s16 sp08 = (var_r28 + 0x4000) - base_p->shape_angle.y;
    if (sp08 < 0) {
        mDoMtx_stack_c::YrotM(-0x8000);
    }
    MTXCopy(mDoMtx_stack_c::get(), i_this->mMtx);

    i_this->mpBgW->Move();

    cXyz sp14(base_p->current.pos);
    if (i_this->field_0x57c & 1) {
        sp14.y -= base_p->scale.x * 70.0f;
    }

    var_r28 = base_p->shape_angle.y;
    if (sp08 < 0) {
        var_r28 += (s16) -0x8000;
    }

    var_r28 -= player->shape_angle.y;
    if ((var_r28 < 20000 && var_r28 > -20000) || i_this->mHitTimer != 0) {
        sp14.x += 20000.0f;
    }

    i_this->mSphCc.SetC(sp14);
    i_this->mSphCc.SetR((150.0f + TREG_F(2)) * base_p->scale.x);

    dComIfG_Ccsp()->Set(&i_this->mSphCc);
    return 1;
}

static int daObj_Web0_IsDelete(obj_web0_class* i_this) {
    return 1;
}

static u8 data_80D35290;

static int daObj_Web0_Delete(obj_web0_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "Obj_web0");

    if (i_this->field_0x739) {
        data_80D35290 = 0;
    }

    dComIfG_Bgsp().Release(i_this->mpBgW);
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    obj_web0_class* _this = static_cast<obj_web0_class*>(i_this);

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_web0", 5);
    _this->mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);

    if (_this->mpModel == NULL) {
        return 0;
    }

    _this->mpBrk = new mDoExt_brkAnm();
    if (_this->mpBrk == NULL) {
        return 0;
    }

    J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes("Obj_web0", 8);
    if (!_this->mpBrk->init(_this->mpModel->getModelData(), brk, TRUE, 0, 1.0f, 0,
                            -1)) {
        return 0;
    }

    _this->mpBrk->setPlaySpeed(0.0f);

    _this->mpBgW = new dBgW();
    if (_this->mpBgW == NULL) {
        return 0;
    }

    cBgD_t* dzb = (cBgD_t*)dComIfG_getObjectRes("Obj_web0", 11);
    if (_this->mpBgW->Set(dzb, 1, &_this->mMtx) == 1) {
        return 0;
    }

    _this->mpBgW->SetCrrFunc(dBgS_MoveBGProc_Typical);
    return 1;
}

static daObj_Web0_HIO_c l_HIO;

static int daObj_Web0_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, obj_web0_class);
    obj_web0_class* _this = static_cast<obj_web0_class*>(i_this);

    static dCcD_SrcSph cc_sph_src = {
        {
            {0, {{0, 0, 0}, {0xD8FBFFFF, 3}, 0}},
            {dCcD_SE_NONE, 0, 0, 0, {0}},
            {dCcD_SE_HARD_BODY, 0, 0, 0, {3}},
            {0},
        },
        {
            {
                {0.0f, 0.0f, 0.0f},
                40.0f,
            },
        }
    };

    int phase = dComIfG_resLoad(&_this->mPhase, "Obj_web0");
    if (phase == cPhs_COMPLEATE_e) {
        int switchNo = fopAcM_GetParam(_this) >> 0x18;

        if (switchNo != 0xFF && dComIfGs_isSwitch(switchNo, fopAcM_GetRoomNo(_this))) {
            return cPhs_ERROR_e;
        }

        _this->mPrm0 = fopAcM_GetParam(_this);
        if (_this->mPrm0 == 0xFF) {
            _this->mPrm0 = 0;
        }

        _this->mScaleXY = fopAcM_GetParam(_this) >> 8;

        if (!fopAcM_entrySolidHeap(_this, (heapCallbackFunc)useHeapInit, 0xA20)) {
            return cPhs_ERROR_e;
        }

        if (_this->mpBgW != NULL && dComIfG_Bgsp().Regist(_this->mpBgW, _this)) {
            return cPhs_ERROR_e;
        }

        if (!data_80D35290) {
            _this->field_0x739 = 1;
            data_80D35290 = true;
            l_HIO.field_0x4 = -1;
        }

        fopAcM_SetMtx(_this, _this->mpModel->getBaseTRMtx());
        fopAcM_SetMin(_this, -1000.0f, -1000.0f, -1000.0f);
        fopAcM_SetMax(_this, 1000.0f, 1000.0f, 1000.0f);
        _this->health = 30;
        _this->field_0x560 = 30;

        _this->mStts.Init(250, 0, _this);
        _this->mSphCc.Set(cc_sph_src);
        _this->mSphCc.SetStts(&_this->mStts);
        _this->mSphCc.OnTgNoHitMark();

        if (_this->mScaleXY != 0xFF && _this->mScaleXY != 0) {
            _this->scale.x = _this->mScaleXY;
            _this->scale.y = _this->mScaleXY;
        }

        _this->eyePos = _this->current.pos;
        daObj_Web0_Execute(_this);
    }

    return phase;
}

static actor_method_class l_daObj_Web0_Method = {
    (process_method_func)daObj_Web0_Create,  (process_method_func)daObj_Web0_Delete,
    (process_method_func)daObj_Web0_Execute, (process_method_func)daObj_Web0_IsDelete,
    (process_method_func)daObj_Web0_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_WEB0 = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_OBJ_WEB0,
    &g_fpcLf_Method.base,
    sizeof(obj_web0_class),
    0,
    0,
    &g_fopAc_Method.base,
    47,
    &l_daObj_Web0_Method,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
