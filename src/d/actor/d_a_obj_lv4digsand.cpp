/**
 * @file d_a_obj_lv4digsand.cpp
 * Arbiter's Grounds Dig Sand
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv4digsand.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/d_item.h"
#include <cmath.h>

void daObjL4DigSand_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjL4DigSand_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjL4DigSand_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    
    attention_info.distances[fopAc_attn_ETC_e] = 31;
    mode_init_wait();
    return 1;
}

static char* l_arcName = "P_DSand";

int daObjL4DigSand_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    JUT_ASSERT(0x9B, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daObjL4DigSand_c::create1st() {
    if (fopAcM_isSwitch(this, getSwNo())) {
        return cPhs_ERROR_e;
    }

    int phase_state = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        phase_state = MoveBGCreate(l_arcName, 7, NULL, 0x1100, NULL);
        if (phase_state == cPhs_ERROR_e) {
            return phase_state;
        }
    }

    return phase_state;
}

int daObjL4DigSand_c::Execute(Mtx** param_0) {
    if (daPy_py_c::checkNowWolf() && checkItemGet(fpcNm_ITEM_SMELL_POH, 1)) {
        attention_info.flags = fopAc_AttnFlag_ETC_e;
    } else {
        attention_info.flags = 0;
    }

    action();

    *param_0 = &mBgMtx;
    setBaseMtx();
    return 1;
}

void daObjL4DigSand_c::action() {
    typedef void (daObjL4DigSand_c::*mode_func)();
    static mode_func l_func[] = {
        &daObjL4DigSand_c::mode_wait,
        &daObjL4DigSand_c::mode_dig,
        &daObjL4DigSand_c::mode_end,
    };

    (this->*l_func[mMode])();
}

void daObjL4DigSand_c::mode_init_wait() {
    mMode = 0;
}

void daObjL4DigSand_c::mode_wait() {
    if (field_0x941 == 1) {
        mode_init_dig();
    }
}

void daObjL4DigSand_c::mode_init_dig() {
    dBgS_ObjGndChk gndchk;
    gndchk.SetActorPid(base.base.id);
    gndchk.SetPos(&current.pos);

    f32 gnd_y = dComIfG_Bgsp().GroundCross(&gndchk);
    if (gnd_y != -G_CM3D_F_INF) {
        mGroundY = gnd_y;
    } else {
#if DEBUG
        // "No BG below dig-sand!"
        OS_REPORT_ERROR("掘れる砂の下にＢＧがありません！");
        fopAcM_delete(this);
#endif
    }

    mTimer = 20;
    mMode = 1;
}

void daObjL4DigSand_c::mode_dig() {
    if (cLib_calcTimer<u8>(&mTimer) == 0) {
        f32 var_f31 = std::fabs(((current.pos.y - mGroundY) + 10.0f) / 15.0f);
        if (var_f31 < 1.0f) {
            var_f31 = 1.0f;
        }

        if (cLib_chaseF(&current.pos.y, mGroundY - 10.0f, var_f31) && field_0x941 == 2) {
            mode_init_end();
        }
    }
}

void daObjL4DigSand_c::mode_init_end() {
    fopAcM_onSwitch(this, getSwNo());
    fopAcM_delete(this);
    mMode = 2;
}

void daObjL4DigSand_c::mode_end() {}

int daObjL4DigSand_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjL4DigSand_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daObjL4DigSand_create1st(daObjL4DigSand_c* i_this) {
    fopAcM_ct(i_this, daObjL4DigSand_c);
    return i_this->create1st();
}

static int daObjL4DigSand_MoveBGDelete(daObjL4DigSand_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjL4DigSand_MoveBGExecute(daObjL4DigSand_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjL4DigSand_MoveBGDraw(daObjL4DigSand_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjL4DigSand_METHODS = {
    (process_method_func)daObjL4DigSand_create1st,
    (process_method_func)daObjL4DigSand_MoveBGDelete,
    (process_method_func)daObjL4DigSand_MoveBGExecute,
    (process_method_func)NULL,
    (process_method_func)daObjL4DigSand_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_Lv4DigSand = {
  fpcLy_CURRENT_e,          // mLayerID
  3,                        // mListID
  fpcPi_CURRENT_e,          // mListPrio
  PROC_Obj_Lv4DigSand,      // mProcName
  &g_fpcLf_Method.base,    // sub_method
  sizeof(daObjL4DigSand_c), // mSize
  0,                        // mSizeOther
  0,                        // mParameters
  &g_fopAc_Method.base,     // sub_method
  551,                      // mPriority
  &daObjL4DigSand_METHODS,  // sub_method
  0x00044100,               // mStatus
  fopAc_ACTOR_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e,   // cullType
};
