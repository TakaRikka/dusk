/**
 * d_a_obj_wind_stone.cpp
 * Object - Wind Stone
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_wind_stone.h"
#include "d/actor/d_a_alink.h"
#include "d/d_meter2_info.h"
#include "dolphin/types.h"

static int daWindStone_c_createHeap(fopAc_ac_c* i_this) {
    return static_cast<daWindStone_c*>(i_this)->createHeap();
}

static char* l_arcName = "WindStone";

daWindStone_c::daWindStone_c() : mpBgW(NULL), field_0x5c4(0) {}

daWindStone_c::~daWindStone_c() {
    dComIfG_resDelete(this, l_arcName);
}

int daWindStone_c::createHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }
    mpBgW = new dBgW();
    if (mpBgW == NULL) {
        return 0;
    }
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    MTXCopy(mDoMtx_stack_c::get(), field_0x58c);
    cBgD_t* res = (cBgD_t*)dComIfG_getObjectRes(l_arcName, 7);
    int bgWBase_set = mpBgW->Set(res, 1, &field_0x58c);
    if (bgWBase_set == 1) {
        return 0;
    }
    return 1;
}

static u8 const lit_3760[12] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

int daWindStone_c::create() {
    fopAcM_ct(this, daWindStone_c);

    int phase = dComIfG_resLoad(this, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, daWindStone_c_createHeap, 0x1000)) {
            return cPhs_ERROR_e;
        }
        if (dComIfG_Bgsp().Regist(mpBgW, this)) {
            return cPhs_ERROR_e;
        }
        init();
        setModelMtx();
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());

        dBgS_AcchCir acchCir;
        dBgS_ObjAcch objAcch;
        acchCir.SetWall(0.0f, 10.0f);
        objAcch.Set(&current.pos, &old.pos, this, 1, &acchCir, fopAcM_GetSpeed_p(this), NULL,
                    NULL);
        objAcch.CrrPos(dComIfG_Bgsp());
        field_0x5bc = objAcch.GetGroundH();
        mPolyInfo = objAcch.m_gnd;

        if (field_0x5c4 == 0) {
            fopAcM_onSwitch(this, getSwBit1());
        }
    }
    return phase;
}

int daWindStone_c::execute() {
    switch (field_0x5c4) {
    case 0:
        exeModeHowl();
        break;
    case 1:
        exeModeMapDisp();
        break;
    case 2:
        break;
    }
    setModelMtx();
    return 1;
}

int daWindStone_c::draw() {
    g_env_light.settingTevStruct(8, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    cXyz shadowPos(current.pos.x, current.pos.y, current.pos.z);
    field_0x5c0 = dComIfGd_setShadow(field_0x5c0, 1, mpModel, &shadowPos, 500.0f, 0.0f,
                                     current.pos.y, field_0x5bc, mPolyInfo, &tevStr, 0, 1.0f,
                                     dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

int daWindStone_c::Delete() {
    if (fopAcM_isSwitch(this, getSwBit1())) {
        fopAcM_offSwitch(this, getSwBit1());
    }
    if (mpBgW != NULL && mpBgW->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgW);
    }
    this->~daWindStone_c();
    return 1;
}

void daWindStone_c::setModelMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daWindStone_c::init() {
    if (!chkEveOccur()) {
        if (chkMapDispMode()) {
            field_0x5c4 = 1;
        } else {
            field_0x5c4 = 2;
        }
    } else {
        field_0x5c4 = 0;
    }
    field_0x5c0 = 0;
}

bool daWindStone_c::chkWlfInRange() {
    if (dComIfGp_getPlayer(0) == NULL) {
        return false;
    }
    if (daPy_py_c::checkNowWolf()) {
        cXyz player_pos = dComIfGp_getPlayer(0)->current.pos;
        if (player_pos.abs2(current.pos) < 40000.0f) {
            return true;
        }
    }
    return false;
}

bool daWindStone_c::chkEveOccur() {
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[getDelEveFlgId()]) == false) {
        return true;
    }
    return false;
}

void daWindStone_c::exeModeHowl() {
    if (!chkEveOccur()) {
        attention_info.flags = 0;
        field_0x5c4 = 2;
    } else {
        s8 tuneId = getTuneId();
        daAlink_getAlinkActorClass()->startWindStoneSound(tuneId, &current.pos);
        attention_info.flags = 0;
        if (chkWlfInRange()) {
            attention_info.flags |= fopAc_AttnFlag_ETC_e;
            attention_info.distances[fopAc_attn_ETC_e] = 65;
        }
    }
}

void daWindStone_c::exeModeMapDisp() {
    if (dComIfGp_getEvent().isOrderOK() || !dComIfGp_event_runCheck()) {
        dMeter2Info_setGoldWolfMapType(getGoldWolfIdx());
        dMeter2Info_setPauseStatus(5);
        fopAcM_onSwitch(this, getSwBit2());
        field_0x5c4 = 2;
    }
}

bool daWindStone_c::chkMapDispMode() {
    if (fopAcM_isSwitch(this, getSwBit2())) {
        return false;
    }
    s16 start_point = dComIfGs_getStartPoint();
    switch (getTuneId()) {
    case 3:
    case 5:
    case 7:
    case 8:
        if (start_point == 100) {
            return true;
        }
        break;
    case 4:
    case 6:
        if (start_point == 101) {
            return true;
        }
        break;
    }
    return false;
}

u8 daWindStone_c::getGoldWolfIdx() {
    switch (getTuneId()) {
    case 3:
        return 2;
    case 4:
        return 3;
    case 5:
        return 4;
    case 6:
        return 5;
    case 7:
        return 6;
    case 8:
        return 7;
    default:
        return 2;
    }
}

static int daWindStone_create(daWindStone_c* i_this) {
    fopAcM_ct(i_this, daWindStone_c);

    return i_this->create();
}

static int daWindStone_Delete(daWindStone_c* i_this) {
    return i_this->Delete();
}

static int daWindStone_execute(daWindStone_c* i_this) {
    return i_this->execute();
}

static int daWindStone_draw(daWindStone_c* i_this) {
    return i_this->draw();
}

static actor_method_class daWindStone_METHODS = {
    (process_method_func)daWindStone_create,  (process_method_func)daWindStone_Delete,
    (process_method_func)daWindStone_execute, (process_method_func)NULL,
    (process_method_func)daWindStone_draw,
};

extern actor_process_profile_definition g_profile_Obj_WindStone = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Obj_WindStone,     // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daWindStone_c),  // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    732,                    // mPriority
    &daWindStone_METHODS,   // sub_method
    0x40100,                // mStatus
    fopAc_ENV_e,            // mActorType
    fopAc_CULLBOX_0_e,      // cullType
};
