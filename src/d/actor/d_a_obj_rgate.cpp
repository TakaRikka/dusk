/**
 * d_a_obj_rgate.cpp
 * Bulblin Gate
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_rgate.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_obj_eff.h"

#define GATE_L_JNT 1
#define GATE_R_JNT 2

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

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model_p = j3dSys.getModel();
        daObjRgate_c* actor_p = (daObjRgate_c*)model_p->getUserArea();

        if (jnt_no == GATE_L_JNT) {
            MTXCopy(model_p->getAnmMtx(jnt_no), mDoMtx_stack_c::get());
            mDoMtx_stack_c::YrotM(actor_p->mGateLAngle);
            model_p->setAnmMtx(jnt_no, mDoMtx_stack_c::get());

            MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
        } else if (jnt_no == GATE_R_JNT) {
            MTXCopy(model_p->getAnmMtx(jnt_no), mDoMtx_stack_c::get());
            mDoMtx_stack_c::YrotM(actor_p->mGateRAngle);
            model_p->setAnmMtx(jnt_no, mDoMtx_stack_c::get());

            MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static const cull_box l_cull_box = {
    {-400.0f, 0.0f, -400.0f},
    {400.0f, 450.0f, 400.0f},
};

static const dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x1e}, {0x20, 0x11}, 0x78}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},             // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6},             // mGObjTg
        {0x0},                                          // mGObjCo
    },                                                  // mObjInf
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        40.0f,               // mRadius
        350.0f               // mHeight
    }                        // mCyl
};

void daObjRgate_c::initBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    MTXCopy(mDoMtx_stack_c::get(), field_0xb14);
    mpGateModel->setBaseScale(scale);

    if (getSwNo() != 0xFF) {
        if (mpKeyModel != NULL) {
            mpKeyModel->setBaseScale(scale);
        }

        if (mpHookModel != NULL) {
            mpHookModel->setBaseScale(scale);
        }
    }

    cXyz eff_pos_offset(0.0f, 128.0f, 38.0f);
    mDoMtx_stack_c::YrotS(current.angle.y);
    mDoMtx_stack_c::multVec(&eff_pos_offset, &eff_pos_offset);

    mEffPos = current.pos;
    mEffPos += eff_pos_offset;
    mKeyRot.set(0, current.angle.y, 0);
    setBaseMtx();
}

void daObjRgate_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpGateModel->setBaseTRMtx(mDoMtx_stack_c::get());

    cXyz sp30(-350.0f, 0.0f, 0.0f);
    cXyz sp3C(350.0f, 0.0f, 0.0f);

    mDoMtx_stack_c::YrotS(current.angle.y);
    mDoMtx_stack_c::multVec(&sp30, &sp30);
    mDoMtx_stack_c::multVec(&sp3C, &sp3C);

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::transM(sp30);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::YrotM(mGateLAngle);

    MTXCopy(mDoMtx_stack_c::get(), field_0xc30);
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
    field_0xbe8 = current.pos + sp30;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::transM(sp3C);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::YrotM(0x7FFF);
    mDoMtx_stack_c::YrotM(mGateRAngle);

    MTXCopy(mDoMtx_stack_c::get(), field_0xc00);
    MTXCopy(mDoMtx_stack_c::get(), field_0xae4);
    field_0xbf4 = sp3C;

    if (getSwNo() != 0xFF) {
        cXyz sp48(0.0f, 128.0f, 38.0f);
        mDoMtx_stack_c::YrotS(current.angle.y);
        mDoMtx_stack_c::transM(-350.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::YrotM(mGateLAngle);
        mDoMtx_stack_c::transM(350.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::multVec(&sp48, &sp48);

        cXyz sp54;
        cXyz sp60;
        sp60 = field_0xbc0;

        mDoMtx_stack_c::YrotS(current.angle.y + mGateLAngle);
        mDoMtx_stack_c::multVec(&sp60, &sp60);
        sp54 = field_0xbb4;

        s16 var_r29 = field_0xbb0 * cM_scos(mCounter * 3000);
        cLib_addCalc(&field_0xbb0, 0.0f, 0.05f, 50.0f, 5.0f);

        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::transM(sp60);
        mDoMtx_stack_c::transM(sp48);
        mDoMtx_stack_c::YrotM(current.angle.y + mGateLAngle);
        mDoMtx_stack_c::XrotM(-0xE38);
        mDoMtx_stack_c::XrotM(-field_0xbaa);
        mDoMtx_stack_c::transM(14.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::ZrotM(field_0xbae);
        mDoMtx_stack_c::ZrotM(var_r29);
        mDoMtx_stack_c::transM(-14.0f, 0.0f, 0.0f);
        mpHookModel->setBaseTRMtx(mDoMtx_stack_c::get());

        if (mpKeyModel != NULL) {
            if (field_0xbae == 0) {
                mDoMtx_stack_c::transS(current.pos);
                mDoMtx_stack_c::transM(sp48);
                mDoMtx_stack_c::YrotM(current.angle.y + mGateLAngle);
                mDoMtx_stack_c::XrotM(-0xE38);
                mDoMtx_stack_c::XrotM(-field_0xbaa);
                mpKeyModel->setBaseTRMtx(mDoMtx_stack_c::get());
            } else {
                field_0xbe4 += -2.5f;
                if (field_0xbe4 < -20.0f) {
                    field_0xbe4 = -20.0f;
                }
                mEffPos.y += field_0xbe4;

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

static char* l_arcName = "M_RGate00";

int daObjRgate_c::Create() {
    static char* l_evName = "RIDER_GATE_OPEN00";

    u8 sw_no = getSwNo();
    if (sw_no != 0xFF && !fopAcM_isSwitch(this, sw_no) &&
         /* dSv_event_flag_c::M_035 - Cutscene - [cutscene: 35] after carriage guarding event */
        !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[68]))
    {\
        eventInfo.setArchiveName(l_arcName);

        mEventID = dComIfGp_getEventManager().getEventIdx(this, l_evName, 0xFF);
        mMapToolID = getEventID();
        setAction(ACT_WAIT_EVENT);
    } else {
        mEventID = -1;
        mMapToolID = 0xFF;
        setAction(ACT_DEAD);
    }

    attention_info.position.y += 150.0f;
    eyePos.y += 150.0f;
    attention_info.flags = fopAc_AttnFlag_DOOR_e;

    initBaseMtx();
    fopAcM_SetMtx(this, field_0xb14);
    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z, l_cull_box.max.x,
                          l_cull_box.max.y, l_cull_box.max.z);

    if (dComIfG_Bgsp().Regist(mpBgW, this)) {
        OS_REPORT_ERROR("キコルの門：ＢＧ登録失敗しました\n");
        return 0;
    } else {
        for (u16 i = 0; i < mpGateModel->getModelData()->getJointNum(); i++) {
            if ((int)i == 1 || (int)i == 2) {
                J3DJoint* jnt = mpGateModel->getModelData()->getJointNodePointer(i);
                JUT_ASSERT(448, jnt != NULL);

                jnt->setCallBack(nodeCallBack);
                mpGateModel->setUserArea((uintptr_t)this);
            }
        }
    }

    return 1;
}

int daObjRgate_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 5);
    JUT_ASSERT(472, modelData != NULL);

    mpGateModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpGateModel == NULL) {
        return 0;
    }

    if (getSwNo() != 0xFF) {
        modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 7);
        JUT_ASSERT(485, modelData != NULL);

        J3DModelData* modelData2 = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 5);
        JUT_ASSERT(495, modelData2 != NULL);

        mDoExt_setupShareTexture(modelData, modelData2);
        mpKeyModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (mpKeyModel == NULL) {
            return 0;
        }

        modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 6);
        JUT_ASSERT(511, modelData != NULL);

        mDoExt_setupShareTexture(modelData, modelData2);
        mpHookModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (mpHookModel == NULL) {
            return 0;
        }
    }

    mpBgW = new dBgW();
    if (mpBgW == NULL ||
        mpBgW->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName, 10), cBgW::MOVE_BG_e, &field_0xae4))
    {
        mpBgW = NULL;
        return 0;
    }

    return 1;
}

int daObjRgate_c::create1st() {
    int phase_state = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        phase_state = MoveBGCreate(l_arcName, 10, NULL, 0x2180, NULL);

        if (phase_state == cPhs_ERROR_e) {
            return phase_state;
        }
    }

    return phase_state;
}

int daObjRgate_c::checkAreaL(cXyz const* unused1, cXyz const* unused2) {
    daPy_py_c* player_p = daPy_getPlayerActorClass();
    Mtx m;
    cXyz pos;
    cXyz offset;
    cXyz bound_a;
    cXyz bound_b;

    bound_a.set(0.0f, 0.0f, -100.0f);
    bound_b.set(400.0f, 0.0f, 300.0f);
    offset.set(0.0f, 0.0f, 100.0f);

    fopAc_ac_c* coach_p = fopAcM_Search(search_coach, this);
    if (coach_p != NULL) {
        mDoMtx_stack_c::transS(coach_p->current.pos);
        mDoMtx_stack_c::YrotM(coach_p->shape_angle.y);
        mDoMtx_stack_c::multVec(&offset, &offset);
        pos = offset;

        mDoMtx_inverse(field_0xc30, m);
        mDoMtx_stack_c::copy(m);
        mDoMtx_stack_c::multVec(&pos, &pos);

        if (bound_a.x <= pos.x && pos.x <= bound_b.x && bound_a.z <= pos.z && pos.z <= bound_b.z) {
            return AREA_CHECK_COACH;
        }
    }

    if (daPy_getPlayerActorClass()->checkHorseRide()) {
        offset.set(0.0f, 0.0f, 250.0f);

        daHorse_c* horse_p = dComIfGp_getHorseActor();
        if (horse_p != NULL) {
            mDoMtx_stack_c::transS(horse_p->current.pos);
            mDoMtx_stack_c::YrotM(horse_p->shape_angle.y);
            mDoMtx_stack_c::multVec(&offset, &offset);
            pos = offset;

            mDoMtx_inverse(field_0xc30, m);
            mDoMtx_stack_c::copy(m);
            mDoMtx_stack_c::multVec(&pos, &pos);

            if (bound_a.x <= pos.x && pos.x <= bound_b.x && bound_a.z <= pos.z &&
                pos.z <= bound_b.z)
            {
                return AREA_CHECK_HORSE;
            }
        }
    }

    bound_a.set(0.0f, 0.0f, -100.0f);
    bound_b.set(350.0f, 0.0f, 100.0f);
    pos = player_p->current.pos;

    if (daPy_py_c::checkNowWolf()) {
        offset.set(0.0f, 0.0f, 90.0f);
    } else {
        offset.set(0.0f, 0.0f, 0.0f);
    }

    mDoMtx_stack_c::YrotS(player_p->shape_angle.y);
    mDoMtx_stack_c::multVec(&offset, &offset);
    pos += offset;

    mDoMtx_inverse(field_0xc30, m);
    mDoMtx_stack_c::copy(m);
    mDoMtx_stack_c::multVec(&pos, &pos);

    if (bound_a.x <= pos.x && pos.x <= bound_b.x && bound_a.z <= pos.z && pos.z <= bound_b.z) {
        return AREA_CHECK_PLAYER;
    }

    return 0;
}

int daObjRgate_c::checkAreaR(cXyz const* unused1, cXyz const* unused2) {
    daPy_py_c* player_p = daPy_getPlayerActorClass();
    Mtx m;
    Mtx m2;
    cXyz pos;
    cXyz offset;
    cXyz bound_a;
    cXyz bound_b;

    bound_a.set(0.0f, 0.0f, -300.0f);
    bound_b.set(400.0f, 0.0f, 100.0f);
    offset.set(0.0f, 0.0f, 100.0f);

    fopAc_ac_c* coach_p = fopAcM_Search(search_coach, this);
    if (coach_p != NULL) {
        mDoMtx_stack_c::transS(coach_p->current.pos);
        mDoMtx_stack_c::YrotM(coach_p->shape_angle.y);
        mDoMtx_stack_c::multVec(&offset, &offset);
        pos = offset;

        mDoMtx_inverse(field_0xc00, m);
        mDoMtx_stack_c::copy(m);
        mDoMtx_stack_c::multVec(&pos, &pos);

        if (bound_a.x <= pos.x && pos.x <= bound_b.x && bound_a.z <= pos.z && pos.z <= bound_b.z) {
            return AREA_CHECK_COACH;
        }
    }

    if (daPy_getPlayerActorClass()->checkHorseRide()) {
        offset.set(0.0f, 0.0f, 250.0f);

        daHorse_c* horse_p = dComIfGp_getHorseActor();
        if (horse_p != NULL) {
            mDoMtx_stack_c::transS(horse_p->current.pos);
            mDoMtx_stack_c::YrotM(horse_p->shape_angle.y);
            mDoMtx_stack_c::multVec(&offset, &offset);
            pos = offset;

            mDoMtx_inverse(field_0xc00, m);
            mDoMtx_stack_c::copy(m);
            mDoMtx_stack_c::multVec(&pos, &pos);

            if (bound_a.x <= pos.x && pos.x <= bound_b.x && bound_a.z <= pos.z &&
                pos.z <= bound_b.z)
            {
                return AREA_CHECK_HORSE;
            }
        }
    }

    bound_a.set(0.0f, 0.0f, -100.0f);
    bound_b.set(350.0f, 0.0f, 100.0f);
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

    mDoMtx_inverse(field_0xc00, m2);
    mDoMtx_stack_c::copy(m2);
    mDoMtx_stack_c::multVec(&pos, &pos);

    if (bound_a.x <= pos.x && pos.x <= bound_b.x && bound_a.z <= pos.z && pos.z <= bound_b.z) {
        return AREA_CHECK_PLAYER;
    }

    return 0;
}

BOOL daObjRgate_c::checkOpen() {
    if (dComIfGs_getKeyNum() == 0) {
        return false;
    }

    daPy_py_c* player_p = daPy_getPlayerActorClass();

    if (abs((s16)(current.angle.y - player_p->current.angle.y)) < 0x5000) {
        return false;
    }

    Mtx m;
    mDoMtx_inverse(field_0xb14, m);

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

BOOL daObjRgate_c::checkDirL(fopAc_ac_c* i_actor) {
    Mtx m;
    mDoMtx_inverse(field_0xc30, m);

    cXyz pos(i_actor->current.pos);
    mDoMtx_stack_c::copy(m);
    mDoMtx_stack_c::multVec(&pos, &pos);

    if (pos.z > 0.0f) {
        return false;
    } else {
        return true;
    }
}

BOOL daObjRgate_c::checkDirR(fopAc_ac_c* i_actor) {
    Mtx m;
    mDoMtx_inverse(field_0xc00, m);

    cXyz pos(i_actor->current.pos);
    mDoMtx_stack_c::copy(m);
    mDoMtx_stack_c::multVec(&pos, &pos);

    if (pos.z < 0.0f) {
        return false;
    } else {
        return true;
    }
}

void daObjRgate_c::setAtkSE() {
    fopAcM_seStart(this, Z2SE_OBJ_RIDER_GATE_ATK, 0);
}

void daObjRgate_c::setCrkSE() {
    fopAcM_seStart(this, Z2SE_OBJ_RIDER_GATE_CRK, 0);
}

void daObjRgate_c::action() {
        /* dSv_event_flag_c::M_035 - Cutscene - [cutscene: 35] after carriage guarding event */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[68])) {
        mGateLAngle = 0x4000;
        mGateRAngle = -0x4000;
    } else {
        action_typeA();
    }
}

void daObjRgate_c::action_typeA() {
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

                field_0xbb0 = 2000.0f;
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

                field_0xbb0 = 2500.0f;
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

                field_0xbb0 = 2500.0f;
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
        cLib_addCalcAngleS(&field_0xbae, 0x3000, 10, 0x400, 0x100);
        cLib_chaseF(&field_0xbb4.x, -39.0f, 1.0f);
        cLib_chaseF(&field_0xbc0.x, -25.0f, 1.0f);
        cLib_addCalcAngleS(&field_0xbcc, 0x7FFF, 10, 0x400, 0x100);
    } else {
        cXyz unused1;
        cXyz unused2;

        int chk_l = checkAreaL(&unused1, &unused2);
        int chk_r = checkAreaR(&unused1, &unused2);
        if (chk_l != 0 || chk_r != 0) {
            daPy_py_c* player_p = daPy_getPlayerActorClass();

            if (daPy_py_c::checkNowWolf()) {
                if (player_p->checkWolfAttackReverse() || player_p->checkFrontRollCrash()) {
                    field_0xb94 = 1200.0f;
                    field_0xb90 = 1200.0f;
                    field_0xbac = 0x800;
                    setAtkSE();
                }
            } else {
                if (player_p->checkFrontRollCrash()) {
                    field_0xb94 = 600.0f;
                    field_0xb90 = 600.0f;
                    field_0xbac = 0x800;
                    setAtkSE();
                }
            }
        }

        mGateLAngle = field_0xb90 * cM_ssin(mCounter * 4000);
        mGateRAngle = field_0xb94 * -cM_ssin(mCounter * 4000);
        cLib_addCalc0(&field_0xb90, 0.1f, 20.0f);
        cLib_addCalc0(&field_0xb94, 0.1f, 20.0f);

        field_0xbac -= 0x160;
        field_0xbaa += field_0xbac;
        if (field_0xbaa >= 0) {
            return;
        }

        field_0xbaa = 0;
        field_0xbac = -field_0xbac * 0.7f;
    }
}

void daObjRgate_c::actionWaitEvent() {
    if (eventInfo.checkCommandDoor()) {
        setAction(ACT_EVENT);
        dComIfGp_setItemKeyNumCount(-1);
        fopAcM_onSwitch(this, getSwNo());
        fopAcM_seStart(this, Z2SE_OBJ_RIDER_GATE_L_OP, 0);
    } else if (checkOpen()) {
        eventInfo.setEventId(mEventID);
        eventInfo.setMapToolId(mMapToolID);
        eventInfo.onCondition(dEvtCnd_CANDOOR_e);
    }
}

void daObjRgate_c::actionEvent() {
    if (dComIfGp_evmng_endCheck(mEventID)) {
        setAction(ACT_DEAD);
        dComIfGp_event_reset();
    } else {
        demoProc();
    }
}

void daObjRgate_c::actionDead() {}

void daObjRgate_c::event_proc_call() {
    typedef void (daObjRgate_c::*func)();
    static func l_func[] = {&daObjRgate_c::actionWaitEvent, &daObjRgate_c::actionEvent,
                            &daObjRgate_c::actionDead};

    (this->*l_func[mAction])();
}

void daObjRgate_c::demoProc() {
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

int daObjRgate_c::getDemoAction() {
    static char* action_table[] = {"WAIT", "ADJUSTMENT", "UNLOCK", "OPEN"};

    return dComIfGp_evmng_getMyActIdx(mStaffID, action_table, ARRAY_SIZEU(action_table), 0, 0);
}

int daObjRgate_c::Execute(Mtx** param_0) {
    event_proc_call();
    action();
    mCounter++;

    *param_0 = &mBgMtx;
    setBaseMtx();
    mpBgW->Move();
    return 1;
}

int daObjRgate_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpGateModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpGateModel);
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

int daObjRgate_c::Delete() {
    if (mpBgW != NULL && mpBgW->ChkUsed()) {
        if (dComIfG_Bgsp().Release(mpBgW)) {
            OS_REPORT("Release Error\n");
        }
    }

    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daObjRgate_create1st(daObjRgate_c* i_this) {
    fopAcM_ct(i_this, daObjRgate_c);
    return i_this->create1st();
}

static int daObjRgate_MoveBGDelete(daObjRgate_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjRgate_MoveBGExecute(daObjRgate_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjRgate_MoveBGDraw(daObjRgate_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjRgate_METHODS = {
    (process_method_func)daObjRgate_create1st,     (process_method_func)daObjRgate_MoveBGDelete,
    (process_method_func)daObjRgate_MoveBGExecute, (process_method_func)NULL,
    (process_method_func)daObjRgate_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_RiderGate = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_RiderGate,
    &g_fpcLf_Method.base,
    sizeof(daObjRgate_c),
    0,
    0,
    &g_fopAc_Method.base,
    600,
    &daObjRgate_METHODS,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
