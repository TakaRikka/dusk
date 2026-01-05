/**
 * d_a_obj_chandelier.cpp
 * Object - Chandelier (Hyrule Castle)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_chandelier.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

static char* l_arcName = "L9Chand";

daObjChandelier_proc daObjChandelier_c::s_exeProc[5] = {
    &daObjChandelier_c::exeModeWait,
    &daObjChandelier_c::exeModeStartSwg,
    &daObjChandelier_c::exeModeDown,
    &daObjChandelier_c::exeModeEndSwg,
    &daObjChandelier_c::exeModeHookSwg,
};

daObjChandelier_c::daObjChandelier_c() {
    /* empty function */
}

daObjChandelier_c::~daObjChandelier_c() {
    dComIfG_resDelete(this, l_arcName);
}

int daObjChandelier_c::CreateHeap() {
    J3DModelData* bmd = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    mpModel = mDoExt_J3DModel__create(bmd, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }
    return 1;
}

cPhs__Step daObjChandelier_c::create1st() {
    fopAcM_ct(this, daObjChandelier_c);
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(this, l_arcName);
    if (step == cPhs_COMPLEATE_e) {
        step = (cPhs__Step)MoveBGCreate(l_arcName, 7, dBgS_MoveBGProc_TypicalRotY, 0x4500, &mMtx);
        if (step == cPhs_ERROR_e) {
            return step;
        }
        init();
        setModelMtx();
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    }
    return step;
}

int daObjChandelier_c::Delete() {
    this->~daObjChandelier_c();
    return 1;
}

int daObjChandelier_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjChandelier_c::Execute(Mtx** param_0) {
    if (s_exeProc[mMode] != NULL) {
        (this->*s_exeProc[mMode])();
    }
    setModelMtx();
    *param_0 = &mMtx;
    return 1;
}

void daObjChandelier_c::exeModeWait() {
    exeModeHookSwg();
    u8 sw = getSW_0();
    if (fopAcM_isSwitch(this, sw)) {
        mMode = MODE_START_SWG;
        field_0x5ec = 0.5f;
        field_0x5f0 = 0.0f;
        field_0x5fc = 0;
        mTimer = 0;
        field_0x5f8 = 20.0f;
    }
}

void daObjChandelier_c::exeModeStartSwg() {
    moveSwing(20.0f, 300.0f, 50.0f, 0.3f);
    if (mTimer >= 90) {
        mMode = MODE_DOWN;
        field_0x5fc = 0;
        mTimer = 0;
        shape_angle.y = home.angle.y;
        shape_angle.z = home.angle.z;
        current.pos = field_0x5a8;
    } else {
        mTimer++;
    }
}

void daObjChandelier_c::exeModeDown() {
    current.pos.y += speed.y;
    speed.y += gravity;
    if (current.pos.y <= field_0x5f4) {
        fopAcM_seStartCurrent(this, Z2SE_OBJ_CHANDLV9_DOWN, 0);
        mMode = MODE_END_SWG;
        field_0x5ec = 1.0f;
        field_0x5a8 = current.pos;
    }
}

void daObjChandelier_c::exeModeEndSwg() {
    moveSwingFall();
    if (mTimer >= 60) {
        mMode = MODE_HOOK_SWG;
        mTimer = 0;
        hookSwingInitParm();
    } else {
        mTimer++;
    }
}

void daObjChandelier_c::exeModeHookSwg() {
    if (field_0x60a != 0 || fopAcM_checkHookCarryNow(this)) {
        f32 f1 = 10.0f;
        f32 f2 = 0.5f;
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        if (player != NULL) {
            f32 delta_y = current.pos.y - player->current.pos.y;
            if (delta_y > 0.0f) {
                f2 = 250.0f / delta_y;
                f1 = 5000.0f / delta_y;
            }
        }
        moveSwing(f2, f1, 15.0f, 0.2f);
    } else if (field_0x60b != 0 && field_0x60a == 0) {
        hookSwingInitParm();
    }
    field_0x60b = field_0x60a;
    field_0x60a = 0;
}

void daObjChandelier_c::init() {
    mMode = MODE_WAIT;
    if (getArg_0() == 1) {
        mMode = MODE_HOOK_SWG;
    }
    field_0x5a8 = current.pos;
    field_0x60a = 0;
    field_0x60b = field_0x60a;
    gravity = -3.0f;
    speed.x = 0.0f;
    speed.y = -1.0f;
    speed.z = 0.0f;
    field_0x5fc = 0;
    field_0x608 = 1;
    field_0x606 = 0;
    hookSwingInitParm();
    field_0x5f4 = current.pos.y - 800.0f;
    if (getArg_0() == 0) {
        u8 sw = getSW_0();
        if (fopAcM_isSwitch(this, sw)) {
            mMode = MODE_HOOK_SWG;
            current.pos.y = field_0x5f4;
            field_0x5a8 = current.pos;
        }
    }
}

void daObjChandelier_c::setModelMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::ZrotM(shape_angle.z);
    PSMTXCopy(mDoMtx_stack_c::get(), mMtx);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daObjChandelier_c::moveSwing(f32 param_0, f32 param_1, f32 param_2, f32 param_3) {
    f32 f1 = -param_1 + param_0 * field_0x5fc;
    shape_angle.z += (s16)(field_0x608 * (f1 * field_0x5ec));
    shape_angle.y += (s16)(field_0x608 * field_0x604 * field_0x5ec);
    if (f1 > param_1) {
        field_0x5fc = 0;
        field_0x608 *= -1;
        field_0x604 = cM_rndFX(param_2);
        if (cLib_chaseF(&field_0x5ec, field_0x5f0, 0.1f)) {
            field_0x5f0 = param_3 + cM_rndF(param_3);
        }
    }
    field_0x606 += field_0x608 * 0x200;
    current.pos.x = home.pos.x + cM_ssin(field_0x606) * 3.0f;
    current.pos.z = home.pos.z + cM_scos(field_0x606) * 3.0f;
    u32 u1 = field_0x5f8 * field_0x5ec * 127.0f * 0.1f;
    if (u1 > 0x7f) {
        u1 = 0x7f;
    }
    Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_CHANDLV9_SWING, &current.pos, u1,
                                  0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    field_0x5fc++;
}

void daObjChandelier_c::moveSwingFall() {
    static Vec TOP_POS_Y_OFFSET = { 0.0f, 3000.0f, 0.0f };
    cLib_chaseF(&field_0x5ec, 0.0f, 0.04f);
    field_0x608 *= -1;
    shape_angle.z = field_0x608 * 50.0f * field_0x5ec;
    shape_angle.y += (s16)cM_rndFX(50.0f);
    cXyz vec1 = field_0x5a8;
    mDoMtx_stack_c::transS(vec1);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    cXyz vec2;
    mDoMtx_stack_c::multVec(&TOP_POS_Y_OFFSET, &vec2);
    cXyz vec3 = vec1 + TOP_POS_Y_OFFSET;
    cXyz vec4 = vec3 - vec2;
    current.pos = vec1 + vec4;
}

void daObjChandelier_c::hookSwingInitParm() {
    field_0x5ec = 1.2f;
    field_0x5f0 = 0.7f;
    shape_angle.y = home.angle.y;
    shape_angle.z = home.angle.z;
    current.pos = field_0x5a8;
    field_0x604 = 30;
    field_0x5fc = 0;
    field_0x5f8 = 5.0f;
}

static cPhs__Step daObjChandelier_create1st(daObjChandelier_c* i_this) {
    fopAcM_ct(i_this, daObjChandelier_c);
    return i_this->create1st();
}

static int daObjChandelier_MoveBGDelete(daObjChandelier_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjChandelier_MoveBGExecute(daObjChandelier_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjChandelier_MoveBGDraw(daObjChandelier_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjChandelier_METHODS = {
    (process_method_func)daObjChandelier_create1st,
    (process_method_func)daObjChandelier_MoveBGDelete,
    (process_method_func)daObjChandelier_MoveBGExecute,
    (process_method_func)NULL,
    (process_method_func)daObjChandelier_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_Chandelier = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_Chandelier,
    &g_fpcLf_Method.base,
    sizeof(daObjChandelier_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x2E5,
    &daObjChandelier_METHODS,
    0x40080,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
