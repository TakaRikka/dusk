#ifndef D_A_FORMATION_MNG_H
#define D_A_FORMATION_MNG_H

#include "d/d_com_inf_game.h"
#include "d/d_path.h"
#include "d/actor/d_a_player.h"
#include "d/d_kankyo.h"
#include "SSystem/SComponent/c_math.h"

struct daFmtMng_Path_c {
    bool checkPathEnd(cXyz, f32);
    bool checkPoint(cXyz, f32);

    bool isPathClose() { return dPath_ChkClose(mPath); }
    s8 getArg0() { return mPath->m_points[field_0x4].mArg0; }
    s8 getArg1() { return mPath->m_points[field_0x4].mArg1; }

    void chgDir() {
        int dir;
        if (field_0xc > 0) {
            dir = -1;
        } else {
            dir = 1;
        }
        field_0xc = dir;
    }

    void setPath(int param_1, int param_2, cXyz* param_3) {
        mPath = dPath_GetRoomPath(param_1, param_2);
        field_0x4 = 0;
        field_0xc = 1;
        field_0x8 = param_3->abs(dPath_GetPnt(mPath, field_0x4)->m_position);
    }

    void getStartPoint(Vec* param_1, SVec* param_2) {
        *param_1 = dPath_GetPnt(mPath, 0)->m_position;
        Vec local_2c = dPath_GetPnt(mPath, 1)->m_position;
        cXyz cStack_38(param_1->x, param_1->y + 200.0f, param_1->z);
        if (fopAcM_gc_c::gndCheck(&cStack_38)) {
            param_1->y = fopAcM_gc_c::getGroundY();
        }
        param_2->x = 0;
        param_2->y = cLib_targetAngleY(param_1, &local_2c);
        param_2->z = 0;
    }

    void getTargetPoint(Vec* param_1) {
        *param_1 = dPath_GetPnt(mPath, field_0x4)->m_position;
    }

    int decIndex(int param_1) {
        param_1--;
        if (param_1 < 0) {
            if (isPathClose()) {
                param_1 = mPath->m_num - 1;
            } else {
                param_1 = 0;
            }
        }
        return param_1;
    }

    int incIndex(int param_1) {
        param_1++;
        if (param_1 >= mPath->m_num) {
            if (isPathClose()) {
                param_1 = 0;
            } else {
                param_1--;
            }
        }
        return param_1;
    }

    void setNextPoint() {
        if (field_0xc > 0) {
            field_0x4 = incIndex(field_0x4);
        } else {
            field_0x4 = decIndex(field_0x4);
        }
        field_0x8 = G_CM3D_F_INF;
    }

    void setNextPoint(cXyz *param_1) {
        setNextPoint();
        field_0x8 = param_1->abs(dPath_GetPnt(mPath, field_0x4)->m_position);
    }

    /* 0x0 */ dPath* mPath;
    /* 0x4 */ int field_0x4;
    /* 0x8 */ f32 field_0x8;
    /* 0xC */ s8 field_0xc;
};

struct FmtPos_c : public daFmtMng_Path_c {
    /* 0x10 */ cXyz field_0x10;
    /* 0x1C */ s16 field_0x1c;
    /* 0x1E */ s16 field_0x1e;
};

struct FmtMember_c {
    /* 0x00 */ u32 mNpcId;
    /* 0x04 */ cXyz field_0x04;
    /* 0x10 */ cXyz field_0x10;
    /* 0x1C */ s16 field_0x1c;
    /* 0x1E */ u16 field_0x1e;
};

enum FmtMngAction {
    MODE_0_e,
    MODE_1_e,
    MODE_2_e,
    MODE_3_e,
    MODE_4_e,
    MODE_MAX_e,
};

struct FmtMngAttributes {
    /* 0x00 */ int field_0x00;
    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0c;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ s16 field_0x14;
    /* 0x16 */ s16 field_0x16;
};

struct daFmtMng_c : public fopAc_ac_c {
    void initWait();
    void initMemberPos();
    void executeWait();
    void initWalk();
    void executeWalk();
    void setMovePath(FmtPos_c*, int);
    void initReverse();
    void executeReverse();
    void initMotion();
    void executeMotion();
    void initFight();
    void executeFight();
    int create();
    void create_init();

    ~daFmtMng_c() {
        FmtMember_c* member = mMember;
        for (int i = 0; i < mFormationLine * mFormationRow; i++, member++) {
            fopAcM_delete(member->mNpcId);
        }
        delete [] mPos;
        delete [] mMember;
    }

    int getTimeHour() {
        if (dKy_darkworld_check()) {
            return dKy_getDarktime_hour();
        }
        return dKy_getdaytime_hour();
    }

    int getTimeMinute() {
        if (dKy_darkworld_check()) {
            return dKy_getDarktime_minute();
        }
        return dKy_getdaytime_minute();
    }

    int getTime() {
        return getTimeHour() * 60 + getTimeMinute();
    }

    u8 getPathID() { return shape_angle.x; }
    u8 getFormationLine() { return fopAcM_GetParam(this) & 0xff; }
    u8 getFormationRow() { return (fopAcM_GetParam(this) >> 8) & 0xff; }
    int getStartTime() { return (fopAcM_GetParam(this) >> 16) & 0xff; }
    int getEndTime() { return (fopAcM_GetParam(this) >> 24) & 0xff; }

    void callExecute() {
        JUT_ASSERT(680, mAction != NULL);
        (this->*(mAction[1]))();
    }

    bool checkEmergency() {
        if (dKy_darkworld_check() || !daPy_py_c::checkNowWolf()) {
            return false;
        }
        FmtMember_c* member = mMember;
        fopAc_ac_c* npcFgd = NULL;
        for (int i = 0; i < mFormationRow * mFormationLine; i++, member++) {
            fopAcM_SearchByID(member->mNpcId, &npcFgd);
            if (npcFgd != NULL) {
                cXyz* iVar6 = fopAcM_GetPosition_p(daPy_getPlayerActorClass());
                f32 dVar10 = iVar6->y;
                cXyz* this_01 = fopAcM_GetPosition_p(npcFgd);
                if (this_01->y - dVar10 < 200.0f) {
                    f32 maxDist = pow(500.0, 2.0);
                    if (fopAcM_searchPlayerDistanceXZ2(npcFgd) < maxDist) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void callInit() {
        JUT_ASSERT(667, mAction != NULL)
        (this->*(*mAction))();
    }

    void setAction(int i_action) {
        JUT_ASSERT(633, i_action < MODE_MAX_e);
        mPrevAction = mCurrentAction;
        mCurrentAction = i_action;
        mAction = &((ActionFunc*)ActionTable)[mCurrentAction * 2];
        callInit();
    }

    int execute() {
        callExecute();
        if (mCurrentAction != MODE_4_e && checkEmergency()) {
            setAction(MODE_4_e);
        }
        return 1;
    }

    bool isAllMemberCulling() {
        FmtMember_c* member = mMember;
        bool memberCulling;
        fopAc_ac_c* npcFgd = NULL;
        for (int i = 0; i < mFormationRow * mFormationLine; i++, member++) {
            memberCulling = false;
            if (member->mNpcId != -1) {
                fopAcM_SearchByID(member->mNpcId, &npcFgd);
                if (npcFgd != NULL) {
                    memberCulling = fopAcM_CheckCondition(npcFgd, 4);
                }
                if (!memberCulling) {
                    return false;
                }
            }
        }
        return true;
    }

    void setMoveSpeed() {
        speedF += attr().field_0x08;
        if (speedF > attr().field_0x04) {
            speedF = attr().field_0x04;
        }
    }

    void setMoveAngle(FmtPos_c* member) {
        if (speedF <= 0.0f) {
            return;
        }
        cXyz acStack_28;
        member->getTargetPoint(&acStack_28);
        cLib_addCalcAngleS2(&member->field_0x1c,
                            cLib_targetAngleY(&member->field_0x10, &acStack_28),
                            attr().field_0x14, attr().field_0x16);
    }

    void setMovePos(cXyz* param_1, s16* param_2) {
        param_1->x += speedF * cM_ssin(*param_2);
        param_1->z += speedF * cM_scos(*param_2);
    }

    void correctFormation() {
        FmtPos_c* pos = mPos + 1;
        for (int i = 1; i < mFormationRow; i++, pos++) {
            if (cLib_distanceAngleS(pos[-1].field_0x1c, pos[0].field_0x1c) <= attr().field_0x00) {
                pos[0].field_0x10.set(0.0f, 0.0f, -attr().field_0x10);
                mDoMtx_stack_c::transS(pos[-1].field_0x10);
                mDoMtx_stack_c::YrotM(pos[-1].field_0x1c);
                mDoMtx_stack_c::multVec(&pos[0].field_0x10, &pos[0].field_0x10);
            }
        }
    }

    void setMemberPos() {
        int j;
        FmtPos_c* pos;
        fopAc_ac_c* npcFgd;
        FmtMember_c* member;
        int i;
        pos = mPos;
        member = mMember;
        for (i = 0; i < mFormationRow; i++, pos++) {
            f32 dVar13 = (mFormationLine - 1) * attr().field_0x0c * 0.5f;
            mDoMtx_stack_c::transS(pos->field_0x10);
            mDoMtx_stack_c::YrotM(pos->field_0x1c);
            for (j = 0; j < mFormationLine; j++, member++) {
                member->field_0x10.set(member->field_0x04);
                member->field_0x04.set(dVar13, 0.0f, 0.0f);
                mDoMtx_stack_c::multVec(&member->field_0x04, &member->field_0x04);
                dVar13 -= attr().field_0x0c;
                npcFgd = NULL;
                fopAcM_SearchByID(member->mNpcId, &npcFgd);
                if (npcFgd != NULL) {
                    cXyz* npcPos = fopAcM_GetPosition_p(npcFgd);
                    csXyz* npcAngle = fopAcM_GetShapeAngle_p(npcFgd);
                    npcPos->x = member->field_0x04.x;
                    npcPos->z = member->field_0x04.z;
                    cLib_addCalcAngleS2(&npcAngle->y,
                                cLib_targetAngleY(&member->field_0x10, &member->field_0x04),
                                attr().field_0x14, attr().field_0x16);
                    fopAcM_SetSpeedF(npcFgd, member->field_0x10.abs(member->field_0x04));
                }
            }
        }
    }

    static FmtMngAttributes const& attr() { return M_attr; }
    static FmtMngAttributes const M_attr;

    typedef void (daFmtMng_c::*ActionFunc)();
    static daFmtMng_c::ActionFunc ActionTable[10];

    /* 0x568 */ FmtPos_c* mPos;
    /* 0x56C */ FmtMember_c* mMember;
    /* 0x570 */ dPath* mRoomPath;
    /* 0x574 */ ActionFunc* mAction;
    /* 0x578 */ int mCurrentAction;
    /* 0x57C */ int mPrevAction;
    /* 0x580 */ int field_0x580;
    /* 0x584 */ int mFormationLine;
    /* 0x588 */ int mFormationRow;
    /* 0x58C */ int mStartTime;
    /* 0x590 */ int mEndTime;
    /* 0x594 */ int mDayOfWeek;
    /* 0x598 */ int field_0x598;
    /* 0x59C */ int field_0x59c;
    /* 0x5A0 */ int field_0x5a0;
};

#endif /* D_A_FORMATION_MNG_H */
