/**
 * d_a_obj_kgate.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_kgate.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_obj_eff.h"

enum {
    ACT_WAIT_EVENT,
    ACT_EVENT,
    ACT_DEAD,
};

enum {
    DEMO_ACT_WAIT,
    DEMO_ACT_ADJUSTMENT,
    DEMO_ACT_UNLOCK,
    DEMO_ACT_OPEN,
};

enum {
    AREA_CHECK_PLAYER = 1,
    AREA_CHECK_COACH,
    AREA_CHECK_HORSE,
};

static void* search_coach(void* i_actor, void* i_data) {
    if (i_actor != NULL && fopAcM_IsActor(i_actor) && fopAcM_GetProfName(i_actor) == PROC_NPC_COACH)
    {
        return i_actor;
    }

    return NULL;
}

static int const l_gateBmdIdx[] = {5, 4, 5};

static int const l_gateKeyIdx[] = {7, -1, 7};

static int const l_gateHookIdx[] = {6, -1, 6};

static int const l_gateDzbIdx[] = {10, 7, 10};

static u32 const l_gate_heap[] = {0x1940, 0x1940, 0x1940};

static u32 const l_key_heap[] = {0x1000, 0x1000, 0x1000};

static cull_box const l_cull_box = {
    {-300.0f, 0.0f, -350.0f},
    {300.0f, 450.0f, 350.0f},
};

const static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x1e}, {0x20, 0x11}, 0x78}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},             // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6},             // mGObjTg
        {0x0},                                          // mGObjCo
    },                                                  // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            40.0f,               // mRadius
            350.0f               // mHeight
        }                        // mCyl
    }
};

void daObjKGate_c::initBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    MTXCopy(mDoMtx_stack_c::get(), field_0xb18);
    mpGateModel->setBaseScale(scale);

    if (mNameArg == 0 || mNameArg == 1) {
        mpGateModel2->setBaseScale(scale);
    } else if (mNameArg == 2) {
        cXyz scale(1.0f, 1.0f, -1.0f);
        mpGateModel2->setBaseScale(scale);
    }

    if (getSwNo() != 0xFF) {
        if (mpKeyModel != NULL) {
            mpKeyModel->setBaseScale(scale);
        }

        if (mpHookModel != NULL) {
            mpHookModel->setBaseScale(scale);
        }
    }

    cXyz eff_pos_offset(0.0f, 105.6f, 25.3f);
    mDoMtx_stack_c::YrotS(current.angle.y);
    mDoMtx_stack_c::multVec(&eff_pos_offset, &eff_pos_offset);

    mEffPos = current.pos;
    mEffPos += eff_pos_offset;
    mKeyRot.set(0, current.angle.y, 0);
    setBaseMtx();
}

void daObjKGate_c::setBaseMtx() {
    cXyz sp30;
    cXyz sp3C;

    if (mNameArg == 0 || mNameArg == 1) {
        sp30.set(-210.0f, 0.0f, 0.0f);
        sp3C.set(210.0f, 0.0f, 0.0f);
    } else {
        sp30.set(-350.0f, 0.0f, 0.0f);
        sp3C.set(350.0f, 0.0f, 0.0f);
    }

    mDoMtx_stack_c::YrotS(current.angle.y);
    mDoMtx_stack_c::multVec(&sp30, &sp30);
    mDoMtx_stack_c::multVec(&sp3C, &sp3C);

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::transM(sp30);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::YrotM(mGateLAngle);

    mpGateModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
    field_0xbf0 = current.pos + sp30;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::transM(sp3C);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::YrotM(0x7FFF);
    mDoMtx_stack_c::YrotM(mGateRAngle);

    mpGateModel2->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), field_0xae8);
    field_0xbfc = sp3C;

    if (mNameArg != 1 && getSwNo() != 0xFF) {
        cXyz sp48(208.7f, 108.6f, 25.3f);
        mDoMtx_stack_c::YrotS(current.angle.y + mGateLAngle);
        mDoMtx_stack_c::multVec(&sp48, &sp48);

        cXyz sp54;
        cXyz sp60;
        sp60 = field_0xbc4;
        mDoMtx_stack_c::multVec(&sp60, &sp60);

        sp54 = field_0xbb8;
        mDoMtx_stack_c::multVec(&sp54, &sp54);

        s16 var_r29 = field_0xbb4 * cM_scos(mCounter * 3000);
        cLib_addCalc(&field_0xbb4, 0.0f, 0.05f, 50.0f, 5.0f);

        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::transM(sp30);
        mDoMtx_stack_c::transM(sp60);
        mDoMtx_stack_c::transM(sp48);
        mDoMtx_stack_c::YrotM(current.angle.y + mGateLAngle);
        mDoMtx_stack_c::XrotM(-0xE38);
        mDoMtx_stack_c::XrotM(-field_0xbae);
        mDoMtx_stack_c::transM(14.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::ZrotM(field_0xbb2);
        mDoMtx_stack_c::ZrotM(var_r29);
        mDoMtx_stack_c::transM(-14.0f, 0.0f, 0.0f);
        mpHookModel->setBaseTRMtx(mDoMtx_stack_c::get());

        if (mpKeyModel != NULL) {
            if (field_0xbb2 == 0) {
                mDoMtx_stack_c::transS(current.pos);
                mDoMtx_stack_c::transM(sp30);
                mDoMtx_stack_c::transM(sp48);
                mDoMtx_stack_c::YrotM(current.angle.y + mGateLAngle);
                mDoMtx_stack_c::XrotM(-0xE38);
                mDoMtx_stack_c::XrotM(-field_0xbae);
                mpKeyModel->setBaseTRMtx(mDoMtx_stack_c::get());
            } else {
                field_0xbe8 += -2.5f;
                if (field_0xbe8 < -20.0f) {
                    field_0xbe8 = -20.0f;
                }
                mEffPos.y += field_0xbe8;

                cXyz sp6C(current.pos.x, current.pos.y + 50.0f, current.pos.z);
                if (fopAcM_gc_c::gndCheck(&sp6C) && fopAcM_gc_c::getGroundY() > mEffPos.y) {
                    mEffPos.y = fopAcM_gc_c::getGroundY() + 2.0f;

                    if (!mCreatedSmokeEff) {
                        daObjEff::Act_c::make_land_smoke(&mEffPos, 0.5f);
                        mCreatedSmokeEff = true;
                    }
                }

                cLib_addCalcAngleS(&mKeyRot.x, -0x4000, 5, 0xC00, 0x400);
                mDoMtx_stack_c::transS(mEffPos);
                mDoMtx_stack_c::ZXYrotM(mKeyRot);
                mpKeyModel->setBaseTRMtx(mDoMtx_stack_c::get());
            }
        }
    }
}

static char* l_arcName[] = {
    "D_Kgate00",
    "D_Kgate01",
    "M_RGate00",
};

int daObjKGate_c::Create() {
    static char* l_evName[] = {"KOKI_GATE_OPEN00", NULL, "RIDER_GATE_OPEN00"};

    u8 sw_no = getSwNo();

    if (mNameArg == 0 || mNameArg == 2) {
        if (sw_no != 0xFF && !fopAcM_isSwitch(this, sw_no)) {
            eventInfo.setArchiveName(l_arcName[mNameArg]);

            mEventID = dComIfGp_getEventManager().getEventIdx(this, l_evName[mNameArg], 0xFF);
            mMapToolID = getEventID();
            setAction(ACT_WAIT_EVENT);
        } else {
            mEventID = -1;
            mMapToolID = 0xFF;

            if (mNameArg != 2) {
                mGateLAngle = cM_deg2s(80);
                mGateRAngle = -cM_deg2s(80);
                field_0xbed = 1;
            }

            setAction(ACT_DEAD);
        }
    } else {
        mEventID = -1;
        mMapToolID = 0xFF;
        setAction(ACT_DEAD);
    }

    attention_info.position.y += 150.0f;
    eyePos.y += 150.0f;
    attention_info.flags = fopAc_AttnFlag_DOOR_e;

    initBaseMtx();
    fopAcM_SetMtx(this, field_0xb18);

    if (mNameArg == 1) {
        field_0x5b8.Init(0xFF, 0xFF, this);

        for (int i = 0; i < 4; i++) {
            field_0x5f4[i].Set(l_cyl_src);
            field_0x5f4[i].SetStts(&field_0x5b8);
        }
    }

    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z, l_cull_box.max.x,
                          l_cull_box.max.y, l_cull_box.max.z);

    if (dComIfG_Bgsp().Regist(mpBgW, this)) {
        OS_REPORT_ERROR("キコルの門：ＢＧ登録失敗しました\n");
        return 0;
    }

    return 1;
}

int daObjKGate_c::CreateHeap() {
    J3DModelData* modelData =
        (J3DModelData*)dComIfG_getObjectRes(l_arcName[mNameArg], l_gateBmdIdx[mNameArg]);
    JUT_ASSERT(500, modelData != NULL);

    mpGateModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    mpGateModel2 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpGateModel == NULL || mpGateModel2 == NULL) {
        return 0;
    }

    if ((mNameArg == 0 || mNameArg == 2) && getSwNo() != 0xFF) {
        modelData =
            (J3DModelData*)dComIfG_getObjectRes(l_arcName[mNameArg], l_gateKeyIdx[mNameArg]);
        JUT_ASSERT(524, modelData != NULL);

        J3DModelData* modelData2 =
            (J3DModelData*)dComIfG_getObjectRes(l_arcName[mNameArg], l_gateBmdIdx[mNameArg]);
        JUT_ASSERT(530, modelData2 != NULL);

        mDoExt_setupShareTexture(modelData, modelData2);
        mpKeyModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (mpKeyModel == NULL) {
            return 0;
        }

        modelData =
            (J3DModelData*)dComIfG_getObjectRes(l_arcName[mNameArg], l_gateHookIdx[mNameArg]);
        JUT_ASSERT(546, modelData != NULL);

        mDoExt_setupShareTexture(modelData, modelData2);
        mpHookModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (mpHookModel == NULL) {
            return 0;
        }
    }

    mpBgW = new dBgW();
    if (mpBgW == NULL ||
        mpBgW->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName[mNameArg], l_gateDzbIdx[mNameArg]),
                   cBgW::MOVE_BG_e, &field_0xae8))
    {
        mpBgW = NULL;
        return 0;
    }

    return 1;
}

int daObjKGate_c::create1st() {
    mNameArg = getNameArg();
    if (mNameArg == 1) {
        if (fopAcM_isSwitch(this, getSwNo())) {
            return cPhs_ERROR_e;
        }
    }

    int phase_state = dComIfG_resLoad(&mPhase, l_arcName[mNameArg]);
    if (phase_state == cPhs_COMPLEATE_e) {
        u32 heap_size = l_gate_heap[mNameArg];

        if (getSwNo() != 0xFF) {
            heap_size += l_key_heap[mNameArg];
        }

        phase_state =
            MoveBGCreate(l_arcName[mNameArg], l_gateDzbIdx[mNameArg], NULL, heap_size, NULL);
        if (phase_state == cPhs_ERROR_e) {
#if DEBUG
            OSReport("KGATE PARAM:%x\n", fopAcM_GetParam(this));
#endif
            return phase_state;
        }
    }

    return phase_state;
}

int daObjKGate_c::checkAreaL(cXyz const* param_0, cXyz const* param_1) {
    daPy_py_c* player_p = daPy_getPlayerActorClass();
    Mtx m;
    cXyz pos;
    cXyz offset;
    cXyz bound_a;
    cXyz bound_b;

    if (mNameArg == 2) {
        bound_a.set(0.0f, 0.0f, -100.0f);
        bound_b.set(400.0f, 0.0f, 300.0f);
        offset.set(0.0f, 0.0f, 100.0f);

        fopAc_ac_c* coach_p = fopAcM_Search(search_coach, this);
        if (coach_p != NULL) {
            mDoMtx_stack_c::transS(coach_p->current.pos);
            mDoMtx_stack_c::YrotM(coach_p->shape_angle.y);
            mDoMtx_stack_c::multVec(&offset, &offset);
            pos = offset;

            mDoMtx_inverse(mpGateModel->getBaseTRMtx(), m);
            mDoMtx_stack_c::copy(m);
            mDoMtx_stack_c::multVec(&pos, &pos);

            if (bound_a.x <= pos.x && pos.x <= bound_b.x && bound_a.z <= pos.z &&
                pos.z <= bound_b.z)
            {
                return AREA_CHECK_COACH;
            }
        }

        offset.set(0.0f, 0.0f, 250.0f);

        daHorse_c* horse_p = dComIfGp_getHorseActor();
        if (horse_p != NULL) {
            mDoMtx_stack_c::transS(horse_p->current.pos);
            mDoMtx_stack_c::YrotM(horse_p->shape_angle.y);
            mDoMtx_stack_c::multVec(&offset, &offset);
            pos = offset;

            mDoMtx_inverse(mpGateModel->getBaseTRMtx(), m);
            mDoMtx_stack_c::copy(m);
            mDoMtx_stack_c::multVec(&pos, &pos);

            if (bound_a.x <= pos.x && pos.x <= bound_b.x && bound_a.z <= pos.z &&
                pos.z <= bound_b.z)
            {
                return AREA_CHECK_HORSE;
            }
        }
    }

    if (mNameArg == 0 || mNameArg == 1) {
        bound_a.set(0.0f, 0.0f, -80.0f);
        bound_b.set(200.0f, 0.0f, 80.0f);
    } else {
        bound_a.set(0.0f, 0.0f, -100.0f);
        bound_b.set(350.0f, 0.0f, 100.0f);
    }

    pos = player_p->current.pos;

    if (daPy_py_c::checkNowWolf()) {
        offset.set(0.0f, 0.0f, 90.0f);
    } else {
        offset.set(0.0f, 0.0f, 0.0f);
    }

    mDoMtx_stack_c::YrotS(player_p->shape_angle.y);
    mDoMtx_stack_c::multVec(&offset, &offset);
    pos += offset;

    mDoMtx_inverse(mpGateModel->getBaseTRMtx(), m);
    mDoMtx_stack_c::copy(m);
    mDoMtx_stack_c::multVec(&pos, &pos);

    if (bound_a.x <= pos.x && pos.x <= bound_b.x && bound_a.z <= pos.z && pos.z <= bound_b.z) {
        return AREA_CHECK_PLAYER;
    }

    return 0;
}

int daObjKGate_c::checkAreaR(cXyz const* param_0, cXyz const* param_1) {
    daPy_py_c* player_p = daPy_getPlayerActorClass();
    Mtx m;
    Mtx m2;
    cXyz pos;
    cXyz offset;
    cXyz bound_a;
    cXyz bound_b;

    if (mNameArg == 2) {
        bound_a.set(0.0f, 0.0f, -300.0f);
        bound_b.set(400.0f, 0.0f, 100.0f);
        offset.set(0.0f, 0.0f, 100.0f);

        fopAc_ac_c* coach_p = fopAcM_Search(search_coach, this);
        if (coach_p != NULL) {
            mDoMtx_stack_c::transS(coach_p->current.pos);
            mDoMtx_stack_c::YrotM(coach_p->shape_angle.y);
            mDoMtx_stack_c::multVec(&offset, &offset);
            pos = offset;

            mDoMtx_inverse(mpGateModel2->getBaseTRMtx(), m);
            mDoMtx_stack_c::copy(m);
            mDoMtx_stack_c::multVec(&pos, &pos);

            if (bound_a.x <= pos.x && pos.x <= bound_b.x && bound_a.z <= pos.z &&
                pos.z <= bound_b.z)
            {
                return AREA_CHECK_COACH;
            }
        }

        offset.set(0.0f, 0.0f, 250.0f);

        daHorse_c* horse_p = dComIfGp_getHorseActor();
        if (horse_p != NULL) {
            mDoMtx_stack_c::transS(horse_p->current.pos);
            mDoMtx_stack_c::YrotM(horse_p->shape_angle.y);
            mDoMtx_stack_c::multVec(&offset, &offset);
            pos = offset;

            mDoMtx_inverse(mpGateModel2->getBaseTRMtx(), m);
            mDoMtx_stack_c::copy(m);
            mDoMtx_stack_c::multVec(&pos, &pos);

            if (bound_a.x <= pos.x && pos.x <= bound_b.x && bound_a.z <= pos.z &&
                pos.z <= bound_b.z)
            {
                return AREA_CHECK_HORSE;
            }
        }
    }

    if (mNameArg == 0 || mNameArg == 1) {
        bound_a.set(0.0f, 0.0f, -80.0f);
        bound_b.set(200.0f, 0.0f, 80.0f);
    } else {
        bound_a.set(0.0f, 0.0f, -100.0f);
        bound_b.set(350.0f, 0.0f, 100.0f);
    }

    pos = player_p->current.pos;

    cXyz player_offset;
    if (daPy_py_c::checkNowWolf()) {
        player_offset.set(0.0f, 0.0f, 90.0f);
    } else {
        player_offset.set(0.0f, 0.0f, 0.0f);
    }

    mDoMtx_stack_c::YrotS(player_p->shape_angle.y);
    mDoMtx_stack_c::multVec(&player_offset, &player_offset);
    pos += player_offset;

    mDoMtx_inverse(mpGateModel2->getBaseTRMtx(), m2);
    mDoMtx_stack_c::copy(m2);
    mDoMtx_stack_c::multVec(&pos, &pos);

    if (bound_a.x <= pos.x && pos.x <= bound_b.x && bound_a.z <= pos.z && pos.z <= bound_b.z) {
        return AREA_CHECK_PLAYER;
    }

    return 0;
}

BOOL daObjKGate_c::checkOpen() {
    if (dComIfGs_getKeyNum() == 0) {
        return false;
    }

    daPy_py_c* player_p = daPy_getPlayerActorClass();

    if (abs((s16)(current.angle.y - player_p->current.angle.y)) < 0x5000) {
        return false;
    }

    Mtx m;
    mDoMtx_inverse(field_0xb18, m);

    cXyz pos(player_p->current.pos);
    mDoMtx_stack_c::copy(m);
    mDoMtx_stack_c::multVec(&pos, &pos);

    cXyz bound_a(-100.0f, 0.0f, 0.0f);
    cXyz bound_b(100.0f, 0.0f, 100.0f);

    if (bound_a.x <= pos.x && pos.x <= bound_b.x && bound_a.z <= pos.z && pos.z <= bound_b.z) {
        return true;
    }

    return false;
}

BOOL daObjKGate_c::checkDirL(fopAc_ac_c* i_actor) {
    Mtx m;
    mDoMtx_inverse(mpGateModel->getBaseTRMtx(), m);

    cXyz pos(i_actor->current.pos);
    mDoMtx_stack_c::copy(m);
    mDoMtx_stack_c::multVec(&pos, &pos);

    if (pos.z > 0.0f) {
        return false;
    } else {
        return true;
    }
}

BOOL daObjKGate_c::checkDirR(fopAc_ac_c* i_actor) {
    Mtx m;
    mDoMtx_inverse(mpGateModel2->getBaseTRMtx(), m);

    cXyz pos(i_actor->current.pos);
    mDoMtx_stack_c::copy(m);
    mDoMtx_stack_c::multVec(&pos, &pos);

    if (pos.z < 0.0f) {
        return false;
    } else {
        return true;
    }
}

void daObjKGate_c::setAtkSE() {
    fopAcM_seStart(this, Z2SE_OBJ_FLN_GATE_ATK, 0);
}

void daObjKGate_c::setCrkSE() {
    fopAcM_seStart(this, Z2SE_OBJ_FLN_GATE_CRK, 0);
}

void daObjKGate_c::action() {
    typedef void (daObjKGate_c::*func)();
    static func l_func[] = {&daObjKGate_c::action_typeA, &daObjKGate_c::action_typeB,
                            &daObjKGate_c::action_typeA};

    if (field_0xbed == 0) {
        (this->*l_func[mNameArg])();
    }
}

void daObjKGate_c::action_typeA() {
    u8 sw_no = getSwNo();
    daPy_py_c* player_p = daPy_getPlayerActorClass();

    if (fopAcM_isSwitch(this, sw_no) || sw_no == 0xFF) {
        cXyz unused1;
        cXyz unused2;

        int chk_l = checkAreaL(&unused1, &unused2);
        int chk_r = checkAreaR(&unused1, &unused2);
        s16 prev_l_move = mGateLMove;
        s16 prev_r_move = mGateRMove;

        if (player_p->getSpeedF() != 0.0f) {
            if (chk_l == AREA_CHECK_PLAYER) {
                switch (checkDirL(player_p)) {
                case FALSE:
                    mGateLMove = 500;
                    break;
                case TRUE:
                    mGateLMove = -500;
                    break;
                }

                field_0xbb4 = 2000.0f;
            }

            if (chk_r == AREA_CHECK_PLAYER) {
                switch (checkDirR(player_p)) {
                case FALSE:
                    mGateRMove = -500;
                    break;
                case TRUE:
                    mGateRMove = 500;
                    break;
                }
            }
        }

        daHorse_c* horse_p = dComIfGp_getHorseActor();
        if (horse_p != NULL && horse_p->speedF != 0.0f) {
            if (chk_l == AREA_CHECK_HORSE) {
                switch (checkDirL(horse_p)) {
                case FALSE:
                    mGateLMove = 800;
                    break;
                case TRUE:
                    mGateLMove = -800;
                    break;
                }

                field_0xbb4 = 2500.0f;
            }

            if (chk_r == AREA_CHECK_HORSE) {
                switch (checkDirR(horse_p)) {
                case FALSE:
                    mGateRMove = -800;
                    break;
                case TRUE:
                    mGateRMove = 800;
                    break;
                }
            }
        }

        fopAc_ac_c* coach_p = fopAcM_Search(search_coach, this);
        if (coach_p != NULL && coach_p->speedF != 0.0f) {
            if (chk_l == AREA_CHECK_COACH) {
                switch (checkDirL(coach_p)) {
                case FALSE:
                    mGateLMove = 900;
                    break;
                case TRUE:
                    mGateLMove = -900;
                    break;
                }

                field_0xbb4 = 2500.0f;
            }

            if (chk_r == AREA_CHECK_COACH) {
                switch (checkDirR(coach_p)) {
                case FALSE:
                    mGateRMove = -900;
                    break;
                case TRUE:
                    mGateRMove = 900;
                    break;
                }
            }
        }

        if (abs(abs(prev_l_move) - abs(mGateLMove)) > 400 ||
            abs(abs(prev_r_move) - abs(mGateRMove)) > 400)
        {
            setCrkSE();
        }

        if (mGateLMove > 1500) {
            mGateLMove = 1500;
        }

        if (mGateLMove < -1500) {
            mGateLMove = -1500;
        }

        if (mGateRMove > 1500) {
            mGateRMove = 1500;
        }

        if (mGateRMove < -1500) {
            mGateRMove = -1500;
        }

        mGateLAngle += mGateLMove;
        if (mGateLAngle > 0x4000) {
            mGateLAngle = 0x4000;
            mGateLMove *= -0.7f;
        }

        if (mGateLAngle < -0x4000) {
            mGateLAngle = -0x4000;
            mGateLMove *= -0.7f;
        }

        mGateRAngle += mGateRMove;
        if (mGateRAngle > 0x4000) {
            mGateRAngle = 0x4000;
            mGateRMove *= -0.7f;
        }

        if (mGateRAngle < -0x4000) {
            mGateRAngle = -0x4000;
            mGateRMove *= -0.7f;
        }

        cLib_addCalcAngleS(&mGateLMove, 0, 10, 100, 10);
        cLib_addCalcAngleS(&mGateRMove, 0, 10, 100, 10);
        cLib_addCalcAngleS(&field_0xbb2, 0x3000, 10, 0x400, 0x100);
        cLib_chaseF(&field_0xbb8.x, -39.0f, 1.0f);
        cLib_chaseF(&field_0xbc4.x, -25.0f, 1.0f);
        cLib_addCalcAngleS(&field_0xbd0, 0x7FFF, 10, 0x400, 0x100);
    } else {
        cXyz unused1;
        cXyz unused2;

        int chk_l = checkAreaL(&unused1, &unused2);
        int chk_r = checkAreaR(&unused1, &unused2);
        if (chk_l != 0 || chk_r != 0) {
            daPy_py_c* player_p = daPy_getPlayerActorClass();

            if (daPy_py_c::checkNowWolf()) {
                if (player_p->checkWolfAttackReverse() || player_p->checkFrontRollCrash()) {
                    field_0xb98 = 1200.0f;
                    field_0xb94 = 1200.0f;
                    field_0xbb0 = 0x800;
                    setAtkSE();
                }
            } else {
                if (player_p->checkFrontRollCrash()) {
                    field_0xb98 = 600.0f;
                    field_0xb94 = 600.0f;
                    field_0xbb0 = 0x800;
                    setAtkSE();
                }
            }
        }

        mGateLAngle = field_0xb94 * cM_ssin(mCounter * 4000);
        mGateRAngle = field_0xb98 * -cM_ssin(mCounter * 4000);
        cLib_addCalc0(&field_0xb94, 0.1f, 20.0f);
        cLib_addCalc0(&field_0xb98, 0.1f, 20.0f);

        field_0xbb0 -= 0x160;
        field_0xbae += field_0xbb0;
        if (field_0xbae >= 0) {
            return;
        }

        field_0xbae = 0;
        field_0xbb0 = -field_0xbb0 * 0.7f;
    }
}

void daObjKGate_c::action_typeB() {
    static const Vec l_tg_offset[] = {
        {-150.0f, 0.0f, 0.0f}, {-50.0f, 0.0f, 0.0f}, {50.0f, 0.0f, 0.0f}, {150.0f, 0.0f, 0.0f}};

    u8 sw_no = getSwNo();
    daPy_py_c* player_p = daPy_getPlayerActorClass();

    if (!fopAcM_isSwitch(this, sw_no)) {
        cXyz unused1;
        cXyz unused2;
        int chk_l = checkAreaL(&unused1, &unused2);
        int chk_r = checkAreaR(&unused1, &unused2);

        if (chk_l != 0 || chk_r != 0) {
            daPy_py_c* player_p = daPy_getPlayerActorClass();

            if (daPy_py_c::checkNowWolf()) {
                if (player_p->checkWolfAttackReverse() || player_p->checkFrontRollCrash()) {
                    field_0xb98 = 1200.0f;
                    field_0xb94 = 1200.0f;
                }
            } else {
                if (player_p->checkFrontRollCrash()) {
                    field_0xb98 = 600.0f;
                    field_0xb94 = 600.0f;
                }
            }
        }

        mGateLAngle = field_0xb94 * cM_ssin(mCounter * 4000);
        mGateRAngle = field_0xb98 * -cM_ssin(mCounter * 4000);
        cLib_addCalc0(&field_0xb94, 0.1f, 20.0f);
        cLib_addCalc0(&field_0xb98, 0.1f, 20.0f);

        for (int i = 0; i < 4; i++) {
            if (field_0x5f4[i].ChkTgHit()) {
                cCcD_Obj* hit_obj = field_0x5f4[i].GetTgHitObj();

                if (hit_obj != NULL && hit_obj->ChkAtType(AT_TYPE_BOMB)) {
                    fopAcM_onSwitch(this, sw_no);
                    fopAcM_delete(this);
                    break;
                }
            }
        }

        cXyz cyl_center;
        for (int i = 0; i < 4; i++) {
            cyl_center = l_tg_offset[i];
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(current.angle.y);
            mDoMtx_stack_c::multVec(&cyl_center, &cyl_center);

            field_0x5f4[i].SetC(cyl_center);
            dComIfG_Ccsp()->Set(&field_0x5f4[i]);
        }
    }
}

void daObjKGate_c::actionWaitEvent() {
    if (eventInfo.checkCommandDoor()) {
        setAction(ACT_EVENT);
        dComIfGp_setItemKeyNumCount(-1);
        fopAcM_onSwitch(this, getSwNo());
        fopAcM_seStart(this, Z2SE_OBJ_FLN_GATE_L_OP, 0);
    } else if (checkOpen()) {
        eventInfo.setEventId(mEventID);
        eventInfo.setMapToolId(mMapToolID);
        eventInfo.onCondition(dEvtCnd_CANDOOR_e);
    }
}

void daObjKGate_c::actionEvent() {
    if (dComIfGp_evmng_endCheck(mEventID)) {
        setAction(ACT_DEAD);
        dComIfGp_event_reset();
    } else {
        demoProc();
    }
}

void daObjKGate_c::actionDead() {}

void daObjKGate_c::event_proc_call() {
    typedef void (daObjKGate_c::*func)();
    static func l_func[] = {&daObjKGate_c::actionWaitEvent, &daObjKGate_c::actionEvent,
                            &daObjKGate_c::actionDead};

    (this->*l_func[mAction])();
}

void daObjKGate_c::demoProc() {
    mStaffID = dComIfGp_evmng_getMyStaffId("K_GATE", NULL, 0);
    int demo_action = getDemoAction();

    if (dComIfGp_evmng_getIsAddvance(mStaffID)) {
        switch (demo_action) {
        case DEMO_ACT_WAIT: {
            int* timer_p = dComIfGp_evmng_getMyIntegerP(mStaffID, "Timer");
            if (timer_p == NULL) {
                mTimer = 1;
            } else {
                mTimer = *timer_p;
            }
            break;
        }
        case DEMO_ACT_OPEN:
            mGateLMove = 1000;
            mGateRMove = -1000;
            break;
        }
    }

    switch (demo_action) {
    case DEMO_ACT_WAIT:
        if (cLib_calcTimer(&mTimer) == 0) {
            dComIfGp_evmng_cutEnd(mStaffID);
        }
        break;
    case DEMO_ACT_OPEN:
        dComIfGp_evmng_cutEnd(mStaffID);
        break;
    case DEMO_ACT_UNLOCK:
        dComIfGp_evmng_cutEnd(mStaffID);
        break;
    case DEMO_ACT_ADJUSTMENT:
        dComIfGp_evmng_cutEnd(mStaffID);
        break;
    default:
        dComIfGp_evmng_cutEnd(mStaffID);
        break;
    }
}

int daObjKGate_c::getDemoAction() {
    static char* action_table[] = {"WAIT", "ADJUSTMENT", "UNLOCK", "OPEN"};

    return dComIfGp_evmng_getMyActIdx(mStaffID, action_table, ARRAY_SIZEU(action_table), 0, 0);
}

int daObjKGate_c::Execute(Mtx** param_0) {
    event_proc_call();
    action();
    mCounter++;

    *param_0 = &mBgMtx;
    setBaseMtx();
    mpBgW->Move();
    return 1;
}

int daObjKGate_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpGateModel, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpGateModel2, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpGateModel);
    mDoExt_modelUpdateDL(mpGateModel2);
    dComIfGd_setList();

    if (getSwNo() != 0xFF) {
        g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);

        if (mpHookModel != NULL) {
            g_env_light.setLightTevColorType_MAJI(mpHookModel, &tevStr);
            mDoExt_modelUpdateDL(mpHookModel);
        }

        if (mpKeyModel != NULL) {
            g_env_light.setLightTevColorType_MAJI(mpKeyModel, &tevStr);
            mDoExt_modelUpdateDL(mpKeyModel);
        }
    }

    return 1;
}

int daObjKGate_c::Delete() {
    if (mpBgW != NULL && mpBgW->ChkUsed()) {
        if (dComIfG_Bgsp().Release(mpBgW)) {
            OS_REPORT("Release Error\n");
        }
    }

    dComIfG_resDelete(&mPhase, l_arcName[mNameArg]);
    return 1;
}

static int daObjKGate_create1st(daObjKGate_c* i_this) {
    fopAcM_ct(i_this, daObjKGate_c);
    return i_this->create1st();
}

static int daObjKGate_MoveBGDelete(daObjKGate_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjKGate_MoveBGExecute(daObjKGate_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjKGate_MoveBGDraw(daObjKGate_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjKGate_METHODS = {
    (process_method_func)daObjKGate_create1st,     (process_method_func)daObjKGate_MoveBGDelete,
    (process_method_func)daObjKGate_MoveBGExecute, (process_method_func)NULL,
    (process_method_func)daObjKGate_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_KkrGate = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_KkrGate,
    &g_fpcLf_Method.base,
    sizeof(daObjKGate_c),
    0,
    0,
    &g_fopAc_Method.base,
    599,
    &daObjKGate_METHODS,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
