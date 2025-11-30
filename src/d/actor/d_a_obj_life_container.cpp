/**
 * @file d_a_obj_life_container.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_life_container.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/d_item_data.h"
#include "SSystem/SComponent/c_math.h"

const static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xffffffff, 0x11}, 0x59}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {0.0f, 0.0f, 0.0f}, // mCenter
        20.0f, // mRadius
        40.0f // mHeight
    } // mCyl
};

static f32 Reflect(cXyz* i_vec, cBgS_PolyInfo const& i_wallpoly, f32 i_scale) {
    cM3dGPla plane;

    if (dComIfG_Bgsp().GetTriPla(i_wallpoly, &plane)) {
        cXyz reflect_vec;
        f32 mag = i_vec->absXZ();

        C_VECReflect(i_vec, &plane.mNormal, &reflect_vec);
        *i_vec = (reflect_vec * mag) * i_scale;
        return i_vec->absXZ();
    }

    return 0.0f;
}

static void lifeGetTgCallBack(fopAc_ac_c* i_tgActor, dCcD_GObjInf* i_tgObjInf,
                              fopAc_ac_c* i_atActor, dCcD_GObjInf* i_atObjInf) {
    if (i_tgActor != NULL &&
        (i_atObjInf->ChkAtType(AT_TYPE_40) || i_atObjInf->ChkAtType(AT_TYPE_BOOMERANG)) &&
        !dComIfGp_event_runCheck() && !((daObjLife_c*)i_tgActor)->chkStatus(daObjLife_c::STATUS_BOOMERANG_CARRY_e))
    {
        ((daObjLife_c*)i_tgActor)->actionInitBoomerangCarry();
    }
}

static void lifeGetCoCallBack(fopAc_ac_c* i_coActorA, dCcD_GObjInf* i_coObjInfA,
                              fopAc_ac_c* i_coActorB, dCcD_GObjInf* i_coObjInfB) {
    if (i_coActorA != NULL && i_coActorB != NULL && i_coActorB == (fopAc_ac_c*)dComIfGp_getLinkPlayer()) {
        if (!daPy_getPlayerActorClass()->checkCanoeRide()) {
            ((daObjLife_c*)i_coActorA)->initActionOrderGetDemo();
        }
    }
}

void daObjLife_c::initBaseMtx() {
    setBaseMtx();
}

void daObjLife_c::setBaseMtx() {
    mpModel->setBaseScale(scale);
    
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daObjLife_c::Create() {
    scale.x = scale.y = scale.z = 0.0f;
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());

    mAcchCir.SetWall(30.0f, 30.0f);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
    
    mCcStts.Init(0, 0xFF, this);
    mCcCyl.Set(l_cyl_src);
    mCcCyl.SetStts(&mCcStts);
    mCcCyl.SetCoHitCallback(lifeGetCoCallBack);
    mCcCyl.SetTgHitCallback(lifeGetTgCallBack);
    mCcCyl.SetR(dItem_data::getR(m_itemNo));
    mCcCyl.SetH(dItem_data::getH(m_itemNo));

    fopAcM_SetCullSize(this, fopAc_CULLSPHERE_0_e);
    fopAcM_SetGravity(this, -3.2f);

    actionWaitInit();
    field_0x94c = 0.7f;
    mRotateSpeed = 7000;

    setEffect();
    mSound.init(&current.pos, 1);
    return 1;
}

void daObjLife_c::setEffect() {
    cXyz size(1.5f, 1.5f, 1.5f);

    if (mEffect0.getEmitter() == NULL) {
        dComIfGp_particle_set(0x8DE, &current.pos, NULL, &size, 0xFF, &mEffect0, -1, NULL, NULL, NULL);
    }

    if (mEffect1.getEmitter() == NULL) {
        dComIfGp_particle_set(0x8DF, &current.pos, NULL, &size, 0xFF, &mEffect1, -1, NULL, NULL, NULL);
    }

    if (mEffect2.getEmitter() == NULL) {
        dComIfGp_particle_set(0x8E0, &current.pos, NULL, &size, 0xFF, &mEffect2, -1, NULL, NULL, NULL);
    }
}

void daObjLife_c::endEffect00() {
    mEffect0.remove();
    mEffect1.remove();
}

void daObjLife_c::endEffect02() {
    mEffect2.remove();
}

int daObjLife_c::__CreateHeap() {
    return 1;
}

int daObjLife_c::create() {
    fopAcM_ct(this, daObjLife_c);

    if (!mIsPrmsInit) {
        field_0x938 = home.angle.x;
        field_0x93a = home.angle.z;
        home.angle.x = home.angle.z = 0;
        current.angle.x = current.angle.z = 0;
        shape_angle.x = shape_angle.z = 0;
        mIsPrmsInit = true;
    }

    m_itemNo = getItemNo();
    if (m_itemNo != fpcNm_ITEM_KAKERA_HEART && m_itemNo != fpcNm_ITEM_UTAWA_HEART) {
        // "Heart Container: Item No is incorrect!<%d>\n"
        OS_REPORT_ERROR("ハートの器：アイテム番号が不正です！<%d>\n", m_itemNo);
    }

    if (fopAcM_isItem(this, getSaveBitNo())) {
        return cPhs_ERROR_e;
    }

    if (m_itemNo == fpcNm_ITEM_UTAWA_HEART && dComIfGs_isStageLife()) {
        return cPhs_ERROR_e;
    }

    int phase_state = dComIfG_resLoad(&mPhase, dItem_data::getFieldArc(m_itemNo));
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, CheckFieldItemCreateHeap, 0x4000)) {
            return cPhs_ERROR_e;
        }

        if (!Create()) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("LIFE CONTAINER:<0x%x>\n", fopAcM_GetParam(this));
    }

    return phase_state;
}

void daObjLife_c::bg_check() {
    f32 bounce_speed_scale = 0.7f;

    if (mAcch.ChkWallHit()) {
        if (speedF > 5.0f) {
            Reflect(&speed, mAcchCir, 1.0f);
        }

        current.angle.y = cM_atan2s(speed.x, speed.z);
    }

    if (mAcch.ChkGroundLanding()) {
        cXyz reflect_vec(speed);
        if (speedF > 5.0f) {
            Reflect(&reflect_vec, mAcchCir, bounce_speed_scale);
        }

        speed.y = -mPrevSpeed.y * bounce_speed_scale;
        if (speed.y < 3.0f) {
            speedF = 0.0f;
        }

        mGndLandCount++;
        fopAcM_seStart(this, Z2SE_OBJ_KEY_BOUND, fabsf(mPrevSpeed.y) * 2.0f);
    }

    if (mGndLandCount >= 2 || (!mAcch.ChkGroundLanding() && mAcch.ChkGroundHit())) {
        endEffect02();
    }
}

int daObjLife_c::actionWaitInit() {
    mCcCyl.OnTgSPrmBit(1);
    mCcCyl.OnCoSPrmBit(1);

    show();
    attention_info.position = current.pos;
    setStatus(STATUS_WAIT_e);

    if (strcmp(dComIfGp_getStartStageName(), "D_MN11A") == 0) {
        fopAcM_SetGravity(this, 0.0f);
        mLv5Counter = 0;
        speed.y = 4.0f;
    }

    return 1;
}

int daObjLife_c::actionWait() {
    if (strcmp(dComIfGp_getStartStageName(), "D_MN11A") == 0) {
        if (mLv5Counter < 20000) {
            mLv5Counter++;
        }

        if (mLv5Counter == 1) {
            speed.y = 5.0f;
            speedF = 3.0f;
        } else if (mLv5Counter >= 50 && mLv5Counter < 2000) {
            cLib_addCalc(&speed.y, -3.2f, 0.05f, 0.5f, 0.1f);
        }

        fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
        mAcch.CrrPos(dComIfG_Bgsp());

        if (mAcch.ChkGroundLanding() || mAcch.ChkGroundHit()) {
            fopAcM_SetSpeed(this, 0.0f, -1.0f, 0.0f);
            fopAcM_SetSpeedF(this, 0.0f);
        } else if (speed.y < 0.0f) {
            f32 var_f31 = 3.5f;
            s16 var_r28 = mCounter * 1100;
            fopAcM_SetSpeedF(this, cM_ssin(var_r28) * var_f31);
        }

        RotateYBase();
    } else {
        if (!fopAcM_checkHookCarryNow(this)) {
            fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
        }

        mAcch.CrrPos(dComIfG_Bgsp());
        bg_check();

        if (mAcch.ChkGroundHit()) {
            speedF *= 0.9f;
            if (speedF < 1.0f) {
                speedF = 0.0f;
            }
        }

        if (mGndLandCount != 0) {
            s16 target_speed = 0xFFFF / getData().mRotateYSpeed;
            cLib_addCalcAngleS2(&mRotateSpeed, target_speed, 5, 2000);
        }

        shape_angle.y += mRotateSpeed;
    }

    if (!daPy_getPlayerActorClass()->checkCanoeRide() && mCcCyl.ChkCoHit()) {
        daPy_py_c* player = daPy_getPlayerActorClass();
        fopAc_ac_c* hit_actor = mCcCyl.GetCoHitAc();
        if (player == hit_actor) {
            initActionOrderGetDemo();
        }
    }

    attention_info.position = current.pos;
    return 1;
}

int daObjLife_c::initActionOrderGetDemo() {
    mCcCyl.OffTgSPrmBit(1);
    mCcCyl.OffCoSPrmBit(1);
    mCcCyl.ClrTgHit();
    mCcCyl.ClrCoHit();

    endEffect00();
    endEffect02();
    
    hide();

    fopAcM_orderItemEvent(this, 0, 0);
    eventInfo.onCondition(dEvtCnd_CANGETITEM_e);

    mItemId = fopAcM_createItemForTrBoxDemo(&current.pos, m_itemNo, -1, fopAcM_GetRoomNo(this), NULL, NULL);
    JUT_ASSERT(699, mItemId != fpcM_ERROR_PROCESS_ID_e);

    setStatus(STATUS_ORDER_GET_DEMO_e);
    return 1;
}

int daObjLife_c::actionOrderGetDemo() {
    if (eventInfo.checkCommandItem()) {
        setStatus(STATUS_GET_DEMO_e);
        
        if (mItemId != fpcM_ERROR_PROCESS_ID_e) {
            dComIfGp_event_setItemPartnerId(mItemId);
        }
    } else {
        fopAcM_orderItemEvent(this, 0, 0);
        eventInfo.onCondition(dEvtCnd_CANGETITEM_e);
    }

    return 1;
}

int daObjLife_c::actionGetDemo() {
    if (dComIfGp_evmng_endCheck("DEFAULT_GETITEM")) {
        dComIfGp_event_reset();
        fopAcM_delete(this);

        u8 savebit = getSaveBitNo();
        if (savebit != 0xFF) {
            fopAcM_onItem(this, savebit);
        }

        if (m_itemNo == fpcNm_ITEM_KAKERA_HEART) {
            if (strcmp(dComIfGp_getStartStageName(), "F_SP121") == 0) {
                if (fopAcM_GetRoomNo(this) == 0) {
                    if (savebit == 128) {
                        dComIfGs_setEventReg(0xECFF, dComIfGs_getEventReg(0xECFF) | 0x10);
                    }
                } else if (fopAcM_GetRoomNo(this) == 3 && savebit == 130) {
                    dComIfGs_setEventReg(0xECFF, dComIfGs_getEventReg(0xECFF) | 0x4);
                }
            } else if (strcmp(dComIfGp_getStartStageName(), "F_SP109") == 0) {
                if (fopAcM_GetRoomNo(this) == 0 && savebit == 140) {
                    dComIfGs_setEventReg(0xECFF, dComIfGs_getEventReg(0xECFF) | 0x2);
                }
            }
        }
    }

    return 1;
}

int daObjLife_c::actionSwOnWait() {
    if (fopAcM_isSwitch(this, getSwNo()) && cLib_calcTimer<u8>(&field_0x935) == 0) {
        actionWaitInit();
    }

    return 1;
}

int daObjLife_c::actionInitBoomerangCarry() {
    mCcCyl.OnTgSPrmBit(1);
    mCcCyl.OnCoSPrmBit(1);

    f32 height = dItem_data::getH(m_itemNo) * 4.0f;
    f32 radius = dItem_data::getR(m_itemNo) * 4.0f;
    mCcCyl.SetR(radius);
    mCcCyl.SetH(height);

    mCcCyl.OnCoSPrmBit(1);

    setStatus(STATUS_BOOMERANG_CARRY_e);
    return 1;
}

int daObjLife_c::actionBoomerangCarry() {
    fopAc_ac_c* boomerang = fopAcM_SearchByName(PROC_BOOMERANG);
    if (boomerang != NULL) {
        current.pos = boomerang->current.pos;
    } else {
        actionWaitInit();
    }

    // "Heart Container / Piece: Pulling towards\n"
    OS_REPORT("ハートの器・欠片：引き寄せ中\n");
    return 1;
}

int daObjLife_c::actionInitWait2() {
    setStatus(STATUS_WAIT_2_e);
    return 1;
}

int daObjLife_c::actionWait2() {
    attention_info.position = current.pos;
    return 1;
}

void daObjLife_c::calcScale() {
    cLib_chaseF(&field_0x954, 1.0f, 0.2f);
    if (field_0x954 == 1.0f) {
        cLib_chaseF(&field_0x94c, 0.0f, 0.05f);
        field_0x950 = field_0x94c * cM_ssin(field_0x95e * 3000);

        if (field_0x95e < 1000) {
            field_0x95e++;
        }
    } else {
        field_0x950 = 0.0f;
    }

    scale.setall(field_0x950 + field_0x954);
}

int daObjLife_c::execute() {
    static int (daObjLife_c::*l_demoFunc[])() = {
        &daObjLife_c::actionWait,
        &daObjLife_c::actionOrderGetDemo,
        &daObjLife_c::actionGetDemo,
        &daObjLife_c::actionSwOnWait,
        &daObjLife_c::actionBoomerangCarry,
        &daObjLife_c::actionWait2,
    };

    mPrevSpeed = speed;
    mCounter++;

    (this->*l_demoFunc[mStatus])();

    eyePos = current.pos;
    eyePos.y += 30.0f;
    attention_info.position = current.pos;

    calcScale();
    setBaseMtx();

    animPlay(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    if (mCcCyl.ChkTgHit()) {
        cCcD_Obj* hitobj = mCcCyl.GetTgHitObj();
        if (hitobj != NULL && hitobj->ChkAtType(AT_TYPE_HOOKSHOT)) {
            mSound.startCollisionSE(Z2SE_HIT_HOOKSHOT_STICK, 0, NULL);
        }
    }

    if (fopAcM_checkHookCarryNow(this)) {
        cXyz offset(current.pos);
        if (mpModel != NULL) {
            offset.y += mpModel->getModelData()->getJointNodePointer(0)->getMax()->y * 0.5f;
        }

        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mDoMtx_stack_c::inverse();
        mDoMtx_stack_c::multVec(&offset, &offset);
        daPy_getPlayerActorClass()->setHookshotCarryOffset(fopAcM_GetID(this), &offset);
    } else if (mIsHookCarry && fopAcM_searchPlayerDistance(this) < 300.0f) {
        initActionOrderGetDemo();
    }

    mCcCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCcCyl);

    mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mIsHookCarry = fopAcM_checkHookCarryNow(this) != 0;
    return 1;
}

int daObjLife_c::draw() {
    if (!chkDraw()) {
        return 1;
    }

    DrawBase();
    return 1;
}

void daObjLife_c::setListStart() {
    dComIfGd_setListDark();
}

int daObjLife_c::_delete() {
    mSound.deleteObject();
    endEffect00();
    endEffect02();

    DeleteBase(dItem_data::getFieldArc(m_itemNo));
    return 1;
}

static int daObjLife_Draw(daObjLife_c* i_this) {
    return i_this->draw();
}

static int daObjLife_Execute(daObjLife_c* i_this) {
    return i_this->execute();
}

static int daObjLife_Delete(daObjLife_c* i_this) {
    return i_this->_delete();
}

static int daObjLife_Create(fopAc_ac_c* i_this) {
    return ((daObjLife_c*)i_this)->create();
}

static actor_method_class l_daObjLife_Method = {
    (process_method_func)daObjLife_Create,
    (process_method_func)daObjLife_Delete,
    (process_method_func)daObjLife_Execute,
    (process_method_func)NULL,
    (process_method_func)daObjLife_Draw,
};

extern actor_process_profile_definition g_profile_Obj_LifeContainer = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_LifeContainer, // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjLife_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  239,                    // mPriority
  &l_daObjLife_Method,    // sub_method
  0x000C4100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
