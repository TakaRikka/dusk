/**
 * d_a_tbox2.cpp
 * Treasure Chest (Opened state not saved)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tbox2.h"
#include "d/d_bg_w.h"
#include "d/actor/d_a_midna.h"

void daTbox2_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daTbox2_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}


static const res_info l_resInfo[] = {
    {9, 5, 13, 14},   // Small Chest
    {10, 6, 15, 16},  // Big Chest
};

static const cM3dGCylS l_cyl_info[] = {
    {
        {0.0f, 0.0f, -40.0f},  // center
        40.0f,                 // radius
        60.0f,                 // height
    },
    {
        {0.0f, 0.0f, -60.0f},  // center
        50.0f,                 // radius
        70.0f,                 // height
    },
    {
        {0.0f, 0.0f, -60.0f},  // center
        70.0f,                 // radius
        80.0f,                 // height
    },
};

static char* l_arcName = "Tbox2";

static char* l_staff_name = "TREASURE";

static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x1e}, {0x0, 0x0}, 0x79}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},           // mGObjAt
        {dCcD_SE_METAL, 0x2, 0x0, 0x0, 0x3},          // mGObjTg
        {0x0},                                        // mGObjCo
    },                                                // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            0.0f,                // mRadius
            0.0f                 // mHeight
        }                        // mCyl
    }
};

int daTbox2_c::Create() {
    init_actionOpenWait();
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());

    attention_info.flags = fopAc_AttnFlag_JUEL_e;
    attention_info.flags |= fopAc_AttnFlag_UNK_0x400000;

    mAcchCir.SetWall(50.0f, 0.0f);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
              fopAcM_GetSpeed_p(this), &current.angle, &shape_angle);
    fopAcM_SetGravity(this, -6.0f);

    mCcStts2.Init(0xFF, 0xFF, this);
    mCcCyl2.Set(l_cyl_src);
    mCcCyl2.SetStts(&mCcStts2);
    mCcCyl2.cM3dGCyl::Set(l_cyl_info[mModelType]);

    cXyz start(home.pos);
    cXyz end(home.pos);
    start.y += 5.0f;
    end.y -= 30.0f;

    if (!fopAcM_lc_c::lineCheck(&start, &end, this)) {
        // "No-Save Treasure Chest: No BG within 25cm, wait for MoveBG\n"
        OS_REPORT("セーブ無し宝箱：２５ｃｍ以内にＢＧが無いのでMoveBGを待ちます！\n");
        mMode = MODE_EXEC_WAIT_e;
    } else {
        // "No-Save Treasure Chest: Chest location initialization complete!\n"
        OS_REPORT("セーブ無し宝箱：宝箱位置初期設定完了！\n");
        mMode = MODE_EXEC_e;
    }

    return 1;
}

int daTbox2_c::CreateHeap() {
    J3DModelData* modelData =
        (J3DModelData*)dComIfG_getObjectRes(l_arcName, l_resInfo[mModelType].bmd_no);
    JUT_ASSERT(0, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return false;
    }

    J3DAnmTransform* pbck =
        (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, l_resInfo[mModelType].bck_no);
    JUT_ASSERT(0, pbck != NULL);
    mpBck = new mDoExt_bckAnm();
    if (mpBck == NULL || !mpBck->init(pbck, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false)) {
        return false;
    }

    mBoxBgW = new dBgW();
    if (mBoxBgW != NULL &&
        !mBoxBgW->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName, l_resInfo[mModelType].dzb_no),
                      cBgW::MOVE_BG_e, &mBgMtx))
    {
        mBoxBgW->SetCrrFunc(dBgS_MoveBGProc_TypicalRotY);
    } else {
        mBoxBgW = NULL;
        return false;
    }

    return true;
}

int daTbox2_c::create1st() {
    fopAcM_ct(this, daTbox2_c);
    mModelType = getModelType();

    int phase_state = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        u32 heap_size;
        if (mModelType == TYPE_SMALL_e) {
            heap_size = 0x1820;
        } else if (mModelType == TYPE_BIG_e) {
            heap_size = 0x2040;
        }

        phase_state = MoveBGCreate(l_arcName, l_resInfo[mModelType].dzb_id,
                                   dBgS_MoveBGProc_TypicalRotY, heap_size, NULL);
        if (phase_state == cPhs_ERROR_e) {
            return phase_state;
        }
    }

    return phase_state;
}

int daTbox2_c::demoProc() {
    static char* action_table[] = {"WAIT", "OPEN", "APPEAR", "OPEN_SHORT"};
    int act_idx =
        dComIfGp_evmng_getMyActIdx(mStaffIdx, action_table, ARRAY_SIZEU(action_table), 0, 0);

    if (dComIfGp_evmng_getIsAddvance(mStaffIdx)) {
        switch (act_idx) {
        case EVACT_OPEN_e:
            fopAcM_seStart(this, Z2SE_OBJ_TBOX_OPEN_B, 0);
            openInit();
            break;
        case EVACT_OPEN_SHORT_e:
            if (mModelType == TYPE_SMALL_e) {
                fopAcM_seStart(this, Z2SE_OBJ_TBOX_OPEN_A, 0);
            } else {
                fopAcM_seStart(this, Z2SE_OBJ_TBOX_OPEN_B, 0);
            }

            openInit();
            break;
        }
    }

    switch (act_idx) {
    case EVACT_APPEAR_e:
        break;
    case EVACT_OPEN_e:
        if (mpBck->play()) {
            dComIfGp_evmng_cutEnd(mStaffIdx);
        }
        break;
    case EVACT_OPEN_SHORT_e:
        if (mpBck->play()) {
            dComIfGp_evmng_cutEnd(mStaffIdx);
        }
        break;
    default:
        dComIfGp_evmng_cutEnd(mStaffIdx);
        break;
    }

    return 0;
}

void daTbox2_c::openInit() {
    daPy_py_c* player_p = daPy_getPlayerActorClass();

    if (!mReturnRupee) {
        if (mpBgW != NULL) {
            dComIfG_Bgsp().Release(mpBgW);
        }

        if (mBoxBgW != NULL) {
            dComIfG_Bgsp().Regist(mBoxBgW, this);
            mBoxBgW->Move();
        }
    }

    mpBck->setPlaySpeed(1.0f);
}

int daTbox2_c::Execute(Mtx** param_0) {
    mode_proc_call();
    *param_0 = &mBgMtx;
    setBaseMtx();

    if (mBoxBgW != NULL && mBoxBgW->ChkUsed()) {
        mBoxBgW->Move();
    }

    cXyz cyl_center(l_cyl_info[mModelType].mCenter);
    mDoMtx_stack_c::YrotS(shape_angle.y);
    mDoMtx_stack_c::multVec(&cyl_center, &cyl_center);
    cyl_center += current.pos;

    mCcCyl2.SetC(cyl_center);
    dComIfG_Ccsp()->Set(&mCcCyl2);
    return 1;
}

void daTbox2_c::mode_proc_call() {
    typedef void (daTbox2_c::*mode_func)();
    static mode_func l_func[] = {
        &daTbox2_c::mode_exec_wait,
        &daTbox2_c::mode_exec,
    };

    (this->*l_func[mMode])();
}

void daTbox2_c::mode_exec_wait() {
    bool start_exec = false;
    mNoDraw = true;

    cXyz start(home.pos);
    cXyz end(home.pos);
    start.y += 5.0f;
    end.y -= 30.0f;

    if (fopAcM_lc_c::lineCheck(&start, &end, this) && fopAcM_lc_c::checkMoveBG()) {
        start_exec = true;
        home.pos = *fopAcM_lc_c::getCrossP();
        current.pos = home.pos;
    }

    if (start_exec) {
        mNoDraw = false;
        mMode = MODE_EXEC_e;
        mode_exec();
    }
}

void daTbox2_c::mode_exec() {
    action();
    fopAcM_posMoveF(this, NULL);

    mAcch.CrrPos(dComIfG_Bgsp());
    attention_info.position = current.pos;
    eyePos = current.pos;
}

void daTbox2_c::action() {
    typedef void (daTbox2_c::*action_func)();
    static action_func l_func[] = {
        &daTbox2_c::actionOpenWait,
        &daTbox2_c::actionOpenDemo,
        &daTbox2_c::actionNotOpenDemo,
        &daTbox2_c::actionWait,
    };

    (this->*l_func[mAction])();
}

void daTbox2_c::init_actionWait() {
    mAction = ACTION_WAIT_e;
}

void daTbox2_c::actionWait() {}

void daTbox2_c::init_actionNotOpenDemo() {
    mAction = ACTION_NOT_OPEN_DEMO_e;
}

void daTbox2_c::actionNotOpenDemo() {
    if (dComIfGp_evmng_endCheck(eventInfo.getEventId())) {
        dComIfGp_event_reset();
        init_actionOpenWait();
    } else {
        demoProc();
    }
}

void daTbox2_c::init_actionOpenDemo() {
    mAction = ACTION_OPEN_DEMO_e;
}

void daTbox2_c::actionOpenDemo() {
    if (dComIfGp_evmng_endCheck(eventInfo.getEventId())) {
        dComIfGp_event_reset();
        dKy_set_allcol_ratio(1.0f);
        dComIfGp_event_setItemPartner(NULL);

        if (mReturnRupee) {
            mpBck->setFrame(0);
            mpBck->setPlaySpeed(0);
            init_actionOpenWait();
        } else {
            init_actionWait();
        }
    } else {
        demoProc();
    }
}

void daTbox2_c::init_actionOpenWait() {
    mAction = ACTION_OPEN_WAIT_e;
}

void daTbox2_c::actionOpenWait() {
    daMidna_c* midna_p = (daMidna_c*)daPy_py_c::getMidnaActor();  // cast needed for reg alloc
    daPy_py_c* player_p = daPy_getPlayerActorClass();

    if (eventInfo.checkCommandDoor()) {
        dComIfGp_event_onEventFlag(4);

        if (mModelType != TYPE_SMALL_e && daPy_py_c::checkNowWolf() &&
            !midna_p->checkMetamorphoseEnable())
        {
            mStaffIdx = dComIfGp_evmng_getMyStaffId(l_staff_name, NULL, 0);
            demoProc();
            init_actionNotOpenDemo();
        } else {
            mReturnRupee = player_p->checkTreasureRupeeReturn(getItemNo());
            setGetDemoItem();
            mStaffIdx = dComIfGp_evmng_getMyStaffId(l_staff_name, NULL, 0);
            demoProc();
            init_actionOpenDemo();
        }
    } else if (boxCheck()) {
        eventInfo.onCondition(dEvtCnd_CANDOOR_e);

        if (mModelType == TYPE_SMALL_e) {
            eventInfo.setEventName("DEFAULT_TREASURE_SIMPLE");
        } else if (daPy_py_c::checkNowWolf() && !midna_p->checkMetamorphoseEnable()) {
            eventInfo.setEventName("DEFAULT_TREASURE_NOTOPEN");
        } else {
            eventInfo.setEventName("DEFAULT_TREASURE_NORMAL");
        }
    }
}

int daTbox2_c::setGetDemoItem() {
    u8 item_no = getItemNo();

    u32 partner_id;
    if (mReturnRupee) {
        partner_id = fopAcM_createItemForPresentDemo(&current.pos, item_no, 1, -1, -1, NULL, NULL);
    } else {
        partner_id = fopAcM_createItemForTrBoxDemo(&current.pos, item_no, -1, -1, NULL, NULL);
    }

    if (partner_id != -1) {
        dComIfGp_event_setItemPartnerId(partner_id);
    }

    return 1;
}

BOOL daTbox2_c::boxCheck() {
    daPy_py_c* player_p = (daPy_py_c*)dComIfGp_getPlayer(0);

    // Disable box in Link's basement unless lit Lantern is pulled out
    if (strcmp(dComIfGp_getStartStageName(), "R_SP01") == 0 && fopAcM_GetRoomNo(this) == 7 &&
        player_p->getKandelaarFlamePos() == NULL)
    {
        return false;
    }

    cXyz dist_to_box = player_p->attention_info.position - current.pos;
    f32 xz_dist = dist_to_box.abs2XZ();
    f32 y_dist = fabsf(player_p->current.pos.y - current.pos.y);

    if (xz_dist < 22500.0f && fopAcM_seenPlayerAngleY(this) < 0x2000 &&
        fopAcM_seenActorAngleY(player_p, this) < 0x2000 && y_dist < 20.0f)
    {
        return true;
    }

    return false;
}

int daTbox2_c::Draw() {
    if (mNoDraw) {
        return 1;
    }

    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mpBck->entry(mpModel->getModelData());
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daTbox2_c::Delete() {
    if (mBoxBgW != NULL && mBoxBgW->ChkUsed()) {
        dComIfG_Bgsp().Release(mBoxBgW);
    }

    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daTbox2_create1st(daTbox2_c* i_this) {
    fopAcM_ct(i_this, daTbox2_c);
    return i_this->create1st();
}

static int daTbox2_MoveBGDelete(daTbox2_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daTbox2_MoveBGExecute(daTbox2_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daTbox2_MoveBGDraw(daTbox2_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daTbox2_METHODS = {
    (process_method_func)daTbox2_create1st,     (process_method_func)daTbox2_MoveBGDelete,
    (process_method_func)daTbox2_MoveBGExecute, (process_method_func)NULL,
    (process_method_func)daTbox2_MoveBGDraw,
};

actor_process_profile_definition g_profile_TBOX2 = {
    fpcLy_CURRENT_e,
    4,
    fpcPi_CURRENT_e,
    PROC_TBOX2,
    &g_fpcLf_Method.base,
    sizeof(daTbox2_c),
    0,
    0,
    &g_fopAc_Method.base,
    269,
    &daTbox2_METHODS,
    0x44100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
