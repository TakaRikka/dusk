/**
 * d_a_obj_zra_rock.cpp
 * Zora Rock (Graveyard)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_zra_rock.h"
#include "d/d_com_inf_game.h"
#include "d/d_bg_w.h"
#include "d/d_procname.h"

static char const* l_arcName = "H_ZraRock";

void daObjZraRock_c::setAttnPos() {
    cXyz vec(10.0f, 152.3f, 0.0f);
    mDoMtx_stack_c::ZXYrotS(current.angle.x, current.angle.y, current.angle.z);
    mDoMtx_stack_c::multVec(&vec, &attention_info.position);
    attention_info.position += current.pos;
}

void daObjZraRock_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mpModelOpa->setBaseTRMtx(mDoMtx_stack_c::get());
    mpModelXlu->setBaseTRMtx(mDoMtx_stack_c::get());
    PSMTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

void daObjZraRock_c::setEnvTevColor() {
    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
    tevStr.room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
}

void daObjZraRock_c::setRoomNo() {
    fopAcM_SetRoomNo(this, dComIfG_Bgsp().GetRoomId(mGndChk));
}

int daObjZraRock_c::Create() {
    setBaseMtx();
    fopAcM_SetMtx(this, mpModelOpa->getBaseTRMtx());
    J3DJoint* joint = mpModelOpa->getModelData()->getJointNodePointer(0);
    fopAcM_setCullSizeBox(this, joint->getMin()->x, joint->getMin()->y, joint->getMin()->z,
                          joint->getMax()->x, joint->getMax()->y, joint->getMax()->z);
    mAcchCir.SetWall(0.0f, 0.0f);
    mObjAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed, &current.angle, &shape_angle);
    mObjAcch.CrrPos(dComIfG_Bgsp());
    mGndChk = mObjAcch.m_gnd;
    setEnvTevColor();
    setRoomNo();
    mDemoStart = false;
    return 1;
}

int daObjZraRock_c::CreateHeap() {
    J3DModelData* model_data_opa = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 6);
    mpModelOpa = mDoExt_J3DModel__create(model_data_opa, 0x80000, 0x11000084);
    if (mpModelOpa == NULL) {
        return 0;
    }
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 5);
    mpModelXlu = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModelXlu == NULL) {
        return 0;
    }
    J3DAnmTevRegKey* tev_reg_anm = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 9);
    if (!mBrkAnm.init(model_data, tev_reg_anm, 1, 0, 1.0f, 0, -1)) {
        return 0;
    }
    mBrkAnm.setPlaySpeed(0.0f);
    return 1;
}

cPhs__Step daObjZraRock_c::create() {
    fopAcM_ct(this, daObjZraRock_c);
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (step == cPhs_ERROR_e || step == cPhs_UNK3_e) {
        return cPhs_ERROR_e;
    } else if (step == cPhs_COMPLEATE_e) {
        int dzb_id = dComIfG_getObjctResName2Index(l_arcName, "H_ZoraRock.dzb");
        step = (cPhs__Step)MoveBGCreate(l_arcName, dzb_id, dBgS_MoveBGProc_Trans, 0x2050, NULL);
        if (dComIfGs_isSwitch((fopAcM_GetParam(this) >> 8) & 0xff, fopAcM_GetRoomNo(this))) {
            step = cPhs_ERROR_e;
        }
    }
    return step;
}

int daObjZraRock_c::Execute(Mtx** i_mtx) {
    mBrkAnm.play();
    *i_mtx = &mBgMtx;
    setAttnPos();
    setBaseMtx();
    return 1;
}

int daObjZraRock_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    if (mDemoStart) {
        g_env_light.setLightTevColorType_MAJI(mpModelXlu, &tevStr);
        dComIfGd_setListBG();
        mBrkAnm.entry(mpModelXlu->getModelData());
        mDoExt_modelUpdateDL(mpModelXlu);
        mBrkAnm.remove(mpModelXlu->getModelData());
        dComIfGd_setList();
    } else {
        g_env_light.setLightTevColorType_MAJI(mpModelOpa, &tevStr);
        dComIfGd_setListBG();
        mDoExt_modelUpdateDL(mpModelOpa);
        dComIfGd_setList();
    }
    return 1;
}

int daObjZraRock_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName);
    if (mpBgW != NULL && mpBgW->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgW);
    }
    return 1;
}

static int daObjZraRock_Draw(daObjZraRock_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daObjZraRock_Execute(daObjZraRock_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjZraRock_IsDelete(daObjZraRock_c* param_0) {
    return 1;
}

static int daObjZraRock_Delete(daObjZraRock_c* i_this) {
    return i_this->MoveBGDelete();
}

static cPhs__Step daObjZraRock_create(fopAc_ac_c* i_this) {
    return static_cast<daObjZraRock_c*>(i_this)->create();
}

static actor_method_class l_daObjZraRock_Method = {
    (process_method_func)daObjZraRock_create,
    (process_method_func)daObjZraRock_Delete,
    (process_method_func)daObjZraRock_Execute,
    (process_method_func)daObjZraRock_IsDelete,
    (process_method_func)daObjZraRock_Draw,

};

extern actor_process_profile_definition g_profile_Obj_ZraRock = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_ZraRock,
    &g_fpcLf_Method.base,
    sizeof(daObjZraRock_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x260,
    &l_daObjZraRock_Method,
    0x44100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
