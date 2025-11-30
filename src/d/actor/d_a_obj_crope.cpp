/**
 * @file d_a_obj_crope.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_crope.h"
#include "d/actor/d_a_player.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_path.h"
#include "SSystem/SComponent/c_math.h"

static char const l_arcName[] = "Crope";

int daObjCrope_c::createHeap() {
    ResTIMG* texImg = (ResTIMG*) dComIfG_getObjectRes(l_arcName, 3);
    JUT_ASSERT(80, texImg != NULL);
    return mLineMat.init(1, 100, texImg, 0) != 0 ? 1 : 0;
}

static int daObjCrope_createHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjCrope_c*>(i_this)->createHeap();
}

static dCcD_SrcSph l_sphSrc = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x0}, 0x79}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 30.0f} // mSph
    } // mSphAttr
};

static GXColor color = {
    0x00,
    0x00,
    0x00,
    0xFF,
};

int daObjCrope_c::create() {
    fopAcM_ct(this, daObjCrope_c);
    int rv = dComIfG_resLoad(&mPhase, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, daObjCrope_createHeap, 0x2900) == 0) {
            return cPhs_ERROR_e;
        }
        attention_info.position = home.pos;
        eyePos = home.pos;
        mCcStts.Init(0, 0xff, this);
        mCcStts.SetRoomId(fopAcM_GetRoomNo(this));
        mCollider.Set(l_sphSrc);
        mCollider.SetStts(&mCcStts);
        tevStr.room_no = fopAcM_GetRoomNo(this);
        gravity = -5.0f;
        cXyz cStack_68;
        cXyz cStack_74;
        if (fopAcM_GetParam(this) == 0xff) {
            return cPhs_ERROR_e;
        } 
        dPath* pPath = dPath_GetRoomPath(fopAcM_GetParam(this), fopAcM_GetRoomNo(this));
        if (pPath == NULL || pPath->m_num != 2) {
            return cPhs_ERROR_e;
        }
        cStack_68 = pPath->m_points[0].m_position;
        cStack_74 = pPath->m_points[1].m_position;
        current.pos = (cStack_68 + cStack_74) * 0.5f;
        mRopeVec = cStack_74 - cStack_68;
        if (fabsf(mRopeVec.x) > fabsf(mRopeVec.z)) {
            field_0x724 = 1.0f / mRopeVec.x;
            field_0x714 = 1;
        } else {
            field_0x724 = 1.0f / mRopeVec.z;
            field_0x714 = 0;
        }
        shape_angle.y = mRopeVec.atan2sX_Z();
        shape_angle.x = mRopeVec.atan2sY_XZ();
        mDoMtx_trans(field_0x6e4, current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_ZXYrotM(field_0x6e4, shape_angle.x,
                                    shape_angle.y, 0);
        f32 dVar12 = mRopeVec.absXZ() * 0.5f + 3.0f;
        fopAcM_SetMtx(this, field_0x6e4);
        fopAcM_SetMax(this, 75.0f, 50.0f, dVar12);
        fopAcM_SetMin(this, -75.0f, -200.0f, -dVar12);
        f32 fVar1 = mRopeVec.abs();
        field_0x720 = fVar1 * (1.0f / 99.0f);
        f32 fVar13 = field_0x720 / fVar1;
        cXyz cStack_80(mRopeVec.x * fVar13,
                        mRopeVec.y * fVar13,
                        mRopeVec.z * fVar13);
        cXyz* posItr;
        cXyz* pos = mLineMat.getPos(0);
        cXyz* this_02 = field_0xbd8;
        *pos =  cStack_68;
        int i;
        for (i = 1, posItr = pos + 1; i < 99; i++, posItr++, this_02++) {
            posItr[0] = posItr[-1] + cStack_80;
            this_02[0] = posItr[0];
        }
        *posItr = cStack_74;
        offRide();
        setNormalRopePos();
    }
    return rv;
}

static int daObjCrope_Create(fopAc_ac_c* i_this) {
    fopAcM_RegisterCreateID(daObjCrope_c, i_this, "Obj_Crope");
    return static_cast<daObjCrope_c*>(i_this)->create();
}

daObjCrope_c::~daObjCrope_c() {
    dComIfG_resDelete(&mPhase, l_arcName);
}

static int daObjCrope_Delete(daObjCrope_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "Obj_Crope");
    i_this->~daObjCrope_c();
    return 1;
}

void daObjCrope_c::setNormalRopePos() {
    int i;
    cXyz* pVVar3;
    cXyz* pVVar5;
    cXyz* pdVar4;
    pVVar3 = mLineMat.getPos(0) + 1;
    pVVar5 = field_0x740;
    pdVar4 = field_0xbd8;
    cXyz cStack_34;
    cXyz cStack_40;
    cXyz cStack_4c;
    f32 local_c8;
    cXyz* pos;
    cXyz* iVar6;
    f32 dVar9;
    dKyw_get_AllWind_vec(&current.pos, &cStack_4c, &local_c8);

    if (cM_rnd() < 0.1f) {
        cStack_4c *= local_c8 * field_0x720 * 0.2f;
    } else {
        cStack_4c *= 0.0f;
    }

    for (i = 1; i < 99; i++, pVVar3++, pdVar4++, pVVar5++) {
        pVVar3[0].y = pdVar4[0].y;
        cStack_34 = pVVar3[0] - pVVar3[-1];
        cStack_34.y += gravity;
        cStack_34 += pVVar5[0];
        if (cM_rnd() < 0.6f) {
            cStack_34 += cStack_4c;
        }
        cStack_34.normalizeZP();
        pVVar3[0] = pVVar3[-1] + cStack_34 * field_0x720;
    }
    pVVar3 = mLineMat.getPos(0) + 98;
    pVVar5 = field_0x740 + 97;
    for (i = 98; i > 0; i--, pVVar3--, pVVar5--) {
        cStack_34 = pVVar3[0] - pVVar3[1];
        cStack_34.y += gravity;
        cStack_34 += pVVar5[0];
        cStack_34.normalizeZP();
        pVVar3[0] = pVVar3[1] + cStack_34 * field_0x720;
    }
    
    pVVar3 = mLineMat.getPos(0) + 1;
    pVVar5 = field_0x740;
    pdVar4 = field_0xbd8;
    iVar6 = mLineMat.getPos(0);
    for (i = 1; i < 99; i++, pVVar3++, pdVar4++, pVVar5++) {
        dVar9 = iVar6->y + (mRopeVec.y * getStartRate(pVVar3));
        pVVar5[0] = (pVVar3[0] - pdVar4[0]) * 0.49f;
        pdVar4[0] = pVVar3[0];
        pVVar3[0].y += (dVar9 - pVVar3[0].y) * 0.8f;
    }
}

void daObjCrope_c::setRideRopePos() {
    daPy_py_c* player = daPy_getLinkPlayerActorClass();
    cXyz* pcVar13;
    cXyz* local_120;
    cXyz* local_124;
    f32 dVar23;
    f32 fVar1;
    f32 fVar2;
    f32 fVar3;
    f32 fVar4;
    f32 fVar5;
    f32 fVar6;
    f32 fVar7;
    f32 fVar8;
    f32 fVar14;
    f32 dVar20;
    f32 dVar21;
    f32 dVar22;
    int i;

    if (mOutFlg) {
        setNormalRopePos();
        mOutFlg = 0;
        return;
    } 

    cXyz* pcVar15 = mLineMat.getPos(0);
    pcVar13 = mLineMat.getPos(0) + 99;
    cXyz local_54;
    cXyz local_60;
    if (mFrontJoint != -1) {
        if (mFrontJoint == -2) {
            local_54 = player->current.pos;
        } else {
            mDoMtx_multVecZero(player->getModelJointMtx(mFrontJoint), &local_54);
            local_54.y -= 5.0f;
        }
    } else {
        local_54 = *pcVar15;
    }

    if (mBackJoint != -1) {
        mDoMtx_multVecZero(player->getModelJointMtx(mBackJoint), &local_60);
        local_60.y -= 5.0f;
    } else {
        local_60 = *pcVar15;
    }
    fVar1 = local_54.x - pcVar15->x;
    fVar2 = local_54.z - pcVar15->z;
    fVar3 = pcVar13->x - local_54.x;
    fVar4 = pcVar13->z - local_54.z;
    fVar5 = local_60.x - pcVar15->x;
    fVar6 = local_60.z - pcVar15->z;
    fVar7 = pcVar13->x - local_60.x;
    fVar8 = pcVar13->z - local_60.z;
    dVar20 = pcVar15->abs(local_54);
    dVar21 = pcVar15->abs(local_60);
    dVar22 = pcVar13->abs(local_54);
    dVar23 = pcVar13->abs(local_60);

    if ((mFrontJoint == -1 ||
            fVar1 * mRopeVec.x + fVar2 * mRopeVec.z <= 0.0f) ||
        fVar3 * mRopeVec.x + fVar4 * mRopeVec.z <= 0.0f)
    {
        local_120 = &local_60;
        local_124 = &local_60;
        dVar20 = dVar21;
        dVar22 = dVar23;
    } else if ((mBackJoint == -1 ||
                fVar5 * mRopeVec.x + fVar6 * mRopeVec.z <=
                    0.0f) ||
                fVar7 * mRopeVec.x + fVar8 * mRopeVec.z <= 0.0f)
    {
        local_120 = &local_54;
        local_124 = &local_54;
    } else if (dVar20 > dVar21) {
        local_120 = &local_60;
        local_124 = &local_54;
        dVar20 = dVar21;
    } else {
        local_120 = &local_54;
        local_124 = &local_60;
        dVar22 = dVar23;
    }
    cXyz cStack_6c = *local_120 - *pcVar15;
    cXyz cStack_78 = *local_124 - *pcVar13;
    fVar14 = 1.0f / field_0x720;
    int local_150 = dVar22 * fVar14;
    if (local_150 > 0x61) {
        local_150 = 0x61;
    }
    cStack_78.normalizeZP();
    cXyz* pcVar12 = pcVar13 - 1;
    for (i = 0; i < local_150; i++, pcVar12--) {
        pcVar12[0] = pcVar12[1] + cStack_78 * field_0x720;
    }
    *pcVar12 = *local_124;
    int local_154 = dVar20 * fVar14;
    if (local_154 > 0x61) {
        local_154 = 0x61;
    }
    cStack_6c.normalizeZP();
    pcVar12 = pcVar15 + 1;
    for (i = 0; i < local_154; i++, pcVar12++) {
        pcVar12[0] = pcVar12[-1] + cStack_6c * field_0x720;
    }
    *pcVar12 = *local_120;
    int local_158 = (0x60 - local_154) - local_150;
    cXyz cStack_84 = *local_124 - *local_120;
    f32 dVar25 = cStack_84.abs() / (local_158 + 1);
    cStack_84.normalizeZP();
    pcVar12++;
    for (i = 0; i < local_158; i++, pcVar12++) {
        pcVar12[0] = pcVar12[-1] + cStack_84 * dVar25;
    }
    pcVar12 = mLineMat.getPos(0) + 1;
    cXyz* local_160 = field_0xbd8;
    cXyz* local_164 = field_0x740;
    cXyz cStack_90;

    f32 dVar20a;
    for (i = 1; i < 0x63; i++, pcVar12++, local_160++, local_164++) {
        dVar20a = pcVar15->y + mRopeVec.y * getStartRate(pcVar12);
        cStack_90 = *pcVar12;
        cStack_90.y =
            pcVar12[0].y + (dVar20a - pcVar12[0].y) * 5.0000005f;
        *local_164 = (cStack_90 - *local_160) * 0.49f;
        *local_160 = cStack_90;
    }
}

int daObjCrope_c::execute() {
    int i;
    cXyz* this_00;
    cXyz* pcVar7;
    daPy_py_c* player = daPy_getLinkPlayerActorClass();
    if (mFrontJoint == -1 && mBackJoint == -1) {
        setNormalRopePos();
        this_00 = mLineMat.getPos(0);
        pcVar7 = mLineMat.getPos(0) + 99;
        cM3dGLin acStack_7c(*this_00, *pcVar7);
        f32 dummyFloat;
        if (cM3d_Len2dSqPntAndSegLine(player->current.pos.x, player->current.pos.z, this_00->x,
                                      this_00->z, pcVar7->x, pcVar7->z, &mPlayerNearPos.x,
                                      &mPlayerNearPos.z, &dummyFloat) != 0)
        {
            cXyz* pPos = this_00 + 1;
            for (i = 1; i < 99; i++, pPos++) {
                f32 diffx = mPlayerNearPos.x - pPos->x;
                f32 diffz = mPlayerNearPos.z - pPos->z;
                if (diffx * mRopeVec.x + diffz * mRopeVec.z < 0.0f) {
                    break;
                }
            }
            f32 dVar9 = (mPlayerNearPos.absXZ(pPos[-1]) / pPos[0].absXZ(pPos[-1]));
            mPlayerNearPos = pPos[-1] + (pPos[0] - pPos[-1]) * dVar9;
        } else {
            if (this_00->abs2(player->current.pos) > pcVar7->abs2(player->current.pos)) {
                mPlayerNearPos = *pcVar7;
            } else {
                mPlayerNearPos = *this_00;
            }
        }
        mCollider.SetC(mPlayerNearPos);
        if (mCoCancelTimer != 0) {
            mCoCancelTimer--;
        } else {
            dComIfG_Ccsp()->Set(&mCollider);
        }
    } else {
        setRideRopePos();
    }
    return 1;
}

static int daObjCrope_Execute(daObjCrope_c* i_this) {
    return i_this->execute();
}

int daObjCrope_c::draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    mLineMat.update(100, 5.0f, color, 0, &tevStr);
    dComIfGd_set3DlineMat(&mLineMat);
    return 1;
}

static int daObjCrope_Draw(daObjCrope_c* i_this) {
    return i_this->draw();
}

static actor_method_class l_daObjCrope_Method = {
    (process_method_func)daObjCrope_Create,
    (process_method_func)daObjCrope_Delete,
    (process_method_func)daObjCrope_Execute,
    NULL,
    (process_method_func)daObjCrope_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Crope = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Crope,         // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjCrope_c),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  248,                    // mPriority
  &l_daObjCrope_Method,   // sub_method
  0x00040100,             // mStatus
  fopAc_ENV_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
