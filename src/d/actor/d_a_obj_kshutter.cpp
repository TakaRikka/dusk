/**
 * @file d_a_obj_kshutter.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_kshutter.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_obj_keyhole.h"
#include "d/d_meter2_info.h"

enum S_shut00_RES_File_ID {
    /* BMDR */
    /* 0x5 */ BMDR_S_SHUT_ROU = 0x5,

    /* DZB */
    /* 0x8 */ DZB_S_SHUT00_S_SHUT_ROU = 0x8,

    /* EVT */
    /* 0xB */ EVT_S_SHUT00_EVENT_LIST = 0xB,
};

enum Lv3shut00_RES_File_ID {
    /* BMDR */
    /* 0x5 */ BMDR_DOOR_SHUTTER_00 = 0x5,

    /* DZB */
    /* 0x8 */ DZB_LV3SHUT00_S_SHUT_ROU = 0x8,

    /* EVT */
    /* 0xB */ EVT_LV3SHUT00_EVENT_LIST = 0xB,
};

enum K_l3bdoor_RES_File_ID {
    /* BMDR */
    /* 0x5 */ BMDR_K_L3BDOOR = 0x5,

    /* DZB */
    /* 0x8 */ DZB_K_L3BDOOR = 0x8,

    /* EVT */
    /* 0xB */ EVT_K_L3BDOOR_EVENT_LIST = 0xB,
};

enum V_Shutter_RES_File_ID {
    /* BCK */
    /* 0x5 */ BCK_V_SHUTTER = 0x5,

    /* BMDR */
    /* 0x8 */ BMDR_V_SHUTTER = 0x8,

    /* DZB */
    /* 0xB */ DZB_V_SHUTTER = 0xB,
};

enum DoorY00_RES_File_ID {
    /* BCK */
    /* 0x4 */ BCK_MD_OJ_DOOROPC = 0x4,
    /* 0x5 */ BCK_OJ_DOORCLOSEC,
    /* 0x6 */ BCK_OJ_DOOROPC,

    /* EVT */
    /* 0x9 */ EVT_DOORY00_EVENT_LIST = 0x9,
};

enum DoorT00_RES_File_ID {
    /* BCK */
    /* 0x4 */ BCK_MD_OJ_DOOROPD = 0x4,
    /* 0x5 */ BCK_OJ_DOORCLOSED,
    /* 0x6 */ BCK_OJ_DOOROPD,

    /* EVT */
    /* 0x9 */ EVT_DOORT00_EVENT_LIST = 0x9,
};

enum Demo_Action {
    /* 0x0 */ DEMO_ACT_WAIT,
    /* 0x1 */ DEMO_ACT_ADJUSTMENT,
    /* 0x2 */ DEMO_ACT_UNLOCK,
    /* 0x3 */ DEMO_ACT_OPEN,
    /* 0x4 */ DEMO_ACT_PLY_NODISP,
    /* 0x5 */ DEMO_ACT_PLY_DISP,
};

enum Type {
    /* 0x0 */ TYPE_0,
    /* 0x1 */ TYPE_1,
    /* 0x2 */ TYPE_2,
    /* 0x3 */ TYPE_L3BOSS,
    /* 0x4 */ TYPE_V_SHUTTER,
};

void daObjKshtr_c::initBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    MTXCopy(mDoMtx_stack_c::get(), mMtx);
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjKshtr_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + field_0x5fc, current.pos.z);

    if (mType == TYPE_V_SHUTTER) {
        mDoMtx_stack_c::YrotM(shape_angle.y);
    } else {
        mDoMtx_stack_c::YrotM(shape_angle.y + 0x7FFF);
    }

    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);

    if (mpBckAnm != NULL) {
        mDoMtx_stack_c::copy(mpModel->getAnmMtx(0));
        mDoMtx_stack_c::multVecZero(&mSePos);
    }
}

static int const l_bmd[5] = {
    BMDR_S_SHUT_ROU, BMDR_S_SHUT_ROU, BMDR_DOOR_SHUTTER_00, BMDR_K_L3BDOOR, BMDR_V_SHUTTER,
};

static int const l_dzb[5] = {
    DZB_S_SHUT00_S_SHUT_ROU, DZB_S_SHUT00_S_SHUT_ROU, DZB_LV3SHUT00_S_SHUT_ROU, DZB_K_L3BDOOR, DZB_V_SHUTTER,
};

static u32 const l_heap_size[5] = {
    0x1300, 0x1300, 0x1300, 0x1300, 0x20D0,
};

static cull_box const l_cull_box = {
    {-450.0f, 0.0f, -50.0f}, {450.0f, 700.0f, 150.0f},
};

static char* l_arcName[5] = {
    "S_shut00",
    "S_shut00",
    "Lv3shut00",
    "K_l3bdoor",
    "V_Shutter",
};

static char* l_anmName[10] = {
    "oj_DoorOpC.bck",
    "md_oj_DoorOpC.bck",
    "oj_DoorOpC.bck",
    "md_oj_DoorOpC.bck",
    "oj_DoorOpD.bck",
    "md_oj_DoorOpD.bck",
    NULL,
    NULL,
    "V_Shutter.bck",
    "V_Shutter.bck",
};

static char* l_eventName[10] = {
    "KEY_JAIL_00",
    "KEY_JAIL_WOLF_00",
    "KEY_JAIL_00",
    "KEY_JAIL_WOLF_00",
    "KEY_JAIL_01",
    "KEY_JAIL_WOLF_01",
    "DEFAULT_BS_SHUTTER_L3_F",
    "DEFAULT_BS_SHUTTER_L3_F",
    NULL,
    NULL,
};

int daObjKshtr_c::Create() {
    if (!fopAcM_isSwitch(this, mSwNo)) {
        field_0x5ec = false;
        field_0x5fc = 0.0f;
        attention_info.position.y += 150.0f;
        eyePos.y += 150.0f;
        attention_info.flags = 0x20;

        if (mIsCheckKey) {
            mAction = 0;
        } else {
            mAction = 0;
        }

        if (mpBckAnm != NULL) {
            mpBckAnm->setFrame(0.0f);
            mpBckAnm->setPlaySpeed(0.0f);
        }
    } else {
        field_0x5ec = true;

        if ((s8)mType != TYPE_L3BOSS) {
            mpBckAnm->setFrame(mpBckAnm->getEndFrame());
        } else {
            field_0x5fc = -450.0f;
        }

        offDzb();

        if (mIsCheckKey) {
            mAction = 2;
        } else {
            mAction = 2;
        }
    }

    if (mType == TYPE_V_SHUTTER) {
        fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x100);
    }

    initBaseMtx();
    fopAcM_SetMtx(this, mMtx);
    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z, l_cull_box.max.x, l_cull_box.max.y, l_cull_box.max.z);

    if (checkKey() == 1) {
        eventInfo.setArchiveName(l_arcName[mType]);

        for (int i = 0; i < 2; i++) {
            field_0x5f0[i] = dComIfGp_getEventManager().getEventIdx(this, l_eventName[i + mType * 2], 0xFF);
        }

        mEvId = getEvId();
    } else {
        mEvId = getEvId();
        field_0x5f0[0] = dComIfGp_getEventManager().getEventIdx(this, mEvId);
        field_0x5f0[1] = -1;
    }

    return 1;
}

void daObjKshtr_c::offDzb() {
    if (mpBgW != NULL && mpBgW->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgW);
    }
}

static char* l_anmArcName[5] = {
    "DoorY00",
    "DoorY00",
    "DoorT00",
    NULL,
    "V_Shutter",
};

int daObjKshtr_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName[mType], l_bmd[mType]);

    JUT_ASSERT(429, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    if (l_anmArcName[mType] != NULL) {
        int index;
        if (daPy_py_c::checkNowWolf()) {
            index = mType * 2 + 1;
        } else {
            index = mType << 1;
        }

        J3DAnmTransform* anm = (J3DAnmTransform*)dComIfG_getObjectRes(l_anmArcName[mType], l_anmName[index]);

        JUT_ASSERT(446, anm != NULL);

        mpBckAnm = new mDoExt_bckAnm();
        if (mpBckAnm == NULL || mpBckAnm->init(anm, 1, 0, 1.0f, 0, -1, false) == 0) {
            return 0;
        }
    }

    #if DEBUG
    if (checkKey() != 0) {
        mIsCheckKey = true;
    }
    #endif

    return 1;
}

void daObjKshtr_c::initKey() {
    if (!field_0x61e) {
        if ((int)checkKey() != 0) {
            mIsCheckKey = true;
        }

        field_0x61e = true;
        mKeyHoleId = fpcM_ERROR_PROCESS_ID_e;

        if (!fopAcM_isSwitch(this, mSwNo) && mIsCheckKey) {
            cXyz sp20(0.0f, 0.0f, 35.0f);

            u32 uVar1 = 0xFFFFFFFF;

            if (mType == TYPE_L3BOSS) {
                uVar1 = 0xFFFFFF03;
            } else {
                uVar1 = 0xFFFFFFFF;
            }

            mKeyHoleId = fopAcM_createChildFromOffset(PROC_OBJ_KEYHOLE, fopAcM_GetID(this), uVar1, &sp20, fopAcM_GetRoomNo(this), NULL, &scale, -1, NULL);
        }
    }
}

cPhs__Step daObjKshtr_c::phase_0() {
    if (!field_0x619) {
        mHomeAngleZ = home.angle.z;
        mHomeAngleX = home.angle.x;
        shape_angle.x = 0;
        current.angle.x = 0;
        home.angle.x = 0;
        shape_angle.z = 0;
        current.angle.z = 0;
        home.angle.z = 0;
        field_0x619 = true;
    }

    mType = getType() + 1;
    mSwNo = getSwNo();

    #if DEBUG
    if (mSwNo == 0xFF) {
        OS_REPORT_ERROR("鍵付き壁ドア：スイッチ指定がありません\n"); // Locked wall door: No switch specification
        return cPhs_ERROR_e;
    }
    #else
    initKey();
    #endif

    #if DEBUG
    if (mType >= 6) {
        OS_REPORT_ERROR("鍵付き壁ドア：引数０のタイプ指定が不正値です\n"); // Locked wall door: The type specification for argument 0 is invalid

        return cPhs_ERROR_e;
    }
    #endif

    cPhs__Step phase;
    if (l_anmArcName[mType] != NULL) {
        phase = (cPhs__Step)dComIfG_resLoad(&mPhase2, l_anmArcName[mType]);
        if (phase != cPhs_COMPLEATE_e) {
                return phase;
        }
    }

    if (dComIfG_resLoad(&mPhase1, l_arcName[mType]) == cPhs_COMPLEATE_e) {
        phase = (cPhs__Step)MoveBGCreate(l_arcName[mType], l_dzb[mType], NULL, l_heap_size[mType], NULL);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }

        mPhase++;
        OS_REPORT("KSHTR PARAM:0x%x\n", fopAcM_GetParam(this));
        return cPhs_INIT_e;
    }
}

cPhs__Step daObjKshtr_c::phase_1() {
    if (mKeyHoleId != fpcM_ERROR_PROCESS_ID_e) {
        obj_keyhole_class* keyhole_p = (obj_keyhole_class*)fopAcM_SearchByID(mKeyHoleId);

        if (keyhole_p != NULL) {
            if (fopAcM_isSwitch(this, mSwNo)) {
                keyhole_p->setOpenEnd();

                OS_REPORT("鍵檻：鍵開いてる状態セット！\n"); // Locked cage: unlocked!\n
            }

            mPhase++;
        }
    } else {
        mPhase++;
    }

    return cPhs_INIT_e;
}

cPhs__Step daObjKshtr_c::phase_2() {
    return cPhs_COMPLEATE_e;
}

cPhs__Step daObjKshtr_c::create1st() {
    static daObjKshtr_c::PhaseFunc l_ct_func[3] = {
        &daObjKshtr_c::phase_0,
        &daObjKshtr_c::phase_1,
        &daObjKshtr_c::phase_2,
    };

    return (this->*l_ct_func[mPhase])();
}

void daObjKshtr_c::event_proc_call() {
    static daObjKshtr_c::ActionFunc l_func[3] = {
        &daObjKshtr_c::actionWaitEvent,
        &daObjKshtr_c::actionEvent,
        &daObjKshtr_c::actionDead,
    };

    (this->*l_func[mAction])();
}

void daObjKshtr_c::event_proc_call2() {
    static daObjKshtr_c::ActionFunc l_func[4] = {
        &daObjKshtr_c::actionWaitEvent2,
        &daObjKshtr_c::actionEvent2,
        &daObjKshtr_c::actionDead2,
        &daObjKshtr_c::actionOpen,
    };

    (this->*l_func[mAction])();
}

int daObjKshtr_c::Execute(Mtx** param_1) {
    if (checkKey() == 1) {
        event_proc_call();
    } else {
        event_proc_call2();
    }

    *param_1 = &mBgMtx;
    setBaseMtx();

    return 1;
}

BOOL daObjKshtr_c::checkArea() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp78;
    cXyz sp84(player->attention_info.position);
    sp84.y = player->current.pos.y;
    sp78 = sp84 - current.pos;
    f32 fVar1 = sp78.abs2XZ();
    cXyz sp90;
    sp90.set(cM_ssin(home.angle.y), 0.0f, cM_scos(home.angle.y));

    if (fVar1 > 62500.0f) {
        return FALSE;
    }

    sp78.normalize();

    f32 fVar2 = sp78.inprodXZ(sp90);
    fVar2 *= fVar1 * fVar2;
    if (fVar2 > 12100.0f) {
        return FALSE;
    }

    if (fVar1 - fVar2 > 12100.0f) {
        return FALSE;
    }

    if (abs((s16)(current.angle.y - player->current.angle.y)) >= 0x5000) {
        return TRUE;
    }

    return FALSE;
}

BOOL daObjKshtr_c::checkOpen() {
    if (mType == TYPE_L3BOSS) {
        if (!dComIfGs_isDungeonItemBossKey()) {
            return false;
        }
    } else if (dComIfGs_getKeyNum() == 0) {
        return false;
    }

    if (!checkArea()) {
        return false;
    }

    return true;
}

int daObjKshtr_c::getDemoAction() {
    static char* action_table[6] = {
        "WAIT",
        "ADJUSTMENT",
        "UNLOCK",
        "OPEN",
        "PLY_NODISP",
        "PLY_DISP",
    };

    return dComIfGp_evmng_getMyActIdx(mStaffId, action_table, 6, 0, 0);
}

BOOL daObjKshtr_c::demoProc2() {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    mStaffId = dComIfGp_evmng_getMyStaffId("JAIL_DOOR", NULL, 0);
    int demoAction = getDemoAction();

    if (dComIfGp_evmng_getIsAddvance(mStaffId) != 0) {
        switch (demoAction) {
            case DEMO_ACT_WAIT: {
                int* timer = dComIfGp_evmng_getMyIntegerP(mStaffId, "Timer");
                if (timer == NULL) {
                    mTimer = 1;
                } else {
                    mTimer = *timer;
                }
                break;
            }

            case DEMO_ACT_OPEN:
                openInit();
                break;

            case DEMO_ACT_UNLOCK:
                keyUnlockInit();
                break;

            case DEMO_ACT_ADJUSTMENT:
                field_0x605 = 5;
                break;

            case DEMO_ACT_PLY_DISP:
                player->offPlayerNoDraw();
                player->offPlayerShadowNoDraw();
                break;

            case DEMO_ACT_PLY_NODISP:
                player->onPlayerNoDraw();
                player->onPlayerShadowNoDraw();
                break;
        }
    }

    switch (demoAction) {
        case DEMO_ACT_WAIT:
            if (cLib_calcTimer(&mTimer) == 0) {
                dComIfGp_evmng_cutEnd(mStaffId);
            }
            break;

        case DEMO_ACT_OPEN:
            if (openProc()) {
                dComIfGp_evmng_cutEnd(mStaffId);
            }
            break;

        case DEMO_ACT_UNLOCK:
            if (keyUnlock()) {
                dComIfGp_evmng_cutEnd(mStaffId);
            }
            break;

        case DEMO_ACT_ADJUSTMENT:
            if (adjustmentProc()) {
                dComIfGp_evmng_cutEnd(mStaffId);
            }
            break;

        default:
            dComIfGp_evmng_cutEnd(mStaffId);
            break;
    }

    return FALSE;
}

BOOL daObjKshtr_c::adjustmentProc() {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp18, sp24;

    sp24 = player->current.pos;
    s16 angleY = player->shape_angle.y;
    cLib_addCalcAngleS2(&angleY, shape_angle.y + 0x7FFF, 5, 3000);

    if (field_0x605 != 0) {
        player->setPlayerPosAndAngle(&sp24, angleY, 0);
        field_0x605--;
    } else {
        player->setPlayerPosAndAngle(&sp24, shape_angle.y + 0x7FFF, 0);
        return TRUE;
    }

    return FALSE;
}

BOOL daObjKshtr_c::keyUnlockInit() {
    obj_keyhole_class* keyhole_p = (obj_keyhole_class*)fopAcM_SearchByID(mKeyHoleId);
    if (keyhole_p != NULL) {
        keyhole_p->setOpen();
    }

    dComIfGp_setItemKeyNumCount(-1);
    fopAcM_seStart(this, Z2SE_OBJ_DOOR_CHAIN_LOCK_OPEN, 0);

    return TRUE;
}

BOOL daObjKshtr_c::keyUnlock() {
    obj_keyhole_class* keyhole_p = (obj_keyhole_class*)fopAcM_SearchByID(mKeyHoleId);
    if (keyhole_p != NULL && keyhole_p->checkOpenEnd()) {
        return TRUE;
    }

    return FALSE;
}

BOOL daObjKshtr_c::openInit() {
    field_0x5fc = 0.0f;
    speedF = 0.0f;
    gravity = 0.0f;
    anmInit();
    offDzb();

    switch (mType) {
        case TYPE_0:
        case TYPE_1:
            break;

        case TYPE_L3BOSS:
            fopAcM_seStart(this, Z2SE_OBJ_LV3_SHTR_DW_STOP, 0);
            break;

        case TYPE_V_SHUTTER:
            eyePos = current.pos;
            fopAcM_seStart(this, Z2SE_OBJ_VSHUTR, 0);
            break;
    }

    return TRUE;
}

BOOL daObjKshtr_c::openProc() {
    switch (mType) {
        case TYPE_0:
        case TYPE_1:
            return openProc_type1();

        default:
            return openProc_type2();
        
        case TYPE_L3BOSS:
            return openProc_typeL3Boss();

        case TYPE_V_SHUTTER:
            if (mpBckAnm->play() != 0) {
                return TRUE;
            }

            return FALSE;
    }
}

BOOL daObjKshtr_c::openProc_type1() {
    BOOL rv = mpBckAnm->play();

    if (mpBckAnm->getFrame() == 1.0f) {
        dComIfGp_particle_set(0x8331, &current.pos, &shape_angle, NULL, 0xFF, NULL, fopAcM_GetRoomNo(this), NULL, NULL, NULL);
        dComIfGp_particle_set(0x8332, &current.pos, &shape_angle, NULL, 0xFF, NULL, fopAcM_GetRoomNo(this), NULL, NULL, NULL);
    } else if (mpBckAnm->getFrame() == 17.0f) {
        dComIfGp_getVibration().StartShock(4, 15, cXyz(0.0f, 1.0f, 0.0f));
    } else if (mpBckAnm->getFrame() == 18.0f) {
        mDoAud_seStart(Z2SE_OBJ_WOOD_DOOR_ROLLOPEN, &mSePos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }

    if (rv != 0) {
        dComIfGp_getVibration().StartShock(4, 31, cXyz(0.0f, 1.0f, 0.0f));
        mDoAud_seStart(Z2SE_OBJ_WOOD_DOOR_OPEN_STOP, &mSePos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }

    return rv;
}

BOOL daObjKshtr_c::openProc_type2() {
    static u16 const l_eff_id[5] = {
        0x8762,
        0x8763,
        0x8764,
        0x8765,
        0x8766,
    };

    BOOL rv = mpBckAnm->play();

    if (mpBckAnm->getFrame() == 1.0f) {
        for (u32 i = 0; i < 5; i++) {
            dComIfGp_particle_set(l_eff_id[i], &current.pos, &shape_angle, NULL, 0xFF, NULL, fopAcM_GetRoomNo(this), NULL, NULL, NULL);
        }
    } else if (mpBckAnm->getFrame() == 17.0f) {
        dComIfGp_getVibration().StartShock(4, 15, cXyz(0.0f, 1.0f, 0.0f));
    }

    if (mpBckAnm->getFrame() == 20.0f) {
        fopAcM_seStart(this, Z2SE_OBJ_LV3_SHTR_UP_STOP, 0);
    }

    return rv;
}

BOOL daObjKshtr_c::openProc_typeL3Boss() {
    cLib_chaseF(&speedF, 30.0f, 4.0f);
    return cLib_chaseF(&field_0x5fc, -450.0f, speedF) != 0 ? TRUE : FALSE;
}

void daObjKshtr_c::demoProc() {
    static daObjKshtr_c::DemoFunc l_demoProc[2] = {
        &daObjKshtr_c::demoJail1,
        &daObjKshtr_c::demoJail2,
    };

    static daObjKshtr_c::DemoFunc l_demoProc2[2] = {
        &daObjKshtr_c::demoJail11,
        &daObjKshtr_c::demoJail21,
    };

    if (mIsCheckKey) {
        (this->*l_demoProc[mType])();
    } else {
        (this->*l_demoProc2[mType])();
    }
}

BOOL daObjKshtr_c::demoJail1() {
    obj_keyhole_class* keyhole_p = (obj_keyhole_class*)fopAcM_SearchByID(mKeyHoleId);

    switch (mDemoMode) {
        case 0:
            if (keyhole_p != NULL) {
                keyhole_p->setOpen();
            }

            mDemoMode = 1;
            dComIfGp_setItemKeyNumCount(-1);
            fopAcM_seStart(this, Z2SE_OBJ_DOOR_CHAIN_LOCK_OPEN, 0);
            break;

        case 1:
            if (keyhole_p != NULL && keyhole_p->checkOpenEnd()) {
                mDemoMode = 2;
            }
            break;
        
        case 2:
            anmInit();
            mDoAud_seStart(Z2SE_OBJ_WOOD_DOOR_ROLLOPEN2, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            dComIfGp_getVibration().StartShock(4, 15, cXyz(0.0f, 1.0f, 0.0f));
            offDzb();
            mDemoMode = 3;
            break;

        case 3:
            if (mpBckAnm->play()) {
                dComIfGp_getVibration().StartShock(4, 31, cXyz(0.0f, 1.0f, 0.0f));
                mDoAud_seStart(Z2SE_OBJ_WOOD_DOOR_OPEN_STOP, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
                mDemoMode = 4;
            }
            break;
        
        case 4:
            break;
    }

    return FALSE;
}

BOOL daObjKshtr_c::demoJail2() {
    obj_keyhole_class* keyhole_p = (obj_keyhole_class*)fopAcM_SearchByID(mKeyHoleId);

    switch (mDemoMode) {
        case 0:
            if (keyhole_p != NULL) {
                keyhole_p->setOpen();
            }

            mDemoMode = 1;
            dComIfGp_setItemKeyNumCount(-1);
            break;

        case 1:
            if (keyhole_p != NULL && keyhole_p->checkOpenEnd()) {
                mDemoMode = 2;
            }
            break;
        
        case 2:
            speedF = 0.0f;
            mDemoMode = 3;
            anmInit();
            break;

        case 3:
            if (mpBckAnm->play()) {
                mDemoMode = 4;
            }
            break;

        case 4:
            break;
    }

    return FALSE;
}

BOOL daObjKshtr_c::demoJail11() {
    switch (mDemoMode) {
        case 0:
            mDemoMode = 1;
            anmInit();
            mDoAud_seStart(Z2SE_OBJ_WOOD_DOOR_ROLLOPEN2, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            offDzb();
            break;
        
        case 1:
            if (mpBckAnm->play()) {
                mDemoMode = 2;
                mDoAud_seStart(Z2SE_OBJ_WOOD_DOOR_OPEN_STOP, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            }
            break;

        case 2:
            break;
    }

    return FALSE;
}

BOOL daObjKshtr_c::demoJail21() {
    return TRUE;
}

void daObjKshtr_c::anmInit() {
    if (mpBckAnm != NULL) {
        BOOL isWolf = FALSE;
        if (daPy_py_c::checkNowWolf()) {
            isWolf = TRUE;
        }

        J3DAnmTransform* anm = (J3DAnmTransform*)dComIfG_getObjectRes(l_anmArcName[mType], l_anmName[isWolf + mType * 2]);

        JUT_ASSERT(1278, anm != NULL);

        #if DEBUG
        if (!mpBckAnm->init(anm, 1, 0, 1.0f, 0, -1, true)) {
            JUT_ASSERT(1281, FALSE);

            mpBckAnm->setPlaySpeed(1.0f);
        }
        #else
        mpBckAnm->init(anm, 1, 0, 1.0f, 0, -1, true);
        mpBckAnm->setPlaySpeed(1.0f);
        #endif
    }
}

void daObjKshtr_c::actionWaitEvent() {
    if (eventInfo.checkCommandDoor()) {
        setAction(1);
        mDemoMode = 0;
        fopAcM_onSwitch(this, mSwNo);

        if (mType == TYPE_L3BOSS) {
            dMeter2Info_onGameStatus(2);
        }
    } else if (checkOpen()) {
        if (daPy_py_c::checkNowWolf()) {
            mIsWolf = true;
        } else {
            mIsWolf = false;
        }

        eventInfo.setEventId(field_0x5f0[mIsWolf]);
        eventInfo.setMapToolId(mEvId);
        eventInfo.onCondition(dEvtCnd_CANDOOR_e);
    }
}

void daObjKshtr_c::actionEvent() {
    if (dComIfGp_evmng_endCheck(field_0x5f0[mIsWolf])) {
        setAction(2);
        dComIfGp_event_reset();
    } else {
        if (mType == TYPE_L3BOSS) {
            dMeter2Info_onGameStatus(2);
        }

        demoProc2();
    }
}

void daObjKshtr_c::actionDead() {
    /* empty function */
}

void daObjKshtr_c::actionWaitEvent2() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(1);
        mDemoMode = 0;
    } else if (fopAcM_isSwitch(this, mSwNo)) {
        if (mType == TYPE_V_SHUTTER) {
            openInit();
            setAction(3);
        } else {
            fopAcM_orderOtherEventId(this, field_0x5f0[0], mEvId, 0xFFFF, 0, 1);
            eventInfo.onCondition(dEvtCnd_CANDEMO_e);
        }
    }
}

void daObjKshtr_c::actionOpen() {
    if (openProc()) {
        setAction(2);
    }
}

void daObjKshtr_c::actionEvent2() {
    if (dComIfGp_evmng_endCheck(field_0x5f0[0])) {
        setAction(2);
        dComIfGp_event_reset();
    } else {
        demoProc();
    }
}

void daObjKshtr_c::actionDead2() {
    /* empty function */
}

int daObjKshtr_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();

    if (mpBckAnm != NULL) {
        mpBckAnm->entry(mpModel->getModelData());
    }

    mDoExt_modelUpdateDL(mpModel);
    mDoExt_bckAnmRemove(mpModel->getModelData());
    dComIfGd_setList();

    return 1;
}

int daObjKshtr_c::Delete() {
    dComIfG_resDelete(&mPhase1, l_arcName[mType]);

    if (l_anmArcName[mType] != NULL) {
        dComIfG_resDelete(&mPhase2, l_anmArcName[mType]);
    }

    return 1;
}

static int daObjKshtr_create1st(daObjKshtr_c* i_this) {
    fopAcM_ct(i_this, daObjKshtr_c);
    return i_this->create1st();
}

static int daObjKshtr_MoveBGDelete(daObjKshtr_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjKshtr_MoveBGExecute(daObjKshtr_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjKshtr_MoveBGDraw(daObjKshtr_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjKshtr_METHODS = {
    (process_method_func)daObjKshtr_create1st,
    (process_method_func)daObjKshtr_MoveBGDelete,
    (process_method_func)daObjKshtr_MoveBGExecute,
    0,
    (process_method_func)daObjKshtr_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_Kshutter = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Kshutter,      // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjKshtr_c),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  443,                    // mPriority
  &daObjKshtr_METHODS,    // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
