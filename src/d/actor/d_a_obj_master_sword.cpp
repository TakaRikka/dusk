/**
 * @file d_a_obj_master_sword.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_master_sword.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2_info.h"

daObjMasterSword_Attr_c const daObjMasterSword_c::mAttr = {1.0f};

void daObjMasterSword_c::initBaseMtx() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());

    Vec scale = {attr(), attr(), attr()};
    mpModel->setBaseScale(scale);

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);

    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daObjMasterSword_c::initWait() {
    cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    current.pos = home.pos;
    current.angle = home.angle;
    shape_angle = home.angle;
}

void daObjMasterSword_c::executeWait() {
    if (daPy_getPlayerActorClass()->checkPriActorOwn(this)) {
        for (int i = 0; i < dComIfGp_getAttention()->GetActionCount(); i++) {
            if (dComIfGp_getAttention()->ActionTarget(i) == this) {
                if (dComIfGp_getAttention()->getActionBtnB() != NULL &&
                    dComIfGp_getAttention()->getActionBtnB()->mType == fopAc_attn_CARRY_e)
                {
                    dComIfGp_setDoStatusForce(8, 0);
                }
            }
        }
    }

    if (fopAcM_checkCarryNow(this)) {
        dMeter2Info_setCloth(fpcNm_ITEM_WEAR_KOKIRI, false);
        fopAcM_orderMapToolEvent(this, getEventID(), 0xFF, 0xFFFF, 1, 0);
    }
}

int daObjMasterSword_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daObjMasterSword_c*>(i_this)->CreateHeap();
}

static char* l_arcName = "MstrSword";

int daObjMasterSword_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 5);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (mpModel == NULL) {
        return 0;
    }

    J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 11);
    if (!mBtk.init(modelData, pbtk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
        return 0;
    }

    J3DAnmTevRegKey* pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 8);
    if (!mBrk.init(modelData, pbrk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
        return 0;
    }

    return 1;
}

static int daObjMasterSword_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjMasterSword_c*>(i_this)->create();
}

actionFunc daObjMasterSword_c::ActionTable[] = {
    &daObjMasterSword_c::initWait, &daObjMasterSword_c::executeWait,
};

void daObjMasterSword_c::initCollision() {
    static dCcD_SrcCyl ccCylSrc = {
        {
            {0, {{0, 0, 0}, {0, 0}, 0x79}},  // mObj
            {dCcD_SE_NONE, 0, 0, 0, 0},      // mGObjAt
            {dCcD_SE_NONE, 0, 0, 0, 4},      // mGObjTg
            {0},                             // mGObjCo
        },                                   // mObjInf
        {
            {current.pos.x, current.pos.y, current.pos.z},  // mCenter
            18.0f,                                          // mRadius
            180.0f                                          // mHeight
        }                                                   // mCyl
    };

    mCcStts.Init(0xFF, 0xFF, this);
    mCyl.Set(ccCylSrc);
    mCyl.SetStts(&mCcStts);
}

void daObjMasterSword_c::callInit() {
    (this->**mActionFunc)();
}

void daObjMasterSword_c::setAction(daObjMasterSword_c::Mode_e i_mode) {
    mMode = i_mode;
    mActionFunc = &ActionTable[2 * mMode];
    callInit();
}

void daObjMasterSword_c::create_init() {
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    initCollision();
    initBaseMtx();

    fopAcM_OnCarryType(this, fopAcM_CARRY_UNK_30);
    cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    attention_info.distances[fopAc_attn_CARRY_e] = 74;
    attention_info.position = current.pos;
    attention_info.position.y += 100.0f;
    eyePos = attention_info.position;

    dBgS_AcchCir cir_check;
    dBgS_ObjAcch obj_check;

    cir_check.SetWall(10.0f, 30.0f);
    obj_check.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &cir_check,
                  fopAcM_GetSpeed_p(this), NULL, NULL);
    obj_check.CrrPos(dComIfG_Bgsp());

    field_0x738 = obj_check.GetGroundH();
    field_0x728 = obj_check.m_gnd;

    setAction(MODE_0_e);
}

int daObjMasterSword_c::create() {
    fopAcM_ct(this, daObjMasterSword_c);

    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[getFlagNo()])) {
        return cPhs_ERROR_e;
    }

    int phase = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, daObjMasterSword_c::createHeapCallBack, 0x1830)) {
            return cPhs_ERROR_e;
        }

        create_init();
    }

    return phase;
}

static int daObjMasterSword_Delete(daObjMasterSword_c* i_this) {
    i_this->~daObjMasterSword_c();
    return 1;
}

daObjMasterSword_c::~daObjMasterSword_c() {
    dComIfG_resDelete(&mPhase, l_arcName);
}

void daObjMasterSword_c::callExecute() {
    (this->*mActionFunc[1])();
}

void daObjMasterSword_c::setCollision() {
    dComIfG_Ccsp()->Set(&mCyl);
}

int daObjMasterSword_c::execute() {
    callExecute();
    setCollision();

    mBtk.play();
    mBrk.play();

    if (dComIfGs_isTmpBit(dSv_event_tmp_flag_c::tempBitLabels[73])) {
        dComIfGs_onItemFirstBit(fpcNm_ITEM_MASTER_SWORD);
        dMeter2Info_setSword(fpcNm_ITEM_MASTER_SWORD, false);
        dComIfGs_setSelectEquipSword(fpcNm_ITEM_MASTER_SWORD);

        dComIfGp_setItemLifeCount(dComIfGs_getMaxLife(), 0);
        dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[getFlagNo()]);
        fopAcM_delete(this);
    }

    return 1;
}

static int daObjMasterSword_Execute(daObjMasterSword_c* i_this) {
    return i_this->execute();
}

static int daObjMasterSword_Draw(daObjMasterSword_c* i_this) {
    return i_this->draw();
}

int daObjMasterSword_c::draw() {
    if (dComIfGs_isTmpBit(dSv_event_tmp_flag_c::tempBitLabels[73])) {
        return 1;
    }

    J3DModelData* modelData = mpModel->getModelData();
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mBtk.entry(modelData);
    mBrk.entry(modelData);
    mDoExt_modelUpdateDL(mpModel);

    mBtk.remove(modelData);
    mBrk.remove(modelData);
    dComIfGd_setList();

    cXyz sp8 = cXyz(current.pos.x, current.pos.y + 50.0f, current.pos.z);
    mShadowKey =
        dComIfGd_setShadow(mShadowKey, 1, mpModel, &sp8, 200.0f, 10.0f, current.pos.y, field_0x738,
                           field_0x728, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());

    return 1;
}

static int daObjMasterSword_IsDelete(daObjMasterSword_c* param_0) {
    return 1;
}

static actor_method_class l_daObjMasterSword_Method = {
    (process_method_func)daObjMasterSword_Create,
    (process_method_func)daObjMasterSword_Delete,
    (process_method_func)daObjMasterSword_Execute,
    (process_method_func)daObjMasterSword_IsDelete,
    (process_method_func)daObjMasterSword_Draw,
};

extern actor_process_profile_definition g_profile_Obj_MasterSword = {
  fpcLy_CURRENT_e,            // mLayerID
  7,                          // mListID
  fpcPi_CURRENT_e,            // mListPrio
  PROC_Obj_MasterSword,       // mProcName
  &g_fpcLf_Method.base,      // sub_method
  sizeof(daObjMasterSword_c), // mSize
  0,                          // mSizeOther
  0,                          // mParameters
  &g_fopAc_Method.base,       // sub_method
  562,                        // mPriority
  &l_daObjMasterSword_Method, // sub_method
  0x00040000,                 // mStatus
  fopAc_ACTOR_e,              // mActorType
  fopAc_CULLBOX_CUSTOM_e,     // cullType
};
