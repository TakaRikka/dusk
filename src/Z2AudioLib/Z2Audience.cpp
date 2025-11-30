#include "Z2AudioLib/Z2Audience.h"
#include "Z2AudioLib/Z2SoundInfo.h"
#include "Z2AudioLib/Z2Calc.h"
#include "Z2AudioLib/Z2Param.h"
#include "JSystem/JAudio2/JAISound.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JMath/JMATrigonometric.h"
#include "JSystem/JGeometry.h"
#include "JSystem/J3DU/J3DUD.h"

void Z2AudibleAbsPos::calc(const JGeometry::TVec3<f32>& pos) {
    velocity_.sub(pos, field_0x0);
    field_0x0.set(pos);
}

void Z2AudibleAbsPos::init(JGeometry::TVec3<f32>* param_0, const JGeometry::TVec3<f32>& param_1,
                           const JGeometry::TVec3<f32>* param_2) {
    param_0->set(param_1);

    if (param_2 != NULL) {
        field_0x0.set(*param_2);
        velocity_.sub(param_1, field_0x0);
    } else {
        field_0x0.set(param_1);
        velocity_.zero();
    }
}

Z2Audible::Z2Audible(const JGeometry::TVec3<f32>& pos, const JGeometry::TVec3<f32>* param_1,
                     u32 channel, bool param_3) {
    mParam.field_0x0.raw = 0xFFFFFFFF;
    mAbsPos.init(&mPos, pos, param_1);

    for (int i = 0; i < 1; i++) {
        if ((channel & (1 << i)) == 0) {
            mChannel[i].init();
        }
    }

    for (int i = 0; i < 1; i = i + 1) {
        field_0x64[i] = 0.0f;
    }
}

Z2AudibleChannel::Z2AudibleChannel() {}

void Z2Audible::calc() {
    if (isDoppler()) {
        mAbsPos.calc(mPos);
    }
}

JASSoundParams* Z2Audible::getOuterParams(int index) {
    JUT_ASSERT(80, index >= 0);
    JUT_ASSERT(81, index < 1);
    return &mChannel[index].mParams;
}

void Z2Audible::setOuterParams(const JASSoundParams& outParams, const JASSoundParams& inParams,
                               int index) {
    JUT_ASSERT(89, index >= 0);
    JUT_ASSERT(90, index < 1);

    Z2AudibleChannel* channel = &mChannel[index];
    u8 iStack_94 = (mParam.field_0x0.half.f1 & 0xf00) >> 8;
    if (iStack_94 == 0) {
        channel->mParams.combine(outParams, inParams);
        return;
    }
    
    if (channel->field_0x28 < 0.0f) {
        channel->field_0x28 = 1.0f;
        channel->mPan = inParams.mPan;
        channel->mDolby = inParams.mDolby;
        channel->mParams.combine(outParams, inParams);
        return;
    } 

    JASSoundParams params;
    params.mFxMix = inParams.mFxMix;
    params.mPitch = inParams.mPitch;

    JGeometry::TVec2<f32> local_b8;
    local_b8.x = inParams.mPan - channel->mPan;
    local_b8.y = inParams.mDolby - channel->mDolby;

    f32 dVar9 = inParams.mVolume * local_b8.length();
    f32 dVar10 = 1.0f;
    f32 dVar12 = Z2Calc::linearTransform(iStack_94, 0.0f, 15.0f, 1.0f, 0.3f, true);
    if (inParams.mVolume > dVar12 && dVar9 > 0.001f) {
        dVar10 = Z2Calc::getParamByExp(dVar9, 0.3f, 0.001f, 0.1f, dVar12, 1.0f,
                                       Z2Calc::CURVE_POSITIVE);
        if (dVar10 < 0.0f) {
            dVar10 = 0.0f;
        }
    }

    f32 dVar15 = dVar10 - channel->field_0x28;
    f32 dVar11 = 1.0f;
    if (dVar15 > 0.0f) {
        dVar11 = Z2Calc::getParamByExp(J3DUD::JMAAbs(dVar15), 1.0f, 0.0f, 0.1f, 0.1f, 0.5f, Z2Calc::CURVE_POSITIVE);
    } else {
        dVar11 = Z2Calc::getParamByExp(J3DUD::JMAAbs(dVar15), -1.0f, 0.0f, 0.1f, 1.0f / 30.0f, 0.5f, Z2Calc::CURVE_POSITIVE);
    }

    dVar10 = channel->field_0x28 + (dVar15 * dVar11);
    if (dVar10 < 0.0f) {
        dVar10 = 0.0f;
    } else if (dVar10 > 1.0f) {
        dVar10 = 1.0f;
    }

    params.mVolume = inParams.mVolume * dVar10;
    channel->field_0x28 = dVar10;

    f32 dVar13 = Z2Calc::getParamByExp(params.mVolume * J3DUD::JMAAbs(local_b8.x), 1.0f, 0.0f, 0.1f,
        1.0f / 30.0f, 1.0 / 3.0f, Z2Calc::CURVE_POSITIVE);
    f32 dVar14 = Z2Calc::getParamByExp(params.mVolume * J3DUD::JMAAbs(local_b8.y), 1.0f,
        0.0f, 0.1f,
        1.0f / 30.0f, 1.0f / 3.0f, Z2Calc::CURVE_POSITIVE);
    if (dVar13 > 1.0f / 3.0f) {
        dVar13 = 1.0f / 3.0f;
    }
    if (dVar14 > 1.0f / 3.0f) {
        dVar14 = 1.0f / 3.0f;
    }

    params.mPan = channel->mPan + (local_b8.x * dVar13);
    params.mDolby = channel->mDolby + (local_b8.y * dVar14);
    params.clamp();

    channel->mParams.combine(outParams, params);
    channel->mPan = params.mPan;
    channel->mDolby = params.mDolby;
}

Z2AudibleChannel* Z2Audible::getChannel(int index) {
    JUT_ASSERT(220, index >= 0);
    JUT_ASSERT(221, index < 1);
    return &mChannel[index];
}

u32 Z2Audible::getDistVolBit() {
    u16 uVar1 = getAudibleParam()->field_0x0.half.f1;
    if (uVar1 != 0) {
        if ((uVar1 & 7) != 0) {
            return uVar1 & 7;
        }
        if ((uVar1 & 0x70) != 0) {
            return ((int)(uVar1 & 0x70) >> 4) + 7;
        }
    }

    return 0;
}

Z2Audience3DSetting::Z2Audience3DSetting() {
    mVolumeDistInit = false;
    mDolbyDistInit = false;
    init();
}

void Z2Audience3DSetting::init() {
    field_0x40 = Z2Param::MIN_DISTANCE_VOLUME;
    field_0x48 = Z2Param::DOLBY_BEHIND_DISTANCE_MAX;
    field_0x44 = Z2Param::DOLBY_FLONT_DISTANCE_MAX;
    field_0x54 = Z2Param::DISTANCE_FX_PARAM;
    field_0x50 = 0.0f;
    field_0x4c = Z2Param::DOLBY_CENTER_VALUE;
    field_0x58 = 0.5f;
    field_0x64 = 0xff;
    field_0x5c = Z2Param::SONIC_SPEED;
    field_0x60 = 1.5f;

    initVolumeDist();
    initDolbyDist();
}

void Z2Audience3DSetting::initVolumeDist() {
    if (mVolumeDistInit) return;

    field_0x0[0] = Z2Param::DISTANCE_MAX;
    field_0x3c = Z2Param::MAX_VOLUME_DISTANCE;

    calcVolumeFactorAll();
    calcPriorityFactorAll();
    calcFxMixFactorAll();

    mVolumeDistInit = true;
}

static f32 cNearFarRatio = Z2Param::MAX_VOLUME_DISTANCE / Z2Param::DISTANCE_MAX;

void Z2Audience3DSetting::updateVolumeDist(f32 param_0) {
    field_0x0[0] = param_0;

    if (param_0 > Z2Param::DISTANCE_MAX) {
        field_0x3c = cNearFarRatio * param_0;
    } else {
        field_0x3c = Z2Param::MAX_VOLUME_DISTANCE;
    }

    calcVolumeFactorAll();
    calcPriorityFactorAll();
    calcFxMixFactorAll();

    mVolumeDistInit = false;
}

void Z2Audience3DSetting::initDolbyDist() {
    if (!mDolbyDistInit) {
        field_0x44 = Z2Param::DOLBY_FLONT_DISTANCE_MAX;
        field_0x48 = Z2Param::DOLBY_BEHIND_DISTANCE_MAX;
        field_0x68 = -field_0x4c / field_0x44;
        field_0x6c = (1.0f - field_0x4c) / field_0x48;
        mDolbyDistInit = true;
    }
}

void Z2Audience3DSetting::updateDolbyDist(f32 param_0, f32 param_1) {
    if (param_0 > -1.0f * Z2Param::DOLBY_FLONT_DISTANCE_MAX) {
        field_0x44 = -1.0f * param_0;
    } else {
        field_0x44 = Z2Param::DOLBY_FLONT_DISTANCE_MAX;
    }

    if (param_1 < 2.0f * field_0x44) {
        param_1 = 2.0f * field_0x44;
    }

    if (param_1 > Z2Param::DOLBY_BEHIND_DISTANCE_MAX) {
        field_0x48 = param_1;
    } else {
        field_0x48 = Z2Param::DOLBY_BEHIND_DISTANCE_MAX;
    }

    mDolbyDistInit = false;
}

Z2AudioCamera::Z2AudioCamera() {
    mFovySin = 1.0f;
    mVolCenterZ = 0.0f;
    mTargetVolume = 0.0f;
    mDolbyCenterZ = 0.0f;
    mCamDist = 0.0f;
    field_0x68 = 0.0f;
    field_0x6c = 0.0f;
    mSetMainCamera = 0;
}

void Z2AudioCamera::init() {
    JGeometry::TPosition3f32 aTStack_38;
    aTStack_38.identity();
    JGeometry::TVec3<f32> VStack_44;
    VStack_44.set(100000.0f, 100000.0f, 100000.0f);
    setCameraState(aTStack_38, *(Vec*)&VStack_44, true);
}

void Z2AudioCamera::setCameraState(f32 const (*param_0)[4], Vec& pos, bool param_2) {
    field_0x0.set(param_0);
    if (param_2) {
        mPos.set(pos);
        field_0x48.set(mPos);
        mVel.zero();
    } else {
        field_0x48.set(mPos);
        mPos.set(pos);
        mVel.sub(mPos, field_0x48);
    }
}

void Z2AudioCamera::setCameraState(f32 (*param_0)[4], Vec& pos, Vec& param_2, f32 param_3,
                                   f32 param_4, bool param_5, bool param_6) {
    JGeometry::TVec3<f32> aTStack_c0;
    VECSubtract(&param_2, &pos, (Vec*)&aTStack_c0);
    mCamDist = aTStack_c0.length();

    f32 dVar10 = 0.5f * param_3;
    mFovySin = JMASinDegree(dVar10);
    f32 fVar8 = JMACosDegree(dVar10);
    field_0x6c = mFovySin / fVar8;
    field_0x68 = field_0x6c * param_4;

    f32 dVar11;
    if (param_3 >= 50.0f) {
        mVolCenterZ = 0.0f;
        dVar11 = mCamDist;
    } else {
        dVar11 = 2.144507f * (mCamDist * field_0x6c);
        mVolCenterZ = mCamDist - dVar11;
        if (mVolCenterZ < JGeometry::TUtil<f32>::epsilon()) {
            mVolCenterZ = 0.0f;
        }
    }

    if (mSetMainCamera && mTargetVolume > 0.0f) {
        if (dVar11 > (Z2Param::DISTANCE_MAX * (1.0f - mTargetVolume))) {
            Z2GetAudience()->getSetting()->updateVolumeDist(dVar11 / (1.0f - mTargetVolume));
        } else {
            Z2GetAudience()->getSetting()->initVolumeDist();
        }
    }

    if (param_5 || aTStack_c0.y >= 0.0f) {
        mDolbyCenterZ = -100.0f;
        if (mSetMainCamera) {
            Z2GetAudience()->getSetting()->updateDolbyDist(mCamDist, mCamDist);
        }
    } else {
        Vec aTStack_cc = {param_0[0][0], param_0[0][1], param_0[0][2]};
        Mtx rotMtx;
        MTXRotAxisRad(rotMtx, &aTStack_cc, 0.017453292f * (-1.0f * dVar10));
        JGeometry::TVec3<f32> aTStack_d8;
        MTXMultVec(rotMtx, aTStack_c0, aTStack_d8);
        aTStack_d8.scale(aTStack_c0.y / aTStack_d8.y);
        mDolbyCenterZ = fVar8 * aTStack_d8.length();
        if (mSetMainCamera) {
            Z2GetAudience()->getSetting()->updateDolbyDist(mCamDist - mDolbyCenterZ, mCamDist + mDolbyCenterZ);
        }
    }

    setCameraState(param_0, pos, param_6);
}

void Z2AudioCamera::convertAbsToRel(Z2Audible* audible, int channelNum) {
    JUT_ASSERT(508, audible);
    JUT_ASSERT(509, channelNum >= 0);
    JUT_ASSERT(510, channelNum < 1);

    Z2AudibleChannel* channel = audible->getChannel(channelNum);
    if (channel == NULL) {
        return;
    }

    Z2AudibleRelPos* relPos = &channel->field_0x14;
    MTXMultVec(field_0x0, (Vec*)&audible->getPos(), (Vec*)&relPos->field_0x00);
    relPos->field_0xC = relPos->field_0x00.length();

    JGeometry::TVec3<f32> aTStack_38(relPos->field_0x00);
    aTStack_38.z += mVolCenterZ;
    relPos->field_0x10 = aTStack_38.length();
}

bool Z2AudioCamera::convertAbsToRel(Vec& src, Vec* dst) const {
    MTXMultVec(field_0x0, &src, dst);
    return isInSight(*dst);
}

bool Z2AudioCamera::isInSight(Vec& param_0) const {
    if (param_0.z >= 0.0f) {
        return false;
    }

    if (J3DUD::JMAAbs(param_0.x) > field_0x68 * J3DUD::JMAAbs(param_0.z)) {
        return false;
    }

    if (J3DUD::JMAAbs(param_0.y) > field_0x6c * J3DUD::JMAAbs(param_0.z)) {
        return false;
    }

    return true;
}

Z2SpotMic::Z2SpotMic() {
    field_0x0 = 200.0f;
    field_0x4 = 400.0f;
    field_0x8 = 0.0f;
    field_0xc = 0.2f;
    mPosPtr = NULL;
    mIgnoreIfOut = true;
    mMicOn = true;
    field_0x1c = 255.0f / (field_0x4 - field_0x0);

    for (int i = 0; i < 1; i++) {
        clearMicState(i);
    }
}

void Z2SpotMic::clearMicState(int camID) {
    field_0x10[camID] = NULL;
    field_0x18[camID] = field_0xc;
    field_0x26[camID] = 0;
    calcVolumeFactor(camID);
}

void Z2SpotMic::calcVolumeFactor(int camID) {
    field_0x20[camID] = (field_0x8 - field_0x18[camID]) / (field_0x4 - field_0x0);
}

void Z2SpotMic::setMicState(Z2AudioCamera* camera, int camID) {
    JUT_ASSERT(622, camID >= 0);
    JUT_ASSERT(623, camID < 1);

    if (mMicOn && mPosPtr != 0 && camera != NULL) {
        clearMicState(camID);
        JGeometry::TVec3<f32> aVStack_58;
        field_0x26[camID] = camera->convertAbsToRel(*mPosPtr, (Vec*)&aVStack_58);
        if (!mIgnoreIfOut || field_0x26[camID]) {
            field_0x10[camID] = camera;
            aVStack_58.z += camera->getVolCenterZ();

            f32 dVar10 = aVStack_58.length();
            f32 dVar11 = Z2GetAudience()->getSetting()->field_0x0[0];
            f32 dVar12 = Z2GetAudience()->getSetting()->field_0x3c;
            if (dVar10 > dVar11) {
                field_0x18[camID] = field_0xc;
            } else {
                if (dVar10 < dVar12) {
                    field_0x18[camID] = 1.0f;
                } else {
                    field_0x18[camID] = JGeometry::TUtil<f32>::clamp(
                        1.0f + ((field_0xc - 1.0f) / (dVar11 - dVar12)) * (dVar10 - dVar12),
                        0.0f, 1.0f);
                }
            }

            calcVolumeFactor(camID);
        }
    }
}

f32 Z2SpotMic::calcMicDist(Z2Audible* audible) {
    if (!mMicOn) {
        return 0.0f;
    }
    if (mPosPtr == NULL) {
        return 0.0f;
    }
    if (audible == NULL) {
        return 0.0f;
    }

    JGeometry::TVec3<f32> aTStack_1c;
    JGeometry::TVec3<f32> aTStack_28(*mPosPtr);

    aTStack_1c.sub(audible->getPos(), aTStack_28);
    return aTStack_1c.length();
}

u32 Z2SpotMic::calcMicPriority(f32 param_0) {
    if (param_0 > field_0x4) {
        return Z2GetAudience()->getSetting()->field_0x64;
    } 

    if (param_0 < field_0x0) {
        return 0;
    }
    
    return field_0x1c * (param_0 - field_0x0);
}

f32 Z2SpotMic::calcMicVolume(f32 param_0, int camID, f32 param_2) {
    JUT_ASSERT(687, camID >= 0);
    JUT_ASSERT(688, camID < 1);

    if (mMicOn == NULL) {
        return param_2;
    }

    if (mPosPtr == NULL) {
        return param_2;
    }

    if (field_0x10[camID] == NULL) {
        return param_2;
    }

    if (mIgnoreIfOut && field_0x26[camID] == 0) {
        return param_2;
    }

    if (param_0 > field_0x4) {
        return -1.0f;
    }

    if (param_2 >= field_0x18[camID]) {
        return param_2;
    }

    if (param_0 < field_0x0) {
        return field_0x18[camID];
    }

    f32 dVar7 = JGeometry::TUtil<f32>::clamp(
        field_0x18[camID] + field_0x20[camID] * (param_0 - field_0x0),
        field_0x8, field_0x18[camID]);
    if (param_2 < dVar7) {
        param_2 = dVar7;
    }
    return param_2;
}

Z2Audience::Z2Audience() : JASGlobalInstance<Z2Audience>(true), field_0x4(1.0f), field_0x8(0x7f) {
    mNumPlayers = 1;
    mUsingOffMicVol = false;
    mAudioCamera[0].init();
    mAudioCamera[0].setMainCamera(true);
    mLinkMic = mSpotMic;
}

Z2Audience::~Z2Audience() {
    // JUT_ASSERT(751, !isActive()); // TODO: need to setup rest of JASMemPool stuff
}

void Z2Audience::setAudioCamera(f32 (*param_0)[4], Vec& pos, Vec& param_2, f32 param_3,
                                f32 param_4, bool param_5, int camID, bool param_7) {
    JUT_ASSERT(687, camID >= 0);
    JUT_ASSERT(688, camID < mNumPlayers);
    mAudioCamera[camID].setCameraState(param_0, pos, param_2, param_3, param_4, param_5, param_7);
    mLinkMic->setMicState(&mAudioCamera[camID], camID);
}

JAIAudible* Z2Audience::newAudible(const JGeometry::TVec3<f32>& pos, JAISoundID soundID,
                                   const JGeometry::TVec3<f32>* param_2, u32 param_3) {
    u32 channelNum = param_3 | ~((1 << mNumPlayers) + -1);
    if (channelNum == 0xFFFFFFFF) {
        JUT_WARN(826, "%s", "You masked all audible channels !");
        return NULL;
    }

    JAUAudibleParam params = Z2GetSoundInfo()->getAudibleSwFull(soundID);
    bool x = params.field_0x0.bytes.b0_0 != 0;

    Z2Audible* audible = new Z2Audible(pos, param_2, channelNum, x);
    if (audible == NULL) {
        return NULL;
    }

    audible->setAudibleParam(params);

    for (int i = 0; i < mNumPlayers; i++) {
        Z2AudibleChannel* channel = audible->getChannel(i);
        if (channel != NULL) {
            u32 distVolBit = audible->getDistVolBit();
            mAudioCamera[i].convertAbsToRel(audible, i);
            calcDeltaPriority_(channel->field_0x14.field_0x10, distVolBit, false);
        }
    }

    for (int i = 0; i < 1; i++) {
        if (mSpotMic[i].isOn()) {
            audible->field_0x64[i] = mSpotMic[i].calcMicDist(audible);
        }
    }

    return audible;
}

void Z2Audience::deleteAudible(JAIAudible* audible) {
    delete audible;
}

Z2Audible::~Z2Audible() {}

u32 Z2Audience::calcPriority(JAIAudible* audible) {
    Z2Audible* Z2audible = (Z2Audible*)audible;

    if (!Z2audible->getAudibleParam()->field_0x0.bytes.b0_4) {
        for (int i = 0; i < mNumPlayers; i++) {
            mAudioCamera[i].convertAbsToRel(Z2audible, i);
        }
        return 0;
    } 
    
    u32 deltaPriority[1];
    u32 rv = 0xFFFFFFFF;
    for (int i = 0; i < mNumPlayers; i++) {
        Z2AudibleChannel* channel = (Z2audible)->getChannel(i);
        if (channel != NULL) {
            mAudioCamera[i].convertAbsToRel(Z2audible, i);
            u32 distvolBit = Z2audible->getDistVolBit();
            const JAUAudibleParam* puVar1 = Z2audible->getAudibleParam();
            deltaPriority[i] = calcDeltaPriority_(channel->field_0x14.field_0x10, distvolBit, puVar1->field_0x0.bytes.b0_7);
            if (deltaPriority[i] < rv) {
                rv = deltaPriority[i];
            }
        }
    }

    for (int i = 0; i < 1; i++) {
        if (mSpotMic[i].isOn()) {
            Z2audible->field_0x64[i] = mSpotMic[i].calcMicDist(Z2audible);
            u32 micPriority = mSpotMic[i].calcMicPriority(Z2audible->field_0x64[i]);
            if (micPriority < rv) {
                rv = micPriority;
            }
        }
    }
    
    return rv;
}

f32 Z2Audience::calcOffMicSound(f32 param_0) {
    if (mUsingOffMicVol == 0) {
        return param_0;
    }

    return param_0 * field_0x4;
}

void Z2Audience::mixChannelOut(const JASSoundParams& outParams, JAIAudible* audible, int channelNum) {
    Z2Audible* Z2audible = (Z2Audible*)audible;
    Z2AudibleChannel* channel = Z2audible->getChannel(channelNum);
    if (channel == NULL) {
        return;
    }

    JASSoundParams local_60;
    u32 distVolBit = Z2audible->getDistVolBit();
    if (Z2audible->getAudibleParam()->field_0x0.bytes.b0_5) {
        local_60.mVolume = calcVolume_(channel->field_0x14.field_0x10, distVolBit);
        f32 dVar9 = mLinkMic->calcMicVolume(Z2audible->field_0x64[0], channelNum, local_60.mVolume);
        if (dVar9 < 0.0f) {
            dVar9 = calcOffMicSound(local_60.mVolume);
        }
        local_60.mVolume = dVar9;

        JAUAudibleParam* params = Z2audible->getAudibleParam();
        if ((params->field_0x0.bytes.b1_2_7 & 8) && (local_60.mVolume <= 0.2f)) {
            local_60.mVolume = 0.2f;
        }
    } else {
        local_60.mVolume = 1.0f;
    }

    if (Z2audible->getAudibleParam()->field_0x0.bytes.b1_1 && mNumPlayers == 1) {
        local_60.mDolby = calcRelPosDolby(*(Vec*)&channel->field_0x14.field_0x00, channelNum);
    } else {
        local_60.mDolby = 0.5f;
    }

    if (Z2audible->getAudibleParam()->field_0x0.bytes.b1_0) {
        if (mNumPlayers > 2) {
            f32 fVar1;
            if (channelNum & 1) {
                fVar1 = 1.0f;
            } else {
                fVar1 = 0.0f;
            }
            local_60.mPan = fVar1;
        } else {
            local_60.mPan = calcRelPosPan(*(Vec*)&channel->field_0x14.field_0x00, channelNum);
        }
    } else {
        local_60.mPan = 0.5f;
    }

    local_60.mPitch = calcPitch_(channel, Z2audible, &mAudioCamera[channelNum]);
    f32 dVar9;
    if (Z2audible->getAudibleParam()->field_0x0.bytes.b0_6) {
        dVar9 = calcFxMix_(channel->field_0x14.field_0xC, distVolBit);
    } else {
        dVar9 = 0.0f;
    }
    local_60.mFxMix = dVar9;
    local_60.clamp();
    Z2audible->setOuterParams(outParams, local_60, channelNum);
}


void Z2Audience::setTargetVolume(f32 volume, int index) {
    mAudioCamera[index].setTargetVolume(volume);
}

bool Z2Audience::convertAbsToRel(Vec& src, Vec* dst, int camID) {
    JUT_ASSERT(1036, camID >= 0);
    JUT_ASSERT(1037, camID < mNumPlayers);
    return getAudioCamera(camID)->convertAbsToRel(src, dst);
}

f32 Z2Audience::calcRelPosVolume(const Vec& param_0, f32 param_1, int camID) {
    JGeometry::TVec3<f32> aTStack_3c(param_0);
    aTStack_3c.z += mAudioCamera[camID].getVolCenterZ();
    aTStack_3c.y *= 1.5f;

    f32 len = aTStack_3c.length();
    if (len > mSetting.field_0x0[0] * param_1) {
        return mSetting.field_0x40;
    }

    if (len < mSetting.field_0x3c) {
        return 1.0f;
    }

    return JGeometry::TUtil<f32>::clamp(
        1.0f + ((mSetting.field_0x40 - 1.0f) /
                ((mSetting.field_0x0[0] * param_1) - mSetting.field_0x3c)) *
                    (len - mSetting.field_0x3c),
        0.0f, 1.0f);
}

f32 Z2Audience::calcRelPosPan(const Vec& param_0, int camID) {
    Vec local_54 = param_0;
    local_54.y = 0.0f;

    f32 dVar6 = VECMag(&local_54);
    if (dVar6 < 0.1f) {
        return 0.5f;
    }

    dVar6 = (0.5f + mSetting.field_0x58 * (local_54.x / dVar6));
    if (local_54.z <= 0.0f) {
        f32 fovySin = mAudioCamera[camID].getFovySin();
        if (dVar6 < 0.5f) {
            dVar6 = Z2Calc::getParamByExp(
                dVar6, 0.0f, 0.5f,
                fovySin, 0.0f, 0.5f, Z2Calc::CURVE_POSITIVE);
        } else {
            dVar6 = Z2Calc::getParamByExp(
                dVar6, 0.5f, 1.0f,
                fovySin, 0.5f, 1.0f, Z2Calc::CURVE_NEGATIVE);
        }
    }

    if (dVar6 > 1.0f) {
        return 1.0f;
    }

    if (dVar6 < 0.0f) {
        return 0.0f;
    }

    return dVar6;
}

f32 Z2Audience::calcRelPosDolby(const Vec& param_0, int camID) {
    f32 fVar1 = param_0.z + mAudioCamera[camID].getDolbyCenterZ();
    if (fVar1 > mSetting.field_0x48) {
        return 1.0f;
    } 

    if (fVar1 < mSetting.field_0x44) {
        return 0.0f;
    }

    if (fVar1 < 0.0f) {
        return Z2Calc::getParamByExp(
            fVar1, mSetting.field_0x44, 0.0f,
            0.3f, 0.0f,
            mSetting.field_0x4c, Z2Calc::CURVE_POSITIVE);
    }

    return Z2Calc::getParamByExp(
        fVar1, 0.0f, mSetting.field_0x48,
        0.3f, mSetting.field_0x4c,
        1.0f, Z2Calc::CURVE_NEGATIVE);
}

f32 Z2Audience::calcVolume_(f32 param_0, int distVolBit) const {
    if (param_0 > mSetting.field_0x0[distVolBit]) {
        return mSetting.field_0x40;
    }

    if (param_0 < mSetting.field_0x3c) {
        return 1.0f;
    }

    return JGeometry::TUtil<f32>::clamp((mSetting.field_0x70[distVolBit] * (param_0 - mSetting.field_0x3c)) + 1.0f, 0.0f, 1.0f);
}

u32 Z2Audience::calcDeltaPriority_(f32 param_0, int distVolBit, bool param_2) const {
    if (param_0 > mSetting.field_0x0[distVolBit]) {
        if (param_2) {
            return -1;
        }
        return mSetting.field_0x64;
    }

    if (param_0 < mSetting.field_0x3c) {
        return 0;
    }

    return mSetting.field_0xac[distVolBit] * (param_0 - mSetting.field_0x3c);
}

/* 802BDCB0-802BDD00 2B85F0 0050+00 1/1 0/0 0/0 .text
 * calcPitchDoppler___10Z2AudienceCFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>f
 */
f32 Z2Audience::calcPitchDoppler_(const JGeometry::TVec3<f32>& param_0,
                                  const JGeometry::TVec3<f32>& param_1,
                                  const JGeometry::TVec3<f32>& param_2, f32 param_3) const {
    f32 dVar8 = param_0.dot(param_1);
    f32 dVar7 = param_0.dot(param_2);
    return (mSetting.field_0x5c + param_3 * dVar8) / (mSetting.field_0x5c + param_3 * dVar7);
}

f32 Z2Audience::calcFxMix_(f32 param_0, int distVolBit) const {
    if (param_0 > mSetting.field_0x0[distVolBit]) {
        return mSetting.field_0x54;
    } 
    if (param_0 < mSetting.field_0x3c) {
        return mSetting.field_0x50;
    } 
    return mSetting.field_0x50 + mSetting.field_0xe8[distVolBit] * (param_0 - mSetting.field_0x3c);
}

f32 Z2Audience::calcPitch_(Z2AudibleChannel* channel, const Z2Audible* audible, const Z2AudioCamera* camera) const {
    JAUAudibleParam audParam = *audible->getAudibleParam();
    if ((*(u8*)&audParam.field_0x0.raw >> 4) & 0xf) {
        JGeometry::TVec3<f32> aTStack_4c;
        aTStack_4c.normalize(channel->field_0x14.field_0x00);
        JAUAudibleParam audParam = *audible->getAudibleParam();

        f32 doppler = audParam.getDopplerPower();
        doppler = calcPitchDoppler_(aTStack_4c, *camera->getVel(), *audible->getVel(), doppler);

        if (channel->field_0x34 > 0.0f) {
            if (channel->field_0x34 > (doppler * mSetting.field_0x60)) {
                return channel->field_0x34;
            } 
            if (doppler > (channel->field_0x34 * mSetting.field_0x60)) {
                return channel->field_0x34;
            } 
            channel->field_0x34 = doppler;
            return doppler;
        }

        if (doppler > 0.0f) {
            channel->field_0x34 = doppler;
            return doppler;
        }

        return 1.0f;
    }

    return 1.0f;
}

int Z2Audience::getMaxChannels() {
    return mNumPlayers;
}
