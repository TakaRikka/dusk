/**
 * d_lib.cpp
 * Utility Functions
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_lib.h"
#include "d/d_debug_viewer.h"
#include "d/d_event.h"
#include "d/d_path.h"
#include "d/d_stage.h"
#include "JSystem/JKernel/JKRAramArchive.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRDecomp.h"
#include "f_op/f_op_actor.h"
#include "m_Do/m_Do_controller_pad.h"

static void dummy1(mDoExt_btkAnm* btk, cXyz& pos) {
    GXColor color;
    OS_REPORT("d_lib.cpp");
    OS_REPORT("brk != 0");
    OS_REPORT("btk != 0");
    OS_REPORT("bck != 0");
    OS_REPORT("0 <= room_no && room_no < 64");
    OS_REPORT("scls_data != 0");
    OS_REPORT("0 <= i_sclsnum && i_sclsnum < sclsinfo->num");
    btk->init((J3DModelData*)NULL, NULL, 0, 0, 0.0f, 0, 0);
    dDbVw_drawCircleOpa(pos, pos.x, color, 0, 0);
    mDoMtx_stack_c::multVecZero(&pos);
    mDoMtx_stack_c::multVec(&pos, &pos);
    dDbVw_drawLineOpa(pos, pos, color, 0, 0);
}

Quaternion ZeroQuat = {
    0.0f,
    0.0f,
    0.0f,
    1.0f
};

STControl::STControl(s16 delayY, s16 delayX, s16 param_2, s16 param_3, f32 threshold, f32 param_5,
                     s16 param_6, s16 param_7) {
    setWaitParm(delayY, delayX, param_2, param_3, threshold, param_5, param_6, param_7);
    init();
}

void STControl::setWaitParm(s16 delayY, s16 delayX, s16 param_2, s16 param_3, f32 threshold,
                            f32 param_5, s16 param_6, s16 param_7) {
    mRepeatDelayY = delayY;
    mRepeatDelayX = delayX;
    field_0x16 = param_2;
    field_0x1c = param_3;
    mThreshold = threshold;
    field_0x08 = param_5;
    field_0x24 = param_6;
    field_0x26 = param_7;
}

void STControl::init() {
    field_0x0e = 0;
    field_0x10 = 0;
    mDirectionTrig = field_0x0d = 0;
    field_0x22 = 0;
    mXwaitTimer = mRepeatDelayY;
    mYwaitTimer = mRepeatDelayY;
    field_0x1e = field_0x1c;
    field_0x20 = field_0x1c;
    mFirstWaitTime = 0;
    field_0x2a = mFirstWaitTime;
    field_0x2c = mFirstWaitTime;
}

void STControl::Xinit() {
    field_0x0e = 0;
    mDirectionTrig &= u8(0xFC);
    mXwaitTimer = mRepeatDelayY;
    field_0x1e = field_0x1c;
    field_0x2a = mFirstWaitTime;
}

void STControl::Yinit() {
    field_0x10 = 0;
    mDirectionTrig &= ~0x0C;
    mYwaitTimer = mRepeatDelayY;
    field_0x20 = field_0x1c;
    field_0x2c = mFirstWaitTime;
}

f32 STControl::getValueStick() {
    return mDoCPd_c::getStickValue(PAD_1);
}

s16 STControl::getAngleStick() {
    return mDoCPd_c::getStickAngle(PAD_1);
}

f32 CSTControl::getValueStick() {
    return mDoCPd_c::getSubStickValue(PAD_1);
}

s16 CSTControl::getAngleStick() {
    return mDoCPd_c::getSubStickAngle(PAD_1);
}

u8 STControl::checkTrigger() {
    field_0x0d = mDirectionTrig;
    f32 stickValue = getValueStick();
    s16 stickAngle = getAngleStick();
    u8 var_r6 = 0;
    s16 temp_r7 = 0x2000 - field_0x26 >> 1;

    if (!cM3d_IsZero(stickValue)) {
        if (stickAngle < field_0x22 - 0x7000 + temp_r7) {
            var_r6 |= TRIG_UP;
        } else if (stickAngle < field_0x22 - 0x5000 - temp_r7) {
            var_r6 |= TRIG_UP;
            var_r6 |= TRIG_LEFT;
        } else if (stickAngle < field_0x22 - 0x3000 + temp_r7) {
            var_r6 |= TRIG_LEFT;
        } else if (stickAngle < field_0x22 - 0x1000 - temp_r7) {
            var_r6 |= TRIG_LEFT;
            var_r6 |= TRIG_DOWN;
        } else if (stickAngle < field_0x22 + 0x1000 + temp_r7) {
            var_r6 |= TRIG_DOWN;
        } else if (stickAngle < field_0x22 + 0x3000 - temp_r7) {
            var_r6 |= TRIG_DOWN;
            var_r6 |= TRIG_RIGHT;
        } else if (stickAngle < field_0x22 + 0x5000 + temp_r7) {
            var_r6 |= TRIG_RIGHT;
        } else if (stickAngle < field_0x22 + 0x7000 - temp_r7) {
            var_r6 |= TRIG_RIGHT;
            var_r6 |= TRIG_UP;
        } else {
            var_r6 |= TRIG_UP;
        }

        if (stickValue >= mThreshold) {
            mDirectionTrig = var_r6;
        } else if (stickValue < field_0x08) {
            mDirectionTrig = 0;
        } else {
            mDirectionTrig &= ~var_r6;
        }

        if (mDirectionTrig != field_0x0d) {
            if (mDirectionTrig == 0) {
                field_0x22 = 0;
            } else if ((stickAngle & 0x1FFF) > 0x1000) {
                field_0x22 = field_0x24;
            } else {
                field_0x22 = -field_0x24;
            }
        }

        if (!(mDirectionTrig & 3)) {
            Xinit();
        }

        if (!(mDirectionTrig & 0xC)) {
            Yinit();
        }
    } else {
        mDirectionTrig = 0;
        Xinit();
        Yinit();
    }

    if ((field_0x0d & mDirectionTrig & 3) && field_0x0e > 0) {
        field_0x0e--;
    }

    if ((field_0x0d & mDirectionTrig & 0xC) && field_0x10 > 0) {
        field_0x10--;
    }

    return mDirectionTrig;
}

bool STControl::checkLeftTrigger() {
    if (field_0x0e == 0 && mDirectionTrig & TRIG_LEFT) {
        field_0x0e = mXwaitTimer + field_0x2a;
        field_0x2a = 0;
        if ((int)field_0x1e == 0) {
            mXwaitTimer -= field_0x16;
            if (mXwaitTimer < mRepeatDelayX) {
                mXwaitTimer = mRepeatDelayX;
            }
        } else {
            field_0x1e--;
        }
        return true;
    }
    return false;
}

bool STControl::checkRightTrigger() {
    if (field_0x0e == 0 && mDirectionTrig & TRIG_RIGHT) {
        field_0x0e = mXwaitTimer + field_0x2a;
        field_0x2a = 0;
        if ((int)field_0x1e == 0) {
            mXwaitTimer -= field_0x16;
            if (mXwaitTimer < mRepeatDelayX) {
                mXwaitTimer = mRepeatDelayX;
            }
        } else {
            field_0x1e--;
        }
        return true;
    }
    return false;
}

bool STControl::checkUpTrigger() {
    if (field_0x10 == 0 && mDirectionTrig & TRIG_UP) {
        field_0x10 = mYwaitTimer + field_0x2c;
        field_0x2c = 0;
        if ((int)field_0x20 == 0) {
            mYwaitTimer -= field_0x16;
            if (mYwaitTimer < mRepeatDelayX) {
                mYwaitTimer = mRepeatDelayX;
            }
        } else {
            field_0x20--;
        }
        return true;
    }
    return false;
}

bool STControl::checkDownTrigger() {
    if (field_0x10 == 0 && mDirectionTrig & TRIG_DOWN) {
        field_0x10 = mYwaitTimer + field_0x2c;
        field_0x2c = 0;
        if ((int)field_0x20 == 0) {
            mYwaitTimer -= field_0x16;
            if (mYwaitTimer < mRepeatDelayX) {
                mYwaitTimer = mRepeatDelayX;
            }
        } else {
            field_0x20--;
        }
        return true;
    }
    return false;
}

static void dummy2(dPath* path, dSv_player_status_b_c* statusB) {
    OS_REPORT("i_path != 0");
    OS_REPORT("i_startPoint < i_path->m_num");
    OS_REPORT("mPath != 0");
    dPath_ChkClose(path);
    statusB->getDateIpl();
}

u8 dLib_getEventSwitchNo(int param_0) {
    dStage_MapEvent_dt_c* mapEvent = dEvt_control_c::searchMapEventData(param_0);
    if (mapEvent != NULL) {
        return mapEvent->switch_no;
    }
    return 0xff;
}

BOOL dLib_checkActorInRectangle(fopAc_ac_c* param_0, fopAc_ac_c* param_1, cXyz const* param_2,
                                cXyz const* param_3) {
    cXyz local_24 = param_0->current.pos - param_1->current.pos;
    mDoMtx_stack_c::YrotS(-param_1->current.angle.y);
    mDoMtx_stack_c::multVec(&local_24, &local_24);
    if (param_2->x <= local_24.x && local_24.x <= param_3->x && param_2->z <= local_24.z &&
        local_24.z <= param_3->z)
    {
        return TRUE;
    }
    return FALSE;
}

u32 dLib_getExpandSizeFromAramArchive(JKRAramArchive* i_aramArchive, char const* param_2) {
    u8 header[32] ATTRIBUTE_ALIGN(32);
    JUT_ASSERT(1252, i_aramArchive != NULL);
    u32 address = i_aramArchive->getAramAddress(param_2);
    if (address == 0) {
        return 0;
    }
    u8* readAddress = JKRAramToMainRam(address, header, 0x20, EXPAND_SWITCH_UNKNOWN0,
                                       0, 0, 0xffffffff, 0);
    JUT_ASSERT(1260, readAddress == header);
    JKRArchive::SDIFileEntry* entry = i_aramArchive->findFsResource(param_2, 0);
    JUT_ASSERT(1263, entry != NULL);
    u32 uVar1 = ALIGN_NEXT(JKRDecompExpandSize(header), 32);
    u32 uVar5 = ALIGN_NEXT(entry->data_size, 32);
    return uVar1 > uVar5 ? uVar1 : uVar5;
}

OSTime dLib_time_c::m_diffTime;

OSTime dLib_time_c::m_stopTime;

bool dLib_time_c::m_timeStopped;

OSTime dLib_time_c::getTime() {
    if (!m_timeStopped) {
        return OSGetTime() - m_diffTime;
    }

    OSTime time = OSGetTime();
    m_diffTime += (time - m_stopTime);
    m_stopTime = time;
    return time - m_diffTime;
}

void dLib_time_c::stopTime() {
    if (!m_timeStopped) {
        m_stopTime = OSGetTime();
        m_timeStopped = true;
    }
}

void dLib_time_c::startTime() {
    if (m_timeStopped) {
        m_diffTime += OSGetTime() - m_stopTime;
        m_timeStopped = false;
    }
}
