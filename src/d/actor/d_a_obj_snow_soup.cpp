/**
 * @file d_a_obj_snow_soup.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_snow_soup.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_procname.h"

static int daObjSnowSoup_c_createHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjSnowSoup_c*>(i_this)->createHeap();
}

static char* l_arcName = "SnowSoup";

static Vec const SOUP_MODEL_OFFSET = {3317.86f, 214.73f, 323.3f};

daObjSnowSoup_c::daObjSnowSoup_c() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            mpSmkEmtChange[i][j] = NULL;
        }
    }
    memset(mpSmkEmtCommon, 0, 0x10);
    memset(mpModel, 0, 0xc);
}

daObjSnowSoup_c::~daObjSnowSoup_c() {
    dComIfG_resDelete(this, l_arcName);
}

int daObjSnowSoup_c::createHeap() {
    static u32 const BMD_IDX[3] = {3, 4, 5};
    for (int i = 0; i < 3; i++) {
        J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, BMD_IDX[i]);
        mpModel[i] = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
        if (mpModel[i] == NULL) {
            return false;
        }
    }
    return true;
}

cPhs__Step daObjSnowSoup_c::create() {
    fopAcM_ct(this, daObjSnowSoup_c);
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(this, l_arcName);
    if (step == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, daObjSnowSoup_c_createHeap, 0x6500)) {
            return cPhs_ERROR_e;
        }
        init();
        setModelMtx();
        fopAcM_SetMtx(this, mpModel[mState]->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel[mState]->getModelData());
        createSmkEmtCommon();
        createSmkEmtChange(mState);
        mBubblePos.set(SOUP_MODEL_OFFSET.x, SOUP_MODEL_OFFSET.y, SOUP_MODEL_OFFSET.z);
        mFirewoodPos.set(SOUP_MODEL_OFFSET.x, 0.0f, SOUP_MODEL_OFFSET.z);
    }
    return step;
}

int daObjSnowSoup_c::Delete() {
    deleteSmkEmtCommon();
    deleteSmkEmtChange(mState);
    this->~daObjSnowSoup_c();
    return 1;
}

int daObjSnowSoup_c::draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel[mState], &tevStr);
    mDoExt_modelUpdate(mpModel[mState]);
    return 1;
}

int daObjSnowSoup_c::execute() {
    mDoAud_seStartLevel(Z2SE_OBJ_NABE_BUBBLE, &mBubblePos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mDoAud_seStartLevel(Z2SE_OBJ_FIREWOOD_BURNING, &mFirewoodPos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    switch (mState) {
    case 0:
            /* dSv_event_flag_c::F_0003 - Snowpeak Ruins - Handed over tomato puree and left room */
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[7])) {
            deleteSmkEmtChange(mState);
            mState = 1;
            createSmkEmtChange(mState);
            setModelMtx();
            fopAcM_SetMtx(this, mpModel[mState]->getBaseTRMtx());
            return 1;
        }
        break;
    case 1:
            /* dSv_event_flag_c::F_0004 - Snowpeak Ruins - Handed over secret ingredient and left room */
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[8])) {
            deleteSmkEmtChange(mState);
            mState = 2;
            createSmkEmtChange(mState);
            setModelMtx();
            fopAcM_SetMtx(this, mpModel[mState]->getBaseTRMtx());
            return 1;
        }
        break;
    case 2:
        break;
    }
    setModelMtx();
    return 1;
}

void daObjSnowSoup_c::init() {
    mState = 0;
        /* dSv_event_flag_c::F_0004 - Snowpeak Ruins - Handed over secret ingredient and left room */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[8])) {
        mState = 2;
               /* dSv_event_flag_c::F_0003 - Snowpeak Ruins - Handed over tomato puree and left room */
    } else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[7])) {
        mState = 1;
    }
}

void daObjSnowSoup_c::setModelMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel[mState]->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daObjSnowSoup_c::createSmkEmtCommon() {
    static u16 const PARTICLE_NAME[4] = {0x87E1, 0x87E2, 0x87E3, 0x87E6};
    cXyz vec1(0.0f, 0.0f, 0.0f);
    csXyz vec2(0, 0, 0);
    cXyz vec3(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < 4; i++) {
        mpSmkEmtCommon[i] = dComIfGp_particle_set(PARTICLE_NAME[i], &vec1, &vec2, &vec3);
    }
}

void daObjSnowSoup_c::deleteSmkEmtCommon() {
    for (int i = 0; i < 4; i++) {
        if (mpSmkEmtCommon[i] != NULL) {
            mpSmkEmtCommon[i]->becomeInvalidEmitter();
            mpSmkEmtCommon[i] = NULL;
        }
    }
}

void daObjSnowSoup_c::createSmkEmtChange(int i_state) {
    static u16 const PARTICLE_NAME[3][2] = {{0x8984, 0x8985}, {0x8A8B, 0x8A8D}, {0x8A8C, 0x8A8E}};
    cXyz vec1(0.0f, 0.0f, 0.0f);
    csXyz vec2(0, 0, 0);
    cXyz vec3(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < 2; i++) {
        mpSmkEmtChange[i_state][i] =
            dComIfGp_particle_set(PARTICLE_NAME[i_state][i], &vec1, &vec2, &vec3);
    }
}

void daObjSnowSoup_c::deleteSmkEmtChange(int i_state) {
    for (int i = 0; i < 2; i++) {
        if (mpSmkEmtChange[i_state][i] != NULL) {
            mpSmkEmtChange[i_state][i]->becomeInvalidEmitter();
            mpSmkEmtChange[i_state][i] = NULL;
        }
    }
}


static cPhs__Step daObjSnowSoup_create(daObjSnowSoup_c* i_this) {
    fopAcM_ct(i_this, daObjSnowSoup_c);
    return i_this->create();
}

static int daObjSnowSoup_Delete(daObjSnowSoup_c* i_this) {
    return i_this->Delete();
}

static int daObjSnowSoup_execute(daObjSnowSoup_c* i_this) {
    return i_this->execute();
}

static int daObjSnowSoup_draw(daObjSnowSoup_c* i_this) {
    return i_this->draw();
}

static actor_method_class daObjSnowSoup_METHODS = {
    (process_method_func)daObjSnowSoup_create,
    (process_method_func)daObjSnowSoup_Delete,
    (process_method_func)daObjSnowSoup_execute,
    (process_method_func)NULL,
    (process_method_func)daObjSnowSoup_draw,
};

actor_process_profile_definition g_profile_Obj_SnowSoup = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Obj_SnowSoup,
    &g_fpcLf_Method.base,
    sizeof(daObjSnowSoup_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x2E2,
    &daObjSnowSoup_METHODS,
    0x44100,
    fopAc_ENV_e,
    fopAc_CULLBOX_CUSTOM_e,
};
