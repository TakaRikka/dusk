/**
 * @file d_a_npc_passer2.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_passer2.h"
#include "d/d_s_play.h"

int daNpcPasser2_c::createHeap() {
    int rv = NpcCreate(m_type);

    if (rv != 0) {
        if (m_objNum != 0) {
            rv = NULL != (mpModel = ObjCreate(m_objNum));
            if (rv == 0) {
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
    daNpcPasser2_c* actor = (daNpcPasser2_c*)i_this;
    return actor->createHeap();
}

daNpcPasser2_c::actionFunc daNpcPasser2_c::ActionTable[1][2] = {
    {&daNpcPasser2_c::initPath, &daNpcPasser2_c::executePath},
};

void daNpcPasser2_c::setAction(daNpcPasser2_c::Mode_e i_action) {
    JUT_ASSERT(302, i_action < MODE_MAX_e);
    mPrevActionIdx = mActionIdx;
    mActionIdx = i_action;
    mAction = ActionTable[mActionIdx];
    callInit();
}

void daNpcPasser2_c::callInit() {
    JUT_ASSERT(320, mAction != NULL);
    (this->*mAction[0])();
}

void daNpcPasser2_c::callExecute() {
    JUT_ASSERT(333, mAction != NULL);
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

void daNpcPasser2_c::initPath() {
    if (getMoveType() == 1) {
        field_0xa3c = 1;
    } else if (getMoveType() == 2) {
        field_0xa3c = 0;
    } else {
        field_0xa3c = cLib_getRndValue(0, 2) != 0 ? 1 : 0;
    }

    int idx = 0;
    if (field_0xa3c == 1) {
        if (getWalkMotionType() == 1) {
            idx = 0;
        } else if (getWalkMotionType() == 2) {
            idx = 1;
        } else {
            idx = cLib_getRndValue(0, 2) == 0 ? 1 : 0;
        }
    } else if (field_0xa3c == 0) {
        if (getRunMotionType() == 1) {
            idx = 2;
        } else if (getRunMotionType() == 2) {
            idx = 3;
        } else {
            idx = cLib_getRndValue(0, 2) != 0 ? TWO : THREE;
        }
    }

    setAnm((J3DAnmTransformKey*)getAnmP(idx, m_objNum), 1.0f, 12.0f, J3DFrameCtrl::EMode_LOOP, 0, -1);
    field_0xa43 = 0;
    field_0xa42 = 1;
}

void daNpcPasser2_c::executePath() {
    if (m_path.checkPoint(current.pos, speedF)) {
        if (m_path.checkPathEnd(current.pos, speedF)) {
            fopAcM_delete(this);
        } else {
            m_path.setNextPoint(current.pos);
        }
    }

    if (field_0xa3c == 1 || field_0xa3c == 0) {
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
    }

    setAngle();
    pathMoveF();
    mAcch.CrrPos(dComIfG_Bgsp());
}

void daNpcPasser2_c::setAngle() {
    shape_angle.y = current.angle.y;
}

void daNpcPasser2_c::setSpeed(f32 param_1, f32 param_2, f32* i_speed, int param_4) {
    f32 target = field_0xa30 * (param_2 * field_0xa30);
    f32 step = field_0xa30 * (param_1 * field_0xa30);

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

void daNpcPasser2_c::pathMoveF() {
    f32 fVar1 = 0.0f;
    f32 speed = 0.0f;
    f32 fVar2 = 0.0f;
    cXyz* ccMoveP = mStts.GetCCMoveP();

    if (field_0xa3c == 1) {
        fVar1 = HIO_walkMaxSpeed(m_type);
        fVar2 = HIO_walkAnmPlaySpeed(m_type);

        cLib_chaseF(&field_0xa30, 0.8f, 0.05f);
        setSpeed(MREG_F(0) + 0.7f, fVar1, &field_0xa34, 0);
        setSpeed(MREG_F(0) + 0.7f, fVar1, &speedF, 1);
    } else if (field_0xa3c == 0) {
        fVar1 = HIO_maxSpeed(m_type);
        fVar2 = HIO_anmPlaySpeed(m_type);
        cLib_chaseF(&field_0xa30, 1.0f, 0.05f);
        setSpeed(MREG_F(0) + 0.7f, fVar1, &field_0xa34, 0);
        setSpeed(MREG_F(0) + 0.7f, fVar1, &speedF, 1);
    }

    cXyz targetPnt;
    m_path.getTargetPoint(&targetPnt);

    s32 angleS = cLib_distanceAngleS(current.angle.y, cLib_targetAngleY(&current.pos, &targetPnt));
    if (angleS >= 0x2000) {
        cLib_chaseF(&field_0xa30, 0.0f, 0.05f);
        cLib_chaseF(&field_0xa34, 0.0f, 1.2f);
        cLib_chaseF(&speedF, 0.0f, 1.2f);
    }

    fopAcM_posMoveF(this, ccMoveP);

    fVar1 = field_0xa34 / fVar1;
    if (fVar1 > 1.0f) {
        fVar1 = 1.0f;
    }

    speed = fVar2 * fVar1;
    if (speed < 0.6f) {
        speed = 0.6f;
    }

    mpMorf->setPlaySpeed(speed);
}

static int daNpcPasser2_Create(void* i_this) {
    return static_cast<daNpcPasser2_c*>(i_this)->create();
}

cPhs__Step daNpcPasser2_c::create() {
    fopAcM_ct(this, daNpcPasser2_c);

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

    if (m_type >= 30 || m_objNum >= 9) {
        return cPhs_ERROR_e;
    }

    if (m_type < 0 || m_type > 30) {
        // Failed to generate low-poly pedestrian actor! ---> m_type = %d
        OS_REPORT("ローポリ通行人アクター生成失敗しました！---> m_type = %d\n", m_type);
        return cPhs_ERROR_e;
    }

    m_objNum = getObjNum();
    if (m_objNum < 0 || m_objNum > 13) {
        // Failed to generate low-poly pedestrian actor! ---> m_objNum = %d
        OS_REPORT("ローポリ通行人アクター生成失敗しました！---> m_objNum = %d\n", m_objNum);
        return cPhs_ERROR_e;
    }

    cPhs__Step phase = (cPhs__Step)loadResrc(m_type, m_objNum);
    if (phase == cPhs_COMPLEATE_e) {
        u32 i_size = 0;
        #if DEBUG
        i_size = 0x15A0;
        #else
        i_size = 0x800015A0;
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

void daNpcPasser2_c::initCollision() {
    mStts.Init(0xD9, 0xFF, this);
    mStts.SetRoomId(fopAcM_GetRoomNo(this));
    mCyl.SetStts(&mStts);
    mCyl.Set(m_cylDat);
    mCyl.SetR(HIO_cylR(m_type));
    mCyl.SetH(HIO_cylH(m_type));
}

void daNpcPasser2_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, mPosY, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
    setAttention(m_type);
}

void daNpcPasser2_c::create_init() {
    #if DEBUG
    // NPC (C-class, D-class) Pedestrian High
    l_Cd_HIO.entryHIO("NPC(C級,D級)通行人 Low");
    #endif

    gravity = -3.0f;
    fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -40.0f, -10.0f, -30.0f, 30.0f, 190.0f, 40.0f);
    mAcchCir.SetWall(HIO_chkWallH(m_type), HIO_chkWallR(m_type));
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this),
              fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
    mAcch.SetRoofNone();
    mAcch.SetWaterNone();
    mAcch.SetWallNone();
    mAcch.CrrPos(dComIfG_Bgsp());

    setEnvTevCol();
    setRoomNo();

    initCollision();
    attention_info.flags = 0;

    if (m_path.setPath(getPathID(), fopAcM_GetRoomNo(this), 1, &current.pos, true)) {
        setAction(MODE_MAX_e);
    }

    mpMorf->setMorf(0.0f);
    mAcch.CrrPos(dComIfG_Bgsp());
    mPosY = current.pos.y = mAcch.GetGroundH();
    setBaseMtx();
}

static int daNpcPasser2_Delete(void* i_this) {
    static_cast<daNpcPasser2_c*>(i_this)->~daNpcPasser2_c();
    return 1;
}

daNpcPasser2_c::~daNpcPasser2_c() {
    if (m_type < 30 && m_objNum < 9) {
        removeResrc(m_type, m_objNum);
    }

    if (heap != NULL) {
        mpMorf->stopZelAnime();
    }
}

int daNpcPasser2_c::execute() {
    mAcchCir.SetWallH(HIO_chkWallH(m_type));
    mAcchCir.SetWallR(HIO_chkWallR(m_type));
    callExecute();
    cLib_addCalc2(&mPosY, current.pos.y, 0.4f, 25.0f);
    animation();
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

void daNpcPasser2_c::setCollision() {
    #if DEBUG
    mCyl.SetR(HIO_cylR(m_type));
    mCyl.SetH(HIO_cylH(m_type));
    #endif

    mCyl.SetC(current.pos);

    if (!fopAcM_CheckCondition(this, fopAcCnd_NODRAW_e)) {
        dComIfG_Ccsp()->Set(&mCyl);
    }
}

static int daNpcPasser2_Execute(void* i_this) {
    return static_cast<daNpcPasser2_c*>(i_this)->execute();
}

int daNpcPasser2_c::draw() {
    drawNpc();
    drawObj(m_objNum, mpModel, HIO_objScale(m_type));
    return 1;
}

static int daNpcPasser2_Draw(void* i_this) {
    return static_cast<daNpcPasser2_c*>(i_this)->draw();
}

static int daNpcPasser2_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpcPasser2_METHODS = {
    (process_method_func)daNpcPasser2_Create,
    (process_method_func)daNpcPasser2_Delete,
    (process_method_func)daNpcPasser2_Execute,
    (process_method_func)daNpcPasser2_IsDelete,
    (process_method_func)daNpcPasser2_Draw,
};

extern actor_process_profile_definition g_profile_NPC_PASSER2 = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_PASSER2,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpcPasser2_c), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  402,                    // mPriority
  &daNpcPasser2_METHODS,  // sub_method
  0x02040107,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
