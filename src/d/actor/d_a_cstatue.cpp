/**
* @file d_a_cstatue.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_cstatue.h"
#include <Z2AudioLib/Z2Instances.h>
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_crod.h"
#include "d/actor/d_a_player.h"
#include "d/d_cc_d.h"
#include "d/d_tresure.h"

static u8 const unused[12] = {};

static char const l_arcName[8] = "Cstatue";

static char const l_arcNameBS[7] = "CstaBS";

enum CStatueAnim {
    CStatueAnimBFall = 7,   //	Copy Statue - B - Fall
    CStatueAnimBJump,       //	Copy Statue - B - Jump
    CStatueAnimBWalk,       //	Copy Statue - B - Walk
    CStatueAnimFall,        //	Copy Statue - Fall
    CStatueAnimStart,       //	Copy Statue - Start
    CStatueAnimStart_demo,  //	Copy Statue - Start - Demo
    CStatueAnimStop,        //	Copy Statue - Stop
    CStatueAnimSwing,       //	Copy Statue - Swing
    CStatueAnimSwingre,     //	Copy Statue - Swing Re
    CStatueAnimWalk,        //	Copy Statue - Walk
};

enum CStatueBossAnim {
    CStatueBossAnimAttackL = 6,     //	Copy Statue - Boss - Attack - Left
    CStatueBossAnimAttackR,         //	Copy Statue - Boss - Attack - Right
    CStatueBossAnimReturnWaitHitL,  //	Copy Statue - Boss - Return - Wait - Hit - Left
    CStatueBossAnimReturnWaitHitR,  //	Copy Statue - Boss - Return - Wait - Hit - Right
    CStatueBossAnimReturnWaitL,     //	Copy Statue - Boss - Return - Wait - Left
    CStatueBossAnimReturnWaitR,     //	Copy Statue - Boss - Return - Wait - Right
};

// this enum may need to be split for boss/non boss types
enum CStatueAnimIndex {
    CStatueAnimIndex_0,
    CStatueAnimIndex_1,
    CStatueAnimIndex_2,
    CStatueAnimIndex_3,
    CStatueAnimIndex_4,
    CStatueAnimIndex_5,
    CStatueAnimIndex_6,
    CStatueAnimIndex_N,
};

u16 const daCstatue_c::m_bckIdxTable[daCstatueType_N][CStatueAnimIndex_N] = {
    // daCstatueType_NORMAL
    {CStatueAnimSwing, CStatueAnimWalk, CStatueAnimSwing, CStatueAnimFall, CStatueAnimStop,
     CStatueAnimStart, CStatueAnimSwingre},
    // daCstatueType_SMALL
    {CStatueAnimBJump, CStatueAnimBWalk, CStatueAnimBJump, CStatueAnimBFall, CStatueAnimBWalk,
     CStatueAnimBWalk, CStatueAnimBJump},
    // daCstatueType_NORMAL_2
    {CStatueAnimSwing, CStatueAnimWalk, CStatueAnimSwing, CStatueAnimFall, CStatueAnimStop,
     CStatueAnimStart, CStatueAnimSwingre},
    // daCstatueType_BOSS_RIGHT
    {CStatueBossAnimReturnWaitR, CStatueBossAnimReturnWaitHitR, CStatueBossAnimAttackR,
     CStatueBossAnimReturnWaitR, CStatueBossAnimReturnWaitR, CStatueBossAnimReturnWaitR,
     CStatueBossAnimReturnWaitR},
    // daCstatueType_BOSS_LEFT
    {CStatueBossAnimReturnWaitL, CStatueBossAnimReturnWaitHitL, CStatueBossAnimAttackL,
     CStatueBossAnimReturnWaitL, CStatueBossAnimReturnWaitL, CStatueBossAnimReturnWaitL,
     CStatueBossAnimReturnWaitL},
};

struct CStatueTblData {
    u16 modelIndex;
    u16 transformIndex;
    u16 brkIndex;
    u16 textureIndex;
    u32 morfIndex;
};

static dCcD_SrcCps l_atCpsSrc = {
    {
        {0x0, {{AT_TYPE_CSTATUE_SWING, 0x4, 0x1f}, {0x0, 0x0}, 0x0}},  // mObj
        {dCcD_SE_STATUE_HAMMER, 0x0, 0x0, 0x0, 0x0},                   // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                            // mGObjTg
        {0x0},                                                         // mGObjCo
    },                                                                 // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 88.0f},  // mCps
    }  // mCpsAttr
};

static dCcD_SrcCyl l_cylSrc = {
    {
        {0x0, {{AT_TYPE_THROW_OBJ, 0x2, 0x12}, {0xd97afddf, 0x11}, 0x79}},  // mObj
        {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x0},                                // mGObjAt
        {dCcD_SE_NONE, 0x5, 0x0, 0x0, 0x3},                                 // mGObjTg
        {0x0},                                                              // mGObjCo
    },                                                                      // mObjInf
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        88.0f,               // mRadius
        392.0f               // mHeight
    }  // mCyl
};

static dCcD_SrcSph l_sphSrc = {
    {
        {0x0, {{AT_TYPE_CSTATUE_BOSS_SWING, 0x4, 0x1e}, {0xd8fafddf, 0x11}, 0x79}},  // mObj
        {dCcD_SE_STONE, 0x0, 0x1, 0x0, 0x0},                                         // mGObjAt
        {dCcD_SE_NONE, 0x5, 0x0, 0x0, 0x3},                                          // mGObjTg
        {0x0},                                                                       // mGObjCo
    },                                                                               // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 100.0f}  // mSph
    }  // mSphAttr
};

void daCstatue_c::atHitCallback(fopAc_ac_c* actor) {
    if (fopAcM_GetName(actor) == PROC_B_GM) {
        onStateFlg0(daCstatue_FLG0_100);
    }
}

static void daCstatue_atHitCallback(fopAc_ac_c* statue, dCcD_GObjInf* _objInfA, fopAc_ac_c* actor,
                                    dCcD_GObjInf* _objInfB) {
    static_cast<daCstatue_c*>(statue)->atHitCallback(actor);
}

int daCstatue_c::createHeap() {
    static struct CStatueTblData const dataTbl[daCstatueType_N] = {
        // daCstatueType_NORMAL
        {19, 13, 31, 35, 0x11000284},
        // daCstatueType_SMALL
        {20, 8, 28, 0, 0x11000084},
        // daCstatueType_NORMAL_2
        {24, 13, 29, 34, 0x11000284},
        // daCstatueType_BOSS_RIGHT
        {14, 11, 18, 21, 0x11000284},
        // daCstatueType_BOSS_LEFT
        {14, 10, 18, 21, 0x11000284},
    };

    const struct CStatueTblData* data = &dataTbl[mType];

    J3DAnmTransform* animation =
        static_cast<J3DAnmTransform*>(dComIfG_getObjectRes(mResName, data->transformIndex));

    int resource_index;
    if (checkStateFlg0(daCstatue_FLG0_400)) {
        resource_index = 21;
    } else {
        resource_index = data->modelIndex;
    }

    mpMorf = new mDoExt_McaMorfSO(
        static_cast<J3DModelData*>(dComIfG_getObjectRes(mResName, (u16)resource_index)), NULL, NULL,
        animation, 0, 0.0f, animation->getFrameMax(), -1, &mSound,
        mType == daCstatueType_Normal2 ? J3DMdlFlag_None : J3DMdlFlag_DifferedDLBuffer, data->morfIndex);

    if (mpMorf == NULL || mpMorf->mpModel == NULL) {
        return cPhs_INIT_e;
    }

    mModel = mpMorf->mpModel;
    if (!initBrk(data->brkIndex)) {
        return cPhs_INIT_e;
    }
    if (data->textureIndex) {
        J3DAnmTextureSRTKey* key =
            (J3DAnmTextureSRTKey*)(dComIfG_getObjectRes(mResName, data->textureIndex));
        if (!mAnim1.init(mModel->getModelData(), key, 1, -1, 1.0f, 0, -1)) {
            return cPhs_INIT_e;
        }
    }

    if (checkBossType()) {
        mSph = new dCcD_Sph[9]();
        if (!mSph) {
            return cPhs_INIT_e;
        }
    } else if (mType == daCstatueType_Normal) {
        mCps1 = new dCcD_Cps();
        if (!mCps1) {
            return cPhs_INIT_e;
        }
        mCps2 = new dCcD_Cps();
        if (!mCps2) {
            return cPhs_INIT_e;
        }
        mCyl2 = new dCcD_Cyl();
        if (!mCyl2) {
            return cPhs_INIT_e;
        }
    } else if (mType == daCstatueType_Normal2 && !mInvisible.create(mpMorf->getModel(), 1)) {
        return cPhs_INIT_e;
    }
    return cPhs_LOADING_e;
}

static int daCstatue_createHeap(fopAc_ac_c* actor) {
    return static_cast<daCstatue_c*>(actor)->createHeap();
}

int daCstatue_c::create() {
    static f32 const bossSphR[9] = {450.0f, 200.0f, 250.0f, 220.0f, 280.0f,
                                    200.0f, 250.0f, 220.0f, 280.0f};

    static int const heapSize[daCstatueType_N] = {4368, 2208, 4688, 6240, 6240};

    fopAcM_ct(this, daCstatue_c);
    mType = (fopAcM_GetParam(this) >> 8) & 0xf;
    if (mType == daCstatueType_Normal2) {
        mType = daCstatueType_Small;
        onStateFlg0(daCstatue_FLG0_8);
    } else if (mType == daCstatueType_BossRight) {
        mType = daCstatueType_Normal;
        onStateFlg0(daCstatue_FLG0_400);
    } else if (mType > daCstatueType_Normal2) {
        // if the type is a boss type, try to map it to a non-boss type
        mType -= daCstatueType_Normal2;
        // or, if it is completely out-of-bounds make a normal type
        if (mType >= daCstatueType_N) {
            mType = daCstatueType_Normal;
        }
    }
    if (checkBossType()) {
        mResName = l_arcNameBS;
    } else {
        mResName = l_arcName;
    }
    int result = dComIfG_resLoad(&mPhaseReq, mResName);
    if (result == cPhs_COMPLEATE_e) {
        mParam0 = fopAcM_GetParam(this) & 0xff;
        mParam1 = (fopAcM_GetParam(this) >> 12) & 0xff;
        mParam2 = (fopAcM_GetParam(this) >> 20) & 0x3f;

        if (mType == daCstatueType_Normal && !checkStateFlg0(daCstatue_FLG0_400)) {
            if (mParam0 != 0xff && !fopAcM_isSwitch(this, mParam0)) {
                return cPhs_ERROR_e;
            }
            int minSwitch;
            if (mParam0 == 0xff) {
                minSwitch = 0;
            } else {
                minSwitch = mParam0 + 1;
            }
            for (int iSwitch = 6; iSwitch >= minSwitch; iSwitch--) {
                if (fopAcM_isSwitch(this, iSwitch)) {
                    return cPhs_ERROR_e;
                }
            }
        } else if (mType == daCstatueType_Normal2 && mParam0 != 0xff &&
                   fopAcM_isSwitch(this, this->mParam0))
        {
            return cPhs_ERROR_e;
        }

        if (!fopAcM_entrySolidHeap(this, daCstatue_createHeap, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        mpMorf->setMorf(1.0f);
        if (mType == daCstatueType_Normal) {
            mCurrentAnim = CStatueAnimIndex_4;
        } else {
            mCurrentAnim = CStatueAnimIndex_0;
        }
        if (mType == daCstatueType_Normal || mType == daCstatueType_Normal2) {
            scale.set(1.6f, 1.6f, 1.6f);
        }
        mModel->setBaseScale(scale);
        mpMorf->play(0, 0);
        mCyl1.Set(l_cylSrc);
        mCyl1.SetStts(&mStts);
        mCyl1.StartCAt(current.pos);
        if (checkStateFlg0(daCstatue_FLG0_400)) {
            mCyl1.SetTgType(mCyl1.GetTgType() & ~0x01000000);
        }
        if (mType == daCstatueType_Normal) {
            mCps1->Set(l_atCpsSrc);
            mCps1->SetStts(&mStts);
            mCps2->Set(l_atCpsSrc);
            mCps2->SetStts(&mStts);
            mCps2->SetR(88.0f);
            mCyl2->Set(l_cylSrc);
            mCyl2->SetStts(&mStts);
            mCyl2->SetAtType(0x2000000);
            mCyl2->SetAtAtp(1);
            mCyl2->OffTgSetBit();
            mCyl2->OffCoSetBit();
            mCyl2->SetAtSe(21);
            mCyl2->SetR(384.0f);
            mCyl2->SetH(200.0f);
            if (mParam2 != 0x3f) {
                onStateFlg0(daCstatue_FLG0_1000);
                dComIfGs_onTbox(mParam2);
                dComIfGs_offTbox(mParam2 - 1);
            }
        } else if (mType == daCstatueType_Normal2) {
            int anyTbox = false;
            for (int iBox = 21; iBox <= 31; iBox += 2) {
                if (dComIfGs_isTbox(iBox)) {
                    dComIfGs_onTbox(iBox + -1);
                    dComIfGs_offTbox(iBox);
                    break;
                }
            }
            for (int iBox = 21; iBox <= 33; iBox++) {
                if (dComIfGs_isTbox(iBox)) {
                    anyTbox = true;
                    break;
                }
            }
            if (!anyTbox) {
                dComIfGs_onTbox(20);
            }
        }
        attention_info.distances[4] = 8;
        fopAcM_SetMtx(this, mModel->getBaseTRMtx());
        int acchTblSize;
        if (mType == daCstatueType_Normal || mType == daCstatueType_Normal2) {
            mAcchCir[0].SetWall(30.01f, 88.0f);
            mAcchCir[1].SetWall(208.0f, 88.0f);
            mAcchCir[2].SetWall(288.0f, 88.0f);
            mAcchCir[3].SetWall(392.0f, 88.0f);
            if (checkStateFlg0(daCstatue_FLG0_400)) {
                mStts.Init(0xff, 0, this);
            } else {
                mStts.Init((u8)-2, 0, this);
            }
            fopAcM_SetMax(this, 425.6f, 528.0f, 425.6f);
            fopAcM_SetMin(this, -425.6f, 0.0f, -425.6f);
            mSomePos.x = 1440.0f;
            gravity = -8.0f;
            maxFallSpeed = -100.0f;
            acchTblSize = 4;
            mControlDistanceOffset = JMAFastSqrt(mCyl1.GetR() * mCyl1.GetR() +
                                                 mCyl1.GetH() * mCyl1.GetH() * 0.6f * 0.6f);
            mTargetFrame = 35.0f;
        } else if (mType == daCstatueType_Small) {
            mAcchCir[0].SetWall(30.01f, 35.0f);
            mCyl1.SetR(35.0f);
            mCyl1.SetH(100.0f);
            mStts.Init((u8)-76, 0, this);
            fopAcM_SetMax(this, 145.0f, 250.0f, 145.0f);
            fopAcM_SetMin(this, -145.0f, 0.0f, -145.0f);
            mSomePos.x = 300.0f;
            gravity = -5.0f;
            maxFallSpeed = -100.0f;
            fopAcM_OnCarryType(this, fopAcM_CARRY_HEAVY);
            acchTblSize = 1;
            mControlDistanceOffset = JMAFastSqrt(4825.0f);
            mTargetFrame = 21.0f;
        } else {
            fopAcM_OffStatus(this, 0x100);
            mCyl1.SetR(300.0f);
            mCyl1.SetH(600.0f);
            mAcchCir[0].SetWall(30.01f, 300.0f);
            mStts.Init((u8)-2, 0, this);
            mSomePos.x = 5500.0f;
            gravity = -8.0f;
            maxFallSpeed = -100.0f;
            acchTblSize = 1;
            dCcD_Sph* obj = mSph;
            for (int iSph = 0; iSph < 9; iSph++, obj++) {
                obj->Set(l_sphSrc);
                obj->SetStts(&mStts);
                obj->SetR(bossSphR[iSph]);
                obj->SetAtHitCallback(daCstatue_atHitCallback);
            }
            mSph->SetTgType(0xd97afddf);
            mControlDistanceOffset = JMAFastSqrt(650000.0f);
            attention_info.distances[0] = 92;
            cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_LOCK_e);
            mTargetFrame = 35.0f;
        }
        mControlDistanceOffset += 100.0f;
        mSomePos.y = current.pos.y;
        mStatueAcch.Set(this, acchTblSize, mAcchCir);
        model = mModel;
        initStopBrkBtk();
        mAnim2.setFrame(mAnim2.getEndFrame());
        mStatueAcch.CrrPos(dComIfG_Bgsp());
        setMatrix();
        setRoomInfo();
        mLinkPos = daPy_getLinkPlayerActorClass()->current.pos;

        mPaPo.init(&mStatueAcch, 30.0f, mCyl1.GetH());
        mSound.init(&current.pos, &eyePos, 4, 1);
        // todo: debug rom
        // ::mDoHIO_entry_c::entryHIO((mDoHIO_entry_c*)&d_a_cstatue::l_HIO, &d_a_cstatue::@129695);
    }
    return result;
}

static int daCstatue_Create(void* actor) {
    return static_cast<daCstatue_c*>(actor)->create();
}

daCstatue_c::~daCstatue_c() {
    mSound.deleteObject();
    if (mpMorf) {
        mpMorf->stopZelAnime();
    }
    if (checkStateFlg0(daCstatue_FLG0_1000)) {
        dComIfGs_offTbox(mParam2);
        dComIfGs_onTbox(mParam2 - 1);
    }
    dComIfG_resDelete(&mPhaseReq, mResName);
}

static int daCstatue_Delete(void* actor) {
    static_cast<daCstatue_c*>(actor)->~daCstatue_c();
    return cPhs_LOADING_e;
}

void daCstatue_c::setRoomInfo() {
    int roomId;
    if (mStatueAcch.GetGroundH() != -G_CM3D_F_INF) {
        roomId = dComIfG_Bgsp().GetRoomId(mStatueAcch.m_gnd);
        tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mStatueAcch.m_gnd);
    } else {
        roomId = dComIfGp_roomControl_getStayNo();
    }
    tevStr.room_no = roomId;
    mReverb = dComIfGp_getReverb(roomId);
    mStts.SetRoomId(roomId);
    fopAcM_SetRoomNo(this, roomId);
}

void daCstatue_c::setMatrix() {
    static Vec const normalLocalBallPos = {70.0f, 0.0f, 0.0f};
    static Vec const smallLocalBallPos = {0.0f, 35.0f, 0.0f};
    static Vec const bossLocalBallPos = {420.0f, 0.0f, 0.0f};

    // todo: there is an additional block here in the debug rom
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::transM(0.0f, mSomePos.z, 0.0f);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
    if (mType == daCstatueType_Normal || mType == daCstatueType_Normal2) {
        attention_info.position.set(current.pos.x, current.pos.y + scale.y * 140.0f, current.pos.z);
        mDoMtx_multVec(mModel->getAnmMtx(1), &normalLocalBallPos, &mBallPos);
    } else if (mType == daCstatueType_Small) {
        attention_info.position.set(current.pos.x, current.pos.y + mCyl1.GetH(), current.pos.z);
        mDoMtx_multVec(mModel->getAnmMtx(0), &smallLocalBallPos, &mBallPos);
    } else {
        attention_info.position.set(current.pos.x, current.pos.y + bossLocalBallPos.x,
                                    current.pos.z);
        mDoMtx_multVec(mModel->getAnmMtx(0), &bossLocalBallPos, &mBallPos);
    }
    eyePos = attention_info.position;
}

void daCstatue_c::posMove() {
    if (checkStateFlg0(daCstatue_FLG0_8)) {
        mStatueAcch.CrrPos(dComIfG_Bgsp());
        if (mStatueAcch.GetGroundH() == -G_CM3D_F_INF) {
            return;
        }
        if (dComIfG_Bgsp().ChkMoveBG_NoDABg(mStatueAcch.m_gnd)) {
            offStateFlg0(daCstatue_FLG0_8);
            current.pos.y = mStatueAcch.GetGroundH();
            mSomePos.y = current.pos.y;
        } else {
            return;
        }
    }
    int groundHit = mStatueAcch.ChkGroundHit();
    mStatueAcch.OffLineCheck();
    if (!fopAcM_checkCarryNow(this)) {
        speed.y += gravity;
        if (speed.y < maxFallSpeed) {
            speed.y = maxFallSpeed;
        }
        current.pos.y += speed.y;
    }
    daAlink_c* link = daAlink_getAlinkActorClass();
    if (!checkStateFlg0(daCstatue_FLG0_Demo) && !mWarpMode &&
        link->getCopyRodControllActor() == this)
    {
        if (!checkStateFlg0(daCstatue_FLG0_800)) {
            shape_angle.y = link->shape_angle.y;
        }
        if (mCurrentAnim == CStatueAnimIndex_6) {
            speedF = 0.0f;
        } else if (mStatueAcch.ChkGroundHit() && mCurrentAnim != CStatueAnimIndex_3) {
            current.pos.x = current.pos.x + speed.x;
            current.pos.z = current.pos.z + speed.z;
            speedF = JMAFastSqrt(speed.x * speed.x + speed.z * speed.z);
            if (mCurrentAnim == CStatueAnimIndex_1) {
                f32 playSpeed = mpMorf->getPlaySpeed();
                f32 targetSpeed;
                if (speedF >= 1.0f) {
                    current.angle.y = speed.atan2sX_Z();
                    targetSpeed = speedF / 18.0f;
                    if (targetSpeed > 1.0f) {
                        targetSpeed = 1.0f;
                    }

                    if (mType != daCstatueType_Small) {
                        targetSpeed = 0.5f + targetSpeed;
                    } else {
                        targetSpeed = 0.5f + targetSpeed;
                    }
                } else {
                    current.angle.y = shape_angle.y;
                    targetSpeed = 0.0f;
                }
                cLib_chaseF(&playSpeed, targetSpeed, 0.2f);
                mpMorf->setPlaySpeed(playSpeed);
            }
        }
    } else {
        if (!fopAcM_checkCarryNow(this) && mMoveMode != daCstatue_MoveMode_0) {
            if (mStatueAcch.ChkGroundHit()) {
                if (mMoveMode != daCstatue_MoveMode_3) {
                    mpMorf->setAnm(static_cast<J3DAnmTransform*>(dComIfG_getObjectRes(mResName, 7)),
                                   -1, 0.0f, 1.0f, 0.0f, -1.0f);
                    mCurrentAnim = CStatueAnimIndex_3;
                    if (speedF > 1.0f) {
                        speedF = 6.0f;
                        onStateFlg0(daCstatue_FLG0_200);
                    } else {
                        speedF = 0.0f;
                    }
                } else {
                    speedF = 0.0f;
                }
                mMoveMode = daCstatue_MoveMode_0;
            } else {
                if (mStatueAcch.ChkWallHit()) {
                    speedF *= 0.25f;
                    current.angle.y =
                        2 * mAcchCir[0].GetWallAngleY() - (current.angle.y - (s16)0x8000);
                }
                current.pos.x += speedF * cM_ssin(current.angle.y);
                current.pos.z += speedF * cM_scos(current.angle.y);
                if (mMoveMode == daCstatue_MoveMode_1) {
                    mMoveMode = daCstatue_MoveMode_2;
                    mStatueAcch.OnLineCheck();
                }
            }
        }
        if (mCurrentAnim == CStatueAnimIndex_0) {
            mpMorf->setPlaySpeed(0.0f);
            if (mMoveMode == daCstatue_MoveMode_0) {
                speedF = 0.0f;
            }
        } else if (mCurrentAnim == CStatueAnimIndex_3 && checkStateFlg0(daCstatue_FLG0_200)) {
            if (mpMorf->checkFrame(7.0f)) {
                speedF = 3.0f;
            } else if (mpMorf->getFrame() > 12.0f) {
                speedF = 0.0f;
            } else if (mpMorf->getFrame() > 7.0f) {
                cLib_chaseF(&speedF, 0.0f, 0.4f);
            }
            current.pos.x = current.pos.x + speedF * cM_ssin(current.angle.y);
            current.pos.z = current.pos.z + speedF * cM_scos(current.angle.y);
        }
    }
    mLinkPos = link->current.pos;
    if (!fopAcM_checkCarryNow(this)) {
        current.pos += *mStts.GetCCMoveP();
        mStts.ClrCcMove();
        mStatueAcch.CrrPos(dComIfG_Bgsp());
        if (groundHit && !mStatueAcch.ChkGroundHit() && mStatueAcch.GetGroundH() != -G_CM3D_F_INF)
        {
            f32 groundDistance = mStatueAcch.GetGroundH() - current.pos.y;
            cM3dGPla plane;
            dComIfG_Bgsp().GetTriPla(mStatueAcch.m_gnd, &plane);
            s16 polyAngle = fopAcM_getPolygonAngle(&plane, current.angle.y);
            f32 angle = current.pos.absXZ(old.pos) * cM_ssin(polyAngle);
            if (angle < 0.0f) {
                angle = 0.0f;
            }
            if (groundDistance < 0.0f && groundDistance >= -30.0f - angle) {
                current.pos.y = mStatueAcch.GetGroundH();
                mStatueAcch.SetGroundHit();
                speed.y = 0.0f;
            } else {
                mSomePos.y = old.pos.y;
            }
        }
        if (!groundHit && mStatueAcch.ChkGroundHit() && mSomePos.y - current.pos.y >= 100.0f) {
            J3DAnmTransform* animation = static_cast<J3DAnmTransform*>(
                dComIfG_getObjectRes(mResName, m_bckIdxTable[mType][CStatueAnimIndex_3]));
            mpMorf->setAnm(animation, -1, 0.0f, 1.0f, 0.0f, -1.0f);
            mCurrentAnim = CStatueAnimIndex_3;
            offStateFlg0(daCstatue_FLG0_200);
        }
        offStateFlg0(daCstatue_FLG0_80);
        mSomePos.z = 0.0f;
    } else {
        speed.y = 0.0f;
        mMoveMode = daCstatue_MoveMode_1;
        cXyz pos(current.pos);
        mStatueAcch.CrrPos(dComIfG_Bgsp());
        (link->setGrabCollisionOffset)(current.pos.x - pos.x, current.pos.z - pos.z, 0);
        current.pos = pos;
        mSomePos.y = current.pos.y;
        if (link->getGrabUpStart()) {
            onStateFlg0(daCstatue_FLG0_80);
        } else if (link->getGrabPutStart()) {
            offStateFlg0(daCstatue_FLG0_80);
        }
        f32 targetZ;
        if (checkStateFlg0(daCstatue_FLG0_80)) {
            targetZ = 10.0f;
        } else {
            targetZ = 0.0f;
        }
        cLib_chaseF(&mSomePos.z, targetZ, 0.75f);
    }
}

void daCstatue_c::setCollision() {
    static Vec const localTop = {0.0f, 90.0f, 0.0f};
    static Vec const localRoot = {0.0f, -30.0f, 0.0f};
    static Vec const hammerSide0 = {-40.0f, 120.0f, 0.0f};
    static Vec const hammerSide1 = {40.0f, 120.0f, 0.0f};
    static u16 const effName[4] = {0x09D0, 0x09D1, 0x09D2, 0x09D5};
    static u16 const bossJntIdx[9] = {2, 6, 7, 7, 8, 3, 4, 4, 5};

    static Vec const bossLocalOffset[9] = {
        {0.0f, 0.0f, 0.0f},   {200.0f, 0.0f, 0.0f},   {50.0f, 0.0f, 0.0f},
        {380.0f, 0.0f, 0.0f}, {200.0f, 0.0f, -20.0f}, {200.0f, 0.0f, 0.0f},
        {50.0f, 0.0f, 0.0f},  {380.0f, 0.0f, 0.0f},   {200.0f, 0.0f, -20.0f},
    };

    mStts.Move();
    if (!fopAcM_checkCarryNow(this) && mType != daCstatueType_Normal2) {
        if (mMoveMode == daCstatue_MoveMode_2 && speedF > 1.0f) {
            mCyl1.OnAtSetBit();
        } else {
            mCyl1.OffAtSetBit();
        }
        mCyl1.MoveCAt(current.pos);
        dComIfG_Ccsp()->Set(&mCyl1);
    } else {
        mCyl1.ResetTgHit();
        mCyl1.ResetAtHit();
        mCyl1.ResetCoHit();
    }
    if (mType == daCstatueType_Normal) {
        if (mCyl2->ChkAtSet()) {
            f32 radius = mCyl2->GetR();
            cLib_chaseF(&radius, 384.0f, 36.0f);
            mCyl2->SetR(radius);
        }

        if (mCurrentAnim == CStatueAnimIndex_2 && mpMorf->getFrame() >= 13.0f &&
            mpMorf->getFrame() < 21.0f)
        {
            cXyz direction(cM_ssin(shape_angle.y) * 10.0f, 0.0f, cM_scos(shape_angle.y) * 10.0f);
            cXyz startPos;
            cXyz endPos;
            mDoMtx_multVec(mModel->getAnmMtx(5), &localTop, &endPos);
            mDoMtx_multVec(mModel->getAnmMtx(5), &localRoot, &startPos);
            mCps1->SetStartEnd(startPos, endPos);
            mCps1->SetAtVec(direction);
            dComIfG_Ccsp()->Set(mCps1);
            mDoMtx_multVec(mModel->getAnmMtx(5), &hammerSide0, &startPos);
            mDoMtx_multVec(mModel->getAnmMtx(5), &hammerSide1, &endPos);
            mCps2->SetStartEnd(startPos, endPos);
            mCps2->SetAtVec(direction);
            dComIfG_Ccsp()->Set(mCps2);
            onStateFlg0(daCstatue_FLG0_10);

            if (mpMorf->checkFrame(17.0f)) {
                cXyz center = (mCps2->GetStart() + mCps2->GetEnd()) * 0.5f;
                if (!fopAcM_gc_c::gndCheck(&center) ||
                    fopAcM_gc_c::getGroundY() < current.pos.y - 10.0f)
                {
                    onStateFlg0(daCstatue_FLG0_20);
                } else {
                    dBgS_ObjGndChk* gndCheck = fopAcM_gc_c::getGroundCheck();
                    mSound.startCollisionSE(Z2SE_HIT_STATUE_HAMMER, dKy_pol_sound_get(gndCheck));
                    f32 distance = mCps2->GetEnd().abs2(daAlink_getAlinkActorClass()->current.pos);
                    if (distance < 160000.0f) {
                        dComIfGp_getVibration().StartShock(VIBMODE_S_POWER4, 0x1f,
                                                           cXyz(0.0f, 1.0f, 0.0f));
                    } else if (distance < 640000.0f) {
                        dComIfGp_getVibration().StartShock(VIBMODE_S_POWER2, 0x1f,
                                                           cXyz(0.0f, 1.0f, 0.0f));
                    }
                    center.y = fopAcM_gc_c::getGroundY() - 100.0f;
                    mCyl2->SetC(center);
                    mCyl2->OnAtSetBit();
                    mCyl2->SetR(240.0f);
                    dComIfG_Ccsp()->Set(mCyl2);
                    cM3dGPla plane;
                    fopAcM_gc_c::getTriPla(&plane);
                    csXyz rotation(cM_atan2s(plane.mNormal.absXZ(), plane.mNormal.y),
                                   plane.mNormal.atan2sX_Z(), 0);
                    center.y = fopAcM_gc_c::getGroundY();
                    for (int iEffect = 0; iEffect < 4; iEffect++) {
                        dComIfGp_particle_set(effName[iEffect], &center, &tevStr, &rotation, 0);
                    }
                }
            } else {
                if (mCyl2->ChkAtSet()) {
                    dComIfG_Ccsp()->Set(mCyl2);
                } else {
                    mCyl2->OffAtSetBit();
                    mCyl2->ResetAtHit();
                }
            }
        } else {
            if (mCurrentAnim == CStatueAnimIndex_2 && mCyl2->ChkAtSet() &&
                mpMorf->getFrame() < 30.0f)
            {
                dComIfG_Ccsp()->Set(mCyl2);
            } else {
                mCps1->ResetAtHit();
                mCps2->ResetAtHit();
                mCyl2->OffAtSetBit();
                mCyl2->ResetAtHit();
                offStateFlg0(daCstatue_FLG0_10);
            }
        }
    }
    static cXyz effScale(2.0f, 2.0f, 2.0f);
    if (!mSph) {
        return;
    }
    cXyz spherePos;
    dCcD_Sph* sphere;
    if (mType == daCstatueType_BossLeft) {
        sphere = &mSph[7];
    } else {
        sphere = &mSph[3];
    }
    for (int iSphere = 0; iSphere < 2; iSphere++) {
        if (mCurrentAnim == CStatueAnimIndex_2) {
            sphere->OnAtSetBit();
        } else {
            sphere->OffAtSetBit();
        }
        sphere++;
    }

    sphere = &mSph[0];
    int index;
    for (index = 0; index < 9; index++, sphere++) {
        MtxP mtx = mModel->getAnmMtx(bossJntIdx[index]);
        mDoMtx_multVec(mtx, &bossLocalOffset[index], &spherePos);
        sphere->MoveCAt(spherePos);
        dComIfG_Ccsp()->Set(sphere);
    }
    if (mBossAtGndHit) {
        if (mType == daCstatueType_BossLeft) {
            sphere = &mSph[8];
        } else {
            sphere = &mSph[4];
        }
        cXyz particlePos(sphere->GetC().x, current.pos.y, sphere->GetC().z);
        for (index = 0; index < 4; index++) {
            dComIfGp_particle_set(effName[index], &particlePos, &tevStr, &csXyz::Zero, &effScale);
            mSound.startCreatureSound(Z2SE_CSTATUE_GM_HIT_GROUND, 0, mReverb);
        }
    }
}

BOOL daCstatue_c::checkHammerReverse() {
    if (!checkStateFlg0(daCstatue_FLG0_10)) {
        return false;
    }

    cXyz start(current.pos.x, current.pos.y + mCyl1.GetH() * 0.5f, current.pos.z);
    cXyz end = (mCps2->GetStart() + mCps2->GetEnd()) * 0.5f;

    if (fopAcM_lc_c::lineCheck(&start, &end, this) && fopAcM_lc_c::getWallCode() != 7 &&
        fopAcM_lc_c::checkWallHit())
    {
        dBgS_LinChk* lineCheck = fopAcM_lc_c::getLineCheck();
        mSound.startCollisionSE(Z2SE_HIT_STATUE_HAMMER, dKy_pol_sound_get(lineCheck));
        return true;
    }
    return false;
}

void daCstatue_c::setDemo() {
    static Vec const hitEffectCenter = {-2.031f, 120.438995f, 320.46704f};
    static Vec const hitEffectEye = {-2.031f, -80.761f, 642.828f};
    static Vec const behindCenter = {24.497f, 203.11401f, -95.48901f};
    static Vec const behindEye = {124.399f, 198.268f, -462.0869f};
    static Vec const startAnimeCenter = {80.604f, 269.01398f, 294.203f};
    static Vec const startAnimeEye = {170.996f, 272.64398f, 654.18896f};

    onStateFlg0(daCstatue_FLG0_Demo);
    cXyz eyePos;
    cXyz centerPos;
    if (eventInfo.checkCommandDemoAccrpt()) {
        camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
        fopAcM_onSwitch(this, mParam1);
        if (mDemoMode == daCstatue_DemoMode_0) {
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            mDoMtx_multVec(mModel->getBaseTRMtx(), &hitEffectCenter, &centerPos);
            mDoMtx_multVec(mModel->getBaseTRMtx(), &hitEffectEye, &eyePos);
            camera->mCamera.Set(centerPos, eyePos);
            mDemoMode = daCstatue_DemoMode_1;
            mDemoTimer = 100;
        } else if (mDemoMode == daCstatue_DemoMode_1) {
            if (mDemoTimer != 0) {
                mDemoTimer--;
            } else {
                mDoMtx_multVec(mModel->getBaseTRMtx(), &behindCenter, &centerPos);
                mDoMtx_multVec(mModel->getBaseTRMtx(), &behindEye, &eyePos);
                camera->mCamera.Set(centerPos, eyePos);
                initStartBrkBtk();
                mDemoMode = daCstatue_DemoMode_2;
                mDemoTimer = 30;
            }
        } else {
            if (mAnim2.checkFrame(mTargetFrame)) {
                mSound.startCreatureSound(Z2SE_CSTATUE_L_START, 0, mReverb);
            } else if (mAnim2.getFrame() >= mTargetFrame) {
                mSound.startCreatureVoiceLevel(Z2SE_CSTATUE_ACTIVE_LOOP, mReverb);
            }
            if (mDemoMode == daCstatue_DemoMode_2) {
                mAnim1.play();
                mAnim2.play();

                if (mAnim1.isStop() && mAnim2.isStop()) {
                    if (mDemoTimer) {
                        mDemoTimer--;
                    } else {
                        mDoMtx_multVec(mModel->getBaseTRMtx(), &startAnimeCenter, &centerPos);
                        mDoMtx_multVec(mModel->getBaseTRMtx(), &startAnimeEye, &eyePos);
                        camera->mCamera.Set(centerPos, eyePos);
                        J3DAnmTransform* animation =
                            static_cast<J3DAnmTransform*>(dComIfG_getObjectRes(mResName, 12));
                        mpMorf->mDoExt_McaMorfSO::setAnm(animation, -1, 3.0f, 1.0f, 0.0f, -1.0f);
                        // this is out-of-bounds of the animation table
                        mCurrentAnim = CStatueAnimIndex_N;
                        mDemoTimer = 30;
                        mDemoMode = daCstatue_DemoMode_3;
                    }
                }
            } else if (mpMorf->isStop()) {
                if (mDemoTimer) {
                    mDemoTimer--;
                } else {
                    camera->mCamera.Start();
                    dComIfGp_event_reset();
                }
            }
        }
    } else {
        fopAcM_orderPotentialEvent(this, 2, (u16)-1, 0);
        eventInfo.onCondition(2);
    }
}

void daCstatue_c::setAnime() {
    daAlink_c* link = daAlink_getAlinkActorClass();
    daCstatue_c* controlledActor = static_cast<daCstatue_c*>(link->getCopyRodControllActor());
    if (eventInfo.checkCommandDemoAccrpt() ||
        (mParam1 != 0xff && !fopAcM_isSwitch(this, mParam1) && controlledActor == this))
    {
        setDemo();
        return;
    }

    if (controlledActor == this) {
        f32 maxDistance = mControlDistanceOffset + link->getCopyRodBallDisMax();
        if (link->getCopyRodActor()->current.pos.abs2(link->current.pos) > SQUARE(maxDistance)) {
            static_cast<daCrod_c*>(link->getCopyRodActor())->offControll();
            controlledActor = NULL;
        }
    }
    if (link->checkCopyRodSwingModeInit()) {
        offStateFlg0(daCstatue_FLG0_20);
    }
    int mNewAnim;
    if (checkBossType()) {
        if ((mCurrentAnim == CStatueAnimIndex_6 || mCurrentAnim == CStatueAnimIndex_1) &&
            !mpMorf->isStop())
        {
            mNewAnim = mCurrentAnim;
        } else if (mCurrentAnim == CStatueAnimIndex_2) {
            if (mpMorf->isStop()) {
                mNewAnim = CStatueAnimIndex_6;
                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
                mBossAtGndHit = true;
            } else if (checkStateFlg0(daCstatue_FLG0_100)) {
                mTimer1 = 15;
                mNewAnim = CStatueAnimIndex_1;
                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
                static_cast<daCrod_c*>(link->getCopyRodActor())->offControll();
                mSound.startCreatureSound(Z2SE_CSTATUE_GM_HIT_GROUND, 0, mReverb);
            } else {
                mNewAnim = CStatueAnimIndex_2;
            }
        } else if (link->checkCopyRodSwingMode() && controlledActor == this) {
            mNewAnim = CStatueAnimIndex_2;
            fopAcM_onSwitch(this, 0xe2);
        } else {
            mNewAnim = CStatueAnimIndex_0;
        }
    } else if (!mWarpMode && controlledActor == this) {
        if (dComIfGp_event_runCheck()) {
            speed.x = 0.0f;
            speed.z = 0.0f;
        } else {
            speed.x = link->speed.x + (f32)link->transAnimeMoveX();
            speed.z = link->speed.z + (f32)link->transAnimeMoveZ();
        }
        if (mCurrentAnim == CStatueAnimIndex_6 && !mpMorf->isStop()) {
            offStateFlg0(daCstatue_FLG0_800);
            if (link->checkCopyRodSwingModeInit()) {
                mNewAnim = CStatueAnimIndex_2;
            } else {
                mNewAnim = CStatueAnimIndex_6;
            }
        } else if (link->checkCopyRodSwingMode()) {
            offStateFlg0(daCstatue_FLG0_800);
            if (checkStateFlg0(daCstatue_FLG0_20)) {
                mNewAnim = CStatueAnimIndex_0;
            } else {
                if (checkHammerReverse()) {
                    mNewAnim = CStatueAnimIndex_6;
                } else {
                    mNewAnim = CStatueAnimIndex_2;
                }
            }
        } else if (mCurrentAnim == CStatueAnimIndex_3 && !mpMorf->isStop()) {
            offStateFlg0(daCstatue_FLG0_800);
            mNewAnim = CStatueAnimIndex_3;
        } else if (fabsf(speed.x) + fabsf(speed.z) > 0.1f && mStatueAcch.ChkGroundHit()) {
            offStateFlg0(daCstatue_FLG0_800);
            mNewAnim = CStatueAnimIndex_1;
        } else {
            if (mCurrentAnim == CStatueAnimIndex_4 ||
                (mCurrentAnim == CStatueAnimIndex_5 && !mpMorf->isStop()))
            {
                mNewAnim = CStatueAnimIndex_5;
            } else {
                mNewAnim = CStatueAnimIndex_0;
            }
        }
    } else if (mType == daCstatueType_Normal) {
        mNewAnim = CStatueAnimIndex_4;
    } else if (mCurrentAnim == CStatueAnimIndex_3 && !mpMorf->isStop()) {
        mNewAnim = CStatueAnimIndex_3;
    } else {
        mNewAnim = CStatueAnimIndex_0;
    }
    if (mNewAnim != mCurrentAnim ||
        (mNewAnim == CStatueAnimIndex_2 && mpMorf->isStop() && link->checkCopyRodSwingModeInit()))
    {
        J3DAnmTransform* animation = static_cast<J3DAnmTransform*>(
            dComIfG_getObjectRes(mResName, m_bckIdxTable[mType][mNewAnim]));

        f32 animStartFrame =
            mNewAnim == CStatueAnimIndex_0 ? animation->getFrameMax() - 0.001f : 0.0f;
        f32 animSpeed = 1.0f;
        if (mSph) {
            if (mNewAnim == CStatueAnimIndex_2) {
                animSpeed = 5.0f;
            } else if (mNewAnim == CStatueAnimIndex_6 || mNewAnim == CStatueAnimIndex_1) {
                animSpeed = 3.0f;
            }
        }
        mpMorf->setAnm(animation, -1, 3.0f, animSpeed, animStartFrame, -1.0f);
        mpMorf->setFrameF(animStartFrame);
        mCurrentAnim = mNewAnim;
    }
    if (controlledActor == this) {
        if (checkStateFlg0(daCstatue_FLG0_4)) {
            u32 creatureSoundId;
            u32 creatureVoiceId = Z2SE_CSTATUE_ACTIVE_LOOP;
            if (mType != daCstatueType_Small) {
                mAnim1.play();
                if (mType == daCstatueType_Normal) {
                    creatureSoundId = Z2SE_CSTATUE_L_START;
                } else {
                    creatureSoundId = Z2SE_CSTATUE_GM_START;
                    creatureVoiceId = Z2SE_CSTATUE_GM_ACTIVE_LP;
                }
            } else {
                creatureSoundId = Z2SE_CSTATUE_S_START;
            }
            if (mAnim2.checkFrame(mTargetFrame)) {
                mSound.startCreatureSound(creatureSoundId, 0, mReverb);
            } else if (mAnim2.getFrame() >= mTargetFrame) {
                mSound.startCreatureVoiceLevel(creatureVoiceId, mReverb);
            }
        } else {
            initStartBrkBtk();
        }
    } else if (checkStateFlg0(daCstatue_FLG0_4)) {
        offStateFlg0(daCstatue_FLG0_4);
        initStopBrkBtk();
        u32 soundId;
        if (checkBossType()) {
            soundId = Z2SE_CSTATUE_GM_STOP;
        } else if (mType == daCstatueType_Small) {
            soundId = Z2SE_CSTATUE_S_STOP;
        } else {
            soundId = Z2SE_CSTATUE_L_STOP;
        }
        mSound.startCreatureSound(soundId, 0, mReverb);
    }
    if (mType == daCstatueType_Normal2) {
        mAnim1.play();
    }
    mAnim2.play();
}

int daCstatue_c::initBrk(u16 i_index) {
    J3DAnmTevRegKey* key = (J3DAnmTevRegKey*)::dComIfG_getObjectRes(mResName, i_index);
    J3DModelData* ctx = mModel->getModelData();
    return mAnim2.init(ctx, key, 1, -1, 1.0f, 0, -1);
}

void daCstatue_c::initStopBrkBtk() {
    static const u16 brkIdx[daCstatueType_N] = {0x1F, 0x1C, 0x1D, 0x12, 0x12};

    mAnim1.setFrame(0.0f);
    initBrk(brkIdx[mType]);
}

void daCstatue_c::initStartBrkBtk() {
    static const u16 brkIdx[daCstatueType_N] = {0x1E, 0x1B, 0x1D, 0x11, 0x11};

    int soundId = checkBossType() ? Z2SE_CSTATUE_GM_HIT_BALL : Z2SE_CSTATUE_HIT_BALL;
    mSound.startCreatureSound(soundId, 0, mReverb);
    onStateFlg0((daCstatue_FLG0)(daCstatue_FLG0_800 | daCstatue_FLG0_4));
    mAnim1.setFrame(0.0f);
    mAnim1.reset();
    initBrk(brkIdx[mType]);
    csXyz angle(shape_angle);
    for (int iParticle = 0; iParticle < 2; iParticle++) {
        dComIfGp_particle_set(0x88bb, &mBallPos, &angle, NULL);
        dComIfGp_particle_set(0x88bc, &mBallPos, &angle, NULL);
        angle.y -= (s16)0x8000;
    }
}

int daCstatue_c::execute() {
    daAlink_c* link = daAlink_getAlinkActorClass();
    mBossAtGndHit = 0;
    // todo: there is any extra block here in the debug rom setting gravity and mMaxFallSpeed
    if (checkStateFlg0(daCstatue_FLG0_2) && !fopAcM_checkCarryNow(this)) {
        if (speedF > 1.0f) {
            speedF = 30.0f;
            speed.y = 50.0f;
        } else {
            mMoveMode = daCstatue_MoveMode_3;
            if (mStatueAcch.ChkGroundHit() || mStatueAcch.GetGroundH() - current.pos.y > -30.0f) {
                mSound.startCreatureSound(Z2SE_CSTATUE_S_WALK_A, 0, mReverb);
            }
        }
    }
    if (mWarpMode) {
        if (checkStateFlg0(daCstatue_FLG0_1)) {
            fopAcM_delete(this);
            if (checkStateFlg0(daCstatue_FLG0_1000)) {
                offStateFlg0(daCstatue_FLG0_1000);
                dComIfGs_offTbox(mParam2);
                dComIfGs_onTbox(mParam2 + 1);
            }
            return 1;
        }
        if (mWarpMode == (daCstatueWarpMode_Active | daCstatueWarpMode_Ground)) {
            daCstatue_c* controlledActor =
                static_cast<daCstatue_c*>(link->getCopyRodControllActor());
            if (controlledActor == this && link->getCopyRodActor()) {
                static_cast<daCrod_c*>(link->getCopyRodActor())->offControll();
            }
            cLib_addCalcPosXZ(&current.pos, mWarpTarget, 0.5f, 5.0f, 1.0f);
        } else if (mWarpMode == daCstatueWarpMode_Active) {
            current.pos = mWarpTarget;
            old.pos = mWarpTarget;
            mWarpMode = 0;
        }
    }
    setAnime();
    int bossType = checkBossType();
    if (bossType != 0) {
        mStatueAcch.CrrPos(dComIfG_Bgsp());
    } else {
        if (mType == daCstatueType_Normal2) {
            if (mParam0 != 0xff && fopAcM_isSwitch(this, mParam0)) {
                fopAcM_delete(this);
                return 1;
            }
            if (dComIfGs_wolfeye_effect_check() && fopAcM_isSwitch(this, 0x1f) &&
                !fopAcM_CheckCondition(this, 4))
            {
                if (!fopAcM_lc_c::lineCheck(&eyePos, &link->eyePos, this)) {
                    /* dSv_event_flag_c::F_0684 - Temple of Time - Look at R00 statue using sense */
                    dComIfGs_onEventBit(0x5440);
                }
            }
        } else {
            if (mType == daCstatueType_Normal && fopAcM_isSwitch(this, 6)) {
                mCyl1.SetTgType(0xd87afddf);
                if (checkStateFlg0(daCstatue_FLG0_1000)) {
                    offStateFlg0(daCstatue_FLG0_1000);
                    dComIfGs_offTbox(mParam2);
                }
            }
            posMove();
        }
    }
    u32 morf =
        mStatueAcch.GetGroundH() != -G_CM3D_F_INF ? dKy_pol_sound_get(&mStatueAcch.m_gnd) : 0;
    mpMorf->play(morf, mReverb);
    if (!bossType && mCurrentAnim == CStatueAnimIndex_2 && link->checkCopyRodSwingMode()) {
        if (mpMorf->getEndFrame() > link->getBaseAnimeFrame()) {
            mpMorf->setFrame(link->getBaseAnimeFrame());
        } else {
            mpMorf->setFrame(mpMorf->getEndFrame());
        }
    } else if (mTimer1) {
        mTimer1--;
        mpMorf->setFrame(0.0f);
    }
    setRoomInfo();
    setMatrix();
    if (mCyl1.ChkTgHit()) {
        dCcD_GObjInf* hitObject = mCyl1.GetTgHitGObj();
        if (hitObject && hitObject->GetAtType() != AT_TYPE_1000000) {
            mSound.startCollisionSE(mCyl1.GetTgHitObjHitSeID(1), 0x2a);
        }
    }
    setCollision();
    if (mType == daCstatueType_Small) {
        if (mStatueAcch.ChkGroundHit() && !fopAcM_checkCarryNow(this)) {
            cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
        } else {
            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
        }
    }

    u32 effect = 0;
    if (mStatueAcch.ChkGroundLanding() ||
        (mType == daCstatueType_Small && mCurrentAnim == CStatueAnimIndex_2 &&
         mpMorf->checkFrame(7.0f)))
    {
        effect = 7;
    }
    cXyz* effectPos = effect ? &current.pos : NULL;
    mPaPo.setEffectCenter(&tevStr, &current.pos, effect, 0, effectPos, &current.angle, &scale,
                          fopAcM_GetRoomNo(this), 1.0f, speedF);
    if (fopAcM_checkCarryNow(this)) {
        onStateFlg0(daCstatue_FLG0_2);
        fopAcM_OffStatus(this, 0x400);
    } else {
        offStateFlg0(daCstatue_FLG0_2);
        fopAcM_OnStatus(this, 0x400);
    }
    if (mParam2 != 0x3f) {
        dTres_c::setIconPositionOfCstatue(mParam2, &current.pos);
    }
    offStateFlg0((daCstatue_FLG0)(daCstatue_FLG0_Demo | daCstatue_FLG0_100));
    return 1;
}

static int daCstatue_Execute(void* actor) {
    return static_cast<daCstatue_c*>(actor)->execute();
}

int daCstatue_c::draw() {
    int tevstrType = checkBossType() ? 0x10 : 8;
    g_env_light.settingTevStruct(tevstrType, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    mAnim2.entry(mModel->getModelData());
    if (mType != daCstatueType_Small) {
        mAnim1.entry(mModel->getModelData());
    }
    if (mType == daCstatueType_Normal2) {
        if (dComIfGs_wolfeye_effect_check()) {
            MtxP mtx = mModel->getAnmMtx(0);
            cXyz position(mtx[0][3], mtx[1][3], mtx[2][3]);
            cMtx_multVec(dComIfGd_getViewMtx(), &position, &position);
            position.z -= 200.0f;
            cMtx_multVec(dComIfGd_getInvViewMtx(), &position, &position);
            mInvisible.entryDL(&position);
        }
    } else {
        mDoExt_modelEntryDL(mModel);
        if (!fopAcM_checkCarryNow(this)) {
            cXyz shadowPos(current.pos.x, mModel->getAnmMtx(0)[1][3] + 30.0f, current.pos.z);
            mShadowKey =
                dComIfGd_setShadow(mShadowKey, 1, mModel, &shadowPos, mSomePos.x, 0.0f,
                                   current.pos.y, mStatueAcch.GetGroundH(), mStatueAcch.m_gnd,
                                   &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
        }
    }
    return 1;
}

static int daCstatue_Draw(void* actor) {
    return static_cast<daCstatue_c*>(actor)->draw();
}

static actor_method_class l_daCstatue_Method = {
    daCstatue_Create, daCstatue_Delete, daCstatue_Execute, NULL, daCstatue_Draw,
};

extern actor_process_profile_definition g_profile_CSTATUE = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_CSTATUE,            // mProcName
    &g_fpcLf_Method.base,    // sub_method
    0x00000B2C,              // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    271,                     // mPriority
    &l_daCstatue_Method,     // sub_method
    0x00060520,              // mStatus
    fopAc_ENV_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES;

#include "JSystem/JAudio2/JAUSectionHeap.h"
template<>
JAUSectionHeap* JASGlobalInstance<JAUSectionHeap>::sInstance;
