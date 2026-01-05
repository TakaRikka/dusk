/**
 * @file d_a_obj_smgdoor.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_smgdoor.h"
#include "d/actor/d_a_player.h"
#include "d/d_meter2_info.h"
#include "d/d_s_play.h"

#if DEBUG
class daObjSmgDoor_HIO_c : public mDoHIO_entry_c {
public:
    daObjSmgDoor_HIO_c();
    void genMessage(JORMContext*);

public:
    /* 0x06 */ u8 field_0x6;
};

daObjSmgDoor_HIO_c l_HIO;

daObjSmgDoor_HIO_c::daObjSmgDoor_HIO_c() {
    field_0x6 = 0;
}

void daObjSmgDoor_HIO_c::genMessage(JORMContext* ctx) {
    // Present & Past Door
    ctx->genLabel("現在＆過去の扉", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    // Check Display
    ctx->genCheckBox("チェック表示", &field_0x6, 1, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

static int eventCallBack(void* a_this, int param_1) {
    daObjSmgDoor_c* i_this = (daObjSmgDoor_c*)a_this;
    i_this->sceneChange();
    return 1;
}

void daObjSmgDoor_c::initBaseMtx() {
    mpModel[0]->setBaseScale(scale);
    mpModel[1]->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    cMtx_copy(mDoMtx_stack_c::get(), field_0x5b8);
    setBaseMtx();
}

void daObjSmgDoor_c::setBaseMtx() {
    cXyz cStack_18(-145.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::YrotS(current.angle.y);
    mDoMtx_stack_c::multVec(&cStack_18, &cStack_18);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::transM(cStack_18);
    mDoMtx_stack_c::YrotM(field_0x5b0);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpModel[0]->setBaseTRMtx(mDoMtx_stack_c::get());
    cStack_18.set(145.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::YrotS(current.angle.y);
    mDoMtx_stack_c::multVec(&cStack_18, &cStack_18);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::transM(cStack_18);
    mDoMtx_stack_c::YrotM(field_0x5b2);
    mDoMtx_stack_c::YrotM(0x7fff);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpModel[1]->setBaseTRMtx(mDoMtx_stack_c::get());
    cMtx_copy(field_0x5b8, mBgMtx);
}

static char* l_arcName[2] = {"A_SMGDoor", "A_SMKDoor"};

static char* l_bmd[2] = {
    "A_SMGDoor.bmd",
    "A_SMKDoor.bmd",
};

static char* l_dzb[2] = {
    "A_SMGDoor.dzb",
    "A_SMKDoor.dzb",
};

static char* l_eventName[2] = {
    "NOW_DOOR_IN",
    "OLD_DOOR_IN",
};

static cull_box l_cull_box = {{-200.0f, 0.0f, -50.0f}, {200.0f, 400.0f, 50.0f}};

int daObjSmgDoor_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, field_0x5b8);
    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z, l_cull_box.max.x,
                          l_cull_box.max.y, l_cull_box.max.z);
    attention_info.position.y += 150.0f;
    eyePos.y += 150.0f;
    attention_info.flags = fopAc_AttnFlag_DOOR_e;
    setAction(ACT_WAIT);
    eventInfo.setArchiveName(l_arcName[mType]);
    mToolId = getEvId();
    mEvId = (s16)dComIfGp_getEventManager().getEventIdx(this, l_eventName[mType], 0xFF);
    OS_REPORT("mEvId<%d>mToolId<%d>\n", mEvId, mToolId);
    OS_REPORT("swbit<%d>\n", getSwbit());
    return 1;
}

int daObjSmgDoor_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName[mType], l_bmd[mType]);
    JUT_ASSERT(252, modelData != NULL);

    for (int i = 0; i < 2; i++) {
        mpModel[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (mpModel[i] == NULL)
            return 0;
    }

    return 1;
}

int daObjSmgDoor_c::create1st() {
    mType = getType();
    int phase = dComIfG_resLoad(&mPhase, l_arcName[mType]);

    if (phase == cPhs_COMPLEATE_e) {
        int dzb_id = dComIfG_getObjctResName2Index(l_arcName[mType], l_dzb[mType]);
        JUT_ASSERT(287, dzb_id != -1);

        phase = MoveBGCreate(l_arcName[mType], dzb_id, NULL, 0x1660, NULL);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
    }

#if DEBUG
    // Present or Past Door
    l_HIO.entryHIO("現在or過去の扉");
#endif
    return phase;
}

int daObjSmgDoor_c::Execute(Mtx** param_0) {
    event_proc_call();
    *param_0 = &mBgMtx;
    setBaseMtx();
    return 1;
}

int daObjSmgDoor_c::getDemoAction() {
    static char* action_table[6] = {
        "WAIT", "OPEN", "SETGOAL", "SETPOS", "SCENE_CHG", "COLOR_CHG",
    };

    return dComIfGp_evmng_getMyActIdx(mStaffId, action_table, 6, 0, 0);
}

int daObjSmgDoor_c::demoProc() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    mStaffId = dComIfGp_evmng_getMyStaffId("smgdoor", 0, 0);
    int demo_action = getDemoAction();

    if (dComIfGp_evmng_getIsAddvance(mStaffId) != 0) {
        switch (demo_action) {
        case 0: {
            int* intP = dComIfGp_evmng_getMyIntegerP(mStaffId, "Timer");
            if (intP == NULL) {
                field_0x5e9 = 1;
            } else {
                field_0x5e9 = *intP;
            }
            break;
        }
        case 1:
            openInit();
            break;
        case 2:
            setGoal();
            break;
        case 5:
            dKy_change_colpat(3);
            field_0x5f2 = 0;
        }
    }

    switch (demo_action) {
    case 0:
        if (cLib_calcTimer(&field_0x5e9) == 0) {
            dComIfGp_evmng_cutEnd(mStaffId);
        }
        break;
    case 1:
        if (openProc() != 0) {
            dComIfGp_evmng_cutEnd(mStaffId);
        }
        break;
    case 4:
        sceneChange();
        break;
    case 5:
        if (field_0x5f2 == KREG_S(0)) {
            fopAcM_seStart(this, Z2SE_OBJ_KAKO_DOOR_OP_LIGHT, 0);
            OS_REPORT("LIGHT SE!\n");
            dComIfGp_evmng_cutEnd(mStaffId);
        }

        field_0x5f2++;
        break;
    default:
        dComIfGp_evmng_cutEnd(mStaffId);
    }

    return 0;
}

int daObjSmgDoor_c::openInit() {
    if (mpBgW != NULL && mpBgW->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgW);
    }

    if (mType == 0) {
        fopAcM_seStart(this, Z2SE_OBJ_KAKO_DOOR_OP_IN, 0);
    } else if (mType == 1) {
        fopAcM_seStart(this, Z2SE_OBJ_KAKO_DOOR_OP_OUT, 0);
    }

    field_0x5ee = 0;
    field_0x5f0 = 0;
    field_0x5b0 = 0;
    field_0x5b2 = 0;
    field_0x5f2 = 0;
    return 1;
}

int daObjSmgDoor_c::openProc() {
    int ret = 0;
    cLib_chaseS(&field_0x5ee, 0x3c, 3);
    cLib_chaseS(&field_0x5f0, 0x96, field_0x5ee);

    switch (mType) {
    case 0:
        cLib_chaseS(&field_0x5b0, -0x5000, field_0x5f0);
        ret = cLib_chaseS(&field_0x5b2, 0x5000, field_0x5f0);
        break;
    case 1:
        cLib_chaseS(&field_0x5b0, 0x5000, field_0x5f0);
        ret = cLib_chaseS(&field_0x5b2, -0x5000, field_0x5f0);
        if (field_0x5f2 == 0x46) {
            fopAcM_seStart(this, Z2SE_OBJ_KAKO_DOOR_OP_LIGHT, 0);
            dKy_change_colpat(3);
            OS_REPORT("COLPAT CHANGE!!!!\n");
        }

        field_0x5f2++;
    }

    return ret;
}

void daObjSmgDoor_c::setGoal() {
    cXyz cStack_18(0.0f, 0.0f, 60.0f);

    if (mType == 1) {
        cStack_18.set(0.0f, 0.0f, 0.0f);
    }

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&cStack_18, &cStack_18);
    dComIfGp_evmng_setGoal(&cStack_18);
}

void daObjSmgDoor_c::setPos() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz acStack_28(0.0f, 0.0f, 300.0f);

    if (mType == 1) {
        acStack_28.set(0.0f, 0.0f, 250.0f);
    }

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&acStack_28, &acStack_28);
    player->setPlayerPosAndAngle(&acStack_28, current.angle.y + 0x7fff, 0);
    return;
}

void daObjSmgDoor_c::sceneChange() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    player->onSceneChangeArea(getNextSceneID(), 0xff, 0);
    // Present & Past Door: Scene Change Destination <%d>
    OS_REPORT("現在＆過去の扉：シーン切替先<%d>\n", getNextSceneID());
}

void daObjSmgDoor_c::event_proc_call() {
    static actionFunc l_func[] = {&daObjSmgDoor_c::actionWaitEvent, &daObjSmgDoor_c::actionEvent,
                                  &daObjSmgDoor_c::actionDead};

    (this->*(l_func[mAction]))();
}

void daObjSmgDoor_c::actionWaitEvent() {
    if (eventInfo.checkCommandDoor()) {
        attention_info.position.y = home.pos.y;

        if (mType == 1) {
            shape_angle.y += 0x7fff;
        }
        setAction(1);
        setPos();
    } else {
        if (checkOpen() != 0) {
            eventInfo.setEventId(mEvId);
            eventInfo.setMapToolId(mToolId);
            eventInfo.onCondition(dEvtCnd_CANDOOR_e);
        }
    }
}

void daObjSmgDoor_c::actionEvent() {
    dMeter2Info_onGameStatus(2);
    dComIfGp_getEvent()->setSkipProc(this, eventCallBack, 0);
    demoProc();
}

void daObjSmgDoor_c::actionDead() {}

int daObjSmgDoor_c::checkArea() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz player_to_door_vec;
    cXyz player_attention_pos(player->attention_info.position);
    player_attention_pos.y = player->current.pos.y;
    player_to_door_vec = player_attention_pos - current.pos;
    f32 sq_distance_xz = player_to_door_vec.abs2XZ();
    cXyz door_forward_vec;
    door_forward_vec.set(cM_ssin(home.angle.y), 0.0f, cM_scos(home.angle.y));

    if (sq_distance_xz > 62500.0f) {
        return 0;
    }

    player_to_door_vec.normalize();
    f32 dot_product_scaled = player_to_door_vec.inprodXZ(door_forward_vec);
    dot_product_scaled *= (sq_distance_xz * dot_product_scaled);

    if (dot_product_scaled > 90000.0f) {
        return 0;
    }
    
    if (sq_distance_xz - dot_product_scaled > 12100.0f) {
        return 0;
    }
    
    if (abs((s16)(current.angle.y - player->current.angle.y)) < 0x5000) {
        return 0;
    } 
    
    return 1;
}

int daObjSmgDoor_c::checkOpen() {
    if (getSwbit() != 0xff) {
        if (fopAcM_isSwitch(this, getSwbit()) == 0) {
            return 0;
        }
    }

    if (daPy_py_c::checkNowWolf()) {
        return 0;
    }
    
    if (checkArea() == 0) {
        return 0;
    } 

    return 1;
}

int daObjSmgDoor_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel[0], &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel[1], &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel[0]);
    mDoExt_modelUpdateDL(mpModel[1]);
    dComIfGd_setList();

#if DEBUG
    if (l_HIO.field_0x6 != 0) {
        mpBgW->CalcPlane();
    }
#endif

    return 1;
}

int daObjSmgDoor_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName[mType]);
#if DEBUG
    l_HIO.removeHIO();
#endif
    return 1;
}

static int daObjSmgDoor_create1st(daObjSmgDoor_c* i_this) {
    fopAcM_ct(i_this, daObjSmgDoor_c);
    return i_this->create1st();
}

static int daObjSmgDoor_MoveBGDelete(daObjSmgDoor_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjSmgDoor_MoveBGExecute(daObjSmgDoor_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjSmgDoor_MoveBGDraw(daObjSmgDoor_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjSmgDoor_METHODS = {
    (process_method_func)daObjSmgDoor_create1st,     (process_method_func)daObjSmgDoor_MoveBGDelete,
    (process_method_func)daObjSmgDoor_MoveBGExecute, 0,
    (process_method_func)daObjSmgDoor_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_SmgDoor = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_SmgDoor,        // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daObjSmgDoor_c),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    553,                     // mPriority
    &daObjSmgDoor_METHODS,   // sub_method
    0x00040100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
