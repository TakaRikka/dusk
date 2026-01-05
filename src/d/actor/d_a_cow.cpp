/**
* @file d_a_cow.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_cow.h"
#include "SSystem/SComponent/c_lib.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_npc_aru.h"
#include "d/actor/d_a_player.h"
#include "d/d_cc_uty.h"
#include "d/d_com_inf_actor.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2_info.h"
#include "d/d_timer.h"
#include "dolphin/types.h"
#include "f_op/f_op_actor_mng.h"
#include "f_op/f_op_camera_mng.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_lib.h"
#include "m_Do/m_Do_mtx.h"

#define GET_FLAG(mask) (mFlags & (mask))    // cLib_checkBit<u16>(mFlags, (mask))
#define CLEAR_FLAG(mask) mFlags &= ~(mask)  // cLib_offBit<u16>(mFlags, (mask))
#define TICK_TIMER(name)                                                                           \
    if ((name)) {                                                                                  \
        (name)--;                                                                                  \
    }
#define COW_INDEX (u8)(fopAcM_GetParam(this) >> 8)
#define CLAMP(val, min, max)                                                                       \
    if ((val) > (max)) {                                                                           \
        (val) = (max);                                                                             \
    }                                                                                              \
    if ((val) < (min)) {                                                                           \
        (val) = (min);                                                                             \
    }

#define CLAMP_COW_BODY_ANGLE(angle) CLAMP((angle), -0x2000, 0x2000);

namespace {
static dCcD_SrcSph cc_sph_src = {{{0, {{0, 0, {0}}, {0xfbfdfb, {0x11}}, {{0x79}}}},
                                  {0, 0, 0, 0, {0}},
                                  {0, 0, 0, 0, {0x0304}},
                                  {{0}}},
                                 {{{0, 0, 0}, 60.0f}}};

static s16 pen_dir = -0x4000;
static s16 gate_dir = -0x8000;

static f32 l_CowRoomPosY = 15000.0f;

static f32 l_CowRoomPosX[20] = {-10600.0f, -10600.0f, -10800.0f, -10800.0f, -11000.0f,
                                -11000.0f, -11200.0f, -11200.0f, -11400.0f, -11400.0f,
                                -11600.0f, -11600.0f, -11800.0f, -11800.0f, -12000.0f,
                                -12000.0f, -12200.0f, -12200.0f, -12400.0f, -12400.0f};

static f32 l_CowRoomPosZ[2] = {-19646.0f, -20926.0f};

#define N_WOLF_BUSTERS 3
fpc_ProcID gWolfBustersID[N_WOLF_BUSTERS] = {fpcM_ERROR_PROCESS_ID_e, fpcM_ERROR_PROCESS_ID_e, fpcM_ERROR_PROCESS_ID_e};

}  // namespace

int daCow_c::calcRunAnime(int resetRunType) {
    if (resetRunType) {
        mRunType = daCow_c::RunType_None;
    }

    switch (mRunType) {
    case daCow_c::RunType_None:
        if (speedF < 35.0f) {
            setBck(daCow_c::Animation_TrotA, J3DFrameCtrl::EMode_LOOP, 5.0f, 1.0f);
            mRunType = daCow_c::RunType_Trot;
        } else {
            setBck(daCow_c::Animation_Run, J3DFrameCtrl::EMode_LOOP, 5.0f, 1.0f);
            mRunType = daCow_c::RunType_Run;
        }
        break;
    case daCow_c::RunType_Trot: {
        f32 animationSpeed = (speedF * 3.0f) / 25.0f;
        if (animationSpeed > 5.0f) {
            animationSpeed = 5.0f;
        }
        mpMorf->setPlaySpeed(animationSpeed);
        if (speedF > 35.0f) {
            setBck(daCow_c::Animation_Run, J3DFrameCtrl::EMode_LOOP, 5.0f, 1.0f);
            mRunType = daCow_c::RunType_Run;
        }
    } break;
    case daCow_c::RunType_Run:
        mpMorf->setPlaySpeed(1.3f);
        if (speedF < 35.0f) {
            setBck(daCow_c::Animation_TrotA, J3DFrameCtrl::EMode_LOOP, 5.0f, 1.0f);
            mRunType = daCow_c::RunType_Trot;
        }
        break;
    }
    return 1;
}

void daCow_c::setBck(int i_index, u8 i_attr, f32 i_morf, f32 i_rate) {
    J3DAnmTransform* animation = (J3DAnmTransform*)dComIfG_getObjectRes("Cow", i_index);
    mpMorf->setAnm(animation, i_attr, i_morf, i_rate, 0.0f, -1.0f);
}

u8 daCow_c::checkBck(int i_index) {
    J3DAnmTransform* animation = (J3DAnmTransform*)dComIfG_getObjectRes("Cow", i_index);
    return mpMorf->getAnm() == animation;
}

namespace {

static cXyz pen_pos(-10200.0f, 15000.0f, -20246.0f);
static cXyz gate_pos(-9246.0f, 15000.0f, -22763.0f);

static u32 l_CowRoomNo = 0;
static u32 l_CowType = 0;
}  // namespace

void daCow_c::setEffect() {
    if (mShouldSetEffect) {
        if (mShouldSetEffect == 1) {
            cXyz effectPos2;
            cXyz effectPos3;
            cXyz effectPos1;

            if (mCounter1 & 1) {
                cXyz offset(0.0f, 10.0f, -70.0f);

                offset.x = 25.0f;
                cLib_offsetPos(&effectPos1, &current.pos, mSavedAngle.y, &offset);
                offset.x = 0.0f;
                cLib_offsetPos(&effectPos2, &current.pos, mSavedAngle.y, &offset);
                offset.x = -25.0f;
                // Not effectPos3? Is this a bug in the original code?
                cLib_offsetPos(&effectPos2, &current.pos, mSavedAngle.y, &offset);
            } else {
                mShouldSetEffect = 0;
            }

            int roomNumber = fopAcM_GetRoomNo(this);

            mParticle.setEffectTwo(&tevStr, &current.pos, 0, 0,
                                   mShouldSetEffect ? &effectPos1 : NULL,
                                   mShouldSetEffect ? &effectPos2 : NULL,
                                   mShouldSetEffect ? &effectPos3 : NULL,  //
                                   &mSavedAngle, NULL, roomNumber, 1.0f, speedF);

            static cXyz runScale(2.0f, 2.0f, 2.0f);

            for (int j, i = 0; i < 3; i++) {
                for (j = 0; j < 2; j++) {
                    JPABaseEmitter* emitter = mParticle.getEmitterTwo(i, j, 0);
                    if (emitter) {
                        emitter->setGlobalScale(runScale);
                        emitter->setRate(1.3f);
                    }
                }
            }
        } else {
            fopAcM_effSmokeSet1(&mSmokeKey, &mSmokeKey2, &current.pos, NULL, 2.0f, &tevStr, 1);
        }
        mShouldSetEffect = 0;
    }
}

bool daCow_c::isChaseCowGame() {
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP00")) {
        if (dComIfG_play_c::getLayerNo(0) == 4 || dComIfG_play_c::getLayerNo(0) == 5) {
            return true;
        }
    }
    return false;
}

void daCow_c::setCarryStatus() {
    f32 zMax = 270.0f;
    f32 xMax = 70.0f;
    if (speedF >= 10.0f) {
        zMax = 600.0f;
        xMax = 100.0f;
    }
    mDoMtx_stack_c::YrotS(-shape_angle.y);
    mDoMtx_stack_c::transM(-current.pos.x, -current.pos.y, -current.pos.z);

    Vec carryPosition;
    mDoMtx_stack_c::multVec(&daPy_getPlayerActorClass()->current.pos, &carryPosition);
    if (fabsf(carryPosition.x) < xMax && carryPosition.z > 0.0f && carryPosition.z < zMax) {
        // todo: what does this mean
        attention_info.flags |= fopAc_AttnFlag_CARRY_e;  // in debug this is 0x80
    }
}

void daCow_c::setActetcStatus() {
    if (!mNadeNade) {
        s32 playerAngle = fopAcM_seenPlayerAngleY(this);
        if (playerAngle < 0x6000 && playerAngle > 0x2000) {
            attention_info.flags |= fopAc_AttnFlag_ETC_e;  // in debug this is 0x800
        }
    }
}

// NOTE: nade nade seems to be petting the Goat

bool daCow_c::checkNadeNadeFinish() {
    if (GET_FLAG(daCow_c::Flag_NaderuFinish)) {
        CLEAR_FLAG(daCow_c::Flag_NaderuFinish);
        mNadeNade = false;
        return true;
    }
    return false;
}

bool daCow_c::checkNadeNade() {
    if (GET_FLAG(daCow_c::Flag_Naderu)) {
        CLEAR_FLAG(daCow_c::Flag_Naderu);
        mNadeNade = true;
        return 1;
    }
    return 0;
}

void daCow_c::setSeSnort() {
    if (mpMorf->checkFrame(1.0f)) {
        mSound.startCreatureVoice(Z2SE_GOAT_V_BREATH, -1);
    }
}

void daCow_c::setRushVibration(int i_vibmode) {
    if (mpMorf->checkFrame(1.0f)) {
        dComIfGp_getVibration().StartShock(i_vibmode, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
    }
}

bool daCow_c::checkThrow() {
    if (mFlags) {
        if (GET_FLAG(daCow_c::Flag_CrazyBeforeCatch)) {
            setProcess(&daCow_c::action_thrown, false);
            initCrazyBeforeCatch(0);
            CLEAR_FLAG(daCow_c::Flag_CrazyBeforeCatch);
            return true;
        }
        if (GET_FLAG(daCow_c::Flag_CrazyCatch)) {
            setProcess(&daCow_c::action_thrown, false);
            initCrazyCatch(0);
            CLEAR_FLAG(daCow_c::Flag_CrazyCatch);
            return true;
        }
    }
    return false;
}

void daCow_c::setBodyAngle(s16 angle) {
    s16 offsetAngle = mSavedAngle.y - angle;

    CLAMP_COW_BODY_ANGLE(offsetAngle);

    // round small angles to 0
    if (abs(offsetAngle) < 0x100) {
        offsetAngle = 0;
    }
    cLib_chaseS(&mJoint8Offset.y, offsetAngle * 0.7f, 0x100);
    cLib_chaseS(&mJoint1Offset.y, offsetAngle * 0.3f, 0x100);
}

void daCow_c::setBodyAngle2(s16 angle) {
    s16 offsetAngle = mSavedAngle.y - angle;

    CLAMP_COW_BODY_ANGLE(offsetAngle);

    if (abs(offsetAngle) < 0x100) {
        offsetAngle = 0;
    }

    cLib_chaseS(&mJoint1Offset.y, offsetAngle * 0.3f, 0x100);
    daPy_py_c* player = daPy_getPlayerActorClass();
    s16 targetAngle = cLib_targetAngleY(&current.pos, &player->current.pos);
    s16 bodyAngle = mSavedAngle.y - targetAngle;

    s16 chaseAngle;
    if (abs(bodyAngle) < 0x5000) {
        CLAMP_COW_BODY_ANGLE(bodyAngle);
        chaseAngle = bodyAngle;
    } else {
        chaseAngle = offsetAngle * 0.7f;
    }

    cLib_chaseS(&mJoint8Offset.y, chaseAngle, 0x100);
}

BOOL daCow_c::checkProcess(void (daCow_c::*process)()) {
    return mProcess == process;
}

bool daCow_c::setProcess(void (daCow_c::*process)(), BOOL doAnim6) {
    mMode = daCow_c::Mode_3;
    if (mProcess) {
        (this->*mProcess)();
    }
    mDoConnectAnim = doAnim6;
    mMode = daCow_c::Mode_0;
    mProcess = process;
    if (mProcess) {
        (this->*mProcess)();
    }

    return true;
}

// determines which attack types will damage the goats
#define COW_ATTACK_TYPES                                                                           \
    (AT_TYPE_NORMAL_SWORD | AT_TYPE_BOMB | AT_TYPE_ARROW | AT_TYPE_SPINNER | AT_TYPE_IRON_BALL)

void daCow_c::damage_check() {
    mCcStts.Move();

    if (getCowIn()) {
        return;
    }

    if (mInvulnerabilityTimer) {
        mInvulnerabilityTimer--;
        return;
    }

    cCcD_ObjHitInf* hitObject = NULL;
    for (int iSphere = 0; iSphere < N_COW_COLLIDERS; iSphere++) {
        if (mSph[iSphere].ChkTgHit()) {
            hitObject = mSph[iSphere].GetTgHitObj();
            break;
        }
    }

    if (!hitObject) {
        return;
    }
    mInvulnerabilityTimer = 10;

    if (checkProcess(&daCow_c::action_crazy)) {
        if (mCrazy == daCow_c::Crazy_Back) {
            if (mAction == daCow_c::Action_Wait) {
                if (hitObject->ChkAtType(COW_ATTACK_TYPES)) {
                    mTakenDamageCounter = 150;
                } else {
                    mTakenDamageCounter += 60;
                }
                if (mTakenDamageCounter >= 150) {
                    mAction = daCow_c::Action_5;
                }
            }
        }
    } else if (checkProcess(&daCow_c::action_angry)) {
        mTimer10 = 200;
    } else if (hitObject->ChkAtType(COW_ATTACK_TYPES)) {
        setProcess(&daCow_c::action_damage, false);
    } else {
        mTakenDamageCounter += 60;
        if (mTakenDamageCounter >= 150) {
            setProcess(&daCow_c::action_damage, false);
        } else {
            mWaitTimer = 90;
            if (!checkProcess(&daCow_c::action_wait)) {
                speedF = 0.0f;
                setProcess(&daCow_c::action_wait, false);
            }
        }
    }

    mSph[0].ClrTgHit();
    mSph[1].ClrTgHit();
    mSph[2].ClrTgHit();
}

void daCow_c::setEnterCow20() {
    for (int iCow = 0; iCow < 20; iCow++) {
        cXyz spawnPosition(l_CowRoomPosX[iCow], l_CowRoomPosY, l_CowRoomPosZ[iCow & 1]);

        l_CowRoomNo |= 1 << iCow;

        csXyz spawnAngle;
        if (iCow & 1) {
            spawnAngle.set(0, 0, 0);
        } else {
            spawnAngle.set(0, -0x8000, 0);
        }

        fopAcM_create(0x106, 0xFFFFFF04, &spawnPosition, fopAcM_GetRoomNo(this), &spawnAngle, 0,
                      -1);
    }
}

static u32 cow_number[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

void daCow_c::setEnterCow10() {
    for (int iCow = 0; iCow < 10; iCow++) {
        int cowNumber = cow_number[iCow];

        cXyz spawnPosition(l_CowRoomPosX[cowNumber], l_CowRoomPosY, l_CowRoomPosZ[cowNumber & 1]);

        l_CowRoomNo |= 1 << cowNumber;

        csXyz spawnAngle;
        if (cowNumber & 1) {
            spawnAngle.set(0, 0, 0);
        } else {
            spawnAngle.set(0, -0x8000, 0);
        }
        int roomNumber = fopAcM_GetRoomNo(this);
        fopAcM_create(0x106, 0xFFFFFF04, &spawnPosition, roomNumber, &spawnAngle, 0, -1);
    }
}

void daCow_c::setGroundAngle() {
    s16 alpha = 0;
    s16 beta = 0;

    if (mAcch.ChkGroundHit()) {
        cM3dGPla plane;

        dComIfG_Bgsp().GetTriPla(mAcch.m_gnd, &plane);
        cXyz* pfVar2 = plane.GetNP();
        alpha = (s16)cM_atan2s(pfVar2->z, pfVar2->y);
        beta = (s16)cM_atan2s(pfVar2->x, pfVar2->y);
    }

    cLib_chaseAngleS(&mGroundTransform.x, alpha, 128);
    cLib_chaseAngleS(&mGroundTransform.y, beta, 128);
}

static f32 m_near_dist;

static s16 m_view_angle_wide;

static s16 m_view_angle;

#define IS_COW(actor) (fopAcM_GetName((actor)) == PROC_COW)
#define IS_VALID_COW_INTERACTION(cow_1, actor)                                                     \
    (fopAcM_IsActor((cow_1)) && !fpcM_IsCreating(fopAcM_GetID((cow_1))) && IS_COW((cow_1)) &&      \
     (cow_1) != (actor))

static void* s_near_cow(void* candidate, void* data) {
    if (IS_VALID_COW_INTERACTION(candidate, data)) {
        daCow_c* candidateCow = (daCow_c*)candidate;
        daCow_c* self = (daCow_c*)data;

        if (!candidateCow->getCowIn()) {
            s16 candidateAngleY = fopAcM_searchActorAngleY(self, candidateCow);
            candidateAngleY = cLib_distanceAngleS(candidateAngleY, self->getShapeAngle().y);
            if (cLib_distanceAngleS(candidateAngleY, m_view_angle) < m_view_angle_wide) {
                f32 candidateDistance = fopAcM_searchActorDistance(candidateCow, self);
                if (candidateDistance < m_near_dist) {
                    m_near_dist = candidateDistance;
                }
            }
        }
    }
    return NULL;
}

static s16 m_angry_cow;

static void* s_angry_cow(void* candidate, void* data) {
    fopAc_ac_c* self = (fopAc_ac_c*)data;
    if (IS_VALID_COW_INTERACTION(candidate, self)) {
        daCow_c* candidateCow = (daCow_c*)candidate;
        if ((candidateCow->isAngry() || candidateCow->isGuardFad()) &&
            fopAcM_searchActorDistance(candidateCow, self) < 500.0f)
        {
            m_angry_cow = true;
        }
    }
    return NULL;
}

static void* s_angry_cow2(void* candidate, void* self) {
    if (IS_VALID_COW_INTERACTION(candidate, (fopAc_ac_c*)self)) {
        daCow_c* candidateCow = (daCow_c*)candidate;
        if (candidateCow->isAngry()) {
            m_angry_cow = true;
            return candidateCow;
        }
    }
    return NULL;
}

bool daCow_c::checkRun() {
    return checkProcess(&daCow_c::action_run);
}

bool daCow_c::checkNearCowRun() {
    if (getCowIn()) {
        return false;
    }

    m_angry_cow = false;
    fpcM_Search(&s_angry_cow, this);

    if (m_angry_cow) {
        return true;
    }

    if (!isChaseCowGame()) {
        return false;
    }

    for (int iSphere = 0; iSphere < N_COW_COLLIDERS; iSphere++) {
        cCcD_Obj* obj = mSph[iSphere].GetCoHitObj();
        if (!obj) {
            continue;
        }

        fopAc_ac_c* actor = dCc_GetAc(obj->GetAc());
        if (actor && IS_COW(actor)) {
            daCow_c* cow = static_cast<daCow_c*>(actor);
            if (!cow->mNoNearCheckTimer && cow->checkRun()) {
                s16 angle = fopAcM_searchActorAngleY(this, actor);
                int angleDifference = cLib_distanceAngleS(angle, mSavedAngle.y);
                if (angleDifference >= 0x1000 && angleDifference < 0x7000) {
                    return true;
                }
            }
        }
    }

    return false;
}

void daCow_c::action_wait() {
    f32 rand = cM_rnd();

    switch (mMode) {
    case daCow_c::Mode_0:
        mIdleTimer = cM_rndF(100.0f) + 300.0f;
        mMode = daCow_c::Mode_1;
        mTimer1 = 0;
        if (!mDoConnectAnim) {
            setBck(daCow_c::Animation_WaitA, J3DFrameCtrl::EMode_LOOP, 12.0f, 1.0f);
            mMode = daCow_c::Mode_2;
        } else {
            setBck(daCow_c::Animation_ConnectA, J3DFrameCtrl::EMode_NONE, 12.0f, 1.0f);
            mpMorf->setFrame(mpMorf->getEndFrame());
            mpMorf->setPlaySpeed(-1.0f);
            mMode = daCow_c::Mode_1;
        }
        break;

    case daCow_c::Mode_1:
        if (mpMorf->isStop()) {
            setBck(daCow_c::Animation_WaitA, J3DFrameCtrl::EMode_LOOP, 0.0f, 1.0f);
            mMode = daCow_c::Mode_2;
        }

    case daCow_c::Mode_2: {
        s16 jointAngle = 0;
        if (mWaitTimer > 30) {
            jointAngle = mSavedAngle.y - fopAcM_searchPlayerAngleY(this);
            CLAMP(jointAngle, -0x2800, 0x2800);
        }

        cLib_addCalcAngleS2(&mJoint8Offset.y, jointAngle * 0.9f, 0x10, 0x100);
        cLib_addCalcAngleS2(&mJoint1Offset.y, jointAngle * 0.1f, 0x10, 0x100);

        if (!getCowIn()) {
            if (checkCowInOwn(0x8000)) {
                return;
            }
            if (mNadeNade) {
                mWaitTimer = 0;
                if (!checkNadeNadeFinish()) {
                    return;
                }
                setProcess(&daCow_c::action_moo, false);
                return;
            }
            if (checkPlayerWait() && (checkPlayerSurprise() || checkPlayerPos())) {
                setProcess(&daCow_c::action_run, false);
                return;
            }
            if (checkWolfBusters()) {
                return;
            }
            setActetcStatus();
            if (checkNadeNade()) {
                return;
            }
            setCarryStatus();
            if (checkThrow()) {
                return;
            }
            if (checkNearCowRun()) {
                setProcess(&daCow_c::action_run, false);
                return;
            }
        }
        if (!cLib_calcTimer(&mIdleTimer) && !mWaitTimer) {
            if (checkNearWolf()) {
                setProcess(&daCow_c::action_moo, false);
            } else {
                daPy_py_c* playerActor = daPy_getPlayerActorClass();
                if (current.pos.absXZ(playerActor->current.pos) > 500.0f && rand < 0.4f) {
                    setProcess(&daCow_c::action_moo, false);
                } else if (rand < 0.5f) {
                    setProcess(&daCow_c::action_eat, true);
                } else {
                    setProcess(&daCow_c::action_shake, true);
                }
            }
        }
    } break;
    case daCow_c::Mode_3:
        mJoint1Offset.y = 0;
        mJoint8Offset.y = 0;
        mWaitTimer = 0;
        mNadeNade = false;
        break;
    }
}

void daCow_c::action_eat() {
    switch (mMode) {
    case daCow_c::Mode_0:
        mIdleTimer = cM_rndF(100.0f) + 300.0f;
        mTimer1 = 0;
        if (!mDoConnectAnim) {
            setBck(daCow_c::Animation_EatA, J3DFrameCtrl::EMode_LOOP, 12.0f, 1.0f);
            mMode = daCow_c::Mode_2;
        } else {
            setBck(daCow_c::Animation_ConnectA, J3DFrameCtrl::EMode_NONE, 12.0f, 1.0f);
            mMode = daCow_c::Mode_1;
        }
        break;
    case daCow_c::Mode_1:
        if (mpMorf->isStop()) {
            setBck(daCow_c::Animation_EatA, J3DFrameCtrl::EMode_LOOP, 0.0f, 1.0f);
            mMode = daCow_c::Mode_2;
        }
    case daCow_c::Mode_2:
        if (mpMorf->checkFrame(10.0f) || mpMorf->checkFrame(40.0f) || mpMorf->checkFrame(68.0f) ||
            mpMorf->checkFrame(98.0f))
        {
            mSound.startCreatureVoice(Z2SE_GOAT_V_EAT, -1);
        }

        if (!getCowIn()) {
            if (checkNearCowRun() || checkPlayerWait()) {
                setProcess(&daCow_c::action_wait, false);
                return;
            }
            setCarryStatus();
            if (checkThrow()) {
                return;
            }
            setActetcStatus();
            if (checkNadeNade()) {
                setProcess(&daCow_c::action_wait, true);
                return;
            }
        }
        if (!cLib_calcTimer(&mIdleTimer) && mpMorf->isLoop()) {
            if (checkNearWolf()) {
                setProcess(&daCow_c::action_moo, false);
            } else {
                f32 rand = cM_rnd();
                if (current.pos.absXZ(daPy_getPlayerActorClass()->current.pos) > 500.0f &&
                    rand < 0.4f)
                {
                    setProcess(&daCow_c::action_moo, false);
                    return;
                }

                if (rand < 0.5f) {
                    setProcess(&daCow_c::action_shake, false);
                } else {
                    setProcess(&daCow_c::action_wait, true);
                }
            }
        }
        break;
    case daCow_c::Mode_3:
        break;
    }
}

void daCow_c::action_moo() {
    switch (mMode) {
    case daCow_c::Mode_0:
        if (!mDoConnectAnim) {
            setBck(daCow_c::Animation_MooA, J3DFrameCtrl::EMode_NONE, 12.0f, 1.0f);
            mMode = daCow_c::Mode_2;
        } else {
            setBck(daCow_c::Animation_ConnectA, J3DFrameCtrl::EMode_NONE, 12.0f, 1.0f);
            mpMorf->setFrame(mpMorf->getEndFrame());
            mpMorf->setPlaySpeed(-1.0f);
            mMode = daCow_c::Mode_1;
        }
        break;
    case daCow_c::Mode_1:
        if (mpMorf->isStop()) {
            setBck(daCow_c::Animation_MooA, J3DFrameCtrl::EMode_NONE, 0.0f, 1.0f);
            mMode = daCow_c::Mode_2;
        }
    case daCow_c::Mode_2:
        if (mpMorf->checkFrame(35.0f)) {
            mSound.startCreatureVoice(Z2SE_GOAT_V_CRY, -1);
        }

        if (!getCowIn()) {
            if (checkNearCowRun() || checkPlayerWait()) {
                setProcess(&daCow_c::action_wait, false);
                return;
            } else {
                setCarryStatus();
                if (checkThrow()) {
                    return;
                }
            }
        }
        if (mpMorf->isStop()) {
            if (checkNearWolf()) {
                setProcess(&daCow_c::action_shake, true);
            } else {
                f32 rand = cM_rnd();
                if (rand < 0.4f) {
                    setProcess(&daCow_c::action_eat, true);
                } else if (rand < 0.7f) {
                    setProcess(&daCow_c::action_shake, true);
                } else {
                    setProcess(&daCow_c::action_wait, false);
                }
            }
        }
    case daCow_c::Mode_3:
        break;
    }
}

void daCow_c::action_shake() {
    switch (mMode) {
    case daCow_c::Mode_0:
        if (!mDoConnectAnim) {
            setBck(daCow_c::Animation_ShakeA, J3DFrameCtrl::EMode_LOOP, 12.0f, 1.0f);
            mMode = daCow_c::Mode_2;
        } else {
            setBck(daCow_c::Animation_ConnectA, J3DFrameCtrl::EMode_NONE, 12.0f, 1.0f);
            mMode = daCow_c::Mode_1;
        }
        break;
    case daCow_c::Mode_1:
        if (mpMorf->isStop()) {
            setBck(daCow_c::Animation_ShakeA, J3DFrameCtrl::EMode_LOOP, 0.0f, 1.0f);
            mMode = daCow_c::Mode_2;
        }
    case daCow_c::Mode_2:

        if (mpMorf->checkFrame(68.0f)) {
            mSound.startCreatureVoice(Z2SE_GOAT_V_NOSE, -1);
        }

        if (!getCowIn()) {
            if (checkNearCowRun() || checkPlayerWait()) {
                setProcess(&daCow_c::action_wait, false);
                return;
            }
            setCarryStatus();
            if (checkThrow()) {
                return;
            }
            setActetcStatus();
            if (checkNadeNade()) {
                setProcess(&daCow_c::action_wait, true);
                return;
            }
        }
        if (mpMorf->isLoop()) {
            if (checkNearWolf()) {
                setProcess(&daCow_c::action_moo, true);
            } else {
                f32 rand = cM_rnd();
                if (current.pos.absXZ(daPy_getPlayerActorClass()->current.pos) > 500.0f &&
                    rand < 0.4f)
                {
                    setProcess(&daCow_c::action_moo, true);
                } else if (rand < 0.5f) {
                    setProcess(&daCow_c::action_eat, false);
                } else {
                    setProcess(&daCow_c::action_wait, true);
                }
            }
        }
    case daCow_c::Mode_3:
        break;
    }
}

bool daCow_c::checkNearWolf() {
    if (daPy_getPlayerActorClass()->checkNowWolf() && fopAcM_searchPlayerDistance(this) < 3000.f) {
        return true;
    } else {
        return false;
    }
}

bool daCow_c::checkPlayerWait() {
    if ((daPy_getPlayerActorClass()->checkHorseRide() ||
         daPy_getPlayerActorClass()->checkNowWolf()) &&
        fopAcM_searchPlayerDistance(this) < mAttentionDistance)
    {
        return true;
    } else {
        return false;
    }
}

bool daCow_c::checkPlayerSurprise() {
    if (getCowIn()) {
        return false;
    }

    daPy_py_c* player = daPy_getPlayerActorClass();

    if (!player->checkHorseRide()) {
        return false;
    }

    if (fopAcM_searchPlayerDistance(this) < 1500.0f && player->checkCowGameLash()) {
        mSound.startCreatureVoice(Z2SE_GOAT_V_CRY, -1);
        mRunTimer = 50;
        return true;
    } else {
        return false;
    }
}

bool daCow_c::checkPlayerPos() {
    if (getCowIn()) {
        return false;
    }

    mExecute = daCow_c::Execute_6;

    f32 playerDistance = fopAcM_searchPlayerDistance(this);
    float cutoffDistance = mAttentionDistance;
    if (mRunTimer) {
        cutoffDistance = 1500.0f;
    }
    if (playerDistance > cutoffDistance) {
        return false;
    }

    s16 angleDifference = fopAcM_searchPlayerAngleY(this) - mSavedAngle.y;
    s16 absAngleDifference = abs(angleDifference);
    if (absAngleDifference < 0x2000) {
        if (angleDifference > 0) {
            mExecute = daCow_c::Execute_0;
        } else {
            mExecute = daCow_c::Execute_1;
        }
    } else if (absAngleDifference > 0x6000) {
        if (angleDifference > 0) {
            mExecute = daCow_c::Execute_4;
        } else {
            mExecute = daCow_c::Execute_5;
        }
    } else {
        f32 a = fabsf(playerDistance * cM_scos(angleDifference));
        f32 b = fabsf(cutoffDistance * cM_scos(0x2000));
        if (a > b) {
            return false;
        }

        f32 c = fabsf(playerDistance * cM_ssin(angleDifference));
        f32 d = fabsf(cutoffDistance * cM_ssin(0x2000));

        if (c > d) {
            return false;
        }

        if (angleDifference > 0) {
            mExecute = daCow_c::Execute_2;
        } else {
            mExecute = daCow_c::Execute_3;
        }
    }

    return true;
}

void daCow_c::checkBeforeBg() {
    s16 x[3] = {0, -0x2000, 0x2000};
    cXyz a;
    f32 y[3];
    cXyz b, c;
    s16 z[3];
    cM3dGPla planes[3];

    b = current.pos;
    b.y += 110.0f;

    bool planeTri[3];
    for (int iPlane = 0; iPlane < 3; iPlane++) {
        planeTri[iPlane] = false;
    }

    f32 f1 = 1000.0f;
    f32 f2 = 700.0f;

    if (checkCowIn(1000.0f, 0.0f)) {
        f1 = 800.0f;
        f2 = 560.0f;
    }

    if (speedF) {
        for (int iPlane = 0; iPlane < 3; iPlane++) {
            c = b;
            if (!iPlane) {
                c.x += f1 * cM_ssin(mSavedAngle.y + x[iPlane]);
                c.z += f1 * cM_scos(mSavedAngle.y + x[iPlane]);
            } else {
                c.x += f2 * cM_ssin(mSavedAngle.y + x[iPlane]);
                c.z += f2 * cM_scos(mSavedAngle.y + x[iPlane]);
            }
            dBgS_LinChk linChk;
            linChk.Set(&b, &c, this);
            if (dComIfG_Bgsp().LineCross(&linChk)) {
                planeTri[iPlane] = dComIfG_Bgsp().GetTriPla(linChk, &planes[iPlane]);
                if ((f32)fabs(planes[iPlane].mNormal.y) <= cM_ssin(0x6000)) {
                    a = current.pos - linChk.GetCross();
                    y[iPlane] = a.absXZ();
                    z[iPlane] = cM_atan2s(planes[iPlane].mNormal.x, planes[iPlane].mNormal.z);
                } else {
                    planeTri[iPlane] = false;
                }
            }
        }
    }

    mBoostSpeed = 1000.0f;
    if (planeTri[0] && y[0] < mBoostSpeed) {
        mBoostSpeed = y[0];
        mBoostAngle = z[0];
    }

    mIntersectedPlanes = 0;
    if (planeTri[0]) {
        mIntersectedPlanes |= 1;
    }
    if (planeTri[1]) {
        mIntersectedPlanes |= 2;
    }
    if (planeTri[2]) {
        mIntersectedPlanes |= 4;
    }
    if (cLib_calcTimer(&mTimer6)) {
        return;
    }

    if (planeTri[1] && planeTri[2]) {
        switch (mAction) {
        case daCow_c::Action_NadeNade:
        case daCow_c::Action_3:
            mAction = daCow_c::Action_3;
            break;
        case daCow_c::Action_Cry:
        case daCow_c::Action_4:
            mAction = daCow_c::Action_4;
            break;
        default:
            if (mExecute & 1) {
                mAction = daCow_c::Action_4;
            } else {
                mAction = daCow_c::Action_3;
            }
        }
        mTimer6 = 10;
    } else if (planeTri[0]) {
        if (planeTri[1]) {
            if (mExecute >= daCow_c::Execute_4) {
                mAction = daCow_c::Action_Cry;
            } else {
                mAction = daCow_c::Action_4;
            }
        } else if (planeTri[2]) {
            if (mExecute >= daCow_c::Execute_4) {
                mAction = daCow_c::Action_NadeNade;
            } else {
                mAction = daCow_c::Action_3;
            }
        } else {
            s16 sVar2 = z[0] - mSavedAngle.y;
            if (abs(sVar2) > 0x7800) {
                if (mExecute & 1) {
                    mAction = daCow_c::Action_4;
                } else {
                    mAction = daCow_c::Action_3;
                }
            } else if (sVar2 > 0) {
                mAction = daCow_c::Action_4;
            } else {
                mAction = daCow_c::Action_3;
            }
        }
    } else if (planeTri[1]) {
        switch (mExecute) {
        case daCow_c::Execute_0:
        case daCow_c::Execute_1:
            mAction = daCow_c::Action_4;
            mTimer6 = 10;
            break;
        case daCow_c::Execute_3:
            mAction = daCow_c::Action_Cry;
            break;
        default:
            mAction = daCow_c::Action_Wait;
            if (y[1] < 600.0f) {
                mAction = daCow_c::Action_Cry;
            }
            break;
        }
    } else if (planeTri[2]) {
        switch (mExecute) {
        case daCow_c::Execute_0:
        case daCow_c::Execute_1:
            mAction = daCow_c::Action_3;
            mTimer6 = 10;
            break;
        case daCow_c::Execute_2:
            mAction = daCow_c::Action_NadeNade;
            break;
        default:
            mAction = daCow_c::Action_Wait;
            if (y[2] < 600.0f) {
                mAction = daCow_c::Action_NadeNade;
            }
            break;
        }
    } else {
        switch (mExecute) {
        case daCow_c::Execute_0:
            mAction = daCow_c::Action_3;
            break;
        case daCow_c::Execute_1:
            mAction = daCow_c::Action_4;
            break;
        case daCow_c::Execute_2:
            mAction = daCow_c::Action_NadeNade;
            break;
        case daCow_c::Execute_3:
            mAction = daCow_c::Action_Cry;
            break;
        default:
            mAction = daCow_c::Action_Wait;
        }
    }
}

int daCow_c::checkOutOfGate(cXyz pos) {
    if (isChaseCowGame()) {
        return 0;
    }

    cXyz x = pos - gate_pos;
    mDoMtx_stack_c::YrotS(gate_dir);
    mDoMtx_stack_c::multVecSR(&x, &x);

    if (x.z > 0.0f && fabsf(x.x) < 1100.0f) {
        return 1;
    }

    x = pos - pen_pos;
    mDoMtx_stack_c::YrotS(-pen_dir);
    mDoMtx_stack_c::multVecSR(&x, &x);

    return x.z > 0.0f ? 2 : 0;
}

s16 daCow_c::getCowshedAngle() {
    return (s16)cLib_targetAngleY(&current.pos, &pen_pos);
}

double daCow_c::getCowshedDist() {
    cXyz diff = pen_pos - current.pos;
    return diff.absXZ();
}

int daCow_c::checkCowIn(f32 cowshedDist, f32 cowshedAngle) {
    if (!isChaseCowGame()) {
        return 0;
    }

    double dist = getCowshedDist();
    if (dist < cowshedDist) {
        s16 angle = getCowshedAngle();
        if (dist < cowshedAngle) {
            if ((s16)angle < pen_dir + 0x2000 && (s16)angle > pen_dir - 0x2000 &&
                cLib_distanceAngleS(angle, mSavedAngle.y) < 0x1800)
            {
                return 1;
            }
            return 2;
        } else if (((s16)angle < pen_dir + 0x2aaa) && (s16)angle > pen_dir - 0x2aaa &&
                   cLib_distanceAngleS(angle, mSavedAngle.y) < 0x3000)
        {
            return 1;
        }
    }
    return 0;
}

bool daCow_c::checkCowInOwn(int angle) {
    if (!isChaseCowGame()) {
        return false;
    }

    cXyz diff = current.pos - pen_pos;
    mDoMtx_stack_c::YrotS(-pen_dir);
    mDoMtx_stack_c::multVecSR(&diff, &diff);
    if (diff.z > 250.0f && fabsf(diff.x) < 220.0f &&
        cLib_distanceAngleS(pen_dir, mSavedAngle.y) < angle)
    {
        setProcess(&daCow_c::action_enter, false);
        setEnterCount();
        return true;
    }

    return false;
}

void daCow_c::action_run() {
    switch (mMode) {
    case daCow_c::Mode_0:
        calcRunAnime(true);
        mMode = daCow_c::Mode_1;
        mTimer1 = 30;
        mForgetCowPTimer = 50;
        mRunDuration = 0;
        mWillGetAngry = false;
        mOutOfGate = 0;
        break;
    case daCow_c::Mode_1: {
        calcRunAnime(false);

        TICK_TIMER(mTimer1);
        TICK_TIMER(mForgetCowPTimer);
        TICK_TIMER(mRunTimer);

        f32 acceleration = 1.0f;
        if (!mForgetCowPTimer) {
            mCowP = NULL;
        }
        if (!isChaseCowGame()) {
            if (!mOutOfGate) {
                setCarryStatus();
            }

            u8 willGetAngry = mWillGetAngry;
            if (checkThrow()) {
                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER2, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
                mWillGetAngry = willGetAngry;
                return;
            }
        } else {
            if (checkCowInOwn(0x8000)) {
                return;
            }
            checkPlayerSurprise();
        }

        bool havePlayerPos = checkPlayerPos();

        if (mExecute == daCow_c::Execute_0 || mExecute == daCow_c::Execute_1 ||
            mExecute == daCow_c::Execute_6)
        {
            mRunDuration = 0;
        }

        checkBeforeBg();

        f32 targetSpeed;
        if (!havePlayerPos || !checkPlayerWait()) {
            if (!mTimer1) {
                targetSpeed = 0.0f;
            } else {
                targetSpeed = (mSpeed - 10.0f) * (mBoostSpeed / 1000.0f);
            }
        } else {
            f32 rand = cM_rndFX(200.0f) * 20.0f;
            mTurningSpeed = rand;
            mCowP = 0;
            acceleration = 2.0f;

            if (daPy_getPlayerActorClass()->checkHorseRide() ||
                daPy_getPlayerActorClass()->checkNowWolf())
            {
                f32 rand = cM_rndF(100.0f);
                mTimer1 = (u8)(rand + 30.0f);
            }
            targetSpeed = mSpeed * (mBoostSpeed / 1000.0f);
        }

        if (!targetSpeed) {
            m_near_dist = 200.0f;
            m_view_angle = -0x8000;
            m_view_angle_wide = 0x2000;
            fpcEx_Search(s_near_cow, this);
            if (m_near_dist < 200.0f) {
                targetSpeed = mSpeed * (mBoostSpeed / 1000.0f);
            }
        } else {
            m_near_dist = 200.0f;
            m_view_angle = 0;
            m_view_angle_wide = 0x2000;
            fpcEx_Search(s_near_cow, this);
            if (m_near_dist < 200.0f) {
                targetSpeed = mSpeed * (mBoostSpeed / 1000.0f) / 2.0f;
            }
        }
        if (mTimer6 == 0 || mTimer6 == 10) {
            s16 targetAngle = current.angle.y;
            if (!mIntersectedPlanes && mCowP) {
                targetAngle = getCowP()->mSavedAngle.y;
            }

            switch (mAction) {
            case daCow_c::Action_NadeNade:
                targetAngle -= 0x1000;
                break;
            case daCow_c::Action_Cry:
                targetAngle += 0x1000;
                break;
            case daCow_c::Action_3:
                targetAngle -= 0x4000;
                break;
            case daCow_c::Action_4:
                targetAngle += 0x4000;
                break;
            case daCow_c::Action_Wait: {
                s16 cowshedAngle = getCowshedAngle();
                s16 playerAngle = fopAcM_searchPlayerAngleY(this);
                targetAngle = playerAngle - (s16)0x8000;
                if (isChaseCowGame() && cLib_distanceAngleS(cowshedAngle, mSavedAngle.y) < 0x3000 &&
                    cLib_distanceAngleS(cowshedAngle, playerAngle) > 0x5800)
                {
                    targetAngle = cowshedAngle;
                }
            } break;
            }

            if (!havePlayerPos) {
                targetAngle += mTurningSpeed;
            }
            mTargetAngle = targetAngle;
        }

        int outOfGate = checkOutOfGate(current.pos);
        if (outOfGate) {
            mTimer1 = 150;
            mOutOfGate = outOfGate;
        }

        if (mOutOfGate == 1) {
            mTargetAngle = gate_dir - (s16)0x8000;
        } else if (mOutOfGate == 2) {
            mTargetAngle = pen_dir - (s16)0x8000;
        }
        int cowIn = checkCowIn(800.0f, 300.0f);
        if (cowIn == 1) {
            setProcess(&daCow_c::action_enter, false);
        } else {
            if (cowIn == 2) {
                targetSpeed = mSpeed * (mBoostSpeed / 1000.0f);
            }
            if (mRunTimer) {
                mShouldSetEffect = 1;
                acceleration = 4.0f;
                targetSpeed = (mBoostSpeed / 1000.0f) * 45.0f;
                mRunDuration++;

                if (mRunDuration >= (u8)(cM_rndF(50.0f) + 100.0f)) {
                    if (daPy_getPlayerActorClass()->checkHorseRide() ||
                        daPy_getPlayerActorClass()->checkNowWolf())
                    {
                        mReadyToDash = false;
                        mRunDuration = 0;
                        setProcess(&daCow_c::action_angry, false);
                        return;
                    }
                }
            } else {
                mRunDuration = 0;
            }
            if (targetSpeed < 0.0f) {
                targetSpeed = 0.0f;
            }
            cLib_chaseF(&speedF, targetSpeed, acceleration);
            cLib_addCalcAngleS2(&current.angle.y, mTargetAngle, 8, 0x400);
            cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 8, 0x400);
            mSavedAngle.y = shape_angle.y;
            if (!speedF) {
                setProcess(&daCow_c::action_wait, false);
            }
        }
        break;
    }
    case daCow_c::Mode_2:
        break;
    case daCow_c::Mode_3:
        mNoNearCheckTimer = 30;
        mRunTimer = 0;
        break;
    }
}

bool daCow_c::checkCurringPen() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (player->current.pos.abs(pen_pos) < 2500.0f) {
        return false;
    }
    cXyz positions;

    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    int windowId = dComIfGp_getCameraWinID(fopCamM_GetParam(camera));
    scissor_class* scissor = dComIfGp_getWindow(windowId)->getScissor();
    mDoLib_project(&current.pos, &positions);

    if (positions.x > 0.0f && positions.x < scissor->width && positions.y > 0.0f &&
        positions.y < scissor->height)
    {
        return false;
    } else {
        return true;
    }
}

void daCow_c::setCowInCage() {
    for (int iSphere = 0; iSphere < N_COW_COLLIDERS; iSphere++) {
        mSph[iSphere].OffCoSetBit();
        mSph[iSphere].OnCoNoCrrBit();
        mCcStts.ClrCcMove();
    }
    mAcchCir.SetWall(0.0f, 0.0f);

    u8 cowIndex = cM_rndF(20.0f);
    if (l_CowRoomNo & (1 << cowIndex)) {
        for (int iCow = 0; iCow < 20; iCow++) {
            if (!(l_CowRoomNo & (1 << iCow))) {
                cowIndex = iCow;
                break;
            }
        }
    }

    if (cowIndex >= 20) {
        cowIndex = 19;
    }

    current.pos.x = l_CowRoomPosX[cowIndex];
    current.pos.z = l_CowRoomPosZ[cowIndex & 1];
    old.pos = current.pos;
    l_CowRoomNo |= 1 << cowIndex;

    if ((cowIndex & 1)) {
        mSavedAngle.y = 0;
        shape_angle.y = 0;
        current.angle.y = 0;
    } else {
        mSavedAngle.y = -0x8000;
        shape_angle.y = -0x8000;
        current.angle.y = -0x8000;
    }
}

void daCow_c::setEnterCount() {
    dTimer_createGetIn2D(2, current.pos);
    dMeter2Info_setNowCount(dMeter2Info_getNowCount() + 1);

    mTimer1 = 50;
    mCrazy = daCow_c::Crazy_Dash;
    mEnterTimerDone = false;

    if ((u32)dMeter2Info_getNowCount() == (u32)dMeter2Info_getMaxCount()) {
        mEnterTimerDone = true;
    }
}

void daCow_c::action_enter() {
    cXyz penDistance = current.pos - pen_pos;
    f32 penDistanceAbs = penDistance.absXZ();
    cXyz penDistanceNow = penDistance;
    mDoMtx_stack_c::YrotS(-pen_dir);
    mDoMtx_stack_c::multVecSR(&penDistanceNow, &penDistanceNow);

    switch (mMode) {
    case daCow_c::Mode_0:
        calcRunAnime(true);
        mMode = daCow_c::Mode_1;
        mCrazy = daCow_c::Crazy_Wait;
        for (int iSphere = 0; iSphere < N_COW_COLLIDERS; iSphere++) {
            mSph[iSphere].OffCoSetBit();
            mCcStts.ClrCcMove();
        }
        break;
    case daCow_c::Mode_1: {
        f32 fVar2 = 30.0f;
        switch (mCrazy) {
        case daCow_c::Crazy_Wait:
            calcRunAnime(false);

            if (penDistanceAbs < 80.0f) {
                setEnterCount();
            } else {
                mTargetAngle = getCowshedAngle();
                if (penDistanceAbs < 200.0f) {
                    cLib_addCalcAngleS2(&current.angle.y, mTargetAngle, 4, 0x1000);
                    cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 4, 0x1000);
                } else {
                    cLib_addCalcAngleS2(&current.angle.y, mTargetAngle, 8, 0x800);
                    cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 8, 0x800);
                }
                mSavedAngle.y = shape_angle.y;
            }
            break;
        case daCow_c::Crazy_Dash:
            calcRunAnime(false);
            mTargetAngle = pen_dir;
            cLib_addCalcAngleS2(&current.angle.y, mTargetAngle, 4, 0x800);
            cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 8, 0x800);
            mSavedAngle.y = shape_angle.y;
            if (penDistanceNow.z > 500.0f) {
                for (int iSphere = 0; iSphere < N_COW_COLLIDERS; iSphere++) {
                    mSph[iSphere].OnCoSetBit();
                }
                mSph[0].SetCoSPrm(0x19);
                if (!cLib_calcTimer(&mTimer1)) {
                    penDistanceNow.set(cM_rndFX(4.0f) * 40.0f, 0.0f,
                                       cM_rndFX(5.0f) * 40.0f + 2000.0f);
                    mDoMtx_stack_c::YrotS(pen_dir);
                    mDoMtx_stack_c::multVecSR(&penDistanceNow, &penDistanceNow);

                    mTarget = pen_pos + penDistanceNow;

                    setBck(daCow_c::Animation_WalkA, J3DFrameCtrl::EMode_LOOP, 12.0f, 1.0f);
                    speedF = 3.0f;
                    mCrazy = daCow_c::Crazy_BeforeCatch;

                    if ((u32)dMeter2Info_getNowCount() == (u32)dMeter2Info_getMaxCount() &&
                        mEnterTimerDone)
                    {
                        daNpc_Aru_c* aru;
                        fopAcM_SearchByName(PROC_NPC_ARU, (fopAc_ac_c**)&aru);
                        if (aru) {
                            aru->setLastIn();
                        }
                    }
                }
            }
            break;
        case daCow_c::Crazy_BeforeCatch: {
            s16 targetAngle = cLib_targetAngleY(&current.pos, &mTarget);
            cLib_addCalcAngleS2(&current.angle.y, targetAngle, 4, 0x100);
            cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 8, 0x800);

            mSavedAngle.y = shape_angle.y;

            if (current.pos.abs(mTarget) < 50.0f) {
                setBck(daCow_c::Animation_MooA, J3DFrameCtrl::EMode_NONE, 12.0f, 1.0f);
                speedF = 0;
                mCrazy = daCow_c::Crazy_Catch;
            }
            if (checkCurringPen()) {
                mCrazy = daCow_c::Crazy_Throw;
                setCowInCage();
            }
        } break;
        case daCow_c::Crazy_Catch:
            if (mpMorf->isStop()) {
                if (current.pos.abs(mTarget) > 100.0f) {
                    mCrazy = daCow_c::Crazy_BeforeCatch;
                    speedF = 3.0f;
                    setBck(daCow_c::Animation_WalkA, J3DFrameCtrl::EMode_LOOP, 12.0f, 1.0f);
                } else {
                    setBck(daCow_c::Animation_MooA, J3DFrameCtrl::EMode_NONE, 12.0f, 1.0f);
                }
            }
            if (checkCurringPen()) {
                mCrazy = daCow_c::Crazy_Throw;
                setCowInCage();
            }
            break;
        case daCow_c::Crazy_Throw:
            speedF = 0;
            mCowIn = 1;

            setProcess(&daCow_c::action_wait, false);
            mAcchCir.SetWall(100.0f, 110.0f);
            break;
        }

        // todo: this seems weird
        if (mCrazy <= daCow_c::Crazy_Dash) {
            m_near_dist = 200.0f;
            m_view_angle = 0;
            m_view_angle_wide = 0x2000;
            fpcM_Search(s_near_cow, this);

            if (m_near_dist < 200.0f) {
                fVar2 = 2.0f;
            }
            if (fVar2 > 30.0f) {
                fVar2 -= 1.0f;
            } else if (fVar2 < 0.0f) {
                fVar2 = 2.0f;
            }
            cLib_chaseF(&speedF, fVar2, 1.5f);
        }
    } break;
    case daCow_c::Mode_3:
        break;
    }

    for (int iSphere = 0; iSphere < N_COW_COLLIDERS; iSphere++) {
        mSph[iSphere].OffTgSetBit();
    }
}

bool daCow_c::isAngry() {
    if (checkProcess(&daCow_c::action_angry) &&
        (mCrazy == daCow_c::Crazy_Wait || mCrazy == daCow_c::Crazy_Dash ||
         mCrazy == daCow_c::Crazy_Attack))
    {
        return true;
    } else {
        return false;
    }
}

bool daCow_c::isGuardFad() {
    if (checkProcess(&daCow_c::action_wolf)) {
        return true;
    } else if ((u32)daPy_getPlayerActorClass()->checkNowWolf() != (u32)0) {
        return isAngry();
    } else {
        return false;
    }
}

void daCow_c::setAngryHit() {
    if (isAngry()) {
        mTargetAngle = mSavedAngle.y - (s16)0x8000;
        speedF = 0.0f;
        current.angle.y = mSavedAngle.y;
        calcRunAnime(true);

        if (!daPy_getPlayerActorClass()->checkHorseRide() &&
            (u32)daPy_getPlayerActorClass()->checkNowWolf() == (u32)0)
        {
            mCrazy = daCow_c::Crazy_Catch;
        } else {
            mCrazy = daCow_c::Crazy_Throw;
            mTimer1 = 30;
            mTargetRedTev = 0.0f;
            mChangeRedTev = 2;
        }
    }
}

bool daCow_c::checkBeforeBgAngry(s16 angle) {
    checkBeforeBg();

    if (mBoostSpeed < 1000.0f) {
        s16 angleDistance = cLib_distanceAngleS(mBoostAngle, mSavedAngle.y);
        if (mBoostSpeed < (fabsf(cM_ssin(angleDistance) * 250.0f) + 200.0f) &&
            angleDistance >= angle)
        {
            speedF = 0.0f;
            return true;
        }
    }
    return false;
}

void daCow_c::setRedTev() {
    if (mChangeRedTev < 2) {
        if (!mChangeRedTev) {
            if (mRedTev == 1.0f) {
                mTargetRedTev = 0.0f;
                mChangeRedTev = 1;
            }
        } else if (mRedTev == 0.0f) {
            mTargetRedTev = 1.0f;
            mChangeRedTev = 0;
        }
    }
}

void daCow_c::setAngryTurn() {
    setBck(daCow_c::Animation_Jump, J3DFrameCtrl::EMode_NONE, 0.0f, 1.0f);
    speedF = mSpeed;
    mCrazy = daCow_c::Crazy_Attack;
}

void daCow_c::action_angry() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    f32 playerDistance = fopAcM_searchPlayerDistance(this);
    s16 playerAngle = fopAcM_searchPlayerAngleY(this);

    s16 targetZ = 0;
    switch (mMode) {
    case daCow_c::Mode_0:
        calcRunAnime(true);
        mMode = daCow_c::Mode_1;
        if (mReadyToDash) {
            mCrazy = daCow_c::Crazy_Dash;
        } else {
            mCrazy = daCow_c::Crazy_Wait;
            mTimer1 = 20;
            mReadyToDash = true;
        }
        mTimer10 = 200;
        mTargetRedTev = 1.0f;
        mChangeRedTev = 0;
        if (!mRunDuration) {
            speedF = 75.0f;
        } else {
            speedF = 60.0f;
        }

        mSound.startCreatureVoice(Z2SE_GOAT_V_ANGRY, -1);
        mDealDamageTimer = 0;
        break;
    case daCow_c::Mode_1:
        setSeSnort();

        if (!player->checkHorseRide()) {
            attention_info.flags |= fopAc_AttnFlag_LOCK_e;
        } else {
            attention_info.flags &= ~fopAc_AttnFlag_LOCK_e;
        }
        if (mCrazy == daCow_c::Crazy_Dash) {
            setCarryStatus();
        }
        if (checkThrow()) {
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            mWillGetAngry = true;
            return;
        }
        if (!mDealDamageTimer) {
            if (!player->checkHorseRide() && mSph[0].ChkCoHit() &&
                fopAcM_GetName(mSph[0].GetCoHitObj()->GetAc()) == PROC_ALINK)
            {
                // if touched link who is not on a horse
                s16 sangle = shape_angle.y;
                cXyz pos = daPy_getPlayerActorClass()->current.pos;
                pos.y += 100.0f;
                cXyz pos2;
                mDoMtx_stack_c::transS(pos);
                mDoMtx_stack_c::YrotM(shape_angle.y);
                mDoMtx_stack_c::transM(0.0f, 0.0f, 200.0f);
                mDoMtx_stack_c::multVecZero(&pos2);

                dBgS_LinChk linkChck;
                linkChck.Set(&pos, &pos2, this);

                if (dComIfG_Bgsp().LineCross(&linkChck)) {
                    sangle = shape_angle.y - (s16)0x8000;
                }

                mDealDamageTimer = 30;
                daPy_getPlayerActorClass()->setThrowDamage(sangle, 35.0f, 40.0f, 0, 0, 0);
            }
        } else {
            mDealDamageTimer--;
        }
        if (mTimer10) {
            mTimer10--;
        } else if (mCrazy != daCow_c::Crazy_Attack) {
            setProcess(&daCow_c::action_run, false);
            mWillGetAngry = true;
            return;
        }

        TICK_TIMER(mForgetCowPTimer);
        TICK_TIMER(mTimer1);
        setRedTev();
        if (checkCowInOwn(0x4000)) {
            return;
        }

        if (mRunDuration) {
            if (checkOutOfGate(daPy_getPlayerActorClass()->current.pos) ||
                checkOutOfGate(current.pos))
            {
                setProcess(&daCow_c::action_run, false);
                mWillGetAngry = true;
                return;
            }
        }

        f32 targetSpeed;

        switch (mCrazy) {
        case daCow_c::Crazy_Wait:
            calcRunAnime(false);
            mShouldSetEffect = 1;
            targetZ = 0x2000;
            if (checkBeforeBgAngry(0)) {
                current.angle.y = mSavedAngle.y;
                mCrazy = daCow_c::Crazy_BeforeCatch;
            } else if (mTimer1 == 0) {
                mCrazy = daCow_c::Crazy_Dash;
            }
            cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 8, 0x800);
            mSavedAngle.y = shape_angle.y;
            break;
        case daCow_c::Crazy_Dash: {
            calcRunAnime(false);
            mShouldSetEffect = 1;
            targetZ = 0x2000;
            targetSpeed = 50.0f;
            if (player->getSpeedF() >= 15.0f) {
                targetSpeed = player->getSpeedF() + 35.0f;

                if (!mRunDuration) {
                    if (targetSpeed > 75.0f) {
                        targetSpeed = 75.0f;
                    }
                } else {
                    if (targetSpeed > 60.0f) {
                        targetSpeed = 60.0f;
                    }
                }
            }

            cLib_chaseF(&speedF, targetSpeed, 4.0f);

            if (checkBeforeBgAngry(0x6000)) {
                current.angle.y = mSavedAngle.y;
                mCrazy = daCow_c::Crazy_BeforeCatch;
                return;
            }
            if (mForgetCowPTimer) {
                mTargetAngle = current.angle.y;
            } else {
                mTargetAngle = playerAngle;
                s16 angleToPlayer = cLib_distanceAngleS(playerAngle, mSavedAngle.y);

                if (player->getSpeedF() > 5.0f) {
                    if (playerDistance < 350.0f) {
                        mForgetCowPTimer = 10;
                    } else if ((playerDistance < 1200.0f) && angleToPlayer >= 0x4000) {
                        setAngryTurn();
                        return;
                    }
                } else if (playerDistance < 500.0f) {
                    mForgetCowPTimer = 35;
                } else if ((playerDistance >= 1500.0f) && angleToPlayer >= 0x5800) {
                    current.angle.y = mSavedAngle.y;
                    mCrazy = daCow_c::Crazy_BeforeCatch;
                    return;
                }
            }

            int lockedOn = false;
            if (mRunDuration && mForgetCowPTimer && dComIfGp_getAttention()->LockonTruth() &&
                (dComIfGp_getAttention()->LockonTarget(0) == this) &&
                (s16)cLib_distanceAngleS(playerAngle, mSavedAngle.y) < 0x800)
            {
                mTargetAngle = playerAngle;
                lockedOn = true;
            }

            if (lockedOn) {
                cLib_chaseAngleS(&current.angle.y, mTargetAngle, 0x800);
            } else {
                cLib_addCalcAngleS2(&current.angle.y, mTargetAngle, 0x10, 0x400);
            }
            cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 4, 0x800);
            mSavedAngle.y = shape_angle.y;

            break;
        }
        case daCow_c::Crazy_BeforeCatch:
            calcRunAnime(false);
            if (checkBeforeBgAngry(0)) {
                speedF = 0.0f;
            }
            if (cLib_chaseF(&speedF, 0.0f, 4.0f)) {
                mCrazy = daCow_c::Crazy_Catch;
                mTargetAngle = playerAngle;
                current.angle.y = mSavedAngle.y;
            }
            break;
        case daCow_c::Crazy_Throw:
            calcRunAnime(false);
            if (mTimer1) {
                break;
            }
        case daCow_c::Crazy_Catch: {
            calcRunAnime(false);
            speedF = 15.0f;
            cLib_addCalcAngleS2(&current.angle.y, mTargetAngle, 8, 0x400);
            mSavedAngle.y = shape_angle.y = current.angle.y;
            setBodyAngle(mTargetAngle);
            s32 angleDist = cLib_distanceAngleS(mTargetAngle, mSavedAngle.y);
            if (angleDist < 0x200 && mJoint8Offset.y < 0x200) {
                if (mCrazy == daCow_c::Crazy_Throw) {
                    setProcess(&daCow_c::action_run, false);
                } else {
                    mCrazy = daCow_c::Crazy_Dash;
                    mJoint1Offset.y = 0;
                    mJoint8Offset.y = 0;
                }
            }
            break;
        }
        case daCow_c::Crazy_Attack:
            mpMorf->setPlaySpeed(1.0f);

            playerAngle = fopAcM_searchPlayerAngleY(this);
            cLib_chaseAngleS(&mSavedAngle.y, playerAngle, 0x800);

            targetZ = mSavedAngle.y;
            shape_angle.y = targetZ;
            current.angle.y = targetZ;

            if (mpMorf->isStop()) {
                mCrazy = daCow_c::Crazy_Dash;
                calcRunAnime(true);
            }
            return;
        }

        cLib_chaseS(&mJoint8Offset.z, targetZ, 0x400);
        break;
    case daCow_c::Mode_2:
        break;
    case daCow_c::Mode_3:
        mForgetCowPTimer = 0;
        mTimer1 = 0;
        mTargetRedTev = 0.0f;
        mJoint8Offset.z = 0;
        mJoint1Offset.y = 0;
        mJoint8Offset.y = 0;

        attention_info.flags &= ~fopAc_AttnFlag_LOCK_e;
        break;
    }
}

void daCow_c::calcCatchPos(f32 distance, BOOL someBool) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    s16 offsetAngle = player->shape_angle.y - (s16)0x8000;
    f32 playerPosY = player->current.pos.y;
    f32 diff = playerPosY - mGroundHeight;
    f32 abs = current.pos.absXZ(player->current.pos);
    int angle = cM_atan2s(abs, diff);

    cXyz catchPos(0.0f, distance * cM_scos(angle), distance * cM_ssin(angle));
    mSavedAngle.x = shape_angle.x = angle - 0x4000;

    if (someBool) {
        cLib_addCalcAngleS(&shape_angle.y, offsetAngle, 8, 0x400, 0x100);
        mSavedAngle.y = current.angle.y = shape_angle.y;
        cXyz* pos = &daPy_getPlayerActorClass()->current.pos;
        cXyz target;
        cLib_offsetPos(&target, pos, offsetAngle, &catchPos);
        cLib_chasePos(&current.pos, target, 30.0f);
    } else {
        cXyz* pos = &daPy_getPlayerActorClass()->current.pos;
        cLib_offsetPos(&current.pos, pos, offsetAngle, &catchPos);
        mSavedAngle.y = current.angle.y = shape_angle.y = offsetAngle;
    }
}

void daCow_c::executeCrazyWait() {
    if (GET_FLAG(daCow_c::Flag_CrazyReady)) {
        CLEAR_FLAG(daCow_c::Flag_CrazyReady);
    }
    if (GET_FLAG(daCow_c::Flag_CrazyReadyDrawOn)) {
        mDrawOff = false;
        CLEAR_FLAG(daCow_c::Flag_CrazyReadyDrawOn);
    }
    if (GET_FLAG(daCow_c::Flag_CrazyGo)) {
        mCrazy = daCow_c::Crazy_Dash;
        speedF = 30.0f;
        setBck(daCow_c::Animation_Rush, J3DFrameCtrl::EMode_LOOP, 12.0f, 1.0f);
        mDrawOff = false;
        mFlags = 0;

        mAcchCir.SetWall(100.0f, 110.0f);
        mTimer1 = 30;
        fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x100);
    }
}

void daCow_c::executeCrazyDash() {
    mShouldSetEffect = 1;
    cXyz pathPointPosition = dPath_GetPnt(mPath, mPointIndex)->m_position;

    setSeSnort();
    setRushVibration(VIBMODE_S_POWER2);

    if (mTimer1 == 1) {
        mSound.startCreatureVoice(Z2SE_GOAT_V_ANGRY, -1);
    }
    if (mPointIndex == 4 || mPointIndex == 5) {
        cLib_chaseS(&mJoint8Offset.z, 0x1000, 0x400);

        if (mFlags) {
            if (GET_FLAG(daCow_c::Flag_CrazyBeforeCatch)) {
                initCrazyBeforeCatch(0);
                mPointIndex = 6;
                CLEAR_FLAG(daCow_c::Flag_CrazyBeforeCatch);
            } else if (GET_FLAG(daCow_c::Flag_CrazyCatch)) {
                initCrazyCatch(0);
                mPointIndex = 6;
                CLEAR_FLAG(daCow_c::Flag_CrazyCatch);
                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            } else if (GET_FLAG(daCow_c::Flag_CrazyDash)) {
                initCrazyAttack(0);
                mPointIndex = 6;
                CLEAR_FLAG(daCow_c::Flag_CrazyDash);
            }
        } else {
            if (mPointIndex == 4) {
                cXyz vec2 = dPath_GetPnt(mPath, mPointIndex - 1)->m_position;
                s16 sVar4 = cLib_targetAngleY(&vec2, &pathPointPosition);

                if (current.pos.abs(pathPointPosition) > 600.0f) {
                    mExecute = daCow_c::Execute_0;
                    s16 sVar5 =
                        cLib_targetAngleY(&vec2, &daPy_getPlayerActorClass()->current.pos) - sVar4;

                    cXyz vec3;
                    if (sVar5 < 0) {
                        vec3.set(200.0f, 0.0f, 0.0f);
                    } else {
                        vec3.set(-200.0f, 0.0f, 0.0f);
                    }
                    cLib_offsetPos(&pathPointPosition, &pathPointPosition, sVar4, &vec3);
                    mTarget = pathPointPosition;
                } else {
                    cLib_addCalcAngleS(&current.angle.y, sVar4, 0x10, 0x800, 0x100);
                    if (current.pos.abs(pathPointPosition) < 250.0f) {
                        mPointIndex++;
                    }
                    return;
                }
            } else {
                mTarget = pathPointPosition;
            }

            s16 sVar4 = cLib_targetAngleY(&current.pos, &mTarget);
            cLib_addCalcAngleS(&current.angle.y, sVar4, 0x10, 0x800, 0x100);
            if (current.pos.abs(pathPointPosition) < 250.0f) {
                mPointIndex++;
            }
        }

    } else {
        s16 sVar4 = cLib_targetAngleY(&current.pos, &pathPointPosition);
        cLib_addCalcAngleS(&current.angle.y, sVar4, 0x10, 0x800, 0x100);
        if (current.pos.abs(pathPointPosition) < 200.0f) {
            mPointIndex++;
            if (mPointIndex >= mPath->m_num) {
                if ((s16)mPath->m_nextID != -1) {
                    mPointIndex = 0;
                    mPath = dPath_GetRoomPath(mPath->m_nextID, fopAcM_GetRoomNo(this));
                    mCrazy = daCow_c::Crazy_Away;
                } else {
                    mCrazy = daCow_c::Crazy_End;
                    speedF = 0.0f;
                }
            }
        }
    }
    cLib_addCalcAngleS(&shape_angle.y, current.angle.y, 8, 0x400, 0x100);
    mSavedAngle.y = shape_angle.y;
}

void daCow_c::initCrazyBeforeCatch(int _unused) {
    mCrazy = daCow_c::Crazy_BeforeCatch;
    speedF = 0.0f;
    mJoint8Offset.z = 0;
    mIsCrazy = true;
    gravity = 0.0f;
}

void daCow_c::executeCrazyBeforeCatch() {
    calcCatchPos(-220.0f, 1);
    if (GET_FLAG(daCow_c::Flag_CrazyCatch)) {
        initCrazyCatch(0);
        CLEAR_FLAG(daCow_c::Flag_CrazyCatch);
        dComIfGp_getVibration().StartShock(VIBMODE_S_POWER8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
    } else if (GET_FLAG(daCow_c::Flag_CrazyDash)) {
        if (!daPy_getPlayerActorClass()->speedF) {
            initCrazyAttack(0);
        } else {
            initCrazyAttack(1);
        }
        CLEAR_FLAG(daCow_c::Flag_CrazyDash);
    }
}

void daCow_c::initCrazyCatch(int _unused) {
    setBck(daCow_c::Animation_Shock, J3DFrameCtrl::EMode_NONE, 0.0f, 1.0f);
    mCrazy = daCow_c::Crazy_Catch;
    speedF = 0.0f;
    mJoint8Offset.z = 0;
    mExecute = daCow_c::Execute_0;
    calcCatchPos(-220.0f, 1);

    for (int iSphere = 0; iSphere < N_COW_COLLIDERS; iSphere++) {
        mSph[iSphere].OffCoSetBit();
        mCcStts.ClrCcMove();
    }

    mIsCrazy = true;
    gravity = 0.0f;
}

void daCow_c::executeCrazyCatch() {
    f32 catchDistance = -220.0f;

    switch (mExecute) {
    case daCow_c::Execute_0:
    case daCow_c::Execute_1:
        mShouldSetEffect = 1;
        if (mExecute == daCow_c::Execute_0 && mpMorf->isStop()) {
            setBck(daCow_c::Animation_Catch, J3DFrameCtrl::EMode_LOOP, 0.0f, 1.0f);
            mExecute = daCow_c::Execute_1;
        }

        if (!daPy_getPlayerActorClass()->speedF) {
            setBck(daCow_c::Animation_Press, J3DFrameCtrl::EMode_NONE, 0.0f, 1.0f);
            mExecute = daCow_c::Execute_2;
        }
        break;
    case daCow_c::Execute_2:
        catchDistance = -260.0f;
        if (mpMorf->isStop()) {
            setBck(daCow_c::Animation_PressWB, J3DFrameCtrl::EMode_LOOP, 0.0f, 1.0f);
            mTimer1 = 60;
            mExecute = daCow_c::Execute_4;
        }
        break;
    case daCow_c::Execute_3:
    case daCow_c::Execute_4:
        catchDistance = -260.0f;

        if (!mTimer1) {
            if (mExecute == daCow_c::Execute_3) {
                setBck(daCow_c::Animation_PressWB, J3DFrameCtrl::EMode_LOOP, 0.0f, 1.0f);
                mTimer1 = 60;
                mExecute = daCow_c::Execute_4;
            } else {
                setBck(daCow_c::Animation_PressWA, J3DFrameCtrl::EMode_LOOP, 0.0f, 1.0f);
                mTimer1 = 60;
                mExecute = daCow_c::Execute_3;
            }
        }
        break;
    }

    if (mFlags) {
        if (GET_FLAG(daCow_c::Flag_CrazyThrowLeft)) {
            initCrazyThrow(0);
        } else if (GET_FLAG(daCow_c::Flag_CrazyThrowRight)) {
            initCrazyThrow(1);
        } else if (GET_FLAG(daCow_c::Flag_CrazyDash)) {
            if (!daPy_getPlayerActorClass()->speedF) {
                initCrazyAttack(0);
            } else {
                initCrazyAttack(1);
            }
        }
        mFlags = 0;
    } else {
        calcCatchPos(catchDistance, true);
    }
}

void daCow_c::initCrazyThrow(int nextAction) {
    if (nextAction != daCow_c::Action_Wait) {
        setBck(daCow_c::Animation_FallR, J3DFrameCtrl::EMode_NONE, 5.0f, 1.0f);
    } else {
        setBck(daCow_c::Animation_FallL, J3DFrameCtrl::EMode_NONE, 5.0f, 1.0f);
    }
    mAction = nextAction;

    mExecute = daCow_c::Execute_0;
    mJointIndex = 1;
    mCrazy = daCow_c::Crazy_Throw;
    gravity = 0.0f;
    speedF = 0.0f;
    speed.y = 0.0f;

    for (int iSphere = 0; iSphere < N_COW_COLLIDERS; iSphere++) {
        mSph[iSphere].OffCoSetBit();
        mCcStts.ClrCcMove();
    }
    calcCatchPos(-260.0f, 0);

    mTarget = current.pos;
    mIsCrazy = true;
    gravity = 0.0f;
}

void daCow_c::executeCrazyThrow() {
    dBgS_LinChk linChk;

    switch (mExecute) {
    case daCow_c::Execute_0:
        current.pos = mTarget;

        if (mpMorf->checkFrame(10.0f)) {
            mSound.startCreatureVoice(Z2SE_GOAT_V_THROWN, -1);
        }
        if (mpMorf->checkFrame(34.0f)) {
            mExecute = daCow_c::Execute_1;
            mJointIndex = 2;
            mIsCrazy = false;
            gravity = -4.0f;

            for (int iSphere = 0; iSphere < N_COW_COLLIDERS; iSphere++) {
                mSph[iSphere].OnCoSetBit();
            }

            if (mAction != daCow_c::Action_Wait) {
                current.angle.y = daPy_getPlayerActorClass()->shape_angle.y + 0x7000;
            } else {
                current.angle.y = daPy_getPlayerActorClass()->shape_angle.y - 0x6800;
            }
            speedF = 10.0f;
            speed.y = 10.0f;
        }
        break;
    case daCow_c::Execute_1:
        mTarget.y += 100.0f;

        linChk.Set(&mTarget, &current.pos, NULL);
        if (dComIfG_Bgsp().LineCross(&linChk)) {
            current.pos = linChk.GetCross();

            cM3dGPla plane;
            dComIfG_Bgsp().GetTriPla(linChk, &plane);

            cXyz* normal = plane.GetNP();
            current.pos.x += normal->x * 50.0f;
            current.pos.z += normal->z * 50.0f;
            speedF = 0.0f;
            old.pos = current.pos;
        }
        mExecute = daCow_c::Execute_2;

    case daCow_c::Execute_2:
        cLib_chaseAngleS(&mSavedAngle.x, 0, 0x800);
        mJointIndex = 3;

        if (mAcch.ChkGroundHit()) {
            shape_angle.x = mSavedAngle.x;

            mSound.startCreatureSound(Z2SE_CM_BODYFALL_M, 0, -1);
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER5, 0x1f, cXyz(0.0f, 1.0f, 0.0f));

            speed.y = 0.0f;
            mTimer1 = 90;
            mExecute = daCow_c::Execute_3;
            mJointIndex = 0;
            mShouldSetEffect = 2;

            if (mAction != daCow_c::Action_Wait) {
                setBck(daCow_c::Animation_DownR, J3DFrameCtrl::EMode_LOOP, 0.0f, 1.0f);
                mSavedAngle.y -= 0x7000;
                mThrowIntensity = -1000;
            } else {
                setBck(daCow_c::Animation_DownL, J3DFrameCtrl::EMode_LOOP, 0.0f, 1.0f);
                mSavedAngle.y += 0x7000;
                mThrowIntensity = 1000;
            }
        }
        break;
    case daCow_c::Execute_3:
        mSavedAngle.y += mThrowIntensity;
        cLib_chaseAngleS(&mThrowIntensity, 0, 30);
        if (cLib_chaseF(&speedF, 0.0f, 0.5f) && !mTimer1) {
            mExecute = daCow_c::Execute_4;
            if (mAction != daCow_c::Action_Wait) {
                setBck(daCow_c::Animation_GetupR, J3DFrameCtrl::EMode_NONE, 5.0f, 1.0f);
            } else {
                setBck(daCow_c::Animation_GetupL, J3DFrameCtrl::EMode_NONE, 5.0f, 1.0f);
            }
            shape_angle.y = mSavedAngle.y;
        }
        break;
    case daCow_c::Execute_4:
        if (mpMorf->isStop()) {
            if (mPrm0 != 3) {
                initCrazyBack(0);
            } else {
                setBck(daCow_c::Animation_WaitA, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
                mExecute = daCow_c::Execute_5;
                mTimer1 = 10;
            }
        }
        break;
    case daCow_c::Execute_5:
        if (!mTimer1) {
            initCrazyBack(0);
        }
    }
}

void daCow_c::initCrazyAttack(int playerMoving) {
    mJoint8Offset.z = 0;
    mCrazy = daCow_c::Crazy_Attack;
    if (playerMoving) {
        mAction = daCow_c::Action_NadeNade;
        speedF = 0.0f;
        setBck(daCow_c::Animation_Toss, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
    } else {
        mAction = daCow_c::Action_Wait;
        setBck(daCow_c::Animation_Shakeoff, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
    }
    for (int iSphere = 0; iSphere < N_COW_COLLIDERS; iSphere++) {
        mSph[iSphere].OnCoSetBit();
    }
    gravity = -4.0f;
}

void daCow_c::executeCrazyAttack() {
    switch (mAction) {
    case daCow_c::Action_Wait:
        cLib_chaseF(&speedF, 10.0f, 1.0f);
        if (mpMorf->checkFrame(2.0f)) {
            mSound.startCreatureVoice(Z2SE_GOAT_V_BREATH_SHAKE, -1);
        }
        if (mpMorf->isStop()) {
            initCrazyAway(0);
        }
        break;
    case daCow_c::Action_NadeNade:
        if (mpMorf->checkFrame(2.0f)) {
            mSound.startCreatureVoice(Z2SE_GOAT_V_BREATH_SHAKE, -1);
        }
        if (mpMorf->checkFrame(10.0f)) {
            if (mPrm0 == 3) {
                initCrazyAway(0);
            } else {
                setBck(daCow_c::Animation_ShakeA, J3DFrameCtrl::EMode_NONE, 7.0f, 1.0f);
                mAction = daCow_c::Action_Cry;
            }
        }
        break;
    case daCow_c::Action_Cry:
        if (mpMorf->isStop()) {
            initCrazyAway(0);
        }
        break;
    }
}

void daCow_c::initCrazyAway(int _unused) {
    mCrazy = daCow_c::Crazy_Away;
    if (mPrm0 == 3) {
        setBck(daCow_c::Animation_Rush, J3DFrameCtrl::EMode_LOOP, 0.0f, 1.0f);
    }
    mSavedAngle.y = current.angle.y = shape_angle.y;
    gravity = -4.0f;
}

void daCow_c::executeCrazyAway() {
    setSeSnort();

    if (current.pos.abs(daPy_getPlayerActorClass()->current.pos) < 2500.0f) {
        setRushVibration(VIBMODE_S_POWER2);
    }

    mShouldSetEffect = 1;
    cLib_chaseF(&speedF, 30.0f, 2.0f);
    cLib_chaseS(&mJoint8Offset.z, 0x1000, 0x400);

    cXyz pointPos = dPath_GetPnt(mPath, mPointIndex)->m_position;
    cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &pointPos), 0x10, 0x800,
                       0x100);

    if (current.pos.abs(pointPos) < 200.0f) {
        mPointIndex++;

        if (mPointIndex >= mPath->m_num) {
            if ((s16)mPath->m_nextID != -1) {
                mPointIndex = 0;
                mPath = dPath_GetRoomPath(mPath->m_nextID, fopAcM_GetRoomNo(this));
            } else {
                mCrazy = daCow_c::Crazy_End;
                speedF = 0.0f;
            }
        }
    }
    cLib_addCalcAngleS(&shape_angle.y, current.angle.y, 8, 0x400, 0x100);
    mSavedAngle.y = shape_angle.y;
    if (GET_FLAG(daCow_c::Flag_CrazyReady)) {
        mCrazy = daCow_c::Crazy_End;
        speedF = 0.0f;
    }
}

void daCow_c::executeCrazyEnd() {
    mAcchCir.SetWall(0.0f, 0.0f);
    mDrawOff = true;
    mMode = daCow_c::Mode_0;
    mPath = dPath_GetRoomPath(COW_INDEX, fopAcM_GetRoomNo(this));
}

void daCow_c::initCrazyBack(int _unused) {
    if (mPrm0 == 3) {
        setBck(daCow_c::Animation_WalkDejected, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
        u8 pathIndex = COW_INDEX;
        if (COW_INDEX != (u8)-1) {
            mPath = dPath_GetRoomPath(pathIndex, fopAcM_GetRoomNo(this));
            mPointIndex = 3;
        }
    }

    mTimer1 = 0;
    mAction = daCow_c::Action_Wait;
    current.angle.y = shape_angle.y = mSavedAngle.y;
    mCrazy = daCow_c::Crazy_Back;
}

void daCow_c::executeCrazyBack() {
    cXyz pointPos;
    s16 angle;

    switch (mAction) {
    case daCow_c::Action_Wait: {
        setActetcStatus();

        pointPos = dPath_GetPnt(mPath, mPointIndex)->m_position;
        angle = cLib_targetAngleY(&current.pos, &pointPos);
        cLib_addCalcAngleS(&current.angle.y, angle, 0x10, 0x100, 0x80);
        if (speedF > 3.0f) {
            cLib_chaseF(&speedF, 2.0f, 3.0f);
        } else {
            cLib_chaseF(&speedF, 2.0f, 1.0f);
        }
        cLib_addCalcAngleS(&shape_angle.y, current.angle.y, 8, 0x100, 0x80);
        mSavedAngle.y = shape_angle.y;
        setBodyAngle(angle);

        if (current.pos.abs(pointPos) < 300.0f) {
            mPointIndex -= 1;
            if (mPointIndex < 0) {
                speedF = 0.0f;
                mAction = daCow_c::Action_3;
                mTargetAngle = mSavedAngle.y - 0x2000;
            }
        }
        if (checkNadeNade()) {
            setBck(daCow_c::Animation_WaitA, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
            mAction = daCow_c::Action_NadeNade;
            speedF = 0.0f;
        }
    } break;
    case daCow_c::Action_NadeNade:
        if (checkNadeNadeFinish()) {
            setBck(daCow_c::Animation_MooA, J3DFrameCtrl::EMode_NONE, 10.0f, 1.0f);
            mAction = daCow_c::Action_Cry;
            speedF = 0.0f;
        }
        break;
    case daCow_c::Action_Cry:
        if (mpMorf->checkFrame(35.0f)) {
            mSound.startCreatureVoice(Z2SE_GOAT_V_CRY, -1);
        }
        if (mpMorf->isStop()) {
            setBck(daCow_c::Animation_WalkDejected, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
            mAction = daCow_c::Action_Wait;
        }
        break;
    case daCow_c::Action_3:
        if (mpMorf->checkFrame(11.0f)) {
            setBck(daCow_c::Animation_WaitA, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
            mAction = daCow_c::Action_4;
        }
        break;
    case daCow_c::Action_4:
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x100);
        if (fopAcM_CheckCondition(this, fopAcCnd_NODRAW_e)) {
            fopAcM_delete(this);
        }
        break;
    case daCow_c::Action_5:
        setBck(daCow_c::Animation_Toss, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
        mAction = daCow_c::Action_6;
    case daCow_c::Action_6:
        if (mpMorf->isStop()) {
            if (mPointIndex < 0) {
                setBck(daCow_c::Animation_WalkDejected, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
                mAction = daCow_c::Action_3;
            } else if (mPointIndex < 2) {
                setBck(daCow_c::Animation_WalkDejected, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
                mAction = daCow_c::Action_Wait;
            } else {
                calcRunAnime(true);
                mAction = daCow_c::Action_Running;
                mTimer1 = 30;
                speedF = 30.0f;
            }
        }
        break;
    case daCow_c::Action_Running: {
        pointPos = dPath_GetPnt(mPath, mPointIndex)->m_position;
        angle = cLib_targetAngleY(&current.pos, &pointPos);
        cLib_addCalcAngleS(&current.angle.y, angle, 0x10, 0x100, 0x80);

        if (mPointIndex < 2) {
            cLib_chaseF(&speedF, 10.0f, 1.0f);
        } else {
            cLib_chaseF(&speedF, 45.0f, 1.0f);
        }
        cLib_addCalcAngleS(&shape_angle.y, current.angle.y, 8, 0x100, 0x80);
        mSavedAngle.y = shape_angle.y;
        setBodyAngle(angle);
        calcRunAnime(false);

        if (current.pos.abs(pointPos) < 300.0f) {
            mPointIndex -= 1;
            if (mPointIndex < 1 && !mTimer1) {
                setBck(daCow_c::Animation_WalkDejected, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
                mAction = daCow_c::Action_Wait;
            }
        }
        break;
    }
    }
}

void daCow_c::action_crazy() {
    cXyz acStack_28;
    s16 angle;

    switch (mMode) {
    case daCow_c::Mode_0:
        mChangeRedTev = 0;
        mPointIndex = 0;

        current.pos = dPath_GetPnt(mPath, mPointIndex)->m_position;
        mPointIndex++;
        acStack_28 = dPath_GetPnt(mPath, mPointIndex)->m_position;

        angle = cLib_targetAngleY(&current.pos, &acStack_28);
        current.angle.y = angle;
        shape_angle.y = angle;
        mSavedAngle.y = angle;

        mCrazy = daCow_c::Crazy_Wait;
        speedF = 0.0f;
        mDrawOff = true;

        dComIfGoat_SetThrow(this);
        mMode = daCow_c::Mode_1;
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x100);
        break;
    case daCow_c::Mode_1:
        TICK_TIMER(mForgetCowPTimer);
        TICK_TIMER(mTimer1);

        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
        if (dComIfGp_event_runCheck() &&
            strcmp(dComIfGp_getEventManager().getRunEventName(), "WILDGOAT") &&
            strcmp(dComIfGp_getEventManager().getRunEventName(), "WILDGOAT_SUCCESS") &&
            strcmp(dComIfGp_getEventManager().getRunEventName(), "WILDGOAT_FAILURE"))
        {
            fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x4000);
        }

        switch (mCrazy) {
        case daCow_c::Crazy_Wait:
            executeCrazyWait();
            break;
        case daCow_c::Crazy_Dash:
            executeCrazyDash();
            break;
        case daCow_c::Crazy_BeforeCatch:
            executeCrazyBeforeCatch();
            break;
        case daCow_c::Crazy_Catch:
            executeCrazyCatch();
            break;
        case daCow_c::Crazy_Throw:
            executeCrazyThrow();
            break;
        case daCow_c::Crazy_Attack:
            executeCrazyAttack();
            break;
        case daCow_c::Crazy_Away:
            executeCrazyAway();
            break;
        case daCow_c::Crazy_End:
            executeCrazyEnd();
            break;
        case daCow_c::Crazy_Back:
            setGroundAngle();
            executeCrazyBack();
        }
        if (mCrazy != daCow_c::Crazy_Back) {
            for (int iSphere = 0; iSphere < N_COW_COLLIDERS; iSphere++) {
                mSph[iSphere].OffTgSetBit();
            }
        }
    case daCow_c::Mode_2:
        break;
    case daCow_c::Mode_3:
        dComIfGoat_SetThrow(0);
        mDrawOff = false;
        mTargetRedTev = 0;
        mJoint8Offset.z = 0;
        mJoint1Offset.y = 0;
        mJoint8Offset.y = 0;
    }
}

void daCow_c::executeCrazyBack2() {
    if (checkOutOfGate(daPy_getPlayerActorClass()->current.pos) || checkOutOfGate(current.pos)) {
        setProcess(&daCow_c::action_run, false);
        mWillGetAngry = true;
        return;
    }

    if (checkCowInOwn(0x8000)) {
        return;
    }

    if (!mWillGetAngry) {
        mReadyToDash = false;
        mRunDuration = 1;
        setProcess(&daCow_c::action_angry, false);
        return;
    }

    switch (mAction) {
    case daCow_c::Action_Wait:
        setBck(daCow_c::Animation_WalkDejected, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
        mAction = daCow_c::Action_NadeNade;
        mTimer1 = 600;
    case daCow_c::Action_NadeNade: {
        setActetcStatus();
        s16 targetAngle = cLib_targetAngleY(&current.pos, &home.pos);
        cLib_addCalcAngleS(&current.angle.y, targetAngle, 0x10, 0x100, 0x80);
        cLib_chaseF(&speedF, 2.0f, 1.0f);
        cLib_addCalcAngleS(&shape_angle.y, current.angle.y, 8, 0x100, 0x800);
        mSavedAngle.y = shape_angle.y;
        setBodyAngle(targetAngle);

        if (current.pos.abs(home.pos) < 200.0f || !mTimer1) {
            speedF = 0.0f;
            setProcess(&daCow_c::action_moo, false);
        } else if (checkNadeNade()) {
            setBck(daCow_c::Animation_WaitA, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
            mAction = daCow_c::Action_Cry;
            speedF = 0.0f;
        }
    } break;
    case daCow_c::Action_Cry:
        if (checkNadeNadeFinish()) {
            setBck(daCow_c::Animation_MooA, J3DFrameCtrl::EMode_NONE, 10.0f, 1.0f);
            mAction = daCow_c::Action_3;
            speedF = 0.0f;
        }
        break;
    case daCow_c::Action_3:
        if (mpMorf->checkFrame(35.0f)) {
            mSound.startCreatureVoice(Z2SE_GOAT_V_CRY, -1);
        }
        if (mpMorf->isStop()) {
            setBck(daCow_c::Animation_WalkDejected, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
            mAction = daCow_c::Action_NadeNade;
        }
    }
}

void daCow_c::action_thrown() {
    daPy_py_c* player;

    switch (mMode) {
    case daCow_c::Mode_0:
        mCrazy = daCow_c::Crazy_BeforeCatch;
        mMode = daCow_c::Mode_1;
        dComIfGoat_SetThrow(this);
        mWillGetAngry = false;
        break;

    case daCow_c::Mode_1:
        TICK_TIMER(mForgetCowPTimer);
        TICK_TIMER(mTimer1);

        switch (mCrazy) {
        case 2:
            executeCrazyBeforeCatch();
            break;
        case 3:
            executeCrazyCatch();
            break;
        case 4:
            executeCrazyThrow();
            break;
        case 5:
            executeCrazyAttack();
            break;
        case 6:
            player = daPy_getPlayerActorClass();
            if (checkOutOfGate(player->current.pos) || checkOutOfGate(current.pos)) {
                setProcess(&daCow_c::action_run, false);
                mWillGetAngry = true;
                return;
            }
            if (checkCowInOwn(0x8000)) {
                return;
            }
            if (checkBck(0x15)) {
                setProcess(&daCow_c::action_wait, true);
            } else {
                setProcess(&daCow_c::action_wait, false);
            }
            break;
        case 8:
            executeCrazyBack2();
        }
        if (mCrazy != daCow_c::Crazy_Back) {
            for (int iSphere = 0; iSphere < N_COW_COLLIDERS; iSphere++) {
                mSph[iSphere].OffTgSetBit();
            }
        }
        break;
    case daCow_c::Mode_3:
        mJoint8Offset.z = 0;
        mJoint1Offset.y = 0;
        mJoint8Offset.y = 0;
        dComIfGoat_SetThrow(0);
    }
}

bool daCow_c::checkWolfBusters() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (!player->checkNowWolf()) {
        return false;
    }

    daNpc_Aru_c* aru;
    fopAcM_SearchByName(PROC_NPC_ARU, (fopAc_ac_c**)&aru);
    if (!aru) {
        return false;
    }

    if (checkOutOfGate(aru->current.pos)) {
        return false;
    }

    if (cM_rnd() < 0.9f) {
        return false;
    }

    for (int iWolfBuster = 0; iWolfBuster < N_WOLF_BUSTERS; iWolfBuster = iWolfBuster + 1) {
        if (gWolfBustersID[iWolfBuster] == -1) {
            gWolfBustersID[iWolfBuster] = fopAcM_GetID(this);
            setProcess(&daCow_c::action_wolf, false);
            return true;
        }
    }
    return false;
}

void daCow_c::action_wolf() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    daNpc_Aru_c* aru;
    fopAcM_SearchByName(PROC_NPC_ARU, (fopAc_ac_c**)&aru);
    if (!aru) {
        return;
    }

    cXyz aruPos = aru->current.pos;
    s16 aruAngle = cLib_targetAngleY(&current.pos, &aru->current.pos);

    switch (mMode) {
    case daCow_c::Mode_0:
        mMode = daCow_c::Mode_1;
        mCrazy = daCow_c::Crazy_Wait;
        calcRunAnime(true);
        attention_info.flags |= fopAc_AttnFlag_LOCK_e;
        mSound.startCreatureVoice(Z2SE_GOAT_V_ANGRY, -1);
        mTimer10 = cM_rndF(90.0f) + 90.0f;
        break;
    case daCow_c::Mode_1:
        TICK_TIMER(mTimer1);
        TICK_TIMER(mTimer10);

        calcRunAnime(false);
        if (!player->checkNowWolf() || checkOutOfGate(aru->current.pos)) {
            setProcess(&daCow_c::action_run, false);
            mWillGetAngry = true;
            return;
        }

        switch (mCrazy) {
        case daCow_c::Crazy_Wait:
            cLib_chaseF(&speedF, 36.0f, 1.0f);
            cLib_addCalcAngleS2(&current.angle.y, aruAngle, 8, 0x400);
            mSavedAngle.y = shape_angle.y = current.angle.y;
            setBodyAngle2(aruAngle);

            if (aruPos.absXZ(current.pos) < 500.0f) {
                mCrazy = daCow_c::Crazy_Dash;
            }
            break;
        case daCow_c::Crazy_Dash:
            if (cM_rnd() < 0.5f) {
                aruAngle = aruAngle + 0x3000;
            } else {
                aruAngle = aruAngle - 0x3000;
            }

            mTarget = aruPos;
            mTarget.x += cM_ssin(aruAngle) * 500.0f;
            mTarget.z += cM_scos(aruAngle) * 500.0f;
            mTargetAngle = cLib_targetAngleY(&current.pos, &mTarget);
            mCrazy = daCow_c::Crazy_BeforeCatch;
            mTimer1 = 150;
        case daCow_c::Crazy_BeforeCatch: {
            mTargetAngle = cLib_targetAngleY(&current.pos, &mTarget);

            f32 fVar13 = current.pos.absXZ(aru->current.pos) / 100.0f;
            if (fVar13 < 7.0f) {
                fVar13 = 7.0f;
            }

            cLib_chaseF(&speedF, fVar13, 1.0f);
            cLib_addCalcAngleS2(&current.angle.y, mTargetAngle, 8, 0x200);
            mSavedAngle.y = shape_angle.y = current.angle.y;
            setBodyAngle2(mTargetAngle);

            if (!mTimer1 || current.pos.absXZ(mTarget) < 100.0f || mAcch.ChkWallHit()) {
                mCrazy = daCow_c::Crazy_Dash;
            }

            if (player->current.pos.absXZ(aru->current.pos) < 700.0f) {
                if (!checkOutOfGate(player->current.pos)) {
                    s16 angleDifference = fopAcM_searchPlayerAngleY(this) - mSavedAngle.y;
                    if (abs(angleDifference) < 0x2000) {
                        mReadyToDash = false;
                        mRunDuration = 1;
                        setProcess(&daCow_c::action_angry, false);
                        return;
                    }
                }
            }
            if (!mTimer10) {
                mTimer10 = cM_rndF(90.0f) + 150.0f;
                if (!checkOutOfGate(player->current.pos)) {
                    m_angry_cow = false;
                    if (!fpcEx_Search(s_angry_cow2, this)) {
                        s16 angleDifference = fopAcM_searchPlayerAngleY(this) - mSavedAngle.y;
                        if (abs(angleDifference) < 0x2000) {
                            mReadyToDash = false;
                            mRunDuration = 1;
                            setProcess(&daCow_c::action_angry, false);
                            return;
                        }
                    }
                }
            }
        } break;
        }
        cLib_chaseS(&mJoint8Offset.z, 0, 0x400);
        break;
    case daCow_c::Mode_2:
        break;
    case daCow_c::Mode_3:
        mTimer10 = 0;
        mForgetCowPTimer = 0;
        mTimer1 = 0;
        mTargetRedTev = 0.0f;
        mJoint8Offset.z = 0;
        mJoint1Offset.y = 0;
        mJoint8Offset.y = 0;

        attention_info.flags &= ~fopAc_AttnFlag_LOCK_e;

        for (int iWolfBuster = 0; iWolfBuster < N_WOLF_BUSTERS; iWolfBuster++) {
            if (gWolfBustersID[iWolfBuster] == fopAcM_GetID(this)) {
                gWolfBustersID[iWolfBuster] = -1;
                break;
            }
        }
    }
}

void daCow_c::action_damage() {
    switch (mMode) {
    case daCow_c::Mode_0:
        setBck(daCow_c::Animation_Toss, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
        mMode = daCow_c::Mode_1;
        mTimer10 = 200;
        mTargetRedTev = 1.0f;
        mChangeRedTev = 0;
        speedF = 0.0f;
        break;
    case daCow_c::Mode_1:
        setRedTev();
        if (mpMorf->isStop()) {
            mReadyToDash = false;
            mRunDuration = 1;
            setProcess(&daCow_c::action_angry, false);
        }
        break;
    case daCow_c::Mode_2:
        return;
    case daCow_c::Mode_3:
        break;
    }
}

void daCow_c::action() {
    if (fopAcM_checkCarryNow(this)) {
        fopAcM_cancelCarryNow(this);
        speedF = 0.0f;
        speed.y = 0.0f;
    }
    TICK_TIMER(mNoNearCheckTimer);
    TICK_TIMER(mTakenDamageCounter);
    TICK_TIMER(mWaitTimer);

    cLib_chaseF(&mRedTev, mTargetRedTev, 0.1f);
    damage_check();
    s16 sVar2 = mSavedAngle.y;
    if (!getCowIn()) {
        for (int iSphere = 0; iSphere < N_COW_COLLIDERS; iSphere++) {
            mSph[iSphere].OnTgSetBit();
        }
    }
    if (mProcess) {
        (this->*mProcess)();
    }

    sVar2 = speedF * (sVar2 - mSavedAngle.y) * 0.2f;

    CLAMP(sVar2, -0x1000, 0x1000);

    cLib_addCalcAngleS2(&mSavedAngle.z, sVar2, 8, 0x800);
    dComIfGp_att_LookRequest(this, 1500.0f, 300.0f, -300.0f, 0x6000, 1);
}

void daCow_c::setMtx() {
    if (mpMorf) {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(mGroundTransform);
        mDoMtx_stack_c::ZXYrotM(mSavedAngle);
        mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
        mpMorf->modelCalc();
    }
}

void daCow_c::setAttnPos() {
    cXyz arg;
    cXyz pos;

    if (mpMorf) {
        if (attention_info.flags & fopAc_AttnFlag_ETC_e) {
            arg.set(0.0f, 0.0f, 0.0f);
            mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(11));
            mDoMtx_stack_c::multVec(&arg, &eyePos);
            pos = eyePos;
            attention_info.position.set(pos.x, pos.y + 30.0f, pos.z);
        } else {
            arg.set(18.0f, 30.0f, 0.0f);
            mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(9));
            mDoMtx_stack_c::multVec(&arg, &eyePos);
            pos = eyePos;
            attention_info.position.set(pos.x, pos.y + 80.0f, pos.z);
        }
    }
}

static f32 m_search_range;

void daCow_c::setCollisions() {
    J3DModel* pJVar1;
    Mtx* pMVar2;
    cCcS* pcVar3;
    cXyz acStack_28[2];

    if (!mDrawOff) {
        static cXyz headOfst(20.0f, 10.0f, 0.0f);
        static cXyz backBornOfst(60.0f, 20.0f, 0.0f);
        static cXyz waistOfst(-30.0f, 30.0f, 0.0f);

        cXyz ofstNow;

        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(9));
        mDoMtx_stack_c::multVec(&headOfst, &ofstNow);
        mSph[0].SetR(40.0f);
        mSph[0].SetC(ofstNow);
        dComIfG_Ccsp()->Set(&mSph[0]);

        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(1));
        mDoMtx_stack_c::multVec(&backBornOfst, &ofstNow);
        mSph[1].SetR(45.0f);
        mSph[1].SetC(ofstNow);
        dComIfG_Ccsp()->Set(&mSph[1]);

        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(0xb));
        mDoMtx_stack_c::multVec(&waistOfst, &ofstNow);
        mSph[2].SetR(45.0f);
        mSph[2].SetC(ofstNow);
        dComIfG_Ccsp()->Set(&mSph[2]);
    }
}

int daCow_c::Execute() {
    mCounter1++;
    mJointIndex = 0;

    attention_info.flags &= ~fopAc_AttnFlag_CARRY_e;
    attention_info.flags &= ~fopAc_AttnFlag_ETC_e;
    action();

    if (!mDrawOff) {
        fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
        mAcch.CrrPos(dComIfG_Bgsp());
        mGroundHeight = mAcch.GetGroundH();
        setEffect();
        mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

        if (!mAnimationInterval) {
            mpBtp->setPlaySpeed(1.0f);
            mAnimationInterval = cM_rndF(120.0f) + 60.0f;
            if (mAnimationInterval < 80) {
                mAnimationInterval = 10;
            }
        } else {
            mAnimationInterval--;
            if (mpBtp->isStop()) {
                mpBtp->setPlaySpeed(0.0f);
                mpBtp->setFrame(0.0f);
            }
        }

        mpBtp->play();
        setMtx();
        setAttnPos();
        setCollisions();
    } else {
        setMtx();
        setAttnPos();
    }
    return 1;
}

static int daCow_Execute(void* actor) {
    return static_cast<daCow_c*>(actor)->Execute();
}

int daCow_c::CreateHeap() {
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("Cow", 0x1f));

    JUT_ASSERT(3826, modelData);

    mpMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000,
                                  0x11020084);

    if (!mpMorf || !mpMorf->getModel()) {
        return cPhs_INIT_e;
    }

    mpMorf->getModel()->setUserArea((uintptr_t)this);

    for (u16 iJoint = 0; iJoint < modelData->getJointNum(); iJoint++) {
        if (iJoint == 1 || iJoint == 8 || iJoint == 0) {
            modelData->getJointNodePointer(iJoint)->setCallBack(daCow_c::ctrlJointCallBack);
        }
    }
    setBck(daCow_c::Animation_WaitA, J3DFrameCtrl::EMode_LOOP, 0.0f, 1.0f);

    mpBtp = new mDoExt_btpAnm();

    if (mpBtp == NULL) {
        return cPhs_ERROR_e;
    }

    J3DAnmTexPattern* pattern = static_cast<J3DAnmTexPattern*>(dComIfG_getObjectRes("Cow", 0x22));
    modelData = mpMorf->getModel()->getModelData();
    if (mpBtp->init(modelData, pattern, 1, 0, 1.0f, 0, -1)) {
        return cPhs_LOADING_e;
    } else {
        return cPhs_ERROR_e;
    }
}

int daCow_c::createHeapCallBack(fopAc_ac_c* actor) {
    return static_cast<daCow_c*>(actor)->CreateHeap();
}

u8 daCow_c::initialize() {
    fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
    mSound.init(&current.pos, &eyePos, 3, 1);

    eventInfo.setArchiveName("Cow");
    mAcchCir.SetWall(100.0f, 110.0f);

    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, &speed,
              fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));

    mCcStts.Init(220, 0, this);

    for (int iSphere = 0; iSphere < N_COW_COLLIDERS; iSphere = iSphere + 1) {
        mSph[iSphere].Set(cc_sph_src);  // todo
        mSph[iSphere].SetStts(&mCcStts);
    }

    fopAcM_setCullSizeBox(this, -100.0f, -100.0f, -200.0f, 100.0f, 250.0f, 200.0f);
    fopAcM_OnStatus(this, 0x8000000);

    mAcch.CrrPos(dComIfG_Bgsp());

    mGroundHeight = mAcch.GetGroundH();

    attention_info.distances[4] = 40;
    attention_info.distances[0] = 22;
    attention_info.distances[7] = 48;
    attention_info.flags = 0;
    mParticle.init(&mAcch, 60.0f, 200.0f);
    current.angle.set(0, home.angle.y, 0);
    shape_angle = mSavedAngle = current.angle;
    speedF = 0.0f;
    speed.set(0.0f, 0.0f, 0.0f);

    if (!mPrm0 && dComIfGs_isTmpBit(dSv_event_tmp_flag_c::T_0091)) {
        mPrm0 = 4;
        setCowInCage();
    }

    switch (mPrm0) {
    case 3: {
        u8 prm1 = COW_INDEX;
        if (prm1 != (u8)-1) {
            mPath = dPath_GetRoomPath(prm1, fopAcM_GetRoomNo(this));
            mPointIndex = 0;

            dPnt* point = dPath_GetPnt(mPath, mPointIndex);
            current.pos = point->m_position;

            setProcess(&daCow_c::action_crazy, false);
        }
    } break;
    case 4:
        mCowIn = 1;
    default:
        int rand = cM_rndF(4.0f) + fopAcM_GetID(this);
        int nextAction = rand % 4;
        if (nextAction == 1) {
            setProcess(&daCow_c::action_shake, false);
        } else if (nextAction == 2) {
            setProcess(&daCow_c::action_moo, false);
        } else if (nextAction == 3) {
            setProcess(&daCow_c::action_eat, false);
        } else {
            setProcess(&daCow_c::action_wait, false);
        }
    }

    mAcchCir.SetWallR(100.f);
    mAcchCir.SetWallH(110.f);
    gravity = -4.0f;

    f32 rand = cM_rnd();
    u8 iSpeed = 0;
    if (rand < 0.1f) {
        if (!(l_CowType & 1)) {
            l_CowType |= 1;
            iSpeed = 1;
        }
    } else {
        if (rand >= 0.9f && !cLib_checkBit<u32>(l_CowType, 2)) {
            l_CowType |= 2;
            iSpeed = 2;
        }
    }

    switch (iSpeed) {
    case 1:
        mAttentionDistance = 1300.0f;
        mSpeed = 35.0f;
        break;
    case 2:
        mAttentionDistance = 700.0f;
        mSpeed = 15.0f;
        break;
    default:
        mAttentionDistance = cM_rndFX(100.0f) + 1000.0f;
        mSpeed = cM_rndFX(5.0f) + 25.0f;
        break;
    }

    mISpeed = iSpeed;
    Execute();

    if (!mPrm0) {
        dMeter2Info_setNowCount(0);
        dMeter2Info_setMaxCount(dMeter2Info_getMaxCount() + 1);
    }
    return 1;
}

int daCow_c::create() {
    fopAcM_ct(this, daCow_c);

    mPrm0 = fopAcM_GetParam(this);
    if (mPrm0 == (u8)-1 || mPrm0 >= 5) {
        mPrm0 = 0;
    }

    switch (mPrm0) {
    case 2:
        setEnterCow20();
        mDrawOff = true;
        return cPhs_ERROR_e;
    case 1:
        setEnterCow10();
        mDrawOff = true;
        return cPhs_ERROR_e;
    default:
        int res = dComIfG_resLoad(&mPhase, "Cow");
        if (res == cPhs_COMPLEATE_e) {
            if (!fopAcM_entrySolidHeap(this, daCow_c::createHeapCallBack, 0x1df0)) {
                return cPhs_ERROR_e;
            }
            if (!initialize()) {
                return cPhs_ERROR_e;
            }
        }
        return res;
    }
}

static int daCow_Create(void* actor) {
    return static_cast<daCow_c*>(actor)->create();
}

int daCow_c::ctrlJoint(J3DJoint* joint, J3DModel* model) {
    int jointNo = joint->getJntNo();

    mDoMtx_stack_c::copy(model->getAnmMtx(jointNo));

    switch (jointNo) {
    case 1:
        mDoMtx_stack_c::YrotM(mJoint1Offset.y);
        break;
    case 8:
        mDoMtx_stack_c::ZrotM(mJoint8Offset.z);
        mDoMtx_stack_c::YrotM(mJoint8Offset.y);
        break;
    }

    model->setAnmMtx(jointNo, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), &J3DSys::mCurrentMtx[0]);

    cXyz currentMtx;

    if (!jointNo) {
        switch (mJointIndex) {
        case 1:
            mJointPos.set(J3DSys::mCurrentMtx[0][3], J3DSys::mCurrentMtx[1][3],
                          J3DSys::mCurrentMtx[2][3]);
            break;
        case 2: {
            currentMtx.set(J3DSys::mCurrentMtx[0][3], J3DSys::mCurrentMtx[1][3],
                           J3DSys::mCurrentMtx[2][3]);

            cXyz currentOffset = mJointPos - current.pos;
            currentMtx = mJointPos - currentMtx;
            current.pos -= currentMtx;
            mJointPos = currentOffset;
            mJointPos.y -= 50.0f;
        } break;
        case 3: {
            currentMtx = current.pos + mJointPos;
            J3DSys::mCurrentMtx[0][3] = currentMtx.x;
            J3DSys::mCurrentMtx[1][3] = currentMtx.y;
            J3DSys::mCurrentMtx[2][3] = currentMtx.z;
        } break;
        }
    }

    return 1;
}

int daCow_c::ctrlJointCallBack(J3DJoint* joint, int skip) {
    if (!skip) {
        J3DModel* model = j3dSys.getModel();
        daCow_c* cow = (daCow_c*)model->getUserArea();
        if (cow) {
            cow->ctrlJoint(joint, model);
        }
    }
    return 1;
}

int daCow_c::Draw() {
    if (mDrawOff) {
        return 1;
    }

    J3DModel* model = mpMorf->getModel();

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);

    mpBtp->entry(model->getModelData());
    mpMorf->entryDL();

    if (!strcmp(dComIfGp_getEventManager().getRunEventName(), "MAKI_OP")) {
        cXyz shadowPos;
        cXyz arg(0.0f, 0.0f, -20.0f);

        cLib_offsetPos(&shadowPos, &current.pos, current.angle.y, &arg);
        dComIfGd_setSimpleShadow(&current.pos, mGroundHeight, 90.0f, mAcch.m_gnd, 0, 1.0f,
                                 dDlst_shadowControl_c::getSimpleTex());

        arg.set(0.0f, 0.0f, 120.0f);

        cLib_offsetPos(&shadowPos, &current.pos, current.angle.y, &arg);
        dComIfGd_setSimpleShadow(&shadowPos, mGroundHeight, 50.0f, mAcch.m_gnd, 0, 1.0f,
                                 dDlst_shadowControl_c::getSimpleTex());

    } else {
        f32 fVar1 = 800.0f;
        if (checkProcess(&daCow_c::action_crazy) || checkProcess(&daCow_c::action_thrown)) {
            if (mCrazy == daCow_c::Crazy_Throw) {
                fVar1 = 1500.0f;
            }
        }
        mShadowKey = dComIfGd_setShadow(mShadowKey, 1, model, &current.pos, fVar1, 0.0f,
                                        current.pos.y, mGroundHeight, mAcch.m_gnd, &tevStr, 0, 1.0f,
                                        dDlst_shadowControl_c::getSimpleTex());
    }
    tevStr.TevColor.r = mRedTev * 50.0f;
    return 1;
}

static int daCow_Draw(void* actor) {
    return static_cast<daCow_c*>(actor)->Draw();
}

int daCow_c::Delete() {
    fopAcM_GetID(this);
    dComIfG_resDelete(&mPhase, "Cow");

    if (heap != NULL) {
        mSound.deleteObject();
    }
    return true;
}

static int daCow_Delete(void* actor) {
    return static_cast<daCow_c*>(actor)->Delete();
}

static int daCow_IsDelete(void* actor) {
    return true;
}

static actor_method_class daCow_MethodTable = {
    daCow_Create, daCow_Delete, daCow_Execute, daCow_IsDelete, daCow_Draw,
};

actor_process_profile_definition g_profile_COW = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_COW,                // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daCow_c),         // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    692,                     // mPriority
    &daCow_MethodTable,      // sub_method
    0x00040100,              // mStatus
    fopAc_NPC_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES;
