//
// Formation Manager
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_formation_mng.h"
#include "d/actor/d_a_npc_fguard.h"
#include "d/d_save.h"
#include "f_op/f_op_actor_mng.h"

FmtMngAttributes const daFmtMng_c::M_attr = {
    0x14, 6.0f, 0.5f,
    120.0f, 150.0f, 10, 0x4000,
};

void daFmtMng_c::initWait() {
    FmtMember_c* member = mMember;
    fopAc_ac_c* npcFgd = NULL;
    for (int i = 0; i < mFormationRow * mFormationLine; i++, member++) {
        fopAcM_SearchByID(member->mNpcId, &npcFgd);
        if (npcFgd != NULL) {
            ((daNpcFgd_c*)npcFgd)->setAnime(0);
        }
    }
    initMemberPos();
}

void daFmtMng_c::initMemberPos() {
    int i;
    int j;
    fopAc_ac_c* npcFgd;
    FmtMember_c* member;
    FmtPos_c* pos = mPos;
    pos->setPath(getPathID(), fopAcM_GetRoomNo(this), &current.pos);
    pos->getStartPoint(&home.pos, &home.angle);
    mDoMtx_stack_c::transS(home.pos);
    mDoMtx_stack_c::YrotM(home.angle.y);
    mDoMtx_stack_c::push();
    for (i = 0; i < mFormationRow; i++, pos++) {
        mDoMtx_stack_c::multVecZero(&pos->field_0x10);
        mDoMtx_stack_c::transM(0.0f, 0.0f, -attr().field_0x10);
        pos->setPath(getPathID(), fopAcM_GetRoomNo(this), &pos->field_0x10);
        pos->field_0x1c = home.angle.y;
    }
    member = mMember;
    mDoMtx_stack_c::pop();
    for (i = 0; i < mFormationRow; i++) {
        f32 dVar8 = ((mFormationLine - 1) * attr().field_0x0c) * 0.5f;
        for (j = 0; j < mFormationLine; j++, member++) {
            member->field_0x04.set(dVar8, 0.0f, 0.0f);
            mDoMtx_stack_c::multVec(&member->field_0x04, &member->field_0x04);
            member->field_0x10.set(member->field_0x04);
            dVar8 -= attr().field_0x0c;
            npcFgd = NULL;
            fopAcM_SearchByID(member->mNpcId, &npcFgd);
            if (npcFgd != NULL) {
                ((daNpcFgd_c*)npcFgd)->initPosAngle(member->field_0x04, home.angle.y);
            }
        }
        mDoMtx_stack_c::transM(0.0f, 0.0f, -attr().field_0x10);
    }
}

daFmtMng_c::ActionFunc daFmtMng_c::ActionTable[10] = {
    &daFmtMng_c::initWait, &daFmtMng_c::executeWait,
    &daFmtMng_c::initWalk, &daFmtMng_c::executeWalk,
    &daFmtMng_c::initReverse, &daFmtMng_c::executeReverse,
    &daFmtMng_c::initMotion, &daFmtMng_c::executeMotion,
    &daFmtMng_c::initFight, &daFmtMng_c::executeFight,

};

void daFmtMng_c::executeWait() {
    int time = getTime();
    if (time >= mStartTime && time < mEndTime) {
        if (isAllMemberCulling()) {
            FmtMember_c* member = mMember;
            for (int i = 0; i < mFormationRow * mFormationLine; i++, member++) {
                fopAc_ac_c* npcFgd = NULL;
                fopAcM_SearchByID(member->mNpcId, &npcFgd);
                if (npcFgd != NULL) {
                    ((daNpcFgd_c*)npcFgd)->onDrawFlag();
                }
            }
            setAction(MODE_1_e);
        }
    } else {
        FmtMember_c* member = mMember;
        fopAc_ac_c* npcFgd = NULL;
        for (int i = 0; i < mFormationRow * mFormationLine; i++, member++) {
            fopAcM_SearchByID(member->mNpcId, &npcFgd);
            if (npcFgd != NULL) {
                ((daNpcFgd_c*)npcFgd)->offDrawFlag();
            }
        }
        if (!dKy_darkworld_check() && daPy_py_c::checkNowWolf()) {
            fopAcM_delete(this);
        }
    }
}

void daFmtMng_c::initWalk() {
    FmtMember_c* member = mMember;
    fopAc_ac_c* npcFgd = NULL;
    for (int i = 0; i < mFormationRow * mFormationLine; i++, member++) {
        fopAcM_SearchByID(member->mNpcId, &npcFgd);
        if (npcFgd != NULL) {
            ((daNpcFgd_c*)npcFgd)->setAnime(2);
        }
    }
}

void daFmtMng_c::executeWalk() {
    setMoveSpeed();
    FmtPos_c* member = mPos;
    for (int i = 0; i < mFormationRow; i++, member++) {
        setMovePath(member, i);
        setMoveAngle(member);
        setMovePos(&member->field_0x10, &member->field_0x1c);
    }
    current.pos.set(mPos->field_0x10);
    correctFormation();
    setMemberPos();
}

void daFmtMng_c::setMovePath(FmtPos_c* i_pos, int param_2) {
    if (i_pos->checkPoint(i_pos->field_0x10, speedF)) {
        if (param_2 == 0) {
            field_0x598 = i_pos->getArg1();
        } else {
            field_0x598 = -1;
        }
        if (i_pos->checkPathEnd(i_pos->field_0x10, speedF)) {
            if (param_2 == 0) {
                int time = getTime();
                if (time > mStartTime && time < mEndTime) {
                    setAction(MODE_2_e);
                } else {
                    setAction(MODE_0_e);
                }
            } else {
                i_pos->chgDir();
            }
        } else if (field_0x598 >= 0) {
            setAction(MODE_3_e);
        }
        i_pos->setNextPoint(&i_pos->field_0x10);
    }
}

bool daFmtMng_Path_c::checkPathEnd(cXyz param_1, f32 param_2) {
    if (isPathClose()) {
        return false;
    }
    if (checkPoint(param_1, param_2)) {
        if (field_0xc > 0) {
            if (field_0x4 >= mPath->m_num - 1) {
                return true;
            }
        } else if (field_0x4 <= 0) {
            return true;
        }
    }
    return false;
}

bool daFmtMng_Path_c::checkPoint(cXyz i_point, f32 param_2) {
    f32 dVar8 = i_point.absXZ(dPath_GetPnt(mPath, field_0x4)->m_position);
    field_0x8 -= param_2;
    if (field_0x8 < 0.0f || dVar8 < param_2 || dVar8 < 10.0f) {
        return true;
    } 
    return false;
}

void daFmtMng_c::initReverse() {
    speedF = 0.0f;
    FmtMember_c* member = mMember;
    fopAc_ac_c* npcFgd = NULL;
    for (int i = 0; i < mFormationRow * mFormationLine; i++, member++) {
        fopAcM_SearchByID(member->mNpcId, &npcFgd);
        if (npcFgd != NULL) {
            member->field_0x1c = fopAcM_GetShapeAngle_p(npcFgd)->y + 0x8000;
        }
    }
}

void daFmtMng_c::executeReverse() {
    FmtMember_c* member;
    bool bVar1;
    FmtPos_c* tempPos;
    fopAc_ac_c* npcFgd;
    int i;
    member = mMember;
    bVar1 = true;
    npcFgd = NULL;
    for (i = 0; i < mFormationRow * mFormationLine; i++, member++) {
        fopAcM_SearchByID(member->mNpcId, &npcFgd);
        if (npcFgd != NULL) {
            csXyz* npcAngle = fopAcM_GetShapeAngle_p(npcFgd);
            if (cLib_distanceAngleS(npcAngle->y, member->field_0x1c) <= 0x800) {
                npcAngle->y = member->field_0x1c;
            } else {
                npcAngle->y -= 0x800;
                bVar1 = false;
            }
        } else {
            bVar1 = false;
        }
    }
    if (bVar1) {
        int i;
        FmtPos_c* currentPos;
        currentPos = mPos;
        tempPos = new FmtPos_c[mFormationRow];
        for (i = 0; i < mFormationRow; i++, currentPos++, tempPos++) {
            tempPos->field_0x10.set(currentPos->field_0x10);
            tempPos->field_0x1c = currentPos->field_0x1c;
        }
        currentPos = mPos;
        tempPos--;
        for (i = 0; i < mFormationRow; i++, currentPos++, tempPos--) {
            currentPos->field_0x10.set(tempPos->field_0x10);
            currentPos->field_0x1c = tempPos->field_0x1c + 0x8000;
            currentPos->chgDir();
            currentPos->setNextPoint(&currentPos->field_0x10);
        }
        delete [] (tempPos + 1);
        setAction(MODE_1_e);
    }
}

void daFmtMng_c::initMotion() {
    speedF = 0.0f;
    u32 arg0 = mPos->getArg0();
    u32 time = getTime();
    field_0x59c = time + arg0 * 10;
    mDayOfWeek = dKy_get_dayofweek();
    if (field_0x59c > 24 * 60) {
        field_0x59c -= 24 * 60;
        mDayOfWeek = (mDayOfWeek + 1) % 7;
    }
    int npcAnime = 0;
    switch(field_0x598) {
    case 0:
        npcAnime = 0;
        break;
    case 1:
        npcAnime = 1;
        break;
    case 2:
        npcAnime = 6;
        break;
    case 3:
        npcAnime = 7;
        break;
    }
    FmtMember_c* member = mMember;
    fopAc_ac_c* npcFgd = NULL;
    for (int i = 0; i < mFormationRow * mFormationLine; i++, member++) {
        fopAcM_SearchByID(member->mNpcId, &npcFgd);
        if (npcFgd != NULL) {
            ((daNpcFgd_c*)npcFgd)->setAnime(npcAnime);
        }
    }
}

void daFmtMng_c::executeMotion() {
    if (mDayOfWeek == dKy_get_dayofweek() && field_0x59c < getTime()) {
        FmtMember_c* member = mMember;
        fopAc_ac_c* npcFgd = NULL;
        for (int i = 0; i < mFormationRow * mFormationLine; i++, member++) {
            fopAcM_SearchByID(member->mNpcId, &npcFgd);
            if (npcFgd != NULL) {
                ((daNpcFgd_c*)npcFgd)->setAnime(2);
            }
        }
        mPrevAction = mCurrentAction;
        mCurrentAction = MODE_1_e;
        mAction = &((ActionFunc*)ActionTable)[mCurrentAction * 2];
        (this->*(*mAction))();
    }
}

void daFmtMng_c::initFight() {
    speedF = 0.0f;
    FmtMember_c* member = mMember;
    fopAc_ac_c* npcFgd = NULL;
    for (int i = 0; i < mFormationRow * mFormationLine; i++, member++) {
        fopAcM_SearchByID(member->mNpcId, &npcFgd);
        if (npcFgd != NULL) {
            ((daNpcFgd_c*)npcFgd)->setAnime(42);
            fopAcM_setCullSizeBox(npcFgd, -40.0f, -10.0f, -80.0f, 40.0f, 190.0f, 200.0f);
        }
    }

    dComIfGs_onSaveDunSwitch(60);
}

void daFmtMng_c::executeFight() {
    FmtMember_c* member = mMember;
    fopAc_ac_c* npcFgd = NULL;
    for (int i = 0; i < mFormationRow * mFormationLine; i++, member++) {
        fopAcM_SearchByID(member->mNpcId, &npcFgd);
        if (npcFgd != NULL) {
            s16 playerAngle = fopAcM_searchPlayerAngleY(npcFgd);
            csXyz* angle = fopAcM_GetShapeAngle_p(npcFgd);
            if (cLib_distanceAngleS(angle->y, (int)playerAngle) > 0x600) {
                ((daNpcFgd_c*)npcFgd)->setAnime(44);
            } else {
                ((daNpcFgd_c*)npcFgd)->setAnime(42);
            }
            cLib_addCalcAngleS2(&angle->y, (int)playerAngle, 3, 0x600);
        }
    }

    if (isAllMemberCulling()) {
        fopAcM_delete(this);
    }
}

static bool daFmtMng_Draw(daFmtMng_c* param_0) {
    return true;
}

static int daFmtMng_Execute(daFmtMng_c* param_0) {
    return param_0->execute();
}

static int daFmtMng_IsDelete(daFmtMng_c* param_0) {
    return 1;
}

static int daFmtMng_Delete(daFmtMng_c* param_1) {
    param_1->~daFmtMng_c();
    return 1;
}

static int daFmtMng_Create(fopAc_ac_c* param_0) {
    return static_cast<daFmtMng_c*>(param_0)->create();
}

int daFmtMng_c::create() {
    fopAcM_ct(this, daFmtMng_c);
    create_init();
    return 4;
}

void daFmtMng_c::create_init() {
    fopAcM_setCullSizeBox(this, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f);
    mRoomPath = dPath_GetRoomPath(getPathID(), fopAcM_GetRoomNo(this));
    field_0x580 = 0;
    mFormationLine = getFormationLine();
    mFormationRow = getFormationRow();
    mPos = NULL;
    mPos = new FmtPos_c[mFormationRow];
    JUT_ASSERT(357, mPos != NULL);
    mMember = NULL;
    mMember = new FmtMember_c[mFormationLine * mFormationRow];
    JUT_ASSERT(361, mMember != NULL);

    FmtMember_c* member = mMember;
    for (int i = 0; i < mFormationLine * mFormationRow; i++, member++) {
        member->mNpcId = -1;
    }

    mStartTime = (getStartTime() / 10) * 60 + (getStartTime() % 10) * 10; 
    mEndTime = (getEndTime() / 10) * 60 + (getEndTime() % 10) * 10;
    initMemberPos();
    bool inTime = false;
    int time = getTime();
    if (time > mStartTime && time < mEndTime) {
        mCurrentAction = MODE_1_e;
        inTime = true;
    } else {
        mCurrentAction = MODE_0_e;
    }
    mPrevAction = mCurrentAction;
    mAction = &((ActionFunc*)ActionTable)[mCurrentAction * 2];
    member = mMember;
    for (int i = 0; i < mFormationLine * mFormationRow; i++, member++) {
        member->mNpcId = fopAcM_createChild(0x294, fopAcM_GetID(this), (inTime << 0x10) | 0x200,
                                                &member->field_0x04, fopAcM_GetRoomNo(this),
                                                &home.angle, NULL, 0xffffffff, NULL);
    }
}

static actor_method_class l_daFmtMng_Method = {
    (process_method_func)daFmtMng_Create,
    (process_method_func)daFmtMng_Delete,
    (process_method_func)daFmtMng_Execute,
    (process_method_func)daFmtMng_IsDelete,
    (process_method_func)daFmtMng_Draw,
};

extern actor_process_profile_definition g_profile_FORMATION_MNG = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_FORMATION_MNG,     // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daFmtMng_c),     // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    410,                    // mPriority
    &l_daFmtMng_Method,     // sub_method
    0x40000,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
