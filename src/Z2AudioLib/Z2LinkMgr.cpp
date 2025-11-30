#include "Z2AudioLib/Z2LinkMgr.h"
#include "d/d_com_inf_game.h"

#if PLATFORM_WII || PLATFORM_SHIELD
#include "Z2AudioLib/Z2AudioCS.h"
#endif

void Z2CreatureLink::setLinkGroupInfo(u8) {}

void Z2CreatureLink::setLinkHp(s32 linkHp, s32) {
    mLinkHp = linkHp;
}

void Z2CreatureLink::setLinkSwordType(s32 swordType, s32 param_1) {
    mLinkSwordType = swordType;
    mFlag3 = param_1 == 1;

    if (mLinkSwordType != 0) {
        if (mLinkShieldType != 0 && !mFlag4) {
            mFlag5 = true;
            return;
        }
    }

    mFlag5 = false;
}

void Z2CreatureLink::setLinkShieldType(s32 shieldType, s32 param_1) {
    mLinkShieldType = shieldType;
    mFlag4 = param_1 == 1;

    if (mLinkSwordType != 0) {
        if (mLinkShieldType != 0 && !mFlag4) {
            mFlag5 = true;
            return;
        }
    }

    mFlag5 = false;
}

void Z2CreatureLink::setLinkBootsType(s32 bootsType) {
    mLinkBootsType = bootsType;

    if (Z2GetSceneMgr()->isSceneExist()) {
        startLinkSound(Z2SE_AL_CHANGE_BOOTS, 0, -1);
    }
}

Z2CreatureLink* Z2CreatureLink::mLinkPtr;

Z2CreatureLink::Z2CreatureLink() {
    mLinkState = 0;
    mLinkPtr = this;

    field_0xc1 = 0xFF;
    mWolfEyeOpen = false;
    mInWater = false;
    mMarkState = 0;
    mResumeAttack = false;

    setSoundStarter(this);
    mKantera.setSoundStarter(this);

    mLinkHp = 0;
    mLinkBootsType = 0;
    mKanteraState = 0;
    mLinkSwordType = 0;
    mLinkShieldType = 0;
    mFlag3 = false;
    mFlag4 = false;
    mFlag5 = false;
    mRiding = false;
    mMagnetized = false;
    mUsingIronBall = false;
}

inline Z2LinkSoundStarter::~Z2LinkSoundStarter() {}

Z2CreatureLink::~Z2CreatureLink() {
    deleteObject();
}

void Z2CreatureLink::deleteObject() {
    if (mKantera.isAlive()) {
        deleteKantera();
    }

    Z2Creature::deleteObject();
}

void Z2CreatureLink::init(Vec* animePosPtr, Vec* simplePosPtr, Vec* simple2PosPtr) {
    OS_REPORT("[Z2CreatureLink::init]");

    Z2Creature::init(animePosPtr, simplePosPtr, simple2PosPtr, 6, 1, 2);
    Z2GetAudience()->getLinkMic()->setPosPtr(getCurrentPos());

    field_0xc1 = 0xFF;
    mWolfEyeOpen = false;
    mInWater = false;
    mMarkState = 0;
    mResumeAttack = false;
    mKanteraState = 0;
    mRiding = false;
    mMagnetized = false;
    mUsingIronBall = false;
    field_0xb4 = *animePosPtr;
    mMoveSpeed = 0;
    mMovingTime = 0;
    mSinkDepth = -1;
}

void Z2CreatureLink::initKantera(Vec* posPtr) {
    mKanteraState = 0;
    mKantera.init(posPtr, 2);
}

void Z2CreatureLink::deleteKantera() {
    mKanteraState = 0;
    mKantera.deleteObject();
}

void Z2CreatureLink::setKanteraState(u8 state) {
    mKanteraState = state;
}

void Z2CreatureLink::framework(u32 mapinfo, s8 reverb) {
    Z2Creature::framework(mapinfo, reverb);

    if (mKantera.isAlive()) {
        mKantera.framework(mapinfo, reverb);
    }

    if (field_0xc1 != 0xFF) {
        field_0xc1++;
    }

    if (mWolfEyeOpen) {
        Z2GetSeMgr()->seStartLevel(Z2SE_WOLF_SENSE_OPEN, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    }

    if (isInWater()) {
        f32 var_f31 = Z2GetStatusMgr()->getCameraInWaterDepthRatio();
        if (var_f31 > 0.0f) {
            JAISoundHandle* var_r29 = startLinkSoundLevel(Z2SE_AL_UNDERWATER_BUBBLE, 0, -1);
            if (var_r29 != NULL && *var_r29) {
                (*var_r29)->getAuxiliary().moveVolume(var_f31, 0);
            }
        }
    }

    if (mMagnetized) {
        startLinkSoundLevel(Z2SE_AL_MAGNETIZED, 0, -1);
    }

    mMoveSpeed = 0;

    if (getCurrentPos() != NULL) {
        field_0xb4.y = getCurrentPos()->y;
        mMoveSpeed = sqrtf(VECSquareDistance(getCurrentPos(), &field_0xb4));
        field_0xb4.x = getCurrentPos()->x;
        field_0xb4.z = getCurrentPos()->z;
    }

    if (Z2GetStatusMgr()->getDemoStatus() == 10) {
        mMovingTime = 180;
    } else if (mMoveSpeed < 5) {
        if (mMovingTime < 180) {
            mMovingTime++;
        }
    } else if (mMovingTime != 0) {
        mMovingTime--;
    }
}

void Z2CreatureLink::setSinkDepth(s8 sinkDepth) {
    mSinkDepth = sinkDepth;
}

void Z2CreatureLink::setRiding(bool isRiding) {
    if (mRiding != isRiding) {
        if (isRiding) {
            OS_REPORT("[Z2CreatureLink::setRiding] true targetVol = 0.35f\n");
            Z2GetAudience()->setTargetVolume(0.35f, 0);
        } else {
            OS_REPORT("[Z2CreatureLink::setRiding] false targetVol = 0.0f\n");
            Z2GetAudience()->setTargetVolume(0.0f, 0);
        }

        mRiding = isRiding;
    }
}

void Z2CreatureLink::setMagnetized(bool isMagnetized) {
    mMagnetized = isMagnetized;
}

void Z2CreatureLink::setWolfEyeOpen(bool wolfEyeOpen) {
    mWolfEyeOpen = wolfEyeOpen;

    if (mWolfEyeOpen) {
        #if PLATFORM_WII || PLATFORM_SHIELD
        if (Z2GetSceneMgr()->isSceneExist()) {
            Z2AudioCS::start(0x55, 0);
        }
        #endif

        Z2GetSeMgr()->seStart(Z2SE_WOLF_SENSE_OPEN_START, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        Z2GetFxLineMgr()->setFxForceOff(true);
        Z2GetSoundObjMgr()->setGhostEnemyState(32);
    } else {
        #if PLATFORM_WII || PLATFORM_SHIELD
        if (Z2GetSceneMgr()->isSceneExist()) {
            Z2AudioCS::start(0x56, 0);
        }
        #endif

        Z2GetSeMgr()->seStart(Z2SE_WOLF_SENSE_CLOSE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        Z2GetFxLineMgr()->setFxForceOff(false);
        Z2GetSoundObjMgr()->setGhostEnemyState(16);
    }
}

Z2SoundHandlePool* Z2CreatureLink::startLinkSound(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    Z2SoundHandlePool* handle = startCreatureSound(soundID, mapinfo, reverb);

    switch (soundID) {
    case Z2SE_AL_DAMAGE_NORMAL:
    case Z2SE_AL_DAMAGE_LARGE:
    case Z2SE_AL_FREEZE:
        Z2GetSeqMgr()->onEnemyDamage();
        break;
    case Z2SE_LK_HS_SHOOT:
        mSoundObjAnime.stopSound(Z2SE_AL_HS_OPEN, 0);
        break;
    case Z2SE_AL_SINK_SAND_OUT:
        if (handle != NULL && *handle) {
            f32 volume = Z2Calc::linearTransform(mSinkDepth, 32.0f, 127.0f, 0.4f, 1.0f, false);
            f32 pitch = Z2Calc::linearTransform(mSinkDepth, 32.0f, 127.0f, 1.0f, 0.7f, false);
            
            (*handle)->getAuxiliary().moveVolume(volume, 0);
            (*handle)->getAuxiliary().movePitch(pitch, 0);
        }
        break;
    case Z2SE_AL_SINK_SNOW_OUT:
        if (handle != NULL && *handle) {
            f32 volume = Z2Calc::linearTransform(mSinkDepth, 0.0f, 23.0f, 0.5f, 1.0f, false);
            f32 pitch = Z2Calc::linearTransform(mSinkDepth, 0.0f, 23.0f, 1.0f, 0.7f, false);
            
            (*handle)->getAuxiliary().moveVolume(volume, 0);
            (*handle)->getAuxiliary().movePitch(pitch, 0);
        }
        break;
    }

    return handle;
}

JAISoundHandle* Z2CreatureLink::startLinkSoundLevel(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    #if PLATFORM_WII || PLATFORM_SHIELD
    if (Z2GetSceneMgr()->isSceneExist()) {
        int iVar3 = -1;
        switch((u32)soundID) {
        case Z2SE_AL_REEL_SLOW_LOOP:
            iVar3 = 0x16;
            break;
        case Z2SE_AL_REEL_FAST_LOOP:
            iVar3 = 0x17;
            break;
        case Z2SE_LK_HS_WIND_UP:
            iVar3 = 0x19;
            break;
        }
        
        if (iVar3 >= 0 && Z2AudioCS::startLevel(iVar3, 0) != 0)
        {
            return NULL;
        }
    }
    #endif

    if (soundID == Z2SE_AL_LIGHTNING_SW_GLOW && (Z2GetStatusMgr()->getDemoStatus() == 2 || !Z2GetSceneMgr()->isInGame())) {
        return NULL;
    }

    Z2SoundHandlePool* handle = startCreatureSoundLevel((u32)soundID, mapinfo, reverb);
    if (handle != NULL && *handle) {
        f32 volume = 1.0f;
        f32 pitch = 1.0f;
        int volumeMoveSteps = 0;

        switch (soundID) {
        case Z2SE_AL_SINK_SAND_LEVEL:
            if (mSinkDepth > 0) {
                volume = Z2Calc::linearTransform(mSinkDepth, 0.0f, 127.0f, 0.4f, 1.0f, true);
                pitch = Z2Calc::linearTransform(mSinkDepth, 0.0f, 127.0f, 1.0f, 0.7f, true);
            }
            break;
        case Z2SE_AL_SINK_SNOW_LEVEL:
            if (mSinkDepth > 0) {
                volume = Z2Calc::linearTransform(mSinkDepth, 0.0f, 23.0f, 0.7f, 1.0f, true);
                pitch = Z2Calc::linearTransform(mSinkDepth, 0.0f, 23.0f, 1.0f, 0.7f, true);
            }
            break;
        case Z2SE_AL_SPINNER_RIDE:
            pitch = Z2Calc::getParamByExp(mMoveSpeed, 0.0f, 30.0f, 0.3f, 0.8f, 1.0f, Z2Calc::CURVE_NEGATIVE);

            if (mMoveSpeed == 0 && mMovingTime > 120) {
                volume = Z2Calc::getParamByExp(mMovingTime, 180.0f, 120.0f, 0.6f, 0.0f, 0.4f, Z2Calc::CURVE_NEGATIVE);
            } else {
                volume = Z2Calc::getParamByExp(mMoveSpeed, 0.0f, 30.0f, 0.6f, 0.4f, 1.0f, Z2Calc::CURVE_NEGATIVE);
            }
            break;
        case Z2SE_AL_SPINNER_RAIL:
            pitch = Z2Calc::getParamByExp(mMoveSpeed, 0.0f, 80.0f, 0.3f, 0.8f, 1.2f, Z2Calc::CURVE_NEGATIVE);
            break;
        case Z2SE_AL_SNOBO_RIDE:
        case Z2SE_AL_SNOBO_BREAK:
                /* dSv_event_flag_c::F_0266 - Snowpeak Ruins - Snowpeak Ruins clear */
            if (dComIfGs_isEventBit((u16)dSv_event_flag_c::saveBitLabels[266])
                && Z2GetSeqMgr()->getSubBgmID() != Z2BGM_SNOW_BOARD) {
                volume = 0.0f;
                volumeMoveSteps = 0;
            } else if (mMoveSpeed == 0) {
                volume = 0.0f;
                volumeMoveSteps = 10;
            } else {
                volume = Z2Calc::getParamByExp(mMoveSpeed, 0.0f, 80.0f, 0.25f, 0.4f, 1.0f, Z2Calc::CURVE_NEGATIVE);
            }

            pitch = Z2Calc::getParamByExp(mMoveSpeed, 0.0f, 80.0f, 0.3f, 0.5f, 1.1f, Z2Calc::CURVE_NEGATIVE);
            break;
        case Z2SE_AL_LIGHTNING_SW_GLOW:
            f32 temp_f1_2 = (*handle)->getAuxiliary().transition_.volume_.targetValue_;
            if (Z2GetStatusMgr()->getDemoStatus() == 2 && temp_f1_2 != 0.3f) {
                volume = 0.3f;
                volumeMoveSteps = 45;
            } else if (temp_f1_2 != 1.0f) {
                volume = 1.0f;
                volumeMoveSteps = 45;
            }
            break;
        }

        (*handle)->getAuxiliary().moveVolume(volume, volumeMoveSteps);
        (*handle)->getAuxiliary().movePitch(pitch, 0);
    }

    return handle;
}

Z2SoundHandlePool* Z2CreatureLink::startLinkVoice(JAISoundID soundID, s8 reverb) {
    if (mLinkState == 1) {
        switch (soundID) {
        case Z2SE_WL_V_BREATH_WAIT:
        case Z2SE_WL_V_BREATH_TIRED:
        case Z2SE_WL_V_BREATH_SIT:
            if (Z2GetStatusMgr()->getDemoStatus() == 2 || !Z2GetSceneMgr()->isInGame()) {
                return NULL;
            }
            // fallthrough
        case Z2SE_WL_V_BREATH_WALK:
        case Z2SE_WL_V_BREATH_RUN:
            if (field_0xc1 < 5) {
                return NULL;
            }
            field_0xc1 = 0;
            break;
        case Z2SE_WL_V_BITE_ATTACK:
            if (mSoundObjSimple1.getHandleSoundID(Z2SE_WL_V_BITE_THROAT) != NULL) {
                return NULL;
            }
            break;
        }
    } else {
        if (soundID == Z2SE_AL_V_ATTACK_RUN) {
            if (field_0xc1 < 10) {
                return NULL;
            }
            field_0xc1 = 0;
        }

        if (mMarkState != 3 && Z2GetSoundObjMgr()->getEnemyNumNear() == 0 && !Z2GetSoundObjMgr()->isForceBattle())
        {
            switch (soundID) {
            case Z2SE_AL_V_ATTACK_S:
                soundID = Z2SE_AL_V_ATTACK_S_FREE;
                break;
            case Z2SE_AL_V_ATTACK_M:
            case Z2SE_AL_V_TATE_OSHI:
                soundID = Z2SE_AL_V_ATTACK_M_FREE;
                break;
            case Z2SE_AL_V_ATTACK_L:
                soundID = Z2SE_AL_V_ATTACK_L_FREE;
                break;
            case Z2SE_AL_V_KAITEN:
                soundID = Z2SE_AL_V_KAITEN_FREE;
                break;
            case Z2SE_AL_V_BACKTEN:
            case Z2SE_AL_V_SOTOMO_ROLL:
                soundID = Z2SE_AL_V_BACKTEN_FREE;
                break;
            case Z2SE_AL_V_ATTACK_RUN:
                soundID = Z2SE_AL_V_ATTACK_RUN_FREE;
                break;
            }
        } else {
            if (soundID == Z2SE_AL_V_JUMP_S) {
                soundID = Z2SE_AL_V_BACKTEN;
            }
        }

        if (isInWater()) {
            soundID = Z2SE_AL_V_UNDER_WATER;
        }
    }

    return startCreatureVoice(soundID, reverb);
}

Z2SoundHandlePool* Z2CreatureLink::startLinkVoiceLevel(JAISoundID soundID, s8 reverb) {
    if (mLinkState == 1) {
        switch (soundID) {
        case Z2SE_WL_V_ROAR:
            f32 volume = 0.0f;
            u8 port_data = 0;
            switch (mMarkState) {
            case 0:
                volume = 1.0f;
                port_data = 2;
                break;
            case 1:
                volume = 0.3f;
                port_data = 1;
                break;
            case 2:
                volume = 0.5f;
                port_data = 1;
                break;
            case 3:
                volume = 0.8f;
                port_data = 2;
                break;
            }

            Z2SoundHandlePool* handle = startCreatureVoiceLevel(Z2SE_WL_V_ROAR, reverb);
            if (handle != NULL && *handle) {
                (*handle)->getAuxiliary().moveVolume(volume, 0);
                Z2GetSoundStarter()->setPortData(handle, 8, port_data, -1);
            }
            return handle;
        }
    }
    
    return startCreatureVoiceLevel(soundID, reverb);
}

void Z2CreatureLink::startLinkSwordSound(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    u32 sound_ID = soundID;  // fakematch

    switch (sound_ID) {
    case Z2SE_SWORD_POWER_COME:
        #if PLATFORM_WII || PLATFORM_SHIELD
        if (Z2GetSceneMgr()->isSceneExist()) {
            Z2AudioCS::startLevel(0x2F, 0);
        }
        #endif
        startCreatureExtraSoundLevel(sound_ID, mapinfo, reverb);
        break;
    case Z2SE_WOLF_POWER_COME:
        #if PLATFORM_WII || PLATFORM_SHIELD
        if (Z2GetSceneMgr()->isSceneExist()) {
            Z2AudioCS::startLevel(0x6B, 0);
        }
        #endif
        startCreatureExtraSoundLevel(sound_ID, mapinfo, reverb);
        break;
    default:
        startCreatureExtraSound(sound_ID, mapinfo, reverb);
        break;
    }
}

Z2SoundHandlePool* Z2CreatureLink::startCollisionSE(u32 hitID, u32 mapinfo) {
    switch (mapinfo) {
    case 0x28:
    case 0x29:
    case 0x2A:
        Z2GetSeqMgr()->onEnemyDamage();
        break;
    case 0x20:
        if (hitID == Z2SE_HIT_SWORD_STAB) {
            Z2GetSeqMgr()->setBattleLastHit(0x14);
        }
        break;
    }

    return mSoundObjAnime.startCollisionSE(hitID, mapinfo, NULL);
}

Z2SoundHandlePool* Z2CreatureLink::startHitItemSE(u32 soundID, u32 mapinfo, Z2SoundObjBase* other, f32 speed) {
    if (other == NULL) {
        other = &mSoundObjSimple2;
    }

    if (soundID == Z2SE_HIT_SHIELD_ATTACK) {
        return mSoundObjAnime.startCollisionSE(soundID, mapinfo, NULL);
    }

    Z2SoundHandlePool* handle = other->startCollisionSE(soundID, mapinfo, NULL);
    if (speed >= 0.0f && handle != NULL && *handle) {
        OS_REPORT("[Z2CreatureLink::startHitItemSE] speed:%.3f\n", speed);
        if (speed >= 100.0f) {
            speed = 100.0f;
        }

        f32 volume = speed / 100.0f;
        (*handle)->getAuxiliary().moveVolume(volume, 0);
    }

    return handle;
}

void Z2CreatureLink::setResumeAttack(bool resumeAttack) {
    mResumeAttack = resumeAttack;
}

Z2LinkSoundStarter::Z2LinkSoundStarter() : Z2SoundStarter(false) {}

bool Z2LinkSoundStarter::startSound(JAISoundID soundID, JAISoundHandle* handle,
                                    const JGeometry::TVec3<f32>* posPtr, u32 mapinfo,
                                    f32 fxMix, f32 pitch, f32 volume, f32 pan,
                                    f32 dolby, u32 moveSteps) {
    bool bVar2 = false;
    bool bVar3 = false;

    if (Z2GetLink()->isInWater() && Z2GetStatusMgr()->getCameraInWaterDepthRatio() > 0.0f) {
        switch (soundID) {
        case Z2SE_AL_SWIM:
        case Z2SE_AL_DIVE_SWIM:
            soundID = Z2SE_AL_SWIM_UNDERWATER;
            break;
        case Z2SE_AL_SWORD_SWING_S:
        case Z2SE_AL_FINISH_SWING1:
        case Z2SE_AL_FINISH_SWING2:
        case Z2SE_AL_WAIT_SWORD_SWING:
            soundID = Z2SE_AL_WATER_STROKE_S;
            break;
        case Z2SE_AL_SWORD_SWING_L:
            soundID = Z2SE_AL_WATER_STROKE_L;
            break;
        case Z2SE_AL_SWORD_THRUST:
            soundID = Z2SE_AL_WATER_STROKE_FAST;
            break;
        case Z2SE_AL_KAITENGIRI:
            soundID = Z2SE_AL_WATER_STROKE_CIRCLE;
            break;
        }
    }

    if (soundID.id_.info.type.parts.groupID == 3 && mapinfo > 26) {
        if (mapinfo == 44) {
            mapinfo = 23;
        } else {
            JUT_WARN_DEVICE(929, 1, "%s", "mapinfo > 26\n");
            mapinfo = 0;
        }
    }

    switch (soundID) {
    case Z2SE_AL_INTO_WATER:
    case Z2SE_AL_DIVE_INTO_WATER:
    case Z2SE_WL_INTO_WATER:
        if (Z2GetLink()->mSoundObjSimple1.isActive()) {
            Z2GetLink()->mSoundObjSimple1.stopAllSounds(1);
        }
        break;
    case Z2SE_FN_WALK_DUMMY:
    case Z2SE_FN_LADDER_CLIMB:
    case Z2SE_FN_LADDER_DOWN:
        bVar2 = true;
        // fallthrough
    case Z2SE_GORON_FOOTNOTE:
    case Z2SE_FN_GOB_JUMP:
        if (Z2GetLink()->mLinkBootsType == 1) {
            soundID = Z2SE_FN_WALK_HEAVY;
            if (Z2GetLink()->mMagnetized) {
                mapinfo = 127;
            }
        }
        bVar3 = true;
        break;
    case Z2SE_FN_JUMP_DUMMY:
        bVar2 = true;
        bVar3 = true;
        if (Z2GetLink()->mLinkBootsType == 1) {
            soundID = Z2SE_FN_JUMP_HEAVY;
            if (Z2GetLink()->mMagnetized) {
                mapinfo = 127;
            }
        }
        if (Z2GetLink()->mSoundObjSimple1.getHandleSoundID(Z2SE_AL_V_FALL) != NULL) {
            Z2GetLink()->startLinkVoice(Z2SE_AL_V_FOOT_MISS, -1);
        }
        break;
    case Z2SE_FN_BOUND_DUMMY:
    case Z2SE_FN_HAND_DUMMY:
        bVar2 = true;
        break;
    case Z2SE_BODY_FALL_DOWN:
        if (Z2GetLink()->mSoundObjSimple1.getHandleSoundID(Z2SE_AL_V_FALL) != NULL) {
            Z2GetLink()->startLinkVoice(Z2SE_AL_V_LANDING_FAIL, -1);
        }
        break;
    case Z2SE_OBJ_ARROW_SHOT_NORMAL:
    case Z2SE_OBJ_ARROW_SHOT_GAIN:
    case Z2SE_OBJ_ARROWBOMB_SHOT:
    case Z2SE_OBJ_ARROWBOMB_SHOTGAIN:
        Z2GetLink()->mSoundObjAnime.stopSound(Z2SE_AL_DRAW_BOW, 0);
        break;
    }

    if (Z2GetLink()->mSinkDepth > 0) {
        switch (soundID) {
        case Z2SE_FN_WALK_DUMMY:
        case Z2SE_FN_JUMP_DUMMY:
        case Z2SE_FN_BOUND_DUMMY:
        case Z2SE_FN_HAND_DUMMY:
        case Z2SE_FN_WALK_HEAVY:
        case Z2SE_FN_JUMP_HEAVY:
        case Z2SE_WL_WALK_L_DUMMY:
        case Z2SE_WL_WALK_R_DUMMY:
        case Z2SE_WL_RUN_L_DUMMY:
        case Z2SE_WL_RUN_R_DUMMY:
            if (mapinfo == 3) {
                pitch *= Z2Calc::linearTransform(Z2GetLink()->mSinkDepth,
                                                   0.0f, 127.0f, 1.0f, 0.5f, true);
            } else if (mapinfo == 25) {
                pitch *= Z2Calc::linearTransform(Z2GetLink()->mSinkDepth,
                                                   0.0f, 23.0f, 1.0f, 0.7f, true);
            }
            break;
        }
    }

    bool ret = Z2SoundStarter::startSound(soundID, handle, posPtr, mapinfo, fxMix, pitch,
                                          volume, pan, dolby, moveSteps);

    if (bVar2 && Z2GetSceneMgr()->getDemoSeWaveNum() != 114) {
        if (Z2GetLink()->mFlag5) {
            Z2GetLink()->startLinkSound(Z2SE_AL_SWORD_SHIELD_ADD, 0, fxMix * 127.0f);
        }

        if (Z2GetLink()->mUsingIronBall) {
            Z2GetLink()->startLinkSound(Z2SE_AL_IB_CHAIN_ADD, 0, fxMix * 127.0f);
        }

        if (Z2GetLink()->mKanteraState != 0) {
            Z2SoundHandlePool* handle =
                Z2GetLink()->mKantera.startSound(Z2SE_AL_KANTERA_SWING_ADD, 0, fxMix * 127.0f);
            if (Z2GetLink()->mKanteraState == 2 && handle != NULL && *handle) {
                (*handle)->getAuxiliary().moveVolume(0.6f, 0);
                (*handle)->getAuxiliary().movePitch(0.9f, 0);
            }
        }
    }

    if (bVar3) {
        if (Z2GetLink()->getLinkState() == 4) {
            Z2GetLink()->startLinkSound(Z2SE_FN_ARMER_LIGHT_ADD, 0, fxMix * 127.0f);
        } else if (Z2GetLink()->getLinkState() == 5) {
            Z2GetLink()->startLinkSound(Z2SE_FN_ARMER_HEAVY_ADD, 0, fxMix * 127.0f);
        }
    }

    return ret;
}

Z2CreatureRide::Z2CreatureRide() : Z2RideSoundStarter(this) {
    mLinkRiding = false;

    setSoundStarter(this);
}

Z2CreatureRide::~Z2CreatureRide() {
    deleteObject();
}

void Z2CreatureRide::deleteObject() {
    Z2Creature::deleteObject();
}

void Z2CreatureRide::init(Vec* animePosPtr, Vec* simplePosPtr, u8 animeHandleNum, u8 simpleHandleNum) {
    Z2Creature::init(animePosPtr, simplePosPtr, animeHandleNum, simpleHandleNum);
}

void Z2CreatureRide::setLinkRiding(bool isRiding) {
    mLinkRiding = isRiding;
}

Z2RideSoundStarter::Z2RideSoundStarter(Z2CreatureRide* ride) : Z2SoundStarter(false) {
    mRide = ride;
    JUT_ASSERT(1294, ride);
}

bool Z2RideSoundStarter::startSound(JAISoundID soundID, JAISoundHandle* handle,
                                    const JGeometry::TVec3<f32>* posPtr, u32 mapinfo,
                                    f32 fxMix, f32 pitch, f32 volume, f32 pan,
                                    f32 dolby, u32 moveSteps) {
    if (!Z2GetSceneMgr()->isInGame()) {
        return false;
    }

    u8 uVar3 = 0;
    switch (soundID) {
    case Z2SE_HS_WALK_L_DUMMY:
    case Z2SE_HS_WALK_R_DUMMY:
    case Z2SE_HS_TROT_L_DUMMY:
    case Z2SE_HS_TROT_R_DUMMY:
    case Z2SE_HS_GALLOP_L_DUMMY:
        uVar3 = 1;
        break;
    case Z2SE_WB_FRONT_L_DUMMY:
    case Z2SE_WB_FRONT_R_DUMMY:
    case Z2SE_WB_REAR_L_DUMMY:
        uVar3 = 2;
        break;
    }

    bool ret = Z2SoundStarter::startSound((u32)soundID, handle, posPtr, mapinfo, fxMix, pitch,
                                          volume, pan, dolby, moveSteps);

    if (mRide->isLinkRiding() && uVar3 != 0) {
        if (Z2GetLink()->mFlag5) {
            Z2GetLink()->startLinkSound(Z2SE_AL_SWORD_SHIELD_ADD, 0, fxMix * 127.0f);
        }

        if (Z2GetLink()->mKanteraState != 0) {
            Z2SoundHandlePool* handle =
                Z2GetLink()->mKantera.startSound(Z2SE_AL_KANTERA_SWING_ADD, 0, fxMix * 127.0f);
            if (Z2GetLink()->mKanteraState == 2 && handle != NULL && *handle) {
                (*handle)->getAuxiliary().moveVolume(0.6f, 0);
                (*handle)->getAuxiliary().movePitch(0.9f, 0);
            }
        }

        switch (uVar3) {
        case 1:
            Z2GetLink()->startLinkSound(Z2SE_AL_HARNESS_ADD_HS, 0, fxMix * 127.0f);
            break;
        case 2:
            Z2GetLink()->startLinkSound(Z2SE_AL_HARNESS_ADD_WB, 0, fxMix * 127.0f);
            break;
        }
    }

    return ret;
}

Z2SoundObjCoach::Z2SoundObjCoach() {
    field_0x20 = 0.0f;
}

void Z2SoundObjCoach::init(Vec* posPtr, u8 handleNum) {
    Z2SoundObjBase::init(posPtr, handleNum);
}

Z2SoundHandlePool* Z2SoundObjCoach::startWheelSound(f32 param_0) {
    Z2SoundHandlePool* handle = NULL;

    if (!(param_0 < 2.0f)) {
        if (param_0 < 35.0f) {
            handle = startLevelSound(Z2SE_COACH_WHEEL_ROLL_A, 0, -1);
            if (handle != NULL && *handle) {
                f32 pitch = Z2Calc::getParamByExp(param_0, 0.0f, 35.0f, 0.25f, 0.88f, 1.25f,
                                                  Z2Calc::CURVE_POSITIVE);
                (*handle)->getAuxiliary().movePitch(pitch, 0);
                f32 volume = Z2Calc::getParamByExp(param_0, 0.0f, 35.0f, 0.3f, 0.7f, 1.0f,
                                                   Z2Calc::CURVE_POSITIVE);
                (*handle)->getAuxiliary().moveVolume(volume, 0);
            }
        } else {
            handle = startLevelSound(Z2SE_COACH_WHEEL_ROLL_B, 0, -1);
            if (handle != NULL && *handle) {
                f32 pitch = Z2Calc::getParamByExp(param_0, 35.0f, 50.0f, 0.25f, 0.88f, 1.25f,
                                                  Z2Calc::CURVE_POSITIVE);
                (*handle)->getAuxiliary().movePitch(pitch, 0);
                f32 volume = Z2Calc::getParamByExp(param_0, 35.0f, 50.0f, 0.3f, 0.8f, 1.0f,
                                                   Z2Calc::CURVE_POSITIVE);
                (*handle)->getAuxiliary().moveVolume(volume, 0);
            }
        }
    }

    field_0x20 = param_0;
    return handle;
}

Z2SoundHandlePool* Z2SoundObjCoach::startFireSound(u16 param_0) {
    Z2SoundHandlePool* handle = startLevelSound(Z2SE_COACH_FIRE, 0, -1);
    
    if (handle != NULL && *handle) {
        f32 pitch = Z2Calc::getParamByExp(param_0, 0.0f, 1500.0f, 0.2f, 0.85f, 1.25f,
                                          Z2Calc::CURVE_POSITIVE);
        (*handle)->getAuxiliary().movePitch(pitch, 0);
        f32 volume = Z2Calc::getParamByExp(param_0, 0.0f, 1500.0f, 0.2f, 0.6f, 1.0f,
                                           Z2Calc::CURVE_POSITIVE);
        (*handle)->getAuxiliary().moveVolume(volume, 0);
    }

    return handle;
}

Z2SoundHandlePool* Z2SoundObjCoach::startSound(JAISoundID soundID, u32 mapinfo, s8 reverb) {
    Z2SoundHandlePool* handle = Z2SoundObjBase::startSound(soundID, mapinfo, reverb);

    if (handle != NULL && *handle && soundID == Z2SE_COACH_SHAKE) {
        f32 pitch = Z2Calc::getParamByExp(field_0x20, 0.0f, 50.0f, 0.2f, 0.85f, 1.25f,
                                          Z2Calc::CURVE_POSITIVE);
        (*handle)->getAuxiliary().movePitch(pitch, 0);
        f32 volume = Z2Calc::getParamByExp(field_0x20, 0.0f, 50.0f, 0.2f, 0.8f, 1.0f,
                                           Z2Calc::CURVE_POSITIVE);
        (*handle)->getAuxiliary().moveVolume(volume, 0);
    }

    return handle;
}
