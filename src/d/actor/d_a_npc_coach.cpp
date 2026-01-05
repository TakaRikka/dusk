/**
 * @file d_a_npc_coach.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_coach.h"
#include "d/actor/d_a_coach_2D.h"
#include "d/actor/d_a_coach_fire.h"
#include "d/actor/d_a_e_kr.h"
#include "d/actor/d_a_npc_theB.h"
#include "d/d_bg_w.h"
#include "d/d_meter2_info.h"
#include "d/d_tresure.h"

enum Coach_RES_File_ID {
    /* BCK */
    /* 0x09 */ BCK_HU_JUMP_END = 0x9,
    /* 0x0A */ BCK_HU_JUMP_MIDDLE,
    /* 0x0B */ BCK_HU_JUMP_START,
    /* 0x0C */ BCK_HU_LASH,
    /* 0x0D */ BCK_HU_RUN_FAST,
    /* 0x0E */ BCK_HU_RUN_SLOW,
    /* 0x0F */ BCK_HU_WAIT_01,
    /* 0x10 */ BCK_HU_WALK_FAST,
    /* 0x11 */ BCK_HU_WALK_SLOW,
    /* 0x12 */ BCK_THEB_BEND_TURN,
    /* 0x13 */ BCK_THEB_BEND_WAIT,
    /* 0x14 */ BCK_THEB_F_BEND_TURN,
    /* 0x15 */ BCK_THEB_F_TALK_A,
    /* 0x16 */ BCK_THEB_F_TALK_B,
    /* 0x17 */ BCK_THEB_F_TALK_C,
    /* 0x18 */ BCK_THEB_FH_BEND_WAIT,
    /* 0x19 */ BCK_THEB_FH_TALK_B,
    /* 0x1A */ BCK_THEB_FH_TALK_C,
    /* 0x1B */ BCK_THEB_SIT,
    /* 0x1C */ BCK_THEB_SIT_B,
    /* 0x1D */ BCK_THEB_WHIP,
    /* 0x1E */ BCK_THEB_WHIP_B,
    /* 0x1F */ BCK_YELIA_BEND,
    /* 0x20 */ BCK_YELIA_WAIT,

    /* BMDR */
    /* 0x23 */ BMDR_COACH = 0x23,
    /* 0x24 */ BMDR_HARNESS,
    /* 0x25 */ BMDR_HORSE,
    /* 0x26 */ BMDR_THEB,
    /* 0x27 */ BMDR_YELIA,

    /* BTK */
    /* 0x2A */ BTK_THEB = 0x2A,

    /* BTP */
    /* 0x2D */ BTP_HU_EYE = 0x2D,
    /* 0x2E */ BTP_THEB,
    /* 0x2F */ BTP_THEB_F_TALK_A,

    /* DZB */
    /* 0x32 */ DZB_COACH = 0x32,
    /* 0x33 */ DZB_HARNESS,

    /* EVT */
    /* 0x36 */ EVT_EVENT_LIST = 0x36,

    /* TEX */
    /* 0x39 */ TEX_TAZUNA = 0x39,
};

enum Coach_Joint {
    /* 0x0 */ JNT_COACH_CENTER,
    /* 0x1 */ JNT_COACH_B_L_WHEEL,
    /* 0x2 */ JNT_COACH_B_R_WHEEL,
    /* 0x3 */ JNT_COACH_COACH_BG_LOC,
    /* 0x4 */ JNT_COACH_BODY,
    /* 0x5 */ JNT_COACH_TERUMA_LOC,
    /* 0x6 */ JNT_COACH_YELIA_LOC,
};

enum Harness_Joint {
    /* 0x0 */ JNT_HARNESS_CENTER,
    /* 0x1 */ JNT_HARNESS_AXLE,
    /* 0x2 */ JNT_HARNESS_F_L_WHEEL,
    /* 0x3 */ JNT_HARNESS_F_R_WHEEL,
    /* 0x4 */ JNT_HARNESS_BS_LOC,
    /* 0x5 */ JNT_HARNESS_HU_LOC,
};

enum Horse_Joint {
    /* 0x00 */ JNT_HORSE_CENTER,
    /* 0x01 */ JNT_HORSE_BACKBONE2,
    /* 0x02 */ JNT_HORSE_BACKBONE1,
    /* 0x03 */ JNT_HORSE_F_L_LEG1,
    /* 0x04 */ JNT_HORSE_F_L_LEG2,
    /* 0x05 */ JNT_HORSE_F_L_LEG3,
    /* 0x06 */ JNT_HORSE_F_L_LEG4,
    /* 0x07 */ JNT_HORSE_F_R_LEG1,
    /* 0x08 */ JNT_HORSE_F_R_LEG2,
    /* 0x09 */ JNT_HORSE_F_R_LEG3,
    /* 0x0A */ JNT_HORSE_R_R_LEG4,
    /* 0x0B */ JNT_HORSE_BG_LOC,
    /* 0x0C */ JNT_HORSE_NECK1,
    /* 0x0D */ JNT_HORSE_NECK2,
    /* 0x0E */ JNT_HORSE_HEAD,
    /* 0x0F */ JNT_HORSE_MOUTH1,
    /* 0x10 */ JNT_HORSE_MOUTH2,
    /* 0x11 */ JNT_HORSE_WAIST,
    /* 0x12 */ JNT_HORSE_B_L_LEG1,
    /* 0x13 */ JNT_HORSE_B_L_LEG2,
    /* 0x14 */ JNT_HORSE_B_L_LEG3,
    /* 0x15 */ JNT_HORSE_B_L_LEG4,
    /* 0x16 */ JNT_HORSE_B_R_LEG1,
    /* 0x17 */ JNT_HORSE_B_R_LEG2,
    /* 0x18 */ JNT_HORSE_B_R_LEG3,
    /* 0x19 */ JNT_HORSE_B_R_LEG4,
    /* 0x1A */ JNT_HORSE_TAIL1,
    /* 0x1B */ JNT_HORSE_TAIL2,
    /* 0x1C */ JNT_HORSE_TAIL3,
};

enum Telma_Joint {
    /* 0x00 */ JNT_TELMA_CENTER,
    /* 0x01 */ JNT_TELMA_BACKBONE1,
    /* 0x02 */ JNT_TELMA_BACKBONE2,
    /* 0x03 */ JNT_TELMA_ARML1,
    /* 0x04 */ JNT_TELMA_ARML2,
    /* 0x05 */ JNT_TELMA_HANDL,
    /* 0x06 */ JNT_TELMA_ARMR1,
    /* 0x07 */ JNT_TELMA_ARMR2,
    /* 0x08 */ JNT_TELMA_HANDR,
    /* 0x09 */ JNT_TELMA_NECK,
    /* 0x0A */ JNT_TELMA_HEAD,
    /* 0x0B */ JNT_TELMA_CHIN,
    /* 0x0C */ JNT_TELMA_HAIR,
    /* 0x0D */ JNT_TELMA_MOUTH,
    /* 0x0E */ JNT_TELMA_WAIST,
    /* 0x0F */ JNT_TELMA_LEGL2,
    /* 0x10 */ JNT_TELMA_LEGR2,
};

enum Ilia_Joint {
    /* 0x0 */ JNT_ILIA_CENTER,
    /* 0x1 */ JNT_ILIA_WAIST,
    /* 0x2 */ JNT_ILIA_BACKBONE,
    /* 0x3 */ JNT_ILIA_NECK,
    /* 0x4 */ JNT_ILIA_HEAD,
};

static int jointHorseCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 != 0) {
        return 1;
    }

    J3DModel* model = j3dSys.getModel();
    daNpcCoach_c* i_this = (daNpcCoach_c*)model->getUserArea();
    if (i_this != NULL) {
        i_this->ctrlJointHorse(i_joint, model);
    }
    return 1;
}

static int jointFrontWheelCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 != 0) {
        return 1;
    }

    J3DJoint* reg_r28 = i_joint;
    int jointNo = reg_r28->getJntNo();
    J3DModel* model = j3dSys.getModel();
    daNpcCoach_c* i_this = (daNpcCoach_c*)model->getUserArea();
    csXyz* front_wheel_rot = i_this->getFrontWheelRot(jointNo);
    cMtx_copy(model->getAnmMtx(jointNo), mDoMtx_stack_c::get());
    mDoMtx_stack_c::YrotM(front_wheel_rot->y);
    mDoMtx_stack_c::ZrotM(front_wheel_rot->z);
    mDoMtx_stack_c::XrotM(front_wheel_rot->x);
    model->setAnmMtx(jointNo, mDoMtx_stack_c::get());
    mDoMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    return 1;
}

static int jointRearWheelCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 != 0) {
        return 1;
    }

    J3DJoint* reg_r28 = i_joint;
    int jointNo = reg_r28->getJntNo();
    J3DModel* model = j3dSys.getModel();
    daNpcCoach_c* i_this = (daNpcCoach_c*)model->getUserArea();
    csXyz* rear_wheel_rot = i_this->getRearWheelRot(jointNo);
    cMtx_copy(model->getAnmMtx(jointNo), mDoMtx_stack_c::get());
    mDoMtx_stack_c::YrotM(rear_wheel_rot->y);
    mDoMtx_stack_c::ZrotM(rear_wheel_rot->z);
    mDoMtx_stack_c::XrotM(rear_wheel_rot->x);
    model->setAnmMtx(jointNo, mDoMtx_stack_c::get());
    mDoMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    return 1;
}

daNpcCoach_Attr_c const daNpcCoach_c::M_attr = {
    -3.0f,
    -200.0f,
    -0.25f,
    -387.0f,
    -190.0f,
    82.0f,
    96.0f,
    80.0f,
    0.5f,
    0.85f,
    6.0f,
    22.0f,
    30.0f,
    40.0f,
    4.0f,
    400.0f,
    1500.0f,
    16.0f,
    62.0f,
    -58.837898f,
    67.627f,
    0.0f,
    0.6f,
    20.0f,
    13,
    0x1800,
    0x100,
    0x400,
    0x400,
    0,
    0,
    0,
    0,
    0,
    1,
    0x5DC,
    7,
    0,
    0,
    0,
    1000.0f,
    -0x2000,
    0x4000,
    0,
    0xFF,
    0x50,
    0,
    3,
    0,
    6000.0f,
    10.0f,
    20.0f,
    0.0f,
    0x19,
};

static int jointCoachCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 != 0) {
        return 1;
    }

    int jointNo = i_joint->getJntNo();
    J3DModel* model = j3dSys.getModel();
    daNpcCoach_c* i_this = (daNpcCoach_c*)model->getUserArea();
    MTXCopy(model->getAnmMtx(jointNo), mDoMtx_stack_c::get());
    mDoMtx_stack_c::XrotM(i_this->getCoachRotate());
    mDoMtx_stack_c::transM(0.0f, i_this->getCoachTrans(), 0.0f);
    model->setAnmMtx(jointNo, mDoMtx_stack_c::get());
    mDoMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    return 1;
}

static void arrowStickCallBack(dBgW* i_bgw, fopAc_ac_c* a_this, fopAc_ac_c* i_actor, cXyz& i_pos) {
    if (fopAcM_GetName(i_actor) == PROC_E_ARROW) {
        if (!daPy_getPlayerActorClass()->checkBoarSingleBattleSecond()) {
            ((daNpcCoach_c*)a_this)->hitFireArrow(i_pos);
            fopAcM_delete(i_actor);
        }
    }
}

void daNpcCoach_c::hitFireArrow(cXyz i_pos) {
    if (field_0x24c0 == 0) {
        mChYelia.mSound.startCreatureVoice(Z2SE_YELIA_V_KYAAA_TRIG, -1);
    }

    if (field_0x24c0 < 5) {
        for (int i = 0; i < 5; i++) {
            if (field_0x247c[i] == fpcM_ERROR_PROCESS_ID_e) {
                cXyz work;
                mDoMtx_stack_c::copy(mChCoach.field_0x598);
                mDoMtx_stack_c::multVec(&i_pos, &work);
                field_0x247c[i] = fopAcM_createChild(PROC_COACH_FIRE, fopAcM_GetID(this),
                                                     0, &work, fopAcM_GetRoomNo(this),
                                                     NULL, NULL, -1, NULL);
                mChCoach.mSound.startSound(Z2SE_COACH_HIT_ARROW, 0, -1);
                field_0x24c0++;
                return;
            }
        }
    }
}

void daNpcCoach_c::deleteFireArrow(fpc_ProcID param_1) {
    for (int i = 0; i < 5; i++) {
        if (field_0x247c[i] != param_1) {
            continue;
        }

        field_0x2559 = 1;
        mChCoach.mSound.startSound(Z2SE_COACH_HIT_WIND, 0, -1);

        mTotalDmgRecv = (mTotalDmgRecv / (attr().damage_durability / 20) - attr().damage_extinguish_level)
                        * (attr().damage_durability / 20);

        if (mTotalDmgRecv <= 0) {
            mTotalDmgRecv = 0;
            field_0x24c0 = 0;

            for (int i = 0; i < 5; i++) {
                if (field_0x247c[i] != fpcM_ERROR_PROCESS_ID_e) {
                    fopAcM_delete(field_0x247c[i]);
                    field_0x247c[i] = fpcM_ERROR_PROCESS_ID_e;
                }
            }
        } else {
            for (int i = 0; i < 5; i++) {
                if (field_0x247c[i] != fpcM_ERROR_PROCESS_ID_e) {
                    ((daCoachFire_c*)fpcM_SearchByID(field_0x247c[i]))->setNoHitTimer();
                }
            }

            mChCoach.field_0x77c = 20;
        }

        break;
    }
}

bool daNpcCoach_c::deleteFireArrowFromList(unsigned int i_actorID) {
    for (int i = 0; i < 5; i++) {
        if (field_0x247c[i] == i_actorID) {
            field_0x247c[i] = -1;
            return true;
        }
    }
    return false;
}

void daNpcCoach_c::setPosAngle(cXyz& param_1, csXyz& param_2) {
    initCoachPosition(param_1, param_2);
    changeAtherPath(getRailID(), param_1, param_2);
}

int daNpcCoach_c::ctrlJointHorse(J3DJoint* i_joint, J3DModel* i_model) {
    int jointNo = i_joint->getJntNo();

    if ((jointNo == JNT_HORSE_BG_LOC || jointNo == JNT_HORSE_MOUTH2) && mChHorse.field_0x78c != 0) {
        J3DAnmTransform* morfAnm = mChHorse.mpBckAnm->getBckAnm();
        J3DAnmTransform* bckAnm = mChHorse.mpModelMorf->getAnm();
        mChHorse.mpBckAnm->changeBckOnly(bckAnm);
        mChHorse.mpModelMorf->changeAnm(morfAnm, NULL);
        f32 anmRate = mChHorse.mpModelMorf->getAnmRate();
        mChHorse.mpModelMorf->setAnmRate(mChHorse.mAnmRate);
        mChHorse.mAnmRate = anmRate;
    }

    return 1;
}

void daNpcCoach_c::initCoachPosition(Vec& i_pos, SVec& i_angle) {
    current.pos = i_pos;
    shape_angle.set(i_angle.x, i_angle.y, i_angle.z);
    old.pos = current.pos;

    mDoMtx_stack_c::transS(i_pos);
    mDoMtx_stack_c::ZXYrotM(i_angle.x, i_angle.y, i_angle.z);
    mDoMtx_stack_c::transM(0.0f, 0.0f, attr().field_0xc);
    mDoMtx_stack_c::multVecZero(&mChHarness.field_0x6e0);
    mChHarness.field_0x6ec = mChHarness.field_0x6e0;
    mChHarness.field_0x700.y = shape_angle.y;
    mChHarness.field_0x706 = mChHarness.field_0x700;
    mChHarness.field_0x70c.field_0x0.set(attr().field_0x14, 0.0f, 0.0f);
    mChHarness.field_0x730.field_0x0.set(-attr().field_0x14, 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&mChHarness.field_0x70c.field_0x0, &mChHarness.field_0x70c.field_0x0);
    mDoMtx_stack_c::multVec(&mChHarness.field_0x730.field_0x0, &mChHarness.field_0x730.field_0x0);

    mDoMtx_stack_c::transM(0.0f, 0.0f, attr().field_0x10);
    mDoMtx_stack_c::multVecZero(&mChCoach.field_0x5e0);
    mChCoach.field_0x5ec = mChCoach.field_0x5e0;
    mChCoach.field_0x79a.y = shape_angle.y;
    mChCoach.field_0x7a0 = mChCoach.field_0x79a;
    mChCoach.field_0x788 = attr().shake_dist + cM_rndF(attr().shake_dist);
    mChCoach.field_0x7a8.field_0x0.set(attr().field_0x18, 0.0f, 0.0f);
    mChCoach.field_0x7cc.field_0x0.set(-attr().field_0x18, 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&mChCoach.field_0x7a8.field_0x0, &mChCoach.field_0x7a8.field_0x0);
    mDoMtx_stack_c::multVec(&mChCoach.field_0x7cc.field_0x0, &mChCoach.field_0x7cc.field_0x0);

    cXyz work(mChHarness.field_0x6e0.x, mChHarness.field_0x6e0.y + 500.0f, mChHarness.field_0x6e0.z);
    field_0x255c = work;

    if (fopAcM_gc_c::gndCheck(&work)) {
        mChHarness.field_0x6e0.y = fopAcM_gc_c::getGroundY();
    }

    work.set(mChCoach.field_0x5e0.x, mChCoach.field_0x5e0.y + 500.0f, mChCoach.field_0x5e0.z);

    if (fopAcM_gc_c::gndCheck(&work)) {
        mChCoach.field_0x5e0.y = fopAcM_gc_c::getGroundY(); 
    }
}

static void* s_sub(void* i_actor, void* i_data) {
    if (fopAc_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_KR && ((e_kr_class*)i_actor)->getId() >= 0) {
        return i_actor;
    }

    return NULL;
}

void daNpcCoach_c::initCollision() {
    static dCcD_SrcCyl const ccCylSrc = {
        {
            {0, {{0, 0, 0}, {0x2420, 0x11}, 0x79}},
            {dCcD_SE_NONE, 0, 0, 0, 0},
            {dCcD_SE_NONE, 0, 0, 0, 4},
            0,
        },
        {
            {
                0.0f, 0.0f
            }
        }
    };

    static dCcD_SrcSph const ccSphSrc = {
        {
            {0, {{0, 0, 0}, {0x10000, 0x11}, 0}},
            {dCcD_SE_NONE, 0, 0, 0, 0},
            {dCcD_SE_NONE, 0, 0, 0, 4},
            0,
        },
        {
            {{0.0f, 0.0f, 0.0f}, 50.0f},
        }
    };
    mAcchCir.SetWall(45.0f, 50.0f);
    mChHorse.mBgc.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this),
                      fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
    mChHorse.mBgc.SetRoofNone();
    mChHorse.mBgc.SetWaterNone();
    mChHorse.mBgc.SetWallNone();
    mChHorse.mBgc.OnLineCheckNone();
    mChHorse.mBgc.CrrPos(dComIfG_Bgsp());
    mStts.Init(0xFF, 0xFF, this);

    for (int i = 0; i < 4; i++) {
        mChHorse.field_0x1f0[i].Set(ccCylSrc);
        mChHorse.field_0x1f0[i].SetStts(&mStts);
        mChHorse.field_0x1f0[i].SetR(45.0f);
        mChHorse.field_0x1f0[i].SetH(250.0f);
    }

    mChHarness.mBgc.Set(&mChHarness.field_0x6e0, &mChHarness.field_0x6ec, this, 1, &mAcchCir, &mChHarness.field_0x6d4, &mChHarness.field_0x700, &mChHarness.field_0x700);
    mChHarness.mBgc.SetRoofNone();
    mChHarness.mBgc.SetWaterNone();
    mChHarness.mBgc.SetWallNone();
    mChHarness.mBgc.OnLineCheckNone();
    mChHarness.mBgc.OffClrSpeedY();
    mChHarness.mBgc.SetGroundCheckOffset(150.0f);
    mChHarness.mBgc.CrrPos(dComIfG_Bgsp());
    mChHarness.field_0x564.Set(ccCylSrc);
    mChHarness.field_0x564.SetStts(&mStts);
    mChHarness.field_0x564.SetR(85.0f);
    mChHarness.field_0x564.SetH(120.0f);
    mChHarness.field_0x6a0->SetCrrFunc(dBgS_MoveBGProc_TypicalRotY);
    dComIfG_Bgsp().Regist(mChHarness.field_0x6a0, this);

    mChCoach.mBgc.Set(&mChCoach.field_0x5e0, &mChCoach.field_0x5ec, this, 1, &mAcchCir, &mChCoach.field_0x5c8, &mChCoach.field_0x79a, &mChCoach.field_0x79a);
    mChCoach.mBgc.SetRoofNone();
    mChCoach.mBgc.SetWaterNone();
    mChCoach.mBgc.SetWallNone();
    mChCoach.mBgc.OnLineCheckNone();
    mChCoach.mBgc.OffClrSpeedY();
    mChCoach.mBgc.SetGroundCheckOffset(150.0f);
    mChCoach.mBgc.CrrPos(dComIfG_Bgsp());
    mChCoach.field_0x564->SetCrrFunc(dBgS_MoveBGProc_TypicalRotY);
    mChCoach.field_0x564->SetArrowStickCallback(arrowStickCallBack);
    dComIfG_Bgsp().Regist(mChCoach.field_0x564, this);
    mChCoach.field_0x644.Set(ccSphSrc);
    mChCoach.field_0x644.SetStts(&mStts);
    mChCoach.field_0x644.SetC(eyePos);
    mChCoach.field_0x644.SetR(50.0f);

    for (int i = 0; i < 2; i++) {
        mChYelia.field_0x41c[i].Set(ccCylSrc);
        mChYelia.field_0x41c[i].SetStts(&mStts);
        mChYelia.field_0x41c[i].SetR(45.0f);

        f32 h = i == 0 ? 100.0f : 50.0f;
        mChYelia.field_0x41c[i].SetH(h);
    }
}


BOOL daNpcChPath_c::setPath(int path_index, int room_no, cXyz& i_vec, bool param_4) {
    mpPath = dPath_GetRoomPath(path_index, room_no);

    if (mpPath == NULL) {
        return FALSE;
    }

    mCurrentID = path_index;
    mPntIndex = 0;

    // Is this really what the Nintendo devs wrote?? The debug ROM suggests as such.
    if (param_4 && &i_vec) {
        f32 fVar1 = G_CM3D_F_INF;
        for (int pnt_index = 0; pnt_index < mpPath->m_num; pnt_index++) {
            dPnt* pnt = dPath_GetPnt(mpPath, pnt_index);
            f32 fVar2 = i_vec.abs2(pnt->m_position);
            if (fVar2 < fVar1) {
                fVar1 = fVar2;
                mPntIndex = pnt_index;
            }
        }

        setNextPoint();
    }

    field_0x8 = G_CM3D_F_INF;

    // ditto.
    if (&i_vec) {
        field_0x4 = &i_vec;
        cXyz targetPoint;
        getTargetPoint(targetPoint);
        field_0x8 = field_0x4->abs(targetPoint);
    }

    return TRUE;
}

void daNpcCoach_c::changeAtherPath(s8 path_index, cXyz& param_2, csXyz& param_3) {
    mChPath.setPath(path_index, fopAcM_GetRoomNo(this), param_2, true);

    if (mChPath.isPath()) {
        mPathID = path_index;

        cXyz targetPoint;
        mChPath.getTargetPoint(targetPoint);

        int dist_ang = cLib_distanceAngleS(param_3.y, cLib_targetAngleY(&param_2, &targetPoint));
        if (dist_ang >= 0x4000) {
            mChPath.setNextPoint();
            mChPath.getTargetPoint(targetPoint);

            int o_dist_ang = cLib_distanceAngleS(shape_angle.y, cLib_targetAngleY(&param_2, &targetPoint));
            if (o_dist_ang >= 0x4000) {
                mChPath.setPrevPoint();
            }
        }
    }
}

static int createSolidHeap(fopAc_ac_c* a_this) {
    daNpcCoach_c* i_this = (daNpcCoach_c*)a_this;
    return i_this->createHeap();
}

static char* l_arcName = "Coach";

static struct {
    int field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    int field_0xc;
    f32 field_0x10;
    f32 field_0x14;
} l_horseAnmParam[5] = {
    {BCK_HU_WAIT_01, 0.0f, 1.0f, BCK_HU_WALK_SLOW, 6.0f, 2.0f},
    {BCK_HU_WALK_SLOW, 6.0f, 2.0f, BCK_HU_WALK_FAST, 22.0f, 2.0f},
    {BCK_HU_WALK_FAST, 22.0f, 2.0f, BCK_HU_RUN_SLOW, 30.0f, 3.0f},
    {BCK_HU_RUN_SLOW, 30.0f, 3.0f, BCK_HU_RUN_FAST, 40.0f, 2.8f},
    {BCK_HU_RUN_FAST, 40.0f, 2.8f, -1, 40.0f, 2.8f},
};

void daNpcCoach_c::setCoachBlazing(u8 damage) {
    static u16 const ParticleName[10] = {
        0x85F0, 0x85EF, 0x85E7, 0x85E8, 0x85E9,
        0x85EA, 0x85EB, 0x85EC, 0x85ED, 0x85EE,
    };

    // I beg your pardon Nintendo devs, but u8s cannot go higher than 0xff...
    JUT_ASSERT(2502, damage <= 0xff);
    f32 reg_f31 = 0.0f;
    f32 reg_f30 = 0.0f;
    int reg_r27 = 0;
    int reg_r26 = 0;
    int reg_r29 = damage / 63;

    if (damage == 0) {
        reg_r29 = 2;
        mCoachBlazing = false;
        reg_r27 = 0;
        reg_f31 = 0.0f;
        reg_r26 = 0;
        reg_f30 = 0.0f;
        attention_info.flags = 0;
    } else if (mCoachBlazing) {
        switch (reg_r29) {
            case 0:
                reg_r29 = 4;
                reg_f31 = 0.0f;
                reg_f30 = 0.0f;
                break;

            case 1:
                reg_r29 = 5;
                reg_f31 = 0.0f;
                reg_f30 = 1.5f;
                break;

            case 2:
                reg_r29 = 7;
                reg_f31 = 0.4f;
                reg_f30 = 1.5f;
                break;

            case 3:
            case 4:
                reg_r29 = 10;
                reg_f31 = 0.6f;
                reg_f30 = 1.5f;
                break;

            default:
                JUT_ASSERT(2552, FALSE);
                break;
        }

        reg_r27 = (u8) damage;
        reg_r26 = (u8) damage;
    } else if (reg_r29 == 1) {
        reg_r29 = 5;
        mCoachBlazing = true;
        reg_r27 = damage;
        reg_f31 = 0.0f;
        reg_r26 = 0;
        reg_f30 = 0.0f;
        attention_info.flags = fopAc_AttnFlag_CHECK_e | fopAc_AttnFlag_LOCK_e;
    } else {
        reg_r29 = 2;
        reg_r27 = 0;
        reg_f31 = 0.0f;
        reg_r26 = 0;
        reg_f30 = 0.0f;
        attention_info.flags = 0;
    }

    cXyz work;

    mDoMtx_stack_c::copy(mChCoach.mCoachModel->getAnmMtx(JNT_COACH_BODY));
    mDoMtx_stack_c::multVecZero(&work);

    int i;
    for (i = 2; i < reg_r29; i++) {
        if (mChCoach.field_0x5f8[i] == NULL) {
            mChCoach.field_0x5f8[i] = dComIfGp_particle_set(ParticleName[i], &work,
                                                            &mChCoach.field_0x79a, NULL);
            if (mChCoach.field_0x5f8[i] != NULL) {
                mChCoach.field_0x5f8[i]->becomeImmortalEmitter();
            }
        }
    }

    i = reg_r29;
    for (; i < 10; i++) {
        if (mChCoach.field_0x5f8[i] != NULL) {
            mChCoach.field_0x5f8[i]->becomeInvalidEmitter();
            mChCoach.field_0x5f8[i] = NULL;
        }
    }

    for (i = 0; i < 10; i++) {
        if (mChCoach.field_0x5f8[i] != NULL) {
            mChCoach.field_0x5f8[i]->setGlobalRTMatrix(mChCoach.mCoachModel->getAnmMtx(JNT_COACH_BODY));
        }
    }

    mChCoach.field_0x5d4 = mChCoach.field_0x5c8 * 0.8f;
    mChCoach.field_0x5f8[0]->setGlobalAlpha(reg_r27);
    mChCoach.field_0x5f8[0]->setRate(reg_f31);
    mChCoach.field_0x5f8[0]->setUserWork((uintptr_t)&mChCoach.field_0x5d4);
    mChCoach.field_0x5f8[1]->setGlobalAlpha(reg_r26);
    mChCoach.field_0x5f8[1]->setRate(reg_f30);
    mChCoach.field_0x5f8[1]->setUserWork((uintptr_t)&mChCoach.field_0x5d4);
}

int daNpcCoach_c::createHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, BMDR_HORSE);

    JUT_ASSERT(2702, modelData != NULL);

    mChHorse.mpModelMorf = new mDoExt_McaMorf2(modelData, NULL, NULL,
                                               (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, BCK_HU_WAIT_01), NULL,
                                               2, 1.0f, 0, -1, &mChHorse.mSound, 0x80000, 0x11020084);
    if (mChHorse.mpModelMorf != NULL && mChHorse.mpModelMorf->getModel() == NULL) {
        mChHorse.mpModelMorf->stopZelAnime();
        mChHorse.mpModelMorf = NULL;
    }

    if (mChHorse.mpModelMorf == NULL) {
        return 0;
    }

    modelData = mChHorse.mpModelMorf->getModel()->getModelData();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        modelData->getJointNodePointer(i)->setCallBack(jointHorseCallBack);
    }
    mChHorse.mpModelMorf->getModel()->setUserArea((uintptr_t)this);

    mChHorse.mpBtpAnm = new mDoExt_btpAnm();
    J3DAnmTexPattern* i_btk = (J3DAnmTexPattern*)dComIfG_getObjectRes(l_arcName, BTP_HU_EYE);
    if (mChHorse.mpBtpAnm == NULL || mChHorse.mpBtpAnm->init(modelData, i_btk, 1, 0,
                                                             1.0f, 0, -1) == 0) {
        return 0;
    }

    mChHorse.mpBckAnm = new mDoExt_bckAnm();
    J3DAnmTransform* i_bck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, BCK_HU_LASH);
    if (mChHorse.mpBckAnm == NULL || mChHorse.mpBckAnm->init(i_bck, 1, 0, 1.0f, 0, -1, false) == 0) {
        return 0;
    }

    mChHorse.mAnmRate = 0.0f;
    mChHorse.field_0x78c = 0;

    if (mChHorse.mChReins.field_0x0.init(2, 7, (ResTIMG*)dComIfG_getObjectRes(l_arcName, TEX_TAZUNA), 1) == 0) {
        return 0;
    }

    if (mChHorse.mChReins.field_0x3c.init(1, 2, (ResTIMG*)dComIfG_getObjectRes(l_arcName, TEX_TAZUNA), 1) == 0) {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, BMDR_HARNESS);
    
    JUT_ASSERT(2762, modelData != NULL);

    mChHarness.mHarnessModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mChHarness.mHarnessModel == NULL) {
        return 0;
    }

    modelData = mChHarness.mHarnessModel->getModelData();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        if (i == 2 || i == 3) {
            modelData->getJointNodePointer(i)->setCallBack(jointFrontWheelCallBack);
        }
    }
    mChHarness.mHarnessModel->setUserArea((uintptr_t)this);

    mChHarness.field_0x6a0 = new dBgW();
    if (mChHarness.field_0x6a0 == NULL) {
        return 0;
    }

    if (mChHarness.field_0x6a0->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName, DZB_HARNESS), 1, &mChHarness.mMtx)) {
        mChHarness.field_0x6a0 = NULL;

        JUT_ASSERT(2782, FALSE);
    }
   
    modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, BMDR_COACH);
    
    JUT_ASSERT(2787, modelData != NULL);

    mChCoach.mCoachModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mChCoach.mCoachModel == NULL) {
        return 0;
    }

    modelData = mChCoach.mCoachModel->getModelData();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        if (i == 1 || i == 2) {
            modelData->getJointNodePointer(i)->setCallBack(jointRearWheelCallBack);
        } else if (i == 4) {
            modelData->getJointNodePointer(i)->setCallBack(jointCoachCallBack);
        }
    }
    mChCoach.mCoachModel->setUserArea((uintptr_t)this);

    mChCoach.field_0x564 = new dBgW();
    if (mChCoach.field_0x564 == NULL) {
        return 0;
    }

    if (mChCoach.field_0x564->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName, DZB_COACH), 1, &mChCoach.field_0x568) != 0) {
        mChCoach.field_0x564 = NULL;

        JUT_ASSERT(2811, FALSE);
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, BMDR_YELIA);
    
    JUT_ASSERT(2816, modelData != NULL);

    mChYelia.mpModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, BCK_YELIA_WAIT),
                                                -1, 1.0f, 0, -1, &mChYelia.mSound, 0x80000, 0x11000084);
    if (mChYelia.mpModelMorf != NULL && mChYelia.mpModelMorf->getModel() == NULL) {
        mChYelia.mpModelMorf->stopZelAnime();
        mChYelia.mpModelMorf = NULL;
    }

    if (mChYelia.mpModelMorf == NULL) {
        return 0;
    }

    return 1;
}

static int daNpcCoach_Execute(daNpcCoach_c* i_this) {
    return i_this->execute();
}

void daNpcCoach_c::setDriverMtx() {
    daNpcTheB_c* telmaB_p = (daNpcTheB_c*)fpcM_SearchByID(parentActorID);
    if (telmaB_p != NULL) {
        telmaB_p->setTRMtx(mChCoach.mCoachModel->getAnmMtx(JNT_COACH_TERUMA_LOC));
    }
}

void daNpcCoach_c::calcYeliaMotion() {
    J3DAnmTransform* i_bck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, BCK_YELIA_WAIT);
    J3DAnmTransform* i_bck2 = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, BCK_YELIA_BEND);

    if (mChYelia.mpModelMorf->getAnm() == i_bck) {
        if (dComIfGs_isSaveDunSwitch(0x37)) {
            mChYelia.mpModelMorf->setAnm(i_bck2, J3DFrameCtrl::EMode_LOOP, 15.0f, 1.0f, 0.0f, -1.0f);
        }
    } else {
        if (mChYelia.mpModelMorf->getAnm() == i_bck2 && !dComIfGs_isSaveDunSwitch(0x37)) {
            mChYelia.mpModelMorf->setAnm(i_bck, J3DFrameCtrl::EMode_LOOP, 20.0f, 1.0f, 0.0f, -1.0f);
        }
    }

    setYeliaMtx();

    mChYelia.mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mChYelia.mpModelMorf->modelCalc();
}

int daNpcCoach_c::execute() {
    calcHorseMotion();
    calcHarnessMotion();
    calcCoachMotion();
    calcDriverMotion();
    calcYeliaMotion();
    reinsExecute();

    dTres_c::setCoachPosition(1, &mChHarness.field_0x6e0, dComIfG_Bgsp().GetRoomId(mChCoach.mBgc.m_gnd));
    checkCoachDamage();

    cXyz work(0.0f, 0.0f, 0.0f);
    int iVar1 = 0;

    for (int i = 0; i < 5; i++) {
        if (field_0x247c[i] != fpcM_ERROR_PROCESS_ID_e) {
            fopAc_ac_c* actor_p = (fopAc_ac_c*)fpcM_SearchByID(field_0x247c[i]);
            if (actor_p != NULL) {
                work += fopAcM_GetPosition(actor_p);
                iVar1++;
            }
        }
    }

    if (mTotalDmgRecv > 0) {
        if (iVar1 == 0) {
            work = eyePos;
        } else {
            work /= iVar1;
        }

        work.y += attr().field_0x8c;

        GXColor color = {attr().field_0x86, attr().field_0x87, attr().field_0x88, 0xFF};

        f32 fVar1 = attr().field_0x90;
        f32 fVar2 = field_0x24c8;

        dKy_BossSpotLight_set(&work, -90.0f, 0.0f, attr().field_0x94, &color,
                              fVar2 * fVar1, 2, attr().field_0x89);

        if (field_0x24c8 < 1.0f) {
            field_0x24c8 += 0.0022222223f;
        } else {
            field_0x24c8 = 1.0f;
        }
    } else {
        field_0x24c8 = 0.0f;
    }

    return 1;
}

void daNpcCoach_c::checkCoachDamage() {
    if (field_0x24c0 > 0) {
        if (!dComIfGp_event_runCheck() && getRailID() != 8) {
            mTotalDmgRecv += attr().damage_reduction;

            if (mTotalDmgRecv > attr().damage_durability) {
                mTotalDmgRecv = attr().damage_durability;
            }
        }

        mChCoach.mSound.startFireSound(mTotalDmgRecv);
        mChYelia.mSound.startCreatureVoiceLevel(Z2SE_YELIA_V_KYAAA_LOOP, -1);
        dComIfGs_onSaveDunSwitch(0x37);
    } else {
        mTotalDmgRecv = 0;
        dComIfGs_offSaveDunSwitch(0x37);
    }

    u8 uVar1 = mTotalDmgRecv * 0xFF / attr().damage_durability;

    if (field_0x1dc0 < mTotalDmgRecv) {
        field_0x1dc0 = mTotalDmgRecv;

        if (field_0x1dc0 < attr().damage_durability / 2) {
            field_0x1dc5 = 0xFF - ((field_0x1dc0 * 0xFF) / attr().damage_durability << 1);
            field_0x1dc6 = 125;
        } else {
            field_0x1dc5 = 0;
            int iVar2 = attr().damage_durability / 2;
            int iVar1 = (field_0x1dc0 - attr().damage_durability / 2) * 20;
            field_0x1dc6 = (int)((f32)iVar1 / (f32)iVar2 + 125.0f);
        }

        field_0x1dc7 = 0xFF - uVar1;
    }

    setCoachBlazing(uVar1);
    ((daCoach2D_c*)fpcM_SearchByID(field_0x2554))->setHitCount((mTotalDmgRecv / (attr().damage_durability / 20)));

    if (mTotalDmgRecv >= attr().damage_durability) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
        mChHorse.field_0x784 = 0.0f;

        daNpcTheB_c* telmaB_p = (daNpcTheB_c*)fpcM_SearchByID(parentActorID);
        if (telmaB_p != NULL) {
            mChCoach.field_0x5f8[0]->setParticleCallBackPtr(NULL);
            mChCoach.field_0x5f8[1]->setParticleCallBackPtr(NULL);
            telmaB_p->setGameOver();
        }
    }

    daCoach2D_c* coach2D_p;
    if (field_0x2558 != 0) {
        if (dComIfGs_isSaveDunSwitch(0x28)) {
            coach2D_p = (daCoach2D_c*)fpcM_SearchByID(field_0x2554);
            if (coach2D_p != NULL) {
                coach2D_p->hide();
                field_0x2558 = 0;
            }
        }
    } else if (mTotalDmgRecv > 0 && !dComIfGs_isSaveDunSwitch(0x28)) {
        coach2D_p = (daCoach2D_c*)fpcM_SearchByID(field_0x2554);
        if (coach2D_p != NULL) {
            coach2D_p->setMaxHitCount(20);
            coach2D_p->show();
            field_0x2558 = 1;
        }
    }

    if (field_0x2559 == 0) {
        if (mTotalDmgRecv > attr().damage_durability / 4) {
            dMeter2Info_setFloatingMessage(0x13EB, 150, false);
            field_0x2559 = 1;
        }
    }
}

void daNpcCoach_c::reinsExecute() {
    daNpcChReins_c* reins = &mChHorse.mChReins;
    if (reins->field_0x16c <= 1) {
        if (++reins->field_0x16c > 1) {
            reinsInit();
        }
    } else {
        daNpcTheB_c* telmaB_p = (daNpcTheB_c*)fpcM_SearchByID(parentActorID);
        cXyz sp24, sp30, sp3c;
        cXyz sp48(attr().field_0x4c);
        mDoMtx_stack_c::copy(mChHorse.mpModelMorf->getModel()->getAnmMtx(attr().field_0x60));
        mDoMtx_stack_c::multVec(&sp48, &sp48);
        reins->field_0x170 = sp48;
        reins->field_0x168 = attr().field_0x48;

        for (int i = 0; i < 2; i++) {
            if (i == 0) {
                sp24.set(63.0f, 17.0f, 11.0f);
            } else {
                sp24.set(63.0f, 17.0f, -11.0f);
            }

            mDoMtx_stack_c::copy(mChHorse.mpModelMorf->getModel()->getAnmMtx(JNT_HORSE_HEAD));
            mDoMtx_stack_c::multVec(&sp24, &sp24);

            if (telmaB_p != NULL) {
                sp30 = telmaB_p->getHandPos2(i);
            }

            cXyz* sp_0x34 = reins->field_0x0.getPos(i);
            cXyz* sp_0x30 = &reins->field_0x78[i][0];
            cXyz* sp_0x2c = &reins->field_0xf0[i][0];
            cXyz sp54;
            *sp_0x34 = sp24;
            ++sp_0x34;

            for (int j = 1; j < 6; j++, sp_0x34++, sp_0x30++, sp_0x2c++) {
                sp3c = *sp_0x34 - sp48;
                sp3c.normalizeZP();
                sp3c *= attr().field_0x5c;
                sp3c.y = 0.0f;
                sp54 = *sp_0x34 - *(sp_0x34 - 1);
                sp54.y += gravity;
                sp54 += *sp_0x2c + sp3c;
                sp54.normalizeZP();
                *sp_0x34 = *(sp_0x34 - 1) + (sp54 * reins->field_0x168);
            }

            *sp_0x34 = sp30;
            sp_0x34 = reins->field_0x0.getPos(i) + 5;
            sp_0x2c = &reins->field_0xf0[i][4];

            for (int j = 5; j > 0; j--, sp_0x34--, sp_0x2c--) {
                sp3c = *sp_0x34 - sp48;
                sp3c.normalizeZP();
                sp3c *= attr().field_0x5c;
                sp3c.y = 0.0f;
                sp54 = *sp_0x34 - *(sp_0x34 + 1);
                sp54.y += gravity;
                sp54 += *sp_0x2c + sp3c;
                sp54.normalizeZP();
                *sp_0x34 = *(sp_0x34 + 1) + (sp54 * reins->field_0x168);
            }

            sp_0x34 = reins->field_0x0.getPos(i);
            sp_0x30 = &reins->field_0x78[i][0];
            sp_0x2c = &reins->field_0xf0[i][0];

            for (int j = 1; j < 6; j++, sp_0x34++, sp_0x30++, sp_0x2c++) {
                *sp_0x2c = (*sp_0x34 - *sp_0x30) * attr().field_0x58;
                *sp_0x30 = *sp_0x34;
            }
        }

        cXyz* sp_0x18 = reins->field_0x3c.getPos(0);

        for (int i = 0; i < 2; i++, sp_0x18++) {
            if (telmaB_p != NULL) {
                *sp_0x18 = telmaB_p->getHandPos1(i);
            }
        }
    }
}

void daNpcCoach_c::reinsInit() {
    daNpcChReins_c* reins = &mChHorse.mChReins;
    daNpcTheB_c* telmaB_p = (daNpcTheB_c*)fpcM_SearchByID(parentActorID);
    cXyz sp1c, sp28;

    for (int i = 0; i < 2; i++) {
        int j;
        f32* pfVar1 = reins->field_0x0.getSize(i);

        for (j = 0; j < 7; j++, pfVar1++) {
            *pfVar1 = 2.0f;
        }

        if (i == 0) {
            sp1c.set(63.0f, 17.0f, 11.0f);
        } else {
            sp1c.set(63.0f, 17.0f, -11.0f);
        }

        mDoMtx_stack_c::copy(mChHorse.mpModelMorf->getModel()->getAnmMtx(JNT_HORSE_HEAD));
        mDoMtx_stack_c::multVec(&sp1c, &sp1c);

        if (telmaB_p != NULL) {
            sp28 = telmaB_p->getHandPos2(i);
        }

        cXyz sp34 = (sp28 - sp1c) / 6.0f;
        reins->field_0x168 = sp34.abs();
        cXyz* sp_0x24 = reins->field_0x0.getPos(i);
        cXyz* sp_0x20 = &reins->field_0x78[i][0];
        *sp_0x24 = sp1c;

        ++sp_0x24;
        for (j = 1; j < 6; j++, ++sp_0x24, ++sp_0x20) {
            *sp_0x24 = *(sp_0x24 - 1) + sp34;
            *sp_0x20 = *sp_0x24;
        }
        *sp_0x24 = sp28;
    }

    cXyz* pcVar4 = reins->field_0x3c.getPos(0);
    f32* pfVar2 = reins->field_0x3c.getSize(0);

    for (int i = 0; i < 2; i++, pcVar4++, pfVar2++) {
        if (telmaB_p != NULL) {
            *pcVar4 = telmaB_p->getHandPos1(i);
        }

        *pfVar2 = 2.0f;
    }
}

void daNpcCoach_c::setCoachCollision() {
    mChCoach.mBgc.CrrPos(dComIfG_Bgsp());

    if (mChCoach.field_0x5e0.y - mChCoach.mBgc.GetGroundH() > 1500.0f) {
        mChCoach.field_0x784 = 0.0f;
    } else {
        mChCoach.field_0x784 = attr().max_fall_speed;
    }

    mChCoach.field_0x564->SetRoomId(dComIfG_Bgsp().GetRoomId(mChCoach.mBgc.m_gnd));

    if (attention_info.flags != 0 && mChCoach.field_0x77c == 0) {
        mChCoach.field_0x644.SetC(eyePos);
        dComIfG_Ccsp()->Set(&mChCoach.field_0x644);
    } else {
        mChCoach.field_0x644.ClrTgHit();
    }

    f32 fVar1 = cM_ssin(mChCoach.field_0x79a.y);
    f32 fVar2 = cM_scos(mChCoach.field_0x79a.y);
    cXyz sp58(35.0f, 0.0f, 15.0f);
    mDoMtx_stack_c::transS(mChYelia.field_0x694);
    mDoMtx_stack_c::YrotM(mChCoach.field_0x79a.y);
    mDoMtx_stack_c::multVec(&sp58, &sp58);
    fVar1 *= 65.0f;
    fVar2 *= 65.0f;

    for (int i = 0; i < 2; i++) {
        mChYelia.field_0x41c[i].SetC(sp58);
        dComIfG_Ccsp()->Set(&mChYelia.field_0x41c[i]);
        sp58.x -= fVar1;
        sp58.z -= fVar2;
    }
}

void daNpcCoach_c::calcSpringF(f32* param_1, f32 param_2, f32* param_3) {
    *param_3 = (*param_3 + (param_2 - *param_1) * attr().spring_constant) * attr().damp_coeff[0];
    *param_1 += *param_3;
}

void daNpcCoach_c::calcSpringS(s16* param_1, s16 param_2, f32* param_3) {
    *param_3 = (*param_3 + (param_2 - *param_1) * attr().spring_constant) * attr().damp_coeff[0];
    *param_1 += *param_3;
}

void daNpcCoach_c::setCoachMtx() {
    cXyz sp18;
    mDoMtx_stack_c::copy(mChHarness.mHarnessModel->getAnmMtx(JNT_HARNESS_BS_LOC));
    mDoMtx_stack_c::multVecZero(&sp18);
    mDoMtx_stack_c::transS(sp18);
    mDoMtx_stack_c::YrotM(mChCoach.field_0x79a.y);
    mDoMtx_stack_c::XrotM(mChCoach.field_0x79a.x);
    mDoMtx_stack_c::ZrotM(mChCoach.field_0x79a.z);

    mChCoach.mCoachModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_stack_c::transM(0.0f, mChCoach.mCoachTrans, 0.0f);
    mDoMtx_stack_c::ZrotM(mChCoach.mCoachRot);
    MTXCopy(mDoMtx_stack_c::get(), mChCoach.field_0x568);

    mChCoach.field_0x564->Move();
    mChCoach.mCoachModel->calc();
    MTXCopy(mDoMtx_stack_c::get(), mCoachMtx);

    eyePos.set(108.0f, 108.0f, 0.0f);
    mDoMtx_stack_c::multVec(&eyePos, &eyePos);

    attention_info.position.set(-90.0f, 290.0f, 0.0f);
    mDoMtx_stack_c::multVec(&attention_info.position, &attention_info.position);

    mDoMtx_stack_c::inverse();
    MTXCopy(mDoMtx_stack_c::get(), mChCoach.field_0x598);
}

void daNpcCoach_c::setWheelSmoke(daNpcChWheel_c* i_wheel, dBgS_ObjAcch* i_bgc, dKy_tevstr_c* i_tevStr, csXyz* i_angle) {
    f32 fVar1 = speedF / attr().max_speed;
    f32 reg_f29 = fVar1;
    f32 fVar2 = fVar1 * 2.0f;
    s16 sVar1 = fVar1 * 15.0f;
    u8 sVar2 = fVar1 * 128.0f;

    if (sVar1 == 0 || sVar2 == 0 || fVar2 == 0.0f || reg_f29 == 0.0f) {
        return;
    }

    if (i_bgc->ChkGroundHit()) {
        fopAcM_effSmokeSet2(i_wheel->field_0xc, i_wheel->field_0xc + 1, &i_wheel->field_0x0,
                            i_angle, 1.0f, i_tevStr);

        for (int i = 0; i < 2; i++) {
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_wheel->field_0xc[i]);
            if (emitter != NULL) {
                emitter->setRate(reg_f29);
                emitter->setLifeTime((s16) sVar1);
                emitter->setGlobalAlpha((u8) sVar2);
                emitter->setGlobalParticleScale(fVar2, fVar2);
            }
        }
    }
}

void daNpcCoach_c::calcRearWheelRotate() {
    if (speedF != 0.0f) {
        f32 fVar1 = mChCoach.field_0x5ec.absXZ(mChCoach.field_0x5e0);
        int reg_r25 = cLib_distanceAngleS(mChCoach.field_0x79a.y, cLib_targetAngleY(&mChCoach.field_0x5ec, &mChCoach.field_0x5e0));
        f32 fVar2 = (reg_r25 < 0x4000) ? fVar1 / 439.823f : -(fVar1 / 439.823f);

        s16 reg_r27 = fVar2 * 65535.0f;
        int ang_diff = mChHarness.field_0x706.y - mChHarness.field_0x700.y;
        f32 reg_f29 = ang_diff / 65535.0f;
        s16 reg_r26 = reg_f29 * 642.61536f / 439.823f * 65535.0f;
        s16 reg_r29 = reg_r26 + reg_r27;
        s16 reg_r28 = reg_r26 - reg_r27;

        if (reg_r29 > 4500) {
            reg_r29 = 4500;
        } else if (reg_r29 < -4500) {
            reg_r29 = -4500;
        }

        if (reg_r28 > 4500) {
            reg_r28 = 4500;
        } else if (reg_r28 < -4500) {
            reg_r28 = -4500;
        }

        mChCoach.field_0x7a8.mWheelRot.x += reg_r29;
        mChCoach.field_0x7cc.mWheelRot.x += reg_r28;

        if (cLib_chaseS(&mChCoach.field_0x7a8.field_0x1c, mChCoach.field_0x7a8.field_0x1a, 16) != 0) {
            mChCoach.field_0x7a8.field_0x1a = cM_rndF(attr().field_0x66);
        }

        if (cLib_chaseS(&mChCoach.field_0x7cc.field_0x1c, mChCoach.field_0x7cc.field_0x1a, 16) != 0) {
            mChCoach.field_0x7cc.field_0x1a = cM_rndF(attr().field_0x66);
        }

        if (cLib_chaseS(&mChCoach.field_0x7a8.field_0x20, mChCoach.field_0x7a8.field_0x1e, 16) != 0) {
            mChCoach.field_0x7a8.field_0x1e = cM_rndF(attr().field_0x68);
        }

        if (cLib_chaseS(&mChCoach.field_0x7cc.field_0x20, mChCoach.field_0x7cc.field_0x1e, 16) != 0) {
            mChCoach.field_0x7cc.field_0x1e = cM_rndF(attr().field_0x68);
        }

        mChCoach.field_0x7a8.mWheelRot.y = mChCoach.field_0x7a8.field_0x1c * cM_ssin(mChCoach.field_0x7a8.mWheelRot.x);
        mChCoach.field_0x7cc.mWheelRot.y = mChCoach.field_0x7cc.field_0x1c * cM_ssin(mChCoach.field_0x7cc.mWheelRot.x);
        mChCoach.field_0x7a8.mWheelRot.z = mChCoach.field_0x7a8.field_0x20 * cM_ssin(mChCoach.field_0x7a8.mWheelRot.x);
        mChCoach.field_0x7cc.mWheelRot.z = mChCoach.field_0x7cc.field_0x20 * cM_ssin(mChCoach.field_0x7cc.mWheelRot.x);

        setWheelSmoke(&mChCoach.field_0x7a8, &mChCoach.mBgc, &mChCoach.mTevStr, &mChCoach.field_0x79a);
        setWheelSmoke(&mChCoach.field_0x7cc, &mChCoach.mBgc, &mChCoach.mTevStr, &mChCoach.field_0x79a);
    }
}

s16 daNpcCoach_c::getGroundSlope(dBgS_ObjAcch* i_bgc, s16 param_2) {
    cM3dGPla plane;

    if (dComIfG_Bgsp().GetTriPla(i_bgc->m_gnd, &plane)) {
        return fopAcM_getPolygonAngle(&plane, param_2);
    }

    return shape_angle.x;
}

void daNpcCoach_c::calcCoachMotion() {
    cXyz sp44 = mChCoach.field_0x5e0 - mChCoach.field_0x5ec;
    mChCoach.field_0x5ec = mChCoach.field_0x5e0;
    mChCoach.field_0x7a0 = mChCoach.field_0x79a;
    mChCoach.field_0x5c8.y += gravity;

    if (mChCoach.field_0x5c8.y < mChCoach.field_0x784) {
        mChCoach.field_0x5c8.y = mChCoach.field_0x784;
    }

    mChCoach.field_0x5e0.y += mChCoach.field_0x5c8.y;
    mChCoach.field_0x5c8.x = sp44.x;
    mChCoach.field_0x5c8.z = sp44.z;

    setCoachCollision();

    if (mChCoach.mBgc.ChkGroundLanding()) {
        if (mChCoach.field_0x5c8.y < -20.0f) {
            mChCoach.mSound.startSound(Z2SE_COACH_SHAKE, 0, -1);
        }

        if (cM_rnd() < 0.5f) {
            mChCoach.field_0x794 += attr().horiz_sway_range;
        } else {
            mChCoach.field_0x794 -= attr().horiz_sway_range;
        }

        mChCoach.field_0x790 += attr().vert_swing_width;
        mChCoach.field_0x5c8.y *= attr().jump_coeff;
    } else if (mChCoach.mBgc.ChkGroundHit()) {
        mChCoach.field_0x5c8.y = 0.0f;
    }

    cXyz sp50;
    mDoMtx_stack_c::copy(mChHarness.mHarnessModel->getAnmMtx(JNT_HARNESS_BS_LOC));
    mDoMtx_stack_c::multVecZero(&sp50);
    sp50.y = mChHarness.field_0x6e0.y;
    mChCoach.field_0x79a.x = cLib_targetAngleX(&sp50, &mChCoach.field_0x5e0);
    mChCoach.field_0x79a.y = cLib_targetAngleY(&mChCoach.field_0x5e0, &sp50);
    cLib_addCalcAngleS(&mChCoach.field_0x79a.z, getGroundSlope(&mChCoach.mBgc, mChCoach.field_0x79a.y - 0x4000),
                       15, 0x200, 0x40);

    if (speedF > 0.0f) {
        mDoMtx_stack_c::transS(sp50);
        mDoMtx_stack_c::YrotM(mChCoach.field_0x79a.y);
        mDoMtx_stack_c::XrotM(mChCoach.field_0x79a.x);
        mDoMtx_stack_c::ZrotM(mChCoach.field_0x79a.z);

        sp50.set(0.0f, 0.0f, attr().field_0x10);
        mDoMtx_stack_c::multVec(&sp50, &sp50);
        mChCoach.field_0x5e0.x = sp50.x;
        mChCoach.field_0x5e0.z = sp50.z;

        sp50.set(attr().field_0x18, 0.0f, attr().field_0x10);
        mDoMtx_stack_c::multVec(&sp50, &mChCoach.field_0x7a8.field_0x0);

        sp50.set(-attr().field_0x18, 0.0f, attr().field_0x10);
        mDoMtx_stack_c::multVec(&sp50, &mChCoach.field_0x7cc.field_0x0);
    }

    mChCoach.field_0x788 -= mChCoach.field_0x5c8.absXZ();
    if (mChCoach.field_0x788 <= 0.0f) {
        if (cM_rnd() < 0.5f) {
            mChCoach.field_0x794 += attr().horiz_sway_range;
        } else {
            mChCoach.field_0x794 -= attr().horiz_sway_range;
        }

        if (cM_rnd() < 0.5f) {
            mChCoach.field_0x790 += attr().vert_swing_width;
        } else {
            mChCoach.field_0x790 -= attr().vert_swing_width;
        }

        mChCoach.field_0x788 = attr().shake_dist + cM_rndF(attr().shake_dist);
    }

    calcSpringF(&mChCoach.mCoachTrans, 0.0f, &mChCoach.field_0x790);
    calcSpringS(&mChCoach.mCoachRot, 0, &mChCoach.field_0x794);
    setCoachMtx();
    mChCoach.mSound.framework(0, 0);
    calcRearWheelRotate();

    if (attention_info.flags != 0) {
        if (mChCoach.field_0x77c > 0) {
            mChCoach.field_0x77c--;
        }

        if (mChCoach.field_0x644.ChkTgHit() && mChCoach.field_0x77c == 0) {
            mChCoach.mSound.startSound(Z2SE_COACH_HIT_WIND, 0, -1);

            mTotalDmgRecv = (mTotalDmgRecv / (attr().damage_durability / 20) - attr().damage_extinguish_level) * (attr().damage_durability / 20);
            if (mTotalDmgRecv <= 0) {
                mTotalDmgRecv = 0;
                field_0x24c0 = 0;

                for (int i = 0; i < 5; i++) {
                    if (field_0x247c[i] != fpcM_ERROR_PROCESS_ID_e) {
                        fopAcM_delete(field_0x247c[i]);
                        field_0x247c[i] = fpcM_ERROR_PROCESS_ID_e;
                    }
                }
            } else {
                mChCoach.field_0x77c = 20;

                for (int i = 0; i < 5; i++) {
                    if (field_0x247c[i] != fpcM_ERROR_PROCESS_ID_e) {
                        ((daCoachFire_c*)fpcM_SearchByID(field_0x247c[i]))->setNoHitTimer();
                    }
                }
            }
        }
    }
}

void daNpcCoach_c::setHarnessCollision() {
    cXyz sp28(0.0f, 0.0f, 160.0f);
    mDoMtx_stack_c::transS(mChHarness.field_0x6e0);
    mDoMtx_stack_c::YrotM(mChHarness.field_0x700.y);
    mDoMtx_stack_c::multVec(&sp28, &sp28);
    mChHarness.field_0x564.SetC(sp28);
    dComIfG_Ccsp()->Set(&mChHarness.field_0x564);
    mChHarness.mBgc.CrrPos(dComIfG_Bgsp());

    if (mChHarness.field_0x6e0.y - mChHarness.mBgc.GetGroundH() > 1500.0f) {
        mChHarness.field_0x6fc = 0.0f;
    } else {
        mChHarness.field_0x6fc = attr().max_fall_speed;
    }

    mChHarness.field_0x6a0->SetRoomId(dComIfG_Bgsp().GetRoomId(mChHarness.mBgc.m_gnd));
}

void daNpcCoach_c::calcHarnessMotion() {
    cXyz sp1c = mChHarness.field_0x6e0 - mChHarness.field_0x6ec;
    mChHarness.field_0x6ec = mChHarness.field_0x6e0;
    mChHarness.field_0x706 = mChHarness.field_0x700;
    mChHarness.field_0x6d4.y += gravity;

    if (mChHarness.field_0x6d4.y < mChHarness.field_0x6fc) {
        mChHarness.field_0x6d4.y = mChHarness.field_0x6fc;
    }

    mChHarness.field_0x6e0.y += mChHarness.field_0x6d4.y;
    mChHarness.field_0x6d4.x = sp1c.x;
    mChHarness.field_0x6d4.z = sp1c.z;

    setHarnessCollision();

    if (mChHarness.mBgc.ChkGroundLanding()) {
        mChHarness.field_0x6d4.y *= attr().jump_coeff;
    } else if (mChHarness.mBgc.ChkGroundHit()) {
        mChHarness.field_0x6d4.y = 0.0f;
    }

    mChHarness.field_0x700.x = cLib_targetAngleX(&current.pos, &mChHarness.field_0x6e0);
    mChHarness.field_0x700.y = cLib_targetAngleY(&mChHarness.field_0x6e0, &current.pos);
    cLib_addCalcAngleS(&mChHarness.field_0x700.z, getGroundSlope(&mChHarness.mBgc, mChHarness.field_0x700.y - 0x4000),
                       15, 0x200, 0x40);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(mChHarness.field_0x700.y);
    mDoMtx_stack_c::XrotM(mChHarness.field_0x700.x);
    mDoMtx_stack_c::ZrotM(mChHarness.field_0x700.z);

    if (speedF > 0.0f) {
        cXyz sp28(0.0f, 0.0f, attr().field_0xc);
        mDoMtx_stack_c::multVec(&sp28, &sp28);
        mChHarness.field_0x6e0.x = sp28.x;
        mChHarness.field_0x6e0.z = sp28.z;

        sp28.set(attr().field_0x14, 0.0f, attr().field_0xc);
        mDoMtx_stack_c::multVec(&sp28, &mChHarness.field_0x70c.field_0x0);

        sp28.set(-attr().field_0x14, 0.0f, attr().field_0xc);
        mDoMtx_stack_c::multVec(&sp28, &mChHarness.field_0x730.field_0x0);
    }

    mChHarness.mHarnessModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mChHarness.mMtx);
    mChHarness.field_0x6a0->Move();
    mChHarness.mHarnessModel->calc();
    calcFrontWheelRotate();
}

void daNpcCoach_c::calcFrontWheelRotate() {
    if (speedF != 0.0f) {
        f32 fVar1 = mChHarness.field_0x6ec.absXZ(mChHarness.field_0x6e0);
        int dist_ang = cLib_distanceAngleS(mChHarness.field_0x700.y, 
                                           cLib_targetAngleY(&mChHarness.field_0x6ec, &mChHarness.field_0x6e0));
        f32 fVar2 = (dist_ang < 0x4000) ? fVar1 / 345.5751953125f : -(fVar1 / 345.5751953125f);

        s16 iVar1 = fVar2 * 65535.0f;
        int angle_diff = mChHarness.field_0x706.y - mChHarness.field_0x700.y;
        f32 reg_f29 = angle_diff / 65535.0f;
        s16 sVar1 = reg_f29 * 556.8225f / 345.5752f * 65535.0f;
        s16 sVar2 = sVar1 + iVar1;
        s16 my_diff = sVar1 - iVar1;

        if (sVar2 > 4500) {
            sVar2 = 4500;
        } else if (sVar2 < -4500) {
            sVar2 = -4500;
        }

        if (my_diff > 4500) {
            my_diff = 4500;
        } else if (my_diff < -4500) {
            my_diff = -4500;
        }

        mChHarness.field_0x70c.mWheelRot.x += sVar2;
        mChHarness.field_0x730.mWheelRot.x += my_diff;

        if (cLib_chaseS(&mChHarness.field_0x70c.field_0x1c, mChHarness.field_0x70c.field_0x1a, 16) != 0) {
            mChHarness.field_0x70c.field_0x1a = cM_rndF(attr().field_0x66);
        }
        if (cLib_chaseS(&mChHarness.field_0x730.field_0x1c, mChHarness.field_0x730.field_0x1a, 16) != 0) {
            mChHarness.field_0x730.field_0x1a = cM_rndF(attr().field_0x66);
        }

        if (cLib_chaseS(&mChHarness.field_0x70c.field_0x20, mChHarness.field_0x70c.field_0x1e, 16) != 0) {
            mChHarness.field_0x70c.field_0x1e = cM_rndF(attr().field_0x68);
        }
        if (cLib_chaseS(&mChHarness.field_0x730.field_0x20, mChHarness.field_0x730.field_0x1e, 16) != 0) {
            mChHarness.field_0x730.field_0x1e = cM_rndF(attr().field_0x68);
        }

        mChHarness.field_0x70c.mWheelRot.y = mChHarness.field_0x70c.field_0x1c * cM_ssin(mChHarness.field_0x70c.mWheelRot.x);
        mChHarness.field_0x730.mWheelRot.y = mChHarness.field_0x730.field_0x1c * cM_ssin(mChHarness.field_0x730.mWheelRot.x);
        mChHarness.field_0x70c.mWheelRot.z = mChHarness.field_0x70c.field_0x20 * cM_ssin(mChHarness.field_0x70c.mWheelRot.x);
        mChHarness.field_0x730.mWheelRot.z = mChHarness.field_0x730.field_0x20 * cM_ssin(mChHarness.field_0x730.mWheelRot.x);

        setWheelSmoke(&mChHarness.field_0x70c, &mChHarness.mBgc, &mChHarness.mTevStr, &mChHarness.field_0x700);
        setWheelSmoke(&mChHarness.field_0x730, &mChHarness.mBgc, &mChHarness.mTevStr, &mChHarness.field_0x700);
    }
}

f32 daNpcCoach_c::calcMaxSpeed() {
    if (!mChPath.isPath()) {
        return mChHorse.mAimSpeed;
    }

    if (field_0x2550 > 0) {
        if (--field_0x2550 >= 0) {
            return attr().deceleration_speed;
        }
    }

    cXyz sp38;
    mChPath.getTargetPoint(sp38);
    int dist_ang = cLib_distanceAngleS(current.angle.y, cLib_targetAngleY(&current.pos, &sp38));
    f32 fVar1 = -dist_ang / 16384.0f + 1.0f;
    if (fVar1 < 0.0f) {
        fVar1 = 0.0f;
    }

    return fVar1 * mChHorse.mAimSpeed;
}

void daNpcCoach_c::setHorseCollision() {
    f32 fVar1 = cM_ssin(shape_angle.y);
    f32 fVar2 = cM_scos(shape_angle.y);
    cXyz sp58(current.pos.x + (fVar1 * 135.0f), current.pos.y, current.pos.z + (fVar2 * 135.0f));
    fVar1 *= 83.0f;
    fVar2 *= 83.0f;

    for (int i = 0; i < 4; i++) {
        mChHorse.field_0x1f0[i].SetC(sp58);
        dComIfG_Ccsp()->Set(&mChHorse.field_0x1f0[i]);
        sp58.x -= fVar1;
        sp58.z -= fVar2;
    }

    mChHorse.mBgc.CrrPos(dComIfG_Bgsp());

    if (current.pos.y - mChHorse.mBgc.GetGroundH() > 1500.0f) {
        maxFallSpeed = 0.0f;
    } else {
        maxFallSpeed = attr().max_fall_speed;
    }
}

void daNpcCoach_c::setHorseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mChHorse.mpModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_stack_c::copy(mChHorse.mpModelMorf->getModel()->getAnmMtx(JNT_HORSE_MOUTH2));
    mDoMtx_stack_c::multVecZero(&mChHorse.field_0x770);
}

void daNpcCoach_c::calcHorseMotion() {
    f32 fVar1 = calcMaxSpeed();
    if (mChHorse.field_0x784 < fVar1) {
        cLib_addCalc(&mChHorse.field_0x784, fVar1, 0.07f, 0.2f, 0.01f);
    } else {
        cLib_addCalc(&mChHorse.field_0x784, fVar1, 0.1f, 0.8f, 0.01f);
    }

    if (maxFallSpeed == 0.0f) {
        mChHorse.field_0x784 = 0.0f;
    } else {
        mChCoach.mSound.startWheelSound(mChHorse.field_0x784);
    }

    shape_angle.y = current.angle.y;
    s16 sVar1 = shape_angle.y - mChHarness.field_0x700.y;
    if (abs(sVar1) > attr().horse_rot_limit) {
        if (sVar1 > 0) {
            shape_angle.y = mChHarness.field_0x700.y + attr().horse_rot_limit;
        } else {
            shape_angle.y = mChHarness.field_0x700.y - attr().horse_rot_limit;
        }
    }

    cLib_addCalcAngleS(&shape_angle.x, getGroundSlope(&mChHorse.mBgc, shape_angle.y), 15, 0x200, 0x40);
    speedF = mChHorse.field_0x784 * cM_scos(shape_angle.x);
    calcHorsePath();
    fopAcM_posMoveF(this, NULL);
    setHorseCollision();
    setHorseMtx();
    calcHorseAnm();
}

void daNpcCoach_c::setHorseAnm(int i_index) {
    if (mChHorse.field_0x788 != i_index) {
        mChHorse.field_0x788 = i_index;

        J3DAnmTransform* anm = (J3DAnmTransform*)(l_horseAnmParam[i_index].field_0x0 >= 0 ? 
                                 dComIfG_getObjectRes(l_arcName, l_horseAnmParam[i_index].field_0x0) : NULL);
        J3DAnmTransform* anm_2 = l_horseAnmParam[i_index].field_0xc >= 0 ? 
                                 (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, l_horseAnmParam[i_index].field_0xc) : NULL;

        f32 frame = mChHorse.mpModelMorf->getFrame();
        f32 playSpeed = mChHorse.mpModelMorf->getPlaySpeed();
        mChHorse.mpModelMorf->setAnm(anm, anm_2, 0.0f, 2, 8.0f, 1.0f, 0.0f, -1.0f);
        mChHorse.mpModelMorf->setFrame(frame + playSpeed);
    }
}

void daNpcCoach_c::eyeWink() {
    mChHorse.field_0x790--;

    if (mChHorse.field_0x790 < 0) {
        mChHorse.mpBtpAnm->init(mChHorse.mpModelMorf->getModel()->getModelData(), (J3DAnmTexPattern*)dComIfG_getObjectRes(l_arcName, BTP_HU_EYE),
                                1, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1);
        mChHorse.field_0x790 = cM_rnd() * 90.0f + 15.0f;
    }
}

void daNpcCoach_c::calcHorseAnm() {
    bool reg_r29 = 0;
    if (mChPath.isPath()) {
        cXyz target_point;
        mChPath.getTargetPoint(target_point);
        int reg_r28 = cLib_distanceAngleS(shape_angle.y, mChHarness.field_0x700.y);
        int reg_r27 = cLib_distanceAngleS(current.angle.y, cLib_targetAngleY(&current.pos, &target_point));
        int reg_r26 = 0;
        if (reg_r28 < attr().horse_rot_limit && reg_r27 > 0x300) {
            reg_r26 = 1;
            reg_r29 = reg_r26;
        }
    }

    f32 fVar1 = 0.0f;
    f32 fVar2 = attr().max_speed;

    switch (mChHorse.field_0x788) {
        case 0:
            fVar2 = attr().field_0x28[mChHorse.field_0x788];

            if (speedF > fVar2) {
                setHorseAnm(1);
            }
            break;

        case 1:
            fVar1 = attr().field_0x28[mChHorse.field_0x788 - 1];
            fVar2 = attr().field_0x28[mChHorse.field_0x788];

            if (speedF <= fVar1) {
                setHorseAnm(0);
            } else if (speedF > fVar2) {
                setHorseAnm(2);
            }
            break;

        case 2:
            fVar1 = attr().field_0x28[mChHorse.field_0x788 - 1];
            fVar2 = attr().field_0x28[mChHorse.field_0x788];

            if (speedF <= fVar1) {
                setHorseAnm(1);
            } else if (speedF > fVar2) {
                setHorseAnm(3);
            }
            break;

        case 3:
            fVar1 = attr().field_0x28[mChHorse.field_0x788 - 1];
            fVar2 = attr().field_0x28[mChHorse.field_0x788];

            if (speedF <= fVar1) {
                setHorseAnm(2);
            } else if (speedF > fVar2) {
                setHorseAnm(4);
            }
            break;

        case 4:
            fVar1 = attr().field_0x28[mChHorse.field_0x788 - 1];

            if (speedF <= fVar1) {
                setHorseAnm(3);
            }
            break;

        default:
            JUT_ASSERT(1521, FALSE);
            break;
    }

    if (mChHorse.field_0x788 < 4) {
        f32 reg_f29 = speedF / fVar2;
        if (reg_f29 > 1.0f) {
            reg_f29 = 1.0f;
        }

        mChHorse.mpModelMorf->setAnmRate(reg_f29);
        f32 reg_f28 = reg_f29 * (l_horseAnmParam[mChHorse.field_0x788].field_0x14
                                 - l_horseAnmParam[mChHorse.field_0x788].field_0x8);
        mChHorse.mpModelMorf->setPlaySpeed(reg_f28 + l_horseAnmParam[mChHorse.field_0x788].field_0x8);
    } else {
        mChHorse.mpModelMorf->setAnmRate(0.0f);
        mChHorse.mpModelMorf->setPlaySpeed(l_horseAnmParam[mChHorse.field_0x788].field_0x8 * (speedF / fVar1));
    }

    mChHorse.mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mChHorse.mpBtpAnm->play();

    if (mChHorse.field_0x78c != 0) {
        mChHorse.mpBckAnm->play();

        if (mChHorse.mpBckAnm->isStop()) {
            mChHorse.field_0x78c = 0;
        } else {
            mChHorse.mpBckAnm->getBckAnm()->setFrame(mChHorse.mpBckAnm->getFrame());
        }
    }

    mChHorse.mpModelMorf->modelCalc();
    eyeWink();
}

s8 daNpcChPath_c::checkNearAttackPoint() {
    int var_r29 = 5;
    int var_r31 = mPntIndex - 2;

    if (var_r31 < 0) {
        if (isClose()) {
            var_r31 = mpPath->m_num + var_r31;
        } else {
            var_r29 += var_r31;
            var_r31 = 0;
        }
    }

    for (; var_r29 > 0; --var_r29) {
        if (mpPath->m_points[var_r31].mArg2 == 1) {
            return mpPath->m_points[var_r31].mArg1;
        }

        ++var_r31;
        if (var_r31 >= mpPath->m_num) {
            if (isClose()) {
                var_r31 = 0;
                continue;
            }

            break;
        }
    }

    return -1;
}

bool daNpcCoach_c::checkKargoAttack() {
    e_kr_class* kargo_p = (e_kr_class*)fpcM_Search(s_sub, this);
    if (kargo_p != NULL && kargo_p->checkBombDrop() != 0) {
        return true;
    }

    return false;
}

BOOL daNpcCoach_c::setExpressionAnm(int param_1, bool param_2) {
    if (!param_2 && mChHorse.field_0x78c != 0) {
        return FALSE;
    }

    int bckIdx, i_attr;
    if ((param_1 == 2 || param_1 > 1) || param_1 < 1) {
        JUT_ASSERT(2635, FALSE);
    } else {
        bckIdx = BCK_HU_LASH;
        i_attr = J3DFrameCtrl::EMode_NONE;
    }

    if (mChHorse.mpBckAnm->init((J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, bckIdx), 1, i_attr, 1.0f, 0, -1, true)) {
        mChHorse.field_0x78c = param_1;
        return TRUE;
    }

    return FALSE;
}

bool daNpcChPath_c::checkPoint(cXyz* param_1, f32 param_2) {
    f32 fVar1 = param_1->absXZ(dPath_GetPnt(mpPath, mPntIndex)->m_position);

    if (field_0x4 != NULL) {
        field_0x8 -= param_2;
    }

    if ((fVar1 < param_2 || fVar1 < 10.0f) || field_0x8 <= 0.0f) {
        return TRUE;
    }

    return FALSE;
}

bool daNpcChPath_c::setNextTarget() {
    bool rv = setNextPoint();
    if (rv && field_0x4 != NULL) {
        cXyz target_point;
        getTargetPoint(target_point);
        field_0x8 = field_0x4->abs(target_point);
    }

    return rv;
}

void daNpcCoach_c::resetOverAngle() {
    if (shape_angle.y != current.angle.y && mChPath.isPath()) {
        cXyz target_point;
        mChPath.getTargetPoint(target_point);
        s16 sVar1 = cLib_targetAngleY(&current.pos, &target_point);
        s16 sVar2 = current.angle.y - shape_angle.y;
        s16 sVar3 = sVar1 - shape_angle.y;

        if (sVar2 > 0 && sVar3 <= 0) {
            current.angle.y = shape_angle.y;
        } else if (sVar2 < 0 && sVar3 >= 0) {
            current.angle.y = shape_angle.y;
        }
    }
}

void daNpcCoach_c::calcHorsePath() {
    if (!mChPath.isPath() || mTotalDmgRecv >= attr().damage_durability) {
        return;
    }

    s8 arg0 = mChPath.getArg0();
    if (arg0 >= 0) {
        mChHorse.mAimSpeed = arg0;
    }

    if (mChHorse.mAimSpeed > attr().max_speed) {
        mChHorse.mAimSpeed = attr().max_speed;
    }

    cXyz target_point;
    mChPath.getTargetPoint(target_point);
    cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &target_point), 15, 0x200, 0x40);

    s8 arg1 = mChPath.getArg1();
    switch (mChPath.getArg2()) {
        case -1:
            if (arg1 != -1) {
                changeAtherPath(arg1, current.pos, shape_angle);
            }
            break;

        case 0:
            if (arg1 == -1) {
                break;
            }

            if (!dComIfGs_isSaveDunSwitch(mChPath.getArg3()) || mTotalDmgRecv >= attr().damage_durability) {
                mChHorse.mAimSpeed = 0.0f;
            } else {
                changeAtherPath(arg1, current.pos, shape_angle);
            }
            break;

        case 2:
            dComIfGs_onSaveDunSwitch(mChPath.getArg3());
            break;
    }

    s8 reg_r27 = mChPath.checkNearAttackPoint();
    if (reg_r27 != -1 && checkKargoAttack()) {
        changeAtherPath(reg_r27, current.pos, shape_angle);
        setExpressionAnm(1, true);

        mChHorse.mSound.startCreatureVoice(Z2SE_HS_V_COACH_CRY, -1);
        mChCoach.mSound.startSound(Z2SE_COACH_SHAKE, 0, -1);

        mChCoach.field_0x794 += cM_rnd() < 0.5f ? 4096.0f : -4096.0f;
        mChCoach.field_0x790 -= 30.0f;
        field_0x2550 = attr().deceleration_time;
    }

    if (mChPath.checkPoint(&current.pos, speedF)) {
        mChPath.setNextTarget();
        resetOverAngle();
    }
}

static int daNpcCoach_Draw(daNpcCoach_c* i_this) {
    return i_this->draw();
}

void daNpcCoach_c::reinsDraw() {
    daNpcChReins_c* reins = &mChHorse.mChReins;

    static GXColor l_color = {
        0x14,
        0x0F,
        0x00,
        0xFF,
    };
    if (reins->field_0x16c > 1) {
        reins->field_0x0.update(7, l_color, &tevStr);
        dComIfGd_set3DlineMat(&reins->field_0x0);
        reins->field_0x3c.update(2, l_color, &tevStr);
        dComIfGd_set3DlineMat(&reins->field_0x3c);
    }
}

int daNpcCoach_c::draw() {
    cXyz sp48;

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mChHorse.mpModelMorf->getModel(), &tevStr);
    mChHorse.mpBtpAnm->entry(mChHorse.mpModelMorf->getModel()->getModelData());
    mChHorse.mpModelMorf->entryDL();
    mChHorse.mpBtpAnm->remove(mChHorse.mpModelMorf->getModel()->getModelData());
    sp48.set(current.pos.x, current.pos.y + 150.0f, current.pos.z);
    tevStr.mLightPosWorld = current.pos;
    tevStr.mLightPosWorld.y += 1000.0f;
    mChHorse.mShadowKey = dComIfGd_setShadow(mChHorse.mShadowKey, 0, mChHorse.mpModelMorf->getModel(), &sp48, 1000.0f, 100.0f,
                                             current.pos.y, mChHorse.mBgc.GetGroundH(), mChHorse.mBgc.m_gnd, &tevStr, 0,
                                             1.0f, dDlst_shadowControl_c::getSimpleTex());

    g_env_light.settingTevStruct(0, &mChHarness.field_0x6e0, &mChHarness.mTevStr);
    g_env_light.setLightTevColorType_MAJI(mChHarness.mHarnessModel, &mChHarness.mTevStr);
    mDoExt_modelUpdateDL(mChHarness.mHarnessModel);
    sp48.set(mChHarness.field_0x6e0.x, mChHarness.field_0x6e0.y + 100.0f, mChHarness.field_0x6e0.z);
    mChHarness.mTevStr.mLightPosWorld = mChHarness.field_0x6e0;
    mChHarness.mTevStr.mLightPosWorld.y += 1000.0f;
    mChHarness.mShadowKey = dComIfGd_setShadow(mChHarness.mShadowKey, 0, mChHarness.mHarnessModel, &sp48, 1400.0f, 100.0f,
                                               mChHarness.field_0x6e0.y, mChHarness.mBgc.GetGroundH(), mChHarness.mBgc.m_gnd,
                                               &mChHarness.mTevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());

    g_env_light.settingTevStruct(0, &mChCoach.field_0x5e0, &mChCoach.mTevStr);
    g_env_light.setLightTevColorType_MAJI(mChCoach.mCoachModel, &mChCoach.mTevStr);

    J3DModelData* modelData = mChCoach.mCoachModel->getModelData();
    for (u16 i = 0; i < 2; i++) {
        J3DMaterial* matNodeP = modelData->getMaterialNodePointer(i);

        if (i == 0) {
            matNodeP->getTevKColor(1)->a = field_0x1dc7;
        } else if (i == 1) {
            matNodeP->getTevKColor(1)->a = field_0x1dc5;
            matNodeP->getTevKColor(2)->a = field_0x1dc6;
        }
    }
    mDoExt_modelUpdateDL(mChCoach.mCoachModel);

    sp48.set(mChCoach.field_0x5e0.x, mChCoach.field_0x5e0.y + 100.0f, mChCoach.field_0x5e0.z);
    mChHarness.mTevStr.mLightPosWorld = mChCoach.field_0x5e0;
    mChHarness.mTevStr.mLightPosWorld.y += 1000.0f;
    mChCoach.mShadowKey = dComIfGd_setShadow(mChCoach.mShadowKey, 0, mChCoach.mCoachModel, &sp48, 1400.0f, 100.0f,
                                             mChCoach.field_0x5e0.y, mChCoach.mBgc.GetGroundH(), mChCoach.mBgc.m_gnd,
                                             &mChHarness.mTevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());

    g_env_light.settingTevStruct(0, &mChYelia.field_0x694, &mChYelia.mTevStr);
    g_env_light.setLightTevColorType_MAJI(mChYelia.mpModelMorf->getModel(), &mChYelia.mTevStr);
    mDoExt_modelUpdateDL(mChYelia.mpModelMorf->getModel());

    reinsDraw();

    return 1;
}

static int daNpcCoach_IsDelete(daNpcCoach_c* i_this) {
    return 1;
}

static int daNpcCoach_Delete(daNpcCoach_c* i_this) {
    i_this->~daNpcCoach_c();
    return 1;
}

daNpcCoach_c::~daNpcCoach_c() {
    if (mChHarness.field_0x6a0 != NULL && mChHarness.field_0x6a0->ChkUsed()) {
        dComIfG_Bgsp().Release(mChHarness.field_0x6a0);
    }

    if (mChCoach.field_0x564 != NULL && mChCoach.field_0x564->ChkUsed()) {
        dComIfG_Bgsp().Release(mChCoach.field_0x564);
    }

    fopAcM_delete(parentActorID);
    fopAcM_delete(field_0x2554);
    dComIfG_resDelete(&mPhase, l_arcName);
    dComIfGs_offTbox(1);
}

static int daNpcCoach_Create(fopAc_ac_c* a_this) {
    daNpcCoach_c* i_this = (daNpcCoach_c*)a_this;
    return i_this->create();
}

cPhs__Step daNpcCoach_c::create() {
    fopAcM_ct(this, daNpcCoach_c);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createSolidHeap, 0x7D60)) {
            return cPhs_ERROR_e;
        }

        create_init();
    }

    return phase;
}

void daNpcCoach_c::create_init() {
    if (strcmp(dComIfGp_getStartStageName(), "F_SP123") == 0) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
    }

    fopAcM_setStageLayer(this);
    fopAcM_setCullSizeBox(this, -180.0f, 0.0f, -750.0f, 180.0f, 450.0f, 200.0f);
    attention_info.flags = 0;
    attention_info.distances[fopAc_attn_LOCK_e] = 72;
    attention_info.distances[fopAc_attn_CHECK_e] = 34;
    gravity = attr().gravity;
    maxFallSpeed = attr().max_fall_speed;
    mChHarness.field_0x6fc = attr().max_fall_speed;
    mChCoach.field_0x784 = attr().max_fall_speed;

    mChHorse.field_0x788 = -1;
    setHorseAnm(0);
    mChHorse.mSound.init(&current.pos, &mChHorse.field_0x770, 6, 1);
    mChCoach.mSound.init(&mChCoach.field_0x5e0, 3);
    mChYelia.mSound.init(&mChYelia.field_0x694, &mChYelia.field_0x694, 3, 1);

    dKy_tevstr_init(&mChHarness.mTevStr, fopAcM_GetRoomNo(this), 0xFF);
    dKy_tevstr_init(&mChCoach.mTevStr, fopAcM_GetRoomNo(this), 0xFF);
    dKy_tevstr_init(&mChYelia.mTevStr, fopAcM_GetRoomNo(this), 0xFF);
    field_0x1dc5 = 0xFF;
    field_0x1dc6 = 0x7D;

    initCoachBlazing();
    initCoachPosition(current.pos, shape_angle);
    initBaseMtx();
    initCollision();

    if (getPathID() != 0xFF) {
        mPathID = getPathID();
        mChPath.setPath(getPathID(), fopAcM_GetRoomNo(this), current.pos, true);

        if (!mChPath.isPath()) {
            return;
        }

        s8 arg0 = mChPath.getArg0();
        if (arg0 >= 0) {
            mChHorse.mAimSpeed = arg0;
        }

        if (mChHorse.mAimSpeed > attr().max_speed) {
            mChHorse.mAimSpeed = attr().max_speed;
        }

        cXyz target_point;
        mChPath.getTargetPoint(target_point);
        int dist_ang = cLib_distanceAngleS(shape_angle.y, cLib_targetAngleY(&current.pos, &target_point));
        if (dist_ang >= 0x4000) {
            mChPath.setNextPoint();
            mChPath.getTargetPoint(target_point);

            int o_dist_ang = cLib_distanceAngleS(shape_angle.y, cLib_targetAngleY(&current.pos, &target_point));
            if (o_dist_ang >= 0x4000) {
                mChPath.setPrevPoint();
            }
        }
    }

    for (int i = 0; i < 5; i++) {
        field_0x247c[i] = fpcM_ERROR_PROCESS_ID_e;
    }

    cXyz i_pos;
    csXyz i_angle;

    mDoMtx_stack_c::copy(mChCoach.mCoachModel->getAnmMtx(JNT_COACH_TERUMA_LOC));
    mDoMtx_stack_c::multVecZero(&i_pos);
    mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &i_angle);
    int msg_no = getMessageNo();
    parentActorID = fopAcM_createChild(PROC_NPC_THEB, fopAcM_GetID(this), msg_no, &i_pos, fopAcM_GetRoomNo(this), &i_angle, NULL, -1, NULL);
    field_0x2554 = fopAcM_createChild(PROC_COACH2D, fopAcM_GetID(this), 0, &current.pos, fopAcM_GetRoomNo(this), NULL, NULL, -1, NULL);
    field_0x2558 = 0;
    field_0x2559 = 0;

#if VERSION != VERSION_SHIELD_DEBUG
    // If NOT in the Great Bridge of Hylia Battle:
    if (strcmp(dComIfGp_getStartStageName(), "F_SP123") != 0)
#endif
    {
        dComIfGs_onTbox(1);
    }
}

void daNpcCoach_c::initBaseMtx() {
    setBaseMtx();
    fopAcM_SetMtx(this, mChHorse.mpModelMorf->getModel()->getBaseTRMtx());
}

void daNpcCoach_c::setHarnessMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(mChHarness.field_0x700.y);
    mDoMtx_stack_c::XrotM(mChHarness.field_0x700.x);
    mDoMtx_stack_c::ZrotM(mChHarness.field_0x700.z);
    mChHarness.mHarnessModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mChHarness.mMtx);
    mChHarness.field_0x6a0->Move();
    mChHarness.mHarnessModel->calc();
}

void daNpcCoach_c::setBaseMtx() {
    setHorseMtx();
    setHarnessMtx();
    setCoachMtx();
    setDriverMtx();
    setYeliaMtx();
}

void daNpcCoach_c::initCoachBlazing() {
    cXyz sp28;

    mDoMtx_stack_c::copy(mChCoach.mCoachModel->getAnmMtx(JNT_COACH_BODY));
    mDoMtx_stack_c::multVecZero(&sp28);

    mChCoach.field_0x5f8[0] = dComIfGp_particle_set(0x85F0, &sp28, &mChCoach.field_0x79a, NULL);
    if (mChCoach.field_0x5f8[0] != NULL) {
        mChCoach.field_0x5f8[0]->setGlobalRTMatrix(mChCoach.mCoachModel->getAnmMtx(JNT_COACH_BODY));
        mChCoach.field_0x5f8[0]->setGlobalAlpha(0);
        mChCoach.field_0x5f8[0]->setRate(0.0f);
        mChCoach.field_0x5f8[0]->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
    }

    mChCoach.field_0x5f8[1] = dComIfGp_particle_set(0x85EF, &sp28, &mChCoach.field_0x79a, NULL);
    if (mChCoach.field_0x5f8[1] != NULL) {
        mChCoach.field_0x5f8[1]->setGlobalRTMatrix(mChCoach.mCoachModel->getAnmMtx(JNT_COACH_BODY));
        mChCoach.field_0x5f8[1]->setGlobalAlpha(0);
        mChCoach.field_0x5f8[1]->setRate(0.0f);
        mChCoach.field_0x5f8[1]->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
    }

    for (int i = 2; i < 10; i++) {
        mChCoach.field_0x5f8[i] = NULL;
    }

    mCoachBlazing = false;
}

daNpcCoach_c::daNpcCoach_c() {}

static actor_method_class l_daNpcCoach_Method = {
    (process_method_func)daNpcCoach_Create,
    (process_method_func)daNpcCoach_Delete,
    (process_method_func)daNpcCoach_Execute,
    (process_method_func)daNpcCoach_IsDelete,
    (process_method_func)daNpcCoach_Draw,
};

actor_process_profile_definition g_profile_NPC_COACH = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_COACH,         // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpcCoach_c),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  330,                    // mPriority
  &l_daNpcCoach_Method,   // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
