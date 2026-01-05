/**
 * @file d_a_obj_smallkey.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_smallkey.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/d_item_data.h"
#include "d/d_tresure.h"
#include "f_op/f_op_camera_mng.h"

const static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xffffffff, 0x11}, 0x59}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            20.0f, // mRadius
            40.0f // mHeight
        } // mCyl
    }
};

static void* searchParentSub(void* i_actor, void* i_data) {
    fopAc_ac_c* a_actor = (fopAc_ac_c*)i_actor;
    fopAc_ac_c* a_other = (fopAc_ac_c*)i_data;

    if (a_actor != NULL && fopAcM_IsActor(a_actor)) {
        if (fopAcM_GetProfName(a_actor) == PROC_OBJ_GM) {
            if (a_other->home.pos.abs(a_actor->home.pos) < 1.0f) {
                return a_actor;
            }
        } else if (fopAcM_GetProfName(a_actor) == PROC_E_GB) {
            if (a_other->home.pos.abs(a_actor->home.pos) < 50.0f) {
                return a_actor;
            }

            // “Small Key: position isn't synced with Big Baba!\n”
            OS_REPORT("\x1b[43;30m小さい鍵：デカババと位置がずれています！\n\x1b[m");
        } else if (fopAcM_GetProfName(a_actor) == PROC_E_ZM) {
            f32 other_dist = a_other->home.pos.abs(a_actor->home.pos);
            u8 temp_r27 = fopAcM_GetParamBit(a_actor, 0, 8);
            if (other_dist < 10.0f && temp_r27 < 10) {
                return a_actor;
            }
        }
    }

    return NULL;
}

static f32 Reflect(cXyz* i_vec, cBgS_PolyInfo const& i_polyinfo, f32 i_scale) {
    cM3dGPla plane;

    if (dComIfG_Bgsp().GetTriPla(i_polyinfo, &plane)) {
        cXyz reflect_vec;
        f32 mag = i_vec->absXZ();

        VECReflect(i_vec, &plane.mNormal, &reflect_vec);
        *i_vec = (reflect_vec * mag) * i_scale;
        return i_vec->absXZ();
    }

    return 0.0f;
}

static void keyGetTgCallBack(fopAc_ac_c* i_tgActor, dCcD_GObjInf* i_tgObjInf,
                             fopAc_ac_c* i_atActor, dCcD_GObjInf* i_atObjInf) {
    daKey_c* a_tgActor = (daKey_c*)i_tgActor;

    if (a_tgActor != NULL) {
        if ((i_atObjInf->ChkAtType(AT_TYPE_40) || i_atObjInf->ChkAtType(AT_TYPE_BOOMERANG)) && !dComIfGp_event_runCheck() && !a_tgActor->chkStatus(6)) {
            a_tgActor->actionInitBoomerangCarry();
        }
    }
}

static void keyGetCoCallBack(fopAc_ac_c* i_coActorA, dCcD_GObjInf* i_coObjInfA,
                             fopAc_ac_c* i_coActorB, dCcD_GObjInf* i_coObjInfB) {
    daKey_c* a_coActorA = (daKey_c*)i_coActorA;

    if (a_coActorA != NULL && i_coActorB != NULL) {
        if (fopAcM_GetName(i_coActorB) == fopAcM_GetName(dComIfGp_getLinkPlayer())) {
            a_coActorA->initActionOrderGetDemo();
        }
    }
}

void daKey_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daKey_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::transM(0.0f, 5.0f, 0.0f);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::XrotM(-0x4000);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daKey_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());

    mAcchCir.SetWall(30.0f, 30.0f);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);

    mCcStts.Init(0, 0xFF, this);
    mCcCyl.Set(l_cyl_src);
    mCcCyl.SetStts(&mCcStts);
    mCcCyl.SetCoHitCallback(keyGetCoCallBack);
    mCcCyl.SetTgHitCallback(keyGetTgCallBack);
    mCcCyl.SetR(dItem_data::getR(m_itemNo));
    mCcCyl.SetH(dItem_data::getH(m_itemNo));
    
    fopAcM_SetCullSize(this, fopAc_CULLSPHERE_0_e);

    if (getSwNo() == 0xFF) {
        actionInitInit();
    } else {
        actionInitSwOnWait();
    }

    fopAcM_SetGravity(this, -6.0f);
    fopAcM_OnCarryType(this, fopAcM_CARRY(fopAcM_CARRY_ITEM | fopAcM_CARRY_LIGHT));
    execute();
    return 1;
}

int daKey_c::__CreateHeap() {
    return 1;
}

int daKey_c::create() {
    fopAcM_ct(this, daKey_c);

    if (!mIsPrmInit) {
        field_0x938 = home.angle.x;
        field_0x93a = home.angle.z;
        home.angle.z = 0;
        home.angle.x = 0;
        current.angle.z = 0;
        current.angle.x = 0;
        shape_angle.z = 0;
        shape_angle.x = 0;
        mIsPrmInit = TRUE;
    }

    m_itemNo = fpcNm_ITEM_SMALL_KEY;

    if (strcmp(dComIfGp_getStartStageName(), "F_SP118") == 0) {
        OS_REPORT(" SMKEY 0\n");
        if (dComIfGs_isTbox(getSaveBitNo())) {
            OS_REPORT(" SMKEY 1\n");
            dComIfGp_setItemKeyNumCount(-dComIfGs_getKeyNum());
            dComIfGs_offTbox(getSaveBitNo());
        }
    }

    if (dComIfGs_isTbox(getSaveBitNo())) {
        return cPhs_ERROR_e;
    }

    int phase_state = dComIfG_resLoad(&mPhase, dItem_data::getFieldArc(m_itemNo));
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, CheckFieldItemCreateHeap, 0x840)) {
            return cPhs_ERROR_e;
        }

        if (!Create()) {
            return cPhs_ERROR_e;
        }
    }

    return phase_state;
}

void daKey_c::bg_check() {
    f32 temp_f31 = 0.7f;

    if (mAcch.ChkWallHit() != 0) {
        if (speedF > 5.0f) {
            Reflect(&speed, mAcchCir, 1.0f);
        }
        current.angle.y = cM_atan2s(speed.x, speed.z);
    }

    if (mAcch.ChkGroundLanding()) {
        cXyz sp8(speed);
        if (speedF > 5.0f) {
            Reflect(&sp8, mAcchCir, temp_f31);
        }

        speed.y = -mPrevSpeed.y * temp_f31;
        if (speed.y < 3.0f) {
            fopAcM_SetSpeedF(this, 0.0f);
        }

        field_0x936++;
        fopAcM_seStart(this, Z2SE_OBJ_KEY_BOUND, 2.0f * fabsf(mPrevSpeed.y));
    }
}

int daKey_c::actionInitInit() {
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    mCcCyl.OffTgSPrmBit(1);
    mCcCyl.OffCoSPrmBit(1);
    setStatus(STATUS_INIT_e);
    return 1;
}

int daKey_c::actionInit() {
    void* pparent = fpcM_Search(searchParentSub, this);
    if (pparent != NULL) {
        parentActorID = fopAcM_GetID(pparent);
        if (fopAcM_GetProfName(pparent) == PROC_OBJ_GM || fopAcM_GetProfName(pparent) == PROC_E_ZM) {
            hide();
            actionParentWaitInit();
        } else if (fopAcM_GetProfName(pparent) == PROC_E_GB) {
            show();
            actionInitE_GB();
        }
    } else {
        show();
        actionWaitInit();
    }

    return 1;
}

int daKey_c::actionParentWaitInit() {
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    mCcCyl.OffTgSPrmBit(1);
    mCcCyl.OffCoSPrmBit(1);
    setStatus(STATUS_PARENT_WAIT_e);
    return 1;
}

int daKey_c::actionParentWait() {
    fopAc_ac_c* pparent = (fopAc_ac_c*)fopAcM_SearchByID(parentActorID);
    if (pparent == NULL) {
        show();
        actionWaitInit();
    } else if (fopAcM_GetProfName(pparent) == PROC_E_ZM) {
        current.pos = pparent->home.pos;
    } else {
        current.pos = pparent->current.pos;
    }

    seStartTwinkle_private(Z2SE_OBJ_KEY_TWINKLE_HIDDEN);
    return 1;
}

int daKey_c::actionWaitInit() {
    mCcCyl.OnTgSPrmBit(1);
    mCcCyl.OffCoSPrmBit(1);
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    attention_info.distances[fopAc_attn_CARRY_e] = 16;
    attention_info.position = current.pos;

    effectSet();
    setStatus(STATUS_WAIT_e);
    return 1;
}

int daKey_c::actionWait() {
    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());
    bg_check();

    if (mAcch.ChkGroundHit()) {
        cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);

        speedF *= 0.9f;
        if (speedF < 1.0f) {
            speedF = 0.0f;
        }
    } else {
        cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    }

    daPy_py_c* player = daPy_getPlayerActorClass();
    if (fopAcM_checkCarryNow(this)) {
        cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
        if (player->getGrabUpStart()) {
            fopAcM_cancelCarryNow(this);
            initActionOrderGetDemo();
        }
    } else {
        cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    }

    if (field_0x936 == 0) {
        shape_angle.x += 4000;
    } else {
        shape_angle.x = 0;
    }

    attention_info.position = current.pos;
    seStartTwinkle_private(Z2SE_OBJ_KEY_TWINKLE);
    return 1;
}

int daKey_c::initActionOrderGetDemo() {
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    mCcCyl.OffTgSPrmBit(1);
    mCcCyl.OffCoSPrmBit(1);
    hide();
    effectStop();

    fopAcM_orderItemEvent(this, 0, 0);
    eventInfo.onCondition(8);

    mItemId = fopAcM_createItemForTrBoxDemo(&current.pos, m_itemNo, -1, fopAcM_GetRoomNo(this), NULL, NULL);
    JUT_ASSERT(699, mItemId != fpcM_ERROR_PROCESS_ID_e);

    setStatus(STATUS_ORDER_GET_DEMO_e);
    return 1;
}

int daKey_c::actionOrderGetDemo() {
    if (eventInfo.checkCommandItem()) {
        setStatus(STATUS_GET_DEMO_e);
        if (mItemId != fpcM_ERROR_PROCESS_ID_e) {
            dComIfGp_event_setItemPartnerId(mItemId);
        }
    } else {
        fopAcM_orderItemEvent(this, 0, 0);
        eventInfo.onCondition(8);
    }

    return 1;
}

int daKey_c::actionGetDemo() {
    if (dComIfGp_evmng_endCheck("DEFAULT_GETITEM")) {
        dComIfGs_onTbox(getSaveBitNo());
        dTres_c::offStatus(getSaveBitNo(), 1);
        dComIfGp_event_reset();
        fopAcM_delete(this);
    }

    return 1;
}

int daKey_c::actionInitSwOnWait() {
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    mCcCyl.OffTgSPrmBit(1);
    mCcCyl.OffCoSPrmBit(1);
    hide();

    if (getSwOnWaitTimer() == 63) {
        mSwOnTimer = 15;
    } else {
        mSwOnTimer = getSwOnWaitTimer() + 1;
    }

    if (fopAcM_isSwitch(this, getSwNo())) {
        mSwOnTimer = 0;
    }

    setStatus(STATUS_SW_ON_WAIT_e);
    return 1;
}

int daKey_c::actionSwOnWait() {
    if (fopAcM_isSwitch(this, getSwNo()) && cLib_calcTimer<u8>(&mSwOnTimer) == 0) {
        show();
        actionWaitInit();
    }

    seStartTwinkle_private(Z2SE_OBJ_KEY_TWINKLE_HIDDEN);
    return 1;
}

int daKey_c::actionInitBoomerangCarry() {
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    mCcCyl.OnTgSPrmBit(1);
    mCcCyl.OnCoSPrmBit(1);

    u8 height = dItem_data::getH(m_itemNo);
    u8 radius = dItem_data::getR(m_itemNo);

    mCcCyl.SetR(4.0f * radius);
    mCcCyl.SetH(4.0f * height);
    mCcCyl.OnCoSPrmBit(1);

    setStatus(STATUS_BOOMERANG_CARRY_e);
    return 1;
}

int daKey_c::actionBoomerangCarry() {
    fopAc_ac_c* pboomerang = fopAcM_SearchByName(PROC_BOOMERANG);
    if (pboomerang != NULL) {
        current.pos = pboomerang->current.pos;
    } else {
        // "Small Key: can't find boomerang!!!\n"
        OS_REPORT_ERROR("小さいカギ：ブーメランが見つかりません！！！\n");
    }

    seStartTwinkle_private(Z2SE_OBJ_KEY_TWINKLE);
    // "Small Key: carrying\n"
    OS_REPORT("小さい鍵：引き寄せ中\n");
    return 1;
}

void daKey_c::effectSet() {
    if (mEffect[0].getEmitter() == NULL) {
        cXyz eff_scale[] = {  // needs to be an array to match...
            cXyz(1.0f, 1.0f, 1.0f)
        };
        mEffRot = shape_angle;
        mEffRot.x = -0x4000;

        dComIfGp_particle_set(0x82BE, &current.pos, &mEffRot, &eff_scale[0], 0xFF, &mEffect[0], fopAcM_GetRoomNo(this), NULL, NULL, NULL);
        dComIfGp_particle_set(0x82BF, &current.pos, &mEffRot, &eff_scale[0], 0xFF, &mEffect[1], fopAcM_GetRoomNo(this), NULL, NULL, NULL);
    }
}

void daKey_c::effectStop() {
    mEffect[0].remove();
    mEffect[1].remove();
}

void daKey_c::effectCtrl() {
    cXyz eff_scale;
    f32 cam_dist = current.pos.abs(dComIfGp_getCamera(0)->lookat.eye);
    f32 var_f29 = 1000.0f;
    f32 max_size = 3.0f;
    f32 size = 1.0f;

    if (0.0f != var_f29) {
        size = cam_dist / var_f29;
        if (size < 1.0f) {
            size = 1.0f;
        }
    }

    if (max_size < 1.0f) {
        max_size = 1.0f;
    }

    if (size > max_size) {
        size = max_size;
    }

    eff_scale.setall(size);

    for (int i = 0; i < 2; i++) {
        JPABaseEmitter* pemitter = mEffect[i].getEmitter();
        if (pemitter != NULL) {
            pemitter->setGlobalScale(eff_scale);
        }
    }
}

void daKey_c::seStartTwinkle_private(u32 i_soundId) {
    if (field_0x984 == 0) {
        fopAcM_seStartLevel(this, i_soundId, 0);
    }
}

int daKey_c::actionInitE_GB() {
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    mCcCyl.OffTgSPrmBit(1);
    mCcCyl.OffCoSPrmBit(1);
    effectSet();
    fopAcM_SetSpeedF(this, 0.0f);
    fopAcM_SetGravity(this, 0.0f);
    setStatus(STATUS_E_GB_e);
    return 1;
}

int daKey_c::actionE_GB() {
    seStartTwinkle_private(Z2SE_OBJ_KEY_TWINKLE_HIDDEN);
    return 1;
}

int daKey_c::execute() {
    static int (daKey_c::*l_demoFunc[])() = {
        &daKey_c::actionInit,
        &daKey_c::actionParentWait,
        &daKey_c::actionWait,
        &daKey_c::actionOrderGetDemo,
        &daKey_c::actionGetDemo,
        &daKey_c::actionSwOnWait,
        &daKey_c::actionBoomerangCarry,
        &daKey_c::actionE_GB,
    };

    mPrevSpeed = speed;
    field_0x930++;

    (this->*l_demoFunc[mStatus])();

    eyePos = current.pos;
    attention_info.position = current.pos;

    effectCtrl();
    setBaseMtx();

    mCcCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCcCyl);

    dTres_c::setPosition(getSaveBitNo(), &current.pos);
    field_0x984 = 0;
    return 1;
}

int daKey_c::draw() {
    if (!chkDraw()) {
        return 1;
    }

    DrawBase();
    return 1;
}

int daKey_c::_delete() {
    effectStop();
    DeleteBase(dItem_data::getFieldArc(m_itemNo));
    return 1;
}

static int daKey_Draw(daKey_c* i_this) {
    return i_this->draw();
}

static int daKey_Execute(daKey_c* i_this) {
    return i_this->execute();
}

static int daKey_Delete(daKey_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);
    return i_this->_delete();
}

static int daKey_Create(fopAc_ac_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);
    return ((daKey_c*)i_this)->create();
}

static actor_method_class l_daKey_Method = {
    (process_method_func)daKey_Create,
    (process_method_func)daKey_Delete,
    (process_method_func)daKey_Execute,
    (process_method_func)NULL,
    (process_method_func)daKey_Draw,
};

actor_process_profile_definition g_profile_Obj_SmallKey = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Obj_SmallKey,      // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daKey_c),        // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    237,                    // mPriority
    &l_daKey_Method,        // sub_method
    0x00040100,             // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
