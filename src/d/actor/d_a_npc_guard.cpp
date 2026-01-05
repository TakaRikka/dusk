/**
 * @file d_a_npc_guard.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_npc_guard.h"
#include "d/d_bg_s.h"

int daNpcGuard_c::createHeap() {
    int rv = NpcCreate(m_type);

    if (rv) {
        rv = NULL != (mpModel = ObjCreate(mObjNum));
        if (!rv) {
            mpMorf->stopZelAnime();
        }
        if (m_type == 1) {
            J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("object", 0x11);
            JUT_ASSERT(125, modelData != NULL);

            mpModel2 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
            if (mpModel2 == NULL) {
                mpMorf->stopZelAnime();
                rv = 0;
            }
        }
    }

    return rv;
}

static int createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpcGuard_c*>(i_this)->createHeap();
}

daNpcGuard_c::actionFunc daNpcGuard_c::ActionTable[7][2] = {
    {&daNpcGuard_c::initPath, &daNpcGuard_c::executePath},
    {&daNpcGuard_c::initRun, &daNpcGuard_c::executeRun},
    {&daNpcGuard_c::initFightWait, &daNpcGuard_c::executeFightWait},
    {&daNpcGuard_c::initFightStep, &daNpcGuard_c::executeFightStep},
    {&daNpcGuard_c::initFightMenace, &daNpcGuard_c::executeFightMenace},
    {&daNpcGuard_c::initFear, &daNpcGuard_c::executeFear},
    {&daNpcGuard_c::initEscape, &daNpcGuard_c::executeEscape},
};

void daNpcGuard_c::setAction(daNpcGuard_c::Mode_e i_action) {
    JUT_ASSERT(286, i_action < MODE_MAX_e);

    mPrevActionIdx = mActionIdx;
    mActionIdx = i_action;
    mAction = ActionTable[mActionIdx];
    callInit();
}

void daNpcGuard_c::callInit() {
    JUT_ASSERT(328, mAction != NULL);
    (this->*mAction[0])();
}

void daNpcGuard_c::callExecute() {
    JUT_ASSERT(341, mAction != NULL);
    (this->*mAction[1])();
}

void daNpcGuard_c::initPath() {
    setAnm((J3DAnmTransformKey*)getAnmP(4, mObjNum), 1.0f, 12.0f, 2, 0, -1);
    field_0xd9c = 1;
}

void daNpcGuard_c::executePath() {
    if (m_path.isPath()) {
        if (m_path.checkPoint(current.pos, speedF)) {
            if (m_path.checkPathEnd(current.pos, speedF)) {
                if (!m_path.isClose()) {
                    setAction(MODE_FIGHT_WAIT_e);
                }
            }
            m_path.setNextPoint(current.pos);
        }
        if (field_0xd9c == 2 || field_0xd9c == 1) {
            cXyz targetPoint;
            m_path.getTargetPoint(&targetPoint);
            cLib_addCalcAngleS2(&shape_angle.y, cLib_targetAngleY(&current.pos, &targetPoint), 0xD,
                                0x600);

            if (current.pos.y != old.pos.y) {
                s16 polyAngle = 0x0;
                cM3dGPla plane;
                if (dComIfG_Bgsp().GetTriPla(mAcch.m_gnd, &plane)) {
                    polyAngle = fopAcM_getPolygonAngle(&plane, shape_angle.y);
                }
                if (polyAngle != 0x0) {
                    field_0xd94 = current.pos.y;
                }
            }
        }
        setAngle();
        pathMoveF();
        mAcch.CrrPos(dComIfG_Bgsp());
        f32 power = std::pow(500.0f, 2.0f);
        f32 playerDistXZ = fopAcM_searchPlayerDistanceXZ2(this);
        if (playerDistXZ < power) {
            setAction(MODE_FIGHT_WAIT_e);
        }
    }
}

void daNpcGuard_c::initRun() {
    setAnm((J3DAnmTransformKey*)getAnmP(4, mObjNum), 1.0f, 12.0f, 2, 0, -1);
    field_0xd9c = 1;
}

void daNpcGuard_c::executeRun() {
    field_0xda0 = fopAcM_searchPlayerAngleY(this);
    cLib_addCalcAngleS2(&shape_angle.y, field_0xda0, 0xD, 0x600);
    setAngle();
    f32 maxSpeed = Cd2_HIO_maxSpeed(m_type);
    f32 anmPlaySpeed = Cd2_HIO_anmPlaySpeed(m_type);
    cLib_chaseF(&field_0xd8c, 1.0f, 0.05f);
    setSpeed(0.7f, maxSpeed, &field_0xd90, 0);
    setSpeed(0.7f, maxSpeed, &speedF, 1);
    if (cLib_distanceAngleS(shape_angle.y, field_0xda0) >= 0x2000) {
        cLib_chaseF(&field_0xd8c, 0.0f, 0.05f);
        cLib_chaseF(&field_0xd90, 0.0f, 1.2f);
        cLib_chaseF(&speedF, 0.0f, 1.2f);
    }
    cXyz* ccMoveP = mStts.GetCCMoveP();
    if (ccMoveP != NULL) {
        *ccMoveP *= 0.25f;
    }
    fopAcM_posMoveF(this, ccMoveP);

    maxSpeed = field_0xd90 / maxSpeed;
    if (maxSpeed > 1.0f) {
        maxSpeed = 1.0f;
    }
    f32 playSpeed = anmPlaySpeed * maxSpeed;
    if (playSpeed < 0.6f) {
        playSpeed = 0.6f;
    }
    mpMorf->setPlaySpeed(playSpeed);
    mAcch.CrrPos(dComIfG_Bgsp());
    f32 power = std::pow(500.0f, 2.0f);
    f32 playerDistXZ = fopAcM_searchPlayerDistanceXZ2(this);
    if (playerDistXZ < power) {
        setAction(MODE_FIGHT_WAIT_e);
    }
}

void daNpcGuard_c::initFightWait() {
    setAnm((J3DAnmTransformKey*)getAnmP(42, mObjNum), 1.0f, 12.0f, 2, 0, -1);
    speedF = 0.0f;
    field_0xd98 = cLib_getRndValue(300.0f, 200.0f);
    if (fopAcM_searchPlayerDistanceXZ(this) - field_0xd98 < 0.0f) {
        mCitizen.playVoice(1);
    }
    field_0xd9c = 0;
}

void daNpcGuard_c::executeFightWait() {
    f32 playerDist = fopAcM_searchPlayerDistanceXZ(this);
    f32 dist = playerDist - field_0xd98;
    f32 distCopy = dist;
    cLib_addCalc2(&distCopy, 0.0f, 0.2f, 10.0f);
    speedF = dist - distCopy;
    field_0xda0 = fopAcM_searchPlayerAngleY(this);
    cLib_addCalcAngleS2(&shape_angle.y, field_0xda0, 0x3, 0x600);
    setAngle();

    cXyz* ccMoveP = mStts.GetCCMoveP();
    if (ccMoveP != NULL) {
        *ccMoveP *= 0.25f;
    }
    fopAcM_posMoveF(this, ccMoveP);
    mAcch.CrrPos(dComIfG_Bgsp());

    f32 power = std::pow(2.0f, 2.0f);
    if (current.pos.abs2XZ(old.pos) > power) {
        setAnm(mpAnmKey2, 1.0f, 12.0f, 2, 0, -1);
    } else {
        if (cLib_distanceAngleS(shape_angle.y, field_0xda0) > 0x300) {
            setAnm(mpAnmKey2, 1.0f, 12.0f, 2, 0, -1);
        } else {
            setAnm(mpAnmKey, 1.0f, 12.0f, 2, 0, -1);
        }
    }

    if (mCyl.ChkTgHit() || mSpheres[0].ChkTgHit() || mSpheres[1].ChkTgHit()) {
        setAction(MODE_FEAR_e);
    } else if (cLib_calcTimer(&field_0xd88) == 0) {
        setAction(MODE_FIGHT_MENACE_e);
    } else if (playerDist > 500.0f) {
        setAction(MODE_RUN_e);
    }
}

void daNpcGuard_c::initFightStep() {
    field_0xd9c = 0;
}

void daNpcGuard_c::executeFightStep() {
    if (fopAcM_searchPlayerDistanceXZ2(this) > std::pow(500.0f, 2.0f)) {
        setAction(MODE_RUN_e);
    }
}

void daNpcGuard_c::initFightMenace() {
    setAnm((J3DAnmTransformKey*)getAnmP(43, mObjNum), 1.0f, 12.0f, 0, 0, -1);
    mCitizen.playVoice(2);
    field_0xd88 = cLib_getRndValue(0x5A, 0x3C);
    field_0xd9c = 0;
}

void daNpcGuard_c::executeFightMenace() {
    field_0xda0 = fopAcM_searchPlayerAngleY(this);
    cLib_addCalcAngleS2(&shape_angle.y, field_0xda0, 0x3, 0x600);
    setAngle();
    if (mpMorf->isStop()) {
        setAction(MODE_FIGHT_WAIT_e);
    }
    if (fopAcM_searchPlayerDistanceXZ2(this) > std::pow(500.0f, 2.0f)) {
        setAction(MODE_RUN_e);
    }
}

void daNpcGuard_c::initFear() {
    setAnm((J3DAnmTransformKey*)getAnmP(46, 0), 1.0f, 4.0f, 2, 0, -1);
    field_0xd88 = cLib_getRndValue(0x96, 0x3C);
    speedF = 0.0f;
    field_0xd9c = 0;
}

void daNpcGuard_c::executeFear() {
    if (mpMorf->checkFrame(1.0f)) {
        mCitizen.playVoice(2);
    }

    cXyz* ccMoveP = mStts.GetCCMoveP();
    if (ccMoveP != NULL) {
        *ccMoveP *= 0.25f;
    }
    fopAcM_posMoveF(this, ccMoveP);
    mAcch.CrrPos(dComIfG_Bgsp());

    u8 cutType = daPy_getPlayerActorClass()->getCutType();
    if ((cutType == daPy_py_c::CUT_TYPE_WOLF_TURN_LEFT ||
         cutType == daPy_py_c::CUT_TYPE_WOLF_TURN_RIGHT) &&
        field_0xda2)
    {
        setAction(MODE_MAX_e);
    } else {
        if (cLib_calcTimer(&field_0xd88) == 0) {
            setAction(MODE_FIGHT_WAIT_e);
        }
    }
}

void daNpcGuard_c::initEscape() {
    setAnm((J3DAnmTransformKey*)getAnmP(4, 3), 2.0f, 8.0f, 2, 0, -1);
    mCitizen.playVoice(0);
    shape_angle.y = fopAcM_searchPlayerAngleY(this) + 0x8000;
    speedF = 20.0f;
    fopAcM_createItemFromTable(&current.pos, 7, -1, -1, NULL, 0, NULL, NULL, NULL, false);
    field_0xd9c = 1;
}

void daNpcGuard_c::executeEscape() {
    if (field_0xd9c != 0) {
        speedF = 20.0f;
        mpMorf->setPlaySpeed(2.0f);
        cLib_addCalcAngleS2(&shape_angle.y, fopAcM_searchPlayerAngleY(this) + 0x8000, 0xD, 0x600);
        if (current.pos.y != old.pos.y) {
            s16 polyAngle = 0x0;
            cM3dGPla plane;
            if (dComIfG_Bgsp().GetTriPla(mAcch.m_gnd, &plane)) {
                polyAngle = fopAcM_getPolygonAngle(&plane, shape_angle.y);
            }
            if (polyAngle != 0x0) {
                field_0xd94 = current.pos.y;
            }
        }
        setAngle();

        cXyz* ccMoveP = mStts.GetCCMoveP();
        if (ccMoveP != NULL) {
            *ccMoveP *= 0.25f;
        }
        fopAcM_posMoveF(this, ccMoveP);
        mAcch.CrrPos(dComIfG_Bgsp());

        if (mAcch.ChkWallHit() && current.pos.abs(old.pos) < 2.0f) {
            field_0xda5++;
            if (field_0xda5 > 30) {
                setAnm((J3DAnmTransformKey*)getAnmP(46, 0), 1.0f, 12.0f, 2, 0, -1);
                speedF = 0.0f;
                field_0xd9c = 0;
            }
        } else {
            field_0xda5 = 0;
        }
    }
    if (fopAcM_CheckCondition(this, 4)) {
        fopAcM_delete(this);
    }
}

void daNpcGuard_c::setAngle() {
    current.angle.y = shape_angle.y;
}

void daNpcGuard_c::setSpeed(f32 param_1, f32 param_2, f32* i_speed, int param_4) {
    f32 target = field_0xd8c * (param_2 * field_0xd8c);
    f32 step = field_0xd8c * (param_1 * field_0xd8c);

    if (param_4 != 0 && mAcch.ChkWallHit()) {
        s16 sVar1 = shape_angle.y + 0x8000 - mAcchCir.GetWallAngleY();
        if (abs(sVar1) < 0x4000) {
            target *= 1.0f - cM_scos(sVar1);
        }
    }

    if (param_2 < target) {
        target = param_2;
    }

    cLib_chaseF(i_speed, target, step);
}

void daNpcGuard_c::pathMoveF() {
    f32 walkSpeed = 0.0f;
    f32 anmSpeed = 0.0f;

    if (field_0xd9c == 2) {
        walkSpeed = Cd2_HIO_walkMaxSpeed(m_type);
        anmSpeed = Cd2_HIO_walkAnmPlaySpeed(m_type);

        cLib_chaseF(&field_0xd8c, 0.8f, 0.05f);
        setSpeed(0.7f, walkSpeed, &field_0xd90, 0);
        setSpeed(0.7f, walkSpeed, &speedF, 1);
    } else if (field_0xd9c == 1) {
        walkSpeed = Cd2_HIO_maxSpeed(m_type);
        anmSpeed = Cd2_HIO_anmPlaySpeed(m_type);
        cLib_chaseF(&field_0xd8c, 1.0, 0.05f);
        setSpeed(0.7f, walkSpeed, &field_0xd90, 0);
        setSpeed(0.7f, walkSpeed, &speedF, 1);
    } else if (field_0xd9c == 0) {
        cLib_chaseF(&field_0xd8c, 0.0, 0.05f);
        cLib_chaseF(&field_0xd90, 0.0f, 1.5f);
        cLib_chaseF(&speedF, 0.0f, 1.5f);
    }
    cXyz targetPoint;
    m_path.getTargetPoint(&targetPoint);
    int angle = cLib_distanceAngleS(shape_angle.y, cLib_targetAngleY(&current.pos, &targetPoint));
    if (angle >= 0x2000) {
        cLib_chaseF(&field_0xd8c, 0.0f, 0.05f);
        cLib_chaseF(&field_0xd90, 0.0f, 1.2f);
        cLib_chaseF(&speedF, 0.0f, 1.2f);
    }

    cXyz* ccMoveP = mStts.GetCCMoveP();
    if (ccMoveP != NULL) {
        *ccMoveP *= 0.25f;
    }
    fopAcM_posMoveF(this, ccMoveP);
    f32 playSpeed;
    if (field_0xd9c != 0) {
        playSpeed = field_0xd90 / walkSpeed;
        if (playSpeed > 1.0f) {
            playSpeed = 1.0f;
        }
        playSpeed = anmSpeed * playSpeed;
        if (playSpeed < 0.6f) {
            playSpeed = 0.6f;
        }
        mpMorf->setPlaySpeed(playSpeed);
    }
}

static int daNpcGuard_Create(void* i_this) {
    return static_cast<daNpcGuard_c*>(i_this)->create();
}

void daNpcGuard_c::initCollision() {
    dCcD_SrcSph sph = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x0}, 0x79}},  // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},          // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},          // mGObjTg
            {0x0},                                       // mGObjCo
        },                                               // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 30.0f}  // mSph
        }  // mSphAttr
    };

    mStts.Init(0x96, 0xff, this);
    mStts.SetRoomId(fopAcM_GetRoomNo(this));
    mCyl.SetStts(&mStts);
    mCyl.Set(m_cylDat);
    mCyl.SetR(m_type != 0 ? 60.0f : 40.0f);
    mCyl.SetH(Cd2_HIO_cylH(m_type));
    mCyl.SetTgType(0xd8000000);
    mCyl.SetTgSPrm(0x11);
    mCyl.OnTgNoHitMark();
    mCyl.OnTgNoConHit();

    for (int i = 0; i < 2; i++) {
        mSpheres[i].SetStts(&mStts);
        mSpheres[i].Set(sph);
        mSpheres[i].SetTgType(0xd8000000);
        mSpheres[i].SetTgSPrm(0x11);
        mSpheres[i].OnTgNoHitMark();
        mSpheres[i].OnTgNoConHit();
    }
}

void daNpcGuard_c::create_init() {
    gravity = -3.0f;
    fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -40.0f, -10.0f, -80.0f, 40.0f, 190.0f, 200.0f);
    mAcchCir.SetWall(Cd2_HIO_chkWallH(m_type), m_type != 0 ? 90.0f : 45.0f);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
              fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
    mAcch.SetRoofNone();
    mAcch.SetWaterNone();
    mAcch.CrrPos(dComIfG_Bgsp());
    setEnvTevCol();
    setRoomNo();
    initCollision();
    attention_info.flags = 0;
    field_0xda2 = 0;
    mpAnmKey = (J3DAnmTransformKey*)getAnmP(0x2A, mObjNum);
    mpAnmKey2 = (J3DAnmTransformKey*)getAnmP(0x2C, mObjNum);
    field_0xd88 = cLib_getRndValue(0x28, 0x28);
    if (m_path.setPath(getPathID(), fopAcM_GetRoomNo(this), 1, &current.pos, false)) {
        setAction(MODE_PATH_e);
    } else {
        setAction(MODE_FIGHT_WAIT_e);
    }
    mpMorf->setMorf(0.0f);
    field_0xd94 = current.pos.y = mAcch.GetGroundH();
    setMtx();
}

void daNpcGuard_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x, field_0xd94, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    field_0xa9c = 0;
    mpMorf->modelCalc();
    setAttention(m_type);
    lookat();
    field_0xa9c = 1;
    mpMorf->modelCalc();
    setAttention(m_type);
}

void daNpcGuard_c::lookat() {
    csXyz vecs1[3] = {
        csXyz(Cd2_HIO_maxLmtX_BACKBONE(m_type), Cd2_HIO_maxLmtY_BACKBONE(m_type), 0),
        csXyz(Cd2_HIO_maxLmtX_NECK(m_type), Cd2_HIO_maxLmtY_NECK(m_type), 0),
        csXyz(Cd2_HIO_maxLmtX_HEAD(m_type), Cd2_HIO_maxLmtY_HEAD(m_type), 0),
    };
    csXyz vecs2[3] = {
        csXyz(Cd2_HIO_minLmtX_BACKBONE(m_type), Cd2_HIO_minLmtY_BACKBONE(m_type), 0),
        csXyz(Cd2_HIO_minLmtX_NECK(m_type), Cd2_HIO_minLmtY_NECK(m_type), 0),
        csXyz(Cd2_HIO_minLmtX_HEAD(m_type), Cd2_HIO_minLmtY_HEAD(m_type), 0),
    };
    int local_68[4];
    cXyz cStack_80(cXyz::Zero);
    if (isM_()) {
        local_68[0] = 1;
        local_68[1] = 2;
        local_68[2] = 3;
        local_68[3] = -1;
    } else {
        local_68[0] = 1;
        local_68[1] = 2;
        local_68[2] = 3;
        local_68[3] = -1;
    }
    mLookat.init(mpMorf->getModel(), local_68, vecs1, vecs2);
    Mtx afStack_38;
    if (mActorMngr.getActorP() != NULL) {
        cMtx_copy(mpMorf->getModel()->getBaseTRMtx(), afStack_38);
        cStack_80 = mActorMngr.getActorP()->eyePos;
    } else {
        cMtx_copy(mpMorf->getModel()->getBaseTRMtx(), afStack_38);
        cStack_80 = cXyz::Zero;
    }
    mLookat.action(cStack_80, eyePos, this, afStack_38, NULL != mActorMngr.getActorP());
}

int daNpcGuard_c::create() {
    fopAcM_ct(this, daNpcGuard_c);

    m_type = cLib_getRndValue(0, 2);
    mObjNum = 2;
    int phase = loadResrc(m_type, mObjNum);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x2220)) {
            // Failed to create actor!
            OS_REPORT("アクター生成失敗しました！\n");
            return cPhs_ERROR_e;
        }
        create_init();
    }

    return phase;
}

static int daNpcGuard_Delete(void* i_this) {
    static_cast<daNpcGuard_c*>(i_this)->~daNpcGuard_c();
    return 1;
}

daNpcGuard_c::~daNpcGuard_c() {
    removeResrc(m_type, mObjNum);
    if (heap != NULL) {
        mpMorf->stopZelAnime();
    }
}

static int daNpcGuard_Execute(void* i_this) {
    return static_cast<daNpcGuard_c*>(i_this)->execute();
}

void daNpcGuard_c::setCollision() {
#if DEBUG
    mCyl.SetR(Cd2_HIO_cylR(m_type));
    mCyl.SetH(Cd2_HIO_cylH(m_type));
#endif
    cXyz c;
    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(4));
    mDoMtx_stack_c::multVecZero(&c);
    c.y = current.pos.y;
    mCyl.SetC(c);
    if (!fopAcM_CheckCondition(this, 4)) {
        dComIfG_Ccsp()->Set(&mCyl);
        cXyz c2;
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(11));
        mDoMtx_stack_c::scaleM(Cd2_HIO_objScale(m_type), Cd2_HIO_objScale(m_type),
                               Cd2_HIO_objScale(m_type));
        for (int i = 0; i < 2; i++) {
            c2.setall(0.0f);
            c2.y = i == 0 ? -80.0f : -150.0f;
            mDoMtx_stack_c::multVec(&c2, &c2);
            mSpheres[i].SetC(c2);
            dComIfG_Ccsp()->Set(&mSpheres[i]);
        }
    }
}

int daNpcGuard_c::execute() {
    mAcchCir.SetWallH(Cd2_HIO_chkWallH(m_type));
    mAcchCir.SetWallR(m_type != 0 ? 90.0f : 45.0f);

    if (mActionIdx != MODE_MAX_e) {
        if (dComIfGp_checkPlayerStatus1(0, 0x800000)) {
            f32 ballSearchScale = daPy_getPlayerActorClass()->getSearchBallScale() + 150.0f;
            if (fopAcM_searchPlayerDistanceXZ(this) <= ballSearchScale) {
                field_0xda2 = 1;
            } else {
                field_0xda2 = 0;
            }
        }
        u8 cutType = daPy_getPlayerActorClass()->getCutType();
        if ((cutType == daPy_py_c::CUT_TYPE_WOLF_TURN_LEFT ||
             cutType == daPy_py_c::CUT_TYPE_WOLF_TURN_RIGHT) &&
            field_0xda2)
        {
            setAction(MODE_MAX_e);
        }
    }
    callExecute();
    cLib_addCalc2(&field_0xd94, current.pos.y, 0.4f, 25.0f);
    mpMorf->play(0, 0);
    setCollision();
    setMtx();
    return 1;
}

int daNpcGuard_c::draw() {
    drawNpc();
    drawObj(mObjNum, mpModel, Cd2_HIO_objScale(m_type));
    drawShadow(60.0f);
    if (m_type == 1 && mpModel2 != NULL) {
        g_env_light.setLightTevColorType_MAJI(mpModel2, &tevStr);
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(7));
        mpModel2->setBaseTRMtx(mDoMtx_stack_c::get());
        mDoExt_modelUpdateDL(mpModel2);
    }
    return 1;
}

static int daNpcGuard_Draw(void* i_this) {
    return static_cast<daNpcGuard_c*>(i_this)->draw();
}

static int daNpcGuard_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpcGuard_METHODS = {
    (process_method_func)daNpcGuard_Create,  (process_method_func)daNpcGuard_Delete,
    (process_method_func)daNpcGuard_Execute, (process_method_func)daNpcGuard_IsDelete,
    (process_method_func)daNpcGuard_Draw,
};

actor_process_profile_definition g_profile_NPC_GUARD = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_NPC_GUARD,          // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daNpcGuard_c),    // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    406,                     // mPriority
    &daNpcGuard_METHODS,     // sub_method
    0x00040107,              // mStatus
    fopAc_NPC_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
