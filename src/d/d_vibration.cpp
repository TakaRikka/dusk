#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_vibration.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_camera_mng.h"
#include "m_Do/m_Do_controller_pad.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_s_play.h"
#include <climits>

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

#if TARGET_PC
    data[0] = BSWAP16(data[0]);
    data[1] = BSWAP16(data[1]);
    data[2] = BSWAP16(data[2]);
#endif

    return data;
}

s32 rollshift(u32 pattern, s32 length, s32 index) {
#if AVOID_UB
    if (length == 0)
        index = 0;
#else
    index %= length;
#endif
    u32 var_r31 = pattern;
    return (var_r31 >> index) | (var_r31 << (length - index));
}

u32 makebits(u32 bits, s32 length, s32 numbits) {
    bits &= (-1 << (32 - length));
    u32 mask = bits;
    s32 i;

    for (i = length; i < numbits; i += length) {
        mask = bits | (mask >> length);
    }

    return mask;
}

u32 randombit(s32 rounds, s32 length) {
    u32 value = 0;
    int i;
    
    for (i = 0; i < rounds; i++) {
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

        mMotor.mShock.mFrame = mMotor.mQuake.mFrame = RESET_FRAME;
    }

    #if DEBUG
    testShake();
    #endif

    if ((mCamera.mShock.field_0x0 & 1) && mCamera.mShock.mFrame != RESET_FRAME) {
        mCamera.mShock.mFrame = 0;
        mCamera.mShock.mVibMode = VIBMODE_S_NONE;
    }

    if ((mCamera.mQuake.field_0x0 & 1) && mCamera.mQuake.mFrame != RESET_FRAME) {
        mCamera.mQuake.mFrame = 0;
        mCamera.mQuake.mVibMode = VIBMODE_Q_NONE;
    }

    if (mCamera.mShock.mFrame == 0 || mCamera.mQuake.mFrame == 0) {
        s32 pattern;
        u32 sp1C = 0;
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

        s32 length, bits, flags;
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

            #if DEBUG
            if (mVibTest.m_displayDbg & 0x8000) {
                OS_REPORT("vibration: %06d: start camera(%d) %x %d\n", mFrame, rumble, pattern, length);
            }
            #endif
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

            #if DEBUG
            if (mVibTest.m_displayDbg & 0x8000) {
                OS_REPORT("vibration: %06d: start camera(%d) %x %d\n", mFrame, rumble, pattern, length);
            }
            #endif
            break;
        case RUMBLE_SHOCK | RUMBLE_QUAKE:
            pattern = mCamera.mShock.mPattern << mCamera.mShock.mFrame;
            length = mCamera.mShock.mLength - mCamera.mShock.mFrame;
            sp1C = makebits(mCamera.mQuake.mPattern, mCamera.mQuake.mLength, length);
            pattern |= rollshift(sp1C, length, mFrame);
            pattern |= randombit(mCamera.mShock.mRounds > mCamera.mQuake.mRounds ? mCamera.mShock.mRounds : mCamera.mQuake.mRounds, length);

            flags = mCamera.mShock.mFlags | mCamera.mQuake.mFlags;
            if (dKy_darkworld_check()) {
                flags &= ~0x30;
            }

            if (dComIfGp_getCamera(0) != NULL && dComIfGp_getCamera(0)->field_0x22f == 0x47) {
                dCam_getBody()->StartShake(length, (u8*)&pattern, flags, cXyz(mCamera.mShock.mPos + mCamera.mQuake.mPos).norm());
            }

            mCamera.mShock.mFrame = mCamera.mQuake.mFrame = 0;

            #if DEBUG
            if (mVibTest.m_displayDbg & 0x8000) {
                OS_REPORT("vibration: %06d: start camera(%d) %x %d\n", mFrame, rumble, pattern, length);
            }
            #endif
            break;
        default:
            if (dComIfGp_getCamera(0) != NULL && dComIfGp_getCamera(0)->field_0x22f == 0x47) {
                dCam_getBody()->StopShake();
            }

            #if DEBUG
            if (mVibTest.m_displayDbg & 0x8000) {
                OS_REPORT("vibration: %06d: stop camera\n", mFrame);
            }
            #endif
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
        s32 sp14 = 0;
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
            mDoCPd_c::startMotorWave(PAD_1, (u8*)makedata(data, pattern, length), JUTGamePad::CRumble::VAL_0, 60);
            
            #if DEBUG
            if (mVibTest.m_displayDbg & 0x8000) {
                OS_REPORT("vibration: %06d: start motor (%d) %x %d\n", mFrame, rumble, pattern, length);
            }
            #endif
            break;
        case RUMBLE_QUAKE:
            length = mMotor.mQuake.mLength;
            pattern = rollshift(mMotor.mQuake.mPattern, length, mFrame);
            pattern |= randombit(mMotor.mQuake.mRounds, length);
            mMotor.mQuake.mStopFrame = INT_MAX;

            OS_REPORT("d_vibration mDoCPd_c::startMotorWave\n");

            mDoCPd_c::startMotorWave(PAD_1, (u8*)makedata(data, pattern, length), JUTGamePad::CRumble::VAL_1, 60);
            
            #if DEBUG
            if (mVibTest.m_displayDbg & 0x8000) {
                OS_REPORT("vibration: %06d: start motor (%d) %x %d\n", mFrame, rumble, pattern, length);
            }
            #endif
            break;
        case RUMBLE_SHOCK | RUMBLE_QUAKE:
            pattern = mMotor.mShock.mPattern << mMotor.mShock.mFrame;
            length = mMotor.mShock.mLength - mMotor.mShock.mFrame;
            sp14 = makebits(mMotor.mQuake.mPattern, mMotor.mQuake.mLength, length);
            pattern |= rollshift(sp14, length, mFrame);
            pattern |= randombit(mMotor.mShock.mRounds > mMotor.mQuake.mRounds ? mMotor.mShock.mRounds : mMotor.mQuake.mRounds, length);
            mMotor.mQuake.mStopFrame = length;
            mMotor.mShock.mStopFrame = length;
            mMotor.mShock.mFrame = mMotor.mQuake.mFrame = 0;

            mDoCPd_c::startMotorWave(PAD_1, (u8*)makedata(data, pattern, length), JUTGamePad::CRumble::VAL_0, 60);
            
            #if DEBUG
            if (mVibTest.m_displayDbg & 0x8000) {
                OS_REPORT("vibration: %06d: start motor (%d) %x %d\n", mFrame, rumble, pattern, length);
            }
            #endif
            break;
        default:
            mDoCPd_c::stopMotorWave(PAD_1);
            mDoCPd_c::stopMotor(PAD_1);
            mMotor.mShock.mStopFrame = mMotor.mQuake.mStopFrame = RESET_FRAME;

            #if DEBUG
            if (mVibTest.m_displayDbg & 0x8000) {
                OS_REPORT("vibration: %06d: stop motor\n", mFrame);
            }
            #endif
            break;
        }
    }

    mCamera.mShock.field_0x0 = mCamera.mQuake.field_0x0 = 0;
    mMotor.mShock.field_0x0 = mMotor.mQuake.field_0x0 = 0;

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

        #if DEBUG
        if (mVibTest.m_displayDbg & 0x8000) {
            OS_REPORT("vibration: %06d: stop motor @ limit restart\n", mFrame);
        }
        #endif
    } else if (mMotor.mQuake.mFrame >= 900) {
        mDoCPd_c::stopMotorWave(PAD_1);
        mDoCPd_c::stopMotor(PAD_1);

        #if DEBUG
        if ((mVibTest.m_displayDbg & 0x8000) && mMotor.mQuake.mFrame == 900) {
            OS_REPORT("vibration: %06d: stop motor @ limit\n", mFrame);
        }
        #endif

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

    #if DEBUG
    if ((mVibTest.m_displayDbg & 0x8000) && ret) {
        OS_REPORT("vibration: %06d: start shock %d %d\n", mFrame, i_vibmode, i_flags);
    }
    #endif

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

    #if DEBUG
    if ((mVibTest.m_displayDbg & 0x8000) && ret) {
        OS_REPORT("vibration: %06d: start quake %d %d\n", mFrame, i_vibmode, i_flags);
    }
    #endif

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

    #if DEBUG
    if ((mVibTest.m_displayDbg & 0x8000) && ret) {
        OS_REPORT("vibration: %06d: start quake %x %d %d\n", mFrame, bits, i_rounds, i_flags);
    }
    #endif

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

    #if DEBUG
    if ((mVibTest.m_displayDbg & 0x8000) && ret) {
        OS_REPORT("vibration: %06d: stop quake %d\n", mFrame, i_flags);
    }
    #endif

    return ret;
}

#if DEBUG
int dVibration_c::testShake() {
    int var_r29, var_r28;
    int ret = FALSE;
    s32 cam_bits, cam_len, cam_rounds;
    s32 motor_bits, motor_len, motor_rounds;

    switch (mVibTest.field_0x10) {
    case 10:
        if (mVibTest.m_vibswitch >= 1 && mVibTest.m_vibswitch < 100) {
            var_r29 = mVibTest.m_vibswitch;
            cam_bits = CS_patt[var_r29].bits;
            cam_len = CS_patt[var_r29].length;
            cam_rounds = CS_patt[var_r29].rounds;

            motor_bits = MS_patt[var_r29].bits;
            motor_len = MS_patt[var_r29].length;
            motor_rounds = MS_patt[var_r29].rounds;

            OS_REPORT("vibration: TEST C b %x l %d r %d\n", cam_bits, cam_len, cam_rounds);
            OS_REPORT("(SHOCK)  m %2d   M b %x l %d r %d\n", motor_bits, motor_len, motor_rounds);
        } else {
            motor_bits = cam_bits = (mVibTest.m_pattern << 0x10) | mVibTest.m_pattern2;
            motor_len = cam_len = mVibTest.m_length;
            motor_rounds = cam_rounds = mVibTest.m_randombit;
        }

        if (mVibTest.field_0xa & 0x7E) {
            mCamera.mShock.mVibMode = 0;
            mCamera.mShock.mFrame = 0;
            mCamera.mShock.mFlags = mVibTest.field_0xa;
            mCamera.mShock.mPos = cXyz(0.0f, 1.0f, 0.0f);
            mCamera.mShock.mPattern = cam_bits;
            mCamera.mShock.mLength = cam_len;
            mCamera.mShock.mRounds = cam_rounds;
        }

        if (mVibTest.field_0xa & 1) {
            mMotor.mShock.mVibMode = 0;
            mMotor.mShock.mFrame = 0;
            mMotor.mShock.mPattern = motor_bits;
            mMotor.mShock.mLength = motor_len;
            mMotor.mShock.mRounds = motor_rounds;
        }

        mVibTest.field_0x10 = 0;
        ret = TRUE;
        break;
    case 20:
        if (mVibTest.m_vibswitch >= 100 && mVibTest.m_vibswitch < 200) {
            var_r28 = mVibTest.m_vibswitch - 100;
            cam_bits = CQ_patt[var_r28].bits;
            cam_len = CQ_patt[var_r28].length;
            cam_rounds = CQ_patt[var_r28].rounds;

            motor_bits = MQ_patt[var_r28].bits;
            motor_len = MQ_patt[var_r28].length;
            motor_rounds = MQ_patt[var_r28].rounds;

            OS_REPORT("vibration: TEST C b %x l %d r %d\n", cam_bits, cam_len, cam_rounds);
            OS_REPORT("(QUAKE)  m %2d   M b %x l %d r %d\n", motor_bits, motor_len, motor_rounds);
        } else {
            motor_bits = cam_bits = (mVibTest.m_pattern << 0x10) | mVibTest.m_pattern2;
            motor_len = cam_len = mVibTest.m_length;
            motor_rounds = cam_rounds = mVibTest.m_randombit;
        }

        if (mVibTest.field_0xa & 0x7E) {
            mCamera.mQuake.mVibMode = 0;
            mCamera.mQuake.mFrame = 0;
            mCamera.mQuake.mFlags = mVibTest.field_0xa;
            mCamera.mQuake.mPos = cXyz(0.0f, 1.0f, 0.0f);
            mCamera.mQuake.mPattern = cam_bits;
            mCamera.mQuake.mLength = cam_len;
            mCamera.mQuake.mRounds = cam_rounds;
        }

        if (mVibTest.field_0xa & 1) {
            mMotor.mQuake.mVibMode = 0;
            mMotor.mQuake.mFrame = 0;
            mMotor.mQuake.mPattern = motor_bits;
            mMotor.mQuake.mLength = motor_len;
            mMotor.mQuake.mRounds = motor_rounds;
        }

        mVibTest.field_0x10 = 0;
        ret = TRUE;
        break;
    case 21:
        OS_REPORT("vibration: TEST STOP\n");
        StopQuake(0x1F);
        mVibTest.field_0x10 = 0;
        break;
    }

    return ret;
}
#endif

void dVibration_c::Kill() {
    mDoCPd_c::stopMotorWaveHard(PAD_1);
    mDoCPd_c::stopMotorHard(PAD_1);
    setDefault();
}   

bool dVibration_c::CheckQuake() {
    return mCamera.mQuake.mVibMode != VIBMODE_Q_NONE || mMotor.mQuake.mVibMode != VIBMODE_Q_NONE;
}

void dVibration_c::setDefault() {
    mCamera.mShock.mVibMode = mMotor.mShock.mVibMode = VIBMODE_S_NONE;
    mCamera.mQuake.mVibMode = mMotor.mQuake.mVibMode = VIBMODE_Q_NONE;
    mCamera.mShock.field_0x0 = mMotor.mShock.field_0x0 = 0;
    mCamera.mQuake.field_0x0 = mMotor.mQuake.field_0x0 = 0;
    mCamera.mShock.mPattern = mMotor.mShock.mPattern = 0;
    mCamera.mQuake.mPattern = mMotor.mQuake.mPattern = 0;
    mCamera.mShock.mLength = mMotor.mShock.mLength = 0;
    mCamera.mQuake.mLength = mMotor.mQuake.mLength = 0;
    mCamera.mShock.mRounds = mMotor.mShock.mRounds = 0;
    mCamera.mQuake.mRounds = mMotor.mQuake.mRounds = 0;
    mCamera.mShock.mFrame = mMotor.mShock.mFrame = RESET_FRAME;
    mCamera.mQuake.mFrame = mMotor.mQuake.mFrame = RESET_FRAME;
    mMotor.mShock.mStopFrame = RESET_FRAME;
    mMotor.mQuake.mStopFrame = RESET_FRAME;
    mMode = MODE_WAIT;
    mFrame = 0;
}

void dVibration_c::Init() {
    Kill();
    setDefault();

    #if DEBUG
    mVibTest.Init();
    #endif
}

void dVibration_c::Pause() {
    if (mMode != MODE_PAUSE) {
        if (mMotor.mShock.mVibMode != VIBMODE_S_NONE || mMotor.mQuake.mVibMode != VIBMODE_Q_NONE) {
            mDoCPd_c::stopMotorWaveHard(PAD_1);
            mDoCPd_c::stopMotorHard(PAD_1);
        }
    
        mCamera.mShock.mVibMode = mMotor.mShock.mVibMode = VIBMODE_S_NONE;
        mCamera.mShock.mFrame = mMotor.mShock.mFrame = RESET_FRAME;

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

#if DEBUG
void dVibTest_c::setDefault() {
    m_pattern = m_pattern2 = 0;
    field_0xa = 0;
    field_0x10 = 0;
    m_randombit = 0;
    m_vibswitch = 0;
    m_displayDbg = 0;
    m_length = 32;
}

dVibTest_c::dVibTest_c() {
    setDefault();
    id = mDoHIO_CREATE_CHILD("振動処理", this);
}

dVibTest_c::~dVibTest_c() {
    mDoHIO_DELETE_CHILD(id);
}

void dVibTest_c::Init() {
    setDefault();
}

void dVibTest_c::genMessage(JORMContext* mctx) {
    int i;

    mctx->genLabel("- パターン", 0, 0);
    mctx->genCheckBox(" ", &m_pattern, 0x8000, 0, NULL, 10, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern, 0x4000, 0, NULL, 0x1e, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern, 0x2000, 0, NULL, 0x32, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern, 0x1000, 0, NULL, 0x46, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern, 0x800, 0, NULL, 0x5a, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern, 0x400, 0, NULL, 0x6e, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern, 0x200, 0, NULL, 0x82, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern, 0x100, 0, NULL, 0x96, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern, 0x80, 0, NULL, 0xaa, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern, 0x40, 0, NULL, 0xbe, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern, 0x20, 0, NULL, 0xd2, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern, 0x10, 0, NULL, 0xe6, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern, 8, 0, NULL, 0xfa, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern, 4, 0, NULL, 0x10e, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern, 2, 0, NULL, 0x122, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern, 1, 0, NULL, 0x136, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern2, 0x8000, 0, NULL, 0x14a, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern2, 0x4000, 0, NULL, 0x15e, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern2, 0x2000, 0, NULL, 0x172, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern2, 0x1000, 0, NULL, 0x186, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern2, 0x800, 0, NULL, 0x19a, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern2, 0x400, 0, NULL, 0x1ae, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern2, 0x200, 0, NULL, 0x1c2, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern2, 0x100, 0, NULL, 0x1d6, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern2, 0x80, 0, NULL, 0x1ea, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern2, 0x40, 0, NULL, 0x1fe, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern2, 0x20, 0, NULL, 0x212, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern2, 0x10, 0, NULL, 0x226, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern2, 8, 0, NULL, 0x23a, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern2, 4, 0, NULL, 0x24e, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern2, 2, 0, NULL, 0x262, 0x14, 0x14, 0x14);
    mctx->genCheckBox(" ", &m_pattern2, 1, 0, NULL, 0x276, 0x14, 0x14, 0x14);

    mctx->genLabel("-", 0, 0);
    mctx->startComboBox("振動切替", &m_vibswitch, 0x130);
    for (i = 0; i < 11; i++) {
        mctx->genComboBoxItem(shock_names[i], i);
    }
    for (i = 0; i < 10; i++) {
        mctx->genComboBoxItem(quake_names[i], i + 100);
    }
    mctx->endComboBox();

    mctx->genLabel("- VIBMODE_S_* は衝撃型用、VIBMODE_Q_* は地震型用です", 0);
    mctx->genLabel("-", 0);

    mctx->genLabel("- タイプ", 0);
    mctx->genCheckBox("振動パック", &field_0xa, 1, 0);
    mctx->genCheckBox("座標揺れ", &field_0xa, 2, 0);
    mctx->genCheckBox("座標揺れ(強)", &field_0xa, 0x40, 0);
    mctx->genCheckBox("画角揺れ", &field_0xa, 4, 0);
    mctx->genCheckBox("Ｚ揺れ", &field_0xa, 8, 0);
    mctx->genCheckBox("ブラー", &field_0xa, 0x10, 0);

    mctx->genLabel("-", 0);
    mctx->genSlider("長さ", &m_length, 0, 0x20);
    mctx->genSlider("ランダムビット", &m_randombit, 0, 0x20);

    mctx->genLabel("-", 0);
    mctx->genButton("衝撃型揺れ開始", 0x12d, 0);
    mctx->genButton("地震型揺れ開始", 0x12e, 0);
    mctx->genButton("地震型揺れ停止", 0x12f, 0);

    mctx->genLabel("-", 0, 0);
    mctx->genCheckBox(" デバッグ表示", &m_displayDbg, 0x8000);

    mctx->genLabel("-", 0, 0);
    mctx->genLabel("-", 0, 0);
    mctx->genButton("揺れ実験（強）", 0x131, 0);
    mctx->genButton("揺れ実験（中）", 0x132, 0);
    mctx->genButton("揺れ実験（弱）", 0x133, 0);

    mctx->genLabel("-", 0, 0);
    mctx->genLabel("-", 0, 0);
}

void dVibTest_c::listenPropertyEvent(const JORPropertyEvent* event) {
    JORReflexible::listenPropertyEvent(event);

    switch ((int)event->id) {
    case 0x12D:
        field_0x10 = 10;
        break;
    case 0x12E:
        field_0x10 = 20;
        break;
    case 0x12F:
        field_0x10 = 21;
        break;
    case 0x131:
        dComIfGp_getVibration().StartShock(XREG_S(1) + 8, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        break;
    case 0x132:
        dComIfGp_getVibration().StartShock(XREG_S(2) + 4, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        break;
    case 0x133:
        dComIfGp_getVibration().StartShock(XREG_S(3) + 2, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        break;
    }
}
#endif
