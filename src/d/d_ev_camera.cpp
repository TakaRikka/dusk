#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_ev_camera.h"
#include "d/d_debug_viewer.h"
#include "d/d_demo.h"
#include "m_Do/m_Do_controller_pad.h"
#include "d/d_camera.h"
#include "d/actor/d_a_midna.h"
#include "d/actor/d_a_player.h"
#include "f_op/f_op_camera_mng.h"
#include "m_Do/m_Do_lib.h"
#include "d/d_drawlist.h"
#include "d/actor/d_a_alink.h"

namespace {
    /* 800889F8-80088A7C 083338 0084+00 1/1 0/0 0/0 .text isStageEvent__25@unnamed@d_ev_camera_cpp@Fi */
    static bool isStageEvent(int param_0) {
        dStage_MapEventInfo_c* info = dComIfGp_getStage()->getMapEventInfo();
        if (info != NULL) {
            for (int i = 0; i < info->num; i++) {
                if (param_0 == info->m_entries[i].field_0x4) {
                    return true;
                }
            }
        }
        return false;
    }
} // namespace

int dCamera_c::StartEventCamera(int param_0, int param_1, ...) {
    if (chkFlag(0x20000000)) {
        return 0;
    }

    mEventData.field_0x14 = param_1;
    mEventData.field_0x18 = param_0;

    va_list args;
    va_start(args, param_1);
    for (int i = 0; i < 8; i++) {
        char* param_name = va_arg(args, char*);
        if (param_name != NULL) {
            strcpy(mEventData.mEventParams[i].name, param_name);
            mEventData.mEventParams[i].field_0x10 = va_arg(args, int);
            mEventData.mEventParams[i].value = va_arg(args, int);
        } else {
            mEventData.mEventParams[i].name[0] = 0;
            break;
        }
    }
    va_end(args);

    setFlag(0x20000000);
    mCurCamStyleTimer = 0;
    mEventData.field_0x0 = 0;
    return 1;
}

int dCamera_c::EndEventCamera(int param_1) {
    if (!chkFlag(0x20000000)) {
        return 0;
    }

    if (mEventData.field_0x14 == -1 || mEventData.field_0x14 == param_1) {
        clrFlag(0x20000000);
        mEventData.field_0x0 = 1;

        if (dComIfGp_getEvent().runCheck()) {
            dComIfGp_getEventManager().setCameraPlay(2);
        }

        return 1;
    }

    return 0;
}

int dCamera_c::searchEventArgData(char* i_eventName) {
    int i;
    bool found_event = false;

    for (i = 0; i < 8; i++) {
        if (*mEventData.mEventParams[i].name == 0) {
            break;
        }

        if (strcmp(mEventData.mEventParams[i].name, i_eventName) == 0) {
            found_event = true;
            break;
        }
    }

    return found_event ? i : -1;
}

bool dCamera_c::getEvIntData(int* i_data, char* i_event) {
    if (chkFlag(0x20000000)) {
        int index = searchEventArgData(i_event);
        if (index == -1) {
            return 0;
        }

        if (mEventData.mEventParams[index].field_0x10 < 0) {
            *i_data = mEventData.mEventParams[index].value;
        } else {
            *i_data = *(int*)mEventData.mEventParams[index].value;
        }
    } else if (dComIfGp_evmng_getMySubstanceNum(mEventData.mStaffIdx, i_event) != 0) {
        *i_data = *dComIfGp_evmng_getMyIntegerP(mEventData.mStaffIdx, i_event);
    } else {
#if DEBUG
        if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
            OS_REPORT("camera: event: %16s: (int) not found\n", i_event);
        }
#endif

        mEventData.field_0x10 = 1;
        return 0;
    }
#if DEBUG
    if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
        OS_REPORT("camera: event: %16s: %d\n", i_event, *i_data);
    }
#endif

    return 1;
}

bool dCamera_c::getEvFloatData(f32* i_data, char* i_event) {
    if (chkFlag(0x20000000)) {
        int index = searchEventArgData(i_event);
        if (index == -1) {
            return 0;
        }

        *i_data = *(f32*)mEventData.mEventParams[index].value;
    } else if (dComIfGp_evmng_getMySubstanceNum(mEventData.mStaffIdx, i_event) != 0) {
        *i_data = *dComIfGp_evmng_getMyFloatP(mEventData.mStaffIdx, i_event);
    } else {
#if DEBUG
        if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
            OS_REPORT("camera: event: %16s: (float) not found\n", i_event);
        }
#endif

        mEventData.field_0x10 = 1;
        return 0;
    }
#if DEBUG
    if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
        OS_REPORT("camera: event: %16s: %f\n", i_event, *i_data);
    }
#endif

    return 1;
}

int dCamera_c::getEvFloatListData(f32** i_data, char* i_event) {
    int num = 0;

    if (chkFlag(0x20000000)) {
        int index = searchEventArgData(i_event);
        if (index == -1) {
            return 0;
        }

        *i_data = (f32*)mEventData.mEventParams[index].value;
        num = mEventData.mEventParams[index].field_0x10;
    } else if ((num = dComIfGp_evmng_getMySubstanceNum(mEventData.mStaffIdx, i_event))) {
        *i_data = dComIfGp_evmng_getMyFloatP(mEventData.mStaffIdx, i_event);
    } else {
#if DEBUG
        if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
            OS_REPORT("camera: event: %16s: (float[]) not found\n", i_event);
        }
#endif
        mEventData.field_0x10 = 1;
        return 0;
    }

#if DEBUG
    if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
        OS_REPORT("camera: event: %16s: %f ... [%d]\n", i_event, **i_data, num);
    }
#endif

    return num;
}

int dCamera_c::getEvXyzListData(cXyz** i_data, char* i_event) {
    int num = 0;

    if (chkFlag(0x20000000)) {
        int index = searchEventArgData(i_event);
        if (index == -1) {
            return 0;
        }

        *i_data = (cXyz*)mEventData.mEventParams[index].value;
        return mEventData.mEventParams[index].field_0x10;
    } else if ((num = dComIfGp_evmng_getMySubstanceNum(mEventData.mStaffIdx, i_event))) {
        *i_data = dComIfGp_evmng_getMyXyzP(mEventData.mStaffIdx, i_event);
    } else {
#if DEBUG
        if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
            OS_REPORT("camera: event: %16s: (xyz[]) not found\n", i_event);
        }
#endif
        mEventData.field_0x10 = 1;
        return 0;
    }

#if DEBUG
    if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
        OS_REPORT("camera: event: %16s: %f %f %f ... [%d]\n", i_event, (*i_data)->x, (*i_data)->y, (*i_data)->z, num);
    }
#endif

    return num;
}

char* dCamera_c::getEvStringPntData(char* i_event) {
    char* string = NULL;

    if (chkFlag(0x20000000)) {
        int index = searchEventArgData(i_event);
        if (index == -1) {
            return 0;
        }

        string = (char*)mEventData.mEventParams[index].value;
    } else if (dComIfGp_evmng_getMySubstanceNum(mEventData.mStaffIdx, i_event)) {
        string = dComIfGp_evmng_getMyStringP(mEventData.mStaffIdx, i_event);
    } else {
#if DEBUG
        if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
            OS_REPORT("camera: event: %16s: (string) not found\n", i_event);
        }
#endif
        mEventData.field_0x10 = 1;
        return 0;
    }

#if DEBUG
    if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
        OS_REPORT("camera: event: %16s: %s\n", i_event, string);
    }
#endif

    return string;
}

bool dCamera_c::getEvIntData(int* i_data, char* i_event, int param_2) {
    if (chkFlag(0x20000000)) {
        int index = searchEventArgData(i_event);
        if (index == -1) {
            *i_data = param_2;
        } else {
            *i_data = *(int*)mEventData.mEventParams[index].value;
        }
    } else if (dComIfGp_evmng_getMySubstanceNum(mEventData.mStaffIdx, i_event) != 0) {
        *i_data = *dComIfGp_evmng_getMyIntegerP(mEventData.mStaffIdx, i_event);
    } else {
        *i_data = param_2;
#if DEBUG
        if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
            OS_REPORT("camera: event: %16s: %d (d)\n", i_event, *i_data);
        }
#endif

        return 0;
    }
#if DEBUG
    if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
        OS_REPORT("camera: event: %16s: %d\n", i_event, *i_data);
    }
#endif

    return 1;
}

bool dCamera_c::getEvFloatData(f32* i_data, char* i_event, f32 param_2) {
    if (chkFlag(0x20000000)) {
        int index = searchEventArgData(i_event);
        if (index == -1) {
            *i_data = param_2;
        } else {
            *i_data = *(f32*)mEventData.mEventParams[index].value;
        }
    } else if (dComIfGp_evmng_getMySubstanceNum(mEventData.mStaffIdx, i_event) != 0) {
        *i_data = *dComIfGp_evmng_getMyFloatP(mEventData.mStaffIdx, i_event);
    } else {
        *i_data = param_2;
#if DEBUG
        if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
            OS_REPORT("camera: event: %16s: %f (d)\n", i_event, *i_data);
        }
#endif
        return 0;
    }
#if DEBUG
    if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
        OS_REPORT("camera: event: %16s: %f\n", i_event, *i_data);
    }
#endif

    return 1;
}

bool dCamera_c::getEvXyzData(cXyz* i_data, char* i_event, cXyz param_2) {
    if (chkFlag(0x20000000)) {
        int index = searchEventArgData(i_event);
        if (index == -1) {
            *i_data = param_2;
        } else {
            *i_data = *(cXyz*)mEventData.mEventParams[index].value;
        }
    } else if (dComIfGp_evmng_getMySubstanceNum(mEventData.mStaffIdx, i_event) != 0) {
        *i_data = *dComIfGp_evmng_getMyXyzP(mEventData.mStaffIdx, i_event);
    } else {
        *i_data = param_2;
#if DEBUG
        if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
            OS_REPORT("camera: event: %16s: %f %f %f (d)\n", i_event, i_data->x, i_data->y, i_data->z);
        }
#endif
        return 0;
    }
#if DEBUG
    if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
        OS_REPORT("camera: event: %16s: %f %f %f\n", i_event, i_data->x, i_data->y, i_data->z);
    }
#endif

    return 1;
}

bool dCamera_c::getEvStringData(char* i_data, char* i_event, char* param_2) {
    if (chkFlag(0x20000000)) {
        int index = searchEventArgData(i_event);
        if (index == -1) {
            strcpy(i_data, param_2);
        } else {
            strcpy(i_data, (char*)mEventData.mEventParams[index].value);
        }
    } else if (dComIfGp_evmng_getMySubstanceNum(mEventData.mStaffIdx, i_event) != 0) {
        strcpy(i_data, dComIfGp_evmng_getMyStringP(mEventData.mStaffIdx, i_event));
    } else {
        strcpy(i_data, param_2);
#if DEBUG
        if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
            OS_REPORT("camera: event: %16s: %s (d)\n", i_event, i_data);
        }
#endif
        return 0;
    }
#if DEBUG
    if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
        OS_REPORT("camera: event: %16s: %s\n", i_event, i_data);
    }
#endif

    return 1;
}

char* dCamera_c::getEvStringPntData(char* i_event, char* param_1) {
    char* string = NULL;

    if (chkFlag(0x20000000)) {
        int index = searchEventArgData(i_event);
        if (index == -1) {
            string = param_1;
        } else {
            string = (char*)mEventData.mEventParams[index].value;
        }
    } else if (dComIfGp_evmng_getMySubstanceNum(mEventData.mStaffIdx, i_event)) {
        string = dComIfGp_evmng_getMyStringP(mEventData.mStaffIdx, i_event);
    } else {
        string = param_1;
#if DEBUG
        if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
            OS_REPORT("camera: event: %16s: %s (d)\n", i_event, string);
        }
#endif
        return string;
    }

#if DEBUG
    if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
        OS_REPORT("camera: event: %16s: %s\n", i_event, string);
    }
#endif

    return string;
}

fopAc_ac_c* dCamera_c::getEvActor(char* i_event) {
    char* string = getEvStringPntData(i_event);
    if (string == NULL) {
        return NULL;
    }

    fopAc_ac_c* actor;
    if (*(u32*)string == '@PLA') {
        actor = mpPlayerActor;
    } else if (*(u32*)string == '@STA') {
        actor = dComIfGp_event_getPt1();
    } else if (*(u32*)string == '@PAR') {
        actor = dComIfGp_event_getPt2();
    } else if (*(u32*)string == '@TAL') {
        actor = dComIfGp_event_getTalkPartner();
    } else if (*(u32*)string == '@DOO') {
        actor = dComIfGp_event_getDoorPartner();
    } else if (*(u32*)string == '@TAR' || *(u32*)string == '@ITE') {
        actor = dComIfGp_event_getItemPartner();
    } else if (*(u32*)string == 'Link') {
        actor = dComIfGp_getLinkPlayer();
    } else {
        actor = fopAcM_searchFromName4Event(string, -1);
    }

#if DEBUG
    if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
        if (actor != NULL) {
            char name[16];
            fopAcM_getNameString(actor, name);
            OS_REPORT("camera: event:                   = %s\n", name);
        } else {
            OS_REPORT("camera: event:                   = (Who?)\n");
        }
    }
#endif

    return actor;
}

fopAc_ac_c* dCamera_c::getEvActor(char* i_event, char* param_1) {
    char string[16];
    string[0] = 0;
    getEvStringData(string, i_event, param_1);
    char* name_str = string;

    fopAc_ac_c* actor;
    if (*(u32*)string == '@PLA') {
        actor = mpPlayerActor;
    } else if (*(u32*)string == '@STA') {
        actor = dComIfGp_event_getPt1();
    } else if (*(u32*)string == '@PAR') {
        actor = dComIfGp_event_getPt2();
    } else if (*(u32*)string == '@TAL') {
        actor = dComIfGp_event_getTalkPartner();
    } else if (*(u32*)string == '@DOO') {
        actor = dComIfGp_event_getDoorPartner();
    } else if (*(u32*)string == '@TAR' || *(u32*)string == '@ITE') {
        actor = dComIfGp_event_getItemPartner();
    } else if (*(u32*)string == 'Link') {
        actor = dComIfGp_getLinkPlayer();
    } else {
        actor = fopAcM_searchFromName4Event(name_str, -1);
    }

#if DEBUG
    if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
        if (actor != NULL) {
            char name[16];
            fopAcM_getNameString(actor, name);
            OS_REPORT("camera: event:                   = %s\n", name);
        } else {
            OS_REPORT("camera: event:                   = (Who?)\n");
        }
    }
#endif

    return actor;
}

bool dCamera_c::pauseEvCamera() {
    EventData* event = (EventData*)mWork;

    if (mCurCamStyleTimer == 0) {
        Reset();
        mStyleSettle.mFinished = 1;
        event->field_0x0 = getEvIntData(&event->field_0x8, "Timer", -1);
        getEvIntData(&event->field_0x4, "Stay", 0);
    }

    if (event->field_0x4 != 0) {
        setFlag(1);
    }

    if (event->field_0x0 && mCurCamStyleTimer < event->field_0x8) {
        return false;
    }

    return true;
}

namespace {
    /* 804253B4-804253C0 0520D4 000C+00 2/3 0/0 0/0 .bss WolfAdditionVec__25@unnamed@d_ev_camera_cpp@*/
    static cXyz WolfAdditionVec(0.0f, -70.0f, 70.0f);
}

bool dCamera_c::fixedFrameEvCamera() {
    FixedFrameEvData* fframe_p = (FixedFrameEvData*)mWork;
    if (mCurCamStyleTimer == 0) {
        cXyz sp38, sp44;
        getEvXyzData(&sp44, "Eye", mEye);
        getEvXyzData(&sp38, "Center", mCenter);
        getEvXyzData(&fframe_p->mBasePos, "BasePos", cXyz::Zero);
        getEvFloatData(&fframe_p->field_0x1c, "Fovy", mFovy);
        fframe_p->field_0x30 = getEvFloatData(&fframe_p->field_0x20, "Bank", 0.0f);
        fframe_p->field_0x0 = getEvIntData(&fframe_p->mTimer, "Timer", -1);
        getEvStringData(fframe_p->mRelUseMask, "RelUseMask", "oo");

#if DEBUG
        if (strlen(fframe_p->mRelUseMask) != 2) {
            OSReport("camera: event:                   bad length -> xx\n");
            strcpy(fframe_p->mRelUseMask, "xx");
            JUTAssertion::showAssert(JUTAssertion::getSDevice(), "d_ev_camera.cpp", 0x32e, "0");
            OSPanic("d_ev_camera.cpp", 0x32e, "Halt");
        }
#endif

        fframe_p->mpRelActor = getEvActor("RelActor");
        if (fframe_p->mpRelActor && fframe_p->mRelUseMask[0] == 'o') {
            fframe_p->field_0x10 = relationalPos(fframe_p->mpRelActor, &sp38);
        } else if (fframe_p->mpRelActor && fframe_p->mRelUseMask[0] == 'w') {
            if (mIsWolf == 1) {
                sp38 += WolfAdditionVec;
            }

            fframe_p->field_0x10 = relationalPos(fframe_p->mpRelActor, &sp38);
        } else if (fframe_p->mpRelActor && fframe_p->mRelUseMask[0] == 'W') {
            if (mIsWolf == 1) {
                sp38 -= WolfAdditionVec;
            }

            fframe_p->field_0x10 = relationalPos(fframe_p->mpRelActor, &sp38);
        } else if (fframe_p->mRelUseMask[0] == 'n') {
            cSGlobe sg28 = mEye - positionOf(fframe_p->mpRelActor);
            cSAngle sa260 = sg28.U() - directionOf(fframe_p->mpRelActor);
            if (sa260 < cSAngle::_0) {
                sp38.x = -sp38.x;
            }

            fframe_p->field_0x10 = relationalPos(fframe_p->mpRelActor, &sp38);
        } else if (fframe_p->mRelUseMask[0] == 'p') {
            cXyz mRelPos = relationalPos(fframe_p->mpRelActor, &sp38);
            f32 fVar1 = cXyz(mRelPos - positionOf(mpPlayerActor)).abs();
            sp38.x = -sp38.x;
            mRelPos = relationalPos(fframe_p->mpRelActor, &sp38);
            f32 fVar2 = cXyz(mRelPos - positionOf(mpPlayerActor)).abs();
            if (fVar1 > fVar2) {
                sp38.x = -sp38.x;
            }

            fframe_p->field_0x10 = relationalPos(fframe_p->mpRelActor, &sp38);
        } else if (fframe_p->mRelUseMask[0] == 't') {
            fframe_p->field_0x10 = attentionPos(fframe_p->mpRelActor) + sp38;
        } else {
            fframe_p->field_0x10 = sp38;
        }

        if (fframe_p->mpRelActor && fframe_p->mRelUseMask[1] == 'o') {
            fframe_p->field_0x4 = relationalPos(fframe_p->mpRelActor, &sp44);
        } else if (fframe_p->mpRelActor && fframe_p->mRelUseMask[1] == 'w') {
            if (mIsWolf == 1) {
                sp44 += WolfAdditionVec;
            }

            fframe_p->field_0x4 = relationalPos(fframe_p->mpRelActor, &sp44);
        } else if (fframe_p->mpRelActor && fframe_p->mRelUseMask[1] == 'W') {
            if (mIsWolf == 1) {
                sp44 -= WolfAdditionVec;
            }

            fframe_p->field_0x4 = relationalPos(fframe_p->mpRelActor, &sp44);
        } else if (fframe_p->mpRelActor && fframe_p->mRelUseMask[1] == 'r') {
            if ((mTicks & 1) != 0) {
                sp44.x = -sp44.x;
            }

            fframe_p->field_0x4 = relationalPos(fframe_p->mpRelActor, &sp44);
            if (lineBGCheck(&fframe_p->field_0x10, &fframe_p->field_0x4, 0x4007)) {
                sp44.x = -sp44.x;
            }

            fframe_p->field_0x4 = relationalPos(fframe_p->mpRelActor, &sp44);
        } else if (fframe_p->mRelUseMask[1] == 'n') {
            cSGlobe cStack_260 = mEye - positionOf(fframe_p->mpRelActor);
            cSAngle cStack_26c = cStack_260.U() - directionOf(fframe_p->mpRelActor);
            if (cStack_26c < cSAngle::_0) {
                sp44.x = -sp44.x;
            }

            fframe_p->field_0x4 = relationalPos(fframe_p->mpRelActor, &sp44);
            if (lineBGCheck(&fframe_p->field_0x10, &fframe_p->field_0x4, 0x4007)) {
                sp44.x = -sp44.x;
            }
        } else if (fframe_p->mRelUseMask[1] == 'p') {
            cXyz mRelPos2 = relationalPos(fframe_p->mpRelActor, &sp44);
            f32 fVar3 = cXyz(mRelPos2 - positionOf(mpPlayerActor)).abs();
            sp44.x = -sp44.x;
            mRelPos2 = relationalPos(fframe_p->mpRelActor, &sp44);
            f32 fVar4 = cXyz(mRelPos2 - positionOf(mpPlayerActor)).abs();
            if (fVar3 > fVar4) {
                sp44.x = -sp44.x;
            }

            fframe_p->field_0x4 = relationalPos(fframe_p->mpRelActor, &sp44);
        } else if (fframe_p->mRelUseMask[1] == 116) {
            fframe_p->field_0x4 = attentionPos(fframe_p->mpRelActor) + sp44;
        } else {
            fframe_p->field_0x4 = sp44;
        }

        mStyleSettle.mFinished = true;
    }

    mViewCache.mCenter = fframe_p->field_0x10;
    mViewCache.mEye = fframe_p->field_0x4;
    mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
    mViewCache.mFovy = fframe_p->field_0x1c;

    if (fframe_p->field_0x30) {
        mViewCache.mBank = cAngle::d2s(fframe_p->field_0x20);
        setFlag(0x400);
    }

    if (fframe_p->field_0x0 && mCurCamStyleTimer < fframe_p->mTimer) {
        return 0;
    }

    return 1;
}

bool dCamera_c::stokerEvCamera() {
    StokerData* stoker_p = (StokerData*)mWork;
    if (mCurCamStyleTimer == 0) {
        cXyz unused_0, unused_1;
        getEvXyzData(&stoker_p->field_0x4, "EyeGap", cXyz::Zero);
        getEvXyzData(&stoker_p->field_0x10, "CtrGap", cXyz::Zero);
        getEvFloatData(&stoker_p->field_0x20, "EyeCus", 1.0f);
        getEvFloatData(&stoker_p->field_0x1c, "CtrCus", 1.0f);
        getEvFloatData(&stoker_p->field_0x24, "Fovy", mFovy);

        stoker_p->field_0x1 = getEvFloatData(&stoker_p->field_0x28, "Bank", 0.0f);
        stoker_p->field_0x0 = getEvIntData(&stoker_p->field_0x40, "Timer", -1);
        stoker_p->field_0x2c = getEvActor("Stoker", "@STARTER");
        stoker_p->field_0x30 = getEvActor("Target", "@PLAYER");

        if (stoker_p->field_0x2c == 0 || stoker_p->field_0x30 == NULL) {
            OS_REPORT("camera: event: error: stoker or target actor missing\n");
            return 1;
        }

        stoker_p->field_0x34 = fopAcM_GetID(stoker_p->field_0x2c);
        stoker_p->field_0x38 = fopAcM_GetID(stoker_p->field_0x30);
        mStyleSettle.mFinished = true;
    }

    cSGlobe cStack_d0;
    if (stoker_p->field_0x30) {
        if (fopAcM_SearchByID(stoker_p->field_0x38) == NULL) {
            OS_REPORT("camera: event: error: target actor dead\n");
            return 1;
        }
    
        cStack_d0.Val(stoker_p->field_0x10);
        cStack_d0.V(cStack_d0.V() + stoker_p->field_0x30->shape_angle.x);
        cStack_d0.U(cStack_d0.U() + stoker_p->field_0x30->shape_angle.y);

        cXyz cStack_38 = attentionPos(stoker_p->field_0x30) + cStack_d0.Xyz();
        mViewCache.mCenter += (cStack_38 - mViewCache.mCenter) * stoker_p->field_0x1c;
    }

    if (stoker_p->field_0x2c) {
        if (!fopAcM_SearchByID(stoker_p->field_0x34)) {
            OS_REPORT("camera: event: error: stoker actor dead\n");
            return 1;
        }

        cStack_d0.Val(stoker_p->field_0x4);
        cStack_d0.V(cStack_d0.V() + stoker_p->field_0x2c->shape_angle.x);
        cStack_d0.U(cStack_d0.U() + stoker_p->field_0x2c->shape_angle.y);
        cXyz tmp_vec = attentionPos(stoker_p->field_0x2c) + cStack_d0.Xyz();
        mViewCache.mEye += (tmp_vec - mViewCache.mEye) * stoker_p->field_0x20;
    }

    mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
    mViewCache.mFovy = stoker_p->field_0x24;

    if (stoker_p->field_0x1) {
        cAngle this_00;
        mViewCache.mBank = this_00.d2s(stoker_p->field_0x28);
        setFlag(0x400);
    }

    if (stoker_p->field_0x0 && mCurCamStyleTimer < stoker_p->field_0x40) {
        return 0;
    }

    return 1;
}

bool dCamera_c::rollingEvCamera() {
    struct RollingData* rolling_p = (RollingData*)mWork;
    if (mCurCamStyleTimer == 0) {
        getEvXyzData(&rolling_p->mEye, "Eye", mEye);
        getEvXyzData(&rolling_p->mCenter, "Center", mCenter);
        getEvFloatData(&rolling_p->mCtrCus, "CtrCus", 1.0f);
        getEvIntData(&rolling_p->mTransType, "TransType", 0);
        getEvFloatData(&rolling_p->mFovy, "Fovy", mFovy);
        rolling_p->field_0x1 = getEvFloatData(&rolling_p->mBank, "Bank", 0.0f);
        getEvFloatData(&rolling_p->mRoll, "Roll", 2.0f);
        getEvFloatData(&rolling_p->mRadiusAdd, "RadiusAdd", 0.0f);
        cSGlobe cStack_2ac = rolling_p->mEye - rolling_p->mCenter;
        getEvFloatData(&rolling_p->mLatitude, "Latitude", cStack_2ac.V().Degree());
        rolling_p->field_0x0 = getEvIntData(&rolling_p->mTimer, "Timer", -1);
        getEvStringData(rolling_p->mRelActor, "RelUseMask", "oo");

        rolling_p->mpRelActor = getEvActor("RelActor");
        if (rolling_p->mpRelActor) {
            if (rolling_p->mRelActor[0] == 'o') {
                rolling_p->field_0x10 = relationalPos(rolling_p->mpRelActor, &rolling_p->mCenter);

            } else if (rolling_p->mRelActor[0] == 'n') {
                cSGlobe cStack_2b4 = mEye - positionOf(rolling_p->mpRelActor);
                cSAngle acStack_2f0 = cStack_2b4.U() - directionOf(rolling_p->mpRelActor);
                if (acStack_2f0 < cSAngle::_0) {
                    rolling_p->mCenter.x = -rolling_p->mCenter.x;
                }
                rolling_p->field_0x10 = relationalPos(rolling_p->mpRelActor, &rolling_p->mCenter);

            } else if (rolling_p->mRelActor[0] == 'p') {
                cXyz sp2E0 = relationalPos(rolling_p->mpRelActor, &rolling_p->mCenter);
                f32 fVar1 = cXyz(sp2E0 - positionOf(mpPlayerActor)).abs();
                rolling_p->mCenter.x = -rolling_p->mCenter.x;
                sp2E0 = relationalPos(rolling_p->mpRelActor, &rolling_p->mCenter);
                f32 fVar2 = cXyz(sp2E0 - positionOf(mpPlayerActor)).abs();
                if (fVar1 > fVar2) {
                    rolling_p->mCenter.x = -rolling_p->mCenter.x;
                }
                rolling_p->field_0x10 = relationalPos(rolling_p->mpRelActor, &rolling_p->mCenter);

            }  
        } else {
            rolling_p->field_0x10 = rolling_p->mCenter;
        }

        if (rolling_p->mpRelActor && rolling_p->mRelActor[1] == 'o') {
            rolling_p->field_0x4 = relationalPos(rolling_p->mpRelActor, &rolling_p->mEye);

        } else if (rolling_p->mpRelActor && rolling_p->mRelActor[1] == 'r') {
            if ((mTicks & 1) != 0) {
                rolling_p->mEye.x = -rolling_p->mEye.x;
            }
            rolling_p->field_0x4 = relationalPos(rolling_p->mpRelActor, &rolling_p->mEye);
            if (lineBGCheck(&rolling_p->field_0x10, &rolling_p->field_0x4, 0x4007)) {
                rolling_p->mEye.x = -rolling_p->mEye.x;
            }
            rolling_p->field_0x4 = relationalPos(rolling_p->mpRelActor, &rolling_p->mEye);

        } else if (rolling_p->mRelActor[1] == 'n') {
            cSGlobe cStack_2e8 = mEye - positionOf(rolling_p->mpRelActor);
            cSAngle acStack_2f8 = cStack_2e8.U() - directionOf(rolling_p->mpRelActor);;
            if (acStack_2f8 < cSAngle::_0) {
                rolling_p->mEye.x = -rolling_p->mEye.x;
            }
            rolling_p->field_0x4 = relationalPos(rolling_p->mpRelActor, &rolling_p->mEye);

        } else if (rolling_p->mRelActor[1] == 'p') {
            cXyz sp2D4 = relationalPos(rolling_p->mpRelActor, &rolling_p->mEye);
            f32 fVar3 = cXyz(sp2D4 - positionOf(mpPlayerActor)).abs();
            rolling_p->mEye.x = -rolling_p->mEye.x;
            sp2D4 = relationalPos(rolling_p->mpRelActor, &rolling_p->mEye);
            f32 fVar4 = cXyz(sp2D4 - positionOf(mpPlayerActor)).abs();
            if (fVar3 > fVar4) {
                rolling_p->mEye.x = -rolling_p->mEye.x;
            }
            rolling_p->field_0x4 = relationalPos(rolling_p->mpRelActor, &rolling_p->mEye);

        } else {
            rolling_p->field_0x4 = rolling_p->mEye;
        }

        mStyleSettle.mFinished = true;
    }

    if ((rolling_p->mTransType == 1 || rolling_p->mTransType == 2) && rolling_p->mpRelActor) {
        if (rolling_p->mRelActor[0] == 'o') {
            rolling_p->field_0x10 = relationalPos(rolling_p->mpRelActor, &rolling_p->mCenter);

        } else if (rolling_p->mRelActor[0] == 'n') {
            cSGlobe cStack_2f0 = mEye - positionOf(rolling_p->mpRelActor);
            cSAngle acStack_2fc = cStack_2f0.U() - directionOf(rolling_p->mpRelActor);
            if (acStack_2fc < cSAngle::_0) {
                rolling_p->mCenter.x = -rolling_p->mCenter.x;
            }
            rolling_p->field_0x10 = relationalPos(rolling_p->mpRelActor, &rolling_p->mCenter);

        } else if (rolling_p->mRelActor[0] == 'p') {
            cXyz sp2C8 = relationalPos(rolling_p->mpRelActor, &rolling_p->mCenter);
            f32 fVar5 = cXyz(sp2C8 - positionOf(mpPlayerActor)).abs();
            rolling_p->mCenter.x = -rolling_p->mCenter.x;
            sp2C8 = relationalPos(rolling_p->mpRelActor, &rolling_p->mCenter);
            f32 fVar6 = cXyz(sp2C8 - positionOf(mpPlayerActor)).abs();
            if (fVar5 > fVar6) {
                rolling_p->mCenter.x = -rolling_p->mCenter.x;
            }
            rolling_p->field_0x10 = relationalPos(rolling_p->mpRelActor, &rolling_p->mCenter);

        }
    }

    mViewCache.mCenter += (rolling_p->field_0x10 - mViewCache.mCenter) * rolling_p->mCtrCus;
    mViewCache.mDirection.Val(rolling_p->field_0x4 - rolling_p->field_0x10);

    if (rolling_p->mTransType == 2) {
        mViewCache.mDirection.V(rolling_p->mLatitude);
    }

    mViewCache.mDirection.U(mViewCache.mDirection.U() + (cSAngle)(mCurCamStyleTimer * rolling_p->mRoll));
    mViewCache.mDirection.R(mCurCamStyleTimer * rolling_p->mRadiusAdd + mViewCache.mDirection.R());

    mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
    mViewCache.mFovy = rolling_p->mFovy;

    if (rolling_p->field_0x1) {
        cAngle this_00;
        mViewCache.mBank = this_00.d2s(rolling_p->mBank);
        setFlag(0x400);
    }

    if (rolling_p->field_0x0 && mCurCamStyleTimer < rolling_p->mTimer) {
        return 0;
    }

    return 1;
}

namespace {
    /* 804253CC-804253D8 0520EC 000C+00 1/2 0/0 0/0 .bss MidnaAdditionVec__25@unnamed@d_ev_camera_cpp@ */
    static cXyz MidnaAdditionVec(0.0f, 0.0f, 70.0f);

    /* 8008E750-8008E774 089090 0024+00 1/1 0/0 0/0 .text isRelChar__25@unnamed@d_ev_camera_cpp@Fc */
    static inline bool isRelChar(char param_1) {
        return param_1 != '-' && param_1 != 'x';
    }
} // namespace

bool dCamera_c::fixedPositionEvCamera() {
    static cXyz DefaultGap(cXyz::Zero);
    FixedPosData* fpos_p = (FixedPosData*)mWork;
    bool rv = true;

    if (mCurCamStyleTimer == 0) {
        cXyz unused_xyz, sp24;
        getEvXyzData(&fpos_p->field_0x10, "CtrGap", DefaultGap);
        getEvXyzData(&sp24, "Eye", mEye);
        getEvFloatData(&fpos_p->field_0x28, "Fovy", mFovy);
        getEvFloatData(&fpos_p->field_0x30, "CtrCus", 1.0f);
        getEvFloatData(&fpos_p->field_0x38, "Radius", 100000.0f);
        getEvFloatData(&fpos_p->field_0x34, "StartRadius", fpos_p->field_0x38);
        fpos_p->field_0x1 = getEvFloatData(&fpos_p->field_0x2c, "Bank", 0.0f);
        getEvStringData(&fpos_p->field_0x48, "RelUseMask", "o");
        fpos_p->field_0x0 = getEvIntData(&fpos_p->field_0x4c, "Timer", -1);

        if ((fpos_p->field_0x40 = getEvActor("Target", "@PLAYER")) == NULL) {
            OS_REPORT("camera: event: error: target actor missing\n");
            return 1;
        }

        fpos_p->field_0x44 = fopAcM_GetID(fpos_p->field_0x40);
        fpos_p->field_0x3c = getEvActor("RelActor");

        if (fpos_p->field_0x3c && isRelChar(fpos_p->field_0x48)) {
            fpos_p->field_0x4 = relationalPos(fpos_p->field_0x3c, &sp24);
        } else {
            fpos_p->field_0x4 = sp24;        
        }

        fpos_p->field_0x1c = mCenter;
        mStyleSettle.mFinished = true;
    }

    if (fopAcM_SearchByID(fpos_p->field_0x44) == NULL) {
        OS_REPORT("camera: event: error: target actor dead\n");
        return 1;
    }

    fpos_p->field_0x1c = relationalPos(fpos_p->field_0x40, &fpos_p->field_0x10);
    mViewCache.mCenter += (fpos_p->field_0x1c - mViewCache.mCenter) * fpos_p->field_0x30;
    mViewCache.mEye = fpos_p->field_0x4;
    mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);

    f32 fVar1 = fpos_p->field_0x38;
    if (fpos_p->field_0x0 && mCurCamStyleTimer < fpos_p->field_0x4c) {
        fVar1 = fpos_p->field_0x34 + (fpos_p->field_0x38 - fpos_p->field_0x34)
                                    * (mCurCamStyleTimer / f32(fpos_p->field_0x4c));
        rv = false;
    }

    if (mViewCache.mDirection.R() > fVar1) {
        mViewCache.mDirection.R(fVar1);
        mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
    }

    mViewCache.mFovy = fpos_p->field_0x28;
    if (fpos_p->field_0x1) {
        cAngle this_00;
        mViewCache.mBank = this_00.d2s(fpos_p->field_0x2c);
        setFlag(0x400);
    }

    if (rv != 0) {
        mStyleSettle.mFinished = true;
    }

    return rv;
}

bool dCamera_c::uniformTransEvCamera() {
    transEvCamera(1);
}

bool dCamera_c::uniformBrakeEvCamera() {
    transEvCamera(2);
}

bool dCamera_c::uniformAcceleEvCamera() {
    transEvCamera(3);
}

namespace {
    inline static bool lineCollisionCheck(cXyz param_0, cXyz param_1, fopAc_ac_c* param_2,
                                          fopAc_ac_c* param_3, fopAc_ac_c* param_4) {
        return dComIfG_Ccsp()->ChkCamera(param_0, param_1, 15.0f, param_2, param_3, param_4);
    }
} // namespace

f32 dummy_lit_3871(int val) {
    f32 my_vec[3] = {0.0f, 0.0f, 0.0f};
    return my_vec[val];
}

bool dCamera_c::transEvCamera(int param_1) {
    TransData* trans = (TransData*)mWork;
    f32 mid_val;

    struct {
        cXyz mXyz_0;
        cXyz mXyz_1;
    } pos, pos2;

    bool rv = 0;

    f32 lit_5984[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
    f32 lit_5985[7] = { 0.0f, 0.0f, 0.4f, 0.7f, 0.9f, 1.0f, 1.0f };
    f32 lit_5986[7] = { 0.0f, 0.0f, 0.1f, 0.3f, 0.6f, 1.0f, 1.0f };
    f32* lit_5987[5] = { NULL, lit_5984, lit_5985, lit_5986, NULL };

    int local_d0[5] = { 0, 4, 7, 7, 0 };

    cXyz my_vec_0, my_vec_1;
    f32 my_main_f32;
    if (mCurCamStyleTimer == 0) {
        if (!getEvIntData(&trans->mTimer, "Timer")) {
            OS_REPORT("camera: event: error: UNITRANS no Timer\n");
            return 1;
        }

        getEvIntData(&trans->mBSpCurve, "BSpCurve", param_1);
        if (trans->mBSpCurve != 0) {
            mEventData.field_0xf0.Init(local_d0[trans->mBSpCurve], trans->mTimer + 1);
            mEventData.field_0xf0.Step();
        } else {
            int float_list_data = getEvFloatListData(&trans->mBSpPoints, "BSpPoints");
            if (float_list_data != 0) {
                mEventData.field_0xf0.Init(float_list_data, trans->mTimer + 1);
                mEventData.field_0xf0.Step();
                trans->mBSpCurve = -1;
            }
        }

        int slot;
        if (!getEvIntData(&slot, "EndSlot")) {
            getEvXyzData(&trans->mEye, "Eye", mEye);
            getEvXyzData(&trans->mCenter, "Center", mCenter);
            getEvFloatData(&trans->mFovy, "Fovy", mFovy);
            trans->field_0x68 = getEvFloatData(&trans->field_0x3c, "Bank", mBank.Degree());
        } else {
            dCamInfo_c* ptr1 = &mSavedViewStack[slot];
            trans->mCenter = (*ptr1).mCenter;
            trans->mEye = (*ptr1).mEye;
            trans->mFovy = (*ptr1).mFovy;
            trans->field_0x3c = (*ptr1).mBank;
            if (float(s16(ptr1->mBank)) != 0.0f) {
                trans->field_0x68 = true;
            }
        }

        if (!getEvIntData(&slot, "StartSlot")) {
            getEvXyzData(&trans->mStartEye, "StartEye", mEye);
            getEvXyzData(&trans->mStartCenter, "StartCenter", mCenter);
            getEvFloatData(&trans->mStartFovy, "StartFovy", mFovy);
            trans->field_0x68 |= getEvFloatData(&trans->field_0x1c, "StartBank", mBank.Degree());
        } else {
            dCamInfo_c* ptr2 = &mSavedViewStack[slot];
            trans->mStartCenter = (*ptr2).mCenter;
            trans->mStartEye = (*ptr2).mEye;
            trans->mStartFovy = (*ptr2).mFovy;
            trans->field_0x1c = (*ptr2).mBank;
            if (float(s16(ptr2->mBank)) != 0.0f) {
                trans->field_0x68 = true;
            }
        }

        getEvIntData(&trans->mTransType, "TransType", 0);
        trans->mRelActor = getEvActor("RelActor");
        getEvStringData(&trans->mRelUseMask, "RelUseMask", "--oo");
        getEvFloatData(&trans->mCushion, "Cushion", 1.0f);

        if (trans->mRelActor) {
            trans->mRelActorID = fopAcM_GetID(trans->mRelActor);
            if (mIsWolf == 1 && trans->mRelActor == mpPlayerActor) {
                daPy_py_c* mPlayer = (daPy_py_c*)mpPlayerActor;
                daMidna_c* mMidna =  (daMidna_c*) daPy_py_c::getMidnaActor();
                cXyz mAdditionVec = WolfAdditionVec;
                if (mPlayer->checkMidnaRide() != 0 && !mMidna->checkNoDraw()) {
                    mAdditionVec = MidnaAdditionVec;
                }

                if (trans->mRelUseMask == 119) {
                    trans->mStartCenter += mAdditionVec;
                }
                if (trans->mRelUseMask == 87) {
                    trans->mStartCenter -= mAdditionVec;
                }

                if (trans->field_0x49 == 119) {
                    trans->mStartEye += mAdditionVec;
                }
                if (trans->field_0x49 == 87) {
                    trans->mStartEye -= mAdditionVec;
                }

                if (trans->field_0x4a == 119) {
                    trans->mCenter += mAdditionVec;
                }
                if (trans->field_0x4a == 87) {
                    trans->mCenter -= mAdditionVec;
                }

                if (trans->field_0x4b == 119) {
                    trans->mEye += mAdditionVec;
                }
                if (trans->field_0x4b == 87) {
                    trans->mEye -= mAdditionVec;
                }
            }

            if (trans->field_0x49 == 114) {
                my_vec_0 = relationalPos(trans->mRelActor, &trans->mStartCenter);
                if ((mTicks & 1) != 0) {
                    trans->mStartEye.x = -trans->mStartEye.x;
                }

                my_vec_1 = relationalPos(trans->mRelActor, &trans->mStartEye);
                if (lineBGCheck(&my_vec_0, &my_vec_1, 0x4007)) {
                    trans->mStartEye.x = -trans->mStartEye.x;
                }
            }

            if (trans->mRelUseMask == 110 || trans->field_0x49 == 110) {
                cSGlobe cStack_7b8(mEye - positionOf(trans->mRelActor));
                cSAngle acStack_898 = cStack_7b8.U() - directionOf(trans->mRelActor);
                if (acStack_898 < cSAngle::_0) {
                    if (trans->mRelUseMask == 110) {
                        trans->mStartCenter.x = -trans->mStartCenter.x;
                    }
                    if (trans->field_0x49 == 110) {
                        trans->mStartEye.x = -trans->mStartEye.x;
                    }
                }

                my_vec_0 = relationalPos(trans->mRelActor, &trans->mStartCenter);
                my_vec_1 = relationalPos(trans->mRelActor, &trans->mStartEye);
                if (lineBGCheck(&my_vec_0, &my_vec_1, 0x4007)) {
                    trans->mStartEye.x = -trans->mStartEye.x;
                }
            }

            if (trans->field_0x4a == 110 || trans->field_0x4b == 110) {
                cSGlobe cStack_7c0(mEye - positionOf(trans->mRelActor));
                cSAngle acStack_89c = cStack_7c0.U() - directionOf(trans->mRelActor);
                if (acStack_89c < cSAngle::_0) {
                    if (trans->field_0x4a == 110) {
                        trans->mCenter.x = -trans->mCenter.x;
                    }

                    if (trans->field_0x4b == 110) {
                        trans->mEye.x = -trans->mEye.x;
                    }
                }

                my_vec_0 = relationalPos(trans->mRelActor, &trans->mCenter);
                my_vec_1 = relationalPos(trans->mRelActor, &trans->mEye);
                if (lineBGCheck(&my_vec_0, &my_vec_1, 0x4007)) {
                    trans->mEye.x = -trans->mEye.x;
                }
            }

            if (trans->mRelUseMask == 78 || trans->field_0x49 == 78) {
                cSGlobe cStack_7c8(mEye - positionOf(trans->mRelActor));
                cSAngle acStack_8a0 = cStack_7c8.U() - directionOf(trans->mRelActor);
                if (acStack_8a0 > cSAngle::_0) {
                    if (trans->mRelUseMask == 78) {
                        trans->mStartCenter.x = -trans->mStartCenter.x;
                    }

                    if (trans->field_0x49 == 78) {
                        trans->mStartEye.x = -trans->mStartEye.x;
                    }
                }

                my_vec_0 = relationalPos(trans->mRelActor, &trans->mStartCenter);
                my_vec_1 = relationalPos(trans->mRelActor, &trans->mStartEye);
                if (lineBGCheck(&my_vec_0, &my_vec_1, 0x4007)) {
                    trans->mStartEye.x = -trans->mStartEye.x;
                }
            }

            if (trans->field_0x4a == 78 || trans->field_0x4b == 78) {
                cSGlobe cStack_7d0(mEye - positionOf(trans->mRelActor));
                cSAngle acStack_8a4 = cStack_7d0.U() - directionOf(trans->mRelActor);
                if (acStack_8a4 > cSAngle::_0) {
                    if (trans->field_0x4a == 78) {
                        trans->mCenter.x = -trans->mCenter.x;
                    }

                    if (trans->field_0x4b == 78) {
                        trans->mEye.x = -trans->mEye.x;
                    }
                }

                my_vec_0 = relationalPos(trans->mRelActor, &trans->mCenter);
                my_vec_1 = relationalPos(trans->mRelActor, &trans->mEye);
                if (lineBGCheck(&my_vec_0, &my_vec_1, 0x4007)) {
                    trans->mEye.x = -trans->mEye.x;
                }
            }

            if (trans->mRelUseMask == 102) {
                cSGlobe cStack_7d8(trans->mStartCenter);
                cStack_7d8.U(directionOf(trans->mRelActor) + cStack_7d8.U());
                trans->mStartCenter = attentionPos(trans->mRelActor) + cStack_7d8.Xyz();
                trans->mRelUseMask = 120;
            }

            if (trans->field_0x49 == 102) {
                cSGlobe cStack_7e0(trans->mStartEye);
                cStack_7e0.U(directionOf(trans->mRelActor) + cStack_7e0.U());
                trans->mStartEye = attentionPos(trans->mRelActor) + cStack_7e0.Xyz();
                trans->field_0x49 = 120;
            }

            if (trans->field_0x4a == 112) {
                cXyz sp114(trans->mCenter);
                cXyz sp120 = relationalPos(trans->mRelActor, &sp114);
                f32 fVar1 = cXyz(sp120 - positionOf(mpPlayerActor)).abs();
                sp114.x = -sp114.x;
                sp120 = relationalPos(trans->mRelActor, &sp114);
                f32 fVar2 = cXyz(sp120 - positionOf(mpPlayerActor)).abs();
                if (fVar1 < fVar2) {
                    trans->mCenter.x = -trans->mCenter.x;
                }
            } else if (trans->field_0x4a == 102) {
                cSGlobe cStack_7e8(trans->mCenter);
                cStack_7e8.U(directionOf(trans->mRelActor) + cStack_7e8.U());
                trans->mCenter = attentionPos(trans->mRelActor) + cStack_7e8.Xyz();
                trans->field_0x4a = 120;
            }

            if (trans->field_0x4b == 112) {
                cXyz sp12c = trans->mEye;
                cXyz sp138(relationalPos(trans->mRelActor, &sp12c));
                f32 fVar3 = cXyz(sp138 - positionOf(mpPlayerActor)).abs();
                sp12c.x = -sp12c.x;
                sp138 = relationalPos(trans->mRelActor, &sp12c);
                f32 fVar4 = cXyz(sp138 - positionOf(mpPlayerActor)).abs();
                if (fVar3 < fVar4) {
                    trans->mEye.x = -trans->mEye.x;
                }
            } else if (trans->field_0x4b == 114) {
                my_vec_0 = relationalPos(trans->mRelActor, &trans->mCenter);
                if ((mTicks & 1) != 0) {
                    trans->mEye.x = -trans->mEye.x;
                }

                my_vec_1 = relationalPos(trans->mRelActor, &trans->mEye);
                if (lineBGCheck(&my_vec_0, &my_vec_1, 0x4007)) {
                    trans->mEye.x = -trans->mEye.x;
                }
            } else if (trans->field_0x4b == 102) {
                cSGlobe cStack_7f0(trans->mEye);
                cStack_7f0.U(directionOf(trans->mRelActor) + cStack_7f0.U());
                trans->mEye = attentionPos(trans->mRelActor) + cStack_7f0.Xyz();
                trans->field_0x4b = 120;
            }
        } else {
            if (trans->field_0x4a == 97) {
                cXyz cStack_320 = dCamMath::xyzRotateY(trans->mCenter, cSAngle(mViewCache.mDirection.U().Inv()));
                trans->mCenter = mViewCache.mCenter + cStack_320;
            }

            if (trans->field_0x4b == 97) {
                cXyz cStack_32c = dCamMath::xyzRotateY(trans->mEye, cSAngle(mViewCache.mDirection.U().Inv()));
                trans->mEye = mViewCache.mEye + cStack_32c;
            }
        }

        trans->field_0x60 = mDirection.Invert();
        mStyleSettle.mFinished = true;
    }

    if (trans->mRelActor && fopAcM_SearchByID(trans->mRelActorID) == NULL) {
        OS_REPORT("camera: event: error: relational base actor dead\n");
        return 1;

    } else {
        if (mCurCamStyleTimer >= trans->mTimer) {
            rv = 1;
            my_main_f32 = 1.0f;
        } else if (trans->mBSpCurve == -1) {
            mEventData.field_0xf0.Step();
            my_main_f32 = mEventData.field_0xf0.Calc(trans->mBSpPoints);
        } else if (trans->mBSpCurve != 0) {
            mEventData.field_0xf0.Step();
            my_main_f32 = mEventData.field_0xf0.Calc(lit_5987[trans->mBSpCurve]);
        } else {
            my_main_f32 = (mCurCamStyleTimer + 1) / f32(trans->mTimer);
        }

        if (trans->mRelActor) {
            if (trans->mRelUseMask == 116) {
                pos.mXyz_1 = attentionPos(trans->mRelActor) + trans->mStartCenter;
            } else if (trans->mRelUseMask == 99) {
                cSGlobe cStack_7f8(trans->mStartCenter);
                cStack_7f8.U(trans->field_0x60.U() + cStack_7f8.U());
                pos.mXyz_1 = attentionPos(trans->mRelActor) + cStack_7f8.Xyz();
            } else if (trans->mRelUseMask == 119 || trans->mRelUseMask == 87) {
                pos.mXyz_1 = relationalPos(trans->mRelActor, &trans->mStartCenter);
            } else {
                if (isRelChar(trans->mRelUseMask)) {
                    pos.mXyz_1 = relationalPos(trans->mRelActor, &trans->mStartCenter);
                } else {
                    pos.mXyz_1 = trans->mStartCenter;
                }
            }

            if (trans->field_0x49 == 116) {
                pos.mXyz_0 = attentionPos(trans->mRelActor) + trans->mStartEye;
            } else if (trans->field_0x49 == 99) {
                cSGlobe cStack_800(trans->mStartEye);
                cStack_800.U(trans->field_0x60.U() + cStack_800.U());
                pos.mXyz_0 = attentionPos(trans->mRelActor) + cStack_800.Xyz();
            } else if (trans->field_0x49 == 119 || trans->field_0x49 == 87) {
                pos.mXyz_0 = relationalPos(trans->mRelActor, &trans->mStartEye);
            } else {
                if (isRelChar(trans->field_0x49)) {
                    pos.mXyz_0 = relationalPos(trans->mRelActor, &trans->mStartEye);
                } else {
                    pos.mXyz_0 = trans->mStartEye;
                }
            }

            if (trans->field_0x4a == 116) {
                pos2.mXyz_1 = attentionPos(trans->mRelActor) + trans->mCenter;
            } else if (trans->field_0x4a == 99) {
                cSGlobe cStack_808(trans->mCenter);
                cStack_808.U(trans->field_0x60.U() + cStack_808.U());
                pos2.mXyz_1 = attentionPos(trans->mRelActor) + cStack_808.Xyz();
            } else if (trans->field_0x4a == 119 || trans->field_0x4a == 87) {
                pos2.mXyz_1 = relationalPos(trans->mRelActor, &trans->mCenter);
            } else if (isRelChar(trans->field_0x4a)) {
                pos2.mXyz_1 = relationalPos(trans->mRelActor, &trans->mCenter);
            } else if (trans->mTransType == 2) {
                pos2.mXyz_1 = dCamMath::xyzRotateY(trans->mCenter, directionOf(trans->mRelActor));
            } else {
                pos2.mXyz_1 = trans->mCenter;
            }

            if (trans->field_0x4b == 116) {
                pos2.mXyz_0 = attentionPos(trans->mRelActor) + trans->mEye;
            } else if (trans->field_0x4b == 99) {
                cSGlobe cStack_810(trans->mEye);
                cStack_810.U(trans->field_0x60.U() + cStack_810.U());
                pos2.mXyz_0 = attentionPos(trans->mRelActor) + cStack_810.Xyz();
            } else {
                if (trans->field_0x4b == 119 || trans->field_0x4b == 87) {
                    pos2.mXyz_0 = relationalPos(trans->mRelActor, &trans->mEye);
                } else if (isRelChar(trans->field_0x4b)) {
                    pos2.mXyz_0 = relationalPos(trans->mRelActor, &trans->mEye);
                } else if (trans->mTransType == 2) {
                    pos2.mXyz_0 = dCamMath::xyzRotateY(trans->mEye, directionOf(trans->mRelActor));
                } else {
                    pos2.mXyz_0 = trans->mEye;
                }
            }
        } else {
            pos.mXyz_1 = trans->mStartCenter;
            pos.mXyz_0 = trans->mStartEye;
            pos2.mXyz_1 = trans->mCenter;
            pos2.mXyz_0 = trans->mEye;
        }

        cXyz sp15c;
        cXyz sp168;
        if (trans->mTransType == 1) {
            sp15c = pos.mXyz_1 + ((pos2.mXyz_1 - pos.mXyz_1) * my_main_f32);
            mViewCache.mCenter += (sp15c - mViewCache.mCenter) * trans->mCushion;

            cSGlobe cStack_818(pos.mXyz_0 - pos.mXyz_1);
            cSGlobe cStack_820(pos2.mXyz_0 - pos2.mXyz_1);

            cSGlobe cStack_828(cStack_818.R() + (my_main_f32 * (cStack_820.R() - cStack_818.R())), 
                               cStack_818.V() + ((cStack_820.V() - cStack_818.V()) * my_main_f32), 
                               cStack_818.U() + ((cStack_820.U() - cStack_818.U()) * my_main_f32));

            sp168 = mViewCache.mCenter + cStack_828.Xyz();

            mViewCache.mEye += (sp168 - mViewCache.mEye) * trans->mCushion;

        } else if (trans->mTransType == 3) {
            sp15c = pos.mXyz_1 + ((pos2.mXyz_1 - pos.mXyz_1) * my_main_f32);
            mViewCache.mCenter += (sp15c - mViewCache.mCenter) * trans->mCushion;

            cSGlobe cStack_830(pos.mXyz_0 - pos.mXyz_1);
            cSGlobe cStack_838(pos2.mXyz_0 - pos2.mXyz_1);

            cSAngle acStack_8a8(cStack_838.U() - cStack_830.U());
            if (acStack_8a8 == cSAngle::_0) {
                acStack_8a8.Val(my_main_f32 * 360.0f);
            } else {
                if (acStack_8a8 > cSAngle::_0) {
                    acStack_8a8 *= my_main_f32;
                } else {
                    acStack_8a8.Val(my_main_f32 * (acStack_8a8.Degree() + 360.0f));
                }
            }

            cSGlobe cStack_840(cStack_830.R() + ((cStack_838.R() - cStack_830.R()) * my_main_f32),
                              (cStack_830.V() + (cStack_838.V() - cStack_830.V()) * my_main_f32), 
                              (cStack_830.U() + acStack_8a8));

            sp168 = mViewCache.mCenter + cStack_840.Xyz();

            mViewCache.mEye += (sp168 - mViewCache.mEye) * trans->mCushion;

        } else if (trans->mTransType == 4) {
            sp15c = pos.mXyz_1 + ((pos2.mXyz_1 - pos.mXyz_1) * my_main_f32);
            mViewCache.mCenter += (sp15c - mViewCache.mCenter) * trans->mCushion;

            cSGlobe cStack_848(pos.mXyz_0 - pos.mXyz_1);
            cSGlobe cStack_850(pos2.mXyz_0 - pos2.mXyz_1);

            cSAngle acStack_8ac(cStack_850.U() - cStack_848.U());
            if (acStack_8ac == cSAngle::_0) {
                acStack_8ac.Val(my_main_f32 * -360.0f);
            } else if (acStack_8ac < cSAngle::_0) {
                acStack_8ac *= my_main_f32;
            } else {
                acStack_8ac.Val(my_main_f32 * (acStack_8ac.Degree() + -360.0f));
            }

            cSGlobe cStack_858(cStack_848.R() + (my_main_f32 * (cStack_850.R() - cStack_848.R())), 
                               cStack_848.V() + (cStack_850.V() - cStack_848.V()) * my_main_f32, 
                               cStack_848.U() + acStack_8ac);

            sp168 = mViewCache.mCenter + cStack_858.Xyz();

            mViewCache.mEye += (sp168 - mViewCache.mEye) * trans->mCushion;

        } else if (trans->mTransType == 2) {
            sp15c = pos.mXyz_1 + (pos2.mXyz_1 * my_main_f32);
            mViewCache.mCenter += (sp15c - mViewCache.mCenter) * trans->mCushion;
            sp168 = pos.mXyz_0 + (pos2.mXyz_0 * my_main_f32);
            mViewCache.mEye += (sp168 - mViewCache.mEye) * trans->mCushion;
            
        } else {
            sp15c = pos.mXyz_1 + ((pos2.mXyz_1 - pos.mXyz_1) * my_main_f32);
            mViewCache.mCenter += (sp15c - mViewCache.mCenter) * trans->mCushion;
            sp168 = pos.mXyz_0 + ((pos2.mXyz_0 - pos.mXyz_0) * my_main_f32);
            mViewCache.mEye += (sp168 - mViewCache.mEye) * trans->mCushion;
        }

        f32 o_float = trans->mStartFovy + (my_main_f32 * (trans->mFovy - trans->mStartFovy));
        mViewCache.mFovy += trans->mCushion * (o_float - mViewCache.mFovy);

        if (trans->field_0x68) {
            mid_val = trans->field_0x1c + my_main_f32 * (trans->field_0x3c - trans->field_0x1c);
            mViewCache.mBank += (cSAngle(mid_val) - mViewCache.mBank) * trans->mCushion;
            setFlag(0x400);
        }

        mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
    }

    return rv;
}

bool dCamera_c::watchActorEvCamera() {
    static cXyz DefaultGap(cXyz::Zero);

    ActorData* actor = (ActorData*)mWork;
    if (mCurCamStyleTimer == 0) {
        getEvXyzData(&actor->mCtrGap, "CtrGap", DefaultGap);
        getEvFloatData(&actor->mCushion, "Cushion", 1.0f);
        getEvFloatData(&actor->mNearDist, "NearDist", 750.0f);
        getEvFloatData(&actor->mZoomDist, "ZoomDist", 400.0f);
        getEvFloatData(&actor->mZoomVAngle, "ZoomVAngle", 0.0f);
        getEvFloatData(&actor->mFarDist, "FarDist", 1500.0f);
        getEvFloatData(&actor->mFovy, "Fovy", 62.0f);
        getEvIntData(&actor->mNearTimer, "NearTimer", 20);
        getEvIntData(&actor->mFarTimer, "FarTimer", 30);
        getEvFloatData(&actor->mFrontAngle, "FrontAngle", 179.0f);
        getEvIntData(&actor->mBlure, "Blure", 0);

        if ((actor->mTargetActor = getEvActor("Target", "@STARTER")) == NULL) {
            OS_REPORT("camera: event: error: target actor missing\n");
            return 1;
        }

        actor->mTargetActorID = fopAcM_GetID(actor->mTargetActor);
        actor->field_0xc = relationalPos(actor->mTargetActor, &actor->mCtrGap);
        actor->field_0x3c.Val(mViewCache.mEye - actor->field_0xc);
        if (actor->field_0x3c.R() < actor->mNearDist) {
            if (pointInSight(&actor->field_0xc)) {
                actor->field_0x54 = 0;
            } else {
                actor->field_0x54 = 1;
            }
        } else if (actor->field_0x3c.R() < actor->mFarDist) {
            actor->field_0x54 = 2;
        } else {
            actor->field_0x54 = 3;
        }

        mStyleSettle.mFinished = true;
    }

    if (fopAcM_SearchByID(actor->mTargetActorID) == NULL) {
        OS_REPORT("camera: event: error: target actor dead\n");
        return 1;
    }

    bool bVar5 = false;
    bool bVar1 = true;
    if (fopAcM_GetProfName(actor->mTargetActor) == 232 || fopAcM_GetProfName(actor->mTargetActor) == 550) {
        bVar5 = true;
    }

    switch (actor->field_0x54) {
        case 0:
            mViewCache.mEye = mEye;
            mViewCache.mDirection = mDirection;
            break;

        case 1:
            if (mCurCamStyleTimer == 0) {
                cXyz cStack_dc = attentionPos(mpPlayerActor);
                cStack_dc.y += 10.0f;
                cSGlobe cStack_288(cStack_dc - actor->field_0xc);
                cSGlobe cStack_290(mViewCache.mEye - positionOf(actor->mTargetActor));

                cSAngle acStack_2e8 = cStack_290.U() - directionOf(actor->mTargetActor);

                if (acStack_2e8 < cSAngle::_0) {
                    cStack_288.U(cStack_288.U() + cSAngle(5.0f));
                } else {
                    cStack_288.U(cStack_288.U() + cSAngle(-5.0f));
                }

                cSAngle acStack_2ec = cStack_288.U() - directionOf(actor->mTargetActor);
                cSAngle unused_angle;
                if (acStack_2ec < cSAngle(-actor->mFrontAngle)) {
                    cStack_288.U(directionOf(actor->mTargetActor) + cSAngle(-actor->mFrontAngle));
                } else if (acStack_2ec > cSAngle(actor->mFrontAngle)) {
                    cStack_288.U(directionOf(actor->mTargetActor) + cSAngle(actor->mFrontAngle));
                }

                actor->field_0x4c.Val(cStack_288.R() + 120.0f, cStack_288.V(), cStack_288.U());

                cSAngle acStack_2f4;
                if (acStack_2ec >= cSAngle::_0) {
                    acStack_2f4.Val(-8.0f);
                } else {
                    acStack_2f4.Val(8.0f);
                }

                cXyz cStack_e8;
                int i = 0;
                cSGlobe cStack_298 = actor->field_0x4c;
                bool unused_bool = false;
                for (i = 0; i < 45; i++) {
                    cStack_e8 = actor->field_0xc + cStack_298.Xyz();

                    if (bVar1 && !lineBGCheck(&actor->field_0xc, &cStack_e8, 0x4007)
                            && !lineCollisionCheck(actor->field_0xc, cStack_e8, mpPlayerActor, actor->mTargetActor, NULL)) {
                        actor->field_0x4c = cStack_298;
                        unused_bool = true;
                        (void) unused_bool;
                        break;
                    }

                    cStack_298.U(cStack_298.U() + acStack_2f4);

                    if (bVar5) {
                        cSAngle acStack_2f8 = cStack_298.U() - directionOf(actor->mTargetActor);
                        f32 degree = acStack_2f8.Degree();
                        if (fabsf(degree) < 70.0f) {
                            bVar1 = true;
                        } else {
                            bVar1 = false;
                        }
                    }

                    f32 my_f32;
                    if ((i & 2) != 0) {
                        my_f32 = -5.0f;
                    } else {
                        my_f32 = 5.0f;
                    }

                    cStack_298.V((cStack_298.V() + cSAngle(my_f32)) - (cStack_298.V() * 0.1f));
                }

                OS_REPORT("camera: event: search %d time(s)\n", i);
            }

            if (mCurCamStyleTimer < actor->mNearTimer) {
                f32 fVar6 = mCurCamStyleTimer / f32(actor->mNearTimer);
                mViewCache.mCenter += (actor->field_0xc - mViewCache.mCenter) * fVar6;

                cSGlobe* dir_p = &mViewCache.mDirection;
                dir_p->R(dir_p->R() + (actor->field_0x4c.R() - dir_p->R()) * fVar6);
                dir_p->U(dir_p->U() + (actor->field_0x4c.U() - dir_p->U()) * fVar6);
                dir_p->V(dir_p->V() + (actor->field_0x4c.V() - dir_p->V()) * fVar6);
                mViewCache.mEye = mViewCache.mCenter + dir_p->Xyz();
                mViewCache.mFovy += fVar6 * (actor->mFovy - mFovy);

                return 0;
            }
            break;

        case 2:
            if (mCurCamStyleTimer == 0) {
                cSGlobe cStack_2a0(attentionPos(mpPlayerActor) - actor->field_0xc);
                if (actor->mZoomVAngle != 0.0f) {
                    cStack_2a0.V(actor->mZoomVAngle);
                }

                cSAngle acStack_2fc(cStack_2a0.U() - directionOf(actor->mTargetActor));
                cSAngle unused_angle;
                if (acStack_2fc < cSAngle(-actor->mFrontAngle)) {
                    cStack_2a0.U(directionOf(actor->mTargetActor) + cSAngle(-actor->mFrontAngle));
                } else if (acStack_2fc > (cSAngle)(actor->mFrontAngle)) {
                    cStack_2a0.U(directionOf(actor->mTargetActor) + cSAngle(actor->mFrontAngle));
                }

                f32 my_diff = cStack_2a0.R() - actor->mZoomDist;
                if (fabsf(my_diff) < 30.0f) {
                    cStack_2a0.U(cStack_2a0.U() + (s16)900);
                }

                actor->field_0x4c.Val(actor->mZoomDist, cStack_2a0.V(), cStack_2a0.U());

                cSAngle acStack_304;
                if (acStack_2fc >= cSAngle::_0) {
                    acStack_304.Val(-8.0f);
                } else {
                    acStack_304.Val(8.0f);
                }

                cXyz cStack_f4;
                int i = 0;
                cSGlobe cStack_2a8(actor->field_0x4c);
                bool unused_bool = false;
                for (i = 0; i < 45; i++) {
                    cStack_f4 = actor->field_0xc + cStack_2a8.Xyz();

                    if (bVar1 && !lineBGCheck(&actor->field_0xc, &cStack_f4, 0x4007)
                            && !lineCollisionCheck(actor->field_0xc, cStack_f4, mpPlayerActor, actor->mTargetActor, NULL)) {
                        actor->field_0x4c = cStack_2a8;
                        unused_bool = true;
                        (void) unused_bool;
                        break;
                    }

                    cStack_2a8.U(cStack_2a8.U() + acStack_304);
                    if (bVar5) {
                        cSAngle acStack_3b4 = cStack_2a8.U() - directionOf(actor->mTargetActor);
                        f32 degree = acStack_3b4.Degree();
                        if (fabsf(degree) < 70.0f) {
                            bVar1 = true;
                        } else {
                            bVar1 = false;
                        }
                    }

                    f32 my_f32;
                    if ((i & 2) != 0) {
                        my_f32 = -5.0f;
                    } else {
                        my_f32 = 5.0f;
                    }

                    cStack_2a8.V((cStack_2a8.V() + cSAngle(my_f32)) - (cStack_2a8.V() * 0.1f));
                }

                OS_REPORT("camera: event: search %d time(s)\n", i);
            }

            if (mCurCamStyleTimer < actor->mFarTimer) {
                f32 fVar6 = mCurCamStyleTimer / f32(actor->mFarTimer);
                mViewCache.mCenter += (actor->field_0xc - mViewCache.mCenter) * fVar6;

                cSGlobe* dir_p = &mViewCache.mDirection;
                dir_p->R(dir_p->R() + (actor->field_0x4c.R() - dir_p->R()) * fVar6);
                dir_p->U(dir_p->U() + (actor->field_0x4c.U() - dir_p->U()) * fVar6);
                dir_p->V(dir_p->V() + (actor->field_0x4c.V() - dir_p->V()) * fVar6);
                mViewCache.mEye = mViewCache.mCenter + dir_p->Xyz();
                mViewCache.mFovy += fVar6 * (actor->mFovy - mFovy);

                return 0;
            }
            break;

        case 3:
            if (mCurCamStyleTimer == 0) {
                mViewCache.mCenter = actor->field_0xc;

                cSGlobe cStack_2b0(attentionPos(mpPlayerActor) - actor->field_0xc);
                cStack_2b0.R(actor->mZoomDist);

                cSAngle acStack_30c = cStack_2b0.U() - directionOf(actor->mTargetActor);
                cSAngle unused_angle;
                if (acStack_30c < cSAngle(-actor->mFrontAngle)) {
                    cStack_2b0.U(directionOf(actor->mTargetActor) + cSAngle(-actor->mFrontAngle));
                } else if (acStack_30c > cSAngle(actor->mFrontAngle)) {
                    cStack_2b0.U(directionOf(actor->mTargetActor) + cSAngle(actor->mFrontAngle));
                }

                if (actor->mZoomVAngle != 0.0f) {
                    cStack_2b0.V(actor->mZoomVAngle);
                }

                actor->field_0x4c.Val(actor->mZoomDist, cStack_2b0.V(), cStack_2b0.U());

                cSAngle acStack_314;
                if (acStack_30c >= cSAngle::_0) {
                    acStack_314.Val(-8.0f);
                } else {
                    acStack_314.Val(8.0f);
                }

                cXyz cStack_100;
                int i = 0;
                cSGlobe cStack_2b8 = actor->field_0x4c;
                bool unused_bool = false;
                for (i = 0; i < 45; i++) {
                    cStack_100 = actor->field_0xc + cStack_2b8.Xyz();

                    if (bVar1 && !lineBGCheck(&actor->field_0xc, &cStack_100, 0x4007)
                            && !lineCollisionCheck(actor->field_0xc, cStack_100, mpPlayerActor, actor->mTargetActor, NULL)) {
                        actor->field_0x4c = cStack_2b8;
                        unused_bool = true;
                        (void) unused_bool;
                        break;
                    }

                    cStack_2b8.U(cStack_2b8.U() + acStack_314);

                    if (bVar5) {
                        cSAngle acStack_318 = cStack_2b8.U() - directionOf(actor->mTargetActor);
                        f32 degree = acStack_318.Degree();
                        if (fabsf(degree) < 70.0f) {
                            bVar1 = true;
                        } else {
                            bVar1 = false;
                        }
                    }

                    f32 my_f32_0;
                    if ((i & 2) != 0) {
                        my_f32_0 = -5.0f;
                    } else {
                        my_f32_0 = 5.0f;
                    }

                    cStack_2b8.V((cStack_2b8.V() + cSAngle(my_f32_0)) - (cStack_2b8.V() * 0.1f));
                }
            }

            mViewCache.mDirection = actor->field_0x4c;
            mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
            mViewCache.mFovy = actor->mFovy;
    }

    mStyleSettle.mFinished = true;

    return 1;
}

bool dCamera_c::restorePosEvCamera() {
    static cXyz DefaultGap(cXyz::Zero);
    RestorePosData* restorePos = (RestorePosData*)mWork;

    if (mCurCamStyleTimer == 0) {
        getEvXyzData(&restorePos->field_0x0, "CtrGap", DefaultGap);
        getEvFloatData(&restorePos->field_0x18, "Cushion", 1.0f);
        getEvFloatData(&restorePos->field_0x20, "NearDist", 750.0f);
        getEvFloatData(&restorePos->field_0x28, "FarDist", 1500.0f);
        getEvIntData(&restorePos->field_0x1c, "NearTimer", 20);
        getEvIntData(&restorePos->field_0x24, "FarTimer", 30);
        getEvIntData(&restorePos->field_0x44, "Dest", 2);
        getEvIntData(&restorePos->field_0x68, "TargetType", 0);

        switch (restorePos->field_0x44) {
            case 0:
                restorePos->field_0x48 = mSavedViewStack[0];
                break;

            case 1:
                restorePos->field_0x48 = mSavedViewStack[1];
                break;

            case 9: {
                s16 sVar1 = restorePos->field_0x48.mBank.Val();
                dComIfGp_loadCameraPosition(0, &restorePos->field_0x48.mCenter, &restorePos->field_0x48.mEye, 
                                            &restorePos->field_0x48.mFovy, &sVar1);
                break;
            }

            default: {
                restorePos->field_0x48 = mSavedView;
                break;
            }
        }

        if ((restorePos->field_0x34 = getEvActor("Target", "@PLAYER")) == NULL) {
            OS_REPORT("camera: event: error: target actor missing\n");
            return 1;
        }

        restorePos->field_0xc = relationalPos(restorePos->field_0x34, &restorePos->field_0x0);

        cSGlobe cStack_e8(restorePos->field_0x48.mEye - mViewCache.mCenter);
        if (cStack_e8.R() < restorePos->field_0x20) {
            if (pointInSight(&restorePos->field_0xc)) {
                restorePos->field_0x40 = 0;
            } else {
                restorePos->field_0x40 = 1;
            }
        } else if (cStack_e8.R() < restorePos->field_0x28) {
            if (lineBGCheck(&mEye, &restorePos->field_0xc, 0x4007)) {
                restorePos->field_0x40 = 3;
            } else {
                restorePos->field_0x40 = 2;
            }
        } else {
            restorePos->field_0x40 = 3;
        }

        mStyleSettle.mFinished = true;
    }

    switch (restorePos->field_0x40) {
        case 0: {
            break;
        }

        case 1:
        case 2:
            if (mCurCamStyleTimer == 0) {
                restorePos->field_0x38.Val(restorePos->field_0x48.mEye - restorePos->field_0x48.mCenter);
            }

            if (mCurCamStyleTimer < restorePos->field_0x24) {
                f32 fVar1 = mCurCamStyleTimer / f32(restorePos->field_0x24);
                if (restorePos->field_0x68 == 1) {
                    mViewCache.mCenter += (restorePos->field_0x48.mCenter - mViewCache.mCenter) * fVar1;
                } else {
                    mViewCache.mCenter += (restorePos->field_0xc - mViewCache.mCenter) * fVar1;
                }

                mViewCache.mDirection.R(mViewCache.mDirection.R() + (fVar1 * (restorePos->field_0x38.R() - mViewCache.mDirection.R())));
                mViewCache.mDirection.U(mViewCache.mDirection.U() + ((restorePos->field_0x38.U() - mViewCache.mDirection.U()) * fVar1));
                mViewCache.mDirection.V((mViewCache.mDirection.V() + ((restorePos->field_0x38.V() - mViewCache.mDirection.V()) * fVar1)));

                mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
                
                mViewCache.mFovy += fVar1 * (restorePos->field_0x48.mFovy - mViewCache.mFovy);
                return 0;
            }
            break;

        case 3:
            if (mCurCamStyleTimer == 0) {
                if (restorePos->field_0x68 == 1) {
                    mViewCache.mCenter = restorePos->field_0x48.mCenter;
                } else {
                    mViewCache.mCenter = restorePos->field_0xc;
                }

                mViewCache.mDirection.Val(restorePos->field_0x48.mEye - restorePos->field_0x48.mCenter);
                mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
                mViewCache.mFovy = restorePos->field_0x48.mFovy;
            }
    }

    mStyleSettle.mFinished = true;
    return 1;
}

bool dCamera_c::talktoEvCamera() {
    fopAc_ac_c* r27 = (fopAc_ac_c*)dComIfGp_event_getPt1();
    s32 style = mCamTypeData[mEventData.field_0xc].field_0x18[mIsWolf][3];

    if (mCurCamStyleTimer == 0) {
        clrFlag(0x200000);
    }

    if (style < 0) {
        style = mCamParam.SearchStyle('TT01');
    }

#if DEBUG
    if (mCurCamStyleTimer == 0 && mCamSetup.CheckFlag(0x40)) {
        u32 id = mCamParam.Id(style);
        OS_REPORT("camera: event: %16s  = %d (%c%c%c%c)\n", "style", style, (id >> 0x18) & 0xFF, (id >> 0x10) & 0xFF, (id >> 0x8) & 0xFF, (id) & 0xFF);
    }
#endif

    return (this->*engine_tbl[mCamParam.Algorythmn(style)])(style);
}

bool dCamera_c::maptoolIdEvCamera() {
    if (mCurCamStyleTimer == 0) {
        int id;
        getEvIntData(&id, "ID", g_dComIfG_gameInfo.play.getEvent().getMapToolId());

        mEventData.field_0x8 = 0;
        field_0x160 = 0;
        mCurCamStyleTimer = 0;
        mEventData.field_0xec = dEvt_control_c::searchMapEventData(id);
        OS_REPORT("%06d: %s: %d: map data ID %d  %x\n", mFrameCounter, __FILE__, 2792, id,
                 mEventData.field_0xec);

        int var_r6 = 0xFF;
        if (mEventData.field_0xec != NULL && mEventData.field_0xec->type == dStage_MapEvent_dt_TYPE_MAPTOOLCAMERA) {
            var_r6 = mEventData.field_0xec->data.maptool.field_0x16;
        }

        getEvIntData(&mEventData.field_0x24, "CameraID", var_r6);
    }

    int temp_r31 = mEventData.field_0x24;
    if (mEventData.field_0xec == NULL || temp_r31 == 0xFF) {
        OS_REPORT("%d: %s: %d: ERROR!! no map data!!!\n", mFrameCounter, __FILE__, 2805);
        return true;
    }

    int room_no = dComIfGp_roomControl_getStayNo();
    if (isStageEvent(mEventData.field_0xec->field_0x4)) {
        room_no = -1;
    }

    if (mEventData.field_0xec->type == dStage_MapEvent_dt_TYPE_MAPTOOLCAMERA && mEventData.field_0xec->field_0xC != 0xFF) {
        if (mEventData.field_0xec->field_0xC & 1) {
            clrFlag(0x200000);
        }

        if (mEventData.field_0xec->field_0xC & 2) {
            mBumpCheckFlags = 0;
        }
    }

    mEventData.field_0xc = GetCameraTypeFromMapToolID(temp_r31, room_no);
    bool var_r31 = false;

    if (mEventData.field_0xc != 0xFF) {
        s32 style = mCamTypeData[mEventData.field_0xc].field_0x18[mIsWolf][0];
#if DEBUG
        if (mCurCamStyleTimer == 0) {
            OSReport("type %d mode %d style %d\n", mEventData.field_0xc, 0, style);
        }
#endif
        bool var_r3 = (this->*engine_tbl[mCamParam.Algorythmn(style)])(style);

        if (mEventData.field_0xec->type == dStage_MapEvent_dt_TYPE_MAPTOOLCAMERA) {
            if (mEventData.field_0xec->data.maptool.field_0x14 == 0xFF ||
                mCurCamStyleTimer > mEventData.field_0xec->data.maptool.field_0x14)
            {
                var_r31 = true;
            }
        } else {
            return var_r3;
        }
    } else {
        mEventData.field_0xec = NULL;
        var_r31 = true;
    }

    if (var_r31) {
        if (mEventData.field_0xec->type == dStage_MapEvent_dt_TYPE_MAPTOOLCAMERA && (mEventData.field_0xec->field_0xC & 0x20)) {
            return mDoCPd_c::getHoldA(mPadID) || mDoCPd_c::getHoldB(mPadID);
        }
        return true;
    }

    return false;
}

bool dCamera_c::styleEvCamera() {
    if (mCurCamStyleTimer == 0) {
        mEventData.field_0x8 = 0;
        field_0x160 = 0;
    }

    s32 style = mCamParam.SearchStyle(*(u32*)getEvStringPntData("Name", "FN01"));
    (this->*engine_tbl[mCamParam.Algorythmn(style)])(style);
    return isModeOK();
}

bool dCamera_c::gameOverEvCamera() {
    int i;

    GameOverData* gover_p = (GameOverData*)mWork;
    daPy_py_c* mPlayer = (daPy_py_c*)mpPlayerActor;
    daMidna_c* mMidna = daPy_py_c::getMidnaActor();

    if (mCurCamStyleTimer == 0) {
        gover_p->field_0x3c = (mPlayer->checkMidnaRide() && !mMidna->checkNoDraw());
    }

    cXyz sp1e8(0.0f, -45.0f, 0.0f);
    cXyz spa8[6] = {
        cXyz(85.0f, -50.0f, 165.0f),
        cXyz(72.0f, -64.0f, 60.0f),
        cXyz(165.0f, -20.0f, 45.0f),
        cXyz(85.0f, 165.0f, 40.0f),
        cXyz(10.0f, -70.0f, 110.0f),
        cXyz(0.0f, 0.0f, 0.0f)
    };

    int w_5c = 5;
    if (gover_p->field_0x3c != 0) {
        sp1e8.y = -10.0f;
    }

    cXyz sp1f4(0.0f, -120.0f, 130.0f);
    cXyz spd8[4] = {
        cXyz(0.0f, 35.0f, 160.0f),
        cXyz(105.0f, -65.0f, 200.0f),
        cXyz(60.0f, 10.0f, 100.0f),
        cXyz(0.0f, 0.0f, 0.0f)
    };

    cXyz sp200(0.0f, -60.0f, -45.0f);
    cXyz sp108[4] = {
        cXyz(0.0f, 85.0f, 5.0f),
        cXyz(-80.0f, 25.0f, 12.0f),
        cXyz(120.0f, 10.0f, -50.0f),
        cXyz(0.0f, 0.0f, 0.0f)
    };

    cXyz sp20c(0.0f, -5.0f, -60.0f);
    cXyz sp138[4] = {
        cXyz(-140.0f, -10.0f, 100.0f),
        cXyz(-20.0f, 190.0f, -150.0f),
        cXyz(20.0f, -75.0f, 115.0f),
        cXyz(0.0f, 0.0f, 0.0f)
    };

    // -----------
    cXyz sp218((mIsWolf != 1) ? sp1f4 : (gover_p->field_0x3c != 0) ? sp20c : sp200);
    cXyz* pos = (mIsWolf != 1) ? spd8 : (gover_p->field_0x3c != 0) ? sp138 : sp108;
    // -----------
 
    int w_54 = 3;
    f32 unused_f32 = 75.0f;
    (void) unused_f32;
    cXyz sp224;
    cXyz sp230;
    cXyz sp23c;
    cXyz sp248;

    cXyz sp254(0.0f, -40.0f, 0.0f);
    cXyz sp168[4] = {
        cXyz(0.0f, 135.0f, 140.0f),
        cXyz(-100.0f, 112.0f, 150.0f),
        cXyz(65.0f, 100.0f, 70.0f),
        cXyz(0.0f, 0.0f, 0.0f)
    };

    cXyz sp260(0.0f, -45.0f, -50.0f);
    cXyz sp198[4] = {
        cXyz(0.0f, 150.0f, 115.0f),
        cXyz(-120.0f, 180.0f, -40.0f),
        cXyz(40.0f, 100.0f, 5.0f),
        cXyz(0.0f, 0.0f, 0.0f)
    };

    cXyz sp26c(0.0f, 15.0f, -100.0f);
    cXyz sp1c8[4] = {
        cXyz(40.0f, 65.0f, 165.0f),
        cXyz(-200.0f, 140.0f, 50.0f),
        cXyz(50.0f, 230.0f, 0.0f),
        cXyz(0.0f, 0.0f, 0.0f)
    };

    // -----------
    int w_50 = 3;
    cXyz sp278((mIsWolf != 1) ? sp254 : (gover_p->field_0x3c != 0) ? sp26c : sp260);
    cXyz* pos2 = (mIsWolf != 1) ? sp168 : (gover_p->field_0x3c != 0) ? sp1c8 : sp198;
    // -----------

    bool bVar3 = false;
    if (dComIfGp_checkPlayerStatus0(mPadID, 0x100000) || mPlayer->checkMagneBootsOn()) {
        if (mBG.field_0x0.field_0x58 >= attentionPos(mpPlayerActor).y + 40.0f) {
            bVar3 = true;
        }
    }

    if (mCurCamStyleTimer == 0) {
        gover_p->field_0x0 = 0;
        gover_p->field_0x4 = 0;
        if ((mFrameCounter & 2) != 0) {
            gover_p->field_0x8 = 0;
        } else {
            gover_p->field_0x8 = 1;
        }

        if (dComIfGp_checkPlayerStatus0(mPadID, 0x100000)) {
            gover_p->field_0x0 = 50;
        }

        mStyleSettle.mFinished = true;
        cXyz sp284 = mViewCache.mEye - attentionPos(mpPlayerActor);
        sp284 = dCamMath::xyzRotateY(sp284, -cSAngle(mpPlayerActor->shape_angle.y));
        spa8[w_5c] = pos[w_54] = pos2[w_50] = sp284;
    }

    cXyz sp291(attentionPos(mpPlayerActor));
    cXyz my_cross;
    bool bVar9 = false;

    switch (gover_p->field_0x0) {
        case 0:
        default: {
            getEvIntData(&gover_p->mType, "Type", 0);
            if (gover_p->mType == 1) {
                cM3dGPla cStack_1dc;
                dComIfG_Bgsp().GetTriPla(mBG.field_0x5c.field_0x4, &cStack_1dc);

                cXyz* vec_p = cStack_1dc.GetNP();
                if (vec_p->y >= 0.9f) {
                    gover_p->field_0x0 = 80;
                } else {
                    gover_p->field_0x0 = 81;
                }

                gover_p->field_0x10 = mCenter;
                gover_p->field_0x1c = mDirection;
                break;
            }
            gover_p->field_0x0 = 1;
        }

        case 1: {
            sp224 = relationalPos(mpPlayerActor, &sp1e8);
            for (i = 0; i < w_5c; i++) {
                if (gover_p->field_0x8 != 0) {
                    spa8[i].x = -spa8[i].x;
                }

                sp230 = relationalPos(mpPlayerActor, &spa8[i]);
                if (sp230.y < mBG.field_0x108.field_0x4 + positionOf(mpPlayerActor).y) {
                    sp230.y = mBG.field_0x108.field_0x4 + positionOf(mpPlayerActor).y;
                }

                if (!lineBGCheck(&sp291, &sp230, 0x40b7)) break;

                spa8[i].x = -spa8[i].x;
                sp230 = relationalPos(mpPlayerActor, &spa8[i]);

                if (sp230.y < mBG.field_0x108.field_0x4 + positionOf(mpPlayerActor).y) {
                    sp230.y = mBG.field_0x108.field_0x4 + positionOf(mpPlayerActor).y;
                }

                if (!lineBGCheck(&sp291, &sp230, 0x40b7)) {
                    break;
                }

                gover_p->field_0x8 ^= 1;
            }

            mViewCache.mCenter = sp224;
            mViewCache.mEye = sp230;
            gover_p->field_0x0++;
            bVar9 = true;
        }
        // fallthrough
        case 2: {
            if (gover_p->field_0x4 != 90) {
                break;
            }

            if (daPy_getPlayerActorClass()->checkHorseRide()) {
                break;
            }

            gover_p->field_0x0++;
            gover_p->field_0x4 = 0.0f;
            mViewCache.mFovy = 60.0f;
        }
        // fallthrough
        case 3: {
            sp224 = relationalPos(mpPlayerActor, &sp218);
            if (lineBGCheck(&sp291, &sp224, &my_cross, 0x40b7)) {
                sp224 = my_cross + mViewCache.mDirection.Norm();
            }

            for (i = 0; i < w_54; i++) {
                if (gover_p->field_0x8 != 0) {
                    pos[i].x = -pos[i].x;
                }

                sp230 = relationalPos(mpPlayerActor, &pos[i]);
                if (sp230.y < (mBG.field_0x108.field_0x4 + positionOf(mpPlayerActor).y)) {
                    sp230.y = mBG.field_0x108.field_0x4 + positionOf(mpPlayerActor).y;
                }

                if (!lineBGCheck(&sp291, &sp230, 0x40b7)) break;

                pos[i].x = -pos[i].x;
                sp230 = relationalPos(mpPlayerActor, &pos[i]);

                if (sp230.y < (mBG.field_0x108.field_0x4 + positionOf(mpPlayerActor).y)) {
                    sp230.y = mBG.field_0x108.field_0x4 + positionOf(mpPlayerActor).y;
                }

                if (!lineBGCheck(&sp291, &sp230, 0x40b7)) {
                    break;
                }

                gover_p->field_0x8 ^= 1;
            }

            mViewCache.mCenter = sp224;
            mViewCache.mEye = sp230;
            gover_p->field_0x0++;
            mViewCache.mFovy = 75.0f;
            bVar9 = 75.0f;
        }
        // fallthrough intentional.
        case 4: {
            if (gover_p->field_0x4 == 40) {
                ++gover_p->field_0x0;
                gover_p->field_0x4 = 0;
            }

            break;
        }

        case 5: {
            break;
        }

        case 50: {
            gover_p->field_0x0 = 51;
        }
        // fallthrough intentional.
        case 51: {
            sp224 = relationalPos(mpPlayerActor, &sp278);
            if (lineBGCheck(&sp291, &sp224, &my_cross, 0x40b7)) {
                sp224 = my_cross + mViewCache.mDirection.Norm();
            }

            for (i = 0; i < w_50; i++) {
                if (gover_p->field_0x8 != 0) {
                    pos2[i].x = -pos2[i].x;
                }

                sp230 = relationalPos(mpPlayerActor, &pos2[i]);

                if (!lineBGCheck(&sp291, &sp230, 0x40b7)) {
                    break;
                }

                pos2[i].x = -pos2[i].x;
                sp230 = relationalPos(mpPlayerActor, &pos2[i]);

                if (!lineBGCheck(&sp291, &sp230, 0x40b7)) {
                    break;
                }

                gover_p->field_0x8 ^= 1;
            }

            mViewCache.mCenter = sp224;
            mViewCache.mEye = sp230;
            bVar9 = true;
            mViewCache.mFovy = 60.0f;
            ++gover_p->field_0x0;
        }
        // fallthrough
        case 52: {
            if (gover_p->field_0x4 == 160) {
                gover_p->field_0x0 = 5;
                gover_p->field_0x4 = 0.0f;
            }
            break;
        }

        case 80:
        case 81: {
            f32 fVar1, mid_var;
            if (mIsWolf == 1) {
                mid_var = -10.0f;
            } else {
                mid_var = -40.0f;
            }

            fVar1 = mid_var;
            if (gover_p->field_0x3c != 0) {
                fVar1 = 40.0f;
            }

            gover_p->field_0x10.y += (fVar1 + attentionPos(mpPlayerActor).y - gover_p->field_0x10.y) * 0.005f;
            mViewCache.mCenter = gover_p->field_0x10;

            cSAngle acStack_43c(gover_p->field_0x1c.V());
            f32 next_f_var;
            if (gover_p->field_0x0 == 80) {
                next_f_var = 30.0f;
            } else {
                next_f_var = 55.0f;
            }

            acStack_43c += (cSAngle(next_f_var) - acStack_43c) * 0.02f;
            gover_p->field_0x1c.V(acStack_43c);

            f32 mRad = gover_p->field_0x1c.R();
            mRad += (350.0f - mRad) * 0.02f;
            gover_p->field_0x1c.R(mRad);
            mViewCache.mFovy += (40.0f - mViewCache.mFovy) * 0.005f;
            mViewCache.mEye = mViewCache.mCenter + gover_p->field_0x1c.Xyz();

            break;
        }
    }

    if (!bVar9 && bVar3) {
        mViewCache.mCenter += mMonitor.field_0x14;
        mViewCache.mEye += (mMonitor.field_0x14 * 0.98f);
    }

    mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
    ++gover_p->field_0x4;
    mStyleSettle.mFinished = true;
    return 1;
}

bool dCamera_c::tactEvCamera() {
    TactData* tact_p = (TactData*)mWork;
    cXyz sp58[6] = {
        cXyz(0.0f, 5.0f, 190.0f),
        cXyz(0.0f, 0.0f, 215.0f),
        cXyz(0.0f, -26.0f, 80.0f),
        cXyz(0.0f, -26.0f, 45.0f),
        cXyz(0.0f, -40.0f, -20.0f),
        cXyz(0.0f, -4.0f, 28.0f)
    };
    cXyz spa0[6] = {
        cXyz(140.0f, 35.0f, 220.0f),
        cXyz(-200.0f, -15.0f, 240.0f),
        cXyz(380.0f, -120.0f, 450.0f),
        cXyz(-235.0f, -142.0f, 470.0f),
        cXyz(30.0f, 105.0f, -260.0f),
        cXyz(-66.0f, 390.0f, 56.0f)
    };
    f32 local_1d0[6] = { 0.75f, 0.35f, 0.8f, 0.8f, 0.8f, 0.8f };
    int const_6_val = 6;
    cXyz spf4[7] = {
        cXyz(0.0f, -16.0f, 14.0f),
        cXyz(0.0f, -26.0f, 14.0f),
        cXyz(0.0f, -26.0f, 14.0f),
        cXyz(0.0f, -10.0f, 15.0f),
        cXyz(-5.0f, -16.0f, -10.0f),
        cXyz(0.0f, -18.0f, 35.0f),
        cXyz(0.0f, -12.0f, 8.0f)
    };
    cXyz sp148[7] = {
        cXyz(-128.0f, -5.0f, 120.0f),
        cXyz(-80.0f, -28.0f, 160.0f),
        cXyz(50.0f, -18.0f, 172.0f),
        cXyz(110.0f, 5.0f, 58.0f),
        cXyz(-50.0f, 40.0f, 85.0f),
        cXyz(-70.0f, 60.0f, -45.0f),
        cXyz(5.0f, 70.0f, -25.0f)
    };
    int const_7_val = 7;
    cXyz sp178[4] = {
        cXyz(0.0f, -10.0f, -56.0f),
        cXyz(5.0f, -10.0f, -54.0f),
        cXyz(0.0f, 0.0f, -15.0f),
        cXyz(0.0f, 0.0f, -20.0f)
    };
    cXyz sp1a8[4] = {
        cXyz(115.0f, -25.0f, 108.0f),
        cXyz(-78.0f, -30.0f, 130.0f),
        cXyz(85.0f, 0.0f, -40.0f),
        cXyz(-25.0f, 70.0f, -75.0f)
    };
    int const_4_val = 4;
    int i;

    if (mCurCamStyleTimer == 0) {
        tact_p->field_0x4 = 0;
        tact_p->field_0x14 = 0;
        tact_p->field_0x0 = 0;
        tact_p->field_0xc = this->mFrameCounter;
        if ((this->mFrameCounter & 2) != 0) {
            tact_p->field_0x8 = 0;
        } else {
            tact_p->field_0x8 = 1;
        }

        mStyleSettle.mFinished = true;
        mEventData.field_0x20 = 0;
        mEventData.field_0x24 = 0;
        tact_p->field_0x1c = sp178;
        tact_p->field_0x20 = sp1a8;
        tact_p->field_0x24 = const_4_val;
        tact_p->field_0x2c = 55.0f;

        dComIfGp_saveCameraPosition(0, &mViewCache.mCenter, &mViewCache.mEye, mViewCache.mFovy, mViewCache.mBank);
    }

    cXyz sp1e8;
    switch (mEventData.field_0x24) {
        case 2: {
            f32 fVar5;
            if ((tact_p->field_0xc & 4) != 0 && (tact_p->field_0xc & 16) != 0 && (tact_p->field_0xc & 128) != 0) {
                sp1e8 = sp58[2];
                sp58[2] = sp58[3];
                sp58[3] = sp1e8;
                sp1e8 = spa0[2];
                spa0[2] = spa0[3];
                spa0[3] = sp1e8;
                fVar5 = local_1d0[2];
                local_1d0[2] = local_1d0[3];
                local_1d0[3] = fVar5;
            }

            if ((tact_p->field_0xc & 2) != 0 && (tact_p->field_0xc & 32) != 0 && (tact_p->field_0xc & 64) != 0) {
                sp1e8 = sp58[2];
                sp58[2] = sp58[0];
                sp58[0] = sp1e8;
                sp1e8 = spa0[2];
                spa0[2] = spa0[0];
                spa0[0] = sp1e8;
                fVar5 = local_1d0[2];
                local_1d0[2] = local_1d0[0];
                local_1d0[0] = fVar5;
            }

            if ((tact_p->field_0xc & 1) != 0 && (tact_p->field_0xc & 8) != 0) {
                sp1e8 = sp58[1];
                sp58[1] = sp58[0];
                sp58[0] = sp1e8;
                sp1e8 = spa0[1];
                spa0[1] = spa0[0];
                spa0[0] = sp1e8;
                fVar5 = local_1d0[1];
                local_1d0[1] = local_1d0[0];
                local_1d0[0] = fVar5;
            }

            if (tact_p->field_0x0 < 20) {
                tact_p->field_0x0 = 20;
            }

            tact_p->field_0x1c = sp58;
            tact_p->field_0x20 = spa0;
            tact_p->field_0x24 = const_6_val;
            tact_p->field_0x2c = 65.0f;
            break;
        }

        case 3:
            if (tact_p->field_0x0 < 22) {
                tact_p->field_0x0 = 22;
            }
            break;

        case 1:
            tact_p->field_0x0 = 0;
            break;

        case 99:
            break;

        default:
            if (mIsWolf == 1) {
                tact_p->field_0x1c = sp178;
                tact_p->field_0x20 = sp1a8;
                tact_p->field_0x24 = const_4_val;
                tact_p->field_0x2c = 65.0f;
            } else {
                tact_p->field_0x1c = spf4;
                tact_p->field_0x20 = sp148;
                tact_p->field_0x24 = const_7_val;
                tact_p->field_0x2c = 55.0f;
            }
    }

    fopAc_ac_c* mHorseActor = (fopAc_ac_c*)dComIfGp_getHorseActor();
    switch (tact_p->field_0x0) {
        case 20: {
            if (mHorseActor) {
                for (i = 0; i < tact_p->field_0x24; i++) {
                    mViewCache.mCenter = relationalPos(mHorseActor, &tact_p->field_0x1c[i]);
                    mViewCache.mEye = relationalPos(mHorseActor, &tact_p->field_0x20[i]);
                    if (!lineBGCheck(&mViewCache.mCenter, &mViewCache.mEye, 0x4007)
                        && !lineCollisionCheck(mViewCache.mCenter, mViewCache.mEye, mHorseActor, NULL, NULL)) {
                        break;
                    }
                }

                mViewCache.mFovy = tact_p->field_0x2c;
                tact_p->field_0x0 = 21;
                tact_p->field_0x14 = i;
            }
            break;
        }

        case 21: {
            if (mHorseActor) {
                cXyz sp208 = relationalPos(mHorseActor, &tact_p->field_0x1c[tact_p->field_0x14]);
                cXyz sp214 = relationalPos(mHorseActor, &tact_p->field_0x20[tact_p->field_0x14]);
                if (!lineBGCheck(&sp208, &sp214, 0x4007) &&!lineCollisionCheck(sp208, sp214, mHorseActor, NULL, NULL)) {
                    mViewCache.mCenter += (sp208 - mViewCache.mCenter) * local_1d0[tact_p->field_0x14];
                    mViewCache.mEye += (sp214 - mViewCache.mEye) * local_1d0[tact_p->field_0x14];
                } else {
                    tact_p->field_0x0 = 22;
                }
            }
            break;
        }

        case 22: {
            if (mHorseActor) {
                cXyz cStack_220 = relationalPos(mHorseActor, &tact_p->field_0x1c[tact_p->field_0x14]);
                cXyz cStack_22c;
                cXyz cStack_238 = attentionPos(mpPlayerActor);
                cXyz cStack_244;
                cXyz local_250(50.0f, -10.0f, -160.0f);
                cXyz local_25c = attentionPos(mHorseActor) - cStack_238;
                cSAngle acStack_47c(cM_atan2s(local_25c.x, local_25c.z));
                if (acStack_47c - directionOf(mpPlayerActor) > cSAngle::_0) {
                    local_250.x = -local_250.x;
                }

                //cSAngle acStack_488 = acStack_47c;
                local_250 = dCamMath::xyzRotateY(local_250, acStack_47c);

                cStack_22c = attentionPos(mpPlayerActor) + local_250;

                cM3dGLin cStack_1f0(cStack_220, cStack_22c);
                f32 fVar1;
                if (cM3d_Len3dSqPntAndSegLine(&cStack_1f0, &cStack_238, &cStack_244, &fVar1)) {
                    cStack_220 = cStack_244;
                }

                if (!lineBGCheck(&cStack_220, &cStack_22c, 0x4007) && !lineBGCheck(&cStack_238, &cStack_22c, 0x4007)) {
                    mViewCache.mCenter = cStack_220;
                    mViewCache.mEye = cStack_22c;
                } else {
                    mViewCache.mCenter = attentionPos(mpPlayerActor) + tact_p->field_0x30;
                    mViewCache.mEye = attentionPos(mpPlayerActor) + tact_p->field_0x3c;
                }

                tact_p->field_0x0 = 23;
            }
            break;
        }

        case 0:
        default: {
            cXyz sp254;
            cXyz sp260 = attentionPos(mpPlayerActor);
            for (i = 0; i < tact_p->field_0x24; i++) {
                mViewCache.mCenter = relationalPos(mpPlayerActor, &tact_p->field_0x1c[i]);
                sp260.y = mViewCache.mCenter.y;
                if (lineBGCheck(&sp260, &mViewCache.mCenter, &sp254, 0x4007)) {
                    mViewCache.mCenter = sp254 - (cXyz(sp254 - sp260).norm() * 5.0f);
                }

                mViewCache.mEye = relationalPos(mpPlayerActor, &tact_p->field_0x20[i]);
                sp254 = mViewCache.mEye + (cXyz(mViewCache.mEye - sp260).norm() * 5.0f);

                if (!lineBGCheck(&sp260, &sp254, 0x4007)
                    && !lineCollisionCheck(mViewCache.mCenter, mViewCache.mEye, mpPlayerActor, NULL, NULL)) {
                    break;
                }
            }

            if (tact_p->field_0x0 == 0) {
                tact_p->field_0x30 = mViewCache.mCenter - sp260;
                tact_p->field_0x3c = mViewCache.mEye - sp260;
            }

            tact_p->field_0x0 = 9;
            mViewCache.mFovy = tact_p->field_0x2c;
            break;
        }

        case 9:
        case 29: {
            mViewCache.mCenter = attentionPos(mpPlayerActor) + tact_p->field_0x30;
            mViewCache.mEye = attentionPos(mpPlayerActor) + tact_p->field_0x3c;
            break;
        }

        case 23: {
            break;
        }
    }

#if DEBUG
    if (mCamSetup.CheckFlag(0x8000)) {
        dDbVw_Report(20, 250, "        %d", tact_p->field_0x0);
    }
#endif

    mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
    tact_p->field_0x4++;
    return 1;
}

bool dCamera_c::turnToActorEvCamera() {
    return true;
}

bool dCamera_c::stbWaitEvCamera() {
    dDemo_camera_c* demo_cam = dDemo_c::getCamera();
#if DEBUG
    char* enabled_report = "-----";
#endif

    if (demo_cam != NULL) {
        if (demo_cam->checkEnable(0x40)) {
            mViewCache.mCenter = demo_cam->getTarget();
#if DEBUG
            enabled_report[0] = 'C';
#endif
        }

        if (demo_cam->checkEnable(0x10)) {
            mViewCache.mEye = demo_cam->getTrans();
#if DEBUG
            enabled_report[1] = 'E';
#endif
        }

        if (demo_cam->checkEnable(0x4)) {
            mViewCache.mFovy = demo_cam->getFovy();
#if DEBUG
            enabled_report[2] = 'F';
#endif
        }

        if (demo_cam->checkEnable(0x20)) {
            mUp = demo_cam->getUp();
#if DEBUG
            enabled_report[3] = 'U';
#endif
        }

        if (demo_cam->checkEnable(0x80)) {
            mViewCache.mBank = cAngle::d2s(-demo_cam->getRoll());
#if DEBUG
            enabled_report[4] = 'B';
#endif
        }

#if DEBUG
        if (mCamSetup.CheckFlag(0x8000)) {
            dDbVw_Report(90, 190, "%s", enabled_report);
        }
#endif

        mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
    }

    return true;
}

bool dCamera_c::saveEvCamera() {
    int slot;
    getEvIntData(&slot, "Slot", 0);

    if (slot == 9) {
        dComIfGp_saveCameraPosition(0, &mViewCache.mCenter, &mViewCache.mEye, mViewCache.mFovy, mViewCache.mBank.Val());
    } else {
        pushInfo(&mSavedViewStack[slot], 1);
    }

    mStyleSettle.mFinished = 1;
    return true;
}

bool dCamera_c::loadEvCamera() {
    LoadData* load = (LoadData*)mWork;
    f32 local_58[6] = { 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f };
    if (mCurCamStyleTimer == 0) {
        getEvIntData(&load->mSlot, "Slot", 0);
        getEvIntData(&load->mTimer, "Timer", 1);
        getEvIntData(&load->mType, "Type", (load->mTimer < 10) ? 0 : 1);

        if (load->mType > 1) {
            OS_REPORT("camera: event: LOAD: warning!! type %d unknown\n", load->mType);
            load->mType = 1;
        }

        if (load->mType != 0) {
            mEventData.field_0xf0.Init(4, load->mTimer + 1);
            mEventData.field_0xf0.Step();
        }

        if (load->mSlot == 9) {
            s16 local_b0;
            dComIfGp_loadCameraPosition(0, &load->field_0x28, &load->field_0x34, &load->field_0x48, &local_b0);
            load->field_0x42 = cSAngle(local_b0);
        } else {
#if DEBUG
            if (mSavedViewStack[load->mSlot].field_0x1e == 0 || load->mSlot >= 2) {
                OS_REPORT("camera: event: LOAD: warning!! slot %d not saved\n", load->mSlot);
            }
#endif

            dCamInfo_c* pdVar3 = &mSavedViewStack[load->mSlot];
            load->field_0x28 = pdVar3->mCenter;
            load->field_0x34 = pdVar3->mEye;
            load->field_0x48 = pdVar3->mFovy;
            load->field_0x42 = pdVar3->mBank;
        }
        load->field_0x1c = mViewCache.mCenter;
        load->field_0xc = mViewCache.mDirection;
        load->field_0x14.Val(load->field_0x34 - load->field_0x28);
        load->field_0x44 = mViewCache.mFovy;
        load->field_0x40 = mViewCache.mBank;
    }

    if (mCurCamStyleTimer < load->mTimer) {
        f32 fVar5;
        if (load->mType != 0) {
            mEventData.field_0xf0.Step();
            fVar5 = mEventData.field_0xf0.Calc(local_58);
        } else {
            fVar5 = (mCurCamStyleTimer + 1) / f32(load->mTimer);
        }

        mViewCache.mCenter = load->field_0x1c + ((load->field_0x28 - load->field_0x1c) * fVar5);
        mViewCache.mDirection.R(load->field_0xc.R() + (fVar5 * (load->field_0x14.R() - load->field_0xc.R())));
        mViewCache.mDirection.V(load->field_0xc.V() + ((load->field_0x14.V() - load->field_0xc.V()) * fVar5));
        mViewCache.mDirection.U(load->field_0xc.U() + ((load->field_0x14.U() - load->field_0xc.U()) * fVar5));
        mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
        mViewCache.mFovy = load->field_0x44 + (fVar5 * (load->field_0x48 - load->field_0x44));
        mViewCache.mBank = load->field_0x40 + ((load->field_0x42 - load->field_0x40) * fVar5);
        return 0;
    } else {
        mStyleSettle.mFinished = true;
        return 1;
    }
}

bool dCamera_c::useItem0EvCamera() {
    ItemData* item_p = (ItemData*)mWork;
    cXyz vec_1dc(10.0f, -15.0f, 20.0f);
    cXyz vecs_338[4] = {
        cXyz(30.0f, 115.0f, 70.0f),
        cXyz(0.0f, 120.0f, 64.0f),
        cXyz(65.0f, 120.0f, -10.0f),
        cXyz(40.0f, 118.0f, 0.0f)
    };
    int const_1st_4_val = 4;

    cXyz vec_1d0(0.0f, -32.0f, 15.0f);
    cXyz vecs_368[6] = {
        cXyz(80.0f, -50.0f, 140.0f),
        cXyz(-52.0f, 34.0f, 80.0f),
        cXyz(80.0f, 116.0f, 88.0f),
        cXyz(55.0f, -33.0f, 100.0f),
        cXyz(105.0f, -55.0f, 70.0f),
        cXyz(16.0f, 52.0f, -8.0f),
    };
    int const_6_val = 6;

    cXyz vec_1c4(0.0f, -80.0f, 25.0f);
    cXyz vecs_308[4] = {
        cXyz(-85.0f, -15.0f, 240.0f),
        cXyz(-95.0f, 92.0f, 165.0f),
        cXyz(155.0f, 80.0f, 120.0f),
        cXyz(5.0f, 108.0f, 155.0f)
    };
    int const_2nd_4_val = 4;

    cXyz vec_1b8(0.0f, -27.0f, 25.0f);
    cXyz vecs_2d8[4] = {
        cXyz(-90.0f, -70.0f, 150.0f),
        cXyz(130.0f, -68.0f, 114.0f),
        cXyz(5.0f, 130.0f, 110.0f),
        cXyz(-45.0f, -50.0f, -110.0f)
    };
    int const_3rd_4_val = 4;

    cXyz vec_1ac(30.241f, 12.653f, 13.95f);
    cXyz vecs_2b4[3] = {
        cXyz(23.639f, 96.636f, 57.318f),
        cXyz(-2.849f, 96.639f, 42.753f),
        cXyz(23.639f, 96.636f, 57.318)
    };
    int const_1st_3_val = 3;

    cXyz vec_1a0(0.0f, -60.0f, -15.0f);
    cXyz vecs_290[3] = {
        cXyz(-90.0f, 115.0f, 120.0f),
        cXyz(75.0f, 105.0f, 65.0f),
        cXyz(80.0f, 110.0f, -130.0f)
    };
    int const_2nd_3_val = 3;

    cXyz vec_194(0.0f, -15.0f, -100.0f);
    cXyz vecs_200[2] = {
        cXyz(-100.0f, -60.0f, -420.0f),
        cXyz(110.0f, -55.0f, -400.0f)
    };
    int const_1st_2_val = 2;

    cXyz vec_188(0.0f, 10.0f, -30.0f);
    cXyz vecs_1e8[2] = {
        cXyz(95.0f, -55.0f, -245.0f),
        cXyz(-58.0f, -65.0f, -250.0f)
    };
    int const_2nd_2_val = 2;

    cXyz vec_17c(0.0f, -15.0f, 20.0f);
    cXyz vecs_260[4] = {
        cXyz(-105.0f, 1.5f, 47.0f),
        cXyz(113.0f, -28.0f, -12.0f),
        cXyz(96.0f, 50.0f, 38.0f),
        cXyz(-68.0f, 70.0f, 82.0f)
    };
    int const_4th_4_val = 4;

    cXyz vec_170(0.0f, 15.0f, -40.0f);
    cXyz vecs_23c[3] = {
        cXyz(65.0f, 350.0f, -85.0f),
        cXyz(-110.0f, 340.0f, -115.0f),
        cXyz(100.0f, 30.0f, 270.0f)
    };
    int const_3rd_3_val = 3;

    cXyz vec_164(0.0f, -20.0f, 30.0f);
    cXyz vecs_218[3] = {
        cXyz(-85.0f, -10.0f, 300.0f),
        cXyz(50.0f, 165.0f, 160.0f),
        cXyz(-260.0f, -50.0f, 30.0f)
    };
    int const_4th_3_val = 3;

    struct UseItem0Data {
        cXyz* field_0x00;
        cXyz* field_0x04;
        int field_0x08;
        f32 field_0x0c;
        int field_0x10;
        int field_0x14;
        int field_0x18;
    };
    UseItem0Data data_list[13] = {
        {&vec_1c4, vecs_308, const_2nd_4_val, 65.0f, 30,  0, -1},
        {&vec_1d0, vecs_368, const_6_val,     65.0f, 40,  0, -1}, 
        {&vec_1c4, vecs_308, const_2nd_4_val, 65.0f, 40,  0,  7},
        {&vec_1b8, vecs_2d8, const_3rd_4_val, 65.0f, 40,  0,  7},
        {&vec_1dc, vecs_338, const_1st_4_val, 65.0f, 10, 73, -1},
        {&vec_1ac, vecs_2b4, const_1st_3_val, 70.0f, 45,  0, -1},
        {&vec_1a0, vecs_290, const_2nd_3_val, 60.0f, 40,  0, -1},
        {&vec_1a0, vecs_290, const_2nd_3_val, 60.0f, 40,  0, -1},
        {&vec_194, vecs_200, const_1st_2_val, 45.0f, 80, 10, -1},
        {&vec_188, vecs_1e8, const_2nd_2_val, 65.0f, 40,  0, -1},
        {&vec_17c, vecs_260, const_4th_4_val, 60.0f, 40,  0, -1},
        {&vec_164, vecs_218, const_4th_3_val, 60.0f,  1, 60, -1},
        {&vec_1dc, vecs_338, const_1st_4_val, 65.0f, 20,  0, -1},
    };

    cSAngle acStack_53c, acStack_540;
    cXyz sp408;
    int idx;
    bool ret = false;

    if (mCurCamStyleTimer == 0) {
        item_p->field_0x0 = 0;
        item_p->field_0x44 = mCenter;
        item_p->field_0x50 = mEye;
        item_p->field_0x5c = mFovy;
    }

    cXyz att_pos = attentionPos(mpPlayerActor);
    cXyz sp424;
    switch (item_p->field_0x0) {
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 0:
        default: {
            getEvIntData(&item_p->mType, "Type", 0);
            item_p->field_0x1c = 0;
            item_p->field_0x20 = 0;
        }
        // fallthrough intentional.
        case 10:
        case 11: {
            item_p->field_0x0 = 10;
            if (++item_p->field_0x20 >= data_list[item_p->mType].field_0x14) {
                item_p->field_0x20 = 0;
                if ((mFrameCounter & data_list[item_p->mType].field_0x18) == 0) {
                    cXyz temp_vec(data_list[item_p->mType].field_0x04[0]);
                    data_list[item_p->mType].field_0x04[0] = data_list[item_p->mType].field_0x04[1];
                    data_list[item_p->mType].field_0x04[1] = temp_vec;
                }

                item_p->field_0xc = relationalPos(mpPlayerActor, data_list[item_p->mType].field_0x00);
                item_p->field_0x18 = data_list[item_p->mType].field_0x0c;
                item_p->field_0x4 = data_list[item_p->mType].field_0x10;

                if (lineBGCheck(&att_pos, &item_p->field_0xc, &sp424, 0x40b7)) {
                    item_p->field_0xc = sp424 + mViewCache.mDirection.Norm();
                }

                for (idx = 0; idx < data_list[item_p->mType].field_0x08; idx++) {
                    sp408 = relationalPos(mpPlayerActor, &data_list[item_p->mType].field_0x04[idx]);
                    if (sp408.y < mBG.field_0x108.field_0x4 + positionOf(mpPlayerActor).y) {
                        sp408.y = mBG.field_0x108.field_0x4 + positionOf(mpPlayerActor).y;
                    }

                    fopAc_ac_c* mActor = NULL;
                    switch (item_p->mType) {
                        case 1:
                        case 10: {
                            if (daPy_getPlayerActorClass()->checkHorseRide() != 0) {
                                mActor = (fopAc_ac_c*)dComIfGp_getHorseActor();
                            } else if (daPy_getPlayerActorClass()->checkCanoeRide() != 0) {
                                mActor = fopAcM_SearchByName(0xed);
                            }
                        }
                    }

                    if (!lineBGCheck(&item_p->field_0xc, &sp408, 0x4007) && !lineBGCheck(&att_pos, &sp408, 0x4007)
                        && !lineCollisionCheck(item_p->field_0xc, sp408, mpPlayerActor, mActor, NULL)) {
                        break;
                    }
                }

                if (idx >= data_list[item_p->mType].field_0x08) {
                    item_p->field_0xc = item_p->field_0x44;
                    item_p->field_0x18 = item_p->field_0x5c;
                    sp408 = item_p->field_0x50;
                }

                item_p->field_0x24.Val(sp408 - item_p->field_0xc);
                item_p->field_0x0 = 1;
            }
            break;
        }

        case 1: {
            f32 fVar1 = item_p->field_0x20 / f32(item_p->field_0x4);
            mViewCache.mFovy += fVar1 * (item_p->field_0x18 - mViewCache.mFovy);
            mViewCache.mCenter += (item_p->field_0xc - mViewCache.mCenter) * fVar1;

            f32 fVar2 = mViewCache.mDirection.R();
            acStack_540 = mViewCache.mDirection.V();
            acStack_53c = mViewCache.mDirection.U();
            fVar2 = fVar2 + (fVar1 * (item_p->field_0x24.R() - fVar2));
            acStack_540 += (item_p->field_0x24.V() - acStack_540) * fVar1;
            acStack_53c += (item_p->field_0x24.U() - acStack_53c) * fVar1;

            mViewCache.mDirection.Val(fVar2, acStack_540, acStack_53c);
            mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();

            if (item_p->field_0x20 < item_p->field_0x4) {
                break;
            }

            item_p->field_0x0 = 2;
        }
        // fallthrough
        case 2: {
            mViewCache.mCenter = item_p->field_0xc;
            mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();

            item_p->field_0x1c++;
            if (item_p->field_0x1c == 1) {
                if (item_p->mType == 0) {
                    item_p->field_0x0 = 11;
                    item_p->mType = 4;
                    break;
                }

                if (item_p->mType == 7) {
                    item_p->field_0x0 = 11;
                    item_p->mType = 8;
                    break;
                }

                if (item_p->mType == 11) {
                    item_p->field_0x0 = 999;
                    break;
                }
            }

            item_p->field_0x0 = 99;
        }
        // fallthrough
        case 99: {
            mStyleSettle.mFinished = true;
            mViewCache.mCenter = item_p->field_0xc;
            mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
        }

        case 999: {
            ret = true;
        }
    }

    item_p->field_0x20++;
    return ret;
}

bool dCamera_c::useItem1EvCamera() {
    ItemData* item_p = (ItemData*)mWork;
    if (mCurCamStyleTimer == 0) {
        getEvIntData(&item_p->mType, "Type", 0);
        if (item_p->mType == 0) {
            item_p->field_0x2c = mViewCache.mCenter - attentionPos(mpPlayerActor);
            item_p->field_0x38 = mViewCache.mEye - attentionPos(mpPlayerActor);
        }
    }

    if (item_p->mType == 0) {
        f32 mult_val = 0.05f;
        item_p->field_0x2c.y += (20.0f - item_p->field_0x2c.y) * mult_val;
        item_p->field_0x2c.x += -item_p->field_0x2c.x * mult_val;
        item_p->field_0x2c.z += -item_p->field_0x2c.z * mult_val;

        f32 r_val = mViewCache.mDirection.R();
        mViewCache.mCenter = attentionPos(mpPlayerActor) + item_p->field_0x2c;
        mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
        mViewCache.mDirection.R(r_val);

        if (mViewCache.mDirection.V() > cSAngle(70.0f)) {
            mViewCache.mDirection.V(cSAngle(70.0f));
        }

        if (mViewCache.mDirection.V() < cSAngle(0.0f)) {
            mViewCache.mDirection.V(cSAngle(0.0f));
        }

        mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
        mViewCache.mFovy += (20.0f - mViewCache.mFovy) * mult_val;
        return 1;
    }

    cXyz spec(0.0f, 0.0f, 0.0f);
    cXyz spf8(0.0f, -70.0f, 70.0f);
    cXyz sp104(0.0f, -30.0f, 70.0f);
    cXyz* sp264 = &spec;
    cXyz vec_1b0;

    daPy_py_c* mPlayer = daPy_getPlayerActorClass();
    daMidna_c* mMidna = (daMidna_c*)daPy_py_c::getMidnaActor();
    if (mIsWolf == 1) {
        if (mPlayer->checkMidnaRide() && mMidna->checkNoDraw()) {
            sp264 = &sp104;
        } else {
            sp264 = &spf8;
        }
    }

    cXyz vec_1a4(10.0f, -60.0f, 0.0f);
    cXyz vecs_218[6] = {
        cXyz(90.0f, 20.0f, 180.0f),
        cXyz(-100.0f, 50.0f, -180.0f),
        cXyz(60.0f, 150.0f, 165.0f),
        cXyz(-150.0f, 30.0f, 170.0f),
        cXyz(0.0f, -20.0f, 165.0f),
        cXyz(40.0f, 150.0f, -85.0f)
    };
    int const_6_val = 6;

    struct UseItem0Data {
        cXyz* field_0x00;
        cXyz* field_0x04;
        int field_0x08;
        f32 field_0x0c;
        int field_0x10;
        int field_0x14;
        int field_0x18;
    };
    UseItem0Data data_list[1][2] = {
        {
            {&vec_1a4, vecs_218, const_6_val, 65.0f,  1, 1,  7},
            {    NULL,     NULL,          -1, 30.0f, 80, 0, -1}
        }
    };

    cSAngle acStack_288;
    cSAngle acStack_28c;
    cXyz vec_198;
    bool rv = false;

    if (mCurCamStyleTimer == 0) {
        item_p->field_0x0 = 0;
        item_p->field_0x44 = mCenter;
        item_p->field_0x50 = mEye;
        item_p->field_0x5c = mFovy;
    }

    cXyz sp134 = attentionPos(mpPlayerActor);
    cXyz sp140;
    bool unused_yet_set_bool = false;
    switch (item_p->field_0x0) {
        case 0: {
            item_p->field_0x1c = 0;
            item_p->field_0x0 = 1;
        }
        // fallthrough
        case 1: {
            item_p->field_0x18 = data_list[item_p->mType][item_p->field_0x1c].field_0x0c;
            item_p->field_0xc = mViewCache.mCenter;
            if (data_list[item_p->mType][item_p->field_0x1c].field_0x00 != NULL) {
                vec_1b0 = data_list[item_p->mType][item_p->field_0x1c].field_0x00[0] + *sp264;
                item_p->field_0xc = relationalPos(mpPlayerActor, &vec_1b0);

                if ((mFrameCounter & data_list[item_p->mType][item_p->field_0x1c].field_0x18) == 0) {
                    cXyz cStack_14c = data_list[item_p->mType][item_p->field_0x1c].field_0x04[0];
                    data_list[item_p->mType][item_p->field_0x1c].field_0x04[0]
                        = data_list[item_p->mType][item_p->field_0x1c].field_0x04[1];
                    data_list[item_p->mType][item_p->field_0x1c].field_0x04[1] = cStack_14c;
                }

                if (lineBGCheck(&sp134, &item_p->field_0xc, &sp140, 0x40b7)) {
                    item_p->field_0xc = sp140 + mViewCache.mDirection.Norm();
                }
            }

            vec_198 = mViewCache.mEye;
            if (data_list[item_p->mType][item_p->field_0x1c].field_0x04 != NULL) {
                int i;
                for (i = 0; i < data_list[item_p->mType][item_p->field_0x1c].field_0x08; i++) {
                    vec_198 = relationalPos(mpPlayerActor, &data_list[item_p->mType][item_p->field_0x1c].field_0x04[i]);
                    fopAc_ac_c* mHorseActor = (fopAc_ac_c*)dComIfGp_getHorseActor();
                    if (!lineBGCheck(&item_p->field_0xc, &vec_198, 0x4007) && !lineBGCheck(&sp134, &vec_198, 0x4007)
                        && !lineCollisionCheck(item_p->field_0xc, vec_198, mpPlayerActor, mHorseActor, NULL)) {
                        break;
                    }
                }

                if (i >= data_list[item_p->mType][item_p->field_0x1c].field_0x08) {
                    vec_198 = mViewCache.mEye;
                }
            }

            item_p->field_0x24.Val(vec_198 - item_p->field_0xc);
            item_p->field_0x0 = 2;
            item_p->field_0x4 = data_list[item_p->mType][item_p->field_0x1c].field_0x10;
            item_p->field_0x20 = 1;
            unused_yet_set_bool = true;
            (void) unused_yet_set_bool;
        }
        // fallthrough intentional.
        case 2: {
            if (item_p->field_0x4 > 1) {
                f32 fVar1 = item_p->field_0x20 / f32(item_p->field_0x4);
                mViewCache.mFovy += fVar1 * (item_p->field_0x18 - mViewCache.mFovy);
                mViewCache.mCenter += (item_p->field_0xc - mViewCache.mCenter) * fVar1;

                f32 fVar2 = mViewCache.mDirection.R();
                acStack_28c = mViewCache.mDirection.V();
                acStack_288 = mViewCache.mDirection.U();
                fVar2 = fVar2 + (fVar1 * (item_p->field_0x24.R() - fVar2));
                acStack_28c += (item_p->field_0x24.V() - acStack_28c) * fVar1;
                acStack_288 += (item_p->field_0x24.U() - acStack_288) * fVar1;
                mViewCache.mDirection.Val(fVar2, acStack_28c, acStack_288);
                mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();

                if (item_p->field_0x20 < item_p->field_0x4) break;
            }

            item_p->field_0x0 = 3;
            item_p->field_0x4 = data_list[item_p->mType][item_p->field_0x1c].field_0x14;
            item_p->field_0x20 = 1;
        }
        // fallthrough
        case 3: {
            mViewCache.mCenter = item_p->field_0xc;
            mViewCache.mEye = mViewCache.mCenter + item_p->field_0x24.Xyz();
            mViewCache.mDirection = item_p->field_0x24;
            mViewCache.mFovy = item_p->field_0x18;

            if (item_p->field_0x20 < item_p->field_0x4) break;

            if (data_list[item_p->mType][1].field_0x08 != NULL && item_p->field_0x1c == 0) {
                item_p->field_0x1c = 1;
                item_p->field_0x0 = 1;
                break;
            }

            item_p->field_0x0 = 99;
        }
        // fallthrough
        case 99:
        default: {
            mStyleSettle.mFinished = true;
            rv = true;
        }
    }

    item_p->field_0x20++;
    return rv;
}

bool dCamera_c::getItemEvCamera() {
    GetItemData* gitem_p = (GetItemData*)mWork;
    cXyz cStack_90[8] = {
        cXyz(0.0f, -27.0f, 32.0f),
        cXyz(84.0f, -18.0f, 134.0f),
        cXyz(17.0f, 10.0f, -45.0f),
        cXyz(-45.0f, 128.0f, 45.0f),
        cXyz(0.0f, -27.0f, -62.0f),
        cXyz(-84.0f, -18.0f, -164.0f),
        cXyz(-12.0f, 10.0f, -130.0f),
        cXyz(50.0f, 128.0f, -220.0f),
    };

    struct GetItemData {
        cXyz* field_0x00;
        cXyz* field_0x04;
        f32 field_0x08;
        int field_0x0c;
        int field_0x10;
    };
    GetItemData data_list[5] = {
        {&cStack_90[0], &cStack_90[1], 50.0f, 12, 1},
        {&cStack_90[2], &cStack_90[3], 50.0f, 12, 0},
        {&cStack_90[4], &cStack_90[5], 50.0f, 17, -1},
        {&cStack_90[4], &cStack_90[5], 50.0f, 17, -1},
        {&cStack_90[6], &cStack_90[7], 50.0f, 17, 0},
    };

    f32 my_floats[6] = { 0.0f, 0.0f, 0.1f, 0.7f, 1.0f, 1.0f };
    int const_6_val = 6;

    if (mCurCamStyleTimer == 0) {
        cXyz vec_134;
        getEvIntData(&gitem_p->field_0x8, "Type", mIsWolf != 1 ? 0 : 1);

        if (gitem_p->field_0x8 == 2) {
            if (mIsWolf != 1) {
                gitem_p->field_0x8 = 3;
            } else {
                gitem_p->field_0x8 = 4;
            }

            OS_REPORT("camera: event:                   => %d\n", gitem_p->field_0x8);
        }

        getEvIntData(&gitem_p->mTimer, "Timer", data_list[gitem_p->field_0x8].field_0x0c);
        if ((gitem_p->field_0x18 = getEvFloatListData(&gitem_p->mCurvePoints, "CurvePoints")) == 0) {
            gitem_p->mCurvePoints = my_floats;
            gitem_p->field_0x18 = const_6_val;
        }

        getEvXyzData(&gitem_p->mStartCenter, "StartCenter", mCenter);
        getEvXyzData(&gitem_p->mStartEye, "StartEye", mEye);

        gitem_p->mRelActor = getEvActor("RelActor", "@PLAYER");
        gitem_p->mRelActorID = fopAcM_GetID(gitem_p->mRelActor);

        getEvXyzData(&vec_134, "Center", *data_list[gitem_p->field_0x8].field_0x00);
        gitem_p->field_0x44 = relationalPos(gitem_p->mRelActor, &vec_134);

        if (data_list[gitem_p->field_0x8].field_0x10 == 1) {
            cSGlobe cStack_208(mEye - positionOf(gitem_p->mRelActor));
            cSAngle acStack_224(cStack_208.U() - directionOf(gitem_p->mRelActor));
            if (acStack_224 < cSAngle::_0) {
                data_list[gitem_p->field_0x8].field_0x04->x = -data_list[gitem_p->field_0x8].field_0x04->x;
            }

            vec_134 = relationalPos(gitem_p->mRelActor, data_list[gitem_p->field_0x8].field_0x04);
            if (lineBGCheck(&gitem_p->field_0x44, &vec_134, 0x4007)) {
                data_list[gitem_p->field_0x8].field_0x04->x = -data_list[gitem_p->field_0x8].field_0x04->x;
            }
        } else if (data_list[gitem_p->field_0x8].field_0x10 == -1) {
            cSGlobe cStack_210(mEye - positionOf(gitem_p->mRelActor));
            cSAngle acStack_228(cStack_210.U() - directionOf(gitem_p->mRelActor));
            if (acStack_228 > cSAngle::_0) {
                data_list[gitem_p->field_0x8].field_0x04->x = -data_list[gitem_p->field_0x8].field_0x04->x;
            }

            vec_134 = relationalPos(gitem_p->mRelActor, data_list[gitem_p->field_0x8].field_0x04);
            if (lineBGCheck(&gitem_p->field_0x44, &vec_134, 0x4007)) {
                data_list[gitem_p->field_0x8].field_0x04->x = -data_list[gitem_p->field_0x8].field_0x04->x;
            }
        }

        getEvXyzData(&vec_134, "Eye", *data_list[gitem_p->field_0x8].field_0x04);
        gitem_p->field_0x50 = relationalPos(gitem_p->mRelActor, &vec_134);
        gitem_p->field_0x3c.Val(gitem_p->mStartEye - gitem_p->mStartCenter);
        gitem_p->field_0x64.Val(gitem_p->field_0x50 - gitem_p->field_0x44);

        mEventData.field_0xf0.Init(gitem_p->field_0x18, gitem_p->mTimer + 1);
        mEventData.field_0xf0.Step();
    }

    if (gitem_p->mRelActor && fopAcM_SearchByID(gitem_p->mRelActorID) == NULL) {
        OS_REPORT("camera: event: error: relational base actor dead\n");
        return 1;
    }

    if (mCurCamStyleTimer < gitem_p->mTimer) {
        mEventData.field_0xf0.Step();
        f32 fVar1 = mEventData.field_0xf0.Calc(gitem_p->mCurvePoints);

        mViewCache.mCenter = gitem_p->mStartCenter + ((gitem_p->field_0x44 - gitem_p->mStartCenter) * fVar1);
        mViewCache.mDirection.R(gitem_p->field_0x3c.R() + (fVar1 * (gitem_p->field_0x64.R() - gitem_p->field_0x3c.R())));
        mViewCache.mDirection.V(gitem_p->field_0x3c.V() + ((gitem_p->field_0x64.V() - gitem_p->field_0x3c.V()) * fVar1));
        mViewCache.mDirection.U(gitem_p->field_0x3c.U() + ((gitem_p->field_0x64.U() - gitem_p->field_0x3c.U()) * fVar1));
        mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
        mViewCache.mFovy += fVar1 * (data_list[gitem_p->field_0x8].field_0x08 - mViewCache.mFovy);

        return 0;
    }

    return 1;
}

namespace {
    inline static int get_camera_id(camera_class* i_camera) {
        return fopCamM_GetParam(i_camera);
    }

    inline static dDlst_window_c* get_window(camera_class* i_camera) {
        return dComIfGp_getWindow(dComIfGp_getCameraWinID(get_camera_id(i_camera)));
    }
}

bool dCamera_c::possessedEvCamera() {
    PossessedData* possessed = (PossessedData*)mWork;
    cXyz vec_d8;

    bool rv = 0;
    if (mCurCamStyleTimer == 0) {
        possessed->field_0x0 = 0;
    }

    switch (possessed->field_0x0) {
        case 0:
        default: {
            if (!(possessed->mTargetActor = getEvActor("Target", "@PLAYER"))) {
                return 1;
            }

            getEvIntData(&possessed->mType, "Type", 0);
            getEvIntData(&possessed->mTimer, "Timer", 10);
            getEvFloatData(&possessed->mRadius, "Radius", 60.0f);
            getEvFloatData(&possessed->mCushion, "Cushion", 1.0f);

            f32 fVar1;
            getEvFloatData(&fVar1, "Latitude", -5.0f);
            possessed->mLatitude.Val(fVar1);
            getEvFloatData(&fVar1, "Longitude", 0.0f);
            possessed->mLongitude.Val(fVar1);

            getEvFloatData(&possessed->mFovy, "Fovy", 45.0f);
            getEvIntData(&possessed->mBlure, "Blure", 0);

            if (possessed->mType == 0) {
                possessed->field_0x28.Val(possessed->mRadius, possessed->mLatitude, possessed->mLongitude + directionOf(possessed->mTargetActor));
                pushInfo(&mSavedViewStack[1], 2);
                mSavedViewStack[1].field_0x1e = 2;
            } else {
                mViewCache.mCenter = eyePos(possessed->mTargetActor);
                possessed->field_0x28.Val(mSavedViewStack[1].mEye - mSavedViewStack[1].mCenter);
                mViewCache.mDirection.Val(possessed->mRadius, possessed->mLatitude, possessed->mLongitude + directionOf(possessed->mTargetActor));
                mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
                mViewCache.mFovy = possessed->mFovy;
            }

            possessed->field_0x0 = 1;
            possessed->field_0xc = possessed->mTimer;
            switch (possessed->mBlure) {
                case 1:
                    StartBlure(possessed->mTimer, possessed->mTargetActor, 0.5f, 0.99f);
                    break;

                case 2:
                    StartBlure(possessed->mTimer, possessed->mTargetActor, 0.63f, 0.99f);
            }

            break;
        }

        case 1: {
            f32 fVar1 = 1.0f / possessed->field_0xc;
            mViewCache.mCenter += ((eyePos(possessed->mTargetActor) - mViewCache.mCenter) * fVar1);
            mViewCache.mDirection.R(mViewCache.mDirection.R() + (fVar1 * (possessed->field_0x28.R() - mViewCache.mDirection.R())));
            mViewCache.mDirection.V(mViewCache.mDirection.V() + ((possessed->field_0x28.V() - mViewCache.mDirection.V()) * fVar1));
            mViewCache.mDirection.U(mViewCache.mDirection.U() + ((possessed->field_0x28.U() - mViewCache.mDirection.U()) * fVar1));
            vec_d8 = mViewCache.mCenter + mViewCache.mDirection.Xyz();
            mViewCache.mEye += (vec_d8 - mViewCache.mEye) * possessed->mCushion;
            mViewCache.mFovy += fVar1 * (possessed->mFovy - mViewCache.mFovy);

            switch (possessed->mBlure) {
                default: {
                    break;
                }

                case 1: {
                    scissor_class* scissor = get_window(field_0x0)->getScissor();
                    cXyz mEyePos = eyePos(possessed->mTargetActor);
                    cXyz sp60;
                    mDoLib_project(&mEyePos, &sp60);
                    SetBlurePosition(sp60.x / scissor->width, sp60.y / scissor->height, 0.0f);
                    SetBlureAlpha((fVar1 * 0.699999988079071f) + 0.5f);
                    SetBlureScale((fVar1 * 0.09000000357627869f) + 1.1f, 0.98f - (fVar1 * 0.18000000715255737f), 0.0f);
                }
            }

            possessed->field_0xc--;
            if (possessed->field_0xc <= 0) {
                possessed->field_0x0 = 99;
            }
            break;
        }

        case 99: {
            rv = 1;
            mStyleSettle.mFinished = true;
        }
    }

    return rv;
}

bool dCamera_c::fixedFramesEvCamera() {
    int const_1_val = 1;
    FixedFramesData* fframes_p = (FixedFramesData*)mWork;
    if (mCurCamStyleTimer == 0) {
        cXyz sp30, sp3c;
        fframes_p->field_0x38 = 9999;

        int substanceNum;
        char* key = "Centers";
        if ((substanceNum = dComIfGp_evmng_getMySubstanceNum(mEventData.mStaffIdx, key)) != 0) {
            fframes_p->field_0x1c[1] = dComIfGp_evmng_getMyXyzP(mEventData.mStaffIdx, key);
            if (fframes_p->field_0x38 > substanceNum) {
                fframes_p->field_0x38 = substanceNum;
            }
        } else {
            OS_REPORT("camera: event: error: %s (xyz*) not found\n", key);
            return 1;
        }

        key = "Eyes";
        if ((substanceNum = dComIfGp_evmng_getMySubstanceNum(mEventData.mStaffIdx, key)) != 0) {
            fframes_p->field_0x1c[0] = dComIfGp_evmng_getMyXyzP(mEventData.mStaffIdx, key);
            if (fframes_p->field_0x38 > substanceNum) {
                fframes_p->field_0x38 = substanceNum;
            }
        } else {
            OS_REPORT("camera: event: error: %s (xyz*) not found\n", key);
            return 1;
        }

        key = "Fovys";
        if ((substanceNum = dComIfGp_evmng_getMySubstanceNum(mEventData.mStaffIdx, key)) != 0) {
            fframes_p->field_0x24 = dComIfGp_evmng_getMyFloatP(mEventData.mStaffIdx, key);
            if (fframes_p->field_0x38 > substanceNum) {
                fframes_p->field_0x38 = substanceNum;
            }
        } else {
            OS_REPORT("camera: event: error: %s (float*) not found\n", key);
            return 1;
        }


        fframes_p->field_0x0 = getEvIntData(&fframes_p->mTimer, "Timer", const_1_val);
        getEvStringData(&fframes_p->mRelUseMask, "RelUseMask", "oo");
#if DEBUG
        if (strlen(&fframes_p->mRelUseMask) != 2) {
            OSReport("camera: event:                   bad length -> xx\n");
            strcpy(&fframes_p->mRelUseMask, "xx");
            JUTAssertion::showAssert(JUTAssertion::getSDevice(), "d_ev_camera.cpp", 0x129c, "Halt");
            OSPanic("d_ev_camera.cpp", 0x129c, "Halt");
        }
#endif

        fframes_p->mRelActor = getEvActor("RelActor");

        int iVar1 = 0, i = 0;
        getEvIntData(&fframes_p->mChoice, "Choice", 0);
        if (fframes_p->mChoice == 1) {
            iVar1 = fabsf(cM_rndFX(fframes_p->field_0x38 - 0.01f));
        }

        for (i = 0; i < fframes_p->field_0x38; i++) {
            sp30 = fframes_p->field_0x1c[1][iVar1];
            sp3c = fframes_p->field_0x1c[0][iVar1];

            if (fframes_p->mRelActor && fframes_p->mRelUseMask == 111) {
                fframes_p->field_0x4 = relationalPos(fframes_p->mRelActor, &sp30);
            } else {
                fframes_p->field_0x4 = sp30;
            }

            if (fframes_p->mRelActor && fframes_p->field_0x31 == 111) {
                fframes_p->field_0x10 = relationalPos(fframes_p->mRelActor, &sp3c);
            } else {
                fframes_p->field_0x10 = sp3c;
            }

            fframes_p->field_0x28 = fframes_p->field_0x24[iVar1];

            if (!lineBGCheck(&fframes_p->field_0x4, &fframes_p->field_0x10, 0x4007)
                && !lineCollisionCheck(fframes_p->field_0x4, fframes_p->field_0x10,
                                       mpPlayerActor, fframes_p->mRelActor, NULL)) {
                break;
            }

            iVar1++;

            if (iVar1 >= fframes_p->field_0x38) {
                iVar1 = 0;
            }
        }

        mStyleSettle.mFinished = true;
    }

    mViewCache.mCenter = fframes_p->field_0x4;
    mViewCache.mEye = fframes_p->field_0x10;
    mViewCache.mFovy = fframes_p->field_0x28;
    mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
    if (fframes_p->field_0x0 && mCurCamStyleTimer < fframes_p->mTimer) {
        return 0;
    }

    return 1;
}

bool dCamera_c::bSplineEvCamera() {
    BSplineData* bSpline = (BSplineData*)mWork;
    if (mCurCamStyleTimer == 0) {
        bSpline->field_0x1c = 0;
        bSpline->field_0x10 = 9999;
        char* key = "Centers";
        int iVar1 = getEvXyzListData(&bSpline->mCenters, key);
        if (iVar1 != 0) {
            if (bSpline->field_0x10 > iVar1) {
                bSpline->field_0x10 = iVar1;
            }
        } else {
            return 1;
        }

        key = "Eyes";
        iVar1 = getEvXyzListData(&bSpline->mEyes, key);
        if (iVar1 != 0) {
            if (bSpline->field_0x10 > iVar1) {
                bSpline->field_0x10 = iVar1;
            }
        } else {
            return 1;
        }

        key = "Fovys";
        iVar1 = getEvFloatListData(&bSpline->mFovys, key);
        if (iVar1 != 0) {
            if (bSpline->field_0x10 > iVar1) {
                bSpline->field_0x10 = iVar1;
            }
        } else {
            return 1;
        }

        if (getEvIntData(&bSpline->mTimer, "Timer") == 0) {
            OS_REPORT("camera: event: error: NO Timer\n");
            return 1;
        }

        bSpline->mRelActor = getEvActor("RelActor");
        mEventData.field_0xf0.Init(bSpline->field_0x10, bSpline->mTimer);
        mStyleSettle.mFinished = true;
    }

    if (bSpline->field_0x1c == 0 && mEventData.field_0xf0.Step()) {
        cXyz tmpcenters = mEventData.field_0xf0.Calc(bSpline->mCenters);
        cXyz tmpeyes = mEventData.field_0xf0.Calc(bSpline->mEyes);

        if (bSpline->mRelActor != NULL) {
            mViewCache.mCenter = relationalPos(bSpline->mRelActor, &tmpcenters);
            mViewCache.mEye = relationalPos(bSpline->mRelActor, &tmpeyes);
        } else {
            mViewCache.mCenter = tmpcenters;
            mViewCache.mEye = tmpeyes;
        }

        mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
        mViewCache.mFovy = mEventData.field_0xf0.Calc(bSpline->mFovys);
        if (mEventData.field_0xf0.Last()) {
            bSpline->field_0x1c = 1;
        }
    }

    return bSpline->field_0x1c;
}

bool dCamera_c::twoActor0EvCamera() {
    static cXyz DefaultGap(0.0f, 0.0f, 0.0f);
    TwoActor0Data* twoact0_p = (TwoActor0Data*)mWork;

    if (mCurCamStyleTimer == 0) {
        twoact0_p->mActor1 = getEvActor("Actor1", "@PLAYER");
        twoact0_p->mActor2 = getEvActor("Actor2", "@STARTER");

        if (twoact0_p->mActor1 == NULL || twoact0_p->mActor2 == NULL) {
            OS_REPORT("camera: event: error: actor 1 or 2 missing\n");
            return 1;
        }

        twoact0_p->mActor1ID = fopAcM_GetID(twoact0_p->mActor1);
        twoact0_p->mActor2ID = fopAcM_GetID(twoact0_p->mActor2);

        getEvXyzData(&twoact0_p->mCtrGap, "CtrGap", DefaultGap);
        getEvFloatData(&twoact0_p->mCtrRatio, "CtrRatio", -0.1f);
        getEvFloatData(&twoact0_p->mCtrCus, "CtrCus", 1.0f);
        getEvFloatData(&twoact0_p->mEyeCus, "EyeCus", 1.0f);
        getEvFloatData(&twoact0_p->mRadiusMin, "RadiusMin", 100.0f);
        getEvFloatData(&twoact0_p->mRadiusMax, "RadiusMax", 10000.0f);
        getEvFloatData(&twoact0_p->mLatitudeMin, "LatitudeMin", -60.0f);
        getEvFloatData(&twoact0_p->mLatitudeMax, "LatitudeMax", 60.0f);
        getEvFloatData(&twoact0_p->mLongitudeMin, "LongitudeMin", 20.0f);
        getEvFloatData(&twoact0_p->mLongitudeMax, "LongitudeMax", 160.0f);

        getEvIntData(&twoact0_p->mLongitudeType, "LongitudeType", -1);
        getEvFloatData(&twoact0_p->mFovy, "Fovy", 60.0f);
        twoact0_p->field_0x4c = mViewCache.mDirection.R();
        twoact0_p->field_0x54 = cSAngle(mViewCache.mDirection.V().Degree());
        twoact0_p->field_0x56 = cSAngle(mViewCache.mDirection.U().Degree());
    }

    cSGlobe cStack_d8(attentionPos(twoact0_p->mActor1) - attentionPos(twoact0_p->mActor2));

    cXyz cStack_34;
    if (fopAcM_SearchByID(twoact0_p->mActor1ID) == NULL) {
        OS_REPORT("camera: event: error: actor1 dead\n");
        return 1;

    } else if (fopAcM_SearchByID(twoact0_p->mActor2ID) == NULL) {
        OS_REPORT("camera: event: error: actor2 dead\n");
        return 1;

    } else if ( !(twoact0_p->mCtrRatio >= 0.0f) || !(twoact0_p->mCtrRatio <= 1.0f) ) {
        cStack_34 = relationalPos(twoact0_p->mActor1, twoact0_p->mActor2, &twoact0_p->mCtrGap, 0.25f);

    } else {
        cStack_34 = (attentionPos(twoact0_p->mActor1) + 
                    attentionPos(twoact0_p->mActor2)) * twoact0_p->mCtrRatio;
    }

    mViewCache.mCenter += (cStack_34 - mViewCache.mCenter) * twoact0_p->mCtrCus;

    if (mCurCamStyleTimer == 0) {
        cSAngle acStack_e0;
        switch (twoact0_p->mLongitudeType) {
            case 1:
                acStack_e0 = cSAngle(directionOf(mpPlayerActor).Inv());
                break;

            case 2:
                acStack_e0 = directionOf(mpPlayerActor);
                break;

            default:
                acStack_e0 = mViewCache.mDirection.U();
        }

        cSAngle acStack_e4 = cStack_d8.U() - acStack_e0;
        if (acStack_e4.Abs() < cSAngle(twoact0_p->mLongitudeMin)) {
            if (acStack_e4 < cSAngle::_0) {
                twoact0_p->field_0x5a = cStack_d8.U() + cSAngle(twoact0_p->mLongitudeMin);
            } else {
                twoact0_p->field_0x5a = cStack_d8.U() - cSAngle(twoact0_p->mLongitudeMin);
            }
        } else if (acStack_e4.Abs() > cSAngle(twoact0_p->mLongitudeMax)) {
            if (acStack_e4 < cSAngle::_0) {
                twoact0_p->field_0x5a = cStack_d8.U() + cSAngle(twoact0_p->mLongitudeMax);
            } else {
                twoact0_p->field_0x5a = cStack_d8.U() - cSAngle(twoact0_p->mLongitudeMax);
            }
        } else {
            twoact0_p->field_0x5a = acStack_e0;
        }

        cSAngle acStack_e8(mViewCache.mDirection.V());
        if (acStack_e8 < cSAngle(twoact0_p->mLatitudeMin)) {
            twoact0_p->field_0x58 = cSAngle(twoact0_p->mLatitudeMin);
        } else {
            if (acStack_e8 > cSAngle(twoact0_p->mLatitudeMax)) {
                twoact0_p->field_0x58 = cSAngle(twoact0_p->mLatitudeMax);
            } else {
                twoact0_p->field_0x58 = mViewCache.mDirection.V();
            }
        }

        f32 fVar1 = mViewCache.mDirection.R();
        if (fVar1 < twoact0_p->mRadiusMin) {
            twoact0_p->field_0x50 = twoact0_p->mRadiusMin;
        } else if (fVar1 > twoact0_p->mRadiusMax) {
            twoact0_p->field_0x50 = twoact0_p->mRadiusMax;
        } else {
            twoact0_p->field_0x50 = mViewCache.mDirection.R();
        }
    }

    twoact0_p->field_0x56 += (twoact0_p->field_0x5a - twoact0_p->field_0x56) * twoact0_p->mEyeCus;
    twoact0_p->field_0x54 += (twoact0_p->field_0x58 - twoact0_p->field_0x54) * twoact0_p->mEyeCus;
    twoact0_p->field_0x4c += twoact0_p->mEyeCus * (twoact0_p->field_0x50 - twoact0_p->field_0x4c);
    mViewCache.mDirection.Val(twoact0_p->field_0x4c, twoact0_p->field_0x54, twoact0_p->field_0x56);
    mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
    mViewCache.mFovy += twoact0_p->mCtrCus * (twoact0_p->mFovy - mViewCache.mFovy);

    return 1;
}

bool dCamera_c::peepHoleEvCamera() {
    if (mCurCamStyleTimer == 0) {
        fopAc_ac_c* mPlayer = dComIfGp_getLinkPlayer();
        cXyz sp20 = positionOf(mPlayer);
        cXyz sp2c(sp20);
        mPlayer = dComIfGp_event_getPt2();
        if (mPlayer == NULL) {
            OS_REPORT("camera: event: error: partner actor not found\n");
        } else {
            cXyz sp38 = positionOf(mPlayer);
            if (dCamMath().xyzHorizontalDistance(sp20, sp38) > 50.0f) {
                sp38.y = sp20.y;
                cXyz cStack_2c = sp20 - sp38;
                cStack_2c .normalize();
                sp2c = sp38 + (cStack_2c * 50.0f);
            }
        }

        dComIfGp_getEventManager().setGoal(&sp2c);
    }

    return 1;
}

bool dCamera_c::digHoleEvCamera() {
    DigHoleData* digHole = (DigHoleData*)mWork;
    if (mCurCamStyleTimer == 0) {
        digHole->field_0x0 = 0;
        getEvIntData(&digHole->mType, "Type", 3);
        if (digHole->mType == 1) {
            mEventData.field_0x20 = 0;
        }

        digHole->field_0x8 = 60;
        digHole->field_0xc = 0.05f;
        digHole->field_0x10 = 1;
        if ((digHole->mType & 1) != 0) {
            cSAngle acStack_158(mViewCache.mDirection.U() - directionOf(mpPlayerActor));
            if (acStack_158.Abs() < s16(cSAngle::_90)) {
                cXyz sp70, sp7c;
                cSGlobe cStack_138;
                cXyz vec_f8(0.0f, -70.0f, -10.0f);
                sp70 = relationalPos(mpPlayerActor, &vec_f8);
                cStack_138.R(350.0f);
                cStack_138.V(cSAngle(30.0f));

                cStack_138.U(directionOf(mpPlayerActor).Inv()
                             + cSAngle( (acStack_158 < cSAngle::_0) ? 10.0f : -10.0f ));

                sp7c = sp70 + cStack_138.Xyz();

                if (!lineBGCheck(&sp70, &sp7c, 0x4007)) {
                    mViewCache.mFovy = 60.0f;
                    mViewCache.mCenter = sp70;
                    mViewCache.mDirection = cStack_138;
                    mViewCache.mEye = sp7c;
                } else {
                    digHole->field_0x10 = 0;
                }
            }
        }

        if ((digHole->mType & 2) != 0) {
            cXyz vec_98(0.0f, 0.0f, 245.0f);
            digHole->field_0x14 = relationalPos(mpPlayerActor, &vec_98);

            cXyz sp64[3] = { cXyz(-85.0f, -10.0f, 300.0f), cXyz(50.0f, 165.0f, 160.0f), cXyz(-260.0f, -50.0f, 30.0f)};
            int i;
            int sp64_size = sizeof(sp64) / sizeof(cXyz);
            fopAc_ac_c* unused_ptr = mpPlayerActor;
            (void) unused_ptr;
            fopAc_ac_c* mMidnaActor = (fopAc_ac_c*)daPy_py_c::getMidnaActor();
            cXyz spa0;
            for (i = 0; i < sp64_size; i++) {
                spa0 = dCamMath::xyzRotateY(sp64[i], directionOf(mpPlayerActor));

                digHole->field_0x20 = digHole->field_0x14 + spa0;

                if (!lineBGCheck(&digHole->field_0x14, &digHole->field_0x20, 0x4007)
                    && !lineCollisionCheck(digHole->field_0x14, digHole->field_0x20, mpPlayerActor, mMidnaActor, NULL)) {
                    break;
                }
            }
        }

        cXyz spac = attentionPos(mpPlayerActor);
        if (dCamMath::xyzHorizontalDistance(mViewCache.mCenter, spac) > 120.0f) {
            digHole->field_0x10 = 0;
        }
    }

    digHole->field_0x0++;

    switch (digHole->mType) {
        default:
        case 1: {
            if (digHole->field_0x10) {
                f32 fVar2 = mViewCache.mDirection.R();
                f32 fVar3 = mViewCache.mDirection.V().Degree();
                mViewCache.mDirection.R(fVar2 + (350.0f - fVar2) * digHole->field_0xc);
                mViewCache.mDirection.V(fVar3 + (40.0f - fVar3) * digHole->field_0xc);
                mViewCache.mFovy += (60.0f - mViewCache.mFovy) * digHole->field_0xc;
                mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
            } else if (!strcmp(dComIfGp_getStartStageName(), "R_SP01")) {
                mBumpCheckFlags = 0;
            }

            if (digHole->field_0x0 >= digHole->field_0x8  && digHole->mType != 1) {
                digHole->field_0x0 = 0;
                digHole->mType = 2;
            }

            break;
        }

        case 2: {
            if (digHole->field_0x0 == 1) {
                mViewCache.mCenter = digHole->field_0x14;
                mViewCache.mEye = digHole->field_0x20;
                mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
                mViewCache.mFovy = 60.0f;
            }
        }
    }

    return 1;
}

bool dCamera_c::hintTalkEvCamera() {
    HintTalkData* hintTalk = (HintTalkData*)mWork;
    daAlink_c* link_p = (daAlink_c*)mpPlayerActor;
    fopAc_ac_c* ride_actor_p;
    int i, j;

    if (mCurCamStyleTimer == 0) {
        hintTalk->mTeacher = getEvActor("Teacher", "Midna");
        hintTalk->mStudent = getEvActor("Student", "@PLAYER");
        getEvFloatData(&hintTalk->mFovy, "Fovy", 55.0f);
        getEvFloatData(&hintTalk->mRadius, "Radius", 280.0f);
        getEvFloatData(&hintTalk->mCtrGapY, "CtrGapY", -25.0f);
        getEvIntData(&hintTalk->mTimer, "Timer", 20);
    }

    if (hintTalk->mTeacher == NULL || hintTalk->mStudent == NULL) {
        OS_REPORT("camera: event: error: actor 1 or 2 missing\n");
        return 1;
    }

    if (mCurCamStyleTimer == 0) {
        cXyz vec_190 = attentionPos(hintTalk->mTeacher);
        cXyz vec_184 = attentionPos(hintTalk->mStudent);
        ride_actor_p = NULL;

        if (fopAcM_GetName(hintTalk->mTeacher) == 0xff) {
            vec_190 = *link_p->getShadowTalkAtnPos();
        }

        if (hintTalk->mStudent == mpPlayerActor && mIsWolf == 1) {
            cXyz vec_178 = positionOf(mpPlayerActor);
            vec_178.y = vec_184.y;
            vec_184 += cXyz(vec_178 - vec_184).norm() * 40.0f;
        }

        cXyz spe4;
        hintTalk->field_0x40 = vec_190;
        hintTalk->mDirection = mViewCache.mDirection;
        cXyz spf0 = vec_184 - vec_190;
        hintTalk->field_0x28.Val(spf0);

        if ((dComIfGp_getCameraAttentionStatus(0) & 0x800) != 0) {
            hintTalk->field_0x28.V(cSAngle(15.0f));
        }

        spe4 = vec_190 + (spf0 * 0.8f);
        spe4.y += hintTalk->mCtrGapY;
        hintTalk->mDirection.R(hintTalk->mRadius);

        f32 local_b0[10] = { 40.0f, 60.0f, 25.0f, 50.0f, 75.0f, 35.0f, 15.0f, 90.0f, 110.0f, 140.0f };
        f32 fVar1;
        if ((mFrameCounter & 16) != 0) {
            fVar1 = local_b0[0];
            local_b0[0] = local_b0[3];
            local_b0[3] = fVar1;
        }
        if ((mFrameCounter & 4) != 0) {
            fVar1 = local_b0[2];
            local_b0[2] = local_b0[1];
            local_b0[1] = fVar1;
        }
        if ((mFrameCounter & 2) != 0) {
            fVar1 = local_b0[0];
            local_b0[0] = local_b0[1];
            local_b0[1] = fVar1;
        }

        bool bVar9 = false;
        cSAngle acStack_1f8(hintTalk->field_0x28.U());
        f32 fVar22;
        if (link_p->checkRide() != 0) {
            ride_actor_p = link_p->getRideActor();
            acStack_1f8 = directionOf(mpPlayerActor);
            if ((hintTalk->field_0x28.U() - acStack_1f8) < cSAngle::_0) {
                fVar22 = -1.0f;
            } else {
                fVar22 = 1.0f;
            }
        } else if (hintTalk->field_0x28.U() - hintTalk->mDirection.U() > cSAngle::_0) {
            fVar22 = -1.0f;
        } else {
            fVar22 = 1.0f;
        }

        cSAngle acStack_1fc(20.0f);
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 12; j++) {
                cSAngle acStack_200(local_b0[j] * fVar22);
                hintTalk->mDirection.U(acStack_1f8 + acStack_200);
                hintTalk->mDirection.V(((hintTalk->field_0x28.V() * acStack_200.Cos()) * 0.2f) + acStack_1fc);
                hintTalk->field_0x8 = spe4 - ((spf0 * 0.2f) * acStack_200.Cos());
                hintTalk->field_0x14 = hintTalk->field_0x8 + hintTalk->mDirection.Xyz();

                if (!lineBGCheck(&vec_184, &hintTalk->field_0x14, 0x4007)
                    && !lineBGCheck(&vec_190, &hintTalk->field_0x14, 0x4007)
                    && !lineBGCheck(&hintTalk->field_0x8, &hintTalk->field_0x14, 0x4007)
                    && !lineCollisionCheck(vec_184, hintTalk->field_0x14, hintTalk->mStudent, hintTalk->mTeacher, ride_actor_p)
                    && !lineCollisionCheck(vec_190, hintTalk->field_0x14, hintTalk->mStudent, hintTalk->mTeacher, ride_actor_p)) {
                    bVar9 = true;
                    break;
                }

                if (j % 4 == 3) {
                    acStack_1fc += cSAngle((i != 0) ? -2.5f : 4.5f);
                }
            }

            if (bVar9) {
                break;
            }

            fVar22 = -fVar22;
            hintTalk->mDirection.R(hintTalk->mDirection.R() * 0.75f);
        }
    }

    if (!mStyleSettle.mFinished) {
        f32 div_val = f32(mCurCamStyleTimer + 1) / hintTalk->mTimer;
        f32 fVar22 = dCamMath::rationalBezierRatio(div_val, 0.4f);
        mViewCache.mCenter += (hintTalk->field_0x8 - mViewCache.mCenter) * fVar22;
        mViewCache.mDirection.R(mViewCache.mDirection.R() + ((hintTalk->mDirection.R() - mViewCache.mDirection.R()) * fVar22));
        mViewCache.mDirection.V(mViewCache.mDirection.V() + ((hintTalk->mDirection.V() - mViewCache.mDirection.V()) * fVar22));
        mViewCache.mDirection.U(mViewCache.mDirection.U() + ((hintTalk->mDirection.U() - mViewCache.mDirection.U()) * fVar22));
        mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
        mViewCache.mFovy += fVar22 * (hintTalk->mFovy - mViewCache.mFovy);

        if (mCurCamStyleTimer == (hintTalk->mTimer - 1)) {
            mStyleSettle.mFinished = true;
        }

        return 0;
    }

    return 1;
}

bool dCamera_c::bspTransEvCamera() {
    BspTransData* bspTrans = (BspTransData*)mWork;
    f32 local_7c[4] = { 0.0f, 0.0f, 1.0f, 1.0f };

    if (mCurCamStyleTimer == 0) {
        mStyleSettle.mFinished = true;
        if (!getEvIntData(&bspTrans->mTimer, "Timer")) {
            OS_REPORT("camera: event: error: BSPTRANS no Timer\n");
            return 1;
        }

        int iVar1 = getEvFloatListData(&bspTrans->mBase, "Base");
        if (iVar1 != 0) {
            mEventData.field_0xf0.Init(iVar1, bspTrans->mTimer + 1);
        } else {
            OS_REPORT("camera: event:                   use 0.0 0.0 1.0 1.0\n");
            mEventData.field_0xf0.Init(4, bspTrans->mTimer + 1);
            bspTrans->mBase = local_7c;
        }
        mEventData.field_0xf0.Step();

        bspTrans->mSet1 = 0;
        char use1[8];
        strcpy(use1, "xxxxxx");

        iVar1 = getEvFloatListData(&bspTrans->mSet1, "Set1");
        if (iVar1 != 0) {
            bspTrans->mSplinePath1.Init(iVar1, bspTrans->mTimer + 1);
            bspTrans->mSplinePath1.Step();
            getEvStringData(use1, "Use1", "ooxxxx");

#if DEBUG
            if (strlen(use1) != 6) {
                OSReport("camera: event:                   bad length -> xxxxxx\n");
                strcpy(use1, "xxxxxx");
                JUTAssertion::showAssert(JUTAssertion::getSDevice(), "d_ev_camera.cpp", 0x14f9, "0");
                OSPanic("d_ev_camera.cpp", 0x14f9, "Halt");
            }
#endif
        }

        bspTrans->mSet2 = 0;
        char use2[8];
        strcpy(use2, "xxxxxx");

        iVar1 = getEvFloatListData(&bspTrans->mSet2, "Set2");
        if (iVar1 != 0) {
            bspTrans->mSplinePath2.Init(iVar1, bspTrans->mTimer + 1);
            bspTrans->mSplinePath2.Step();
            getEvStringData(use2, "Use2", "xxooox");

#if DEBUG
            if (strlen(use2) != 6) {
                OSReport_Error("camera: event:                   bad length -> xxxxxx\n");
                strcpy(use2, "xxxxxx");
                JUTAssertion::showAssert(JUTAssertion::getSDevice(), "d_ev_camera.cpp", 0x1509, "0");
                OSPanic("d_ev_camera.cpp", 0x1509, "Halt");
            }
#endif
        }

        bspTrans->mRelActorID = -1;
        bspTrans->mRelActor = getEvActor("RelActor");
        if (bspTrans->mRelActor) {
            getEvStringData(&bspTrans->mRelUseMask, "RelUseMask", "oo");

#if DEBUG
            if (strlen(&bspTrans->mRelUseMask) != 2) {
                OSReport_Error("camera: event:                   bad length -> xx\n");
                strcpy(&bspTrans->mRelUseMask, "xx");
                JUTAssertion::showAssert(JUTAssertion::getSDevice(), "d_ev_camera.cpp", 0x1515, "0");
                OSPanic("d_ev_camera.cpp", 0x1515, "Halt");
            }
#endif

            bspTrans->mRelActorID = fopAcM_GetID(bspTrans->mRelActor);
        }

        getEvXyzData(&bspTrans->field_0x94, "Center", mCenter);
        getEvXyzData(&bspTrans->field_0xa0, "Eye", mEye);
        getEvFloatData(&bspTrans->field_0xac, "Fovy", mFovy);
        getEvFloatData(&bspTrans->mCushion, "Cushion", 1.0f);

        for (int i = 0; i < 6; i++) {
            bspTrans->field_0xc0[i] = isRelChar(use1[i]) ? isRelChar(use2[i]) ? 2 : 1 : 0;
        }

        bspTrans->field_0x78 = mCenter;
        bspTrans->field_0x84 = mEye;
        bspTrans->field_0x90 = mFovy;
    }

    if (bspTrans->mRelActor != NULL && fopAcM_SearchByID(bspTrans->mRelActorID) == NULL) {
        OS_REPORT("camera: event: error: relational base actor dead\n");
        return 1;
    }

    cXyz sp60, sp70;
    if (mCurCamStyleTimer < bspTrans->mTimer) {
        struct {
            cXyz sp48;
            cXyz sp3c;
        } pos;

        if (bspTrans->mRelActor != NULL) {
            if (isRelChar(bspTrans->mRelUseMask)) {
                pos.sp48 = relationalPos(bspTrans->mRelActor, &bspTrans->field_0x94);
            } else {
                pos.sp48 = bspTrans->field_0x94;
            }

            if (isRelChar(bspTrans->field_0xb9)) {
                pos.sp3c = relationalPos(bspTrans->mRelActor, &bspTrans->field_0xa0);
            } else {
                pos.sp3c = bspTrans->field_0xa0;
            }
        } else {
            pos.sp48 = bspTrans->field_0x94;
            pos.sp3c = bspTrans->field_0xa0;
        }

        mEventData.field_0xf0.Step();
        f32 vals_f4[3];
        vals_f4[0] = mEventData.field_0xf0.Calc(bspTrans->mBase);
        if (bspTrans->mSet1 != 0) {
            bspTrans->mSplinePath1.Step();
            vals_f4[1] = bspTrans->mSplinePath1.Calc(bspTrans->mSet1);
        }

        if (bspTrans->mSet2 != 0) {
            bspTrans->mSplinePath2.Step();
            vals_f4[2] = bspTrans->mSplinePath2.Calc(bspTrans->mSet2);
        }

        sp60.x = bspTrans->field_0x78.x + (pos.sp48.x - bspTrans->field_0x78.x) * vals_f4[bspTrans->field_0xc0[1]];
        sp60.y = bspTrans->field_0x78.y + (pos.sp48.y - bspTrans->field_0x78.y) * vals_f4[bspTrans->field_0xc0[0]];
        sp60.z = bspTrans->field_0x78.z + (pos.sp48.z - bspTrans->field_0x78.z) * vals_f4[bspTrans->field_0xc0[1]];

        mViewCache.mCenter += (sp60 - mViewCache.mCenter) * bspTrans->mCushion;

        cSGlobe cStack_130(bspTrans->field_0x84 - bspTrans->field_0x78);
        cSGlobe cStack_138(pos.sp3c - pos.sp48);
        cSGlobe cStack_140;

        cStack_140.R(cStack_130.R() + ((cStack_138.R() - cStack_130.R()) * vals_f4[bspTrans->field_0xc0[2]]));
        cStack_140.V(cStack_130.V() + ((cStack_138.V() - cStack_130.V()) * vals_f4[bspTrans->field_0xc0[3]]));
        cStack_140.U(cStack_130.U() + ((cStack_138.U() - cStack_130.U()) * vals_f4[bspTrans->field_0xc0[4]]));

        sp70 = mViewCache.mCenter + cStack_140.Xyz();
        mViewCache.mEye += (sp70 - mViewCache.mEye) * bspTrans->mCushion;
        f32 add_val = (bspTrans->field_0x90 + (bspTrans->field_0xac - bspTrans->field_0x90) * vals_f4[bspTrans->field_0xc0[5]]);
        mViewCache.mFovy += bspTrans->mCushion * (add_val - mViewCache.mFovy);
        mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
        return 0;
    }

    return 1;
}

bool dCamera_c::portalWarpEvCamera() {
    return true;
}
