/**
 * @file d_a_npc_passer.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_passer.h"
#include "d/d_s_play.h"

daNpcPasser_c::~daNpcPasser_c() {
    removeResrc(m_type, m_objNum);

    if (heap != NULL) {
        mpMorf->stopZelAnime();
    }

    #if DEBUG
    l_Cd2_HIO.removeHIO(this);
    #endif
}

int daNpcPasser_c::createHeap() {
    int rv = NpcCreate(m_type);
    if (rv != 0) {
        if (m_objNum != 0) {
            rv = NULL != (mpModel = ObjCreate(m_objNum));
            if (!rv) {
                mpMorf->stopZelAnime();
            }
        } else {
            mpModel = NULL;
            rv = 1;
        }
    }

    return rv;
}

static int createHeapCallBack(fopAc_ac_c* i_this) {
    daNpcPasser_c* actor = (daNpcPasser_c*)i_this;
    return actor->createHeap();
}

cPhs__Step daNpcPasser_c::create() {
    fopAcM_ct(this, daNpcPasser_c);

    mIsDarkWorld = dKy_darkworld_check();
    m_type = getType();

    if (getEndTime() < 0xFF) {
        m_endTime = getEndTime();
        m_dayOfWeek = getDayOfWeek();

        if (getTime() > m_endTime) {
            m_dayOfWeek = (m_dayOfWeek + 1) % 7;
        }
    } else {
        m_endTime = 0xFFFF;
    }

    current.angle.x = 0;

    if (m_type < 0 || m_type > 30) {
        // Failed to generate high-poly pedestrian actor! ---> m_type = %d
        OS_REPORT("ハイポリ通行人アクター生成失敗しました！---> m_type = %d\n", m_type);
        return cPhs_ERROR_e;
    }

    m_objNum = getObjNum();
    if (m_objNum < 0 || m_objNum > 13) {
        // Failed to generate high-poly pedestrian actor! ---> m_objNum = %d
        OS_REPORT("ハイポリ通行人アクター生成失敗しました！---> m_objNum = %d\n", m_objNum);
        return cPhs_ERROR_e;
    }

    cPhs__Step phase = (cPhs__Step)loadResrc(m_type, m_objNum);
    if (phase == cPhs_COMPLEATE_e) {
        u32 i_size = 0;
        #if DEBUG
        i_size = 0x2300;
        #else
        i_size = 0x80002300;
        #endif
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, i_size)) {
            // Failed to create actor!
            OS_REPORT("アクター生成失敗しました！\n");
            return cPhs_ERROR_e;
        }

        create_init();
    }

    return phase;
}

int daNpcPasser_c::execute() {
    mAcchCir.SetWallH(Cd2_HIO_chkWallH(m_type));
    mAcchCir.SetWallR(Cd2_HIO_chkWallR(m_type));
    callExecute();
    cLib_addCalc2(&mPosY, current.pos.y, 0.4f, 25.0f);
    animation(-1);
    setCollision();
    setBaseMtx();

    if (fopAcM_CheckCondition(this, fopAcCnd_NODRAW_e) && m_endTime != 0xFFFF && m_dayOfWeek == getDayOfWeek()) {
        int time = getTime();
        if (time >= m_endTime) {
            fopAcM_delete(this);
        }
    }

    return 1;
}

int daNpcPasser_c::draw() {
    drawNpc();
    drawObj(m_objNum, mpModel, Cd2_HIO_objScale(m_type));
    drawShadow(40.0f);

    #if DEBUG
    mAcch.DrawWall(dComIfG_Bgsp());
    #endif

    mLookat.dbView();
    return 1;
}

void daNpcPasser_c::setAction(daNpcPasser_c::Mode_e i_action) {
    JUT_ASSERT(328, i_action < MODE_MAX_e);
    mPrevActionIdx = mActionIdx;
    mActionIdx = i_action;
    mAction = ActionTable[mActionIdx];
    callInit();
}

void daNpcPasser_c::callInit() {
    JUT_ASSERT(346, mAction != NULL);
    (this->*mAction[0])();
}

void daNpcPasser_c::callExecute() {
    JUT_ASSERT(359, mAction != NULL);
    (this->*mAction[1])();
}

enum Rnd_Values {
    /* 0x0 */ ZERO,
    /* 0x1 */ ONE,
    /* 0x2 */ TWO,
    /* 0x3 */ THREE,
    /* 0x4 */ FOUR,
    /* 0x5 */ FIVE,
};

void daNpcPasser_c::initPath() {
    if (getMoveType() == 1) {
        field_0xb1c = 2;
    } else if (getMoveType() == 2) {
        field_0xb1c = 1;
    } else {
        field_0xb1c = cLib_getRndValue(0, 2) != 0 ? TWO : ONE;
    }

    int idx = 2;
    if (field_0xb1c == 2) {
        if (getWalkMotionType() == 1) {
            idx = 2;
        } else if (getWalkMotionType() == 2) {
            idx = 3;
        } else {
            idx = cLib_getRndValue(0, 2) != 0 ? TWO : THREE;
        }
    } else if (field_0xb1c == 1) {
        if (getRunMotionType() == 1) {
            idx = 4;
        } else if (getRunMotionType() == 2) {
            idx = 5;
        } else {
            idx = cLib_getRndValue(0, 2) != 0 ? FOUR : FIVE;
        }
    }

    setAnm((J3DAnmTransformKey*)getAnmP(idx, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
    field_0xb23 = 0;
    field_0xb22 = 1;
    mPathMode = 0;
}

void daNpcPasser_c::executePath() {
    switch (mPathMode) {
        case 0:
            if (m_path.checkPoint(current.pos, speedF)) {
                arg0 = m_path.getArg0();
                if (arg0 >= 0 && arg0 < 28) {
                    field_0xb1c = 0;
                    mPathMode = 1;
                } else {
                    mPathMode = 2;
                }
            }
            break;
        
        case 1:
            if ((this->*m_funcTbl[arg0][field_0xb23])(NULL)) {
                if (m_funcTbl[arg0][field_0xb23 + 1] == NULL) {
                    field_0xb23 = 0;
                    mPathMode = 2;
                } else {
                    field_0xb23++;
                    field_0xb22 = 1;
                }
            }
            break;

        case 2:
            if (m_path.checkPathEnd(current.pos, speedF)) {
                fopAcM_delete(this);
            } else {
                if (field_0xb1c == 0) {
                    setAnm((J3DAnmTransformKey*)getAnmP(2, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
                    field_0xb1c = 2;
                }

                m_path.setNextPoint(current.pos);
                mPathMode = 0;
            }
            break;

        default:
            JUT_ASSERT(433, FALSE);
            break;
    }

    if (field_0xb1c == 2 || field_0xb1c == 1) {
        m_path.checkObstacle(this);
        cXyz targetPnt;
        m_path.getTargetPoint(&targetPnt);
        cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &targetPnt), MREG_S(0) + 13, MREG_S(1) + 0x600);

        if (current.pos.y != old.pos.y) {
            s16 sVar1 = 0;
            cM3dGPla plane;

            bool isTriPla = dComIfG_Bgsp().GetTriPla(mAcch.m_gnd, &plane);
            if (isTriPla) {
                sVar1 = fopAcM_getPolygonAngle(&plane, shape_angle.y);
            }

            if (sVar1 != 0) {
                mPosY = current.pos.y;
            }
        }

        if (mAcch.ChkWallHit() && current.pos.abs(old.pos) < 2.0f) {
            field_0xb25++;
            if (field_0xb25 > 30) {
                setAction(MODE_4);
            }
        } else {
            field_0xb25 = 0;
        }
    }

    setAngle();
    pathMoveF();
    mAcch.CrrPos(dComIfG_Bgsp());

    if (checkFearSituation()) {
        if (m_objNum == 2 && (m_type == 0 || m_type == 1)) {
            setAction(MODE_3);
        } else {
            setAction(MODE_1);
        }

        fopAc_ac_c* actor_p = fopAcM_SearchByID(parentActorID);
        if (actor_p != NULL) {
            fopAcM_delete(actor_p);
        }
    }
}

void daNpcPasser_c::initEscape() {
    mEscapeTag = getEscapeTag();
    JUT_ASSERT(491, mEscapeTag != NULL);
    JUT_ASSERT(492, mEscapeTag->getPathID() != 0xff);
    m_path.setPath(mEscapeTag->getPathID(), fopAcM_GetRoomNo(this), 1, &current.pos, true);
    JUT_ASSERT(494, m_path.isPath());

    m_path.setNextPoint(current.pos);

    if (m_objNum == 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0x29, m_objNum), 1.0f, 0.0f, J3DFrameCtrl::EMode_NONE, 0, -1);
        mCitizen.playVoice(1);
        field_0xb1c = 0;
    } else {
        setAnm((J3DAnmTransformKey*)getAnmP(4, m_objNum),1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        mCitizen.playVoice(0);
        field_0xb1c = 1;
    }

    dComIfGs_onSaveDunSwitch(60);
}

void daNpcPasser_c::executeEscape() {
    if (field_0xb1c == 0) {
        if (mpMorf->isStop()) {
            setAnm((J3DAnmTransformKey*)getAnmP(4, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
            mCitizen.playVoice(0);
            field_0xb1c = 1;
        }

        cLib_addCalcAngleS2(&current.angle.y, fopAcM_searchPlayerAngleY(this), MREG_S(0) + 3, MREG_S(1) + 0x600);
    } else if (field_0xb1c == 1) {
        if (fopAcM_CheckCondition(this, fopAcCnd_NODRAW_e)) {
            fopAcM_delete(this);
        }

        if (m_path.checkPoint(current.pos, speedF)) {
            if (m_path.checkPathEnd(current.pos, speedF)) {
                fopAcM_delete(this);
            }

            m_path.setNextPoint(current.pos);
        }

        m_path.checkObstacle(this);
        cXyz targetPnt;
        m_path.getTargetPoint(&targetPnt);
        cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &targetPnt), MREG_S(0) + 3, MREG_S(1) + 0x600);

        if (current.pos.y != old.pos.y) {
            s16 sVar1 = 0;
            cM3dGPla plane;
            bool isTriPla = dComIfG_Bgsp().GetTriPla(mAcch.m_gnd, &plane);

            if (isTriPla) {
                sVar1 = fopAcM_getPolygonAngle(&plane, shape_angle.y);
            }

            if (sVar1 != 0) {
                mPosY = current.pos.y;
            }
        }

        setAngle();
        pathMoveF();
        mAcch.CrrPos(dComIfG_Bgsp());

        if (mAcch.ChkWallHit() && current.pos.abs(old.pos) < 2.0f) {
            field_0xb25++;
            if (field_0xb25 > 30) {
                setAction(MODE_2);
            }
        } else {
            field_0xb25 = 0;
        }
    }
}

void daNpcPasser_c::initFear() {
    m_objNum = 0;
    setAnm((J3DAnmTransformKey*)getAnmP(0x26, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
    mTargetAngleY = cLib_targetAngleY(fopAcM_GetPosition_p(daPy_getPlayerActorClass()), &current.pos);
    speedF = 0.0f;
    field_0xb1c = 0;
}

void daNpcPasser_c::executeFear() {
    if (mpMorf->checkFrame(1.0f)) {
        mCitizen.playVoice(2);
    }

    if (fopAcM_CheckCondition(this, fopAcCnd_NODRAW_e)) {
        fopAcM_delete(this);
    }

    cLib_addCalcAngleS2(&shape_angle.y, mTargetAngleY, MREG_S(0) + 3, MREG_S(1) + 0x600);
}

void daNpcPasser_c::initFight() {
    setAnm((J3DAnmTransformKey*)getAnmP(0x29, m_objNum), 1.0f, 0.0f, J3DFrameCtrl::EMode_NONE, 0, -1);
    mCitizen.playVoice(1);
    speedF = 0.0f;
    fopAcM_setCullSizeBox(this, -40.0f, -10.0f, -80.0f, 40.0f, 190.0f, 200.0f);
    dComIfGs_onSaveDunSwitch(60);
    field_0xb1c = 0;
}

void daNpcPasser_c::executeFight() {
    if (mpMorf->isStop()) {
        setAnm((J3DAnmTransformKey*)getAnmP(0x2A, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
    }

    mTargetAngleY = fopAcM_searchPlayerAngleY(this);

    if (mpMorf->getAnm() != getAnmP(0x29, m_objNum)) {
        if (mpMorf->checkFrame(1.0f)) {
            mCitizen.playVoice(2);
        }

        if (cLib_distanceAngleS(shape_angle.y, mTargetAngleY) > 0x600) {
            setAnm((J3DAnmTransformKey*)getAnmP(0x2C, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        } else {
            setAnm((J3DAnmTransformKey*)getAnmP(0x2A, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        }
    }

    if (fopAcM_CheckCondition(this, fopAcCnd_NODRAW_e)) {
        fopAcM_delete(this);
    }

    cLib_addCalcAngleS2(&shape_angle.y, mTargetAngleY, MREG_S(0) + 3, MREG_S(1) + 0x600);
}

void daNpcPasser_c::initRelief() {
    setAnm((J3DAnmTransformKey*)getAnmP(0, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
    speedF = 0.0f;
    mTargetAngleY = shape_angle.y + 0x8000;
    field_0xb1c = 0;
}

void daNpcPasser_c::executeRelief() {
    if (fopAcM_CheckCondition(this, fopAcCnd_NODRAW_e)) {
        fopAcM_delete(this);
    }

    if (checkFearSituation()) {
        if (m_objNum == 2 && (m_type == 0 || m_type == 1)) {
            setAction(MODE_3);
        } else {
            setAction(MODE_1);
        }

        fopAc_ac_c* actor_p = fopAcM_SearchByID(parentActorID);
        if (actor_p != NULL) {
            fopAcM_delete(actor_p);
        }
    }

    cLib_addCalcAngleS2(&shape_angle.y, mTargetAngleY, MREG_S(0) + 3, MREG_S(1) + 0x600);
}

void daNpcPasser_c::create_init() {
    #if DEBUG
    // NPC (C-class, D-class) Pedestrian High
    l_Cd2_HIO.entryHIO("NPC(C級,D級)通行人 High");
    #endif

    gravity = -3.0f;
    fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -40.0f, -10.0f, -30.0f, 30.0f, 190.0f, 40.0f);
    mAcchCir.SetWall(Cd2_HIO_chkWallH(m_type), Cd2_HIO_chkWallR(m_type));
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this),
              fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
    mAcch.SetRoofNone();
    mAcch.SetWaterNone();
    mAcch.CrrPos(dComIfG_Bgsp());

    setEnvTevCol();
    setRoomNo();

    initCollision();
    attention_info.flags = 0;

    if (m_path.setPath(getPathID(), fopAcM_GetRoomNo(this), 1, &current.pos, true)) {
        setAction(MODE_0);
    }

    mpMorf->setMorf(0.0f);
    mAcch.CrrPos(dComIfG_Bgsp());
    mPosY = current.pos.y = mAcch.GetGroundH();
    setBaseMtx();
}

daNpcPasser_c::actionFunc daNpcPasser_c::ActionTable[5][2] = {
    {&daNpcPasser_c::initPath, &daNpcPasser_c::executePath},
    {&daNpcPasser_c::initEscape, &daNpcPasser_c::executeEscape},
    {&daNpcPasser_c::initFear, &daNpcPasser_c::executeFear},
    {&daNpcPasser_c::initFight, &daNpcPasser_c::executeFight},
    {&daNpcPasser_c::initRelief, &daNpcPasser_c::executeRelief},
};

daNpcPasser_c::seqFunc* daNpcPasser_c::m_funcTbl[28] = {
    daNpcPasser_c::m_seq00_funcTbl, daNpcPasser_c::m_seq01_funcTbl,
    daNpcPasser_c::m_seq02_funcTbl, daNpcPasser_c::m_seq03_funcTbl,
    daNpcPasser_c::m_seq04_funcTbl, daNpcPasser_c::m_seq05_funcTbl,
    daNpcPasser_c::m_seq06_funcTbl, daNpcPasser_c::m_seq07_funcTbl,
    daNpcPasser_c::m_seq08_funcTbl, daNpcPasser_c::m_seq09_funcTbl,
    daNpcPasser_c::m_seq10_funcTbl, daNpcPasser_c::m_seq11_funcTbl,
    daNpcPasser_c::m_seq12_funcTbl, daNpcPasser_c::m_seq13_funcTbl,
    daNpcPasser_c::m_seq14_funcTbl, daNpcPasser_c::m_seq15_funcTbl,
    daNpcPasser_c::m_seq16_funcTbl, daNpcPasser_c::m_seq17_funcTbl,
    daNpcPasser_c::m_seq18_funcTbl, daNpcPasser_c::m_seq19_funcTbl,
    daNpcPasser_c::m_seq20_funcTbl, daNpcPasser_c::m_seq21_funcTbl,
    daNpcPasser_c::m_seq22_funcTbl, daNpcPasser_c::m_seq23_funcTbl,
    daNpcPasser_c::m_seq24_funcTbl, daNpcPasser_c::m_seq25_funcTbl,
    daNpcPasser_c::m_seq26_funcTbl, daNpcPasser_c::m_seq27_funcTbl,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq00_funcTbl[2] = {
    &daNpcPasser_c::walka,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq01_funcTbl[2] = {
    &daNpcPasser_c::runa,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq02_funcTbl[2] = {
    &daNpcPasser_c::waita,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq03_funcTbl[2] = {
    &daNpcPasser_c::talka,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq04_funcTbl[2] = {
    &daNpcPasser_c::runb,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq05_funcTbl[4] = {
    &daNpcPasser_c::turnr,
    &daNpcPasser_c::waita,
    &daNpcPasser_c::talka,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq06_funcTbl[4] = {
    &daNpcPasser_c::turnl,
    &daNpcPasser_c::waita,
    &daNpcPasser_c::talka,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq07_funcTbl[2] = {
    &daNpcPasser_c::waitwall,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq08_funcTbl[7] = {
    &daNpcPasser_c::talkwall,
    &daNpcPasser_c::waitwall,
    &daNpcPasser_c::talkbwall,
    &daNpcPasser_c::waitwall,
    &daNpcPasser_c::talkwall,
    &daNpcPasser_c::talkbwall,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq09_funcTbl[2] = {
    &daNpcPasser_c::talkb,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq10_funcTbl[2] = {
    &daNpcPasser_c::talkc,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq11_funcTbl[6] = {
    &daNpcPasser_c::talka,
    &daNpcPasser_c::waita,
    &daNpcPasser_c::talkb,
    &daNpcPasser_c::waitb,
    &daNpcPasser_c::talkc,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq12_funcTbl[2] = {
    &daNpcPasser_c::waitb,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq13_funcTbl[6] = {
    &daNpcPasser_c::browsea,
    &daNpcPasser_c::browseb,
    &daNpcPasser_c::talka,
    &daNpcPasser_c::talkb,
    &daNpcPasser_c::waita,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq14_funcTbl[2] = {
    &daNpcPasser_c::walkb,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq15_funcTbl[2] = {
    &daNpcPasser_c::sitwaita,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq16_funcTbl[7] = {
    &daNpcPasser_c::sittalka,
    &daNpcPasser_c::sitwaita,
    &daNpcPasser_c::sittalkab,
    &daNpcPasser_c::sitwaita,
    &daNpcPasser_c::sittalka,
    &daNpcPasser_c::sittalkab,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq17_funcTbl[2] = {
    &daNpcPasser_c::playm,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq18_funcTbl[2] = {
    &daNpcPasser_c::sitwaitb,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq19_funcTbl[7] = {
    &daNpcPasser_c::sittalkb,
    &daNpcPasser_c::sitwaitb,
    &daNpcPasser_c::sittalkbb,
    &daNpcPasser_c::sitwaitb,
    &daNpcPasser_c::sittalkb,
    &daNpcPasser_c::sittalkbb,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq20_funcTbl[2] = {
    &daNpcPasser_c::looka,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq21_funcTbl[2] = {
    &daNpcPasser_c::lookb,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq22_funcTbl[4] = {
    &daNpcPasser_c::looka,
    &daNpcPasser_c::lookb,
    &daNpcPasser_c::waita,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq23_funcTbl[7] = {
    &daNpcPasser_c::talka,
    &daNpcPasser_c::waita,
    &daNpcPasser_c::talkb,
    &daNpcPasser_c::waita,
    &daNpcPasser_c::talka,
    &daNpcPasser_c::talkb,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq24_funcTbl[5] = {
    &daNpcPasser_c::talkb,
    &daNpcPasser_c::talka,
    &daNpcPasser_c::talkc,
    &daNpcPasser_c::waita,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq25_funcTbl[7] = {
    &daNpcPasser_c::browsea,
    &daNpcPasser_c::browseb,
    &daNpcPasser_c::talka,
    &daNpcPasser_c::talkb,
    &daNpcPasser_c::talkc,
    &daNpcPasser_c::waita,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq26_funcTbl[3] = {
    &daNpcPasser_c::browsea,
    &daNpcPasser_c::browseb,
    NULL,
};

daNpcPasser_c::seqFunc daNpcPasser_c::m_seq27_funcTbl[1] = {
    &daNpcPasser_c::waitTime,
};

void daNpcPasser_c::setAngle() {
    shape_angle.y = current.angle.y;
}

void daNpcPasser_c::setSpeed(f32 param_1, f32 param_2, f32* i_speed, int param_4) {
    f32 target = field_0xb10 * (param_2 * field_0xb10);
    f32 step = field_0xb10 * (param_1 * field_0xb10);

    if (param_4 != 0 && mAcch.ChkWallHit()) {
        s16 sVar1 = current.angle.y + 0x8000 - mAcchCir.GetWallAngleY();
        if (abs(sVar1) < 0x4000) {
            target *= 1.0f - cM_scos(sVar1);
        }
    }

    if (param_2 < target) {
        target = param_2;
    }

    cLib_chaseF(i_speed, target, step);
}

void daNpcPasser_c::pathMoveF() {
    f32 fVar1 = 0.0f;
    f32 speed = 0.0f;
    f32 fVar2 = 0.0f;
    cXyz* ccMoveP = mStts.GetCCMoveP();

    if (field_0xb1c == 2) {
        fVar1 = Cd2_HIO_walkMaxSpeed(m_type);
        fVar2 = Cd2_HIO_walkAnmPlaySpeed(m_type);
        cLib_chaseF(&field_0xb10, 0.8f, 0.05f);
        setSpeed(MREG_F(0) + 0.7f, fVar1, &field_0xb14, 0);
        setSpeed(MREG_F(0) + 0.7f, fVar1, &speedF, 1);
    } else if (field_0xb1c == 1) {
        fVar1 = Cd2_HIO_maxSpeed(m_type);
        fVar2 = Cd2_HIO_anmPlaySpeed(m_type);

        if (mActionIdx == 1) {
            fVar1 *= 1.5f;
            fVar2 *= 1.5f;
        }

        cLib_chaseF(&field_0xb10, 1.0f, 0.05f);
        setSpeed(MREG_F(0) + 0.7f, fVar1, &field_0xb14, 0);
        setSpeed(MREG_F(0) + 0.7f, fVar1, &speedF, 1);
    } else if (field_0xb1c == 0) {
        cLib_chaseF(&field_0xb10, 0.0f, 0.05f);
        cLib_chaseF(&field_0xb14, 0.0f, 1.5f);
        cLib_chaseF(&speedF, 0.0f, 1.5f);
    }

    cXyz targetPnt;
    m_path.getTargetPoint(&targetPnt);

    s32 angleS = cLib_distanceAngleS(current.angle.y, cLib_targetAngleY(&current.pos, &targetPnt));
    if (angleS >= 0x2000) {
        cLib_chaseF(&field_0xb10, 0.0f, 0.05f);
        cLib_chaseF(&field_0xb14, 0.0f, 1.2f);
        cLib_chaseF(&speedF, 0.0f, 1.2f);
    }

    fopAcM_posMoveF(this, ccMoveP);

    if (field_0xb1c != 0) {
        fVar1 = field_0xb14 / fVar1;
        if (fVar1 > 1.0f) {
            fVar1 = 1.0f;
        }

        speed = fVar2 * fVar1;
        if (speed < 0.6f) {
            speed = 0.6f;
        }

        mpMorf->setPlaySpeed(speed);
    }
}

void daNpcPasser_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, mPosY, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
    setAttention(m_type);
}

void daNpcPasser_c::initCollision() {
    mStts.Init(0xD9, 0xFF, this);
    mStts.SetRoomId(fopAcM_GetRoomNo(this));
    mCyl.SetStts(&mStts);
    mCyl.Set(m_cylDat);
    mCyl.SetR(Cd2_HIO_cylR(m_type));
    mCyl.SetH(Cd2_HIO_cylH(m_type));
}

void daNpcPasser_c::setCollision() {
    #if DEBUG
    mCyl.SetR(Cd2_HIO_cylR(m_type));
    mCyl.SetH(Cd2_HIO_cylH(m_type));
    #endif

    mCyl.SetC(current.pos);

    if (!fopAcM_CheckCondition(this, fopAcCnd_NODRAW_e)) {
        dComIfG_Ccsp()->Set(&mCyl);
    }
}


BOOL daNpcPasser_c::waita(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::talka(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(6, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::waitwall(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0xB, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::talkwall(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0xC, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::talkbwall(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0xD, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::talkb(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(7, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::talkc(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(8, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::waitb(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(1, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::sitwaita(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0xE, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::sittalka(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0xF, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::sittalkab(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0x10, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::sitwaitb(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0x11, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::sittalkb(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0x12, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::sittalkbb(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0x13, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::looka(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(9, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::lookb(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0xA, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::playm(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0x22, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::turnr(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        mTargetAngleY = current.angle.y - 0x4000;
        field_0xb22 = 0;
    }

    cLib_addCalcAngleS2(&current.angle.y, mTargetAngleY, 4, 0x2000);

    if (abs(mTargetAngleY - current.angle.y) <= 4) {
        current.angle.y = mTargetAngleY;
        return TRUE;
    }

    return FALSE;
}

int daNpcPasser_c::turnl(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        mTargetAngleY = current.angle.y + 0x4000;
        field_0xb22 = 0;
    }

    cLib_addCalcAngleS2(&current.angle.y, mTargetAngleY, 4, 0x2000);

    if (abs(mTargetAngleY - current.angle.y) <= 4) {
        current.angle.y = mTargetAngleY;
        return TRUE;
    }

    return FALSE;
}

int daNpcPasser_c::walka(void* param_1) {
    setAnm((J3DAnmTransformKey*)getAnmP(2, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
    field_0xb1c = 2;
    return TRUE;
}

int daNpcPasser_c::runa(void* param_1) {
    setAnm((J3DAnmTransformKey*)getAnmP(4, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
    field_0xb1c = 1;
    return TRUE;
}

int daNpcPasser_c::runb(void* param_1) {
    setAnm((J3DAnmTransformKey*)getAnmP(5, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
    field_0xb1c = 1;
    return TRUE;
}

int daNpcPasser_c::walkb(void* param_1) {
    setAnm((J3DAnmTransformKey*)getAnmP(3, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
    field_0xb1c = 2;
    return TRUE;
}

int daNpcPasser_c::browsea(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0x14, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::browseb(void* param_1) {
    if (!isStop()) {
        return FALSE;
    }

    if (field_0xb22 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0x15, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
        field_0xaa0 = cLib_getRndValue(2.0f, 2.0f) + 0.1f;
        field_0xb22 = 0;
    }

    return field_0xaa0 == 0;
}

int daNpcPasser_c::waitTime(void* param_1) {
    if (field_0xb22 != 0) {
        field_0xb22 = 0;
    }

    return FALSE;
}

static int daNpcPasser_Create(void* i_this) {
    return static_cast<daNpcPasser_c*>(i_this)->create();
}

static int daNpcPasser_Delete(void* i_this) {
    static_cast<daNpcPasser_c*>(i_this)->~daNpcPasser_c();
    return 1;
}

static int daNpcPasser_Execute(void* i_this) {
    return static_cast<daNpcPasser_c*>(i_this)->execute();
}

static int daNpcPasser_Draw(void* i_this) {
    return static_cast<daNpcPasser_c*>(i_this)->draw();
}

static int daNpcPasser_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpcPasser_METHODS = {
    (process_method_func)daNpcPasser_Create,
    (process_method_func)daNpcPasser_Delete,
    (process_method_func)daNpcPasser_Execute,
    (process_method_func)daNpcPasser_IsDelete,
    (process_method_func)daNpcPasser_Draw,
};

extern actor_process_profile_definition g_profile_NPC_PASSER = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_PASSER,        // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpcPasser_c),  // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  401,                    // mPriority
  &daNpcPasser_METHODS,   // sub_method
  0x02040107,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
