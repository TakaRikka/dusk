#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_vibration.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_camera_mng.h"
#include "m_Do/m_Do_controller_pad.h"
#include "SSystem/SComponent/c_math.h"
#include <limits.h>

#define RESET_FRAME -99

#define RUMBLE_SHOCK 1
#define RUMBLE_QUAKE 2

#define FLAG_CAMERA_VIB 0x7E
#define FLAG_MOTOR_VIB  0x1

namespace {
u16* makedata(u16* data, u32 pattern, s32 length) {
    data[0] = length;
    data[1] = pattern >> 16;
    data[2] = pattern;
    data[3] = 0;
    return data;
}

s32 rollshift(u32 pattern, s32 length, s32 index) {
    index %= length;
    return (pattern >> index) | (pattern << (length - index));
}

u32 makebits(u32 bits, s32 length, s32 numbits) {
    s32 i;
    u32 mask = bits & (-1 << (32 - length));
    bits = mask;

    for (i = length; i < numbits; i += length) {
        bits = mask | (bits >> length);
    }

    return bits;
}

u32 randombit(s32 rounds, s32 length) {
    u32 value = 0;
    
    for (int i = 0; i < rounds; i++) {
        value |= 0x40000000 >> (u32)(length * cM_rnd());
    }
    return value;
}
};

int dVibration_c::Run() {
    mMode = MODE_RUN;

    if (dComIfGs_checkOptVibration() != 1) {
        if (mMotor.mShock.mVibMode != VIBMODE_S_NONE) {
            mMotor.mShock.mVibMode = VIBMODE_S_NONE;
            mMotor.mShock.field_0x0 |= 1;
        }

        if (mMotor.mQuake.mVibMode != VIBMODE_Q_NONE) {
            mMotor.mQuake.mVibMode = VIBMODE_Q_NONE;
            mMotor.mQuake.field_0x0 |= 1;
        }

        mMotor.mQuake.mFrame = RESET_FRAME;
        mMotor.mShock.mFrame = RESET_FRAME;
    }

    if ((mCamera.mShock.field_0x0 & 1) && mCamera.mShock.mFrame != RESET_FRAME) {
        mCamera.mShock.mFrame = 0;
        mCamera.mShock.mVibMode = VIBMODE_S_NONE;
    }

    if ((mCamera.mQuake.field_0x0 & 1) && mCamera.mQuake.mFrame != RESET_FRAME) {
        mCamera.mQuake.mFrame = 0;
        mCamera.mQuake.mVibMode = VIBMODE_Q_NONE;
    }

    if (mCamera.mShock.mFrame == 0 || mCamera.mQuake.mFrame == 0) {
        u32 rumble = 0;
        if (mCamera.mShock.mVibMode == VIBMODE_S_NONE || (mCamera.mShock.field_0x0 & 1)) {
            mCamera.mShock.mFrame = RESET_FRAME;
            mCamera.mShock.mPattern = 0;
            mCamera.mShock.mLength = 0;
            mCamera.mShock.mRounds = 0;
        } else if (mCamera.mShock.mFrame >= 0) {
            rumble |= RUMBLE_SHOCK;
        }

        if (mCamera.mQuake.mVibMode == VIBMODE_Q_NONE || (mCamera.mQuake.field_0x0 & 1)) {
            mCamera.mQuake.mFrame = RESET_FRAME;
            mCamera.mQuake.mPattern = 0;
            mCamera.mQuake.mLength = 0;
            mCamera.mQuake.mRounds = 0;
        } else if (mCamera.mQuake.mFrame >= 0) {
            rumble |= RUMBLE_QUAKE;
        }

        s32 length, pattern, bits, flags;
        switch (rumble) {
        case RUMBLE_SHOCK:
            length = mCamera.mShock.mLength;
            pattern = mCamera.mShock.mPattern;
            pattern |= randombit(mCamera.mShock.mRounds, length);

            flags = mCamera.mShock.mFlags;
            if (dKy_darkworld_check()) {
                flags &= ~0x30;
            }

            if (dComIfGp_getCamera(0) != NULL && dComIfGp_getCamera(0)->field_0x22f == 0x47) {
                dCam_getBody()->StartShake(length, (u8*)&pattern, flags, mCamera.mShock.mPos.norm());
            }
            break;
        case RUMBLE_QUAKE:
            length = mCamera.mQuake.mLength;
            pattern = rollshift(mCamera.mQuake.mPattern, length, mFrame);
            pattern |= randombit(mCamera.mQuake.mRounds, length);

            flags = mCamera.mQuake.mFlags;
            if (dKy_darkworld_check()) {
                flags &= ~0x30;
            }

            if (dComIfGp_getCamera(0) != NULL && dComIfGp_getCamera(0)->field_0x22f == 0x47) {
                dCam_getBody()->StartShake(length, (u8*)&pattern, flags, mCamera.mQuake.mPos.norm());
            }
            break;
        case RUMBLE_SHOCK | RUMBLE_QUAKE:
            pattern = mCamera.mShock.mPattern << mCamera.mShock.mFrame;
            length = mCamera.mShock.mLength - mCamera.mShock.mFrame;
            pattern |= rollshift(makebits(mCamera.mQuake.mPattern, mCamera.mQuake.mLength, length), length, mFrame);
            pattern |= randombit(mCamera.mShock.mRounds > mCamera.mQuake.mRounds ? mCamera.mShock.mRounds : mCamera.mQuake.mRounds, length);

            flags = mCamera.mShock.mFlags | mCamera.mQuake.mFlags;
            if (dKy_darkworld_check()) {
                flags &= ~0x30;
            }

            if (dComIfGp_getCamera(0) != NULL && dComIfGp_getCamera(0)->field_0x22f == 0x47) {
                dCam_getBody()->StartShake(length, (u8*)&pattern, flags, cXyz(mCamera.mShock.mPos + mCamera.mQuake.mPos).norm());
            }

            mCamera.mQuake.mFrame = 0;
            mCamera.mShock.mFrame = 0;
            break;
        default:
            if (dComIfGp_getCamera(0) != NULL && dComIfGp_getCamera(0)->field_0x22f == 0x47) {
                dCam_getBody()->StopShake();
            }
            break;
        }
    }

    if ((mMotor.mShock.field_0x0 & 1) && mMotor.mShock.mFrame != RESET_FRAME) {
        mMotor.mShock.mFrame = 0;
        mMotor.mShock.mVibMode = VIBMODE_S_NONE;
    }

    if ((mMotor.mQuake.field_0x0 & 1) && mMotor.mQuake.mFrame != RESET_FRAME) {
        mMotor.mQuake.mFrame = 0;
        mMotor.mQuake.mVibMode = VIBMODE_Q_NONE;
    }

    if (mMotor.mShock.mFrame == 0 || mMotor.mQuake.mFrame == 0) {
        u32 rumble = 0;
        if (mMotor.mShock.mVibMode == VIBMODE_S_NONE || (mMotor.mShock.field_0x0 & 1)) {
            mMotor.mShock.mFrame = RESET_FRAME;
            mMotor.mShock.mPattern = 0;
            mMotor.mShock.mLength = 0;
            mMotor.mShock.mRounds = 0;
        } else if (mMotor.mShock.mFrame >= 0) {
            rumble |= RUMBLE_SHOCK;
        }

        if (mMotor.mQuake.mVibMode == VIBMODE_Q_NONE || (mMotor.mQuake.field_0x0 & 1)) {
            mMotor.mQuake.mFrame = RESET_FRAME;
            mMotor.mQuake.mPattern = 0;
            mMotor.mQuake.mLength = 0;
            mMotor.mQuake.mRounds = 0;
        } else if (mMotor.mQuake.mFrame >= 0) {
            rumble |= RUMBLE_QUAKE;
        }

        static u16 data[4];
        s32 pattern, length, bits;

        switch (rumble) {
        case RUMBLE_SHOCK:
            length = mMotor.mShock.mLength;
            pattern = mMotor.mShock.mPattern;
            pattern |= randombit(mMotor.mShock.mRounds, length);
            mMotor.mShock.mStopFrame = length;
            mDoCPd_c::startMotorWave(PAD_1, makedata(data, pattern, length), JUTGamePad::CRumble::VAL_0, 60);
            break;
        case RUMBLE_QUAKE:
            length = mMotor.mQuake.mLength;
            pattern = rollshift(mMotor.mQuake.mPattern, length, mFrame);
            pattern |= randombit(mMotor.mQuake.mRounds, length);
            mMotor.mQuake.mStopFrame = INT_MAX;
            mDoCPd_c::startMotorWave(PAD_1, makedata(data, pattern, length), JUTGamePad::CRumble::VAL_1, 60);
            break;
        case RUMBLE_SHOCK | RUMBLE_QUAKE:
            pattern = mMotor.mShock.mPattern << mMotor.mShock.mFrame;
            length = mMotor.mShock.mLength - mMotor.mShock.mFrame;
            pattern |= rollshift(makebits(mMotor.mQuake.mPattern, mMotor.mQuake.mLength, length), length, mFrame);
            pattern |= randombit(mMotor.mShock.mRounds > mMotor.mQuake.mRounds ? mMotor.mShock.mRounds : mMotor.mQuake.mRounds, length);
            mMotor.mQuake.mStopFrame = length;
            mMotor.mShock.mStopFrame = length;
            mMotor.mQuake.mFrame = 0;
            mMotor.mShock.mFrame = 0;
            mDoCPd_c::startMotorWave(PAD_1, makedata(data, pattern, length), JUTGamePad::CRumble::VAL_0, 60);
            break;
        default:
            mDoCPd_c::stopMotorWave(PAD_1);
            mDoCPd_c::stopMotor(PAD_1);
            mMotor.mQuake.mStopFrame = RESET_FRAME;
            mMotor.mShock.mStopFrame = RESET_FRAME;
            break;
        }
    }

    mCamera.mQuake.field_0x0 = 0;
    mCamera.mShock.field_0x0 = 0;
    mMotor.mQuake.field_0x0 = 0;
    mMotor.mShock.field_0x0 = 0;

    if (mCamera.mShock.mFrame >= 0) {
        mCamera.mShock.mFrame++;

        if (mCamera.mShock.mFrame > mCamera.mShock.mLength) {
            mCamera.mShock.field_0x0 |= 1;
        }
    }

    if (mMotor.mShock.mFrame >= 0) {
        mMotor.mShock.mFrame++;

        if (mMotor.mShock.mFrame > mMotor.mShock.mStopFrame) {
            mMotor.mShock.field_0x0 |= 1;
        }
    }

    if (mCamera.mQuake.mFrame >= 0) {
        mCamera.mQuake.mFrame++;

        if (mCamera.mQuake.mFrame > mCamera.mQuake.mLength) {
            mCamera.mQuake.mFrame = 0;
        }
    }

    if (mMotor.mQuake.mFrame >= 930) {
        mMotor.mQuake.mFrame = 0;
    } else if (mMotor.mQuake.mFrame >= 900) {
        mDoCPd_c::stopMotorWave(PAD_1);
        mDoCPd_c::stopMotor(PAD_1);
        mMotor.mQuake.mFrame++;
    } else if (mMotor.mQuake.mFrame >= 0) {
        mMotor.mQuake.mFrame++;
        if (mMotor.mQuake.mFrame > mMotor.mQuake.mStopFrame) {
            mMotor.mQuake.mFrame = 0;
        }
    }

    mFrame++;
    return 1;
}

bool dVibration_c::StartShock(int i_vibmode, int i_flags, cXyz i_pos) {
    bool ret = false;
    if (i_flags & FLAG_CAMERA_VIB) {
        mCamera.mShock.mVibMode = i_vibmode;
        mCamera.mShock.mFrame = 0;
        mCamera.mShock.mFlags = i_flags;
        mCamera.mShock.mPos = i_pos;

        mCamera.mShock.mPattern = CS_patt[i_vibmode].bits;
        mCamera.mShock.mLength = CS_patt[i_vibmode].length;
        mCamera.mShock.mRounds = CS_patt[i_vibmode].rounds;
        ret = true;
    }

    if ((i_flags & FLAG_MOTOR_VIB) && dComIfGs_checkOptVibration() == 1) {
        mMotor.mShock.mVibMode = i_vibmode;
        mMotor.mShock.mFrame = 0;

        mMotor.mShock.mPattern = MS_patt[i_vibmode].bits;
        mMotor.mShock.mLength = MS_patt[i_vibmode].length;
        ret = true;
    }

    return ret;
}

bool dVibration_c::StartQuake(int i_vibmode, int i_flags, cXyz i_pos) {
    bool ret = false;
    if (i_flags & FLAG_CAMERA_VIB) {
        mCamera.mQuake.mVibMode = i_vibmode;
        mCamera.mQuake.mFrame = 0;
        mCamera.mQuake.mFlags = i_flags;
        mCamera.mQuake.mPos = i_pos;
        mCamera.mQuake.mPattern = CQ_patt[i_vibmode].bits;
        mCamera.mQuake.mLength = CQ_patt[i_vibmode].length;
        mCamera.mQuake.mRounds = CQ_patt[i_vibmode].rounds;
        ret = true;
    }

    if ((i_flags & FLAG_MOTOR_VIB) && dComIfGs_checkOptVibration() == 1) {
        mMotor.mQuake.mVibMode = i_vibmode;
        mMotor.mQuake.mFrame = 0;
        mMotor.mQuake.mPattern = MQ_patt[i_vibmode].bits;
        mMotor.mQuake.mLength = MQ_patt[i_vibmode].length;
        mMotor.mQuake.mRounds = CQ_patt[i_vibmode].rounds;
        ret = true;
    }

    return ret;
}

bool dVibration_c::StartQuake(const u8* i_pattern, int i_rounds, int i_flags, cXyz i_pos) {
    bool ret = false;

    int length = (i_pattern[0] << 8) | i_pattern[1];
    u32 bits = i_pattern[length >= 1  ? 2 : 0] << 24 |
               i_pattern[length >= 9  ? 3 : 0] << 16 |
               i_pattern[length >= 17 ? 4 : 0] << 8  |
               i_pattern[length >= 25 ? 5 : 0] << 0;

    if (i_flags & FLAG_CAMERA_VIB) {
        mCamera.mQuake.mVibMode = VIBMODE_Q_CUSTOM;
        mCamera.mQuake.mFrame = 0;
        mCamera.mQuake.mFlags = i_flags;
        mCamera.mQuake.mPos = i_pos;
        mCamera.mQuake.mPattern = makebits(bits, i_pattern[1], 32);
        mCamera.mQuake.mLength = 32;
        mCamera.mQuake.mRounds = i_rounds;
        ret = true;
    }

    if ((i_flags & FLAG_MOTOR_VIB) && dComIfGs_checkOptVibration() == 1) {
        mMotor.mQuake.mVibMode = VIBMODE_Q_CUSTOM;
        mMotor.mQuake.mFrame = 0;
        mMotor.mQuake.mPattern = makebits(bits, i_pattern[1], 32);
        mMotor.mQuake.mLength = 32;
        mMotor.mQuake.mRounds = i_rounds;
        ret = true;
    }

    return ret;
}

int dVibration_c::StopQuake(int i_flags) {
    int ret = FALSE;
    if (i_flags & FLAG_CAMERA_VIB) {
        if (mCamera.mQuake.mFrame >= 0) {
            mCamera.mQuake.mFlags &= ~i_flags;
            if (mCamera.mQuake.mFlags == 0) {
                mCamera.mQuake.field_0x0 |= 1;
            }
            ret = TRUE;
        }
    }

    if ((i_flags & FLAG_MOTOR_VIB) && mMotor.mQuake.mFrame >= 0) {
        mMotor.mQuake.field_0x0 |= 1;
        ret = TRUE;
    }

    return ret;
}

void dVibration_c::Kill() {
    mDoCPd_c::stopMotorWaveHard(PAD_1);
    mDoCPd_c::stopMotorHard(PAD_1);
    setDefault();
}   

bool dVibration_c::CheckQuake() {
    return mCamera.mQuake.mVibMode != VIBMODE_Q_NONE || mMotor.mQuake.mVibMode != VIBMODE_Q_NONE;
}

void dVibration_c::setDefault() {
    mMotor.mShock.mVibMode = VIBMODE_S_NONE;
    mCamera.mShock.mVibMode = VIBMODE_S_NONE;
    mMotor.mQuake.mVibMode = VIBMODE_Q_NONE;
    mCamera.mQuake.mVibMode = VIBMODE_Q_NONE;
    mMotor.mShock.field_0x0 = 0;
    mCamera.mShock.field_0x0 = 0;
    mMotor.mQuake.field_0x0 = 0;
    mCamera.mQuake.field_0x0 = 0;
    mMotor.mShock.mPattern = 0;
    mCamera.mShock.mPattern = 0;
    mMotor.mQuake.mPattern = 0;
    mCamera.mQuake.mPattern = 0;
    mMotor.mShock.mLength = 0;
    mCamera.mShock.mLength = 0;
    mMotor.mQuake.mLength = 0;
    mCamera.mQuake.mLength = 0;
    mMotor.mShock.mRounds = 0;
    mCamera.mShock.mRounds = 0;
    mMotor.mQuake.mRounds = 0;
    mCamera.mQuake.mRounds = 0;
    mMotor.mShock.mFrame = RESET_FRAME;
    mCamera.mShock.mFrame = RESET_FRAME;
    mMotor.mQuake.mFrame = RESET_FRAME;
    mCamera.mQuake.mFrame = RESET_FRAME;
    mMotor.mShock.mStopFrame = RESET_FRAME;
    mMotor.mQuake.mStopFrame = RESET_FRAME;
    mMode = MODE_WAIT;
    mFrame = 0;
}

void dVibration_c::Init() {
    Kill();
    setDefault();
}

void dVibration_c::Pause() {
    if (mMode != MODE_PAUSE) {
        if (mMotor.mShock.mVibMode != VIBMODE_S_NONE || mMotor.mQuake.mVibMode != VIBMODE_Q_NONE) {
            mDoCPd_c::stopMotorWaveHard(PAD_1);
            mDoCPd_c::stopMotorHard(PAD_1);
        }
    
        mMotor.mShock.mVibMode = VIBMODE_S_NONE;
        mCamera.mShock.mVibMode = VIBMODE_S_NONE;
        mMotor.mShock.mFrame = RESET_FRAME;
        mCamera.mShock.mFrame = RESET_FRAME;

        if (mCamera.mQuake.mVibMode != VIBMODE_Q_NONE) {
            mCamera.mQuake.mFrame = 0;
        }
        if (mMotor.mQuake.mVibMode != VIBMODE_Q_NONE) {
            mMotor.mQuake.mFrame = 0;
        }

        mMode = MODE_PAUSE;
    }
}

void dVibration_c::Remove() {
    Kill();
}
