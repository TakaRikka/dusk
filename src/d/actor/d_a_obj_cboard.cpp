/**
 * d_a_obj_cboard.cpp
 * Clear (Invisible) Wall
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_cboard.h"
#include "d/d_bg_w.h"
#include "d/d_procname.h"

void daObjBoard_c::initBaseMtx() {
    setBaseMtx();
}

void daObjBoard_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

static char* l_arcName = "Obj_Board";

static char* l_dzbName[5] = {
    "ClearB00.dzb", "ClearB01.dzb", "ClearB02.dzb", "ClearB03.dzb", "ClearB04.dzb",
};

static cull_box l_cull_box = {
    {-100.0f, -10.0f, -100.0f},
    {100.0f, 10.0f, 100.0f},
};

int daObjBoard_c::Create() {
    if (getNameArg() == 1) {
        scale.x *= 10.0f;
        scale.z *= 10.0f;
    }

    initBaseMtx();
    fopAcM_SetMtx(this, field_0x5a8);
    fopAcM_setCullSizeBox(this, l_cull_box.min.x * scale.x, l_cull_box.min.y,
                          l_cull_box.min.z * scale.z, l_cull_box.max.x * scale.x, l_cull_box.max.y,
                          l_cull_box.max.z * scale.z);
    return 1;
}

int daObjBoard_c::CreateHeap() {
    return 1;
}

int daObjBoard_c::create1st() {
    int ret = dComIfG_resLoad(&mPhase, l_arcName);
    if (ret == cPhs_COMPLEATE_e) {
        int dzb_no;

        switch (getArg0()) {
        case 1:
        case 2:
            dzb_no = 1;
            break;
        case 3:
        case 4:
            dzb_no = 2;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            dzb_no = 4;
            break;
        default:
            dzb_no = 0;
            break;
        }

        int dzb_id = dComIfG_getObjctResName2Index(l_arcName, l_dzbName[dzb_no]);
        ret = MoveBGCreate(l_arcName, dzb_id, NULL, 0x200, NULL);

        if (ret == cPhs_ERROR_e) {
            return ret;
        }
    }

    return ret;
}

int daObjBoard_c::Execute(Mtx** param_0) {
    switch (getArg0()) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 15:
        if (fopAcM_isSwitch(this, getSwNo())) {
            if (!mpBgW->ChkUsed()) {
                dComIfG_Bgsp().Regist(mpBgW, this);
            }
        } else {
            if (mpBgW->ChkUsed()) {
                dComIfG_Bgsp().Release(mpBgW);
            }
        }
        break;
    case 0:
    case 2:
    case 4:
    case 6:
    case 8:
        if (fopAcM_isSwitch(this, getSwNo())) {
            if (mpBgW->ChkUsed()) {
                dComIfG_Bgsp().Release(mpBgW);
            }
        } else {
            if (!mpBgW->ChkUsed()) {
                dComIfG_Bgsp().Regist(mpBgW, this);
            }
        }
        break;
    }

    *param_0 = &mBgMtx;
    return 1;
}

int daObjBoard_c::Draw() {
    return 1;
}

int daObjBoard_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daObjBoard_create1st(daObjBoard_c* i_this) {
    fopAcM_ct(i_this, daObjBoard_c);

    return i_this->create1st();
}

static int daObjBoard_MoveBGDelete(daObjBoard_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjBoard_MoveBGExecute(daObjBoard_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjBoard_MoveBGDraw(daObjBoard_c* i_this) {
    return i_this->Draw();
}

static actor_method_class daObjBoard_METHODS = {
    (process_method_func)daObjBoard_create1st,     (process_method_func)daObjBoard_MoveBGDelete,
    (process_method_func)daObjBoard_MoveBGExecute, (process_method_func)NULL,
    (process_method_func)daObjBoard_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_Cboard = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_Cboard,
    &g_fpcLf_Method.base,
    sizeof(daObjBoard_c),
    0,
    0,
    &g_fopAc_Method.base,
    453,
    &daObjBoard_METHODS,
    0x40000,
    fopAc_CULLBOX_0_e,
    fopAc_CULLBOX_CUSTOM_e,
};
