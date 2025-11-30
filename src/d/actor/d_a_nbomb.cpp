/**
 * d_a_nbomb.cpp
 * Bomb Actor
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_nbomb.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "SSystem/SComponent/c_math.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_kankyo_mng.h"
#include "d/actor/d_a_mirror.h"

void daNbomb_c::coHitCallback(fopAc_ac_c* i_hitActor) {
    if (fopAcM_GetGroup(i_hitActor) == fopAc_ENEMY_e ||
        (checkStateFlg0(FLG0_INSECT_BOMB) &&
         (!checkStateFlg0(FLG0_NO_HIT_PLAYER) || i_hitActor != daAlink_getAlinkActorClass())))
    {
        onStateFlg0(FLG0_BOMB_HIT);
    }
}

static void daNbomb_coHitCallback(fopAc_ac_c* i_coActorA, dCcD_GObjInf* i_coObjInfA,
                                  fopAc_ac_c* i_coActorB, dCcD_GObjInf* i_coObjInfB) {
    ((daNbomb_c*)i_coActorA)->coHitCallback(i_coActorB);
}

static u8 const lit_3767[12] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static Vec const l_localCenterOffset = {0.0f, 30.0f, 0.0f};

static char const l_arcNameBombF[] = "Bombf";

static char const l_arcNameBombE[] = "E_BI";

static char const l_arcNameBombEW[] = "E_BG";

void daNbomb_c::tgHitCallback(dCcD_GObjInf* i_hitObj) {
    if (i_hitObj->ChkAtType(AT_TYPE_BOOMERANG)) {
        if (!fopAcM_checkCarryNow(this) && !fopAcM_checkHookCarryNow(this) &&
            !checkStateFlg0(FLG0_INSECT_BOMB))
        {
            procBoomerangMoveInit(i_hitObj);
        }
    } else if (i_hitObj->ChkAtType(AT_TYPE_HOOKSHOT) && fopAcM_CheckStatus(this, 0x80000)) {
        if (fopAcM_checkCarryNow(this)) {
            fopAcM_cancelCarryNow(this);
        }

        fopAcM_setHookCarryNow(this);
        procCarryInit();
        onStateFlg0(FLG0_SET_HOOKSHOT_OFFSET);
    } else if (i_hitObj->ChkAtType(0x200)) {
        if (i_hitObj->GetAtMtrl() == dCcD_MTRL_WIND && !checkStateCarry() && !checkStateExplode() &&
            fopAcM_GetParam(this) != PRM_FLOWER_BOMB)
        {
            cLib_addCalcPos(&field_0xc20, *i_hitObj->GetAtVecP() * 2.0f, 0.5f, 10.0f, 1.0f);
            onStateFlg0(FLG0_UNK_8000);
        }
    } else {
        onStateFlg0(FLG0_BOMB_HIT);
    }

    if (i_hitObj->GetAtMtrl() == dCcD_MTRL_ICE) {
        setFreeze();
    }
}

static void daNbomb_tgHitCallback(fopAc_ac_c* i_tgActor, dCcD_GObjInf* i_tgObjInf,
                                  fopAc_ac_c* i_atActor, dCcD_GObjInf* i_atObjInf) {
    ((daNbomb_c*)i_tgActor)->tgHitCallback(i_atObjInf);
}

int daNbomb_c::searchEnemy(fopAc_ac_c* i_enemy) {
    mDoMtx_multVec(field_0xa70, &i_enemy->current.pos, &field_0xc14);

    if (i_enemy->attention_info.flags & (fopAc_AttnFlag_BATTLE_e | fopAc_AttnFlag_LOCK_e) && field_0xc14.abs2XZ() < 250000.0f &&
        fabsf(field_0xc14.y) < 100.0f)
    {
        if (abs(field_0xc14.atan2sX_Z()) < 0x4000) {
            field_0xc14.y = 0.0f;
            field_0xc14.normalizeZP();
            mDoMtx_multVecSR(mpModel->getBaseTRMtx(), &field_0xc14, &field_0xc14);
            return 1;
        }
    }

    return 0;
}

static void* daNbomb_searchEnemy(fopAc_ac_c* i_actor, void* i_data) {
    if (fopAcM_GetGroup(i_actor) == fopAc_ENEMY_e &&
        ((daNbomb_c*)i_data)->searchEnemy(i_actor) != NULL)
    {
        return i_actor;
    }

    return NULL;
}

static dCcD_SrcSph l_sphSrc = {
    {
        {0x0, {{AT_TYPE_BOMB, 0x4, 0x1e}, {0xd8fbffef, 0x11}, 0x79}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x1, 0x0, 0x0},                            // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4},                            // mGObjTg
        {0x0},                                                         // mGObjCo
    },                                                                 // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 30.0f}  // mSph
    }  // mSphAttr
};

const char* daNbomb_c::m_arcNameList[6] = {
    daAlink_c::getAlinkArcName(), daAlink_c::getAlinkArcName(), daAlink_c::getAlinkArcName(),
    l_arcNameBombF, l_arcNameBombE, l_arcNameBombEW,
};

int daNbomb_c::createHeap() {
    static u16 const bmdIdx[6] = {0x001E, 0x0027, 0x0026, 0x0003, 0x000E, 0x000A};

    J3DModelData* modelData =
        (J3DModelData*)dComIfG_getObjectRes(m_arcNameList[mType], bmdIdx[mType]);
    JUT_ASSERT(0, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    J3DAnmTransform* trans = NULL;
    if (mType == TYPE_INSECT_ENEMY) {
        trans = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcNameBombE, 6);
        JUT_ASSERT(0, trans);
    } else if (mType == TYPE_WATER_ENEMY) {
        trans = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcNameBombEW, 5);
        JUT_ASSERT(0, trans);
    } else if (mType == TYPE_INSECT_PLAYER) {
        trans = (J3DAnmTransform*)dComIfG_getObjectRes(daAlink_c::getAlinkArcName(), 0x15);
        JUT_ASSERT(0, trans);
    }

    if (trans != NULL) {
        mpBck = new mDoExt_bckAnm();
        if (mpBck == NULL) {
            return 0;
        }

        if (!mpBck->init(trans, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false)) {
            return 0;
        }
    }

    mSound.init(&current.pos, 2);
    return 1;
}

static int daNbomb_createHeap(fopAc_ac_c* i_this) {
    return ((daNbomb_c*)i_this)->createHeap();
}

int daNbomb_c::create() {
    fopAcM_GetID(this);
    fopAcM_ct(this, daNbomb_c);

    BOOL is_octaeel_bomb = false;

    if (fopAcM_GetParam(this) == PRM_FLOWER_BOMB) {
        mType = TYPE_FLOWER;
    } else if (fopAcM_GetParam(this) == PRM_ENEMY_BOMB ||
               fopAcM_GetParam(this) == PRM_ENEMY_BOMB_HOOKSHOT ||
               fopAcM_GetParam(this) == PRM_ENEMY_BOMB_BOOMERANG)
    {
        mType = TYPE_INSECT_ENEMY;
    } else if (fopAcM_GetParam(this) == PRM_ENEMY_WATER_BOMB ||
               fopAcM_GetParam(this) == PRM_ENEMY_WATER_BOMB_HOOKSHOT ||
               fopAcM_GetParam(this) == PRM_UNK_15)
    {
        if (fopAcM_GetParam(this) == PRM_ENEMY_WATER_BOMB) {
            fopAcM_SetParam(this, PRM_ENEMY_BOMB);
        } else if (fopAcM_GetParam(this) == PRM_ENEMY_WATER_BOMB_HOOKSHOT) {
            fopAcM_SetParam(this, PRM_ENEMY_BOMB_HOOKSHOT);
            if (strcmp(dComIfGp_getStartStageName(), "D_MN01A") == 0) {
                is_octaeel_bomb = true;
            }
        } else {
            fopAcM_SetParam(this, PRM_ENEMY_BOMB_BOOMERANG);
        }

        mType = TYPE_WATER_ENEMY;
        onStateFlg0(FLG0_WATER_BOMB);
    } else if (fopAcM_GetParam(this) == PRM_INSECT_BOMB_PLAYER) {
        mType = TYPE_INSECT_PLAYER;
    } else if (fopAcM_GetParam(this) == PRM_WATER_BOMB_PLAYER) {
        mType = TYPE_WATER_PLAYER;
    } else {
        mType = TYPE_NORMAL_PLAYER;
    }

    if (mType >= TYPE_FLOWER && dComIfG_resLoad(&mPhase, m_arcNameList[mType]) != cPhs_COMPLEATE_e)
    {
        return cPhs_ERROR_e;
    }

    static u32 const heapSize[] = {0xEB0, 0xEB0, 0x17C0, 0xEB0, 0x2500, 0xF90};

    if (!fopAcM_entrySolidHeap(this, daNbomb_createHeap, heapSize[mType])) {
        return cPhs_ERROR_e;
    }

    mAcchCir.SetWall(30.0f, 30.0f);
    mAcch.Set(this, 1, &mAcchCir);
    mAcch.ClrWaterNone();
    mAcch.SetRoofCrrHeight(60.0f);

    if (fopAcM_GetParam(this) != PRM_INSECT_BOMB_PLAYER) {
        mAcch.ClrRoofNone();
    }

    mAcch.SetWaterCheckOffset(10000.0f);
    mAcch.SetWtrChkMode(2);
    mAcch.OnLineCheck();

    mDoMtx_copy(cMtx_getIdentity(), field_0xa40);
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());

    mCcStts.Init(30, 0xFF, this);

    mCcSph.Set(l_sphSrc);
    mCcSph.SetStts(&mCcStts);
    mCcSph.SetTgHitCallback(daNbomb_tgHitCallback);

    mSph2.Set(l_sphSrc);
    mSph2.SetStts(&mCcStts);
    mSph2.SetTgMtrl(2);
    mSph2.OffCoSetBit();
    mSph2.SetR(110.0f);
    mSph2.SetTgHitCallback(daNbomb_tgHitCallback);

    daAlink_c* player = daAlink_getAlinkActorClass();
    gravity = player->getBombGravity();
    maxFallSpeed = player->getBombMaxFallSpeed();
    mExTime = player->getBombExplodeTime();

    if (is_octaeel_bomb) {
        mExTime *= 0.75f;
    }

    cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    fopAcM_OnCarryType(this, fopAcM_CARRY(fopAcM_CARRY_LIGHT | fopAcM_CARRY_SIDE));
    field_0xb5c = shape_angle.y;

    if (fopAcM_GetParam(this) == PRM_NORMAL_BOMB_EXPLODE ||
        fopAcM_GetParam(this) == PRM_WATER_BOMB_EXPLODE)
    {
        mAcch.CrrPos(dComIfG_Bgsp());
        procExplodeInit();
        onStateFlg0(FLG0_UNK_2000);
    } else {
        mAcch.CrrPos(dComIfG_Bgsp());
        setRoomInfo();

        if (fopAcM_GetParam(this) == PRM_NORMAL_BOMB_PLAYER ||
            fopAcM_GetParam(this) == PRM_WATER_BOMB_PLAYER)
        {
            onStateFlg0(FLG0_PLAYER_MAKE);

            if (fopAcM_GetParam(this) == PRM_WATER_BOMB_PLAYER) {
                onStateFlg0(FLG0_WATER_BOMB);
            }

            if (checkWaterIn()) {
                onStateFlg0(FLG0_UNDERWATER);
                gravity = player->getBombWaterGravity();
                maxFallSpeed = player->getBombWaterMaxFallSpeed();
            }

            procCarryInit();
        } else if (fopAcM_GetParam(this) == PRM_ENEMY_BOMB) {
            if (player->setEnemyBomb(this)) {
                procCarryInit();
            } else {
                procWaitInit();
            }
        } else if (fopAcM_GetParam(this) == PRM_ENEMY_BOMB_HOOKSHOT) {
            if (player->setEnemyBombHookshot(this)) {
                setHookshotOffset();
                procCarryInit();
            } else {
                procWaitInit();
            }
        } else if (fopAcM_GetParam(this) == PRM_ENEMY_BOMB_BOOMERANG) {
            procBoomerangMoveInit(NULL);
        } else if (fopAcM_GetParam(this) == PRM_FLOWER_BOMB) {
            procFlowerWaitInit();
        } else if (fopAcM_GetParam(this) == PRM_BOMB_CARGO_CARRY) {
            onStateFlg0(FLG0_CARGO_CARRY);
            procCarryInit();
        } else if (fopAcM_GetParam(this) == PRM_INSECT_BOMB_PLAYER) {
            onStateFlg0(daNbomb_FLG0(FLG0_INSECT_BOMB | FLG0_NO_HIT_PLAYER | FLG0_PLAYER_MAKE));
            mExTime = player->getBombExplodeTime() * 2;
            shape_angle.y = player->shape_angle.y;
            current.angle.y = player->shape_angle.y;

            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(shape_angle.y);
            mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
            mDoMtx_inverse(mpModel->getBaseTRMtx(), field_0xa70);

            scale.set(0.6f, 0.6f, 0.6f);
            mpModel->setBaseScale(scale);
            field_0xbf0 = cXyz::BaseY;

            field_0xbfc.set(cM_ssin(shape_angle.y), 0.0f, cM_scos(shape_angle.y));
            field_0xc08.set(field_0xbfc.z, 0.0f, -field_0xbfc.x);

            cXyz start = current.pos + (field_0xbf0 * 5.0f);
            cXyz end = current.pos - (field_0xbf0 * 100.0f);
            mLineChk.Set(&start, &end, this);

            if (insectLineCheck() == 1) {
                procInsectMoveInit();
            } else {
                start = current.pos - (field_0xbf0 * 5.0f);
                end = start - (field_0xbfc * 100.0f);
                mLineChk.Set(&start, &end, this);

                if (insectLineCheck() == 1) {
                    procInsectMoveInit();
                } else {
                    procExplodeInit();
                    onStateFlg0(FLG0_UNK_2000);
                }
            }
        } else {
            procWaitInit();
        }
    }

    fopAcM_SetMin(this, -36.0f, 0.0f, -36.0f);
    fopAcM_SetMax(this, 36.0f, 66.0f, 36.0f);
    fopAcM_setCullSizeFar(this, 10.0f);

    if (!checkStateFlg0(FLG0_INSECT_BOMB)) {
        mDoMtx_stack_c::transS(current.pos);

        if (!checkStateFlg0(FLG0_INSECT_BOMB)) {
            fopAcM_OnStatus(this, 0x80000);
        }

        mDoMtx_stack_c::ZXYrotM(0, shape_angle.y, shape_angle.z);
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
        mDoMtx_inverse(mpModel->getBaseTRMtx(), field_0xa70);
    }

    if (mpBck != NULL) {
        mpBck->entry(mpModel->getModelData());
    }

    mpModel->calc();
    setSmokePos();
    model = mpModel;
    return cPhs_COMPLEATE_e;
}

static int daNbomb_Create(fopAc_ac_c* i_this) {
    return ((daNbomb_c*)i_this)->create();
}

daNbomb_c::~daNbomb_c() {
    if (mExplodeMode < 2) {
        dKy_actor_addcol_set(0, 0, 0, 0.0f);
    }

    dKy_plight_cut(&mLightInfluence);
    dKyw_pntwind_cut(&mWindInfluence);
    mSound.deleteObject();

    if (checkStateFlg0(FLG0_UNK_40)) {
        for (int i = 0; i < 5; i++) {
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mEffectEmitterIDs[i]);
            if (emitter != NULL) {
                emitter->stopDrawParticle();
            }
        }
    }

    if (mType >= TYPE_FLOWER) {
        dComIfG_resDelete(&mPhase, m_arcNameList[mType]);
    }

    daAlink_c* player = daAlink_getAlinkActorClass();
    if (player != NULL) {
        if (checkStateFlg0(FLG0_INSECT_BOMB)) {
            player->decrementInsectBombCnt();
        } else if (checkStateFlg0(FLG0_PLAYER_MAKE)) {
            player->decrementBombCnt();
        }
    }

    fopAcM_cancelCarryNow(this);
    fopAcM_cancelHookCarryNow(this);

    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mEffectEmitterIDs[0]);
    if (emitter != NULL) {
        emitter->setParticleCallBackPtr(NULL);
    }
}

static int daNbomb_Delete(daNbomb_c* i_this) {
    i_this->~daNbomb_c();
    return 1;
}

bool daNbomb_c::checkTimerStop() {
    return fopAcM_GetParam(this) == PRM_FLOWER_BOMB ||
           checkStateFlg0(daNbomb_FLG0(FLG0_UNK_800 | FLG0_FROZEN)) ||
           (mType != TYPE_INSECT_ENEMY && mType != TYPE_WATER_ENEMY &&
            fopAcM_GetParam(this) == PRM_ENEMY_BOMB_BOOMERANG_MOVE);
}

BOOL daNbomb_c::checkExplode() {
    if (checkStateFlg0(daNbomb_FLG0(FLG0_UNK_800 | FLG0_FROZEN))) {
        return false;
    }

    if (mExTime > 0) {
        if (!dComIfGp_event_runCheck() && !checkTimerStop()) {
            mExTime--;

            if (checkStateFlg0(FLG0_CARGO_CARRY)) {
                daAlink_c* player = daAlink_getAlinkActorClass();
                if (mExTime < player->getBombExplodeTime()) {
                    mExTime = player->getBombExplodeTime() * 1.5f;
                }
            }
        }

        if ((mExTime == 0 || checkStateFlg0(FLG0_BOMB_HIT)) ||
            ((fopAcM_GetParam(this) == PRM_BOMB_BOOMERANG_MOVE ||
              fopAcM_GetParam(this) == PRM_ENEMY_BOMB_BOOMERANG_MOVE) &&
             mAcch.ChkWallHit() && !daAlink_c::checkIcePolygonDamage(&mAcchCir)))
        {
            return procExplodeInit();
        }
    }

    offStateFlg0(FLG0_BOMB_HIT);
    return false;
}

void daNbomb_c::setRoomInfo() {
    int room_no;
    if (mAcch.GetGroundH() != -G_CM3D_F_INF) {
        room_no = dComIfG_Bgsp().GetRoomId(mAcch.m_gnd);
        tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mAcch.m_gnd);
        mPolySound = dKy_pol_sound_get(&mAcch.m_gnd);
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
    }

    tevStr.room_no = room_no;
    mSoundReverb = dComIfGp_getReverb(room_no);
    mCcStts.SetRoomId(room_no);
    fopAcM_SetRoomNo(this, room_no);
}

void daNbomb_c::setSmokePos() {
    if (mType == TYPE_INSECT_ENEMY || mType == TYPE_WATER_ENEMY) {
        mDoMtx_multVecZero(mpModel->getBaseTRMtx(), &mEffectPosition);
    } else {
        static Vec const localOffset = {0.0f, 60.0f, 0.0f};
        mDoMtx_multVec(mpModel->getBaseTRMtx(), &localOffset, &mEffectPosition);
    }

    field_0xbe4 = (mEffectPosition - mEffectLastPosition) * 0.5f;
}

void daNbomb_c::setEffect() {
    static cXyz effectScale(1.8f, 1.8f, 1.8f);

    if (mpBck != NULL || checkStateFlg0(FLG0_WATER_BOMB)) {
        static u16 enemyBombID[] = {0xA0D, 0xA0E, 0xA0F, 0xA10, 0xA11};
        u32* emitter_id = &mEffectEmitterIDs[0];
        u16* effect_id = &enemyBombID[0];

        mDoMtx_stack_c::copy(mpModel->getAnmMtx(0));

        if (mType == TYPE_WATER_ENEMY || checkStateFlg0(FLG0_WATER_BOMB)) {
            mDoMtx_stack_c::ZXYrotM(0, 0x4000, 0x4000);
        }

        for (int i = 0; i < 5; i++, emitter_id++, effect_id++) {
            *emitter_id = dComIfGp_particle_set(*emitter_id, *effect_id, &current.pos, &tevStr);

            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(*emitter_id);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(mDoMtx_stack_c::get());
            }
        }
    } else {
        mEffectEmitterIDs[0] =
            dComIfGp_particle_set(mEffectEmitterIDs[0], 0x1DF, &mEffectPosition, &tevStr, NULL,
                                  &effectScale, 0xFF, NULL, -1, NULL, NULL, NULL);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mEffectEmitterIDs[0]);
        if (emitter != NULL) {
            emitter->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
            emitter->setUserWork((uintptr_t)&field_0xbe4);
        }

        mEffectEmitterIDs[1] =
            dComIfGp_particle_set(mEffectEmitterIDs[1], 0x1DE, &mEffectPosition, &tevStr, NULL,
                                  &effectScale, 0xFF, NULL, -1, NULL, NULL, NULL);
    }
}

void daNbomb_c::setHookshotOffset() {
    daAlink_c* player = daAlink_getAlinkActorClass();

    cXyz offset = player->current.pos - current.pos;
    offset.y = 0.0f;
    offset.normalizeZP();
    offset *= 30.0f;
    offset.y += 30.0f;

    player->setHookshotCarryOffset(fopAcM_GetID(this), &offset);
    offStateFlg0(FLG0_SET_HOOKSHOT_OFFSET);
}

void daNbomb_c::setFreeze() {
    if (!checkStateFlg0(FLG0_FROZEN)) {
        onStateFlg0(FLG0_FROZEN);
        mFreezeTimer = 90;
    }
}

BOOL daNbomb_c::checkWaterIn() {
    return mAcch.ChkWaterHit() && current.pos.y + 30.0f < mAcch.m_wtr.GetHeight();
}

int daNbomb_c::insectLineCheck() {
    if (dComIfG_Bgsp().LineCross(&mLineChk)) {
        cM3dGPla chk_poly;
        dComIfG_Bgsp().GetTriPla(mLineChk, &chk_poly);

        daAlink_c* player = daAlink_getAlinkActorClass();

        if (chk_poly.mNormal.y < cM_scos(cM_deg2s(player->getBombInsectLimitAngle())) ||
            dComIfG_Bgsp().GetGroundCode(mLineChk) == 1)
        {
            return 2;
        }

        return 1;
    }

    return 0;
}

void daNbomb_c::setHitPolygon(BOOL param_0) {
    if (!param_0) {
        mInsectHitPolyInfo.SetPolyInfo(mLineChk);
        current.pos = mLineChk.GetCross();
    }

    cM3dGPla poly;
    dComIfG_Bgsp().GetTriPla(mInsectHitPolyInfo, &poly);

    f32 var_f31 = field_0xbf0.inprod(poly.mNormal);
    if (!(fabsf(var_f31) > 0.999f)) {
        cXyz sp20 = field_0xbf0.outprod(poly.mNormal);
        sp20.normalizeZP();

        mDoMtx_stack_c::rotAxisRadS(&sp20,
                                    cM_atan2f(JMAFastSqrt(1.0f - (var_f31 * var_f31)), var_f31));
        field_0xbf0 = poly.mNormal;

        mDoMtx_stack_c::multVecSR(&field_0xc08, &field_0xc08);
        mDoMtx_stack_c::multVecSR(&field_0xbfc, &field_0xbfc);

        csXyz sp58(current.angle);
        mDoMtx_stack_c::ZXYrotM(current.angle);
        mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &current.angle);

        if (param_0) {
            shape_angle.x += current.angle.x - sp58.x;
            shape_angle.y += current.angle.y - sp58.y;
            shape_angle.z += current.angle.z - sp58.z;
        }
    }

    if (!param_0) {
        cLib_addCalcAngleS(&shape_angle.x, current.angle.x, 3, 0x2000, 0x400);
        cLib_addCalcAngleS(&shape_angle.y, current.angle.y, 3, 0x2000, 0x400);
        cLib_addCalcAngleS(&shape_angle.z, current.angle.z, 3, 0x2000, 0x400);
    }

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_inverse(mpModel->getBaseTRMtx(), field_0xa70);
}

BOOL daNbomb_c::procExplodeInit() {
    fopAcM_OnStatus(this, 0x20000);
    daAlink_c* player = daAlink_getAlinkActorClass();

    scale.x = player->getBombEffScale();
    scale.y = scale.x;
    scale.z = scale.x;

    shape_angle.y = cM_rndF(0x10000);
    field_0xbe4 = cXyz::Zero;

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);

    static const u16 normalNameID[] = {0x161, 0x162, 0x163, 0x164, 0x165,
                                       0x166, 0x167, 0x168, 0x1EC};
    static const u16 waterNameID[] = {0xA05, 0xA06, 0xA07, 0xA08, 0xA09, 0xA0A, 0xA0B, 0xA0C};
    static const u16 waterNameID2[] = {0x9FC, 0x9FD, 0x9FE, 0x9FF, 0xA00,
                                       0xA01, 0xA02, 0xA03, 0xA04};

    const u16* eff_list;
    int eff_num;
    u32 sound;

    if (checkStateFlg0(FLG0_UNDERWATER) || fopAcM_GetParam(this) == PRM_WATER_BOMB_EXPLODE) {
        if (mAcch.ChkWaterHit() &&
            mAcch.m_wtr.GetHeight() - current.pos.y < player->getBombExplodeWaterEffectLimit())
        {
            eff_list = waterNameID2;
            eff_num = 9;
            sound = Z2SE_OBJ_BOMB_EXP_WTRSURF;
        } else {
            eff_list = waterNameID;
            eff_num = 8;
            sound = Z2SE_OBJ_WATERBOMB_EXPLODE;
        }
    } else {
        eff_list = normalNameID;
        eff_num = 9;
        sound = Z2SE_OBJ_BOMB_EXPLODE;
    }

    for (int i = 0; i < eff_num; i++, eff_list++) {
        dComIfGp_particle_setColor(*eff_list, &current.pos, &tevStr, NULL, NULL, 0.0f, 0xFF,
                                   &shape_angle, &scale, NULL, -1, NULL);
    }

    mLightInfluence.mPosition = current.pos;
    mLightInfluence.mPosition.y += 100.0f;
    mLightInfluence.mColor.r = 100;
    mLightInfluence.mColor.g = 100;
    mLightInfluence.mColor.b = 80;
    mLightInfluence.mPow = 600.0f;
    mLightInfluence.mFluctuation = 100.0f;
    dKy_plight_set(&mLightInfluence);

    mWindInfluence.position = current.pos;
    mWindInfluence.mDirection.x = 0.0f;
    mWindInfluence.mDirection.y = 1.0f;
    mWindInfluence.mDirection.z = 0.0f;
    mWindInfluence.mRadius = 500.0f;
    mWindInfluence.field_0x20 = 0.0f;
    mWindInfluence.mStrength = 0.5f;
    dKyw_pntwind_set(&mWindInfluence);

    mExplodeMode = 0;
    mExplosionStrength = 0.0f;

    mProcFunc = &daNbomb_c::procExplode;

    speedF = 0.0f;
    speed = cXyz::Zero;
    gravity = 0.0f;
    fopAcM_SetParam(this, PRM_NORMAL_BOMB_EXPLODE);
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);

    mCcSph.OffTgSetBit();
    mCcSph.OffCoSetBit();
    mCcSph.OnAtSetBit();
    mCcSph.SetR(player->getBombAtR());
    mCcSph.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCcSph);
    dComIfG_Ccsp()->SetMass(&mCcSph, 1);

    mSound.startSound(sound, 0, mSoundReverb);

    fopAcM_cancelCarryNow(this);
    fopAcM_cancelHookCarryNow(this);
    dKy_Sound_set(current.pos, 0xFF, fopAcM_GetID(this), 10);
    mExTime = 0;

    f32 var_f31 = current.pos.abs2(player->current.pos);
    if (var_f31 < 160000.0f) {
        dComIfGp_getVibration().StartShock(4, 31, cXyz(0.0f, 1.0f, 0.0f));
    } else if (var_f31 < 640000.0f) {
        dComIfGp_getVibration().StartShock(2, 31, cXyz(0.0f, 1.0f, 0.0f));
    }

    field_0xb51 = 1;
    return true;
}

BOOL daNbomb_c::procExplode() {
    camera_class* camera = dComIfGp_getCamera(0);
    f32 dist_scale = 0.0f;

    mLightInfluence.mPow = mExplosionStrength * 1500.0f;
    mWindInfluence.mStrength = mExplosionStrength;

    f32 dist_to_cam = current.pos.abs(camera->lookat.eye);
    if (dist_to_cam < 1500.0f) {
        dist_scale = dist_to_cam / 1500.0f;
        dist_scale *= dist_scale * dist_scale;
        dist_scale = 1.0f - dist_scale;
    }

    switch (mExplodeMode) {
    case 0:
        cLib_addCalc(&mExplosionStrength, 1.0f, 0.5f, 0.5f, 0.02f);

        if (mExplosionStrength >= 0.99f) {
            mExplodeMode++;
        }
        break;
    case 1:
        cLib_addCalc(&mExplosionStrength, 0.0f, 0.25f, 0.1f, 0.02f);

        if (mExplosionStrength <= 0.1f) {
            mExplodeMode++;
            mExplosionStrength = 0.0f;
        }
        break;
    }

    dKy_actor_addcol_amb_set(100, 60, 50, mExplosionStrength * dist_scale);
    dKy_bg_addcol_amb_set(100, 60, 50, mExplosionStrength * dist_scale);

    if (mExplodeMode < 2) {
        mExTime--;

        if (mExTime > -3) {
            mCcSph.SetC(current.pos);
            dComIfG_Ccsp()->Set(&mCcSph);
            dComIfG_Ccsp()->SetMass(&mCcSph, 1);
        }
    } else {
        dKy_actor_addcol_set(0, 0, 0, 0);
        fopAcM_delete(this);
    }

    return true;
}

BOOL daNbomb_c::procCarryInit() {
    mProcFunc = &daNbomb_c::procCarry;

    offStateFlg0(FLG0_UNK_10000);
    field_0xc20 = cXyz::Zero;
    field_0xc2c = cXyz::Zero;

    if (fopAcM_checkHookCarryNow(this)) {
        fopAcM_SetParam(this, PRM_BOMB_HOOKSHOT);
    } else {
        fopAcM_SetParam(this, PRM_BOMB_CARRY);
    }

    if (checkStateFlg0(FLG0_INSECT_BOMB)) {
        mpBck->init((J3DAnmTransform*)dComIfG_getObjectRes(daAlink_c::getAlinkArcName(), 0x15),
                    TRUE, -1, 1.0f, 0, -1, true);
        shape_angle.set(0, daAlink_getAlinkActorClass()->shape_angle.y, 0);
        mDoMtx_copy(cMtx_getIdentity(), field_0xa40);
    }

    speedF = 0.0f;
    speed = cXyz::Zero;
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    mCcSph.OffCoSetBit();
    return true;
}

BOOL daNbomb_c::procCarry() {
    daAlink_c* player = daAlink_getAlinkActorClass();

    cLib_chaseF(&field_0xbb0, 0.0f, 1.0f);

    if (checkStateFlg0(FLG0_SET_HOOKSHOT_OFFSET)) {
        setHookshotOffset();
    }

    if (!fopAcM_checkCarryNow(this) && !fopAcM_checkHookCarryNow(this)) {
        if (fopAcM_GetParam(this) == PRM_BOMB_HOOKSHOT && player->setForceGrab(this, 0, 1)) {
            procCarryInit();
            return true;
        }

        if (checkStateFlg0(FLG0_INSECT_BOMB)) {
            if (fopAcM_GetParam(this) != PRM_BOMB_HOOKSHOT) {
                mDoMtx_multVecSR(mpModel->getBaseTRMtx(), &cXyz::BaseY, &field_0xbf0);
                mDoMtx_multVecSR(mpModel->getBaseTRMtx(), &cXyz::BaseZ, &field_0xbfc);
                mDoMtx_multVecSR(mpModel->getBaseTRMtx(), &cXyz::BaseX, &field_0xc08);
                mDoMtx_MtxToRot(mpModel->getBaseTRMtx(), &shape_angle);
                current.angle = shape_angle;

                cXyz start = current.pos + (field_0xbf0 * 5.0f);
                cXyz end = current.pos - (field_0xbf0 * 100.0f);
                mLineChk.Set(&start, &end, this);

                if (insectLineCheck() == 1) {
                    procInsectMoveInit();
                    return procInsectMove();
                }

                start = current.pos - (field_0xbf0 * 5.0f);
                end = start - (field_0xbfc * 100.0f);
                mLineChk.Set(&start, &end, this);

                if (insectLineCheck() == 1) {
                    procInsectMoveInit();
                    return procInsectMove();
                }
            }

            if (checkWaterIn()) {
                onStateFlg0(FLG0_DELETE_BOMB);
                return true;
            }

            return procExplodeInit();
        } else {
            procWaitInit();
            return procWait();
        }
    } else if (checkExplode()) {
        return true;
    }

    cXyz sp40(current.pos);
    mAcch.CrrPos(dComIfG_Bgsp());
    setRoomInfo();

    if (fopAcM_GetParam(this) == 1) {
        if (daAlink_getAlinkActorClass()->getGrabActorID() == fopAcM_GetID(this)) {
            daAlink_getAlinkActorClass()->setGrabCollisionOffset(current.pos.x - sp40.x,
                                                                 current.pos.z - sp40.z, NULL);
        }
    }

    current.pos = sp40;
    speedF = 0.0f;
    speed = cXyz::Zero;
    return true;
}

BOOL daNbomb_c::procWaitInit() {
    if (fopAcM_GetParam(this) == PRM_BOMB_CARRY) {
        onStateFlg0(FLG0_UNK_20000);
    }

    mProcFunc = &daNbomb_c::procWait;
    fopAcM_SetParam(this, PRM_BOMB_WAIT);
    mCcSph.OnCoSetBit();
    return true;
}

BOOL daNbomb_c::procWait() {
    if (checkExplode()) {
        return true;
    }

    if (fopAcM_checkCarryNow(this)) {
        return procCarryInit();
    }

    f32 prev_speedf = speedF;

    if (mAcch.ChkGroundHit() && !mAcch.ChkGroundLanding()) {
        s16 var_r28 = fopAcM_getPolygonAngle(mAcch.m_gnd, current.angle.y);

        speed.y -= speedF * fabsf(cM_ssin(var_r28));
        speedF *= cM_scos(var_r28);
    } else {
        cLib_chaseF(&field_0xbb0, 0.0f, 1.0f);
    }

    f32 prev_gravity = gravity;

    if (field_0xc20.y > 5.0f) {
        gravity = 0.0f;

        if (speed.y < 0.0f) {
            speed.y += field_0xc20.y;

            if (speed.y > 0.0f) {
                field_0xc20.y = speed.y;
                speed.y = 0.0f;
            } else {
                field_0xc20.y = 0.0f;
            }
        }

        if (field_0xc20.abs2XZ() < 0.1f) {
            speedF *= 0.5f;
        }
    }

    if (field_0xc2c.y > 2.0f) {
        gravity = 0.0f;

        if (speed.y < 0.0f) {
            speed.y += field_0xc2c.y;

            if (speed.y > 0.0f) {
                field_0xc2c.y = speed.y;
                speed.y = 0.0f;
            } else {
                field_0xc2c.y = 0.0f;
            }

            onStateFlg0(FLG0_UNK_10000);
        }

        if (field_0xc2c.abs2XZ() < 0.1f) {
            speedF *= 0.5f;
        }
    }

    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());

    if (checkStateFlg0(FLG0_UNK_10000) && checkStateFlg0(FLG0_UNDERWATER) && mAcch.ChkWaterHit() &&
        (current.pos.y + 30.0f) + 1.0f >= mAcch.m_wtr.GetHeight())
    {
        current.pos.y = (mAcch.m_wtr.GetHeight() - 30.0f) - 1.0f;
        speed.y = 0.0f;
    }

    speedF = prev_speedf;
    gravity = prev_gravity;

    f32 prev_speedY = speed.y;
    s16 var_r28 = 0;
    s16 var_r27 = 0;

    current.pos += field_0xc20 + field_0xc2c;
    mAcch.CrrPos(dComIfG_Bgsp());
    setRoomInfo();

    if (checkStateFlg0(FLG0_INSECT_BOMB) &&
        (mAcch.ChkGroundHit() || mAcch.ChkWallHit() || mAcch.ChkRoofHit()))
    {
        return procExplodeInit();
    }

    if (mAcch.ChkGroundHit()) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(mAcch.m_gnd, &plane);

        field_0xb5a = cM_atan2s(plane.mNormal.absXZ(), plane.mNormal.y);
        var_r27 = plane.mNormal.atan2sX_Z();
        var_r28 = var_r27 - current.angle.y;

        if (checkStateFlg0(FLG0_UNK_800) &&
            (!mAcch.ChkWaterHit() || current.pos.y + 300.0f > mAcch.m_wtr.GetHeight()))
        {
            onStateFlg0(FLG0_DELETE_BOMB);
            return true;
        }

        if (daAlink_c::checkSnowCodePolygon(mAcch.m_gnd)) {
            field_0xbb0 = -20.0f;
            speedF *= 0.7f;
            speed.y = 0.0f;
            prev_speedY = 0.0f;
        }
    }

    daAlink_c* player = daAlink_getAlinkActorClass();

    if (mAcch.ChkWallHit()) {
        speedF *= player->getBombBoundRate();
        current.angle.y = (mAcchCir.GetWallAngleY() * 2) - (current.angle.y + 0x8000);
    } else if (mAcch.ChkGroundLanding() && speedF > 5.0f && cM_deg2s(50.0f) <= field_0xb5a) {
        if ((f32)fabs(var_r28) >= (f32)0x4000) {
            speedF *= player->getBombBoundRate();
            current.angle.y = (var_r27 * 2) - (current.angle.y + 0x8000);
            mAcch.ClrGroundLanding();
            mAcch.ClrGroundHit();
            speed.y = prev_speedY;
        }
    }

    if (mAcch.ChkGroundLanding()) {
        u32 spC0 = fabsf(prev_speedY) * 2.0f;

        if (checkStateFlg0(FLG0_UNK_20000) && speedF < 1.0f) {
            spC0 = 50;
        } else if (spC0 > 100) {
            spC0 = 100;
        }

        Z2AudioMgr::getInterface()->seStart(Z2SE_OBJ_BOMB_BOUND, &current.pos, mPolySound,
                                            mSoundReverb, 1.0f, spC0, -1.0f, -1.0f, 0);

        f32 var_f31 = prev_speedY * -player->getBombBoundRate();
        if (var_f31 < player->getBombStopSpeedY()) {
            cLib_chaseF(&speedF, 0.0f, 7.0f);
        } else {
            speedF *= 0.9f;

            s16 gnd_angle = fopAcM_getPolygonAngle(mAcch.m_gnd, current.angle.y);
            if (gnd_angle < 0) {
                speedF *= cM_scos(gnd_angle);
            }

            if (var_f31 > player->getBombMaxSpeedY()) {
                speed.y = player->getBombMaxSpeedY();
            } else {
                speed.y = var_f31;
            }
        }

        fopAcM_seStartCurrent(this, Z2SE_OBJ_BOMB_BOUND, 0);
    } else if (mAcch.ChkGroundHit()) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(mAcch.m_gnd, &plane);

        if (field_0xb5a >= cM_deg2s(20.0f) && !daAlink_c::checkSnowCodePolygon(mAcch.m_gnd)) {
            f32 var_f28 = 2.0f * cM_ssin(field_0xb5a);
            f32 var_f30 = (plane.mNormal.x * var_f28) + (speedF * cM_ssin(current.angle.y));
            f32 var_f29 = (plane.mNormal.z * var_f28) + (speedF * cM_scos(current.angle.y));

            current.angle.y = cM_atan2s(var_f30, var_f29);
            speedF = JMAFastSqrt(var_f30 * var_f30 + var_f29 * var_f29);
            if (speedF > 20.0f) {
                speedF = 20.0f;
            }
        } else {
            if (cLib_chaseF(&speedF, 0.0f, 2.0f)) {
                if (field_0xc20.abs2() < 1.0f && field_0xc2c.abs2() < 1.0f &&
                    !checkStateFlg0(FLG0_FROZEN))
                {
                    cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
                } else {
                    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
                }
            }
        }
    } else {
        cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    }

    offStateFlg0(FLG0_UNK_20000);
    return true;
}

BOOL daNbomb_c::procFlowerWaitInit() {
    mProcFunc = &daNbomb_c::procFlowerWait;

    mCcSph.OnCoSetBit();
    mCcSph.OffTgSetBit();
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    scale.set(0.1f, 0.1f, 0.1f);
    mCcStts.SetWeight(0xFE);
    mpModel->setBaseScale(scale);
    return true;
}

BOOL daNbomb_c::procFlowerWait() {
    if (scale.x < 1.0f) {
        cLib_chaseF(&scale.x, 1.0f, 0.1f);
        scale.y = scale.x;
        scale.z = scale.x;
        mpModel->setBaseScale(scale);
    } else {
        cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
        mCcSph.OnTgSetBit();

        if (checkExplode()) {
            return true;
        }

        if (fopAcM_checkCarryNow(this)) {
            mCcStts.SetWeight(30);
            return procCarryInit();
        }
    }

    return true;
}

BOOL daNbomb_c::procBoomerangMoveInit(dCcD_GObjInf* unused) {
    if (mProcFunc == &daNbomb_c::procBoomerangMove) {
        return false;
    }

    mProcFunc = &daNbomb_c::procBoomerangMove;

    if (fopAcM_GetParam(this) == PRM_FLOWER_BOMB ||
        fopAcM_GetParam(this) == PRM_ENEMY_BOMB_BOOMERANG)
    {
        fopAcM_SetParam(this, PRM_ENEMY_BOMB_BOOMERANG_MOVE);
    } else {
        fopAcM_SetParam(this, PRM_BOMB_BOOMERANG_MOVE);
    }

    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    mCcStts.SetWeight(30);
    mCcSph.OnCoSetBit();
    mCcSph.SetCoHitCallback(daNbomb_coHitCallback);
    speedF = 0.0f;

    mBoomerangMove.initOffset(&current.pos);
    mExTime = daAlink_getAlinkActorClass()->getBombExplodeTime() * 1.5f;
    return true;
}

BOOL daNbomb_c::procBoomerangMove() {
    if (checkExplode()) {
        return true;
    }

    daAlink_c* player = daAlink_getAlinkActorClass();

    fopAc_ac_c* var_r29;
    if (checkStateFlg0(FLG0_FROZEN)) {
        var_r29 = NULL;
    } else {
        var_r29 = this;
    }

    if (checkStateFlg0(FLG0_FROZEN) && mAcch.ChkWallHit()) {
        speed.y = 0.0f;
        mCcSph.SetCoHitCallback(NULL);
        return procWaitInit();
    }

    if (!mBoomerangMove.posMove(&current.pos, &shape_angle.y, var_r29, 0x1C00)) {
        mExTime = player->getBombExplodeTime();
        mCcSph.SetCoHitCallback(NULL);

        if (fopAcM_checkCarryNow(this)) {
            return procCarryInit();
        }

        speed.y = 0.0f;
        procWaitInit();
    } else {
        if (mExTime < player->getBombExplodeTime()) {
            mExTime = player->getBombExplodeTime() * 1.5f;
        }
    }

    current.angle.y = shape_angle.y;

    if (mCcStts.GetCCMoveP() != NULL) {
        current.pos += *mCcStts.GetCCMoveP();
    }

    mAcch.CrrPos(dComIfG_Bgsp());
    setRoomInfo();
    mBoomerangMove.bgCheckAfterOffset(&current.pos);
    return true;
}

BOOL daNbomb_c::procInsectMoveInit() {
    mProcFunc = &daNbomb_c::procInsectMove;

    fopAcM_SetParam(this, PRM_INSECT_BOMB_MOVE);
    mpBck->init((J3DAnmTransform*)dComIfG_getObjectRes(daAlink_c::getAlinkArcName(), 0x14), TRUE,
                -1, 3.0f, 0, -1, true);

    setHitPolygon(0);
    mCcSph.OnCoSetBit();
    mCcSph.SetCoHitCallback(daNbomb_coHitCallback);
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);

    speedF = 20.0f;
    old.pos = current.pos;
    mNoHitPlayerTimer = 5;
    return true;
}

BOOL daNbomb_c::procInsectMove() {
    if (mNoHitPlayerTimer != 0) {
        mNoHitPlayerTimer--;

        if (mNoHitPlayerTimer == 0) {
            offStateFlg0(FLG0_NO_HIT_PLAYER);
        }
    }

    if (checkExplode()) {
        return true;
    }

    if (field_0xc20.abs2() > 1.0f) {
        mpBck->init((J3DAnmTransform*)dComIfG_getObjectRes(daAlink_c::getAlinkArcName(), 0x15),
                    TRUE, -1, 1.0f, 0, -1, true);

        mDoMtx_copy(cMtx_getIdentity(), field_0xa40);
        current.pos += field_0xbf0 * 50.0f;
        field_0xbfc += field_0xbf0;
        field_0xbfc *= 10.0f;

        speed.y = field_0xbfc.y;
        speedF = field_0xbfc.absXZ();
        current.angle.y = field_0xbfc.atan2sX_Z();
        mAcch.ClrGroundHit();
        mAcch.ClrWallHit();
        mAcch.ClrRoofHit();
        mAcch.ClrRoofNone();
        return procWaitInit();
    }

    if (dComIfG_Bgsp().ChkPolySafe(mInsectHitPolyInfo) &&
        dComIfG_Bgsp().ChkMoveBG(mInsectHitPolyInfo))
    {
        dComIfG_Bgsp().MoveBgTransPos(mInsectHitPolyInfo, true, &current.pos, &shape_angle, NULL);
        setHitPolygon(1);
    }

    if (!checkStateFlg0(FLG0_FROZEN)) {
        if (fopAcIt_Judge((fopAcIt_JudgeFunc)daNbomb_searchEnemy, this)) {
            f32 var_f30 = field_0xbfc.inprod(field_0xc14);

            if (!(fabsf(var_f30) > 0.999f)) {
                cXyz sp60 = field_0xbfc.outprod(field_0xc14);
                sp60.normalizeZP();

                f32 var_f29 = cM_atan2f(JMAFastSqrt(1.0f - (var_f30 * var_f30)), var_f30);
                f32 var_f31 = var_f29 * 0.2f;
                // The number 0.08726647f should be DEG_TO_RAD(5.0f) but that rounds wrong
                if (var_f31 > 0.08726647f) {
                    var_f31 = 0.08726647f;
                } else if (var_f31 < (0.08726647f * 0.1f)) {
                    var_f31 = 0.08726647f * 0.1f;

                    if (0.08726647f * 0.1f > var_f29) {
                        var_f31 = var_f29;
                    }
                }

                mDoMtx_stack_c::rotAxisRadS(&sp60, var_f31);
                mDoMtx_stack_c::multVecSR(&field_0xbf0, &field_0xbf0);
                mDoMtx_stack_c::multVecSR(&field_0xc08, &field_0xc08);
                mDoMtx_stack_c::multVecSR(&field_0xbfc, &field_0xbfc);

                csXyz sp104(current.angle);
                mDoMtx_stack_c::ZXYrotM(current.angle);
                mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &current.angle);

                shape_angle.x += current.angle.x - sp104.x;
                shape_angle.y += current.angle.y - sp104.y;
                shape_angle.z += current.angle.z - sp104.z;
            }
        }

        cXyz sp6C = old.pos + (field_0xbf0 * 25.0f);
        cXyz sp78 = sp6C + (field_0xbfc * speedF);
        mLineChk.Set(&sp6C, &sp78, this);

        int var_r28 = insectLineCheck();
        if (var_r28 == 1) {
            setHitPolygon(0);
        } else if (var_r28 == 0) {
            sp6C = sp78 + field_0xbf0;
            sp78 = sp6C - (field_0xbf0 * 30.0f);
            mLineChk.Set(&sp6C, &sp78, this);

            var_r28 = insectLineCheck();
            if (var_r28 == 1) {
                setHitPolygon(0);
            } else if (var_r28 == 0) {
                sp6C = sp78 - (field_0xbfc * speedF);
                mLineChk.Set(&sp78, &sp6C, this);

                var_r28 = insectLineCheck();
                if (var_r28 == 1) {
                    setHitPolygon(0);
                }
            }
        }

        if (var_r28 != 1) {
            if (checkStateFlg0(FLG0_UNK_800)) {
                onStateFlg0(FLG0_DELETE_BOMB);
                return true;
            }

            if (var_r28 == 2) {
                current.pos = mLineChk.GetCross();
            } else {
                current.pos += field_0xbfc * speedF;
            }

            return procExplodeInit();
        }
    } else {
        mpBck->setPlaySpeed(0.0f);
    }

    cXyz sp84(current.pos);
    mAcch.CrrPos(dComIfG_Bgsp());
    current.pos = sp84;

    setRoomInfo();

    if (mpBck->checkFrame(0.0f) || mpBck->checkFrame(8.0f)) {
        mSound.startSound(Z2SE_EN_BI_FOOTNOTE, dKy_pol_sound_get(&mInsectHitPolyInfo),
                          mSoundReverb);
    }

    return true;
}

int daNbomb_c::execute() {
    if (!checkStateFlg0(FLG0_UNK_2000)) {
        field_0xb51 = 0;
    } else {
        offStateFlg0(FLG0_UNK_2000);
    }

    cXyz spB4(speedF * cM_ssin(current.angle.y), speed.y, speedF * cM_scos(current.angle.y));

    f32 var_f31 = field_0xc20.abs2();
    if (!checkStateFlg0(FLG0_UNK_8000)) {
        if (var_f31 > 1.0f) {
            spB4 += field_0xc20;
            current.angle.y = spB4.atan2sX_Z();
            speedF = spB4.absXZ();
            speed.y = spB4.y;
        }

        var_f31 = 0.0f;
        field_0xc20 = cXyz::Zero;
    } else {
        offStateFlg0(FLG0_UNK_8000);
    }

    f32 var_f30 = field_0xc2c.abs2();
    cXyz spC0;
    int sp18;

    if (checkStateFlg0(FLG0_UNDERWATER) && fopAcM_GetParam(this) == PRM_BOMB_WAIT &&
        fopAcM_getWaterStream(&current.pos, mAcch.m_gnd, &spC0, &sp18, 0))
    {
        cLib_addCalcPos(&field_0xc2c, (spC0 * sp18) * 0.5f, 0.5f, 5.0f, 1.0f);
    } else {
        if (var_f30 > 1.0f) {
            spB4 += field_0xc2c;
            current.angle.y = spB4.atan2sX_Z();
            speedF = spB4.absXZ();
            speed.y = spB4.y;
        }

        var_f30 = 0.0f;
        field_0xc2c = cXyz::Zero;
    }

    field_0xb5a = 0;
    mEffectLastPosition = mEffectPosition;

    if (checkStateFlg0(FLG0_UNK_800) || (checkStateFlg0(FLG0_FROZEN) && mFreezeTimer == 0)) {
        if (cLib_chaseF(&scale.x, 0.0f, 0.025f)) {
            onStateFlg0(FLG0_DELETE_BOMB);
        } else {
            scale.z = scale.y = scale.x;
            mpModel->setBaseScale(scale);
        }
    }

    if (mProcFunc != NULL) {
        (this->*mProcFunc)();
    }

    if (checkStateFlg0(FLG0_DELETE_BOMB)) {
        fopAcM_delete(this);
        return 1;
    }

    if ((mAcch.ChkWallHit() && daAlink_c::checkIcePolygonDamage(&mAcchCir)) ||
        (mAcch.ChkGroundHit() && daAlink_c::checkIcePolygonDamage(&mAcch.m_gnd)) ||
        (mAcch.ChkRoofHit() && daAlink_c::checkIcePolygonDamage(&mAcch.m_roof)))
    {
        setFreeze();
    }

    if (mpBck != NULL) {
        mpBck->play();
    }

    daAlink_c* player = daAlink_getAlinkActorClass();
    if (fopAcM_GetParam(this) != PRM_NORMAL_BOMB_EXPLODE) {
        if (checkWaterIn()) {
            if (dComIfG_Bgsp().GetPolyAtt0(mAcch.m_wtr) == 6) {
                onStateFlg0(FLG0_DELETE_BOMB);
            } else if (!checkStateFlg0(daNbomb_FLG0(FLG0_UNK_800 | FLG0_UNDERWATER))) {
                fopAcM_seStartCurrent(this, Z2SE_CM_BODYFALL_WATER_S, 0);
                cXyz pillar_pos;

                f32 dist_to_old_y = fabsf(old.pos.y - current.pos.y);
                f32 dist_to_water = fabsf(mAcch.m_wtr.GetHeight() - current.pos.y);
                if (dist_to_old_y < 1.0f) {
                    pillar_pos.x = current.pos.x;
                    pillar_pos.y = mAcch.m_wtr.GetHeight();
                    pillar_pos.z = current.pos.z;
                } else {
                    f32 temp_f29 = dist_to_water / dist_to_old_y;
                    if (temp_f29 > 1.0f) {
                        temp_f29 = 1.0f;
                    }

                    pillar_pos = (old.pos * temp_f29) + (current.pos * (1.0f - temp_f29));
                }

                fopKyM_createWpillar(&pillar_pos, 1.0f, 0);

                if (!checkStateFlg0(FLG0_WATER_BOMB) &&
                    (fopAcM_checkCarryNow(this) || fopAcM_checkHookCarryNow(this) ||
                     fopAcM_GetParam(this) == PRM_ENEMY_BOMB_BOOMERANG_MOVE ||
                     fopAcM_GetParam(this) == PRM_BOMB_BOOMERANG_MOVE))
                {
                    onStateFlg0(FLG0_DELETE_BOMB);
                } else if (checkStateFlg0(FLG0_INSECT_BOMB)) {
                    speedF *= 0.5f;
                    mpBck->setPlaySpeed(mpBck->getPlaySpeed() * 0.5f);
                    onStateFlg0(FLG0_UNK_800);
                } else {
                    if (checkStateFlg0(FLG0_WATER_BOMB)) {
                        onStateFlg0(FLG0_UNDERWATER);
                    } else {
                        onStateFlg0(FLG0_UNK_800);
                        field_0xbe4 = cXyz::Zero;
                    }

                    speedF *= 0.5f;
                    speed.y *= 0.5f;
                    gravity = player->getBombWaterGravity();
                    maxFallSpeed = player->getBombWaterMaxFallSpeed();
                }
            } else if (!checkStateFlg0(FLG0_WATER_BOMB) &&
                       (fopAcM_checkCarryNow(this) || fopAcM_checkHookCarryNow(this)))
            {
                onStateFlg0(FLG0_DELETE_BOMB);
            }

            if (checkStateFlg0(FLG0_DELETE_BOMB)) {
                fopAcM_delete(this);
                field_0xbe4 = cXyz::Zero;
                return 1;
            }
        } else {
            if (checkStateFlg0(FLG0_UNK_800)) {
                fopAcM_delete(this);
                return 1;
            }

            offStateFlg0(FLG0_UNDERWATER);
            gravity = player->getBombGravity();
            maxFallSpeed = player->getBombMaxFallSpeed();
        }

        if (speedF > 1.0f || var_f31 > 1.0f || var_f30 > 1.0f) {
            int var_r27 = 1;
            f32 var_f27;
            s16 temp_r3;

            if (var_f31 > 1.0f || var_f30 > 1.0f) {
                cXyz spD8(field_0xc2c.x + (field_0xc20.x + (speedF * cM_ssin(current.angle.y))),
                          field_0xc20.y + field_0xc2c.y,
                          field_0xc2c.z + (field_0xc20.z + (speedF * cM_scos(current.angle.y))));

                var_f27 = spD8.absXZ();
                temp_r3 = spD8.atan2sX_Z();
            } else {
                var_f27 = speedF;
                temp_r3 = current.angle.y;

                if (mAcch.ChkGroundHit()) {
                    var_r27 = 0;
                }
            }

            s16 var_r26 = (var_f27 / 180.0f) * (f32)0x10000;
            s16 var_r25 = temp_r3 - shape_angle.y;
            mDoMtx_stack_c::YrotS(var_r25);

            if (var_r27 == 0) {
                mDoMtx_stack_c::XrotM(var_r26);
            } else {
                mDoMtx_stack_c::XrotM(var_r26 >> 1);
            }

            mDoMtx_stack_c::YrotM(-var_r25);
            mDoMtx_stack_c::concat(field_0xa40);
            mDoMtx_copy(mDoMtx_stack_c::get(), field_0xa40);
        }

        if (player->getGrabActorID() == fopAcM_GetID(this) && player->checkGrabUp()) {
            mDoMtx_stack_c::YrotS(field_0xb5c - shape_angle.y);
            mDoMtx_stack_c::concat(field_0xa40);
            mDoMtx_copy(mDoMtx_stack_c::get(), field_0xa40);
        }

        f32 var_f26 = 0.0f;
        if (field_0xb5a != 0 && field_0xb5a < 0x4000) {
            var_f26 = (1.0f / cM_scos(field_0xb5a) - 1.0f) * 25.0f * 0.75f;
        }

        if (fopAcM_GetParam(this) != PRM_INSECT_BOMB_MOVE) {
            mDoMtx_stack_c::transS(current.pos.x, field_0xbb0 + (current.pos.y + var_f26),
                                   current.pos.z);
            mDoMtx_stack_c::ZXYrotM(shape_angle);
            mDoMtx_stack_c::transM(0.0f, 25.0f, 0.0f);
            mDoMtx_stack_c::concat(field_0xa40);
            mDoMtx_stack_c::transM(0.0f, -25.0f, 0.0f);
            mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
        }

        mDoMtx_multVec(mpModel->getBaseTRMtx(), &l_localCenterOffset, &attention_info.position);
        eyePos = attention_info.position;
        mCcSph.SetC(attention_info.position);
        mCcSph.SetR(scale.x * 30.0f);
        dComIfG_Ccsp()->Set(&mCcSph);

        if (fopAcM_GetParam(this) != PRM_FLOWER_BOMB) {
            mSph2.SetC(attention_info.position);
            dComIfG_Ccsp()->Set(&mSph2);
        }

        if (!checkStateFlg0(FLG0_UNK_800)) {
            setSmokePos();
        }

        if (mpBck != NULL) {
            mpBck->entry(mpModel->getModelData());
        }

        mpModel->calc();

        if (!checkTimerStop()) {
            mSound.startLevelSound(Z2SE_OBJ_BOMB_IGNITION, 0, mSoundReverb);
            setEffect();
        }

        field_0xb5c = shape_angle.y;

        if (checkStateFlg0(FLG0_FROZEN) && mFreezeTimer != 0) {
            mFreezeTimer--;
        }
    }

    mSound.framework(mPolySound, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    return 1;
}


static int daNbomb_Execute(daNbomb_c* i_this) {
    return i_this->execute();
}

int daNbomb_c::draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);

    if (fopAcM_GetParam(this) == PRM_NORMAL_BOMB_EXPLODE) {
        return 1;
    }

    daAlink_c* player = daAlink_getAlinkActorClass();
    J3DGXColorS10 bomb_color;
    bomb_color.g = 0;
    bomb_color.a = 0;

    if (checkStateFlg0(FLG0_UNK_800)) {
        bomb_color.r = 0;
        bomb_color.b = 0;
    } else if (checkStateFlg0(FLG0_FROZEN)) {
        bomb_color.r = player->getFreezeR();
        bomb_color.g = player->getFreezeG();
        bomb_color.b = player->getFreezeB();
    } else {
        s16 explode_time = player->getBombExplodeTime();
        f32 brightness;

        if (mExTime > explode_time) {
            brightness = 1.0f - fabsf(cM_fcos(((f32)(mExTime - explode_time) * M_PI) /
                                              (f32)FAST_DIV(explode_time, 2)));
        } else if (mExTime > FAST_DIV(explode_time, 2)) {
            brightness = 1.0f - fabsf(cM_fcos(((f32)(mExTime - FAST_DIV(explode_time, 2)) * M_PI) /
                                              (f32)FAST_DIV(explode_time, 4)));
        } else if (mExTime > FAST_DIV(explode_time, 4)) {
            brightness = fabsf(cM_fsin(((f32)(mExTime - FAST_DIV(explode_time, 4)) * M_PI) /
                                       (f32)FAST_DIV(explode_time, 7)));
        } else {
            brightness = fabsf(cM_fsin(((f32)(mExTime - FAST_DIV(explode_time, 7)) * M_PI) /
                                       (f32)FAST_DIV(explode_time, 8)));
        }

        if (mType == TYPE_INSECT_ENEMY || mType == TYPE_WATER_ENEMY) {
            bomb_color.r = (s16)(brightness * (f32)player->getEnemyBombColorR()) & 0xFF;
        } else {
            bomb_color.r = (s16)(brightness * 15.0f) & 0xFF;
        }

        bomb_color.b = 0;
    }

    if (checkStateFlg0(FLG0_FROZEN)) {
        tevStr.TevColor.r = player->getFreezeR();
        tevStr.TevColor.g = player->getFreezeG();
        tevStr.TevColor.b = player->getFreezeB();
    }

    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    J3DModelData* modelData = mpModel->getModelData();

    J3DMaterial* mat;
    if (mType == TYPE_INSECT_ENEMY) {
        mat = modelData->getMaterialNodePointer(1);

        modelData->getMaterialNodePointer(0)->getShape()->hide();
        modelData->getMaterialNodePointer(2)->getShape()->hide();
        modelData->getMaterialNodePointer(1)->getShape()->show();
        modelData->getMaterialNodePointer(3)->getShape()->show();
    } else {
        mat = modelData->getMaterialNodePointer(0);
    }

    if (mType == TYPE_WATER_ENEMY || checkStateFlg0(FLG0_WATER_BOMB)) {
        mat->setTevColor(0, &bomb_color);

        mat = modelData->getMaterialNodePointer(1);
        mat->setTevColor(0, &bomb_color);
    } else {
        mat->setTevColor(1, &bomb_color);
    }

    mDoExt_modelEntryDL(mpModel);
    daMirror_c::entry(mpModel);

    bomb_color.r = 0;
    bomb_color.g = 0;
    bomb_color.b = 0;

    if (mType == TYPE_WATER_ENEMY || checkStateFlg0(FLG0_WATER_BOMB)) {
        mat->setTevColor(0, &bomb_color);
        modelData->getMaterialNodePointer(0)->setTevColor(0, &bomb_color);
    } else {
        mat->setTevColor(1, &bomb_color);
    }

    if (mAcch.GetGroundH() != -G_CM3D_F_INF && !fopAcM_checkCarryNow(this)) {
        cM3dGPla ground_poly;
        if (dComIfG_Bgsp().GetTriPla(mAcch.m_gnd, &ground_poly)) {
            dComIfGd_setSimpleShadow(&current.pos, mAcch.GetGroundH(), scale.x * 25.0f,
                                     &ground_poly.mNormal, 0, 1.0f,
                                     dDlst_shadowControl_c::getSimpleTex());
        }
    }

    return 1;
}


static int daNbomb_Draw(daNbomb_c* i_this) {
    return i_this->draw();
}

AUDIO_INSTANCES;

static actor_method_class l_daNbombMethod = {
    (process_method_func)daNbomb_Create,
    (process_method_func)daNbomb_Delete,
    (process_method_func)daNbomb_Execute,
    0,
    (process_method_func)daNbomb_Draw,
};

extern actor_process_profile_definition g_profile_NBOMB = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NBOMB,             // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNbomb_c),      // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  270,                    // mPriority
  &l_daNbombMethod,       // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
