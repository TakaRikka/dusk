/**
 * d_a_npc_zra.cpp
 * NPC - Zora
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_zra.h"
#include "SSystem/SComponent/c_math.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "Z2AudioLib/Z2Instances.h"
#include "f_op/f_op_actor_mng.h"
#include "f_op/f_op_kankyo_mng.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2_info.h"
#include "d/d_procname.h"
#include "d/actor/d_a_canoe.h"
#include "d/actor/d_a_npc_hoz.h"
#include "d/actor/d_a_obj_zraMark.h"

class daNpc_zrA_HIO_c {
public:
    daNpc_zrA_Param_c::param m;
};

#if DEBUG
#define HIO_PARAM(i_this) (i_this->mpHIO->m)
#else
#define HIO_PARAM(_) (daNpc_zrA_Param_c::m)
#endif

static daNpc_zrA_Param_c l_HIO;

daNpc_zrA_Param_c::param const daNpc_zrA_Param_c::m = {
    230.0f,   // mAttnOffsetY
    -3.0f,    // mGravity
    1.0f,     // mScale
    800.0f,   // mShadowDepth
    255.0f,   // mCcWeight
    230.0f,   // mCylH
    0.0f,     // mWallH
    40.0f,    // mWallR
    30.0f,    // mBodyUpAngle
    -20.0f,   // mBodyDownAngle
    30.0f,    // mBodyLeftAngle
    -30.0f,   // mBodyRightAngle
    15.0f,    // mHeadUpAngle
    0.0f,     // mHeadDownAngle
    0.0f,     // mHeadLeftAngle
    0.0f,     // mHeadRightAngle
    0.6f,     // mNeckAngleScl
    12.0f,    // mMorfFrames
    3,        // mSpeakDistIdx
    6,        // mSpeakAngleIdx
    5,        // mTalkDistIdx
    6,        // mTalkAngleIdx
    80.0f,    // mAttnFovY
    500.0f,   // mAttnRadius
    300.0f,   // mAttnUpperY
    -300.0f,  // mAttnLowerY
    60,
    8,        // mDamageTimer
    0,        // mTestExpression
    0,        // mTestMotion
    0,        // mTestLookMode
    false,    // mTest
    20.0f,    // mSwimSpeed
    0.5f,     // mMinSwimSpeedScale
    0x580,    // mSwimAngleSpeed
    15.0f,    // mSwimAnmRate
    0.0f,
    0.0f,
    1.5f,     // mMaxScaleFactor
    150.0f,   // mMinDepth
    100.0f,
    20.0f,
    4.0f,     // mWalkSpeed
    0x800,    // mWalkAngleSpeed
    2,        // mWalkAngleScale
    6.0f,     // mWalkAnmRate
    140.0f,
    350.0f,
    5000.0f,
    3000.0f,
};

BOOL daNpc_zrA_Path_c::getDstPosDst2(cXyz i_pos, cXyz& o_pnt) {
    BOOL ret = false;
    o_pnt = getPntPos(getIdx());
    if (chkPassedDst(i_pos)) {
        ret = true;
        if (!setNextIdx()) {
            o_pnt = getPntPos(getIdx());
            mPosDst = (i_pos - o_pnt).abs();
            mPosition = i_pos;
        }
    }
    return ret;
}

void daNpc_zrA_Path_c::setNextIdxDst(cXyz param_0) {
    if (!setNextIdx()) {
        mPosDst = (param_0 - getPntPos(getIdx())).abs();
        mPosition = param_0;
    }
}

BOOL daNpc_zrA_Path_c::chkPassedDstXZ(cXyz i_pos) {
    return mPosDst + field_0x10 <= (mPosition - i_pos).absXZ();
}

BOOL daNpc_zrA_Path_c::getDstPosDstXZ(cXyz i_pos, cXyz& o_pnt) {
    BOOL ret = false;
    o_pnt = getPntPos(getIdx());
    if (chkPassedDstXZ(i_pos)) {
        if (setNextIdx()) {
            ret = true;
        } else {
            o_pnt = getPntPos(getIdx());
            mPosDst = (i_pos - o_pnt).absXZ();
            mPosition = i_pos;
        }
    }
    return ret;
}

BOOL daNpc_zrA_Path_c::chkPassedChase(u16 i_idx, cXyz i_pos) {
    return daNpcF_chkPassed(i_pos, (dPnt*)mPathInfo->m_points, i_idx,
                            mPathInfo->m_num, mIsClosed, mIsReversed);
}

int daNpc_zrA_Path_c::getDstPosChase(u16 i_idx, cXyz i_pos, cXyz& o_pnt) {
    BOOL done = false;
    while (!done) {
        o_pnt = getPntPos(i_idx);
        if (!chkPassedChase(i_idx, i_pos)) {
            break;
        }

        if (mIsReversed) {
            if (i_idx == 0) {
                done = true;
            } else {
                i_idx--;
            }
        } else {
            if (i_idx == getEndIdx()) {
                done = true;
            } else {
                i_idx++;
            }
        }
    }
    return i_idx;
}

f32 daNpc_zrA_Path_c::chkPassDst(u16 i_idx, cXyz i_pos) {
    u16 prev_idx, next_idx;
    dPnt* points = mPathInfo->m_points;
    u16 idx = mIdx;
    u8 reversed = mIsReversed;
    mIdx = i_idx;
    mIsReversed = false;
    next_idx = getNextIdx();
    mIdx = i_idx;
    prev_idx = getBeforeIdx();
    mIdx = idx;
    mIsReversed = reversed;

    cXyz prev_pos, cur_pos, next_pos, pos;
    prev_pos.set(points[prev_idx].m_position.x,
                 points[prev_idx].m_position.y,
                 points[prev_idx].m_position.z);
    cur_pos.set(points[i_idx].m_position.x,
                points[i_idx].m_position.y,
                points[i_idx].m_position.z);
    next_pos.set(points[next_idx].m_position.x,
                 points[next_idx].m_position.y,
                 points[next_idx].m_position.z);

    f32 dist;
    s16 angle;
    if (prev_idx != i_idx || next_idx != i_idx) {
        if (prev_idx < i_idx && i_idx < next_idx) {
            dist = (next_pos - prev_pos).absXZ();
            angle = cM_atan2s(next_pos.x - prev_pos.x, next_pos.z - prev_pos.z);
            pos = prev_pos;
            prev_pos.x = pos.x + dist * -1.0f * cM_ssin(angle);
            prev_pos.z = pos.z + dist * -1.0f * cM_scos(angle);
            next_pos.x = pos.x + dist * 2.0f * cM_ssin(angle);
            next_pos.z = pos.z + dist * 2.0f * cM_scos(angle);
        } else if (prev_idx < i_idx) {
            dist = (cur_pos - prev_pos).absXZ();
            angle = cM_atan2s(cur_pos.x - prev_pos.x, cur_pos.z - prev_pos.z);
            pos = cur_pos;
            prev_pos.x = pos.x + dist * -2.0f * cM_ssin(angle);
            prev_pos.z = pos.z + dist * -2.0f * cM_scos(angle);
            next_pos.x = pos.x + dist * 2.0f * cM_ssin(angle);
            next_pos.z = pos.z + dist * 2.0f * cM_scos(angle);
        } else if (i_idx < next_idx) {
            dist = (next_pos - cur_pos).absXZ();
            angle = cM_atan2s(next_pos.x - cur_pos.x, next_pos.z - cur_pos.z);
            pos = cur_pos;
            prev_pos.x = pos.x + dist * -2.0f * cM_ssin(angle);
            prev_pos.z = pos.z + dist * -2.0f * cM_scos(angle);
            next_pos.x = pos.x + dist * 2.0f * cM_ssin(angle);
            next_pos.z = pos.z + dist * 2.0f * cM_scos(angle);
        }

        f32 proj_x, proj_z, proj2_x, proj2_z;
        daNpcF_pntVsLineSegmentLengthSquare2D(i_pos.x, i_pos.z, prev_pos.x, prev_pos.z,
                                              next_pos.x, next_pos.z, &proj_x, &proj_z, &dist);
        if (cM3d_IsZero(dist)) {
            return -G_CM3D_F_INF;
        } else {
            daNpcF_pntVsLineSegmentLengthSquare2D(cur_pos.x, cur_pos.z, prev_pos.x, prev_pos.z,
                                                next_pos.x, next_pos.z, &proj2_x, &proj2_z, &dist);
            s16 angle2;
            if (mIsReversed) {
                angle2 = cM_atan2s(prev_pos.x - next_pos.x, prev_pos.z - next_pos.z);
            } else {
                angle2 = cM_atan2s(next_pos.x - prev_pos.x, next_pos.z - prev_pos.z);
            }
            s16 angle_diff = angle2 - cM_atan2s(proj2_x - proj_x, proj2_z - proj_z);
            dist = JMAFastSqrt((proj2_x - proj_x) * (proj2_x - proj_x)
                                + (proj2_z - proj_z) * (proj2_z - proj_z));
            if ((u16)abs(angle_diff) > 0x4000) {
                return dist;
            } else {
                return -dist;
            }
        }
    } else {
        return -G_CM3D_F_INF;
    }
}

static daNpc_GetParam1 l_bmdGetParamList[6] = {
    {3, 4},   // zra
    {13, 5},  // zra_tw
    {3, 7},   // zra_met
    {4, 7},   // zra_sp
    {3, 10},  // zra_met_tw
    {4, 10},  // zra_sp_tw
};

static daNpc_GetParam1 l_bckGetParamList[63] = {
    {-1, 0},
    {8, 0},   // zra_f_talk_a
    {9, 0},   // zra_f_talk_a_sp
    {11, 0},  // zra_f_wait_swim
    {10, 0},  // zra_f_talk_nomal
    {13, 0},  // zra_fh_talk_nomal
    {5, 5},   // zra_fh_sadsit_a
    {4, 6},   // zra_fh_sadsit_b
    {5, 6},   // zra_fh_sadsit_c
    {6, 5},   // zra_fh_sadsit_d
    {7, 5},   // zra_fh_sadsit_e
    {5, 3},   // zra_f_looking_sp
    {8, 3},   // zra_fh_looking_sp
    {4, 9},   // zra_f_lookup
    {7, 9},   // zra_fh_lookup
    {7, 3},   // zra_f_talk_swim_sp
    {6, 3},   // zra_f_talk_b_sp
    {5, 9},   // zra_f_spa_talk_a
    {8, 9},   // zra_fh_spa_wait_a
    {6, 9},   // zra_f_spa_talk_b
    {9, 9},   // zra_fh_spa_wait_b
    {17, 0},  // zra_swim_turn
    {14, 0},  // zra_still
    {12, 0},  // zra_fallswim
    {16, 0},  // zra_swim_talk
    {8, 5},   // zra_sadsit_a
    {6, 6},   // zra_sadsit_b
    {7, 6},   // zra_sadsit_c
    {9, 5},   // zra_sadsit_d
    {10, 5},  // zra_sadsit_e
    {4, 8},   // zra_tobikomi_s
    {5, 8},   // zra_tobikomi_t
    {3, 8},   // zra_tobikomi_e
    {11, 3},  // zra_looking_sp
    {17, 3},  // zra_talk_swim_sp
    {16, 3},  // zra_talk_b_sp
    {10, 9},  // zra_lookup
    {14, 9},  // zra_spa_wait_b
    {12, 9},  // zra_spa_talk_b
    {13, 9},  // zra_spa_wait_a
    {11, 9},  // zra_spa_talk_a
    {5, 2},   // zra_wait_a
    {10, 1},  // zra_walk_a
    {4, 2},   // zra_talk_a
    {7, 1},   // zra_swim_a
    {8, 1},   // zra_swim_b
    {9, 1},   // zra_wait_swim
    {3, 1},   // zra_dive
    {4, 1},   // zra_dive_b
    {5, 1},   // zra_float
    {6, 1},   // zra_float_b
    {3, 2},   // zra_step
    {18, 3},  // zra_wait_sp
    {20, 3},  // zra_walk_a_sp
    {15, 3},  // zra_talk_a_sp
    {13, 3},  // zra_swim_a_sp
    {14, 3},  // zra_swim_b_sp
    {19, 3},  // zra_wait_swim_sp
    {4, 3},   // zra_dive_sp
    {3, 3},   // zra_dive_b_sp
    {10, 3},  // zra_float_sp
    {9, 3},   // zra_float_b_sp
    {12, 3},  // zra_step_sp
};

static daNpc_GetParam1 l_btpGetParamList[8] = {
    {34, 0},  // zra
    {16, 5},  // zra_fh_sadsit_a
    {10, 6},  // zra_fh_sadsit_b
    {11, 6},  // zra_fh_sadsit_c
    {17, 5},  // zra_fh_sadsit_d
    {18, 5},  // zra_fh_sadsit_e
    {17, 9},  // zra_f_spa_talk_b
    {18, 9},  // zra_fh_spa_wait_b
};

static daNpc_GetParam1 l_btkGetParamList[4] = {
    {28, 0},  // zra
    {31, 0},  // zra_water02
    {29, 0},  // zra_reset
    {30, 0},  // zra_w_eyeball
};

static daNpc_GetParam1 l_brkGetParamList[2] = {
    {24, 0},  // zra
    {25, 0},  // zra_water02
};

static daNpc_GetParam1 l_bpkGetParamList[2] = {
    {20, 0},  // zra
    {21, 0},  // zra_water02
};

static daNpc_GetParam1 l_evtGetParamList[13] = {
    {0, 0},
    {1, 0},
    {2, 0},
    {3, 0},
    {4, 0},
    {5, 0},
    {6, 0},
    {7, 0},
    {8, 0},
    {9, 0},
    {10, 0},
    {11, 0},
    {12, 0},
};

static char* l_evtNames[13] = {
    NULL,
    "TALK_SWIM",
    "BEFORE_BLAST_ZRR",
    "AFTER_BLAST_ZRR",
    "THANKS_BLAST",
    "RESULT_ANNOUNCE",
    "CARRY_WATERFALL",
    "CARRY_WATERFALL_NIGHT",
    "CARRY_WATERFALL_SKIP",
    "CARRY_WATERFALL_NIGHT_SKIP",
    "SEARCH_PRINCE",
    "TALK_MULTI",
    "TALK_MULTI2",
};

static int l_loadObj_list[3][3] = {
    {2, 3, -1},
    {2, -1, -1},
    {-1, -1, -1},
};

static int l_loadObj_listTW[3][3] = {
    {4, 5, -1},
    {4, -1, -1},
    {-1, -1, -1},
};

static int l_loadRes_ZRAa[8] = {0, 1, 2, 4, 6, -1, -1, -1};

static int l_loadRes_Swim[8] = {0, 1, 2, 4, -1, -1, -1, -1};

static int l_loadRes_Tobi[8] = {0, 1, 2, 4, 8, -1, -1, -1};

static int l_loadRes_Spa[8] = {0, 2, 4, 9, -1, -1, -1, -1};

static int l_loadRes_ZRA0[8] = {0, 1, 2, 4, -1, -1, -1, -1};

static int* l_loadRes_list[8] = {
    l_loadRes_ZRAa, l_loadRes_Swim, l_loadRes_Swim, l_loadRes_Swim,
    l_loadRes_Tobi, l_loadRes_Tobi, l_loadRes_Spa,  l_loadRes_ZRA0,
};

static char* l_resNames[11] = {
    "zrA",
    "zrA_nml",
    "zrA_nml2",
    "zrA_sp",
    "zrA_MDL",
    "zrA_TW",
    "zrA_sad",
    "zrA_obj",
    "zrA_tobi",
    "zrA2",
    "zrA_objTW",
};

static char* l_myName[4] = {
    "zrA",
    "zrA_talk",
    "zrR",
    "zrWF",
};

char* daNpc_zrA_c::mEvtCutNameList[11] = {
    "",
    "TALK_SWIM",
    "BEFORE_BLAST_ZRR",
    "AFTER_BLAST_ZRR",
    "THANKS_BLAST",
    "RESULT_ANNOUNCE",
    "CARRY_WATERFALL",
    "CARRY_WATERFALL_SKIP",
    "SEARCH_PRINCE1",
    "SEARCH_PRINCE2",
    "TALK_MULTI",
};

daNpc_zrA_c::EventFn daNpc_zrA_c::mEvtCutList[11] = {
    NULL,
    &daNpc_zrA_c::ECut_talkSwim,
    &daNpc_zrA_c::ECut_beforeBlastzrR,
    &daNpc_zrA_c::ECut_afterBlastzrR,
    &daNpc_zrA_c::ECut_thanksBlast,
    &daNpc_zrA_c::ECut_resultAnnounce,
    &daNpc_zrA_c::ECut_carryWaterfall,
    &daNpc_zrA_c::ECut_carryWaterfallSkip,
    &daNpc_zrA_c::ECut_searchPrince1,
    &daNpc_zrA_c::ECut_searchPrince2,
    &daNpc_zrA_c::ECut_talkMulti,
};

daNpc_zrA_c::daNpc_zrA_c() {}

daNpc_zrA_c::~daNpc_zrA_c() {
    int i;
    for (i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        int res_no = l_loadRes_list[mType][i];
        if (res_no == 4) {
            if (mTwilight) {
                res_no = 5;
            }
        } else if (res_no == 1) {
            if (mSoldierType == SOLDIER_SPEAR) {
                res_no = 3;
            }
        }
        dComIfG_resDelete(&mPhase[i], l_resNames[res_no]);
    }

    if (mSoldierType != SOLDIER_NONE) {
        if (mTwilight) {
            dComIfG_resDelete(&mPhase[i], l_resNames[10]);
        } else {
            dComIfG_resDelete(&mPhase[i], l_resNames[7]);
        }
    }

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }
}

cPhs__Step daNpc_zrA_c::create() {
    fopAcM_ct(this, daNpc_zrA_c);

    mType = getTypeFromArgument();
    if (home.angle.x != 0xffff) {
        mFlowID = home.angle.x;
    } else {
        mFlowID = -1;
    }
    mSoldierType = getSoldierTypeFromParam();
    mGameMode = getGameModeFromParam();
    mSwitch1 = (u8)home.angle.z;
    mSwitch2 = (u8)((u16)home.angle.z >> 8);
    mTwilight = dKy_darkworld_check();

    if (isDelete()) {
        return cPhs_ERROR_e;
    }

    int res_count = 0;
    cPhs__Step step;
    int i;
    for (i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        int res_no = l_loadRes_list[mType][i];
        if (res_no == 4) {
            if (mTwilight) {
                res_no = 5;
            }
        } else if (res_no == 1) {
            if (mSoldierType == SOLDIER_SPEAR) {
                res_no = 3;
            }
        }
        
        step = (cPhs__Step)dComIfG_resLoad(&mPhase[i], l_resNames[res_no]);

        if (step == cPhs_ERROR_e || step == cPhs_UNK3_e) {
            return cPhs_ERROR_e;
        }
        if (step == cPhs_COMPLEATE_e) {
            res_count++;
        }
    }

    if (mSoldierType != SOLDIER_NONE) {
        if (mTwilight) {
            step = (cPhs__Step)dComIfG_resLoad(&mPhase[i], l_resNames[10]);
        } else {
            step = (cPhs__Step)dComIfG_resLoad(&mPhase[i], l_resNames[7]);
        }

        if (step == cPhs_ERROR_e || step == cPhs_UNK3_e) {
            return cPhs_ERROR_e;
        }
        if (step == cPhs_COMPLEATE_e) {
            res_count++;
        }

        i++;
    }

    if (res_count == i) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x80007280)) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        mCreatureSound.init(&current.pos, &eyePos, 3, 1);
        mAcchCir.SetWall(daNpc_zrA_Param_c::m.mWallR, daNpc_zrA_Param_c::m.mWallH);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mPaPo.init(&mAcch, daNpc_zrA_Param_c::m.mCylH, daNpc_zrA_Param_c::m.mCylH);
        mCcStts.Init(daNpc_zrA_Param_c::m.mCcWeight, 0, this);
        mCcCyl.Set(mCcDCyl);
        mCcCyl.SetStts(&mCcStts);
        mCcCyl.SetTgHitCallback(tgHitCallBack);
        mAcch.SetWtrChkMode(2);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        setEnvTevColor();
        setRoomNo();
        reset();
        Execute();
        
        return cPhs_COMPLEATE_e;
    }

    return cPhs_INIT_e;
}

int daNpc_zrA_c::CreateHeap() {
    J3DModelData* model_data = NULL;
    u32 model_flag;
    if (mTwilight) {
        if (l_bmdGetParamList[1].fileIdx >= 0) {
            model_data = static_cast<J3DModelData*>(
                dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[1].arcIdx],
                                     l_bmdGetParamList[1].fileIdx));
        }
        model_flag = 0x80000;
    } else {
        if (l_bmdGetParamList[0].fileIdx >= 0) {
            model_data = static_cast<J3DModelData*>(
                dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[0].arcIdx],
                                     l_bmdGetParamList[0].fileIdx));
        }
        model_flag = 0;
    }
    mAnm_p = new mDoExt_McaMorfSO(model_data, NULL, NULL, NULL, -1, 1.0f, 0, -1,
                                  &mCreatureSound, model_flag, 0x11020285);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }
    if (mAnm_p == NULL) {
        return 0;
    }

    J3DModel* model = mAnm_p->getModel();
    for (u16 i = 0; i < model_data->getJointNum(); i++) {
        switch (i) {
        case 0:
        case 1:
        case 3:
        case 4:
        case 5:
        case 14:
        case 29:
            model_data->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
            break;
        default:
            model_data->getJointNodePointer(i)->setCallBack(NULL);
        }
    }
    model->setUserArea((uintptr_t)this);

    mpMatAnm = new daNpcF_MatAnm_c();
    if (mpMatAnm == NULL) {
        return 0;
    }

    setMotionWaterAnm(1);
    if (!setExpressionAnm(ANM_F_TALK_A, false)) {
        return 0;
    }
    setMotionAnm(ANM_WAIT_A, 0.0f);

    if (mSoldierType != SOLDIER_NONE) {
        for (int i = 0; i < 3; i++) {
            int index;
            if (mTwilight) {
                index = l_loadObj_listTW[mSoldierType][i];
            } else {
                index = l_loadObj_list[mSoldierType][i];
            }

            if (index > 0) {
                model_data = static_cast<J3DModelData*>(
                    dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[index].arcIdx],
                                        l_bmdGetParamList[index].fileIdx));
                if (model_data != NULL) {
                    mpObjectModel[i] = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
                    if (mpObjectModel[i] == NULL) {
                        return 0;
                    }
                } else {
                    return 0;
                }
            } else {
                mpObjectModel[i] = NULL;
            }
        }
    }

    return 1;
}

int daNpc_zrA_c::Delete() {
    this->~daNpc_zrA_c();
    return 1;
}

int daNpc_zrA_c::Execute() {
    return execute();
}

int daNpc_zrA_c::Draw() {
    BOOL bvar2 = false;
    J3DModel* model = mAnm_p->getModel();
    J3DModelData* model_data = model->getModelData();
    model_data->getMaterialNodePointer(1)->setMaterialAnm(mpMatAnm);

    if (!mHide) {
        if (mTwilight) {
            bvar2 = false;
        } else {
            bvar2 = true;
        }
    }

    if (!checkHide()) {
        if (mTwilight) {
            g_env_light.settingTevStruct(4, &current.pos, &tevStr);
        } else {
            g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        }
        g_env_light.setLightTevColorType_MAJI(model, &tevStr);

        if (!drawDbgInfo()) {
            if (mWaterAnmFlags & ANM_PLAY_BTK) {
                mWaterBtkAnm.entry(model_data);
            }
            if (mWaterAnmFlags & ANM_PLAY_BPK) {
                mWaterBpkAnm.entry(model_data);
            }
            if (mAnmFlags & ANM_PLAY_BTP) {
                mBtpAnm.entry(model_data);
            }
            if (mAnmFlags & ANM_PLAY_BTK) {
                mBtkAnm.entry(model_data);
            }
            if (mAnmFlags & ANM_PLAY_BRK) {
                mBrkAnm.entry(model_data);
            }

            if (bvar2) {
                fopAcM_setEffectMtx(this, model_data);
            }

            if (mTwilight) {
                dComIfGd_setListDark();
                mAnm_p->entryDL();
                dComIfGd_setList();
            } else {
                mAnm_p->entryDL();
            }

            if (mAnmFlags & ANM_PLAY_BTP) {
                mBtpAnm.remove(model_data);
            }
            if (mAnmFlags & ANM_PLAY_BTK) {
                mBtkAnm.remove(model_data);
            }
            if (mAnmFlags & ANM_PLAY_BRK) {
                mBrkAnm.remove(model_data);
            }
            if (mWaterAnmFlags & ANM_PLAY_BPK) {
                mWaterBpkAnm.remove(model_data);
            }
            if (mWaterAnmFlags & ANM_PLAY_BTK) {
                mWaterBtkAnm.remove(model_data);
            }

            mShadowKey = dComIfGd_setShadow(mShadowKey, 1, model, &current.pos,
                                            daNpc_zrA_Param_c::m.mShadowDepth, 20.0f,
                                            current.pos.y, mGroundH, mGndChk, &tevStr,
                                            0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
            
            drawOtherMdls();
        }
    }

    return 1;
}

int daNpc_zrA_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jnt_no = i_joint->getJntNo();
    Mtx mtx1, mtx2;
    cXyz pos;

    if (mSwimMode != SWIM_WAIT) {
        mDoMtx_stack_c::copy(i_model->getAnmMtx(jnt_no));

        switch (jnt_no) {
        case 29:
            calcWaistAngle();
            MTXCopy(mDoMtx_stack_c::get(), mtx1);
            pos.set(mtx1[0][3], mtx1[1][3], mtx1[2][3]);
            mtx1[0][3] = mtx1[1][3] = mtx1[2][3] = 0.0f;

            mDoMtx_stack_c::ZXYrotS(mCurAngle);
            mDoMtx_stack_c::inverse();
            MTXCopy(mDoMtx_stack_c::get(), mtx2);

            mDoMtx_stack_c::transS(pos);
            mDoMtx_stack_c::ZXYrotM(mCurAngle);
            mDoMtx_stack_c::ZXYrotM(mWaistAngle);
            mDoMtx_stack_c::concat(mtx2);
            mDoMtx_stack_c::concat(mtx1);
            break;
        }

        i_model->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
        MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
        return 1;
    }

    int lookat_joints[3] = {1, 3, 4};

    if (jnt_no == 0) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(1));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(3));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(jnt_no));

    switch (jnt_no) {
    case 1:
    case 3:
    case 4:
        setLookatMtx(jnt_no, lookat_joints, daNpc_zrA_Param_c::m.mNeckAngleScl);
        break;
    }

    if (jnt_no == 1) {
        mDoMtx_stack_c::YrotM(field_0x908[0].z);
        mDoMtx_stack_c::ZrotM(-field_0x908[0].x);
    } else if (jnt_no == 4) {
        mDoMtx_stack_c::YrotM(field_0x908[2].z);
        mDoMtx_stack_c::ZrotM(field_0x908[2].x);
    }

    i_model->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    if ((jnt_no == 4 || jnt_no == 14) && (mAnmFlags & ANM_PLAY_BCK)) {
        J3DAnmTransform* bck_anm = mBckAnm.getBckAnm();
        mBckAnm.changeBckOnly(mAnm_p->getAnm());
        mAnm_p->changeAnm(bck_anm);
    }

    return 1;
}

int daNpc_zrA_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_zrA_c*>(i_this)->CreateHeap();
}

int daNpc_zrA_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_zrA_c* _this = (daNpc_zrA_c*)model->getUserArea();
        if (_this != NULL) {
            _this->ctrlJoint(i_joint, model);
        }
    }
    return 1;
}

void daNpc_zrA_c::setParam() {
    u32 attn_flags = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
    BOOL in_water = mAcch.ChkWaterIn();
    if (mTwilight) {
        attn_flags = (fopAc_AttnFlag_UNK_0x800000 | fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
    }

    selectAction();

    if (!mTwilight && daPy_py_c::checkNowWolf()) {
        attn_flags = 0;
    }

    if (field_0x1521) {
        field_0x14d0 = 0;
        field_0x14d4 = 0;

        attn_flags = fopAc_AttnFlag_TALK_e;
        if (daPy_getPlayerActorClass()->checkSwimUp()) {
            attn_flags = 0;
        }
        if (field_0x153c == true || mSwimMode == SWIM_TURN) {
            attn_flags = 0;
        }
        
        attention_info.distances[fopAc_attn_LOCK_e] = 64;
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = 63;
    } else {
        if (mType != TYPE_WAIT && mType != TYPE_SPA && mType != TYPE_SEARCH
            && (mType != TYPE_WATERFALL || mActionSelect != 0))
        {
            attn_flags = 0;
        }

        field_0x14d0 = 0;
        field_0x14d4 = 0;

        attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(5, 6);
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(3, 6);
    }

    attention_info.flags = attn_flags;

    scale.set(daNpc_zrA_Param_c::m.mScale,
              daNpc_zrA_Param_c::m.mScale,
              daNpc_zrA_Param_c::m.mScale);
    if (mSwimMode != SWIM_WAIT) {
        scale *= mScaleFactor;
    }

    mAcchCir.SetWallR(daNpc_zrA_Param_c::m.mWallR);
    mAcchCir.SetWallH(daNpc_zrA_Param_c::m.mWallH);

    if ((mType == TYPE_WAIT && !in_water) || mType == TYPE_SPA) {
        gravity = daNpc_zrA_Param_c::m.mGravity;
    }
}

BOOL daNpc_zrA_c::main() {
    if (!doEvent()) {
        mOrderEvtNo = EVT_NONE;
        if (mpNextActionFn != NULL) {
            if (mpActionFn == mpNextActionFn) {
                (this->*mpActionFn)(NULL);
            } else {
                setAction(mpNextActionFn);
            }
        }
    }

    if (checkHide()) {
        attention_info.flags = 0;
    }

    if (!daNpc_zrA_Param_c::m.mTest
        && (!dComIfGp_event_runCheck() || (mOrderNewEvt && dComIfGp_getEvent()->isOrderOK())))
    {
        if (mOrderEvtNo != EVT_NONE) {
            eventInfo.setArchiveName(l_resNames[l_evtGetParamList[mOrderEvtNo].arcIdx]);
        }
        orderEvent(mOrderSpeakEvt, l_evtNames[l_evtGetParamList[mOrderEvtNo].fileIdx],
                   0xffff, 4, 0xff, 2);
        if (!mTwilight && mType == TYPE_WAIT && !field_0x1550) {
            eventInfo.onCondition(dEvtCnd_CANTALKITEM_e);
        }
    }

    if (field_0x9ee) {
        mExpressionMorfOverride = 0.0f;
        mMotionMorfOverride = 0.0f;
        field_0x9ee = false;
    }

    playExpression();
    playMotion();

    if (mAcch.ChkWaterIn()) {
        setMotionWaterAnm(1);
    } else {
        setMotionWaterAnm(0);
    }

    if (mWaterAnmFlags & ANM_PLAY_BTK) {
        f32 speed = mWaterBtkAnm.getPlaySpeed();
        if (mWaterAnmFlags & ANM_PAUSE_BTK) {
            mWaterBtkAnm.setPlaySpeed(0.0f);
        }
        mWaterBtkAnm.play();
        mWaterBtkAnm.setPlaySpeed(speed);
    }

    if (mWaterAnmFlags & ANM_PLAY_BPK) {
        f32 speed = mWaterBpkAnm.getPlaySpeed();
        if (mWaterAnmFlags & ANM_PAUSE_BPK) {
            mWaterBpkAnm.setPlaySpeed(0.0f);
        }
        mWaterBpkAnm.play();
        mWaterBpkAnm.setPlaySpeed(speed);
    }

    mWaterAnmFlags &= ~(ANM_PAUSE_ALL | ANM_PAUSE_BPK);

    return true;
}

BOOL daNpc_zrA_c::ctrlBtk() {
    if (mpMatAnm != NULL) {
        J3DAnmTextureSRTKey* btk_anm = NULL;
        if (l_btkGetParamList[mBtkID].fileIdx >= 0) {
            btk_anm = getTexSRTKeyAnmP(l_resNames[l_btkGetParamList[mBtkID].arcIdx],
                                       l_btkGetParamList[mBtkID].fileIdx);
        }

        if (btk_anm == mBtkAnm.getBtkAnm()) {
            if (mBtkID == 0) {
                mpMatAnm->setNowOffsetX(cM_ssin(mEyeAngle.y) * 0.2f * -1.0f);
            } else {
                mpMatAnm->setNowOffsetX(cM_ssin(mEyeAngle.y) * 0.2f);
            }
            mpMatAnm->setNowOffsetY(cM_ssin(mEyeAngle.x) * 0.12f);
            mpMatAnm->onEyeMoveFlag();
            return true;
        }
        mpMatAnm->offEyeMoveFlag();
    }

    return false;
}

void daNpc_zrA_c::setAttnPos() {
    static cXyz eyeOffset(-20.0f, 10.0f, 0.0f);
    f32 offset = daNpc_zrA_Param_c::m.mAttnOffsetY;
    cXyz center, vec2, vec3, vec4, vec5;

    mDoMtx_stack_c::YrotS(field_0x990);

    cLib_addCalc2(&field_0x984[0], 0.0f, 0.1f, 125.0f);
    cLib_addCalc2(&field_0x984[2], 0.0f, 0.1f, 125.0f);

    for (int i = 0; i < 3; i++) {
        vec3.set(0.0f, 0.0f, field_0x984[i] * cM_ssin(field_0x992));
        mDoMtx_stack_c::multVec(&vec3, &vec4);
        field_0x908[i].x = -vec4.z;
        field_0x908[i].z = -vec4.x;
    }

    cLib_chaseS(&field_0x992, 0, 0x555);

    if (mLookMode == LOOK_RESET) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    J3DModelData* model_data = mAnm_p->getModel()->getModelData();

    for (u16 i = 0; i < model_data->getJointNum(); i++) {
        switch (i) {
        case 0:
        case 1:
        case 3:
        case 4:
        case 5:
        case 14:
        case 29:
            model_data->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
            break;
        default:
            model_data->getJointNodePointer(i)->setCallBack(NULL);
            break;
        }
    }

    setMtx();
    lookat();
    setPrtcl();

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&eyeOffset, &eyePos);
    eyeOffset.y = 0.0f;
    mDoMtx_stack_c::multVec(&eyeOffset, &vec3);
    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &vec3);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &vec3);

    cXyz* attn_pos = mLookat.getAttnPos();
    if (attn_pos != NULL) {
        vec2 = *attn_pos - eyePos;
        mEyeAngle.y = -(mLookatAngle[2].y + mCurAngle.y);
        mEyeAngle.y += cM_atan2s(vec2.x, vec2.z);
        mEyeAngle.x = -cM_atan2s(vec2.y, vec2.absXZ());
        mEyeAngle.x += mHeadAngle.x;
    } else {
        mEyeAngle.x = mEyeAngle.y = 0;
    }

    f32 extra_height = 0.0f;
    f32 extra_radius = 0.0f;
    vec2.set(0.0f, 0.0f, 0.0f);
    if ((mType == TYPE_WAIT || mType == TYPE_SPA) && mActionType != ACT_TYPE_0) {
        if (mType == TYPE_SPA || (mType == TYPE_WAIT
                                && (mActionType == ACT_TYPE_1
                                    || mActionType == ACT_TYPE_2
                                    || mActionType == ACT_TYPE_3)))
        {
            mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(29));
            mDoMtx_stack_c::multVecZero(&center);
            center.y = current.pos.y;
            extra_radius = 70.0f;
        } else {
            center = current.pos;
        }

        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&vec5);
        attention_info.position.set(vec5.x, vec5.y + 40.0f, vec5.z);
    } else {
        if (mType == TYPE_TOBIKOMI) {
            extra_height = 700.0f;
            extra_radius = 800.0f;
        } else if (mType == TYPE_SWIM && mPath.getPathInfo() != NULL) {
            extra_height = -150.0f;
        }
        
        center = current.pos;
        attention_info.position.set(center.x, center.y + offset, center.z);
    }

    if (!mHide) {
        if (mDamageTimer == 0) {
            if (mType == TYPE_TOBIKOMI) {
                mCcCyl.SetTgType(0x2020);
                mCcCyl.SetTgSPrm(0x3f);
                mCcCyl.OnTgNoHitMark();
            } else {
                mCcCyl.SetTgType(0xd8fbfdff);
                mCcCyl.SetTgSPrm(0x1f);
                mCcCyl.OnTgNoHitMark();
            }
        } else {
            mCcCyl.SetTgType(0);
            mCcCyl.SetTgSPrm(0);
        }

        if (mType == TYPE_TOBIKOMI) {
            mCcCyl.SetCoSPrm(0x379);
        } else {
            switch (mOrderEvtNo) {
            case EVT_CARRY_WATERFALL:
            case EVT_CARRY_WATERFALL_NIGHT:
                mCcCyl.SetCoSPrm(0x179);
                break;
            default:
                mCcCyl.SetCoSPrm(0x79);
                break;
            }
        }

        mCcCyl.SetC(center);
        mCcCyl.SetH(daNpc_zrA_Param_c::m.mCylH + extra_height);
        mCcCyl.SetR(daNpc_zrA_Param_c::m.mWallR + extra_radius);
        dComIfG_Ccsp()->Set(&mCcCyl);
    }

    mCcCyl.ClrAtHit();
    mCcCyl.ClrTgHit();
    mCcCyl.ClrCoHit();
}

void daNpc_zrA_c::setMtx() {
    J3DModel* model = mAnm_p->getModel();
    cXyz pos = current.pos;
    pos += mModulationOffset;
    mDoMtx_stack_c::transS(pos);
    mDoMtx_stack_c::ZXYrotM(mCurAngle);
    mDoMtx_stack_c::scaleM(scale);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    model->setUserArea((uintptr_t)this);

    if (mAnmFlags & ANM_PLAY_BCK) {
        mBckAnm.getBckAnm()->setFrame(mBckAnm.getFrame());
        mAnm_p->modelCalc();
    } else {
        mAnm_p->modelCalc();
    }
}

bool daNpc_zrA_c::setExpressionAnm(int i_idx, bool i_modify) {
    J3DAnmTransform* bck_anm = NULL;
    int attr = J3DFrameCtrl::EMode_NONE;
    mAnmFlags &= ~ANM_EXPRESSION_FLAGS;

    if (l_bckGetParamList[i_idx].fileIdx >= 0) {
        bck_anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_idx].arcIdx],
                                    l_bckGetParamList[i_idx].fileIdx);
    }

    bool res = false;
    switch (i_idx) {
    case ANM_NONE:
        res = setExpressionBtp(0);
        break;
    case ANM_F_TALK_A:
        res = setExpressionBtp(0);
        break;
    case ANM_F_TALK_A_SP:
        res = setExpressionBtp(0);
        break;
    case ANM_F_WAIT_SWIM:
        res = setExpressionBtp(0);
        break;
    case ANM_F_TALK_NOMAL:
        res = setExpressionBtp(0);
        break;
    case ANM_FH_TALK_NOMAL:
        res = setExpressionBtp(0);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_FH_SADSIT_A:
        res = setExpressionBtp(1);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_FH_SADSIT_B:
        res = setExpressionBtp(2);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_FH_SADSIT_C:
        res = setExpressionBtp(3);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_FH_SADSIT_D:
        res = setExpressionBtp(4);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_FH_SADSIT_E:
        res = setExpressionBtp(5);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_F_LOOKING_SP:
        res = setExpressionBtp(0);
        break;
    case ANM_FH_LOOKING_SP:
        res = setExpressionBtp(0);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_F_LOOKUP:
        res = setExpressionBtp(0);
        break;
    case ANM_FH_LOOKUP:
        res = setExpressionBtp(0);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_F_TALK_SWIM_SP:
        res = setExpressionBtp(0);
        break;
    case ANM_F_TALK_B_SP:
        res = setExpressionBtp(0);
        break;
    case ANM_F_SPA_TALK_A:
        res = setExpressionBtp(0);
        break;
    case ANM_FH_SPA_WAIT_A:
        res = setExpressionBtp(0);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_F_SPA_TALK_B:
        res = setExpressionBtp(6);
        break;
    case ANM_FH_SPA_WAIT_B:
        res = setExpressionBtp(7);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    default:
        bck_anm = NULL;
        break;
    }

    if (!res) {
        return false;
    }

    if (bck_anm == NULL) {
        return true;
    }

    if (setBckAnm(bck_anm, 1.0f, attr, 0, -1, i_modify)) {
        mAnmFlags |= ANM_PLAY_BCK | ANM_PAUSE_BCK;
        mExpressionLoops = 0;
        return true;
    }

    return false;
}

bool daNpc_zrA_c::setExpressionBtp(int i_idx) {
    J3DAnmTexPattern* btp_anm = NULL;
    int attr = J3DFrameCtrl::EMode_NONE;
    mAnmFlags &= ~(ANM_PLAY_BTP | ANM_PAUSE_BTP | ANM_FLAG_800);

    if (l_btpGetParamList[i_idx].fileIdx >= 0) {
        btp_anm = getTexPtrnAnmP(l_resNames[l_btpGetParamList[i_idx].arcIdx],
                                 l_btpGetParamList[i_idx].fileIdx);
    }

    switch (i_idx) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case 6:
        break;
    default:
        btp_anm = NULL;
        break;
    }

    if (btp_anm == NULL) {
        return true;
    }

    if (setBtpAnm(btp_anm, mAnm_p->getModel()->getModelData(), 1.0f, attr)) {
        mAnmFlags |= ANM_PLAY_BTP | ANM_PAUSE_BTP;
        if (i_idx == 0) {
            mAnmFlags |= ANM_FLAG_800;
        }
        return true;
    }

    return false;
}

void daNpc_zrA_c::setExpression(int i_expression, f32 i_morf) {
    if (i_expression >= 0 && i_expression < 0x15) {
        mExpression = i_expression;
        mExpressionMorfOverride = i_morf;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

void daNpc_zrA_c::setMotionAnm(int i_idx, f32 i_morf) {
    J3DAnmTransformKey* bck_anm = NULL;
    J3DAnmTextureSRTKey* btk_anm = NULL;
    int btk_idx = 0;
    int attr = J3DFrameCtrl::EMode_LOOP;
    mBaseMotionAnm = i_idx;
    
    if (mAcch.ChkWaterIn()) {
        btk_idx = 3;
    }
    
    if (mSoldierType == SOLDIER_SPEAR && mBaseMotionAnm >= 0x29 && mBaseMotionAnm <= 0x33) {
        i_idx += 0xb;
    }
    
    mAnmFlags &= ~(ANM_PLAY_MORF | ANM_PLAY_BTK | ANM_PAUSE_MORF | ANM_PAUSE_BTK);

    if (l_bckGetParamList[i_idx].fileIdx >= 0) {
        bck_anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_idx].arcIdx],
                                    l_bckGetParamList[i_idx].fileIdx);
    }

    switch (i_idx) {
    case ANM_SWIM_TURN:
    case ANM_STILL:
    case ANM_SWIM_TALK:
    case ANM_TOBIKOMI_S:
    case ANM_TOBIKOMI_E:
    case ANM_SPA_TALK_B:
    case ANM_SPA_TALK_A:
    case ANM_TALK_A:
    case ANM_DIVE:
    case ANM_DIVE_B:
    case ANM_FLOAT:
    case ANM_FLOAT_B:
    case ANM_STEP:
    case ANM_TALK_A_SP:
    case ANM_DIVE_SP:
    case ANM_DIVE_B_SP:
    case ANM_FLOAT_SP:
    case ANM_FLOAT_B_SP:
    case ANM_STEP_SP:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    case ANM_FALLSWIM:
    case ANM_SADSIT_A:
    case ANM_SADSIT_B:
    case ANM_SADSIT_C:
    case ANM_SADSIT_D:
    case ANM_SADSIT_E:
    case ANM_TOBIKOMI_T:
    case ANM_LOOKING_SP:
    case ANM_TALK_SWIM_SP:
    case ANM_TALK_B_SP:
    case ANM_LOOKUP:
    case ANM_SPA_WAIT_B:
    case ANM_SPA_WAIT_A:
    case ANM_WAIT_A:
    case ANM_WALK_A:
    case ANM_SWIM_A:
    case ANM_SWIM_B:
    case ANM_WAIT_SWIM:
    case ANM_WAIT_SP:
    case ANM_WALK_A_SP:
    case ANM_SWIM_A_SP:
    case ANM_SWIM_B_SP:
    case ANM_WAIT_SWIM_SP:
        break;
    default:
        bck_anm = NULL;
        btk_anm = NULL;
        break;
    }

    if (l_btkGetParamList[btk_idx].fileIdx >= 0) {
        btk_anm = getTexSRTKeyAnmP(l_resNames[l_btkGetParamList[btk_idx].arcIdx],
                                   l_btkGetParamList[btk_idx].fileIdx);
    }

    if (btk_anm != NULL && setBtkAnm(btk_anm, mAnm_p->getModel()->getModelData(), 1.0f, 2)) {
        mAnmFlags |= ANM_PLAY_BTK | ANM_PAUSE_BTK;
    }
    
    mBtkID = btk_idx;

    if (bck_anm != NULL && setMcaMorfAnm(bck_anm, 1.0f, i_morf, attr, 0, -1)) {
        mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
        mMotionLoops = 0;
    }
}

void daNpc_zrA_c::setMotionWaterAnm(int i_idx) {
    J3DAnmTevRegKey* brk_anm = NULL;
    J3DAnmTextureSRTKey* btk_anm = NULL;
    J3DAnmColor* bpk_anm = NULL;
    int btk_idx = 0;
    int bpk_idx = 0;
    mAnmFlags &= ~(ANM_PLAY_BRK | ANM_PAUSE_BRK);
    mWaterAnmFlags &= ~(ANM_PLAY_BTK | ANM_PAUSE_BTK | ANM_PLAY_BPK | ANM_PAUSE_BPK);

    if (!mTwilight) {
        if (l_brkGetParamList[i_idx].fileIdx >= 0) {
            brk_anm = getTevRegKeyAnmP(l_resNames[l_brkGetParamList[i_idx].arcIdx],
                                       l_brkGetParamList[i_idx].fileIdx);
        }
    }

    switch (i_idx) {
    case 0:
        btk_idx = 2;
        bpk_idx = i_idx;
        break;
    case 1:
        btk_idx = i_idx;
        bpk_idx = i_idx;
        break;
    default:
        brk_anm = NULL;
    }

    mBpkID = bpk_idx;

    if (brk_anm != NULL) {
        if (mBrkAnm.getBrkAnm() == brk_anm) {
            mAnmFlags |= ANM_PLAY_BRK;
        } else if (setBrkAnm(brk_anm, mAnm_p->getModel()->getModelData(), 1.0f, 2)) {
            mAnmFlags |= ANM_PLAY_BRK | ANM_PAUSE_BRK;
        }
    }

    if (!mTwilight) {
        if (l_btkGetParamList[btk_idx].fileIdx >= 0) {
            btk_anm = getTexSRTKeyAnmP(l_resNames[l_btkGetParamList[btk_idx].arcIdx],
                                       l_btkGetParamList[btk_idx].fileIdx);
        }
    }

    if (btk_anm != NULL) {
        if (mWaterBtkAnm.getBtkAnm() == btk_anm) {
            mWaterAnmFlags |= ANM_PLAY_BTK;
        } else if (mWaterBtkAnm.init(mAnm_p->getModel()->getModelData(),
                                     btk_anm, 1, 2, 1.0f, 0, -1)) {
            mWaterAnmFlags |= ANM_PLAY_BTK | ANM_PAUSE_BTK;
        }
    }

    if (l_bpkGetParamList[bpk_idx].fileIdx >= 0) {
        bpk_anm = static_cast<J3DAnmColor*>(
            dComIfG_getObjectRes(l_resNames[l_bpkGetParamList[bpk_idx].arcIdx],
                                 l_bpkGetParamList[bpk_idx].fileIdx));
    }

    if (bpk_anm != NULL) {
        if (mWaterBpkAnm.getBpkAnm() == bpk_anm) {
            mWaterAnmFlags |= ANM_PLAY_BPK;
        } else if (mWaterBpkAnm.init(mAnm_p->getModel()->getModelData(),
                                     bpk_anm, 1, 2, 1.0f, 0, -1)) {
            mWaterAnmFlags |= ANM_PLAY_BPK | ANM_PAUSE_BPK;
        }
    }
}

void daNpc_zrA_c::setMotion(int i_motion, f32 i_morf, BOOL i_restart) {
    s16 motion = i_motion;
    if (i_restart || mMotion != motion) {
        if (i_motion >= 0 && i_motion < 0x24) {
            mMotion = motion;
            mMotionMorfOverride = i_morf;
            mMotionPrevPhase = -1;
            mMotionPhase = 0;
        }
    }
}

BOOL daNpc_zrA_c::drawDbgInfo() {
    return false;
}

void daNpc_zrA_c::drawOtherMdls() {
    static int const l_jntNumTbl[3][3] = {
        {4, 25, -1},
        {4, -1, -1},
        {-1, -1, -1},
    };

    if (mSoldierType != SOLDIER_NONE) {
        for (int i = 0; i < 3; i++) {
            int jnt_no = l_jntNumTbl[mSoldierType][i];
            J3DModel* model = mpObjectModel[i];
            if (model != NULL && jnt_no >= 0) {
                g_env_light.setLightTevColorType_MAJI(model, &tevStr);
                mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(jnt_no));
                mDoMtx_stack_c::scaleM(scale);
                model->setBaseTRMtx(mDoMtx_stack_c::get());
                mDoExt_modelUpdateDL(model);
                dComIfGd_addRealShadow(mShadowKey, model);
            }
        }
    }
}

daNpc_zrA_c::Type daNpc_zrA_c::getTypeFromArgument() {
    switch (argument) {
    case 0:
        return TYPE_WAIT;
    case 1:
        return TYPE_SWIM;
    case 2:
        return TYPE_WATERFALL;
    case 3:
        return TYPE_RIVER;
    case 4:
        return TYPE_TOBIKOMI;
    case 5:
        return TYPE_SEARCH;
    case 6:
        return TYPE_SPA;
    default:
        return TYPE_WAIT;
    }
}

daNpc_zrA_c::SoldierType daNpc_zrA_c::getSoldierTypeFromParam() {
    if (mType == TYPE_WATERFALL || mType == TYPE_SEARCH) {
        return SOLDIER_NOSPEAR;
    }

    if (mType == TYPE_TOBIKOMI) {
        return SOLDIER_NONE;
    }

    switch (fopAcM_GetParam(this) & 0xff) {
    case 0:
        return SOLDIER_SPEAR;
    default:
        return SOLDIER_NONE;
    }
}

daNpc_zrA_c::GameMode daNpc_zrA_c::getGameModeFromParam() {
    switch ((fopAcM_GetParam(this) >> 0x14) & 0xf) {
    case 1:
        return GAME_MODE_3;
    default:
        return GAME_MODE_0;
    }
}

daNpc_zrA_c::ActionType daNpc_zrA_c::getActionTypeFromParam() {
    u8 param = (fopAcM_GetParam(this) >> 0x10) & 0xf;
    if (mType == TYPE_WAIT) {
        switch (param) {
        case 1:
            return ACT_TYPE_1;
        case 2:
            return ACT_TYPE_2;
        case 3:
            return ACT_TYPE_3;
        case 4:
            return ACT_TYPE_4;
        case 5:
            return ACT_TYPE_5;
        case 6:
            return ACT_TYPE_6;
        default:
            return ACT_TYPE_0;
        }
    } else if (mType == TYPE_SPA) {
        switch (param) {
        case 1:
            return ACT_TYPE_2;
        case 2:
            return ACT_TYPE_3;
        default:
            return ACT_TYPE_0;
        }
    } else {
        return (ActionType)param;
    }
}

u8 daNpc_zrA_c::getMultiModeFromParam() {
    u8 param = (fopAcM_GetParam(this) >> 0x14) & 0xf;
    if (mType != 0) {
        return 0;
    }
    if (param == 0xf) {
        param = 0;
    }
    return param;
}

u8 daNpc_zrA_c::getMultiNoFromParam() {
    u8 param = (fopAcM_GetParam(this) >> 0x18) & 0xf;
    if (mType != 0) {
        return 0;
    }
    return param == 0xf ? (u8)0 : param;
}

BOOL daNpc_zrA_c::isDelete() {
    if ((mType == TYPE_SEARCH && dComIfGs_isSwitch(mSwitch1, fopAcM_GetRoomNo(this)))
        || (mType == TYPE_SPA && !daNpcF_chkEvtBit(0x10a))
        || (mType == TYPE_RIVER && mGameMode == GAME_MODE_3
            && (strcmp(dComIfGp_getStartStageName(), "F_SP115") || fopAcM_GetRoomNo(this) != 0
                || dComIfGs_getStartPoint() != 2)))
    {
        return true;
    }
    return false;
}

void daNpc_zrA_c::reset() {
    initialize();
    mpMatAnm->initialize();
    mPath.initialize();
    if (mType != TYPE_TOBIKOMI
        && mPath.setPathInfo(getPathNoFromParam(), fopAcM_GetRoomNo(this), 0))
    {
        mPath.setRange(100.0f);
    }
    mLookat.initialize();
    for (int i = 0; i < 3; i++) {
        mActorMngr[i].initialize();
    }

    mpNextActionFn = NULL;
    mpActionFn = NULL;
    field_0x14d0 = 0;
    field_0x14d4 = 0;
    mSwimTurnTimer = 0;
    field_0x14dc = 0;
    mLookMode = -1;
    mMode = 0;
    mHide = false;
    mItemID = -1;

    current.pos = home.pos;
    old.pos = current.pos;
    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;
    speedF = 0.0f;
    speed.setall(0.0f);

    if ((mType == TYPE_SWIM && mPath.getPathInfo() != NULL) || mType == TYPE_WATERFALL) {
        fopAcM_OnStatus(this, 0x8000000);
    }

    mBtkID = 0;
    field_0x1510 = 0;

    if (mType == TYPE_RIVER) {
        mStaffName = l_myName[2];
    } else if (mType == TYPE_WATERFALL) {
        mStaffName = l_myName[3];
    } else {
        mStaffName = l_myName[0];
    }

    mSpinAngle = 0;
    mSpinTimer = 0;
    mSwimSpeedScale = 1.0f;
    mSwimSpeedF = mSwimSpeedScale * daNpc_zrA_Param_c::m.mSwimSpeed;
    mSwimMode = SWIM_WAIT;
    field_0x1521 = false;
    mWaistAngle.set(0, 0, 0);
    mSwimAngleCalc = current.angle;
    field_0x153c = false;
    field_0x153d = false;
    field_0x153e = false;
    mSwimFastTurnTimer = 0;
    mSwimSpeed.set(0.0f, 0.0f, 0.0f);
    mEvasionTimer = 0;
    mCheckSwimTurnTimer = 0;
    mCheckSwimTurn = false;
    mResetWaistAngle = false;
    field_0x1550 = false;
    field_0x1554 = 0;
    mModulationOffset.set(0.0f, 0.0f, 0.0f);
    mModulationParam = 210;
    mActionType = getActionTypeFromParam();
    mIsAboveWater = false;
    mIsTurning = false;
    mActionSelect = 0;
    mMeterCount = 0;
    mRiverPathIdx = 0;
    field_0x15c0 = 0;
    mBlastFlag = false;
    mWaterAnmFlags = 0;

    if (mType == TYPE_SEARCH) {
        if (getNoFromParam() == 0) {
            eventInfo.setIdx(1);
        } else {
            eventInfo.setIdx(2);
        }
    } else if (mType == TYPE_WAIT && getMultiModeFromParam() != 0) {
        eventInfo.setIdx(getMultiNoFromParam());
    }

    field_0x9ee = 1;
}

void daNpc_zrA_c::playExpression() {
    daNpcF_anmPlayData dat0a = {ANM_F_TALK_NOMAL, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat0b = {ANM_FH_TALK_NOMAL, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat0[2] = {&dat0a, &dat0b};
    daNpcF_anmPlayData dat1 = {ANM_FH_SADSIT_A, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat1[1] = {&dat1};
    daNpcF_anmPlayData dat2 = {ANM_FH_SADSIT_B, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};
    daNpcF_anmPlayData dat3 = {ANM_FH_SADSIT_C, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat3[1] = {&dat3};
    daNpcF_anmPlayData dat4 = {ANM_FH_SADSIT_D, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat4[1] = {&dat4};
    daNpcF_anmPlayData dat5 = {ANM_FH_SADSIT_E, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat5[1] = {&dat5};
    daNpcF_anmPlayData dat6a = {ANM_F_LOOKING_SP, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat6b = {ANM_FH_LOOKING_SP, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat6[2] = {&dat6a, &dat6b};
    daNpcF_anmPlayData dat7a = {ANM_F_LOOKUP, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat7b = {ANM_FH_LOOKUP, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat7[2] = {&dat7a, &dat7b};
    daNpcF_anmPlayData dat8a = {ANM_F_SPA_TALK_A, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat8b = {ANM_FH_SPA_WAIT_A, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat8[2] = {&dat8a, &dat8b};
    daNpcF_anmPlayData dat9a = {ANM_F_SPA_TALK_B, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat9b = {ANM_FH_SPA_WAIT_B, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat9[2] = {&dat9a, &dat9b};
    daNpcF_anmPlayData dat10a = {ANM_F_TALK_SWIM_SP, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat10b = {ANM_NONE, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat10[2] = {&dat10a, &dat10b};
    daNpcF_anmPlayData dat11a = {ANM_F_TALK_B_SP, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat11b = {ANM_NONE, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat11[2] = {&dat11a, &dat11b};
    daNpcF_anmPlayData dat12a = {ANM_F_TALK_A, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat12b = {ANM_NONE, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat12[2] = {&dat12a, &dat12b};
    daNpcF_anmPlayData dat13a = {ANM_F_WAIT_SWIM, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat13b = {ANM_NONE, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat13[2] = {&dat13a, &dat13b};
    daNpcF_anmPlayData dat14a = {ANM_F_TALK_A_SP, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat14b = {ANM_NONE, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat14[2] = {&dat14a, &dat14b};
    daNpcF_anmPlayData dat15 = {ANM_FH_TALK_NOMAL, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat15[1] = {&dat15};
    daNpcF_anmPlayData dat16 = {ANM_FH_LOOKING_SP, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat16[1] = {&dat16};
    daNpcF_anmPlayData dat17 = {ANM_FH_LOOKUP, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat17[1] = {&dat17};
    daNpcF_anmPlayData dat18 = {ANM_FH_SPA_WAIT_A, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat18[1] = {&dat18};
    daNpcF_anmPlayData dat19 = {ANM_FH_SPA_WAIT_B, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat19[1] = {&dat19};
    daNpcF_anmPlayData dat20 = {ANM_NONE, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat20[1] = {&dat20};
    daNpcF_anmPlayData** ppDat[21] = {
        pDat0, pDat1, pDat2, pDat3, pDat4, pDat5, pDat6, pDat7, pDat8, pDat9, pDat10, pDat11,
        pDat12, pDat13, pDat14, pDat15, pDat16, pDat17, pDat18, pDat19, pDat20,
    };
    if (mExpression >= 0 && mExpression < 0x15) {
        playExpressionAnm(ppDat);
    }
}

void daNpc_zrA_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_WAIT_A, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1 = {ANM_WAIT_SWIM, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat1[1] = {&dat1};
    daNpcF_anmPlayData dat2a = {ANM_SWIM_TALK, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat2b = {ANM_WAIT_SWIM, 0.0f, 0};
    daNpcF_anmPlayData* pDat2[2] = {&dat2a, &dat2b};
    daNpcF_anmPlayData dat3 = {ANM_WAIT_A, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat3[1] = {&dat3};
    daNpcF_anmPlayData dat4 = {ANM_WAIT_SWIM, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat4[1] = {&dat4};
    daNpcF_anmPlayData dat5a = {ANM_TALK_A, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat5b = {ANM_WAIT_A, 0.0f, 0};
    daNpcF_anmPlayData* pDat5[2] = {&dat5a, &dat5b};
    daNpcF_anmPlayData dat6 = {ANM_SADSIT_A, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat6[1] = {&dat6};
    daNpcF_anmPlayData dat7 = {ANM_SADSIT_B, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat7[1] = {&dat7};
    daNpcF_anmPlayData dat8 = {ANM_SADSIT_C, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat8[1] = {&dat8};
    daNpcF_anmPlayData dat9 = {ANM_SADSIT_D, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat9[1] = {&dat9};
    daNpcF_anmPlayData dat10 = {ANM_SADSIT_E, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat10[1] = {&dat10};
    daNpcF_anmPlayData dat11 = {ANM_LOOKING_SP, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat11[1] = {&dat11};
    daNpcF_anmPlayData dat12 = {ANM_LOOKUP, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat12[1] = {&dat12};
    daNpcF_anmPlayData dat13a = {ANM_SPA_TALK_A, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat13b = {ANM_SPA_WAIT_A, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat13[2] = {&dat13a, &dat13b};
    daNpcF_anmPlayData dat14a = {ANM_SPA_TALK_B, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat14b = {ANM_SPA_WAIT_B, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat14[2] = {&dat14a, &dat14b};
    daNpcF_anmPlayData dat15a = {ANM_TALK_SWIM_SP, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat15b = {ANM_WAIT_SWIM_SP, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat15[2] = {&dat15a, &dat15b};
    daNpcF_anmPlayData dat16a = {ANM_TALK_B_SP, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat16b = {ANM_WAIT_SP, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat16[2] = {&dat16a, &dat16b};
    daNpcF_anmPlayData dat17a = {ANM_TALK_A, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat17b = {ANM_WAIT_A, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat17[2] = {&dat17a, &dat17b};
    daNpcF_anmPlayData dat18 = {ANM_SPA_WAIT_A, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat18[1] = {&dat18};
    daNpcF_anmPlayData dat19 = {ANM_SPA_WAIT_B, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat19[1] = {&dat19};
    daNpcF_anmPlayData dat20 = {ANM_WALK_A, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat20[1] = {&dat20};
    daNpcF_anmPlayData dat22 = {ANM_SWIM_A, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat22[1] = {&dat22};
    daNpcF_anmPlayData dat23 = {ANM_SWIM_B, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat23[1] = {&dat23};
    daNpcF_anmPlayData dat24a = {ANM_DIVE, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat24b = {ANM_SWIM_A, 0.0f, 0};
    daNpcF_anmPlayData* pDat24[2] = {&dat24a, &dat24b};
    daNpcF_anmPlayData dat25a = {ANM_DIVE_B, 3.0f, 1};
    daNpcF_anmPlayData dat25b = {ANM_SWIM_A, 0.0f, 0};
    daNpcF_anmPlayData* pDat25[2] = {&dat25a, &dat25b};
    daNpcF_anmPlayData dat26a = {ANM_DIVE, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat26b = {ANM_SWIM_B, daNpc_zrA_Param_c::m.mMorfFrames / 2.0f, 0};
    daNpcF_anmPlayData* pDat26[2] = {&dat26a, &dat26b};
    daNpcF_anmPlayData dat27a = {ANM_DIVE_B, 3.0f, 1};
    daNpcF_anmPlayData dat27b = {ANM_SWIM_B, daNpc_zrA_Param_c::m.mMorfFrames / 2.0f, 0};
    daNpcF_anmPlayData* pDat27[2] = {&dat27a, &dat27b};
    daNpcF_anmPlayData dat28 = {ANM_STEP, 2.0f, 0};
    daNpcF_anmPlayData* pDat28[1] = {&dat28};
    daNpcF_anmPlayData dat29a = {ANM_FLOAT, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat29b = {ANM_WAIT_SWIM, 2.0f, 0};
    daNpcF_anmPlayData* pDat29[2] = {&dat29a, &dat29b};
    daNpcF_anmPlayData dat30a = {ANM_FLOAT_B, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat30b = {ANM_WAIT_SWIM, 2.0f, 0};
    daNpcF_anmPlayData* pDat30[2] = {&dat30a, &dat30b};
    daNpcF_anmPlayData dat31a = {ANM_SWIM_TURN, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat31b = {ANM_STILL, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat31[2] = {&dat31a, &dat31b};
    daNpcF_anmPlayData dat32 = {ANM_TOBIKOMI_S, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat32[1] = {&dat32};
    daNpcF_anmPlayData dat33 = {ANM_TOBIKOMI_T, 0.0f, 0};
    daNpcF_anmPlayData* pDat33[1] = {&dat33};
    daNpcF_anmPlayData dat34a = {ANM_TOBIKOMI_E, daNpc_zrA_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat34b = {ANM_SWIM_A, 0.0f, 0};
    daNpcF_anmPlayData* pDat34[2] = {&dat34a, &dat34b};
    daNpcF_anmPlayData dat35 = {ANM_FALLSWIM, daNpc_zrA_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat35[1] = {&dat35};
    daNpcF_anmPlayData** ppDat[36] = {
        pDat0, pDat1, pDat2, pDat3, pDat4, pDat5, pDat6, pDat7, pDat8, pDat9, pDat10, pDat11,
        pDat12, pDat13, pDat14, pDat15, pDat16, pDat17, pDat18, pDat19, pDat20, NULL, pDat22,
        pDat23, pDat24, pDat25, pDat26, pDat27, pDat28, pDat29, pDat30, pDat31, pDat32,
        pDat33, pDat34, pDat35,
    };
    if (mMotion >= 0 && mMotion < 0x24) {
        playMotionAnm(ppDat);
    }
}

BOOL daNpc_zrA_c::chkAction(ActionFn i_action) {
    return mpActionFn == i_action;
}

bool daNpc_zrA_c::setAction(ActionFn i_action) {
    mMode = 3;
    if (mpActionFn != NULL) {
        (this->*mpActionFn)(NULL);
    }
    mMode = 0;
    mpActionFn = i_action;
    if (mpActionFn != NULL) {
        (this->*mpActionFn)(NULL);
    }
    return true;
}

bool daNpc_zrA_c::selectAction() {
    mpNextActionFn = NULL;
    if (daNpc_zrA_Param_c::m.mTest) {
        mpNextActionFn = &daNpc_zrA_c::test;
    } else {
        switch (mType) {
        case TYPE_SWIM:
            mpNextActionFn = &daNpc_zrA_c::swim;
            break;
        case TYPE_WATERFALL:
            switch (mActionSelect) {
            case 0:
                mpNextActionFn = &daNpc_zrA_c::waitWaterfall;
                break;
            case 1:
                mpNextActionFn = &daNpc_zrA_c::swimWaterfall;
                break;
            case 2:
                break;
            case 3:
                mpNextActionFn = &daNpc_zrA_c::diveWaterfall;
                break;
            }
            break;
        case TYPE_RIVER:
            switch (mActionSelect) {
            case 0:
                mpNextActionFn = &daNpc_zrA_c::waitRiverDescend;
                break;
            case 1:
                mpNextActionFn = &daNpc_zrA_c::swimRiverDescend;
                break;
            case 6:
                mpNextActionFn = &daNpc_zrA_c::swimRiverDescend2;
                break;
            case 3:
                mpNextActionFn = &daNpc_zrA_c::diveRiverDescend;
                break;
            case 4:
                mpNextActionFn = &daNpc_zrA_c::swimGoalRiverDescend;
                break;
            default:
                mpNextActionFn = &daNpc_zrA_c::returnRiverDescend;
                break;
            }
            break;
        case TYPE_TOBIKOMI:
            switch (mActionSelect) {
            case 0:
                mpNextActionFn = &daNpc_zrA_c::tobiSearch;
                break;
            case 1:
                mpNextActionFn = &daNpc_zrA_c::tobiWait;
                break;
            case 2:
                mpNextActionFn = &daNpc_zrA_c::tobikomi1;
                break;
            case 3:
                mpNextActionFn = &daNpc_zrA_c::tobikomi2;
                break;
            case 4:
                mpNextActionFn = &daNpc_zrA_c::tobikomi3;
                break;
            case 5:
                mpNextActionFn = &daNpc_zrA_c::tobiJump;
                break;
            case 6:
                mpNextActionFn = &daNpc_zrA_c::tobiEnd;
                break;
            }
            break;
        case TYPE_SEARCH:
            switch (mActionSelect) {
            case 0:
                mpNextActionFn = &daNpc_zrA_c::waitSearch;
                break;
            }
            break;
        case TYPE_SPA:
            mpNextActionFn = &daNpc_zrA_c::waitSpa;
            break;
        case TYPE_WAIT:
            if (mActionType == ACT_TYPE_6) {
                mpNextActionFn = &daNpc_zrA_c::waitLake;
            } else {
                mpNextActionFn = &daNpc_zrA_c::wait;
            }
            break;
        default:
            mpNextActionFn = &daNpc_zrA_c::wait;
            break;
        }
    }
    return true;
}

BOOL daNpc_zrA_c::doEvent() {
    dEvent_manager_c* event_manager = NULL;
    BOOL ret = false;
    int staff_id;

    if (dComIfGp_event_runCheck()) {
        event_manager = &dComIfGp_getEventManager();
        if (eventInfo.checkCommandTalk() || eventInfo.checkCommandDemoAccrpt()) {
            mOrderNewEvt = false;
        }

        if (eventInfo.checkCommandTalk()) {
            if (field_0x1521 == true && mType == TYPE_SWIM) {
                mOrderEvtNo = EVT_TALK_SWIM;
                changeEvent(l_resNames[l_evtGetParamList[mOrderEvtNo].arcIdx],
                            l_evtNames[l_evtGetParamList[mOrderEvtNo].fileIdx], 1, 0xffff);
                mStaffName = l_myName[1];
            } else if (mType == TYPE_WATERFALL) {
                if (chkAction(&daNpc_zrA_c::talkSwim)) {
                    (this->*mpActionFn)(NULL);
                } else if (dComIfGp_event_chkTalkXY() == false || dComIfGp_evmng_ChkPresentEnd()) {
                    setAction(&daNpc_zrA_c::talkSwim);
                }
            } else if (mType == TYPE_SEARCH) {
                mOrderEvtNo = EVT_SEARCH_PRINCE;
                changeEvent(l_resNames[l_evtGetParamList[mOrderEvtNo].arcIdx],
                            l_evtNames[l_evtGetParamList[mOrderEvtNo].fileIdx], 1, 0xffff);
            } else if (mType == TYPE_WAIT && getMultiModeFromParam() != 0) {
                if (getMultiModeFromParam() == 1) {
                    mOrderEvtNo = EVT_TALK_MULTI;
                } else {
                    mOrderEvtNo = EVT_TALK_MULTI2;
                }
                changeEvent(l_resNames[l_evtGetParamList[mOrderEvtNo].arcIdx],
                            l_evtNames[l_evtGetParamList[mOrderEvtNo].fileIdx], 1, 0xffff);
            } else if (chkAction(&daNpc_zrA_c::talk)) {
                (this->*mpActionFn)(NULL);
            } else if (dComIfGp_event_chkTalkXY()) {
                if (dComIfGp_evmng_ChkPresentEnd()) {
                    if (dComIfGp_event_getPreItemNo() == 0x91) {
                        if (mSoldierType != SOLDIER_NONE) {
                            if (home.angle.x == 0x6d) {
                                mFlowID = 4;
                            } else {
                                mFlowID = 5;
                            }
                        } else {
                            mFlowID = 6;
                        }
                        setAction(&daNpc_zrA_c::talk);
                    } else {
                        s16 event_idx =
                            dComIfGp_getEventManager().getEventIdx(this, "NO_RESPONSE", 0xff);
                        dComIfGp_getEvent()->reset(this);
                        fopAcM_orderChangeEventId(this, event_idx, 1, 0xffff);
                        field_0x9ec = true;
                    }
                }
            } else {
                setAction(&daNpc_zrA_c::talk);
            }
            ret = true;
        } else {
            if (mItemID != -1) {
                dComIfGp_event_setItemPartnerId(mItemID);
                mItemID = -1;
            }

            staff_id = event_manager->getMyStaffId(mStaffName, this, 0);
            if (staff_id != -1) {
                mStaffID = staff_id;
                int act_idx = event_manager->getMyActIdx(staff_id, mEvtCutNameList,
                                                        ARRAY_SIZEU(mEvtCutNameList), 0, 0);
                JUT_ASSERT(0xf0e, act_idx >= 0 && act_idx < 11);
                JUT_ASSERT(0xf0f, mEvtCutList[act_idx] != NULL);
                if ((this->*mEvtCutList[act_idx])(staff_id)) {
                    event_manager->cutEnd(staff_id);
                }
                ret = true;
            }

            if (eventInfo.checkCommandDemoAccrpt()) {
                if (mEventIdx != -1 && event_manager->endCheck(mEventIdx)) {
                    switch (mOrderEvtNo) {
                    case EVT_TALK_SWIM:
                        dComIfGp_event_reset();
                        setAction(&daNpc_zrA_c::swim);
                        mOrderEvtNo = 0;
                        mEventIdx = -1;
                        mStaffName = l_myName[0];
                        break;
                    case EVT_THANKS_BLAST:
                    case EVT_RESULT_ANNOUNCE:
                        dComIfGp_event_reset();
                        mActionSelect = 5;
                        setAction(&daNpc_zrA_c::returnRiverDescend);
                        mOrderEvtNo = 0;
                        mEventIdx = -1;
                        dComIfGs_offSwitch(mSwitch1, fopAcM_GetRoomNo(this));
                        dComIfGs_offSwitch(mSwitch2, fopAcM_GetRoomNo(this));
                        break;
                    case EVT_CARRY_WATERFALL:
                    case EVT_CARRY_WATERFALL_NIGHT:
                        dComIfGp_event_reset();
                        mActionSelect = 3;
                        setAction(&daNpc_zrA_c::diveWaterfall);
                        mOrderEvtNo = 0;
                        mEventIdx = -1;
                        break;
                    default:
                        dComIfGp_event_reset();
                        mOrderEvtNo = 0;
                        mEventIdx = -1;
                        break;
                    }
                } else {
                    switch (mOrderEvtNo) {
                    case EVT_CARRY_WATERFALL:
                        setSkipZev(EVT_CARRY_WATERFALL, EVT_CARRY_WATERFALL_SKIP);
                        break;
                    case EVT_CARRY_WATERFALL_NIGHT:
                        setSkipZev(EVT_CARRY_WATERFALL_NIGHT, EVT_CARRY_WATERFALL_NIGHT_SKIP);
                        break;
                    }
                }
            }
        }

        int expression, motion;
        int prev_msg_timer = mMsgTimer;
        if (ctrlMsgAnm(expression, motion, this, false)) {
            if (!field_0x9eb) {
                setExpression(expression, -1.0f);
                setMotion(motion, -1.0f, false);
            }
        } else if (!field_0x9eb && prev_msg_timer != 0 && mMsgTimer == 0) {
            setExpressionTalkAfter();
        }
    }

    if (!ret) {
        if (mStaffID != -1) {
            mpActionFn = NULL;
            mStaffID = -1;
        }
        mMsgTimer = 0;
    }

    return ret;
}

BOOL daNpc_zrA_c::setSkipZev(int i_idx1, int i_idx2) {
    if (!strcmp(dComIfGp_getEventManager().getRunEventName(), l_evtNames[i_idx1])) {
        dComIfGp_getEvent()->setSkipZev(this, l_evtNames[i_idx2]);
        return true;
    }
    return false;
}

void daNpc_zrA_c::setLookMode(int i_lookMode) {
    if (i_lookMode >= 0 && i_lookMode < 6 && i_lookMode != mLookMode) {
        mLookMode = i_lookMode;
    }
}

void daNpc_zrA_c::lookat() {
    fopAc_ac_c* attn_actor = NULL;
    J3DModel* model = mAnm_p->getModel();
    BOOL snap = false;
    f32 body_down_angle = daNpc_zrA_Param_c::m.mBodyDownAngle;
    f32 body_up_angle = daNpc_zrA_Param_c::m.mBodyUpAngle;
    f32 body_right_angle = daNpc_zrA_Param_c::m.mBodyRightAngle;
    f32 body_left_angle = daNpc_zrA_Param_c::m.mBodyLeftAngle;
    f32 head_down_angle = daNpc_zrA_Param_c::m.mHeadDownAngle;
    f32 head_up_angle = daNpc_zrA_Param_c::m.mHeadUpAngle;
    f32 head_right_angle = daNpc_zrA_Param_c::m.mHeadRightAngle;
    f32 head_left_angle = daNpc_zrA_Param_c::m.mHeadLeftAngle;
    s16 angle_delta = mCurAngle.y - mOldAngle.y;
    cXyz lookat_pos[3] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* lookat_angle[3] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};

    switch (mLookMode) {
    case LOOK_RESET:
        snap = true;
        break;
    case LOOK_PLAYER:
    case LOOK_PLAYER_TALK:
        attn_actor = daPy_getPlayerActorClass();
        if (mLookMode == LOOK_PLAYER_TALK) {
            head_right_angle = -80.0f;
            head_left_angle = 80.0f;
        }
        break;
    case LOOK_ACTOR:
        attn_actor = mActorMngr[2].getActorP();
        break;
    case LOOK_ATTN:
        attn_actor = mActorMngr[1].getActorP();
        break;
    }

    if (attn_actor != NULL) {
        mLookPos = attn_actor->attention_info.position;
        if (mLookMode != LOOK_PLAYER && mLookMode != LOOK_PLAYER_TALK && mLookMode != LOOK_ATTN) {
            mLookPos.y -= 40.0f;
        }
        mLookat.setAttnPos(&mLookPos);
        if (mBaseMotionAnm != ANM_WAIT_SWIM && mBaseMotionAnm != ANM_SWIM_TALK
            && mBaseMotionAnm != ANM_TALK_SWIM_SP && mBaseMotionAnm != ANM_WAIT_SWIM_SP)
        {
            body_up_angle = body_down_angle = 0.0f;
        }
    } else {
        mLookat.setAttnPos(NULL);
    }

    mLookat.setParam(body_down_angle, body_up_angle, body_right_angle, body_left_angle,
                     0.0f, 0.0f, 0.0f, 0.0f,
                     head_down_angle, head_up_angle, head_right_angle, head_left_angle,
                     mCurAngle.y, lookat_pos);
    mLookat.calc(this, model->getBaseTRMtx(), lookat_angle, snap, angle_delta, false);
}

BOOL daNpc_zrA_c::chkFindPlayer() {
    BOOL check;
    if (mActorMngr[0].getActorP() == NULL) {
        check = chkPlayerInSpeakArea(this);
    } else {
        check = chkPlayerInTalkArea(this);
    }

    if (check) {
        mActorMngr[0].entry(daPy_getPlayerActorClass());
    } else {
        mActorMngr[0].remove();
    }

    return check;
}

void daNpc_zrA_c::setExpressionTalkAfter() {
    switch (mExpression) {
    case EXPR_TALK_NOMAL:
        setExpression(EXPR_TALK_NOMAL_2, -1.0f);
        break;
    case EXPR_LOOKING_SP:
        setExpression(EXPR_LOOKING_SP_2, -1.0f);
        break;
    case EXPR_LOOKUP:
        setExpression(EXPR_LOOKUP_2, -1.0f);
        break;
    case EXPR_SPA_TALK_A:
        setExpression(EXPR_SPA_WAIT_A, -1.0f);
        break;
    case EXPR_SPA_TALK_B:
        setExpression(EXPR_SPA_WAIT_B, -1.0f);
        break;
    default:
        setExpression(EXPR_NONE, -1.0f);
        break;
    }
}

// fakematch to get the clib_calctimer functions to appear in the correct order.
//  Perhaps clever inline keyword usage can fix the problem, but this is easier.
int dummy_clib_u8(u8* arg) {
    return cLib_calcTimer<u8>(arg);
}

void daNpc_zrA_c::setPrtcl() {
    u32 flags = 0x40002;
    cXyz ptcl_scale(1.5f, 1.5f, 1.5f);
    if (field_0x1550) {
        if (cLib_calcTimer(&field_0x1554) != 0) {
            flags |= 0x20000;
        } else {
            field_0x1554 = 10;
        }
    }
    mPaPo.setEffectCenter(&tevStr, &current.pos, 1, flags, NULL, NULL, &ptcl_scale,
                          fopAcM_GetRoomNo(this), 1.0f, speedF);
}


BOOL daNpc_zrA_c::test(void* param_0) {
    switch (mMode) {
    case 0:
        speedF = 0.0f;
        speed.setall(0.0f);
        mMode = 2;
        // fallthrough

    case 2:
        if (daNpc_zrA_Param_c::m.mTestExpression != mExpression) {
            setExpression(daNpc_zrA_Param_c::m.mTestExpression, daNpc_zrA_Param_c::m.mMorfFrames);
        }
        setMotion(daNpc_zrA_Param_c::m.mTestMotion, daNpc_zrA_Param_c::m.mMorfFrames, false);
        setLookMode(daNpc_zrA_Param_c::m.mTestLookMode);
        mOrderEvtNo = EVT_NONE;
        attention_info.flags = 0;
        break;

    case 3:
        break;
    }

    return true;
}

static cPhs__Step daNpc_zrA_Create(void* i_this) {
    return static_cast<daNpc_zrA_c*>(i_this)->create();
}

static int daNpc_zrA_Delete(void* i_this) {
    return static_cast<daNpc_zrA_c*>(i_this)->Delete();
}

static int daNpc_zrA_Execute(void* i_this) {
    return static_cast<daNpc_zrA_c*>(i_this)->Execute();
}

static int daNpc_zrA_Draw(void* i_this) {
    return static_cast<daNpc_zrA_c*>(i_this)->Draw();
}

static int daNpc_zrA_IsDelete(void* i_this) {
    return 1;
}

#include "d/actor/d_a_npc_zra.inc"

AUDIO_INSTANCES

static actor_method_class daNpc_zrA_MethodTable = {
    (process_method_func)daNpc_zrA_Create,
    (process_method_func)daNpc_zrA_Delete,
    (process_method_func)daNpc_zrA_Execute,
    (process_method_func)daNpc_zrA_IsDelete,
    (process_method_func)daNpc_zrA_Draw,
};

actor_process_profile_definition g_profile_NPC_ZRA = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_NPC_ZRA,
    &g_fpcLf_Method.base,
    sizeof(daNpc_zrA_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x183,
    &daNpc_zrA_MethodTable,
    0x44100,
    fopAc_NPC_e,
    fopAc_CULLBOX_CUSTOM_e,
};
