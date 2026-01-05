/**
 * @file d_a_obj_smw_stone.cpp
 * Object - Sacred Meadow Howling Stone
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_smw_stone.h"
#include "d/actor/d_a_alink.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"

static char* l_arcName = "WStoneF";

static int daSmWStone_c_createHeap(fopAc_ac_c* i_this) {
    return static_cast<daSmWStone_c*>(i_this)->createHeap();
}

daSmWStone_c::daSmWStone_c() {
    mpBgW = NULL;
    mMode = 0;
}

daSmWStone_c::~daSmWStone_c() {
    dComIfG_resDelete(this, l_arcName);
}

static u8 const lit_3776[12] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

cPhs__Step daSmWStone_c::create() {
    fopAcM_ct(this, daSmWStone_c);
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(this, l_arcName);

    if (step == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, daSmWStone_c_createHeap, 0x1000)) {
            return cPhs_ERROR_e;
        }

        if (dComIfG_Bgsp().Regist(mpBgW, this)) {
            return cPhs_ERROR_e;
        }

        init();
        setModelMtx();
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());

        dBgS_AcchCir acch_cir;
        dBgS_ObjAcch acch;
        acch_cir.SetWall(0.0f, 10.0f);
        acch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                 &acch_cir, fopAcM_GetSpeed_p(this), NULL, NULL);
        acch.CrrPos(dComIfG_Bgsp());
        mGroundH = acch.GetGroundH();
        mPolyInfo = acch.m_gnd;
    }

    return step;
}

int daSmWStone_c::execute() {
    switch (mMode) {
    case 0:
        exeModeHowl();
        // fallthrough
    case 1:
    default:
        setModelMtx();
        break;
    }

    return 1;
}

int daSmWStone_c::draw() {
    g_env_light.settingTevStruct(8, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    cXyz pos(current.pos.x, current.pos.y, current.pos.z);
    mShadowKey = dComIfGd_setShadow(mShadowKey, 1, mpModel, &pos, 500.0f, 0.0f, pos.y,
                                    mGroundH, mPolyInfo, &tevStr, 0, 1.0f,
                                    dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

int daSmWStone_c::Delete() {
    attention_info.flags = 0;
    if (mpBgW != NULL && mpBgW->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgW);
    }
    this->~daSmWStone_c();
    return 1;
}

void daSmWStone_c::exeModeHowl() {
    u8 swbit;
    if (mDelete || (swbit = getSwBit0(), fopAcM_isSwitch(this, swbit))) {
        attention_info.flags = 0;
        mMode = 1;
    } else {
        if (!dComIfGp_event_runCheck()) {
            daAlink_getAlinkActorClass()->startWindStoneSound(2, &current.pos);
        }
        attention_info.flags = 0;
        if (chkWlfInRange()) {
            attention_info.flags |= fopAc_AttnFlag_ETC_e;
            attention_info.distances[fopAc_attn_ETC_e] = 0x41;
            daAlink_c* link = daAlink_getAlinkActorClass();
            if (link != NULL && link->checkWolfHowlSuccessAnime()) {
                attention_info.flags = 0;
                mMode = 1;
            }
        }
    }
}

void daSmWStone_c::setModelMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daSmWStone_c::createHeap() {
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
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
    cBgD_t* dzb = (cBgD_t*)dComIfG_getObjectRes(l_arcName, 7);
    if (mpBgW->Set(dzb, 1, &mBgMtx) == 1) {
        return 0;
    }

    return 1;
}

void daSmWStone_c::init() {
    u8 swbit = getSwBit0();
    if (fopAcM_isSwitch(this, swbit)) {
        mMode = 1;
    } else {
        mMode = 0;
    }
    mDelete = false;
    mShadowKey = 0;
}

bool daSmWStone_c::chkWlfInRange() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    if (player == NULL) {
        return false;
    }
    if (daPy_py_c::checkNowWolf()) {
        cXyz pos = player->current.pos;
        if (pos.abs2(current.pos) < 40000.0f) {
            return true;
        }
    }
    return false;
}

static cPhs__Step daSmWStone_create(daSmWStone_c* i_this) {
    fopAcM_ct(i_this, daSmWStone_c);
    return i_this->create();
}

static int daSmWStone_Delete(daSmWStone_c* i_this) {
    return i_this->Delete();
}

static int daSmWStone_execute(daSmWStone_c* i_this) {
    return i_this->execute();
}

static int daSmWStone_draw(daSmWStone_c* i_this) {
    return i_this->draw();
}

static actor_method_class daSmWStone_METHODS = {
    (process_method_func)daSmWStone_create,
    (process_method_func)daSmWStone_Delete,
    (process_method_func)daSmWStone_execute,
    NULL,
    (process_method_func)daSmWStone_draw,
};

actor_process_profile_definition g_profile_Obj_SmWStone = {
  fpcLy_CURRENT_e,       // mLayerID
  7,                     // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_Obj_SmWStone,     // mProcName
  &g_fpcLf_Method.base, // sub_method
  sizeof(daSmWStone_c),  // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  735,                   // mPriority
  &daSmWStone_METHODS,   // sub_method
  0x00044100,            // mStatus
  fopAc_ENV_e,           // mActorType
  fopAc_CULLBOX_0_e,     // cullType
};
