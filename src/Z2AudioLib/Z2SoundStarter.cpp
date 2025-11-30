#include "Z2AudioLib/Z2SoundStarter.h"
#include "Z2AudioLib/Z2EnvSeMgr.h"
#include "Z2AudioLib/Z2AudioMgr.h"

Z2SoundStarter::Z2SoundStarter(bool setInstance) :
    JAISoundStarter(setInstance), JASGlobalInstance<Z2SoundStarter>(setInstance) {}

bool Z2SoundStarter::startSound(JAISoundID soundID, JAISoundHandle* handlePtr, const JGeometry::TVec3<f32>* posPtr) {
    return Z2SoundStarter::startSound(soundID, handlePtr, posPtr, 0, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0);
}

bool Z2SoundStarter::startSound(JAISoundID soundID, JAISoundHandle* handlePtr,
                                const JGeometry::TVec3<f32>* posPtr, u32 mapinfo, f32 fxMix,
                                f32 pitch, f32 volume, f32 pan, f32 dolby, u32 moveSteps) {
    JUT_ASSERT(45, handlePtr);
    if (mapinfo == 6) {
        switch (soundID) {
        case Z2SE_CM_BODYFALL_S:
            soundID = Z2SE_CM_BODYFALL_ASASE_S;
            break;
        case Z2SE_CM_BODYFALL_M:
            soundID = Z2SE_CM_BODYFALL_ASASE_M;
            break;
        case Z2SE_CM_BODYFALL_L:
            soundID = Z2SE_CM_BODYFALL_ASASE_L;
            break;
        }
    }

    if (soundID.id_.info.type.parts.groupID != 0 && soundID.id_.info.type.parts.groupID != 9) {
        if (Z2GetSceneMgr()->isInDarkness()) {
            fxMix = 1.0f;
        } else {
            f32 inWaterMix = Z2GetStatusMgr()->getCameraInWaterDepthRatio() > 0.0f;
            if (inWaterMix) {
                fxMix = inWaterMix;
            } else {
                fxMix += 0.5f * Z2GetEnvSeMgr()->getFogDensity();
                if (fxMix > 1.0f) {
                    fxMix = 1.0f;
                }
            }
        }
    }

    bool startSoundRes = Z2GetAudioMgr()->startSound(soundID, handlePtr, posPtr);
    if (*handlePtr) {
        if (mapinfo != 0) {
            setPortData(handlePtr, 6, mapinfo, -1);
        }
        if (fxMix > 0.0f) {
            (*handlePtr)->getAuxiliary().moveFxMix(fxMix, moveSteps);
        }
        if (pitch != 1.0f) {
            (*handlePtr)->getAuxiliary().movePitch(pitch, moveSteps);
        }
        if (volume != 1.0f) {
            (*handlePtr)->getAuxiliary().moveVolume(volume, moveSteps);
        }
        if (pan != -1.0f) {
            (*handlePtr)->getAuxiliary().movePan(pan, moveSteps);
        }
        if (dolby != -1.0f) {
            (*handlePtr)->getAuxiliary().moveDolby(dolby, moveSteps);
        }
    }

    return startSoundRes;
}

void Z2SoundStarter::setPortData(JAISoundHandle* handlePtr, u32 portNum, u16 data, s8 childIndex) {
    if (handlePtr != NULL && *handlePtr) {
        JASTrack* track = handlePtr->getSound()->getTrack();
        if (childIndex != -1) {
            if (track->getChild(childIndex) == NULL) {
                track->openChild(childIndex);
            }
            track = track->getChild(childIndex);
        }

        JUT_ASSERT(122, track);
        track->writePort(portNum, data);
    }
}

u16 Z2SoundStarter::getPortData(JAISoundHandle* handlePtr, u32 portNum, s8 childIndex) {
    if (handlePtr != NULL && *handlePtr) {
        JASTrack* track = NULL;
        if (childIndex == -1) {
            track = handlePtr->getSound()->getTrack();
        } else {
            track = handlePtr->getSound()->getChildTrack(childIndex);
        }

        JUT_ASSERT(140, track);
        return track->readPort(portNum);
    }

    return 0;
}
