/**
 * @file d_a_arrow.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_arrow.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "JSystem/JAudio2/JAUSectionHeap.h"
#include "SSystem/SComponent/c_math.h"
#include "Z2AudioLib/Z2Instances.h"
#include "f_op/f_op_kankyo_mng.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_alink.h"
#include "d/d_bomb.h"
#include "d/actor/d_a_e_pz.h"
#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_hozelda.h"

int daArrow_c::createHeap() {
    J3DModelData* model_data;

    if (mArrowType == 2) {
        model_data = (J3DModelData*)dComIfG_getObjectRes("HoZelda", 0x1f);
    } else {
        u16 index;
        
        if (mArrowType == 4) {
            index = 0x20;
        } else if (mArrowType == 1) {
            index = 0x1d;
        } else {
            index = 0x1c;
        }
        model_data = (J3DModelData*)dComIfG_getObjectRes(daAlink_c::getAlinkArcName(), index);
    }

    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);

    if (mpModel == NULL) {
        return FALSE;
    }

    mSoundObjArrow.init(&current.pos, 1);
    mReverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));

    return TRUE;
}

static u8 const lit_3768[12] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static int daArrow_createHeap(fopAc_ac_c* i_this) {
    return static_cast<daArrow_c*>(i_this)->createHeap();
}

const static dCcD_SrcCps l_atCpsSrc = {
    {
        {0x0, {{AT_TYPE_ARROW, 0x2, 0x1b}, {0xd8fbffff, 0x11}, 0x0}}, // mObj
        {dCcD_SE_23, 0x1, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 5.0f}, // mCps
    } // mCpsAttr
};

const static dCcD_SrcSph l_coSphSrc = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x0}, 0x319}}, // mObj
        {dCcD_SE_23, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 25.0f} // mSph
    } // mSphAttr
};

void daArrow_c::atHitCallBack(dCcD_GObjInf* i_atObjInf, fopAc_ac_c* i_tgActor, dCcD_GObjInf* i_tgObjInf) {
    if (i_tgObjInf->ChkTgArrowThrough()) {
        if (i_tgActor != NULL && fopAcM_GetName(i_tgActor) == PROC_E_PZ) {
            ((daE_PZ_c*)i_tgActor)->onBombArrowHit();
        }
    } else if (i_tgActor != NULL && fopAcM_IsActor(i_tgActor)) {
        cXyz* hit_pos_p = i_atObjInf->GetAtHitPosP();
        f32 dist_to_hitpos = current.pos.abs(*hit_pos_p);

        if (dist_to_hitpos < field_0x998) {
            field_0x998 = dist_to_hitpos;
            mHitAcID = fopAcM_GetID(i_tgActor);

            if (mArrowType == 1) {
                field_0x9a8 = *hit_pos_p;
            } else if (i_tgObjInf->ChkTgShield()) {
                field_0x93e = 1;
            } else {
                field_0x93e = 0;
                dJntCol_c * jnt = fopAcM_GetJntCol(i_tgActor);
                if (jnt != NULL) {
                    field_0x960 = jnt->getArrowOffsetPosAndAngle(hit_pos_p, &shape_angle, &field_0x9a8, &field_0x9b4);
                }
            }
        }
    }
}

static void daArrow_atHitCallBack(fopAc_ac_c* i_this, dCcD_GObjInf* i_atObjInf, fopAc_ac_c* i_tgActor, dCcD_GObjInf* i_tgObjInf) {
    static_cast<daArrow_c*>(i_this)->atHitCallBack(i_atObjInf, i_tgActor, i_tgObjInf);
}

void daArrow_c::decAlphaBlur() {
    for (int i = 0; i < field_0x946; i++) {
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(field_0x97c[0]);
        if (emitter != NULL) {
            u8 alpha = emitter->getGlobalAlpha();
            if (alpha > 0x33) {
                setBlur();
                emitter->setGlobalAlpha(alpha - 51);
            } else {
                emitter->setGlobalAlpha(0);
            }
        }
    }
}

void daArrow_c::setBlur() {
    for (int i = 0; i < field_0x946; i++) {
        field_0x97c[i] = dComIfGp_particle_set(field_0x97c[i], field_0x94c, &current.pos, &tevStr);
        dComIfGp_particle_levelEmitterOnEventMove(field_0x97c[i]);
    }
}

void daArrow_c::setLightArrowHitMark(cXyz const* param_0) {
    static u16 const effName[] = {0x896F, 0x8970, 0x8971, 0x8972, 0x8973, 0x8974};
    for (int i = 0; i < 6; i++) {
        dComIfGp_particle_set(effName[i], param_0, &shape_angle, NULL);
    }
    
    field_0x93f = 1;
    mSoundObjArrow.startSound(Z2SE_ZELDA_ARROW_HIT, 0, mReverb);
}

void daArrow_c::setLightChargeEffect(int param_0) {
    static u16 const effName[] = {0x8975, 0x8976, 0x8977, 0x8978};
    static Vec const localEffPos = {0.0f, 0.0f, 90.0f};
    if (mpModel != NULL) {
        cXyz pos;
        BOOL bVar1 = param_0 == 0 && (fopAcM_GetParam(this) == 1 || fopAcM_GetParam(this) == 2 || fopAcM_GetParam(this) == 0);
        mDoMtx_multVec(mpModel->getBaseTRMtx(), &localEffPos, &pos);
        for (int i = 0; i < 4; i++) {
            if (bVar1) {
                field_0x96c[i] = dComIfGp_particle_set(field_0x96c[i], effName[i], &pos, &tevStr);
                dComIfGp_particle_levelEmitterOnEventMove(field_0x96c[i]);
            } else {
                JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(field_0x96c[i]);
                if (emitter != NULL) {
                    emitter->stopDrawParticle(); 
                }
            }
        }
    }
}

int daArrow_c::setArrowWaterNextPos(cXyz* i_start, cXyz* i_end) {
    field_0x5dc.Set(i_start, i_end, this);

    if (dComIfG_Bgsp().LineCross(&field_0x5dc)) {
        if (dComIfG_Bgsp().GetPolyAtt0(field_0x5dc) == 6) {
            *i_end = field_0x5dc.GetCross();
            return 2;
        }
        
        cXyz v = *i_end - field_0x5dc.GetCross();
        f32 dis = (*i_end).abs(*i_start);

        if (dis > 0.0001f) {
            dis = v.abs() / dis;
        } else {
            dis = 1.0f;
        }

        v.normalizeZP();
        *i_end = field_0x5dc.GetCross() + v * 80.0f * dis;
        return 1;
    } 
    
    if (fopAcM_wt_c::waterCheck(i_start) && fopAcM_wt_c::getWaterY() > i_start->y) {
        if (fopAcM_wt_c::getPolyAtt0() == 6) {
            i_end->y = fopAcM_wt_c::getWaterY();
            return 4;
        }

        cXyz v = *i_end - *i_start;
        v.normalizeZP();
        *i_end = *i_start + (v * 80.0f);
        return 3;
    }

    return 0;
}

void daArrow_c::setArrowAt(f32 param_0) {
    daAlink_c* player = daAlink_getAlinkActorClass();

    f32 radius;
    if (mArrowType == 4) {
        radius = 8.0f;
    } else {
        radius = 5.0f;
    }

    f32 arrow_start = player->getArrowIncAtMaxStart();
    if (param_0 > arrow_start) {
        f32 inc = player->getArrowIncAtMax();
        if (inc <= arrow_start) {
            radius += player->getArrowIncAtR();
        } else {
            f32 mod = (param_0 - arrow_start) / (inc - arrow_start);
            if (mod > 1.0f) {
                mod = 1.0f;
            }
            radius += (mod * player->getArrowIncAtR());
        }
    }

    cXyz target = current.pos + speed * (mOutLengthRate + 1.0f);

    if (field_0x945 == 0) {
        setArrowWaterNextPos(&current.pos, &target);
    } else {
        f32 fVar7 = target.y - field_0x9fc.y;
        if (fVar7 < -300.0f) {
            f32 fVar1 = fabsf(target.y - current.pos.y);
            fVar7 = -300.0f - fVar7;

            if (fVar1 > fVar7) {
                target = current.pos + speed * ((mOutLengthRate + 1.0f) * ((fVar1 - fVar7) / fVar1));
            }
        }
    }

    field_0x56c.Set(&current.pos, &target, this);

    if (dComIfG_Bgsp().LineCross(&field_0x56c)) {
        target = field_0x56c.GetCross();
    }

    static_cast<cM3dGCps*>(&field_0x688)->Set(current.pos, target, radius);
    field_0x688.CalcAtVec();

    dComIfG_Ccsp()->Set(&field_0x688);
    dComIfG_Ccsp()->SetMass(&field_0x688, 1);
}

void daArrow_c::arrowShooting() {
    daAlink_c* link = daAlink_getAlinkActorClass();

    field_0x950 = link->getBombArrowFlyExplodeTime();

    if (mArrowType == 4) {
        current.angle.x = -link->getBodyAngleX();
        current.angle.y = link->shape_angle.y + link->getBodyAngleY();
    } else if (dComIfGp_checkPlayerStatus0(0, 0x200000) || fopAcM_GetParam(this) == 2)  {
        cXyz* pos = link->checkBowCameraArrowPosP(&shape_angle.x, &shape_angle.y);
        if (pos != NULL) {
            current.pos = *pos;
            
            if (link->checkMagneBootsOn()) {
                mDoMtx_stack_c::copy(link->getMagneBootsMtx());
                mDoMtx_stack_c::ZXYrotM(shape_angle);
                mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &shape_angle);
            }

            current.angle.x = -shape_angle.x;
            current.angle.y = shape_angle.y;
        }
    }

    speedF = 100.f;
    mStartPos = current.pos;

    f32 cos = cM_scos(current.angle.x);
    speed.x = cos * (field_0x99c * cM_ssin(current.angle.y));
    speed.y = field_0x99c * cM_ssin(current.angle.x);
    speed.z = cos * (field_0x99c * cM_scos(current.angle.y));

    daPy_py_c* player = daPy_getPlayerActorClass();
    if (player->checkHorseRide()) {
        daHorse_c* horse = dComIfGp_getHorseActor();
        cMtx_YrotS(*calc_mtx, horse->shape_angle.y);

        cXyz src;
        cXyz dst;

        src.x = 0.0f;
        src.y = 0.0f;
        src.z = horse->speedF;

        MtxPosition(&src, &dst);

        speed += dst;
    }

    if (mArrowType == 2) {
        field_0x688.SetAtMtrl(dCcD_MTRL_LIGHT);
        field_0x688.OnAtNoHitMark();
    } else {
        if (mArrowType == 4) {
            field_0x688.SetAtType(0x80);
            field_0x688.SetAtSe(22);
            field_0x688.SetAtAtp(0);
            field_0x688.OnAtNoHitMark();
        }

        field_0x94e = m_count;
        m_count++;
        if (m_count == 6) {
            m_count = 0;
        }
    }

    setBlur();
    setArrowAt(0.0f);
    clearNearActorData();
}

void daArrow_c::setBombArrowExplode(cXyz* param_0) {
    dBomb_c::createNormalBombExplode(param_0);
    field_0x93f = 1;
}

void daArrow_c::setRoomInfo() {
    int roomNo;

    if (fopAcM_gc_c::gndCheck(&current.pos)) {
        roomNo = fopAcM_gc_c::getRoomId();
        tevStr.YukaCol = fopAcM_gc_c::getPolyColor();
    } else {
        roomNo = dComIfGp_roomControl_getStayNo();
    }
    
    tevStr.room_no = roomNo;
    field_0x64c.SetRoomId(roomNo);
    fopAcM_SetRoomNo(this, roomNo);
}

void daArrow_c::clearNearActorData() {
    field_0x93e = 0;
    field_0x998 = FLT_MAX;
    mHitAcID = -1;
}

s16 daArrow_c::getVibAngle() {
    s16 angle;
    if (cLib_calcTimer(&field_0x952)) {
        field_0x954 += (s16)(21243.0f - cM_rndF(4096.0f));

        f32 f = field_0x952 * 0.02f;
        angle = f * 1024.0f * f * cM_ssin(field_0x954);
    } else {
        angle = 0;
    }

    return angle;
}

void daArrow_c::setNormalMatrix() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, 0);

    u32 param = fopAcM_GetParam(this);
    if (param == 4) {
        mDoMtx_stack_c::transM(0.0f, 0.0f, -95.0f);
    } else if (param == 5) {
        mDoMtx_stack_c::transM(0.0f, 0.0f, -47.5f);

    }
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daArrow_c::setSmokePos() {
    static cXyz localOffset(-3.3f, -3.0f, 94.0f);

    mDoMtx_multVec(mpModel->getBaseTRMtx(), &localOffset, &field_0x9cc);
    field_0x958.y = shape_angle.y;

    mSmokePos = (field_0x9cc - field_0x9d8) * 0.9f;
}

void daArrow_c::setKeepMatrix() {
    if (mArrowType == 2) {
        daHoZelda_c* actor = (daHoZelda_c*)field_0xa08.getActor();
        if (actor != NULL) {
            mDoMtx_stack_c::copy(actor->getRightHandMtx());
            mDoMtx_stack_c::transM(20.0f, -2.0f, 0.0f);
            mDoMtx_XYZrotM(mDoMtx_stack_c::get(), cM_deg2s(90.0f), cM_deg2s(60.0f), cM_deg2s(90.0f));
        } else {
            field_0x93f = 1;
            return;
        }
    } else {
        mDoMtx_stack_c::YrotS(-0x8000);
        mDoMtx_stack_c::revConcat(daAlink_getAlinkActorClass()->getLeftItemMatrix());
    }
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_multVecZero(mpModel->getBaseTRMtx(), &current.pos);
    mDoMtx_MtxToRot(mpModel->getBaseTRMtx(), &shape_angle);

    current.angle.y = shape_angle.y;
    current.angle.x = -shape_angle.x;
}

fopAc_ac_c* daArrow_c::setStopActorMatrix() {
    s16 vibe_angle = getVibAngle();
    fopAc_ac_c* actor = fopAcM_SearchByID(mHitAcID);
    if (actor == NULL) {
        return NULL;
    }

    dJntCol_c* jnt = fopAcM_GetJntCol(actor);
    if (jnt == NULL) {
        return NULL;
    }
    
    jnt->setArrowPosAndAngle(&field_0x9a8, &field_0x9b4, field_0x960, &current.pos, &shape_angle);
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x + vibe_angle, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::transM(0.0f, 0.0f, -95.0f);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    return actor;
}

void daArrow_c::setBombMoveEffect() {
    if (mArrowType == 1 && field_0x945 == 0 && field_0x943 == 0) {
        setSmokePos();
        field_0x964 = dComIfGp_particle_set(field_0x964, 0x1e0, &field_0x9cc, &tevStr, &field_0x958,
                                            NULL, 0xff, &field_0xa10, -1, NULL, NULL, NULL);
        dComIfGp_particle_levelEmitterOnEventMove(field_0x964);
        field_0xa10.setRate(0.25f);

        field_0x968 = dComIfGp_particle_set(field_0x968, 0x1de, &field_0x9cc, &tevStr, &field_0x958,
                                            NULL, 0xff, NULL, -1, NULL, NULL, NULL);
        dComIfGp_particle_levelEmitterOnEventMove(field_0x968);

        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(field_0x968);
        if (emitter != NULL) {
            emitter->playDrawParticle();
        }
    }
}

bool daArrow_c::checkReget() {
    if (field_0x7cc.ChkCoHit()) {
        dComIfGp_setItemArrowNumCount(1);
        fopAcM_createItemForSimpleDemo(&current.pos, fpcNm_ITEM_ARROW_1, -1, NULL, NULL, 0.0f, 0.0f);
        mDoAud_seStart(Z2SE_CONSUMP_ITEM_GET, 0, 0, 0);
        field_0x93f = 1;
        return true;
    } else {
        field_0x7cc.SetC(current.pos);
        dComIfG_Ccsp()->Set(&field_0x7cc);
        return false;
    }
}

int daArrow_c::procWait() {
    setKeepMatrix();

    u32 param = fopAcM_GetParam(this);
    if (param == 1 || param == 2) {
        tevStr.TevColor.r = 0;
        tevStr.TevColor.g = 0;
        tevStr.TevColor.b = 0;

        daAlink_c* link = daAlink_getAlinkActorClass();
        if (mArrowType == 1) {
            field_0x688.SetAtAtp(0);
            if (!link->checkCanoeSlider()) {
                field_0x688.OnAtSPrmBit(0x20);
            }
        } else {
            field_0x688.SetAtAtp(2);
        }
        link->getArrowFlyData(&mFlyMax, &field_0x99c, 0);
        mOutLengthRate = 95.0f / field_0x99c;
        field_0xa10.setOldPosP(&field_0x9d8, &field_0x9e4);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(field_0x964);
        if (emitter != NULL) {
            emitter->setParticleCallBackPtr(NULL);
        }
        mProcFunc = &daArrow_c::procMove;
        arrowShooting();
        setBombMoveEffect();
    } else if (mArrowType == 1) {
        setSmokePos();
        
        field_0x964 = dComIfGp_particle_set(field_0x964, 0x1df, &field_0x9cc, &tevStr, &field_0x958,
                                            NULL, 0xff, NULL, -1, NULL, NULL, NULL);
        dComIfGp_particle_levelEmitterOnEventMove(field_0x964);
        
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(field_0x964);
        if (emitter != NULL) {
            emitter->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
            emitter->setUserWork((uintptr_t)&mSmokePos);

            if (field_0x942 != 0) {
                emitter->stopDrawParticle();
            } else {
                emitter->playDrawParticle();
            }
        }
        
        field_0x968 = dComIfGp_particle_set(field_0x968, 0x1de, &field_0x9cc, &tevStr, &field_0x958,
                                            NULL, 0xff, NULL, -1, NULL, NULL, NULL);
        dComIfGp_particle_levelEmitterOnEventMove(field_0x968);
        emitter = dComIfGp_particle_getEmitter(field_0x968);
        if (emitter != NULL) {
            if (field_0x942 != 0) {
                emitter->stopDrawParticle();
            } else {
                emitter->playDrawParticle();
            }
        }

        mSoundObjArrow.startLevelSound(Z2SE_OBJ_BOMB_IGNITION, 0, mReverb);
    }

    if(dComIfGp_checkCameraAttentionStatus(dComIfGp_getPlayerCameraID(0), 2)) {
        field_0x942 = 3;
    } else {
        field_0x942 = 0;
    }

    return TRUE;
}

int daArrow_c::procMove() {
    if (fopAcM_GetParam(this) == 6) {
        setBlur();
        mProcFunc = &daArrow_c::procActorControllStop;
        speedF = 0.0f;
        procActorControllStop();
        return TRUE;
    }

    current.pos += speed;
    cXyz v = current.pos + speed * mOutLengthRate;
    BOOL bVar14 = FALSE;
    if (field_0x945 == 0) {
        field_0x56c.Set(&old.pos, &v, this);

        cXyz v2;
        if (dComIfG_Bgsp().LineCross(&field_0x56c)) {
            v2 = field_0x56c.GetCross();
        } else {
            v2 = v;
        }

        int water_next_pos = setArrowWaterNextPos(&old.pos, &v2);
        if (water_next_pos != 0) {
            if (water_next_pos == 2 || water_next_pos == 4) {
                bVar14 = TRUE;
            } else if (water_next_pos == 1) {
                fopKyM_createWpillar(field_0x5dc.GetCrossP(), 0.3f, 0);
                mDoAud_seStart(Z2SE_CM_BODYFALL_WATER_S, field_0x5dc.GetCrossP(), 0, mReverb);
            }

            field_0x945 = 1;
            if (water_next_pos == 4 || water_next_pos == 3) {
                field_0x9fc = old.pos;
            } else {
                field_0x9fc = field_0x5dc.GetCross();
            }

            speed.normalizeZP();
            current.pos = v2 - (speed * 95.0f); 
            speed *= 80.0f;

            if (mArrowType != 4) {
                mOutLengthRate = 1.1875f;
            }

            v = v2;
        }
    }

    field_0x56c.Set(&old.pos, &v, this);

    u8 uVar17;
    BOOL line_cross = dComIfG_Bgsp().LineCross(&field_0x56c);
    if (field_0x688.ChkTgHit() && field_0x688.GetTgHitGObj() != NULL) {
        uVar17 = field_0x688.GetTgHitGObj()->GetAtMtrl();
    } else {
        uVar17 = 0;
    }

    if (uVar17 == 2 || (line_cross && daAlink_c::checkIcePolygonDamage(&field_0x56c))) {
        field_0x943 = 1;
    } else if (uVar17 == 3) {
        current.pos = *field_0x688.GetTgHitPosP();
        if(field_0x688.GetTgRVecP()->abs2XZ() < 1.0f ) {
            if (field_0x688.GetTgHitAc() != NULL) {
                current.angle.y = fopAcM_searchActorAngleY(this, field_0x688.GetTgHitAc());
            } else {
                current.angle.y = cLib_targetAngleY(&old.pos, &current.pos);
            }
        } else {
            current.angle.y = cM_atan2s(-field_0x688.GetTgRVecP()->x, -field_0x688.GetTgRVecP()->z);
        }
        shape_angle.y = current.angle.y;
        procReturnInit(1);
        return TRUE;
    }

    int iVar3;
    if (field_0x688.ChkAtHit() != 0) {
        if (mArrowType == 1 && field_0x943 == 0) {
            if (field_0x945 != 0) {
                field_0x93f = 1;
                return TRUE;
            }

            if (mHitAcID != -1) {
                setBombArrowExplode(&field_0x9a8);
                return TRUE;
            }
        }

        if (mArrowType == 4) {
            if (field_0x688.GetAtHitAc() != NULL && fopAcM_GetName(field_0x688.GetAtHitAc()) == PROC_E_PZ) {
                iVar3 = 0;
            } else {
                procSlingHitInit(field_0x688.GetAtHitPosP(), &field_0x688);
                return TRUE;
            }
        } else if (field_0x943 != 0) {
            iVar3 = 2;
        } else {
            if (mHitAcID == -1) {
                iVar3 = 0;
            } else {
                iVar3 = 1;
                fopAc_ac_c* hit_ac = fopAcM_SearchByID(mHitAcID);
                if (field_0x93e) {
                    iVar3 = 2;
                } else {
                    if (hit_ac != NULL && fopAcM_GetJntCol(hit_ac) != NULL) {
                        if (field_0x960 >= 0) {
                            iVar3 = 4;
                        } else {
                            if (field_0x960 == -1) {
                                iVar3 = 3;
                            } else if (field_0x960 == -2) {
                                iVar3 = 2;
                            }
                        }
                    }
                }
            }
        }

        if (iVar3 != 4) {
            clearNearActorData();
        }
    } else {
        if (line_cross && field_0x943 != 0) {
            iVar3 = 2;
        } else {
            iVar3 = 0;
        }
    }

    if (iVar3 != 0 && iVar3 != 3) {
        if (mArrowType == 2) {
            setLightArrowHitMark(field_0x688.GetAtHitPosP());
            setBlur();
        } else if (mArrowType == 4) {
            procSlingHitInit(field_0x56c.GetCrossP(), NULL);
        } else if (iVar3 == 2) {
            if (field_0x945) {
                field_0x93f = 1;
            } else {
                procReturnInit(0);
                current.pos = old.pos;
                setNormalMatrix();
            }
        } else if (iVar3 == 4) {
            setBlur();
            fopAcM_SetParam(this, 3);
            mProcFunc = &daArrow_c::procActorStop;
            speedF = 0.0f;
            setStopActorMatrix();
        } else {
            field_0x93f = 1;
            if (field_0x945 == 0 && field_0x688.ChkAtHit()) {
                current.pos = *field_0x688.GetAtHitPosP();
                setBlur();
            }
        }
        return TRUE;
    }

    if (line_cross) {
        current.pos = field_0x56c.GetCross();

        if (field_0x945 != 0 && (mArrowType == 1 || current.pos.y - field_0x9fc.y < -300.0f)) {
            field_0x93f = 1;
            return TRUE;
        }

        dKy_Sound_set(current.pos, 3, fopAcM_GetID(this), 10);

        if (mArrowType == 1) {
            setBombArrowExplode(&current.pos);
            return TRUE;
        }

        if (mArrowType == 2) {
            setLightArrowHitMark(&current.pos);
        }

        cM3dGPla local_4c;
        u32 se_id;
        dComIfG_Bgsp().GetTriPla(field_0x56c, &local_4c);
        int local_d8 = dComIfG_Bgsp().GetPolyAtt0(field_0x56c);
        if (local_d8 == 2 || mArrowType == 4 || local_d8 == 0xe || local_d8 == 10 || local_d8 == 8) {
            if (field_0x945 != 0) {
                field_0x93f = 1;
                return TRUE;
            }

            csXyz angle(
                cM_atan2s(local_4c.GetNP()->y, local_4c.GetNP()->absXZ()), 
                cM_atan2s(-local_4c.GetNP()->x, -local_4c.GetNP()->z), 
                0.0f
            );
            if (mArrowType == 4) {
                se_id = Z2SE_HIT_PACHINKO;
                current.angle = angle;
                procSlingHitInit(&current.pos, NULL);
            } else {
                procReturnInit(0);
                dComIfGp_setHitMark(9, this, &current.pos, &angle, NULL, 0);
                se_id = Z2SE_HIT_AL_ARROW_REBOUND;
            }
        } else {
            setBlur();
            mProcFunc = &daArrow_c::procBGStop;
            fopAcM_SetParam(this, 4);
            speedF = 0.0f;
            se_id = Z2SE_HIT_AL_ARROW_STICK;
            current.angle.x = shape_angle.x;
            dComIfG_Bgsp().ArrowStickCallBack(field_0x56c, this, field_0x56c.GetCross());
        }

        daAlink_getAlinkActorClass()->itemHitSE(se_id, dKy_pol_sound_get(&field_0x56c) & 0xff, &mSoundObjArrow);
    } else {
        if (field_0x945 != 0) {
            if (bVar14) {
                field_0x93f = 1;
                return TRUE;
            }

            f32 f = current.pos.y - field_0x9fc.y;
            if (f < -300.0f) {
                field_0x688.OffAtSetBit();
                field_0x688.ResetAtHit();
                if (cLib_chaseF(&scale.x, 0.0f, 0.1f)) {
                    field_0x93f = 1;
                    return TRUE;
                }

                f32 x = scale.x;
                scale.y = x;
                scale.z = x;
                mpModel->setBaseScale(scale);
            } else {
                if (f > 95.0f) {
                    field_0x93f = 1;
                    return TRUE;
                }
                setArrowAt(current.pos.abs(mStartPos));
            }
        } else {
            f32 distance = current.pos.abs(mStartPos);
            if (distance > mFlyMax || fopAcM_GetGravity(this) < 0.0f) {
                if (mArrowType == 4) {
                    fopAcM_SetGravity(this, -5.0f);
                } else {
                    fopAcM_SetGravity(this, -2.0f);
                }
                speed.y += fopAcM_GetGravity(this);

                if (field_0x956 != 0) {
                    field_0x956--;
                } else {
                    field_0x93f = 1;
                    return TRUE;

                }
                shape_angle.x = speed.atan2sY_XZ();
                current.angle.x = -shape_angle.x;

                if (mArrowType != 4) {
                    mOutLengthRate = 95.0f / speed.abs();
                }
            }

            u32 sound_id;
            if (mArrowType == 2) {
                sound_id = Z2SE_ZELDA_ARROW_FLY;
            } else {
                if (fopAcM_GetParam(this) == 2) {
                    if (mArrowType == 1) {
                        sound_id = Z2SE_OBJ_ARROWBOMB_FLYGAIN;
                    } else {
                        sound_id = Z2SE_OBJ_ARROW_FLY_GAIN;
                    }
                } else if (mArrowType == 1) {
                    sound_id = Z2SE_OBJ_ARROWBOMB_FLY;
                } else {
                    sound_id = Z2SE_OBJ_ARROW_FLY_NORMAL;
                }
            }

            if (mArrowType != 4) {
                mSoundObjArrow.startLevelSound(sound_id, 0, mReverb);
            }

            setArrowAt(distance);
            setBlur();
        }
    }

    setNormalMatrix();
    setBombMoveEffect();

    return TRUE;
}

int daArrow_c::procReturnInit(int param_0) {
    setBlur();
    setBombMoveEffect();
    fopAcM_SetParam(this, 5);

    mProcFunc = &daArrow_c::procReturn;
    speedF = 0.0f;

    s16 rnd_y = cM_rndFX(8192.0f) + (current.angle.y + 0x8000);
    s16 sVar6;

    if (param_0 != 0) {
        sVar6 = 8192.0f - cM_rndF(4096.0f);
    } else {
        sVar6 = -cM_rndF(4096.0f);
    }

    f32 f = cM_rndF(15.0f) + 15.0f;
    speed.y = f * cM_ssin(sVar6);
    speed.x = (f * cM_ssin(rnd_y)) * cM_scos(sVar6);
    speed.z = (f * cM_scos(rnd_y)) * cM_scos(sVar6);

    field_0x954 = 11264.0f - cM_rndF(4096.0f);

    return TRUE;
}

int daArrow_c::procReturn() {
    setBombMoveEffect();

    speed.y -= 2.0f;
    current.pos += speed;

    shape_angle.x += field_0x954;

    cXyz pos = current.pos + speed * mOutLengthRate;
    field_0x56c.Set(&old.pos, &pos, this);
    decAlphaBlur();

    if(dComIfG_Bgsp().LineCross(&field_0x56c)) {
        if (mArrowType == 1 && field_0x943 == 0) {
            setBombArrowExplode(field_0x56c.GetCrossP());
            return TRUE;
        }

        cM3dGPla cStack_34;
        cXyz pos;
        dComIfG_Bgsp().GetTriPla(field_0x56c, &cStack_34);
        f32 speed_magnitude = speed.abs();
        C_VECReflect(&speed, cStack_34.GetNP(), &pos);
        speed.x = pos.x * speed_magnitude * 0.5f;
        speed.y = pos.y * speed_magnitude * 0.5f;
        speed.z = pos.z * speed_magnitude * 0.5f;
        field_0x954 = -(field_0x954 >> 1);

        if (dBgS_CheckBGroundPoly(field_0x56c)) {
            field_0x93d = 1;
        }
    } else {
        if ((field_0x93d != 0 && speed.y <= 0.0f) || mStartPos.y - 5000.0f > current.pos.y) {
            field_0x93f = 1;
            return TRUE;
        }
    }

    setNormalMatrix();

    return TRUE;
}

int daArrow_c::procBGStop() {
    if (field_0x944 != 0 || !dComIfG_Bgsp().ChkPolySafe(field_0x56c)) {
        field_0x93f = 1;
        return TRUE;
    }

    BOOL update_angle = FALSE;
    s16 angle = getVibAngle();

    if(field_0x952) {
        update_angle = TRUE;
    }
    decAlphaBlur();

    if (dComIfG_Bgsp().ChkMoveBG(field_0x56c)) {
        dComIfG_Bgsp().MoveBgTransPos(field_0x56c, true, &current.pos, &current.angle, &shape_angle);
        update_angle = TRUE;
    }

    if (update_angle) {
        shape_angle.x = current.angle.x + angle;
        setNormalMatrix();
    }

    checkReget();
    return TRUE;
}

int daArrow_c::procActorStop() {
    decAlphaBlur();

    fopAc_ac_c* actor = setStopActorMatrix();
    if (field_0x944 != 0 || actor == NULL) {
        field_0x93f = 1;
    } else {
        if (actor->group != 4) {
            checkReget();
        }
    }

    return TRUE;
}

int daArrow_c::procActorControllStop() {
    decAlphaBlur();
    return TRUE;
}

int daArrow_c::procSlingHitInit(cXyz* param_0, dCcD_GObjInf* param_1) {
    fopAc_ac_c* hit_ac;
    if (param_1 != NULL) {
        hit_ac = param_1->GetAtHitAc();
    } else {
        hit_ac = NULL;
    }

    if (!daAlink_c::notSwordHitVibActor(hit_ac)) {
        u16 hitmark;
        if (hit_ac != NULL && fopAcM_CheckStatus(hit_ac, 0x10000) != 0) {
            hitmark = 1;
        } else {
            hitmark = 9;
            if (hit_ac != NULL && param_1->GetAtHitGObj() != NULL) {
                if (param_1->GetAtHitGObj()->ChkTgNoSlingHitInfSet()) {
                    mDoAud_seStart(Z2SE_HIT_PACHINKO, param_0, 0x2d, mReverb);
                }
            }
        }
        dComIfGp_setHitMark(hitmark, hit_ac, param_0, &current.angle, NULL, 0);
    }

    setBlur();
    fopAcM_SetParam(this, 8);
    mProcFunc = &daArrow_c::procSlingHit;
    speedF = 0.0f;
    field_0x956 = 5;

    return TRUE;
}

int daArrow_c::procSlingHit() {
    decAlphaBlur();

    if (field_0x956 != 0) {
        field_0x956--;
    } else {
        field_0x93f = 1;
    }
    return TRUE;
}

int daArrow_c::execute() {
    field_0xa08.setActor();
    mReverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));

    if (field_0x945 == 0 && field_0x943 == 0) {
        field_0x9e4 = field_0x9d8;
        field_0x9d8 = field_0x9cc;
    }

    if (field_0x93f != 0) {
        setLightChargeEffect(1);
        fopAcM_delete(this);
        return TRUE;
    } else {
        if (field_0x94e == m_count) {
            field_0x944 = 1;
        }

        if (field_0x942 != 0) {
            field_0x942--;
        }

        (this->*mProcFunc)();

        if (mArrowType == 1 && field_0x93f == 0) {
            if (field_0x950 != 0) {
                field_0x950--;
            } else {
                f32 amp = cM_scos(current.angle.x) * 95.0f;
                cXyz pos(
                    current.pos.x + amp * cM_ssin(current.angle.y),
                    current.pos.y + cM_ssin(current.angle.x) * 95.0f,
                    current.pos.z + amp * cM_scos(current.angle.y)
                );
                setBombArrowExplode(&pos);
                return TRUE;
            }
        }

        if (mArrowType == 2) {
            setLightChargeEffect(0);
        }

        attention_info.position = current.pos;
        eyePos = current.pos;

        setRoomInfo();

        mSoundObjArrow.framework(0, mReverb);
    }

    return TRUE;
}

static int daArrow_execute(daArrow_c* i_this) {
    return i_this->execute();
}

int daArrow_c::draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);

    if (field_0x942 != 0 || field_0x93f != 0 || fopAcM_GetParam(this) == 8) {
        return TRUE;
    }
    
    static const GXColorS10 tmpColor = {0x00, 0x00, 0x00, 0x00};
    J3DGXColorS10 color = tmpColor;

    daAlink_c* link = daAlink_getAlinkActorClass();
    if (fopAcM_GetParam(this) == 0 && field_0x940 != 0) {
        setKeepMatrix();
        field_0x940 = 0;
    }

    if (mArrowType == 1) {
        if (field_0x943 != 0) {
            color.r = link->getFreezeR();
            color.g = link->getFreezeG();
            color.b = link->getFreezeB();
        } else {
            s16 explode_time = daAlink_getAlinkActorClass()->getBombExplodeTime();
            f32 r;

            if (field_0x950 > explode_time >> 1) {
                r = fabsf(cM_fsin((field_0x950 - (explode_time >> 1)) * M_PI / (explode_time >> 2)));
            } else if (field_0x950 > explode_time >> 2) {
                r = fabsf(cM_fsin((field_0x950 - (explode_time >> 2)) * M_PI / (explode_time >> 3)));
            } else {
                r = fabsf(cM_fsin((field_0x950 - (explode_time >> 3)) * M_PI / (explode_time >> 4)));
            }

            color.r = (u8)(r * 50.0f);
        }
        
        mpModel->getModelData()->getMaterialNodePointer(1)->setTevColor(1, &color);
    }

    if (field_0x943 != 0) {
        tevStr.TevColor.r = link->getFreezeR();
        tevStr.TevColor.g = link->getFreezeG();
        tevStr.TevColor.b = link->getFreezeB();
    }

    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    mDoExt_modelUpdateDL(mpModel);

    if (mArrowType == 1) {
        color.r = 0;
        color.g = 0;
        color.b = 0;
        mpModel->getModelData()->getMaterialNodePointer(1)->setTevColor(1, &color);
    }

    return TRUE;
}

static int daArrow_draw(daArrow_c* i_this) {
    return i_this->draw();
}

static void* daAlink_searchHorseZelda(fopAc_ac_c* param_0, void* param_1) {
    if (fopAcM_GetName(param_0) == PROC_HOZELDA) {
        return param_0;
    }

    return NULL;
}

cPhs__Step daArrow_c::create() {
    fopAcM_ct(this, daArrow_c);

    mArrowType = fopAcM_GetParamBit(this, 8, 8);
    fopAcM_SetParam(this, (u8)fopAcM_GetParam(this));
    
    if (!fopAcM_entrySolidHeap(this, daArrow_createHeap, mArrowType == 1 ? 0xea0 : 0x810)) {
        return cPhs_ERROR_e;
    }
    
    if (mArrowType == 2) {
        field_0xa08.setData(fopAcM_Search((fopAcIt_JudgeFunc)daAlink_searchHorseZelda, NULL));
        field_0x946 = 6;
        field_0x94c = 0x896e;
    } else if (mArrowType == 0) {
        field_0x946 = 1;
        field_0x94c = 0x3b8;
    } else if (mArrowType == 4) {
        field_0x946 = 1;
        field_0x94c = 0xa55;
    } else {
        field_0x946 = 0;
    }

    field_0x940 = 1;

    field_0x64c.Init(10, 0xff, this);
    field_0x688.Set(l_atCpsSrc);
    field_0x688.SetStts(&field_0x64c);
    field_0x688.SetAtHitCallback(daArrow_atHitCallBack);
    field_0x7cc.Set(l_coSphSrc);
    field_0x7cc.SetStts(&field_0x64c);

    daAlink_c* player = daAlink_getAlinkActorClass();
    if (mArrowType == 4) {
        setNormalMatrix();
        player->getArrowFlyData(&mFlyMax, &field_0x99c, 0);
        mOutLengthRate = 0.0f;
        arrowShooting();
        mProcFunc = &daArrow_c::procMove;
    } else {
        mProcFunc = &daArrow_c::procWait;
        setKeepMatrix();
    }

    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_SetMin(this, -6.0f, -6.0f, 0.0f);
    fopAcM_SetMax(this, 6.0f, 6.0f, 110.0f);

    setSmokePos();

    field_0x94e = -1; 
    field_0x956 = 300;
    field_0x952 = 50.0f - cM_rndF(20.0f);

    field_0x950 = player->getBombExplodeTime();
    field_0x5dc.OffFullGrp();
    field_0x5dc.OnWaterGrp();

    return cPhs_COMPLEATE_e;
}

static int daArrow_create(daArrow_c* i_this) {
    return i_this->create();
}

daArrow_c::~daArrow_c() {
    mSoundObjArrow.deleteObject();
    setLightChargeEffect(1);
}

static int daArrow_delete(daArrow_c* i_this) {
    i_this->~daArrow_c();
    return TRUE;
}

AUDIO_INSTANCES;
template<>
JAUSectionHeap* JASGlobalInstance<JAUSectionHeap>::sInstance;

static actor_method_class l_daArrowMethodTable = {
    (process_method_func)daArrow_create,
    (process_method_func)daArrow_delete,
    (process_method_func)daArrow_execute,
    NULL,
    (process_method_func)daArrow_draw,
};

actor_process_profile_definition g_profile_ARROW = {
  fpcLy_CURRENT_e,        // mLayerID
  9,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_ARROW,             // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daArrow_c),      // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  750,                    // mPriority
  &l_daArrowMethodTable,  // sub_method
  0x00060000,             // mStatus
  fopAc_UNK_GROUP_5_e,    // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
